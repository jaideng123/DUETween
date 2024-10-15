#include "DUETweenPool.h"

#include "DUETweenModule.h"
#include "DUETweenSettings.h"
#include "Logging/StructuredLog.h"

DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Pooled Tweens"), STAT_POOLED_TWEENS, STATGROUP_DUETween);
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Tween Pool Size"), STAT_TWEEN_POOL_SIZE, STATGROUP_DUETween);
LLM_DEFINE_TAG(FDUETweenPoolTag);

void FDUETweenPool::InitTweenPool()
{
	ClearPool(TweenPool);

	check(GetDefault<UDUETweenSettings>()->InitialTweenPoolSize <= GetDefault<UDUETweenSettings>()->MaxTweenPoolSize)

	CurrentTotalPoolSize = GetDefault<UDUETweenSettings>()->InitialTweenPoolSize;

	TweenPool = ReallocatePool(nullptr, CurrentTotalPoolSize);

	for (int i = 0; i < CurrentTotalPoolSize; ++i)
	{
		int nextIndex = i + 1;
		TweenPool[i].TweenPtr.NextFreeTween = i != (CurrentTotalPoolSize - 1)
												  ? FActiveDUETweenHandle(nextIndex, nullptr, 0)
												  : FActiveDUETweenHandle::NULL_HANDLE();
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
		TweenPool[i].Handle = FActiveDUETweenHandle(i, nullptr, 0);
		// Memzeroing this because Tween Data is move only (due to tfunction) and we need to avoid garbage
		FMemory::Memzero(&TweenPool[i].TweenData, sizeof(FDUETweenData));
	}

	NextAvailableTween = TweenPool[0].Handle;

	SET_DWORD_STAT(STAT_POOLED_TWEENS, CurrentTotalPoolSize);
	SET_DWORD_STAT(STAT_TWEEN_POOL_SIZE, CurrentTotalPoolSize);
}

void FDUETweenPool::ClearPool(FActiveDUETween*& Pool)
{
	LLM_SCOPE_BYTAG(FDUETweenPoolTag);
	for (int i = 0; i < CurrentTotalPoolSize; ++i)
	{
		// Need to do this so tfunctions release their memory
		Pool[i].TweenData = FDUETweenData();
	}
	if (Pool != nullptr)
	{
		FMemory::Free(Pool);
	}
	Pool = nullptr;
}

void FDUETweenPool::Deinitialize()
{
	ClearPool(TweenPool);
}

FActiveDUETween* FDUETweenPool::ReallocatePool(FActiveDUETween* OldPool, const int& NewPoolSize)
{
	LLM_SCOPE_BYTAG(FDUETweenPoolTag);
	return static_cast<FActiveDUETween*>(FMemory::Realloc(OldPool, NewPoolSize * sizeof(FActiveDUETween)));
}

void FDUETweenPool::ExpandPool(const int& Amount)
{
	DECLARE_CYCLE_STAT(TEXT("ExpandPool"), STAT_ExpandPool, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_ExpandPool);
	const int MaxPoolSize = GetDefault<UDUETweenSettings>()->MaxTweenPoolSize;
	if (CurrentTotalPoolSize == MaxPoolSize)
	{
		UE_LOG(LogDUETween, Warning,
		       TEXT("Tween Pool Has Already Reached Max Size: %d"), MaxPoolSize);
		return;
	}

	const int OldTweenPoolSize = CurrentTotalPoolSize;
	FActiveDUETween* OldTweenPool = TweenPool;

	CurrentTotalPoolSize = FMath::Min(CurrentTotalPoolSize + Amount, MaxPoolSize);
	UE_LOG(LogDUETween, Verbose,
	       TEXT("Reallocating: %d"), CurrentTotalPoolSize * static_cast<int>(sizeof(FActiveDUETween)));
	TweenPool = ReallocatePool(OldTweenPool, CurrentTotalPoolSize);

	for (int i = OldTweenPoolSize; i < CurrentTotalPoolSize; ++i)
	{
		int nextIndex = i + 1;
		TweenPool[i].TweenPtr.NextFreeTween = i != (CurrentTotalPoolSize - 1)
			                                      ? FActiveDUETweenHandle(nextIndex, nullptr, 0)
			                                      : FActiveDUETweenHandle::NULL_HANDLE();
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
		TweenPool[i].Handle = FActiveDUETweenHandle(i, nullptr, 0);
		// Memzeroing this because Tween Data is move only (due to tfunction) and we need to avoid garbage
		FMemory::Memzero(&TweenPool[i].TweenData, sizeof(FDUETweenData));
	}
	TweenPool[OldTweenPoolSize - 1].TweenPtr.NextFreeTween = FActiveDUETweenHandle(OldTweenPoolSize, nullptr, 0);


	if (NextAvailableTween == nullptr)
	{
		NextAvailableTween = TweenPool[OldTweenPoolSize].Handle;
	}
	INC_DWORD_STAT_BY(STAT_POOLED_TWEENS, CurrentTotalPoolSize - OldTweenPoolSize);
	SET_DWORD_STAT(STAT_TWEEN_POOL_SIZE, CurrentTotalPoolSize);
}

FActiveDUETween* FDUETweenPool::GetTweenFromHandle(const FActiveDUETweenHandle TweenHandle,
                                                   const bool& CheckVersion) const
{
	if (TweenHandle == nullptr || TweenHandle.HandleIndex >= CurrentTotalPoolSize)
	{
		UE_LOGFMT(LogDUETween, Error, "Trying to access invalid tween",
		          TweenHandle.HandleIndex, TweenHandle.Version);
		return nullptr;
	}
	if (CheckVersion && TweenPool[TweenHandle.HandleIndex].Handle.Version != TweenHandle.Version)
	{
		UE_LOGFMT(LogDUETween, Error, "Trying to access tween {0} with outdated handle verion {1}",
		          TweenHandle.HandleIndex, TweenHandle.Version);
		return nullptr;
	}
	return &TweenPool[TweenHandle.HandleIndex];
}

FActiveDUETweenHandle FDUETweenPool::GetTweenFromPool(const TWeakObjectPtr<UWorld>& World = nullptr)
{
	DECLARE_CYCLE_STAT(TEXT("GetTweenFromPool"), STAT_GetTweenFromPool, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_GetTweenFromPool);

	FActiveDUETweenHandle HandleFromPool = NextAvailableTween;
	if (NextAvailableTween == nullptr)
	{
		UE_LOG(LogDUETween, Error,
		       TEXT("Unable to find available tween in pool"));
		return FActiveDUETweenHandle::NULL_HANDLE();
	}
	UE_LOG(LogDUETween, Verbose, TEXT("Retrieved Tween: %u from pool"),
	       GetTweenFromHandle(NextAvailableTween, false)->ID);

	NextAvailableTween = GetTweenFromHandle(NextAvailableTween, false)->TweenPtr.NextFreeTween;

	if (GetTweenFromHandle(NextAvailableTween, false)->TweenPtr.NextFreeTween == nullptr)
	{
		// If there's only 1 tween left, expand the pool
		ExpandPool(GetDefault<UDUETweenSettings>()->PoolExpansionIncrement);
	}

	DEC_DWORD_STAT(STAT_POOLED_TWEENS);

	// Flip Version back to positive as we send it out
	HandleFromPool.Version = FMath::Abs(HandleFromPool.Version);

	check(HandleFromPool.Version != INT_MAX);

	HandleFromPool.Version += 1;

	HandleFromPool.BoundWorld = World;

	GetTweenFromHandle(HandleFromPool, false)->Handle = HandleFromPool;

	return HandleFromPool;
}

void FDUETweenPool::ReturnTweenToPool(FActiveDUETweenHandle TweenToReturnHandle)
{
	FActiveDUETween* TweenToReturn = GetTweenFromHandle(TweenToReturnHandle, true);
	DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool"), STAT_ReturnTweenToPool, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool);
	UE_LOG(LogDUETween, Verbose, TEXT("Returning Tween: %u to pool"), TweenToReturn->ID);
	// add to free list
	{
		DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool_AddToFreeList"), STAT_ReturnTweenToPool_AddToFreeList,
		                   STATGROUP_DUETween);
		SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool_AddToFreeList);
		TweenToReturn->Status = EDUETweenStatus::Unset;
		TweenToReturn->TweenData.UpdateCallback = nullptr;
		TweenToReturn->TweenPtr.NextFreeTween = NextAvailableTween;
		// Need to do this so tfunctions release their memory
		TweenToReturn->TweenData = FDUETweenData();
		// Flip version negative to indicate that it's been freed
		TweenToReturn->Handle.Version = FMath::Abs(TweenToReturn->Handle.Version) * -1;
		TweenToReturn->Handle.BoundWorld = nullptr;
		NextAvailableTween = TweenToReturn->Handle;

		INC_DWORD_STAT(STAT_POOLED_TWEENS);
	}
}

int FDUETweenPool::GetCurrentPoolSize() const
{
	return CurrentTotalPoolSize;
}
