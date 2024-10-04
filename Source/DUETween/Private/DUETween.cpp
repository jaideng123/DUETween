#include "DUETween.h"

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
