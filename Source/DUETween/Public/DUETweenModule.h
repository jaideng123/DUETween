#pragma once

#include "CoreMinimal.h"
#include "DUEEasingFunctionLibrary.h"
#include "ActiveDUETween.h"
#include "Containers/Union.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDUETween, Log, All);
DECLARE_STATS_GROUP(TEXT("DUETween"), STATGROUP_DUETween, STATCAT_Advanced);


/**
 * Main Module for Due Tween
 */
class FDUETweenModule : public IModuleInterface
{
public:
	// Get active DUETween Module Instance
	static FDUETweenModule& Get();
	
	// IModuleInterface Methods
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
