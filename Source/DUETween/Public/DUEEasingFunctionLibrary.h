#pragma once
#include "Kismet/KismetMathLibrary.h"

// Easing Types mapping to various functions
// Taken from https://easings.net/
UENUM()
enum EDueEasingType : int
{
	// Linear Interp
	Linear,
	// Sinusoidal Interps
	InSin,
	OutSin,
	InOutSin,
	// Quad (x^2) Interps
	InQuad,
	OutQuad,
	InOutQuad,
	// Cubic (x^3) Interps
	InCubic,
	OutCubic,
	InOutCubic,
	// Quart (x^4) Interps
	InQuart,
	OutQuart,
	InOutQuart,
	// Quint (x^5) Interps
	InQuint,
	OutQuint,
	InOutQuint,
	// Exponential (2^x) Interps
	InExpo,
	OutExpo,
	InOutExpo,
	// Circular (sqrt) Interps
	InCircular,
	OutCircular,
	InOutCircular,
	// Elastic Interps
	InElastic,
	OutElastic,
	InOutElastic,
	// In Back Interps
	InBack,
	OutBack,
	InOutBack,
	// Bounce Interps
	InBounce,
	OutBounce,
	InOutBounce
};

/**
 * Library of easing functions used by DUETween
 */
class DUEEasingFunctionLibrary
{
public:
	static double Ease(const double& Begin, const double& End, const double& Alpha, const EDueEasingType& Easing, const int32& Steps = 0);

	// Easing Functions
	static double EaseInSin(const double& Alpha);
	static double EaseOutSin(const double& Alpha);
	static double EaseInOutSin(const double& Alpha);
	static double EaseInQuad(const double& Alpha);
	static double EaseOutQuad(const double& Alpha);
	static double EaseInOutQuad(const double& Alpha);
	static double EaseInCubic(const double& Alpha);
	static double EaseOutCubic(const double& Alpha);
	static double EaseInOutCubic(const double& Alpha);
	static double EaseInQuart(const double& Alpha);
	static double EaseOutQuart(const double& Alpha);
	static double EaseInOutQuart(const double& Alpha);
	static double EaseInQuint(const double& Alpha);
	static double EaseOutQuint(const double& Alpha);
	static double EaseInOutQuint(const double& Alpha);
	static double EaseInExpo(const double& Alpha);
	static double EaseOutExpo(const double& Alpha);
	static double EaseInOutExpo(const double& Alpha);
	static double EaseInCirc(const double& Alpha);
	static double EaseOutCirc(const double& Alpha);
	static double EaseInOutCirc(const double& Alpha);
	static double EaseInElastic(const double& Alpha);
	static double EaseOutElastic(const double& Alpha);
	static double EaseInOutElastic(const double& Alpha);
	static double EaseInBack(const double& Alpha);
	static double EaseOutBack(const double& Alpha);
	static double EaseInOutBack(const double& Alpha);
	static double EaseInBounce(const double& Alpha);
	static double EaseOutBounce(const double& Alpha);
	static double EaseInOutBounce(const double& Alpha);
};
