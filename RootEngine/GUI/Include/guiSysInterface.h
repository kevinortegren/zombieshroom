#pragma once


#include <Rocket\Core.h>
#include <Rocket\Core\SystemInterface.h>

class guiSysInterface : public Rocket::Core::SystemInterface
{
public:
	float GetElapsedTime() { return m_deltaTime; }

	void SetElapsedTime(float p_deltaTime) {m_deltaTime = p_deltaTime;}

private:
	float m_deltaTime;

};

