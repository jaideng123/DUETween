﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DUETweenBlueprintFunctionLibrary.h"

#include "DUETween.h"
#include "DUETweenSubsystem.h"
#include "DUETweenAction.h"
#include "Components/CanvasPanelSlot.h"

void UDUETweenBlueprintFunctionLibrary::DueMove(UObject* Target,
                                                const FLatentActionInfo LatentInfo,
                                                const float Duration,
                                                const FVector TargetLocation,
                                                const EDueEasingType DueEasingType,
                                                int& OutHandle,
                                                const int32 Steps)
{
	FActiveDUETweenHandle Handle = DUETween::DUEMove(Target, TargetLocation, Duration, DueEasingType, Steps);
	OutHandle = CreateAndStartLatentAction(Target, LatentInfo, Handle);
}

void UDUETweenBlueprintFunctionLibrary::DueMove2D(UObject* Target,
                                                  const FLatentActionInfo LatentInfo,
                                                  const float Duration,
                                                  FVector2D TargetValue,
                                                  const EDueEasingType DueEasingType,
                                                  int& OutHandle,
                                                  const int32 Steps)
{
	FActiveDUETweenHandle Handle = DUETween::DUEMove2D(Target, TargetValue, Duration, DueEasingType, Steps);
	OutHandle = CreateAndStartLatentAction(Target, LatentInfo, Handle);
}

void UDUETweenBlueprintFunctionLibrary::DueRotate(UObject* Target,
                                                  const FLatentActionInfo LatentInfo,
                                                  const float Duration,
                                                  const FRotator TargetRotation,
                                                  const EDueEasingType DueEasingType,
                                                  int& OutHandle,
                                                  const int32 Steps)
{
	FActiveDUETweenHandle Handle = DUETween::DUERotate(Target, TargetRotation, Duration, DueEasingType, Steps);
	OutHandle = CreateAndStartLatentAction(Target, LatentInfo, Handle);
}

void UDUETweenBlueprintFunctionLibrary::DueFloatField(UObject* Target,
                                                      const FLatentActionInfo LatentInfo,
                                                      const FName FieldName,
                                                      const float Duration,
                                                      const float TargetValue,
                                                      const EDueEasingType DueEasingType,
                                                      int& OutHandle,
                                                      const int32 Steps)
{
	const FActiveDUETweenHandle Handle = DUETween::StartDUETween(Target, FieldName, TargetValue, Duration,
	                                                             DueEasingType, Steps);
	OutHandle = CreateAndStartLatentAction(Target, LatentInfo, Handle);
}

void UDUETweenBlueprintFunctionLibrary::DueDoubleField(UObject* Target,
                                                       const FLatentActionInfo LatentInfo,
                                                       const FName FieldName,
                                                       const float Duration,
                                                       const double TargetValue,
                                                       const EDueEasingType DueEasingType,
                                                       int& OutHandle,
                                                       const int32 Steps)
{
	const FActiveDUETweenHandle Handle = DUETween::StartDUETween(Target, FieldName, TargetValue, Duration,
	                                                             DueEasingType, Steps);
	OutHandle = CreateAndStartLatentAction(Target, LatentInfo, Handle);
}

void UDUETweenBlueprintFunctionLibrary::DueVectorField(UObject* Target,
                                                       const FLatentActionInfo LatentInfo,
                                                       const FName FieldName,
                                                       const float Duration,
                                                       const FVector TargetValue,
                                                       const EDueEasingType DueEasingType,
                                                       int& OutHandle,
                                                       const int32 Steps)
{
	const FActiveDUETweenHandle Handle = DUETween::StartDUETween(Target, FieldName, TargetValue, Duration,
	                                                             DueEasingType, Steps);
	OutHandle = CreateAndStartLatentAction(Target, LatentInfo, Handle);
}

void UDUETweenBlueprintFunctionLibrary::DueRotatorField(UObject* Target,
                                                        const FLatentActionInfo LatentInfo,
                                                        const FName FieldName,
                                                        const float Duration,
                                                        const FRotator TargetValue,
                                                        const EDueEasingType DueEasingType,
                                                        int& OutHandle,
                                                        const int32 Steps)
{
	const FActiveDUETweenHandle Handle = DUETween::StartDUETween(Target, FieldName, TargetValue, Duration,
	                                                             DueEasingType, Steps);
	OutHandle = CreateAndStartLatentAction(Target, LatentInfo, Handle);
}

void UDUETweenBlueprintFunctionLibrary::DueVector2DField(UObject* Target,
                                                         FLatentActionInfo LatentInfo, FName FieldName,
                                                         float Duration,
                                                         FVector2D TargetValue,
                                                         EDueEasingType DueEasingType,
                                                         int& OutHandle,
                                                         int32 Steps)
{
	const FActiveDUETweenHandle Handle = DUETween::StartDUETween(Target, FieldName, TargetValue, Duration,
	                                                             DueEasingType, Steps);
	OutHandle = CreateAndStartLatentAction(Target, LatentInfo, Handle);
}

void UDUETweenBlueprintFunctionLibrary::PauseDUETween(UObject* Target, const int& DUETweenHandle, bool& Success)
{
	Success = DUETween::PauseDUETween(Target, DUETweenHandle);
}

void UDUETweenBlueprintFunctionLibrary::ResumeDUETween(UObject* Target, const int& DUETweenHandle, bool& Success)
{
	Success = DUETween::ResumeDUETween(Target, DUETweenHandle);
}

void UDUETweenBlueprintFunctionLibrary::FastForwardDUETween(UObject* Target, const int& DUETweenHandle, bool& Success)
{
	Success = DUETween::FastForwardDUETween(Target, DUETweenHandle);
}

void UDUETweenBlueprintFunctionLibrary::StopDUETween(UObject* Target, const int& DUETweenHandle, bool& Success)
{
	Success = DUETween::StopDUETween(Target, DUETweenHandle);
}


FActiveDUETweenHandle UDUETweenBlueprintFunctionLibrary::CreateAndStartLatentAction(
	UObject* Target, const FLatentActionInfo& LatentInfo,
	const FActiveDUETweenHandle& TweenHandle)
{
	if (TweenHandle == NULL_DUETWEEN_HANDLE)
	{
		UE_LOG(LogDUETween, Warning, TEXT("Unable to start due tween with UUID: %d"), LatentInfo.UUID);
		return NULL_DUETWEEN_HANDLE;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FDUETweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
			nullptr)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
			                                 new FDUETweenAction(LatentInfo, TweenHandle));
			UE_LOG(LogDUETween, Verbose, TEXT("Starting latent due tween action with UUID: %d"), LatentInfo.UUID);
			return TweenHandle;
		}
	}
	UE_LOG(LogDUETween, Warning, TEXT("Unable to start due tween latent action with UUID: %d"), LatentInfo.UUID);
	DUETween::StopDUETween(Target, TweenHandle);
	return NULL_DUETWEEN_HANDLE;
}
