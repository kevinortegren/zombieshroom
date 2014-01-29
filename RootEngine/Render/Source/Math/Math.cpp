#include <RootEngine/Render/Include/Math/Math.h>

float RootEngine::gauss( int x, float sigma )
{
	return  powf( RootEngine::E, -(x*x)/(2*sigma*sigma) );
}

double RootEngine::gaussian (double x, double mu, double sigma) 
{
	return exp( -(((x-mu)/(sigma))*((x-mu)/(sigma)))/2.0 );
}