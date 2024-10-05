#pragma once
#include "ActiveDUETween.h"

LLM_DECLARE_TAG(FDUETweenPoolTag);

/**
 * Pool that manages a heap-allocated array of tweens
 */
class FDUETweenPool
{
public:
	// Tween Pool Methods
	void InitTweenPool();
	void ExpandPool(const int& Amount);
	FActiveDUETween* GetTweenFromHandle(FActiveDUETweenHandle NextAvailableTween) const;
	FActiveDUETweenHandle GetTweenFromPool();
	void ReturnTweenToPool(FActiveDUETweenHandle TweenToReturnHandle);
	int GetCurrentPoolSize() const;
	void Deinitialize();

private:
	TArray<int> Test;
	int CurrentTotalPoolSize = 0;
	int CurrentUnusedElements = 0;
	FActiveDUETween* TweenPool = nullptr;
	FActiveDUETweenHandle NextAvailableTween = NULL_DUETWEEN_HANDLE;
	void ClearPool(FActiveDUETween*& Pool);
	static FActiveDUETween* AllocateNewPool(const int& NewPoolSize);
	FActiveDUETween* ReallocatePool(FActiveDUETween* OldPool, const int& NewPoolSize);
};
