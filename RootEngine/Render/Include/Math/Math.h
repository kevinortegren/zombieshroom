#pragma once
#include <cmath>

namespace RootEngine
{
	const float PI = (float)atan(1.0) * 4;
    const float E =  2.718281828f;
    
	float gauss( int x, float sigma );
	double gaussian (double x, double mu, double sigma);	

}
