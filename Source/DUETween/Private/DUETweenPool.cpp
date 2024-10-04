#include "DUETweenPool.h"

#include "DUETweenModule.h"
#include "DUETweenSettings.h"

DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Pooled Tweens"), STAT_POOLED_TWEENS, STATGROUP_DUETween);
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Tween Pool Size"), STAT_TWEEN_POOL_SIZE, STATGROUP_DUETween);
LLM_DEFINE_TAG(FDUETweenPoolTag);

void FDUETweenPool::InitTweenPool()
{
	ClearPool(TweenPool);

	check(GetDefault<UDUETweenSettings>()->InitialTweenPoolSize <= GetDefault<UDUETweenSettings>()->MaxTweenPoolSize)

	CurrentTotalPoolSize = GetDefault<UDUETweenSettings>()->InitialTweenPoolSize;

	TweenPool = AllocateNewPool(CurrentTotalPoolSize);

	for (int i = 0; i < CurrentTotalPoolSize - 1; ++i)
	{
		TweenPool[i].TweenPtr.NextFreeTween = FActiveDUETweenHandle(i + 1);
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
		TweenPool[i].Handle = i;
	}
	TweenPool[CurrentTotalPoolSize - 1].TweenPtr.NextFreeTween = NULL_DUETWEEN_HANDLE;
	TweenPool[CurrentTotalPoolSize - 1].Status = EDUETweenStatus::Unset;
	TweenPool[CurrentTotalPoolSize - 1].ID = 0;
	TweenPool[CurrentTotalPoolSize - 1].Handle = CurrentTotalPoolSize - 1;

	NextAvailableTween = 0;

	SET_DWORD_STAT(STAT_POOLED_TWEENS, CurrentTotalPoolSize);
	SET_DWORD_STAT(STAT_TWEEN_POOL_SIZE, CurrentTotalPoolSize);
}

void FDUETweenPool::ClearPool(FActiveDUETween*& Pool)
{
	LLM_SCOPE_BYTAG(FDUETweenPoolTag);
	if (Pool != nullptr)
	{
		delete[] Pool;
	}
	Pool = nullptr;
}

void FDUETweenPool::Deinitialize()
{
	ClearPool(TweenPool);
}

FActiveDUETween* FDUETweenPool::AllocateNewPool(const int& NewPoolSize)
{
	LLM_SCOPE_BYTAG(FDUETweenPoolTag);
	return new FActiveDUETween[NewPoolSize];
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
	       TEXT("Allocating: %d"), CurrentTotalPoolSize * static_cast<int>(sizeof(FActiveDUETween)));
	TweenPool = AllocateNewPool(CurrentTotalPoolSize);

	// Copy old pool to new
	FMemory::Memcpy(TweenPool, OldTweenPool, sizeof(FActiveDUETween) * OldTweenPoolSize);

	// Delete the old pool
	UE_LOG(LogDUETween, Verbose,
	       TEXT("De-Allocating: %d"), OldTweenPoolSize * static_cast<int>(sizeof(FActiveDUETween)));
	ClearPool(OldTweenPool);

	TweenPool[OldTweenPoolSize - 1].TweenPtr.NextFreeTween = OldTweenPoolSize;
	for (int i = OldTweenPoolSize; i < CurrentTotalPoolSize - 1; ++i)
	{
		TweenPool[i].TweenPtr.NextFreeTween = i + 1;
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
		TweenPool[i].Handle = i;
	}
	TweenPool[CurrentTotalPoolSize - 1].TweenPtr.NextFreeTween = NULL_DUETWEEN_HANDLE;
	TweenPool[CurrentTotalPoolSize - 1].Status = EDUETweenStatus::Unset;
	TweenPool[CurrentTotalPoolSize - 1].ID = 0;
	TweenPool[CurrentTotalPoolSize - 1].Handle = CurrentTotalPoolSize - 1;
	if (NextAvailableTween == NULL_DUETWEEN_HANDLE)
	{
		NextAvailableTween = OldTweenPoolSize;
	}
	INC_DWORD_STAT_BY(STAT_POOLED_TWEENS, CurrentTotalPoolSize - OldTweenPoolSize);
	SET_DWORD_STAT(STAT_TWEEN_POOL_SIZE, CurrentTotalPoolSize);
}

FActiveDUETween* FDUETweenPool::GetTweenFromHandle(const FActiveDUETweenHandle TweenHandle) const
{
	if (TweenHandle == NULL_DUETWEEN_HANDLE || TweenHandle >= CurrentTotalPoolSize)
	{
		return nullptr;
	}
	return &TweenPool[TweenHandle];
}

FActiveDUETweenHandle FDUETweenPool::GetTweenFromPool()
{
	DECLARE_CYCLE_STAT(TEXT("GetTweenFromPool"), STAT_GetTweenFromPool, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_GetTweenFromPool);

	const FActiveDUETweenHandle HandleFromPool = NextAvailableTween;
	if (NextAvailableTween == NULL_DUETWEEN_HANDLE)
	{
		UE_LOG(LogDUETween, Error,
		       TEXT("Unable to find available tween in pool"));
		return NULL_DUETWEEN_HANDLE;
	}
	UE_LOG(LogDUETween, Verbose, TEXT("Retrieved Tween: %u from pool"), GetTweenFromHandle(NextAvailableTween)->ID);

	NextAvailableTween = GetTweenFromHandle(NextAvailableTween)->TweenPtr.NextFreeTween;

	if (GetTweenFromHandle(NextAvailableTween)->TweenPtr.NextFreeTween == NULL_DUETWEEN_HANDLE)
	{
		// If there's only 1 tween left, expand the pool
		ExpandPool(GetDefault<UDUETweenSettings>()->PoolExpansionIncrement);
	}

	DEC_DWORD_STAT(STAT_POOLED_TWEENS);
	return HandleFromPool;
}

void FDUETweenPool::ReturnTweenToPool(FActiveDUETweenHandle TweenToReturnHandle)
{
	FActiveDUETween* TweenToReturn = GetTweenFromHandle(TweenToReturnHandle);
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
		NextAvailableTween = TweenToReturn->Handle;

		INC_DWORD_STAT(STAT_POOLED_TWEENS);
	}
}

int FDUETweenPool::GetCurrentPoolSize() const
{
	return CurrentTotalPoolSize;
}
