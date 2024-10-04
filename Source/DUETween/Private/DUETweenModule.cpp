#include "DUETweenModule.h"

DEFINE_LOG_CATEGORY(LogDUETween);

void FDUETweenModule::StartupModule()
{
	UE_LOG(LogDUETween, Display, TEXT("Loaded DUETween Module"));
}

void FDUETweenModule::ShutdownModule()
{
	UE_LOG(LogDUETween, Display, TEXT("UnLoaded DUETween Module"));
}

FDUETweenModule& FDUETweenModule::Get()
{
	return FModuleManager::LoadModuleChecked<FDUETweenModule>("DUETween");
}

IMPLEMENT_MODULE(FDUETweenModule, DUETween)
