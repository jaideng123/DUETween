#pragma once

#include "ActiveDUETweenHandle.generated.h"

USTRUCT(BlueprintType)
struct FActiveDUETweenHandle
{
	GENERATED_BODY()

	int HandleIndex;
	int Version;
	TWeakObjectPtr<UWorld> BoundWorld;


	FActiveDUETweenHandle()
	{
		HandleIndex = -1;
		Version = -1;
		BoundWorld = nullptr;
	}

	static FActiveDUETweenHandle NULL_HANDLE()
	{
		return FActiveDUETweenHandle();
	}

	FActiveDUETweenHandle(int HandleIndex, TWeakObjectPtr<UWorld> BoundWorld, int Version)
		: HandleIndex(HandleIndex),
		  Version(Version),
		  BoundWorld(BoundWorld)
	{
	}


	// Tween API

	// Pauses this tween
	FActiveDUETweenHandle& PauseTween();

	// Resumes this tween
	FActiveDUETweenHandle& ResumeTween();

	// Skips to the end of this tween
	FActiveDUETweenHandle& FastForwardTween();

	// Stops and ends this tween (handle will be invalid 1 frame after this)
	FActiveDUETweenHandle& StopTween();

	// Updates whether or not this tween should yoyo (return to it's starting position)
	FActiveDUETweenHandle& SetSteps(int32 Steps);

	// Updates whether or not this tween should yoyo (return to it's starting position)
	FActiveDUETweenHandle& SetYoYo(bool ShouldYoYo);

	// Updates Number Of Loops this tween will do before completing (-1 = infinite)
	FActiveDUETweenHandle& SetLoopCount(int32 NewLoopCount);

	// Binds a completion event (performs a move on the argument)
	// Note: Each tween can only have one completion callback
	// FActiveDUETweenHandle& OnComplete(FTweenCompleteCallback& CompletionCallback);


	/**
	 * Nullptr inequality operator.
	 */
	FORCEINLINE bool operator==(TYPE_OF_NULLPTR) const
	{
		return HandleIndex < 0;
	}

	/**
	 * Nullptr inequality operator.
	 */
	FORCEINLINE bool operator!=(TYPE_OF_NULLPTR) const
	{
		return HandleIndex >= 0;
	}


	friend bool operator==(const FActiveDUETweenHandle& Lhs, const FActiveDUETweenHandle& RHS)
	{
		return Lhs.HandleIndex == RHS.HandleIndex
			&& Lhs.Version == RHS.Version
			&& Lhs.BoundWorld == RHS.BoundWorld;
	}

	friend bool operator!=(const FActiveDUETweenHandle& Lhs, const FActiveDUETweenHandle& RHS)
	{
		return !(Lhs == RHS);
	}
};
