
#include "GameInfo.h"

// 180�� = PI

float RadianToDegree(float Radian)
{
	return Radian / PI * 180.f;
}

float DegreeToRadian(float Degree)
{
	return Degree / 180.f * PI;
}
