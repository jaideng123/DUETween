#include "DUETween.h"

#include "Components/CanvasPanelSlot.h"

FActiveDUETweenHandle DUETween::DUEMove(const TWeakObjectPtr<UObject>& Target, const FVector& TargetLocation,
                                        const float& Duration, const EDueEasingType& Easing, const int32& Steps,
                                        const int32& LoopCount, const bool& YoYo)
{
	if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
	{
		FVector StartingValue = TargetAsActor->GetActorLocation();
		FTweenUpdateCallback TargetCallback = [TargetAsActor](const FValueContainer& UpdatedValue)
		{
			TargetAsActor->SetActorLocation(UpdatedValue.GetSubtype<FVector>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetLocation, Duration,
		                     Easing, Steps, LoopCount, YoYo);
	}

	if (USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(Target);
		TargetAsSceneComponent)
	{
		const FVector StartingValue = TargetAsSceneComponent->GetRelativeLocation();
		FTweenUpdateCallback TargetCallback = [TargetAsSceneComponent](const FValueContainer& UpdatedValue)
		{
			TargetAsSceneComponent->SetRelativeLocation(UpdatedValue.GetSubtype<FVector>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetLocation, Duration,
		                     Easing, Steps, LoopCount, YoYo);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move: %s"),
	       *Target->StaticClass()->GetClassPathName().ToString());

	return FActiveDUETweenHandle::NULL_HANDLE();
}

FActiveDUETweenHandle DUETween::DUERotate(const TWeakObjectPtr<UObject>& Target, const FRotator& TargetRotation,
                                          const float& Duration, const EDueEasingType& Easing, const int32& Steps,
                                          const int32& LoopCount, const bool& YoYo)
{
	if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
	{
		FRotator StartingValue = TargetAsActor->GetActorRotation();
		FTweenUpdateCallback TargetCallback = [TargetAsActor](const FValueContainer& UpdatedValue)
		{
			TargetAsActor->SetActorRotation(UpdatedValue.GetSubtype<FRotator>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetRotation, Duration,
		                     Easing, Steps, LoopCount, YoYo);
	}

	if (USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(Target);
		TargetAsSceneComponent)
	{
		FRotator StartingValue = TargetAsSceneComponent->GetRelativeRotation();
		FTweenUpdateCallback TargetCallback = [TargetAsSceneComponent](const FValueContainer& UpdatedValue)
		{
			TargetAsSceneComponent->SetRelativeRotation(UpdatedValue.GetSubtype<FRotator>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetRotation, Duration,
		                     Easing, Steps, LoopCount, YoYo);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due rotate: %s"),
	       *Target->StaticClass()->GetClassPathName().ToString());
	return FActiveDUETweenHandle::NULL_HANDLE();
}

FActiveDUETweenHandle DUETween::DUEMove2D(const TWeakObjectPtr<UObject>& Target, const FVector2D& TargetValue,
                                          const float& Duration, const EDueEasingType& Easing, const int32& Steps,
                                          const int32& LoopCount, const bool& YoYo)
{
	if (UCanvasPanelSlot* TargetAsCanvasPanelSlot = Cast<UCanvasPanelSlot>(Target);
		TargetAsCanvasPanelSlot)
	{
		FVector2D StartingValue = TargetAsCanvasPanelSlot->GetPosition();
		FTweenUpdateCallback TargetCallback = [TargetAsCanvasPanelSlot](const FValueContainer& UpdatedValue)
		{
			TargetAsCanvasPanelSlot->SetPosition(UpdatedValue.GetSubtype<FVector2D>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetValue, Duration,
		                     Easing, Steps, LoopCount, YoYo);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move 2d: %s"),
	       *Target->StaticClass()->GetClassPathName().ToString());
	return FActiveDUETweenHandle::NULL_HANDLE();
}

bool DUETween::PauseDUETween(const TWeakObjectPtr<UObject>& Target, const FActiveDUETweenHandle& DUETweenHandle)
{
	if (!Target.IsValid())
	{
		return false;
	}
	if (const UWorld* World = GEngine->GetWorldFromContextObject(Target.Get(), EGetWorldErrorMode::ReturnNull))
	{
		return World->GetSubsystem<UDUETweenSubsystem>()->PauseTween(DUETweenHandle);
	}
	return false;
}

bool DUETween::ResumeDUETween(const TWeakObjectPtr<UObject>& Target, const FActiveDUETweenHandle& DUETweenHandle)
{
	if (!Target.IsValid())
	{
		return false;
	}
	if (const UWorld* World = GEngine->GetWorldFromContextObject(Target.Get(), EGetWorldErrorMode::ReturnNull))
	{
		return World->GetSubsystem<UDUETweenSubsystem>()->ResumeTween(DUETweenHandle);
	}
	return false;
}

bool DUETween::FastForwardDUETween(const TWeakObjectPtr<UObject>& Target, const FActiveDUETweenHandle& DUETweenHandle)
{
	if (!Target.IsValid())
	{
		return false;
	}
	if (const UWorld* World = GEngine->GetWorldFromContextObject(Target.Get(), EGetWorldErrorMode::ReturnNull))
	{
		return World->GetSubsystem<UDUETweenSubsystem>()->FastForwardTween(DUETweenHandle);
	}
	return false;
}

bool DUETween::StopDUETween(const TWeakObjectPtr<UObject>& Target, const FActiveDUETweenHandle& DUETweenHandle)
{
	if (!Target.IsValid())
	{
		return false;
	}
	if (const UWorld* World = GEngine->GetWorldFromContextObject(Target.Get(), EGetWorldErrorMode::ReturnNull))
	{
		return World->GetSubsystem<UDUETweenSubsystem>()->StopTween(DUETweenHandle);
	}
	return false;
}

bool DUETween::StopAllDUETweens(const TWeakObjectPtr<UObject>& Target)
{
	if (!Target.IsValid())
	{
		return false;
	}
	if (const UWorld* World = GEngine->GetWorldFromContextObject(Target.Get(), EGetWorldErrorMode::ReturnNull))
	{
		World->GetSubsystem<UDUETweenSubsystem>()->StopAllTweens(Target);
		return true;
	}
	return false;
}
