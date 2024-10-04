#pragma once
#include "DUETweenModule.h"
#include "DUEEasingFunctionLibrary.h"
#include "DUETweenSubsystem.h"
#include "LatentActions.h"


/**
 * Latent Action that tracks an active tween
 */
class DUETWEEN_API FDUETweenAction : public FPendingLatentAction
{
public:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

private:
	FActiveDUETweenHandle ActiveDUETween;

public:
	FDUETweenAction(const FLatentActionInfo& LatentInfo, const FActiveDUETweenHandle& NewActiveDUETweenHandle)
		: ExecutionFunction(LatentInfo.ExecutionFunction)
		  , OutputLink(LatentInfo.Linkage)
		  , CallbackTarget(LatentInfo.CallbackTarget)
	{
		ActiveDUETween = NewActiveDUETweenHandle;
	}

	virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
	// Returns a human readable description of the latent operation's current state
	virtual FString GetDescription() const override
	{
		static const FNumberFormattingOptions DelayTimeFormatOptions = FNumberFormattingOptions()
		                                                               .SetMinimumFractionalDigits(3)
		                                                               .SetMaximumFractionalDigits(3);
		FActiveDUETween* tween = CallbackTarget.Get()->GetWorld()->GetSubsystem<UDUETweenSubsystem>()->
		                                        GetTweenFromHandle(ActiveDUETween);
		return FText::Format(
			NSLOCTEXT("DUETweenAction", "DelayActionTimeFmt", "Tween ({0} seconds left)"),
			FText::AsNumber(
				tween->TweenData.Duration - CallbackTarget.Get()->GetWorld()->GetSubsystem<UDUETweenSubsystem>()->
				                                           GetTweenFromHandle(ActiveDUETween)->TimeElapsed,
				&DelayTimeFormatOptions)).ToString();
	}
#endif
};
