#include "ActiveDUETweenHandle.h"

#include "ActiveDUETween.h"
#include "DUETweenSubsystem.h"

FActiveDUETweenHandle& FActiveDUETweenHandle::PauseTween()
{
	if (BoundWorld == nullptr)
	{
		BoundWorld->GetSubsystem<UDUETweenSubsystem>()->PauseTween(*this);
	}
	return *this;
}

FActiveDUETweenHandle& FActiveDUETweenHandle::ResumeTween()
{
	if (BoundWorld == nullptr)
	{
		BoundWorld->GetSubsystem<UDUETweenSubsystem>()->ResumeTween(*this);
	}
	return *this;
}

FActiveDUETweenHandle& FActiveDUETweenHandle::FastForwardTween()
{
	if (BoundWorld == nullptr)
	{
		BoundWorld->GetSubsystem<UDUETweenSubsystem>()->FastForwardTween(*this);
	}
	return *this;
}

FActiveDUETweenHandle& FActiveDUETweenHandle::StopTween()
{
	if (BoundWorld == nullptr)
	{
		BoundWorld->GetSubsystem<UDUETweenSubsystem>()->StopTween(*this);
	}
	return *this;
}

FActiveDUETweenHandle& FActiveDUETweenHandle::SetSteps(int32 Steps)
{
	if (BoundWorld == nullptr)
	{
		FActiveDUETween* Tween = BoundWorld->GetSubsystem<UDUETweenSubsystem>()->GetTweenFromHandle(*this);
		if(Tween != nullptr)
		{
			Tween->TweenData.Steps = Steps;
		}
	}
	return *this;
}

FActiveDUETweenHandle& FActiveDUETweenHandle::SetYoYo(bool ShouldYoYo)
{
	if (BoundWorld == nullptr)
	{
		FActiveDUETween* Tween = BoundWorld->GetSubsystem<UDUETweenSubsystem>()->GetTweenFromHandle(*this);
		if(Tween != nullptr)
		{
			Tween->TweenData.ShouldYoYo = ShouldYoYo;
		}
	}
	return *this;
}

FActiveDUETweenHandle& FActiveDUETweenHandle::SetLoopCount(int32 NewLoopCount)
{
	if (BoundWorld == nullptr)
	{
		FActiveDUETween* Tween = BoundWorld->GetSubsystem<UDUETweenSubsystem>()->GetTweenFromHandle(*this);
		if(Tween != nullptr)
		{
			Tween->TweenData.LoopCount = NewLoopCount;
		}
	}
	return *this;
}

// FActiveDUETweenHandle& FActiveDUETweenHandle::OnComplete(FTweenCompleteCallback& CompletionCallback)
// {
// 	if (BoundWorld == nullptr)
// 	{
// 		FActiveDUETween* Tween = BoundWorld->GetSubsystem<UDUETweenSubsystem>()->GetTweenFromHandle(*this);
// 		if(Tween != nullptr)
// 		{
// 			Tween->TweenData.CompletionCallback = MoveTemp(CompletionCallback);
// 		}
// 	}
// 	return *this;
// }
