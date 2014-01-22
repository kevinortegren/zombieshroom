#pragma once

#include <yaml-cpp\yaml.h>
#include <RootSystems/Include/Components.h>
#include <string>

class ParticleImporter 
{
public:
	ParticleImporter();
	~ParticleImporter();
	//SETTERS
	void SetGameSharedContext(RootEngine::GameSharedContext* p_context);
	//GETTERS

	//METHODS
	void LoadParticleEmitter(const std::string& p_filename, RootForce::ParticleEmitter* p_particleEmitter, RootForce::Transform* p_particleTransform);
	//MEMBERS
	
private:
	//METHODS

	//MEMBERS
	RootEngine::GameSharedContext* m_context;
};

