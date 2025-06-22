#include "DUETween.h"

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"

FActiveDUETweenHandle DUETween::DUEMove(const TWeakObjectPtr<UObject>& Target, const FVector& TargetLocation,
                                        const float& Duration, const EDueEasingType& Easing)
{
	if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
	{
		FVector StartingValue = TargetAsActor->GetActorLocation();
		FTweenUpdateCallback TargetCallback = [TargetAsActor](const FValueContainer& UpdatedValue)
		{
			TargetAsActor->SetActorLocation(UpdatedValue.GetSubtype<FVector>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetLocation, Duration,
		                     Easing);
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
		                     Easing);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move: %s"),
	       *Target->StaticClass()->GetClassPathName().ToString());

	return FActiveDUETweenHandle::NULL_HANDLE();
}

FActiveDUETweenHandle DUETween::DUERotate(const TWeakObjectPtr<UObject>& Target, const FRotator& TargetRotation,
                                          const float& Duration, const EDueEasingType& Easing)
{
	if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
	{
		FRotator StartingValue = TargetAsActor->GetActorRotation();
		FTweenUpdateCallback TargetCallback = [TargetAsActor](const FValueContainer& UpdatedValue)
		{
			TargetAsActor->SetActorRotation(UpdatedValue.GetSubtype<FRotator>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetRotation, Duration,
		                     Easing);
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
		                     Easing);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due rotate: %s"),
	       *Target->StaticClass()->GetClassPathName().ToString());
	return FActiveDUETweenHandle::NULL_HANDLE();
}

FActiveDUETweenHandle DUETween::DUEMove2D(const TWeakObjectPtr<UObject>& Target, const FVector2D& TargetValue,
                                          const float& Duration, const EDueEasingType& Easing)
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
		                     Easing);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move 2d: %s"),
	       *Target->StaticClass()->GetClassPathName().ToString());
	return FActiveDUETweenHandle::NULL_HANDLE();
}

FActiveDUETweenHandle DUETween::DUERotate2D(const TWeakObjectPtr<UObject>& Target, const float& TargetRotation,
										  const float& Duration, const EDueEasingType& Easing)
{
	if (UWidget* TargetAsUserWidget = Cast<UWidget>(Target);TargetAsUserWidget)
	{
		float StartingValue = TargetAsUserWidget->GetRenderTransformAngle();
		FTweenUpdateCallback TargetCallback = [TargetAsUserWidget](const FValueContainer& UpdatedValue)
		{
			TargetAsUserWidget->SetRenderTransformAngle(UpdatedValue.GetSubtype<float>());
		};
		return StartDUETween(Target, TargetCallback, StartingValue, TargetRotation, Duration,
							 Easing);
	}

	UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due rotate 2d: %s"),
		   *Target->StaticClass()->GetClassPathName().ToString());
	return FActiveDUETweenHandle::NULL_HANDLE();
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
