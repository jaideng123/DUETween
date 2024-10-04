#pragma once
#include "ActiveDUETween.h"
#include "DUETweenModule.h"


/**
 * Internal utilities for tweening
 */
class FDUETweenInternalUtils
{
public:
	static FValueContainer GetCurrentValueFromProperty(const FDUETweenData& TweenData);
	static void SetProperty(const FDUETweenData& TweenData, const FValueContainer& NewValue);
	static void SetCurrentValue(const FDUETweenData& TweenData, const FValueContainer& NewValue);
};
