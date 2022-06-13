
#include "GameInfo.h"

// 180µµ = PI

float RadianToDegree(float Radian)
{
	return Radian / PI * 180.f;
}

float DegreeToRadian(float Degree)
{
	return Degree / 180.f * PI;
}
