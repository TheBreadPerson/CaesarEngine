#include <globals.hpp>

static float reMap(float x, float iMin, float iMax, float oMin, float oMax)
{
	return (x = iMin) * (oMax - oMin) / (iMax - iMin) + oMin;
}

namespace Time
{
	double deltaTime = 0.0;
	double lastFrame = 0.0;
	void Tick()
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
}
