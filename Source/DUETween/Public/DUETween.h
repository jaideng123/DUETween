#pragma once
#include "ActiveDUETween.h"
#include "DUETweenModule.h"
#include "DUETweenSubsystem.h"

class DUETWEEN_API DUETween
{
public:
	// Start Tween with property pointer
	template <typename T>
	static FActiveDUETweenHandle StartDUETween(const TWeakObjectPtr<UObject>& Target,
	                                           FProperty* Property,
	                                           const T& TargetValue,
	                                           const float& Duration,
	                                           const EDueEasingType& Easing = EDueEasingType::InOutSin,
	                                           const int32& Steps = 0,
	                                           const int32& LoopCount = 0,
	                                           const bool& YoYo = false)
	{
		static_assert(GetDueValueTypeConst<T>() == EDueValueType::Unset, "Unsupported Type");

		FDUETweenData TweenData;
		TweenData.Target = Target;
		TweenData.Duration = Duration;
		TweenData.EasingType = Easing;
		TweenData.Steps = Steps;
		TweenData.UpdateType = EDueUpdateType::Property;
		TweenData.TargetProperty = Property;
		TweenData.TargetValue.SetSubtype<T>(TargetValue);
		constexpr EDueValueType ValueType = GetDueValueType<T>();
		TweenData.ValueType = ValueType;

		const UWorld* World = Target.Get()->GetWorld();
		return World->GetSubsystem<UDUETweenSubsystem>()->AddTween(TweenData);
	}

	// Start Tween with property name
	template <typename T>
	static FActiveDUETweenHandle StartDUETween(const TWeakObjectPtr<UObject>& Target,
	                                           const FName& PropertyName,
	                                           const T& TargetValue,
	                                           const float& Duration,
	                                           const EDueEasingType& Easing = EDueEasingType::InOutSin,
	                                           const int32& Steps = 0,
	                                           const int32& LoopCount = 0,
	                                           const bool& YoYo = false)
	{
		static_assert(GetDueValueTypeConst<T>() == EDueValueType::Unset, "Unsupported Type");

		FProperty* PropertyRef = Target->GetClass()->FindPropertyByName(PropertyName);
		if (PropertyRef == nullptr)
		{
			UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *PropertyName.ToString());
			return FActiveDUETweenHandle::NULL_HANDLE();
		}
		return StartDUETween(Target, PropertyRef, TargetValue, Duration, Easing, Steps, LoopCount, YoYo);
	}

	// Start Tween with callback update function
	template <typename T>
	static FActiveDUETweenHandle StartDUETween(const TWeakObjectPtr<UObject>& Target,
	                                           FTweenUpdateCallback& UpdateCallback,
	                                           const T& StartingValue,
	                                           const T& TargetValue,
	                                           const float& Duration,
	                                           const EDueEasingType& Easing = EDueEasingType::InOutSin,
	                                           const int32& Steps = 0,
	                                           const int32& LoopCount = 0,
	                                           const bool& YoYo = false)
	{
		static_assert(GetDueValueTypeConst<T>() == EDueValueType::Unset, "Unsupported Type");

		UpdateCallback.CheckCallable();
		
		FDUETweenData TweenData;
		TweenData.Target = Target;
		TweenData.Duration = Duration;
		TweenData.EasingType = Easing;
		TweenData.Steps = Steps;
		TweenData.UpdateType = EDueUpdateType::Function;
		TweenData.UpdateCallback = MoveTemp(UpdateCallback);
		TweenData.StartingValue.SetSubtype<T>(StartingValue);
		TweenData.TargetValue.SetSubtype<T>(TargetValue);
		constexpr EDueValueType ValueType = GetDueValueType<T>();
		TweenData.ValueType = ValueType;
		TweenData.LoopCount = LoopCount;
		TweenData.ShouldYoYo = YoYo;

		const UWorld* World = Target.Get()->GetWorld();
		return World->GetSubsystem<UDUETweenSubsystem>()->AddTween(TweenData);
	}

	static FActiveDUETweenHandle DUEMove(const TWeakObjectPtr<UObject>& Target,
										   const FVector& TargetValue,
										   const float& Duration,
										   const EDueEasingType& Easing = EDueEasingType::InOutSin,
										   const int32& Steps = 0,
										   const int32& LoopCount = 0,
										   const bool& YoYo = false);

	static FActiveDUETweenHandle DUERotate(const TWeakObjectPtr<UObject>& Target,
									   const FRotator& TargetValue,
									   const float& Duration,
									   const EDueEasingType& Easing = EDueEasingType::InOutSin,
									   const int32& Steps = 0,
									   const int32& LoopCount = 0,
									   const bool& YoYo = false);

	static FActiveDUETweenHandle DUEMove2D(const TWeakObjectPtr<UObject>& Target,
								   const FVector2D& TargetValue,
								   const float& Duration,
								   const EDueEasingType& Easing = EDueEasingType::InOutSin,
								   const int32& Steps = 0,
								   const int32& LoopCount = 0,
								   const bool& YoYo = false);

	// Pauses a currently running tween (returns true if successful)
	static bool PauseDUETween(const TWeakObjectPtr<UObject>& Target, const FActiveDUETweenHandle& DUETweenHandle);

	// Resumes a paused tween (returns true if successful)
	static bool ResumeDUETween(const TWeakObjectPtr<UObject>& Target, const FActiveDUETweenHandle& DUETweenHandle);

	// Skips to the end of a tween (returns true if successful)
	static bool FastForwardDUETween(const TWeakObjectPtr<UObject>& Target, const FActiveDUETweenHandle& DUETweenHandle);

	// Stops and cancels tween (returns true if successful)
	static bool StopDUETween(const TWeakObjectPtr<UObject>& Target, const FActiveDUETweenHandle& DUETweenHandle);

	// Stops and cancels all tweens on a given target
	static bool StopAllDUETweens(const TWeakObjectPtr<UObject>& Target);
};
