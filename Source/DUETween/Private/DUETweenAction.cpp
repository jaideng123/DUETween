#include "DUETweenAction.h"

void FDUETweenAction::UpdateOperation(FLatentResponse& Response)
{
	if (ActiveDUETween == nullptr)
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	FActiveDUETween* Tween = CallbackTarget.Get()->GetWorld()->GetSubsystem<UDUETweenSubsystem>()->
	                                        GetTweenFromHandle(ActiveDUETween);

	Response.FinishAndTriggerIf(Tween->Status == EDUETweenStatus::Completed || Tween->Status == EDUETweenStatus::Unset,
	                            ExecutionFunction, OutputLink,
	                            CallbackTarget);
}
