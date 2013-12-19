 #pragma once
#include <Lua/lua.hpp>
#include <glm/glm.hpp>
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
		#define NumberOfArgs(n) CheckNrOfArgs(p_luaState, n, __FUNCTION__, __LINE__)

		void CheckNrOfArgs(lua_State* p_luaState, int p_expectedNrOfArgs, std::string p_func, int p_line)
		{
			int nargs = lua_gettop(p_luaState);
			if(nargs > p_expectedNrOfArgs)
			{
				g_engineContext.m_logger->LogText(LogTag::SCRIPT, LogLevel::NON_FATAL_ERROR, "Lua Error: Too many arguments when calling C function %s at line %s!", p_func.c_str(), std::to_string(p_line).c_str());
			}
			else if(nargs < p_expectedNrOfArgs)
			{
				g_engineContext.m_logger->LogText(LogTag::SCRIPT, LogLevel::FATAL_ERROR, "Lua Error: Too few arguments when calling C function %s at line %s!", p_func.c_str(), std::to_string(p_line).c_str());
				luaL_argerror(p_luaState, nargs, "Too few arguments!");
			}
		}

		static int CreateEntity(lua_State* p_luaState)
		{
			NumberOfArgs(0);
			ECS::Entity **s = (ECS::Entity**)lua_newuserdata(p_luaState, sizeof(ECS::Entity*));
			*s = g_world->GetEntityManager()->CreateEntity();
			luaL_setmetatable(p_luaState, "Entity");
			return 1;
		}

		static int GetEntityByTag(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			ECS::Entity **s = (ECS::Entity**)lua_newuserdata(p_luaState, sizeof(ECS::Entity*));
			*s = g_world->GetTagManager()->GetEntityByTag(luaL_checkstring(p_luaState, 1));
			luaL_setmetatable(p_luaState, "Entity");
			return 1;
		}
		
		static int GetId(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			lua_pushnumber(p_luaState, (*e)->GetId());
			return 1;
		}

		static int GetTransformation(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform **s = (RootForce::Transform **)lua_newuserdata(p_luaState, sizeof(RootForce::Transform *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->GetComponent<RootForce::Transform>(*e);
			luaL_setmetatable(p_luaState, "Transformation");
			return 1;
		}

		static int CreateRenderable(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Renderable **s = (RootForce::Renderable **)lua_newuserdata(p_luaState, sizeof(RootForce::Renderable *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(*e);
			luaL_setmetatable(p_luaState, "Renderable");
			return 1;
		}

		static int CreateTransformation(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform **s = (RootForce::Transform **)lua_newuserdata(p_luaState, sizeof(RootForce::Transform *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Transform>(*e);
			luaL_setmetatable(p_luaState, "Transformation");
			return 1;
		}

		static int GetPos(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->m_position);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int SetPos(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			(*ptemp)->m_position = (*v1);
			return 0;
		}

		static int GetFront(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->m_orientation.GetFront());
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int GetUp(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->m_orientation.GetUp());
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int GetRight(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->m_orientation.GetRight());
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int CreateCollision(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Collision **s = (RootForce::Collision **)lua_newuserdata(p_luaState, 4);
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Collision>(*e);
			luaL_setmetatable(p_luaState, "Collision");
			return 1;
		}

		static int SetMeshHandle(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Collision** rtemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 1, "Collision");
			std::string handle = luaL_checkstring(p_luaState, 2);
			(*rtemp)->m_meshHandle = handle;
			return 0;
		}

		static int CreateHandle(lua_State* p_luaState)
		{
			NumberOfArgs(4);
			RootForce::Collision** rtemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 1, "Collision");
			(*rtemp)->m_handle = g_engineContext.m_physics->CreateHandle((unsigned int)luaL_checknumber(p_luaState, 2), (RootEngine::Physics::PhysicsType::PhysicsType)((int)luaL_checknumber(p_luaState, 3)), lua_toboolean(p_luaState, 4) == 1 ? true : false );
			return 0;
		}

		static int CreateCollisionResponder(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::CollisionResponder **s = (RootForce::CollisionResponder **)lua_newuserdata(p_luaState, 4);
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::CollisionResponder>(*e);
			luaL_setmetatable(p_luaState, "CollisionResponder");
			return 1;
		}
		
		static int CreatePhysics(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Physics **s = (RootForce::Physics **)lua_newuserdata(p_luaState, sizeof(RootForce::Physics *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Physics>(*e);
			luaL_setmetatable(p_luaState, "Physics");
			return 1;
		}

		static int SetMass(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Physics** ptemp = (RootForce::Physics**)luaL_checkudata(p_luaState, 1, "Physics");
			(*ptemp)->m_mass = (float)luaL_checknumber(p_luaState, 2);
			return 0;
		}

		static int BindShape(lua_State* p_luaState)
		{
			NumberOfArgs(7);
			RootForce::Collision** rtemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 2, "Collision");
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 3, "Vec3");
			glm::quat* q1 = (glm::quat*)luaL_checkudata(p_luaState, 4, "Quat");
			float radius = (float)luaL_checknumber(p_luaState, 5);
			float mass = (float)luaL_checknumber(p_luaState, 6);
			bool collideWorld = lua_toboolean(p_luaState, 7) == 1 ? true : false;
			g_engineContext.m_physics->BindSphereShape((*(*rtemp)->m_handle), (*v1), (*q1), radius, mass, collideWorld);
			return 0;
		}

		static int SetPhysPos(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Physics** ptemp = (RootForce::Physics**)luaL_checkudata(p_luaState, 1, "Physics");
			(*ptemp)->m_mass = (float)luaL_checknumber(p_luaState, 2);
			return 0;
		}
		static int SetPhysVelocity(lua_State* p_luaState)
		{
			NumberOfArgs(3);
			RootForce::Physics** ptemp = (RootForce::Physics**)luaL_checkudata(p_luaState, 1, "Physics");
			RootForce::Collision** rtemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 2, "Collision");
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 3, "Vec3");

			g_engineContext.m_physics->SetVelocity((*(*rtemp)->m_handle), (*v1));
			return 0;
		}
		
		static int SetRenderableMaterial(lua_State* p_luaState)
        {
            RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
            std::string handle = luaL_checkstring(p_luaState, 2);
            (*rtemp)->m_material = g_engineContext.m_resourceManager->GetMaterial(handle);
            return 0;
        }
		static int SetRenderableModel(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			std::string handle = luaL_checkstring(p_luaState, 2);
			(*rtemp)->m_model = g_engineContext.m_resourceManager->LoadCollada(handle);
			return 0;
		}

		static int SetRenderableDiffuse(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			(*rtemp)->m_material->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture(luaL_checkstring(p_luaState, 2), Render::TextureType::TEXTURE_2D);
			return 0;
		}

		static int SetRenderableSpecular(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			(*rtemp)->m_material->m_specularMap = g_engineContext.m_resourceManager->LoadTexture(luaL_checkstring(p_luaState, 2), Render::TextureType::TEXTURE_2D);
			return 0;
		}

		static int SetRenderableNormal(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			(*rtemp)->m_material->m_normalMap = g_engineContext.m_resourceManager->LoadTexture(luaL_checkstring(p_luaState, 2), Render::TextureType::TEXTURE_2D);
			return 0;
		}

		static int SetRenderableEffect(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			(*rtemp)->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect(luaL_checkstring(p_luaState, 2));
			return 0;
		}

		static int CreateVec3(lua_State* p_luaState)
		{
			NumberOfArgs(3);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((float)lua_tonumber(p_luaState, 1), (float)lua_tonumber(p_luaState, 2), (float)lua_tonumber(p_luaState, 3));
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int SetVec3Value(lua_State* p_luaState)
		{
			glm::vec3* ptemp = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			std::string memberName = luaL_checkstring(p_luaState, 2);
			float setValue = (float)luaL_checknumber(p_luaState, 3);
			if(memberName == "x")
				ptemp->x = setValue;
			if(memberName == "y")
				ptemp->y = setValue;
			if(memberName == "z")
				ptemp->z = setValue;
			return 0;
		}

		static int GetVec3Value(lua_State* p_luaState)
		{
			glm::vec3* ptemp = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			std::string memberName = luaL_checkstring(p_luaState, 2);
			if(memberName == "x")
				lua_pushnumber(p_luaState, ptemp->x);
			if(memberName == "y")
				lua_pushnumber(p_luaState, ptemp->y);
			if(memberName == "z")
				lua_pushnumber(p_luaState, ptemp->z);
			return 1;
		}

		static int AddVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3 v3 = (*v1)+(*v2);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int SubVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3 v3 = (*v1)-(*v2);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int MulVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3 v3 = (*v1)*(*v2);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int DivVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3 v3 = (*v1)/(*v2);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int DotVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			float vecdot = glm::dot((*v1), (*v2));
			lua_pushnumber(p_luaState, vecdot);
			return 1;
		}

		static int CrossVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3  v3 = glm::cross((*v1), (*v2));
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int DistanceVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			float vecdist = glm::distance((*v1), (*v2));
			lua_pushnumber(p_luaState, vecdist);
			return 1;
		}

		static int LengthVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			float vecdot = glm::length((*v1));
			lua_pushnumber(p_luaState, vecdot);
			return 1;
		}
		
		static int NormalizeVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3  v3 = glm::normalize((*v1));
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int ReflectVec3(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3  v3 = glm::reflect((*v1), (*v2));
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int CreateArray(lua_State* p_luaState)
		{
			return 0;
		}
		static int SetArray(lua_State* p_luaState)
		{
			return 0;
		}
		static int GetArray(lua_State* p_luaState)
		{
			return 0;
		}
		static int GetArrayLength(lua_State* p_luaState)
		{
			return 0;
		}
	
		static int GetQuatValue(lua_State* p_luaState)
		{
			glm::quat* ptemp = (glm::quat*)luaL_checkudata(p_luaState, 1, "Quat");
			std::string memberName = luaL_checkstring(p_luaState, 2);
			if(memberName == "x")
				lua_pushnumber(p_luaState, ptemp->x);
			if(memberName == "y")
				lua_pushnumber(p_luaState, ptemp->y);
			if(memberName == "z")
				lua_pushnumber(p_luaState, ptemp->z);
			if(memberName == "w")
				lua_pushnumber(p_luaState, ptemp->w);
			return 1;
		}

		static int SetQuatValue(lua_State* p_luaState)
		{
			glm::quat* ptemp = (glm::quat*)luaL_checkudata(p_luaState, 1, "Quat");
			std::string memberName = luaL_checkstring(p_luaState, 2);
			float setValue = (float)luaL_checknumber(p_luaState, 3);
			if(memberName == "x")
				ptemp->x = setValue;
			if(memberName == "y")
				ptemp->y = setValue;
			if(memberName == "z")
				ptemp->z = setValue;
			if(memberName == "w")
				ptemp->w = setValue;
			return 0;
		}

		static int CreateQuat(lua_State* p_luaState)
		{
			NumberOfArgs(4);
			glm::quat *s = (glm::quat *)lua_newuserdata(p_luaState, sizeof(glm::quat));
			float a1 = (float)lua_tonumber(p_luaState, 1);
			float a2 = (float)lua_tonumber(p_luaState, 2);
			float a3 = (float)lua_tonumber(p_luaState, 3);
			float a4 = (float)lua_tonumber(p_luaState, 4);

			*s = glm::quat(a1,a2,a3,a4);
			luaL_setmetatable(p_luaState, "Quat");
			return 1;
		}

		//Entity functions
		static const struct luaL_Reg entity_f [] = {
			{"New", CreateEntity},
			{"GetEntityByTag", GetEntityByTag},
			{NULL, NULL}
		};

		static const struct luaL_Reg entity_m [] = {
			{"GetId", GetId},
			{"GetTransformation", GetTransformation},
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
			{"SetMaterialDiffuse", SetRenderableDiffuse},
			{"SetMaterialSpecular", SetRenderableSpecular},
			{"SetMaterialNormal", SetRenderableNormal},
			{"SetMaterialEffect", SetRenderableEffect},
			{NULL, NULL}
		};


		//Transform functions & methods
		static const struct luaL_Reg transformation_f [] = {
			{"New", CreateTransformation},
			{NULL, NULL}
		};
		
		static const struct luaL_Reg transformation_m [] = {
			{"GetPos",		GetPos},
			{"SetPos",		SetPos},
			{"GetFront",	GetFront},
			{"GetUp",		GetUp},
			{"GetRight",	GetRight},
			{NULL, NULL}
		};

		static const struct luaL_Reg collision_f [] = {
			{"New", CreateCollision},
			{NULL, NULL}
		};

		static const struct luaL_Reg collision_m [] = {
			{"SetMeshHandle", SetMeshHandle},
			{"CreateHandle", CreateHandle},
			{NULL, NULL}
		};

		//Physics function & methods
		static const struct luaL_Reg physicsaccessor_f [] = {
			{"New", CreatePhysics},
			{NULL, NULL}
		};

		static const struct luaL_Reg physicsaccessor_m [] = {
			{"SetMass", SetMass},
			{"BindShape", BindShape},
			{"SetPos", SetPhysPos},
			{"SetVelocity", SetPhysVelocity},
			{NULL, NULL}
		};

		static const struct luaL_Reg vec3_f [] = {
			{"New", CreateVec3},
			{"Dot", DotVec3},
			{"Cross", CrossVec3},
			{"Distance", DistanceVec3},
			{"Length", LengthVec3},
			{"Normalize", NormalizeVec3},
			{"Reflect", ReflectVec3},
			{NULL, NULL}
		};

		static const struct luaL_Reg vec3_m [] = {
			{"__newindex", SetVec3Value},
			{"__add", AddVec3},
			{"__sub", SubVec3},
			{"__mul", MulVec3},
			{"__div", DivVec3},
			{"__index", GetVec3Value},
			{"__len", LengthVec3},
			{NULL, NULL}
		};

		static const struct luaL_Reg array_f [] = {
			{"New", CreateArray},
			{NULL, NULL}
		};

		static const struct luaL_Reg array_m [] = {
			{"__newindex", SetArray},
			{"__index", GetArray},
			{"__len", GetArrayLength},
			{NULL, NULL}
		};
		static const struct luaL_Reg collisionresponder_f [] = {
			{"New", CreateCollisionResponder},
			{NULL, NULL}
		};

		static const struct luaL_Reg collisionresponder_m [] = {
			{NULL, NULL}
		};

		static const struct luaL_Reg quat_f [] = {
			{"New", CreateQuat},
			{NULL, NULL}
		};
		
		static const struct luaL_Reg quat_m [] = {
			{"__newindex", SetQuatValue},
			{"__index", GetQuatValue},
			{NULL, NULL}
		};

		static int LuaSetupType(lua_State* p_luaState, const luaL_Reg* p_funcReg, const luaL_Reg* p_methodReg, std::string p_typeName)
		{
			luaL_newmetatable(p_luaState, p_typeName.c_str());
			int meta_id = lua_gettop(p_luaState);
			luaL_setfuncs(p_luaState, p_methodReg, 0);

			lua_setfield(p_luaState, meta_id, "__index");  

			luaL_newlib(p_luaState, p_funcReg);

			lua_setglobal(p_luaState, p_typeName.c_str());

			return 1;
		}

		static int LuaSetupTypeNoMethods(lua_State* p_luaState, const luaL_Reg* p_funcReg, const luaL_Reg* p_methodReg, std::string p_typeName)
		{
			luaL_newmetatable(p_luaState, p_typeName.c_str());
			int meta_id = lua_gettop(p_luaState);
			luaL_setfuncs(p_luaState, p_methodReg, 0);

			luaL_newlib(p_luaState, p_funcReg);

			lua_setglobal(p_luaState, p_typeName.c_str());

			return 1;
		}
	}
}