// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DUETweenModule.h"
#include "DUETweenPool.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DUETweenSubsystem.generated.h"

/**
 * World Subsystem that manages and ticks tweens
 */
UCLASS()
class UDUETweenSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	// Creates and starts a new tween
	FActiveDUETweenHandle AddTween(FDUETweenData& TweenData);
	// Pauses a currently running tween (returns true if successful)
	bool PauseTween(const FActiveDUETweenHandle& TweenHandle) const;
	// Resumes a paused tween (returns true if successful)
	bool ResumeTween(const FActiveDUETweenHandle& TweenHandle) const;
	// Skips to the end of a tween (returns true if successful)
	bool FastForwardTween(const FActiveDUETweenHandle& TweenHandle) const;
	// Stops and ends tween (returns true if successful)
	bool StopTween(const FActiveDUETweenHandle& TweenHandle) const;
	// Stops and cancels all tweens on a given target
	void StopAllTweens(TWeakObjectPtr<UObject> Object) const;
	// Gets a pointer to a tween from its handle DO NOT SAVE THIS POINTER
	FActiveDUETween* GetTweenFromHandle(const FActiveDUETweenHandle& TweenHandle) const;
	
	// Tick
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	// NOTE: Every tickable game object needs this or it will throw an exception
	virtual TStatId GetStatId() const override { return TStatId(); }

	void TickTween(float DeltaTime, FActiveDUETween* CurrentTween);

private:
	FDUETweenPool Pool = FDUETweenPool();
	void RemoveTweenFromActiveChain(FActiveDUETweenHandle TweenHandleToRemove);
	FActiveDUETweenHandle ActiveTweenChainStart = NULL_DUETWEEN_HANDLE;
	unsigned int LastAssignedTweenId = 0;
};
