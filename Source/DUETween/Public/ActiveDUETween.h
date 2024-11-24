#pragma once

#include "Containers/Union.h"
#include "ActiveDUETweenHandle.h"

enum EDueEasingType : int;
using FValueContainer = TUnion<FVector, FVector2D, FRotator, float, double>;
using FTweenUpdateCallback = TUniqueFunction<void(const FValueContainer&)>;
using FTweenCompleteCallback = TUniqueFunction<void()>;

struct FActiveDUETweenHandle;

/**
 * Status of a tween
 */
enum class EDUETweenStatus
{
	// This tween is unused and ready to be reclaimed
	Unset,
	// This tween is running
	Running,
	// This tween is completed and will be marked unused next frame
	Completed,
	// This tween is paused
	Paused,
	// This tween is being fast-forwarded to it's end point
	FastForward,
};

/**
 * Value type for data manipulated by tween
 */
enum class EDueValueType
{
	Unset,
	Float,
	Double,
	Vector,
	Rotator,
	Vector2D
	/**
	 * When adding a new type update:
	 * 1. This
	 * 2. FValueContainer
	 * 3. Set and GetCurrentValueFromProperty
	 * 4. TickTween
	 * 5. UDUETweenBlueprintFunctionLibrary
	 * 6. GetDueValueType
	 */
};

// This doesn't work if you change or add anything for some reason
// TODO: figure out why
template <typename T>
constexpr EDueValueType GetDueValueTypeConst()
{
	if (std::is_same_v<T, const float&>)
	{
		return EDueValueType::Float;
	}
	if (std::is_same_v<T, const double&>)
	{
		return EDueValueType::Double;
	}
	if (std::is_same_v<T, const FVector&>)
	{
		return EDueValueType::Vector;
	}
	if (std::is_same_v<T, const FRotator&>)
	{
		return EDueValueType::Rotator;
	}
	if (std::is_same_v<T, const FVector2D&>)
	{
		return EDueValueType::Vector2D;
	}
	return EDueValueType::Unset;
}


// This is necessary because above only works at compile time and this only works at runtime
template <typename T>
constexpr EDueValueType GetDueValueType()
{
	if (std::is_same_v<T, float>)
	{
		return EDueValueType::Float;
	}
	if (std::is_same_v<T, double>)
	{
		return EDueValueType::Double;
	}
	if (std::is_same_v<T, FVector>)
	{
		return EDueValueType::Vector;
	}
	if (std::is_same_v<T, FRotator>)
	{
		return EDueValueType::Rotator;
	}
	if (std::is_same_v<T, FVector2D>)
	{
		return EDueValueType::Vector2D;
	}
	return EDueValueType::Unset;
}

/**
 * Value type for data manipulated by tween
 */
enum class EDueUpdateType
{
	Unset,
	Property,
	Function
	/**
	 * When adding a new type update:
	 * 1. This
	 * 2. Set and GetCurrentValue
	 */
};

/**
 * Data structure that defines the characteristics and callbacks for a tween
 */
struct FDUETweenData
{
	FTweenUpdateCallback UpdateCallback;
	FTweenCompleteCallback CompletionCallback;
	FProperty* TargetProperty;
	EDueUpdateType UpdateType = EDueUpdateType::Unset;

	EDueValueType ValueType = EDueValueType::Float;

	FValueContainer TargetValue;
	FValueContainer StartingValue;

	TWeakObjectPtr<UObject> Target;
	float Duration = 0;
	EDueEasingType EasingType;
	int32 Steps = 0;
	int32 LoopCount = 0;
	bool ShouldYoYo = false;

	FDUETweenData(): EasingType()
	{
		TargetProperty = nullptr;
		UpdateCallback = nullptr;
		CompletionCallback = nullptr;
	}

	FDUETweenData(FDUETweenData&& Other) noexcept
		: UpdateCallback(std::move(Other.UpdateCallback)),
		  CompletionCallback(std::move(Other.CompletionCallback)),
		  TargetProperty(Other.TargetProperty),
		  UpdateType(Other.UpdateType),
		  ValueType(Other.ValueType),
		  TargetValue(std::move(Other.TargetValue)),
		  StartingValue(std::move(Other.StartingValue)),
		  Target(std::move(Other.Target)),
		  Duration(Other.Duration),
		  EasingType(Other.EasingType),
		  Steps(Other.Steps),
		  LoopCount(Other.LoopCount),
		  ShouldYoYo(Other.ShouldYoYo)

	{
	}


	FDUETweenData& operator=(FDUETweenData&& Other) noexcept
	{
		if (this == &Other)
			return *this;
		UpdateCallback = std::move(Other.UpdateCallback);
		CompletionCallback = std::move(Other.CompletionCallback);
		Other.UpdateCallback = nullptr;
		TargetProperty = Other.TargetProperty;
		UpdateType = Other.UpdateType;
		ValueType = Other.ValueType;
		TargetValue = std::move(Other.TargetValue);
		StartingValue = std::move(Other.StartingValue);
		Target = std::move(Other.Target);
		Duration = Other.Duration;
		EasingType = Other.EasingType;
		Steps = Other.Steps;
		LoopCount = Other.LoopCount;
		ShouldYoYo = Other.ShouldYoYo;
		return *this;
	}
};

/**
 * Data structure for an active tween
 */
struct FActiveDUETween
{
	union
	{
		struct FActiveNode
		{
			FActiveDUETweenHandle NextActiveTween;
			FActiveDUETweenHandle LastActiveTween;
		};

		FActiveNode ActiveNode;
		FActiveDUETweenHandle NextFreeTween;
	} TweenPtr;

	FDUETweenData TweenData;

	// Constant state
	FActiveDUETweenHandle Handle;

	// Per-Tween State
	unsigned int ID = 0;
	float TimeElapsed = 0;
	EDUETweenStatus Status = EDUETweenStatus::Unset;
	bool IsReversing = false;

	FValueContainer StartingValue;
};
