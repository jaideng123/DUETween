// Fill out your copyright notice in the Description page of Project Settings.


#include "DUETweenSubsystem.h"

#include "DUETweenInternalUtils.h"
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Active Tweens"), STAT_ACTIVE_TWEENS, STATGROUP_DUETween);


void UDUETweenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Pool.InitTweenPool();
	ActiveTweenChainStart = NULL_DUETWEEN_HANDLE;
	LastAssignedTweenId = 0;
}

void UDUETweenSubsystem::Deinitialize()
{
	Pool.Deinitialize();
	Super::Deinitialize();
}

bool UDUETweenSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || WorldType == EWorldType::Game;
}

FActiveDUETweenHandle UDUETweenSubsystem::AddTween(FDUETweenData& TweenData)
{
	DECLARE_CYCLE_STAT(TEXT("AddTween"), STAT_AddTween, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_AddTween);
	FActiveDUETween* NewTweenObject = GetTweenFromHandle(Pool.GetTweenFromPool());
	if (NewTweenObject == nullptr)
	{
		return NULL_DUETWEEN_HANDLE;
	}
	{
		DECLARE_CYCLE_STAT(TEXT("AddTween_AddToChain"), STAT_AddTween_AddToChain, STATGROUP_DUETween);
		SCOPE_CYCLE_COUNTER(STAT_AddTween_AddToChain);
		// Add to tween chain
		NewTweenObject->TweenPtr.ActiveNode.NextActiveTween = ActiveTweenChainStart;
		if (ActiveTweenChainStart != NULL_DUETWEEN_HANDLE)
		{
			GetTweenFromHandle(ActiveTweenChainStart)->TweenPtr.ActiveNode.LastActiveTween = NewTweenObject->Handle;
		}
		NewTweenObject->TweenPtr.ActiveNode.LastActiveTween = NULL_DUETWEEN_HANDLE;
		ActiveTweenChainStart = NewTweenObject->Handle;
	}
	{
		DECLARE_CYCLE_STAT(TEXT("AddTween_Init"), STAT_AddTween_Init, STATGROUP_DUETween);
		SCOPE_CYCLE_COUNTER(STAT_AddTween_Init);
		NewTweenObject->TweenData = MoveTemp(TweenData);
		NewTweenObject->Status = EDUETweenStatus::Running;
		NewTweenObject->TimeElapsed = 0;
		NewTweenObject->StartingValue = TweenData.StartingValue.GetCurrentSubtypeIndex() == static_cast<uint8>(-1)
			                                ? FDUETweenInternalUtils::GetCurrentValueFromProperty(TweenData)
			                                : TweenData.StartingValue;
		NewTweenObject->ID = LastAssignedTweenId + 1;
		UE_LOG(LogDUETween, Verbose, TEXT("Creating Tween: %u"), NewTweenObject->ID);
		LastAssignedTweenId += 1;

		INC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}

	return NewTweenObject->Handle;
}

bool UDUETweenSubsystem::PauseTween(const FActiveDUETweenHandle& TweenHandle) const
{
	FActiveDUETween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDUETweenStatus::Unset)
	{
		return false;
	}

	if (Tween->Status == EDUETweenStatus::Running)
	{
		Tween->Status = EDUETweenStatus::Paused;
		return true;
	}
	return false;
}

bool UDUETweenSubsystem::ResumeTween(const FActiveDUETweenHandle& TweenHandle) const
{
	FActiveDUETween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDUETweenStatus::Unset)
	{
		return false;
	}

	if (Tween->Status == EDUETweenStatus::Paused)
	{
		Tween->Status = EDUETweenStatus::Running;
		return true;
	}
	return false;
}

bool UDUETweenSubsystem::FastForwardTween(const FActiveDUETweenHandle& TweenHandle) const
{
	FActiveDUETween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDUETweenStatus::Unset)
	{
		return false;
	}
	if (Tween->Status != EDUETweenStatus::Completed &&
		Tween->Status != EDUETweenStatus::FastForward)
	{
		Tween->Status = EDUETweenStatus::FastForward;
		return true;
	}
	return false;
}

bool UDUETweenSubsystem::StopTween(const FActiveDUETweenHandle& TweenHandle) const
{
	FActiveDUETween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDUETweenStatus::Unset)
	{
		return false;
	}
	if (Tween->Status != EDUETweenStatus::Completed)
	{
		Tween->Status = EDUETweenStatus::Completed;
		return true;
	}
	return false;
}

void UDUETweenSubsystem::StopAllTweens(TWeakObjectPtr<UObject> Object) const
{
	if (!Object.IsValid())
	{
		return;
	}
	FActiveDUETweenHandle CurrentTweenHandle = ActiveTweenChainStart;
	while (CurrentTweenHandle != NULL_DUETWEEN_HANDLE)
	{
		FActiveDUETween* CurrentTween = GetTweenFromHandle(CurrentTweenHandle);

		// Grab this before we tick in-case tween is removed from chain during tick
		const FActiveDUETweenHandle NextTween = CurrentTween->TweenPtr.ActiveNode.NextActiveTween;
		if (CurrentTween->TweenData.Target.IsValid() && CurrentTween->TweenData.Target.Get() == Object.Get())
		{
			StopTween(CurrentTweenHandle);
		}
		CurrentTweenHandle = NextTween;
	}
}

FActiveDUETween* UDUETweenSubsystem::GetTweenFromHandle(const FActiveDUETweenHandle& TweenHandle) const
{
	return Pool.GetTweenFromHandle(TweenHandle);
}

void UDUETweenSubsystem::Tick(float DeltaTime)
{
	DECLARE_CYCLE_STAT(TEXT("TickAllTweens"), STAT_TickAllTweens, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_TickAllTweens);
	FActiveDUETweenHandle CurrentTweenHandle = ActiveTweenChainStart;
	while (CurrentTweenHandle != NULL_DUETWEEN_HANDLE)
	{
		FActiveDUETween* CurrentTween = GetTweenFromHandle(CurrentTweenHandle);

		// Grab this before we tick in-case tween is removed from chain during tick
		const FActiveDUETweenHandle NextTween = CurrentTween->TweenPtr.ActiveNode.NextActiveTween;

		TickTween(DeltaTime, CurrentTween);

		CurrentTweenHandle = NextTween;
	}
}

bool UDUETweenSubsystem::IsTickable() const
{
	return ActiveTweenChainStart != NULL_DUETWEEN_HANDLE;
}

void UDUETweenSubsystem::TickTween(float DeltaTime, FActiveDUETween* CurrentTween)
{
	DECLARE_CYCLE_STAT(TEXT("TickTween"), STAT_TickTween, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_TickTween);


	// Early-Complete tween if target is gone
	if (!CurrentTween->TweenData.Target.IsValid() && CurrentTween->Status != EDUETweenStatus::Completed)
	{
		CurrentTween->Status = EDUETweenStatus::Completed;
		return;
	}

	// Make sure we're not paused
	if (CurrentTween->TweenData.Target.IsValid())
	{
		UWorld* TweenWorld = CurrentTween->TweenData.Target.Get()->GetWorld();
		if (TweenWorld && TweenWorld->IsPaused())
		{
#if WITH_EDITOR
			if (!TweenWorld->bDebugFrameStepExecutedThisFrame)
			{
#endif
				UE_LOG(LogDUETween, Verbose, TEXT("Skipping Paused Tween"));
				return;
#if WITH_EDITOR
			}
#endif
		}
	}

	if (CurrentTween->Status == EDUETweenStatus::Running || CurrentTween->Status == EDUETweenStatus::FastForward)
	{
		CurrentTween->TimeElapsed += DeltaTime;

		float TweenProgress = CurrentTween->TimeElapsed / CurrentTween->TweenData.Duration;
		if (CurrentTween->Status == EDUETweenStatus::FastForward)
		{
			TweenProgress = 1.0;
		}

		DECLARE_CYCLE_STAT(TEXT("ProgressTween"), STAT_ProgressTween, STATGROUP_DUETween);
		SCOPE_CYCLE_COUNTER(STAT_ProgressTween);

		switch (CurrentTween->TweenData.ValueType)
		{
		case EDueValueType::Float:
			{
				const float NewValue = DUEEasingFunctionLibrary::Ease(CurrentTween->StartingValue.GetSubtype<float>(),
				                                                      CurrentTween->TweenData.TargetValue.GetSubtype<
					                                                      float>(),
				                                                      TweenProgress,
				                                                      CurrentTween->TweenData.EasingType,
				                                                      CurrentTween->TweenData.Steps);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Float Value:: %f"), NewValue);
				FDUETweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
		case EDueValueType::Double:
			{
				const double NewValue = DUEEasingFunctionLibrary::Ease(CurrentTween->StartingValue.GetSubtype<double>(),
				                                                       CurrentTween->TweenData.TargetValue.GetSubtype<
					                                                       double>(),
				                                                       TweenProgress,
				                                                       CurrentTween->TweenData.EasingType,
				                                                       CurrentTween->TweenData.Steps);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Double Value:: %f"), NewValue);
				FDUETweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
		case EDueValueType::Vector:
			{
				const double Alpha = DUEEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps);
				const FVector NewValue = FMath::Lerp(CurrentTween->StartingValue.GetSubtype<FVector>(),
				                                     CurrentTween->TweenData.TargetValue.GetSubtype<FVector>(), Alpha);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Vector Value: %s"), *NewValue.ToString());
				FDUETweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
		case EDueValueType::Rotator:
			{
				const double Alpha = DUEEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps);
				const FRotator NewValue = FRotator(
					FQuat::Slerp(
						CurrentTween->StartingValue.GetSubtype<FRotator>().Quaternion(),
						CurrentTween->TweenData.TargetValue.GetSubtype<FRotator>().Quaternion(),
						Alpha)
				);

				UE_LOG(LogDUETween, Verbose, TEXT("Updating Rotator Value: %s"), *NewValue.ToString());
				FDUETweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
			}
			break;
		case EDueValueType::Vector2D:
			{
				const double Alpha = DUEEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps);
				const FVector2D NewValue = FMath::Lerp(CurrentTween->StartingValue.GetSubtype<FVector2D>(),
				                                       CurrentTween->TweenData.TargetValue.GetSubtype<FVector2D>(),
				                                       Alpha);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Vector 2D Value: %s"), *NewValue.ToString());
				FDUETweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
			break;
		}
		if (TweenProgress >= 1.0)
		{
			CurrentTween->Status = EDUETweenStatus::Completed;
		}
	}
	else if (CurrentTween->Status == EDUETweenStatus::Paused)
	{
		// Do nothing
	}
	else if (CurrentTween->Status == EDUETweenStatus::Completed)
	{
		RemoveTweenFromActiveChain(CurrentTween->Handle);
		Pool.ReturnTweenToPool(CurrentTween->Handle);
	}
}

void UDUETweenSubsystem::RemoveTweenFromActiveChain(FActiveDUETweenHandle TweenHandleToRemove)
{
	const FActiveDUETween* TweenToRemove = Pool.GetTweenFromHandle(TweenHandleToRemove);
	// First remove from the active tween chain
	{
		DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool_RemoveFromChain"), STAT_ReturnTweenToPool_RemoveFromChain,
		                   STATGROUP_DUETween);
		SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool_RemoveFromChain);
		if (TweenToRemove->Handle == ActiveTweenChainStart)
		{
			ActiveTweenChainStart = Pool.GetTweenFromHandle(ActiveTweenChainStart)->TweenPtr.ActiveNode.NextActiveTween;
		}
		else
		{
			FActiveDUETween* PreviousNode = Pool.GetTweenFromHandle(TweenToRemove->TweenPtr.ActiveNode.LastActiveTween);
			FActiveDUETween* NextNode = Pool.GetTweenFromHandle(TweenToRemove->TweenPtr.ActiveNode.NextActiveTween);
			if (PreviousNode != nullptr)
			{
				PreviousNode->TweenPtr.ActiveNode.NextActiveTween = TweenToRemove->TweenPtr.ActiveNode.NextActiveTween;
				UE_LOG(LogDUETween, Verbose, TEXT("Tween: %u next now points to Tween: %u"), PreviousNode->ID,
				       NextNode != nullptr ?NextNode->ID : 0);
			}
			if (NextNode != nullptr)
			{
				NextNode->TweenPtr.ActiveNode.LastActiveTween = TweenToRemove->TweenPtr.ActiveNode.LastActiveTween;
				UE_LOG(LogDUETween, Verbose, TEXT("Tween: %u previous now points to Tween: %u"), NextNode->ID,
				       PreviousNode != nullptr ? PreviousNode->ID: 0);
			}
		}
		DEC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}
}
