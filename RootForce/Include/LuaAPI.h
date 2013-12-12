<<<<<<< HEAD
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
			// Allocate memory for a pointer to to object
			ECS::Entity **s = (ECS::Entity**)lua_newuserdata(p_luaState, sizeof(ECS::Entity*));
			*s = g_world->GetEntityManager()->CreateEntity();

			//Userdata already on stack
			return 1;
		}

		static int CreateRenderable(lua_State* p_luaState)
		{
			// Allocate memory for a pointer to to object
			RootForce::Renderable **s = (RootForce::Renderable **)lua_newuserdata(p_luaState, sizeof(RootForce::Renderable *));
			
			ECS::Entity** e = (ECS::Entity**)lua_touserdata(p_luaState, 1);
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(*e);

			luaL_setmetatable(p_luaState, "Renderable");
			//Userdata already on stack
			return 1;
		}

		static int CreateTransformation(lua_State* p_luaState)
		{
			// Allocate memory for a pointer to to object
			RootForce::Transform **s = (RootForce::Transform **)lua_newuserdata(p_luaState, sizeof(RootForce::Transform *));
			
			ECS::Entity** e = (ECS::Entity**)lua_touserdata(p_luaState, 1);
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Transform>(*e);

			luaL_setmetatable(p_luaState, "Transformation");
			//Userdata already on stack
			return 1;
		}

		static int CreatePhysicsAccessor(lua_State* p_luaState)
		{
			// Allocate memory for a pointer to to object
			RootForce::PhysicsAccessor **s = (RootForce::PhysicsAccessor **)lua_newuserdata(p_luaState, sizeof(RootForce::PhysicsAccessor *));

			ECS::Entity** e = (ECS::Entity**)lua_touserdata(p_luaState, 1);
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::PhysicsAccessor>(*e);

			luaL_setmetatable(p_luaState, "PhysicsAccessor");

			//Userdata already on stack
			return 1;
		}

		static int SetRenderableModel(lua_State* p_luaState)
		{
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");

			std::string handle = lua_tostring(p_luaState, 2);
			std::string effecthandle = lua_tostring(p_luaState, 3);
			
			(*rtemp)->m_model = g_engineContext.m_resourceManager->GetModel(handle);
			(*rtemp)->m_material.m_diffuseMap = g_engineContext.m_resourceManager->GetTexture((*rtemp)->m_model->m_textureHandles[0]);
			(*rtemp)->m_material.m_specularMap = g_engineContext.m_resourceManager->GetTexture((*rtemp)->m_model->m_textureHandles[1]);
			(*rtemp)->m_material.m_normalMap = g_engineContext.m_resourceManager->GetTexture((*rtemp)->m_model->m_textureHandles[2]);
			(*rtemp)->m_material.m_effect = g_engineContext.m_resourceManager->GetEffect(effecthandle);
			return 0;
		}

		static int SetRenderableMaterial(lua_State* p_luaState)
		{
			return 0;
		}

		static int SetInfo(lua_State* p_luaState)
		{
			RootForce::PhysicsAccessor** ptemp = (RootForce::PhysicsAccessor**)luaL_checkudata(p_luaState, 1, "PhysicsAccessor");

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
	
			(*ptemp)->m_handle = g_engineContext.m_physics->AddAbilityToWorld(info);
			return 0;
		}
		//Entity functions
		static const struct luaL_Reg entity_f [] = {
			{"New", CreateEntity},
			{NULL, NULL}
		};
		//Renderable functions&methods
		static const struct luaL_Reg renderable_f [] = {
			{"New", CreateRenderable},
			{NULL, NULL}
		};

		static const struct luaL_Reg renderable_m [] = {
			{"SetModel", SetRenderableModel},
			{"SetMaterial", SetRenderableMaterial},
			{NULL, NULL}
		};
		//Transform functions & methods
		static const struct luaL_Reg transformation_f [] = {
			{"New", CreateTransformation},
			{NULL, NULL}
		};
		
		static const struct luaL_Reg transformation_m [] = {
			{"SetModel", SetRenderableModel},
			{"SetMaterial", SetRenderableMaterial},
			{NULL, NULL}
		};
		//Physics function & methods
		static const struct luaL_Reg physicsaccessor_f [] = {
			{"New", CreatePhysicsAccessor},
			{NULL, NULL}
		};

		static const struct luaL_Reg physicsaccessor_m [] = {
			{"SetInfo", SetInfo},
			{NULL, NULL}
		};

		static int LuaSetupType(lua_State* p_luaState, const luaL_Reg* p_funcReg, std::string p_typeName)
		{
			luaL_newlib(p_luaState, p_funcReg);
			lua_setglobal(p_luaState, p_typeName.c_str());

			return 1;
		}

		static int LuaSetupTypeWithMethods(lua_State* p_luaState, const luaL_Reg* p_funcReg, const luaL_Reg* p_methodReg, std::string p_typeName)
		{
			luaL_newmetatable(p_luaState, p_typeName.c_str());
			int meta_id = lua_gettop(p_luaState);
		
			luaL_setfuncs(p_luaState, p_methodReg, 0);
			lua_setfield(p_luaState, meta_id, "__index");  

			luaL_newlib(p_luaState, p_funcReg);

			lua_setglobal(p_luaState, p_typeName.c_str());

			return 1;
		}
	}
}