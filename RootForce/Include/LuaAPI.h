#pragma once
#include <Lua/lua.hpp>
#include <Utility/ECS/Include/World.h>
#include <Utility/ECS/Include/Entity.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/Transform.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;

namespace RootForce
{
	namespace LuaAPI
	{
		static int CreateEntity(lua_State* p_luaState)
		{
			lua_pushlightuserdata(p_luaState, g_world->GetEntityManager()->CreateEntity());
			return 1;
		}

		static int CreateRenderable(lua_State* p_luaState)
		{
			lua_pushlightuserdata(p_luaState, g_world->GetEntityManager()->CreateComponent<RootForce::Renderable>((ECS::Entity*)lua_topointer(p_luaState, 1)));
			return 1;
		}

		static int CreateTransformation(lua_State* p_luaState)
		{
			lua_pushlightuserdata(p_luaState, g_world->GetEntityManager()->CreateComponent<RootForce::Transform>((ECS::Entity*)lua_topointer(p_luaState, 1)));
			return 1;
		}

		static int CreatePhysicsAccessor(lua_State* p_luaState)
		{
			lua_pushlightuserdata(p_luaState, g_world->GetEntityManager()->CreateComponent<RootForce::PhysicsAccessor>((ECS::Entity*)lua_topointer(p_luaState, 1)));
			return 1;
		}

		static int SetRenderableModel(lua_State* p_luaState)
		{
			std::string handle = lua_tostring(p_luaState, 1);
			std::string effecthandle = lua_tostring(p_luaState, 2);
			RootForce::Renderable* rtemp = (RootForce::Renderable*)lua_topointer(p_luaState, 3);
			rtemp->m_model = g_engineContext.m_resourceManager->GetModel(handle);
			rtemp->m_material.m_diffuseMap = g_engineContext.m_resourceManager->GetTexture(rtemp->m_model->m_textureHandles[0]);
			rtemp->m_material.m_specularMap = g_engineContext.m_resourceManager->GetTexture(rtemp->m_model->m_textureHandles[1]);
			rtemp->m_material.m_normalMap = g_engineContext.m_resourceManager->GetTexture(rtemp->m_model->m_textureHandles[2]);
			rtemp->m_material.m_effect = g_engineContext.m_resourceManager->GetEffect(effecthandle);
			return 0;
		}

		static int SetRenderableMaterial(lua_State* p_luaState)
		{
			return 0;
		}

		static int SetPhysicsAccessorInfo(lua_State* p_luaState)
		{
			RootForce::PhysicsAccessor* ptemp  = (RootForce::PhysicsAccessor*)lua_topointer(p_luaState, 1);
			RootEngine::Physics::AbilityPhysicsInfo info;
			bool collide = lua_toboolean(p_luaState, 2) == 1 ? true : false; 
			info.m_collidesWorld	= collide;
			float direction[]		= {(float)lua_tonumber(p_luaState, 3), (float)lua_tonumber(p_luaState, 4), (float)lua_tonumber(p_luaState, 5)};
			float gravity[]			= {(float)lua_tonumber(p_luaState, 6), (float)lua_tonumber(p_luaState, 7), (float)lua_tonumber(p_luaState, 8)};
			float orientation[]		= {(float)lua_tonumber(p_luaState, 9), (float)lua_tonumber(p_luaState, 10), (float)lua_tonumber(p_luaState, 11), (float)lua_tonumber(p_luaState, 12)};
			float position[]		= {(float)lua_tonumber(p_luaState, 13), (float)lua_tonumber(p_luaState, 14), (float)lua_tonumber(p_luaState, 15)};
			info.m_direction		= direction;
			info.m_entityId			= 0;
			info.m_gravity			= gravity;
			info.m_height			= (float)lua_tonumber(p_luaState, 16);
			info.m_mass				= (float)lua_tonumber(p_luaState, 17);
			info.m_orientation		= orientation;
			info.m_position			= position;
			info.m_radius			= (float)lua_tonumber(p_luaState, 18);
			info.m_shape			= (RootEngine::Physics::AbilityShape::AbilityShape)((int)lua_tonumber(p_luaState, 19));
			info.m_speed			= (float)lua_tonumber(p_luaState, 20);
			info.m_type				= (RootEngine::Physics::PhysicsType::PhysicsType)((int)lua_tonumber(p_luaState, 21));
	
			ptemp->m_handle = g_engineContext.m_physics->AddAbilityToWorld(info);
			return 0;
		}
	}
}
