#pragma once

#include <RootEngine/Include/GameSharedContext.h>

RootEngine::GameSharedContext g_engineContext;

class TreenityMain
{
public:
	TreenityMain(const std::string& p_path);
	~TreenityMain();


	void Update(float dt);

private:
	void* m_engineModule;

};