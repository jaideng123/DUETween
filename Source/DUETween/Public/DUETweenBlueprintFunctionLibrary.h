// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DUEEasingFunctionLibrary.h"
#include "DUETweenModule.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DUETweenBlueprintFunctionLibrary.generated.h"

struct FDUETweenData;
/**
 * Due Tween Function Library
 */
UCLASS()
class DUETWEEN_API UDUETweenBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Run a tween to move an actor/component from it's current location to a target location
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueMove(UObject* Target,
	                    FLatentActionInfo LatentInfo,
	                    float Duration,
	                    FVector TargetLocation,
	                    EDueEasingType DueEasingType,
	                    FActiveDUETweenHandle& OutHandle,
	                    int32 Steps = 0,
	                    int32 LoopCount = 0,
	                    const bool YoYo = false);

	// Run a tween to move a canvas panel slot from it's current location to a target location
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueMove2D(UObject* Target,
							   FLatentActionInfo LatentInfo,
							   float Duration,
							   FVector2D TargetValue,
							   EDueEasingType DueEasingType,
							   FActiveDUETweenHandle& OutHandle,
							   int32 Steps = 0,
							   int32 LoopCount = 0,
							   const bool YoYo = false);
	
	// Run a tween that will change the angle of a widget from it's current value to a target rotation
	UFUNCTION(BlueprintCallable, Category = "DUETween",
	meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueRotate2D(UObject* Target,
	                        FLatentActionInfo LatentInfo,
	                        float Duration, float TargetValue,
	                        EDueEasingType DueEasingType,
	                        FActiveDUETweenHandle& OutHandle,
	                        int32 Steps,
	                        int32 LoopCount,
	                        bool YoYo);

	// Run a tween to rotate an actor/component from it's current rotation to a target rotation
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueRotate(UObject* Target,
	                      FLatentActionInfo LatentInfo,
	                      float Duration,
	                      FRotator TargetRotation,
	                      EDueEasingType DueEasingType,
	                      FActiveDUETweenHandle& OutHandle,
	                      int32 Steps = 0,
	                      int32 LoopCount = 0,
	                      const bool YoYo = false);

	// Run a tween that will change a float property from it's current value to the target value
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueFloatField(UObject* Target,
	                          FLatentActionInfo LatentInfo,
	                          FName FieldName,
	                          float Duration,
	                          float TargetValue,
	                          EDueEasingType DueEasingType,
	                          FActiveDUETweenHandle& OutHandle,
	                          int32 Steps = 0,
	                          int32 LoopCount = 0,
	                          const bool YoYo = false);

	// Run a tween that will change a double property from it's current value to the target value
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueDoubleField(UObject* Target,
	                           FLatentActionInfo LatentInfo,
	                           FName FieldName,
	                           float Duration,
	                           double TargetValue,
	                           EDueEasingType DueEasingType,
	                           FActiveDUETweenHandle& OutHandle,
	                           int32 Steps = 0,
	                           int32 LoopCount = 0,
	                           const bool YoYo = false);

	// Run a tween that will change a vector property from it's current value to the target value
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueVectorField(UObject* Target,
	                           FLatentActionInfo LatentInfo,
	                           FName FieldName,
	                           float Duration,
	                           FVector TargetValue,
	                           EDueEasingType DueEasingType,
	                           FActiveDUETweenHandle& OutHandle,
	                           int32 Steps = 0,
	                           int32 LoopCount = 0,
	                           const bool YoYo = false);

	// Run a tween that will change a rotator property from it's current value to the target value
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueRotatorField(UObject* Target,
	                            FLatentActionInfo LatentInfo,
	                            FName FieldName,
	                            float Duration,
	                            FRotator TargetValue,
	                            EDueEasingType DueEasingType,
	                            FActiveDUETweenHandle& OutHandle,
	                            int32 Steps = 0,
	                            int32 LoopCount = 0,
	                            const bool YoYo = false);
	
	// Run a tween that will change a vector2d property from it's current value to the target value
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueVector2DField(UObject* Target,
							   FLatentActionInfo LatentInfo,
							   FName FieldName,
							   float Duration,
							   FVector2D TargetValue,
							   EDueEasingType DueEasingType,
							   FActiveDUETweenHandle& OutHandle,
							   int32 Steps = 0,
							   int32 LoopCount = 0,
							   const bool YoYo = false);

	// Pause an actively running tween
	UFUNCTION(BlueprintCallable, Category = "DUETween", meta = (DefaultToSelf="Target"))
	static void PauseDUETween(FActiveDUETweenHandle DUETweenHandle);
	
	// Resume a paused tween
	UFUNCTION(BlueprintCallable, Category = "DUETween", meta = (DefaultToSelf="Target"))
	static void ResumeDUETween(FActiveDUETweenHandle DUETweenHandle);

	// Fast-Forward a tween to it's end state
	UFUNCTION(BlueprintCallable, Category = "DUETween", meta = (DefaultToSelf="Target"))
	static void FastForwardDUETween(FActiveDUETweenHandle DUETweenHandle);

	// Stop and cancel a tween
	UFUNCTION(BlueprintCallable, Category = "DUETween", meta = (DefaultToSelf="Target"))
	static void StopDUETween(FActiveDUETweenHandle DUETweenHandle);

	UFUNCTION(BlueprintCallable, Category = "DUETween")
	static bool IsDUETweenHandleNull(FActiveDUETweenHandle Handle);

private:
	static FActiveDUETweenHandle CreateAndStartLatentAction(UObject* Target,
	                                                        const FLatentActionInfo& LatentInfo, const FActiveDUETweenHandle& TweenHandle);
};
