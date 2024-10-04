#include "DUETween.h"

#include "Components/CanvasPanelSlot.h"

FActiveDUETweenHandle DUETween::DUEMove(const TWeakObjectPtr<UObject>& Target, const FVector& TargetLocation,
                                        const float& Duration, const EDueEasingType& Easing, const int32& Steps)
{
	if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
	{
		FVector StartingValue = TargetAsActor->GetActorLocation();
		FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
		                                   const TWeakObjectPtr<UObject>& TargetToUpdate)
		{
			AActor* TargetActor = Cast<AActor>(TargetToUpdate.Get());
			TargetActor->SetActorLocation(UpdatedValue.GetSubtype<FVector>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetLocation, Duration,
		                     Easing, Steps);
	}

	if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(Target);
		TargetAsSceneComponent)
	{
		const FVector StartingValue = TargetAsSceneComponent->GetRelativeLocation();
		FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
		                                                         const TWeakObjectPtr<UObject>& TargetToUpdate)
		{
			USceneComponent* SceneComp = Cast<USceneComponent>(TargetToUpdate.Get());
			SceneComp->SetRelativeLocation(UpdatedValue.GetSubtype<FVector>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetLocation, Duration,
		                     Easing, Steps);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move: %s"),
	       *Target->StaticClass()->GetClassPathName().ToString());

	return NULL_DUETWEEN_HANDLE;
}

FActiveDUETweenHandle DUETween::DUERotate(const TWeakObjectPtr<UObject>& Target, const FRotator& TargetRotation,
                                          const float& Duration, const EDueEasingType& Easing, const int32& Steps)
{
	if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
	{
		FRotator StartingValue = TargetAsActor->GetActorRotation();
		FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
		                                   const TWeakObjectPtr<UObject>& TargetToUpdate)
		{
			AActor* TargetActor = Cast<AActor>(TargetToUpdate.Get());
			TargetActor->SetActorRotation(UpdatedValue.GetSubtype<FRotator>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetRotation, Duration,
		                     Easing, Steps);
	}

	if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(Target);
		TargetAsSceneComponent)
	{
		FRotator StartingValue = TargetAsSceneComponent->GetRelativeRotation();
		FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
		                                   const TWeakObjectPtr<UObject>& TargetToUpdate)
		{
			USceneComponent* SceneComp = Cast<USceneComponent>(TargetToUpdate.Get());
			SceneComp->SetRelativeRotation(UpdatedValue.GetSubtype<FRotator>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetRotation, Duration,
		                     Easing, Steps);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due rotate: %s"),
	       *Target->StaticClass()->GetClassPathName().ToString());
	return NULL_DUETWEEN_HANDLE;
}

FActiveDUETweenHandle DUETween::DUEMove2D(const TWeakObjectPtr<UObject>& Target, const FVector2D& TargetValue,
                                          const float& Duration, const EDueEasingType& Easing, const int32& Steps)
{
	if (UCanvasPanelSlot* TargetAsCanvasPanelSlot = Cast<UCanvasPanelSlot>(Target);
		TargetAsCanvasPanelSlot)
	{
		FVector2D StartingValue = TargetAsCanvasPanelSlot->GetPosition();
		FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
		                                   const TWeakObjectPtr<UObject>& TargetToUpdate)
		{
			UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(TargetToUpdate.Get());
			CanvasPanelSlot->SetPosition(UpdatedValue.GetSubtype<FVector2D>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetValue, Duration,
		                     Easing, Steps);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move 2d: %s"),
	       *Target->StaticClass()->GetClassPathName().ToString());
	return NULL_DUETWEEN_HANDLE;
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
