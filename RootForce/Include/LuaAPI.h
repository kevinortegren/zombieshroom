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

		//////////////////////////////////////////////////////////////////////////
		//LOGGING
		//////////////////////////////////////////////////////////////////////////
		static int Log(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			const char* s = luaL_checkstring(p_luaState, 1);
			int l = (int) luaL_checkint(p_luaState, 2);
			
			lua_Debug ar;
			lua_getstack(p_luaState, 1, &ar);
			lua_getinfo(p_luaState, "Sl", &ar);
			
			g_engineContext.m_logger->LogScript(ar.short_src, ar.currentline, LogTag::SCRIPT, (LogLevel::LogLevel) l, s);

			return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		//ENTITY
		//////////////////////////////////////////////////////////////////////////
		static int EntityCreate(lua_State* p_luaState)
		{
			NumberOfArgs(0);
			ECS::Entity **s = (ECS::Entity**)lua_newuserdata(p_luaState, sizeof(ECS::Entity*));
			*s = g_world->GetEntityManager()->CreateEntity();
			luaL_setmetatable(p_luaState, "Entity");
			return 1;
		}

		static int EntityGetByTag(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			ECS::Entity **s = (ECS::Entity**)lua_newuserdata(p_luaState, sizeof(ECS::Entity*));
			*s = g_world->GetTagManager()->GetEntityByTag(luaL_checkstring(p_luaState, 1));
			luaL_setmetatable(p_luaState, "Entity");
			return 1;
		}

		static int EntityGetByID(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			ECS::Entity **s = (ECS::Entity**)lua_newuserdata(p_luaState, sizeof(ECS::Entity*));
			//*s = g_world->GetGroupManager()->
			luaL_setmetatable(p_luaState, "Entity");
			return 1;
		}
		
		static int EntityGetId(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			lua_pushnumber(p_luaState, (*e)->GetId());
			return 1;
		}

		static int EntityGetTransformation(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform **s = (RootForce::Transform **)lua_newuserdata(p_luaState, sizeof(RootForce::Transform *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->GetComponent<RootForce::Transform>(*e);
			luaL_setmetatable(p_luaState, "Transformation");
			return 1;
		}

		static int EntityGetRenderable(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Renderable **s = (RootForce::Renderable **)lua_newuserdata(p_luaState, sizeof(RootForce::Renderable *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->GetComponent<RootForce::Renderable>(*e);
			luaL_setmetatable(p_luaState, "Renderable");
			return 1;
		}

		static int EntityGetPhysics(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Physics **s = (RootForce::Physics **)lua_newuserdata(p_luaState, sizeof(RootForce::Physics *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->GetComponent<RootForce::Physics>(*e);
			luaL_setmetatable(p_luaState, "Physics");
			return 1;
		}

		static int EntityGetCollision(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Collision **s = (RootForce::Collision **)lua_newuserdata(p_luaState, sizeof(RootForce::Collision *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->GetComponent<RootForce::Collision>(*e);
			luaL_setmetatable(p_luaState, "Collision");
			return 1;
		}

		static int EntityGetCollisionResponder(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::CollisionResponder **s = (RootForce::CollisionResponder **)lua_newuserdata(p_luaState, sizeof(RootForce::CollisionResponder *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->GetComponent<RootForce::CollisionResponder>(*e);
			luaL_setmetatable(p_luaState, "CollisionResponder");
			return 1;
		}

		//////////////////////////////////////////////////////////////////////////
		//TRANSFORMATION
		//////////////////////////////////////////////////////////////////////////
		static int TransformationCreate(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform **s = (RootForce::Transform **)lua_newuserdata(p_luaState, sizeof(RootForce::Transform *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Transform>(*e);
			luaL_setmetatable(p_luaState, "Transformation");
			return 1;
		}

		static int TransformationGetPos(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->m_position);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int TransformationSetPos(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			(*ptemp)->m_position = (*v1);
			return 0;
		}

		static int TransformationGetScale(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->m_scale);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int TransformationSetScale(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			(*ptemp)->m_scale = (*v1);
			return 0;
		}

		static int TransformationGetOrient(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			RootForce::Orientation **s = (RootForce::Orientation **)lua_newuserdata(p_luaState, sizeof(RootForce::Orientation*));
			*s = &(*ptemp)->m_orientation;
			luaL_setmetatable(p_luaState, "Orientation");
			return 1;
		}

		static int TransformationSetOrient(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Transform** ptemp = (RootForce::Transform**)luaL_checkudata(p_luaState, 1, "Transformation");
			RootForce::Orientation** v1 = (RootForce::Orientation**)luaL_checkudata(p_luaState, 2, "Orientation");
			(*ptemp)->m_orientation = (*(*v1));
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		//ORIENT
		//////////////////////////////////////////////////////////////////////////
		static int OrientCreate(lua_State* p_luaState)
		{
			NumberOfArgs(0);
			RootForce::Orientation *s = (RootForce::Orientation *)lua_newuserdata(p_luaState, sizeof(RootForce::Orientation));
			*s = RootForce::Orientation();
			luaL_setmetatable(p_luaState, "Orientation");
			return 1;
		}

		static int OrientGetFront(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->GetFront());
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int OrientGetUp(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->GetUp());
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int OrientGetRight(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->GetRight());
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int OrientGetAxis(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((*ptemp)->GetAxis());
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int OrientGetAngle(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			lua_pushnumber(p_luaState, (*ptemp)->GetAngle());
			return 1;
		}

		static int OrientGetQuaternion(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			glm::quat *s = (glm::quat *)lua_newuserdata(p_luaState, sizeof(glm::quat));
			*s = glm::quat((*ptemp)->GetQuaternion());
			luaL_setmetatable(p_luaState, "Quat");
			return 1;
		}

		static int OrientSetOrientation(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			glm::quat* rtemp = (glm::quat*)luaL_checkudata(p_luaState, 2, "Quat");
			(*ptemp)->SetOrientation(*rtemp);
			return 0;
		}

		static int OrientPitch(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			(*ptemp)->Pitch((float)luaL_checknumber(p_luaState, 2));
			return 0;
		}

		static int OrientYaw(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			(*ptemp)->Yaw((float)luaL_checknumber(p_luaState, 2));
			return 0;
		}

		static int OrientRoll(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			(*ptemp)->Roll((float)luaL_checknumber(p_luaState, 2));
			return 0;
		}

		static int OrientRotate(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			glm::quat* rtemp = (glm::quat*)luaL_checkudata(p_luaState, 2, "Quat");
			(*ptemp)->Rotate(*rtemp);
			return 0;
		}

		static int OrientPitchGlobal(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			(*ptemp)->PitchGlobal((float)luaL_checknumber(p_luaState, 2));
			return 0;
		}

		static int OrientYawGlobal(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			(*ptemp)->YawGlobal((float)luaL_checknumber(p_luaState, 2));
			return 0;
		}

		static int OrientRollGlobal(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			(*ptemp)->RollGlobal((float)luaL_checknumber(p_luaState, 2));
			return 0;
		}

		static int OrientRotateGlobal(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			glm::quat* rtemp = (glm::quat*)luaL_checkudata(p_luaState, 2, "Quat");
			(*ptemp)->RotateGlobal(*rtemp);
			return 0;
		}

		static int OrientLookAt(lua_State* p_luaState)
		{
			NumberOfArgs(3);
			RootForce::Orientation** ptemp = (RootForce::Orientation**)luaL_checkudata(p_luaState, 1, "Orientation");
			glm::vec3* dir = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3* up = (glm::vec3*)luaL_checkudata(p_luaState, 3, "Vec3");
			(*ptemp)->LookAt(*dir, *up);
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		//COLLISION
		//////////////////////////////////////////////////////////////////////////
		static int CollisionCreate(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Collision **s = (RootForce::Collision **)lua_newuserdata(p_luaState, 4);
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Collision>(*e);
			luaL_setmetatable(p_luaState, "Collision");
			return 1;
		}

		static int CollisionSetMeshHandle(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Collision** rtemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 1, "Collision");
			std::string handle = luaL_checkstring(p_luaState, 2);
			(*rtemp)->m_meshHandle = handle;
			return 0;
		}

		static int CollisionCreateHandle(lua_State* p_luaState)
		{
			NumberOfArgs(4);
			RootForce::Collision** rtemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 1, "Collision");
			(*rtemp)->m_handle = g_engineContext.m_physics->CreateHandle((unsigned int)luaL_checknumber(p_luaState, 2), (RootEngine::Physics::PhysicsType::PhysicsType)((int)luaL_checknumber(p_luaState, 3)), lua_toboolean(p_luaState, 4) != 0 );
			return 0;
		}

		static int CollisionGetHandle(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Collision** rtemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 1, "Collision");
			lua_pushnumber(p_luaState, *(*rtemp)->m_handle); 
			return 1;
		}
		//////////////////////////////////////////////////////////////////////////
		//COLLISIONRESPONDER
		//////////////////////////////////////////////////////////////////////////
		static int CollisionResponderCreate(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::CollisionResponder **s = (RootForce::CollisionResponder **)lua_newuserdata(p_luaState, 4);
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::CollisionResponder>(*e);
			luaL_setmetatable(p_luaState, "CollisionResponder");
			return 1;
		}
		
		static int CollisionResponderSetCollisionContainer(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::CollisionResponder** rtemp = (RootForce::CollisionResponder**)luaL_checkudata(p_luaState, 1, "CollisionResponder");
			RootForce::Collision** ctemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 2, "Collision");
			g_engineContext.m_physics->SetCollisionContainer(*(*ctemp)->m_handle, &(*rtemp)->m_collidedEntityId);
			return 0;
		}
		//////////////////////////////////////////////////////////////////////////
		//PHYSICS
		//////////////////////////////////////////////////////////////////////////
		static int PhysicsCreate(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Physics **s = (RootForce::Physics **)lua_newuserdata(p_luaState, sizeof(RootForce::Physics *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Physics>(*e);
			luaL_setmetatable(p_luaState, "Physics");
			return 1;
		}

		static int PhysicsSetMass(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Physics** ptemp = (RootForce::Physics**)luaL_checkudata(p_luaState, 1, "Physics");
			(*ptemp)->m_mass = (float)luaL_checknumber(p_luaState, 2);
			return 0;
		}

		static int PhysicsSetPos(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Physics** ptemp = (RootForce::Physics**)luaL_checkudata(p_luaState, 1, "Physics");
			(*ptemp)->m_mass = (float)luaL_checknumber(p_luaState, 2);
			return 0;
		}
		static int PhysicsBindShape(lua_State* p_luaState)
		{
			NumberOfArgs(7);
			RootForce::Collision** rtemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 2, "Collision");
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 3, "Vec3");
			glm::quat* q1 = (glm::quat*)luaL_checkudata(p_luaState, 4, "Quat");
			float radius = (float)luaL_checknumber(p_luaState, 5);
			float mass = (float)luaL_checknumber(p_luaState, 6);
			bool collideWorld = lua_toboolean(p_luaState, 7) != 0;
			g_engineContext.m_physics->BindSphereShape((*(*rtemp)->m_handle), (*v1), (*q1), radius, mass, collideWorld);
			return 0;
		}
		static int PhysicsSetVelocity(lua_State* p_luaState)
		{
			NumberOfArgs(3);
			RootForce::Physics** ptemp = (RootForce::Physics**)luaL_checkudata(p_luaState, 1, "Physics");
			RootForce::Collision** rtemp = (RootForce::Collision**)luaL_checkudata(p_luaState, 2, "Collision");
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 3, "Vec3");

			g_engineContext.m_physics->SetVelocity((*(*rtemp)->m_handle), (*v1));
			return 0;
		}
		static int PhysicsKnockBack(lua_State* p_luaState)
		{
			NumberOfArgs(4);
			RootForce::Physics** ptemp = (RootForce::Physics**)luaL_checkudata(p_luaState, 1, "Physics");
			g_engineContext.m_physics->KnockbackObject((int)luaL_checknumber(p_luaState, 2), *((glm::vec3*)luaL_checkudata(p_luaState, 3, "Vec3")), (float)luaL_checknumber(p_luaState, 4));
			return 0;
		}
		
		//////////////////////////////////////////////////////////////////////////
		//RENDERABLE
		//////////////////////////////////////////////////////////////////////////
		static int RenderableCreate(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Renderable **s = (RootForce::Renderable **)lua_newuserdata(p_luaState, sizeof(RootForce::Renderable *));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(*e);
			luaL_setmetatable(p_luaState, "Renderable");
			return 1;
		}

		static int RenderableSetMaterial(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			std::string handle = luaL_checkstring(p_luaState, 2);
			(*rtemp)->m_material = g_engineContext.m_resourceManager->GetMaterial(handle);
			return 0;
		}
		static int RenderableSetModel(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			std::string handle = luaL_checkstring(p_luaState, 2);
			(*rtemp)->m_model = g_engineContext.m_resourceManager->LoadCollada(handle);
			return 0;
		}

		static int RenderableSetDiffuse(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			(*rtemp)->m_material->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture(luaL_checkstring(p_luaState, 2), Render::TextureType::TEXTURE_2D);
			return 0;
		}

		static int RenderableSetSpecular(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			(*rtemp)->m_material->m_specularMap = g_engineContext.m_resourceManager->LoadTexture(luaL_checkstring(p_luaState, 2), Render::TextureType::TEXTURE_2D);
			return 0;
		}

		static int RenderableSetNormal(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			(*rtemp)->m_material->m_normalMap = g_engineContext.m_resourceManager->LoadTexture(luaL_checkstring(p_luaState, 2), Render::TextureType::TEXTURE_2D);
			return 0;
		}

		static int RenderableSetEffect(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			(*rtemp)->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect(luaL_checkstring(p_luaState, 2));
			return 0;
		}

		static int RenderableGetMaterial(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			lua_pushstring(p_luaState, g_engineContext.m_resourceManager->ResolveStringFromMaterial((*rtemp)->m_material).c_str());
			return 1;
		}
		static int RenderableGetModel(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			lua_pushstring(p_luaState, g_engineContext.m_resourceManager->ResolveStringFromModel((*rtemp)->m_model).c_str());
			return 1;
		}

		static int RenderableGetDiffuse(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			lua_pushstring(p_luaState, g_engineContext.m_resourceManager->ResolveStringFromTexture((*rtemp)->m_material->m_diffuseMap).c_str());
			return 1;
		}

		static int RenderableGetSpecular(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			lua_pushstring(p_luaState, g_engineContext.m_resourceManager->ResolveStringFromTexture((*rtemp)->m_material->m_specularMap).c_str());
			return 1;
		}

		static int RenderableGetNormal(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			lua_pushstring(p_luaState, g_engineContext.m_resourceManager->ResolveStringFromTexture((*rtemp)->m_material->m_normalMap).c_str());
			return 1;
		}

		static int RenderableGetEffect(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::Renderable** rtemp = (RootForce::Renderable**)luaL_checkudata(p_luaState, 1, "Renderable");
			lua_pushstring(p_luaState, g_engineContext.m_resourceManager->ResolveStringFromEffect((*rtemp)->m_material->m_effect).c_str());
			return 1;
		}

		//////////////////////////////////////////////////////////////////////////
		//VEC3
		//////////////////////////////////////////////////////////////////////////
		static int Vec3Create(lua_State* p_luaState)
		{
			NumberOfArgs(3);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3((float)lua_tonumber(p_luaState, 1), (float)lua_tonumber(p_luaState, 2), (float)lua_tonumber(p_luaState, 3));
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int Vec3SetValue(lua_State* p_luaState)
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

		static int Vec3GetValue(lua_State* p_luaState)
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

		static int Vec3Add(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3 v3 = (*v1)+(*v2);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int Vec3Sub(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3 v3 = (*v1)-(*v2);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int Vec3Mul(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3 v3 = (*v1)*(*v2);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int Vec3Div(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3 v3 = (*v1)/(*v2);
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int Vec3Dot(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			float vecdot = glm::dot((*v1), (*v2));
			lua_pushnumber(p_luaState, vecdot);
			return 1;
		}

		static int Vec3Cross(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3  v3 = glm::cross((*v1), (*v2));
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int Vec3Distance(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			float vecdist = glm::distance((*v1), (*v2));
			lua_pushnumber(p_luaState, vecdist);
			return 1;
		}

		static int Vec3Length(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			float vecdot = glm::length((*v1));
			lua_pushnumber(p_luaState, vecdot);
			return 1;
		}
		
		static int Vec3Normalize(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3  v3 = glm::normalize((*v1));
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}

		static int Vec3Reflect(lua_State* p_luaState)
		{
			glm::vec3* v1 = (glm::vec3*)luaL_checkudata(p_luaState, 1, "Vec3");
			glm::vec3* v2 = (glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			glm::vec3  v3 = glm::reflect((*v1), (*v2));
			glm::vec3 *s = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*s = glm::vec3(v3);
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}
		//////////////////////////////////////////////////////////////////////////
		//VEC4
		//////////////////////////////////////////////////////////////////////////
		static int Vec4Create(lua_State* p_luaState)
		{
			NumberOfArgs(4);
			glm::vec4 *s = (glm::vec4 *)lua_newuserdata(p_luaState, sizeof(glm::vec4));
			*s = glm::vec4((float)luaL_checknumber(p_luaState, 1), (float)luaL_checknumber(p_luaState, 2), (float)luaL_checknumber(p_luaState, 3), (float)luaL_checknumber(p_luaState, 4));
			luaL_setmetatable(p_luaState, "Vec4");
			return 1;
		}

		static int Vec4SetValue(lua_State* p_luaState)
		{
			glm::vec4* ptemp = (glm::vec4*)luaL_checkudata(p_luaState, 1, "Vec4");
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

		static int Vec4GetValue(lua_State* p_luaState)
		{
			glm::vec4* ptemp = (glm::vec4*)luaL_checkudata(p_luaState, 1, "Vec4");
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

		static int Vec4Add(lua_State* p_luaState)
		{
			glm::vec4* v1 = (glm::vec4*)luaL_checkudata(p_luaState, 1, "Vec4");
			glm::vec4* v2 = (glm::vec4*)luaL_checkudata(p_luaState, 2, "Vec4");
			glm::vec4 v3 = (*v1)+(*v2);
			glm::vec4 *s = (glm::vec4 *)lua_newuserdata(p_luaState, sizeof(glm::vec4));
			*s = glm::vec4(v3);
			luaL_setmetatable(p_luaState, "Vec4");
			return 1;
		}

		static int Vec4Sub(lua_State* p_luaState)
		{
			glm::vec4* v1 = (glm::vec4*)luaL_checkudata(p_luaState, 1, "Vec4");
			glm::vec4* v2 = (glm::vec4*)luaL_checkudata(p_luaState, 2, "Vec4");
			glm::vec4 v3 = (*v1)-(*v2);
			glm::vec4 *s = (glm::vec4 *)lua_newuserdata(p_luaState, sizeof(glm::vec4));
			*s = glm::vec4(v3);
			luaL_setmetatable(p_luaState, "Vec4");
			return 1;
		}

		static int Vec4Mul(lua_State* p_luaState)
		{
			glm::vec4* v1 = (glm::vec4*)luaL_checkudata(p_luaState, 1, "Vec4");
			glm::vec4* v2 = (glm::vec4*)luaL_checkudata(p_luaState, 2, "Vec4");
			glm::vec4 v3 = (*v1)*(*v2);
			glm::vec4 *s = (glm::vec4 *)lua_newuserdata(p_luaState, sizeof(glm::vec4));
			*s = glm::vec4(v3);
			luaL_setmetatable(p_luaState, "Vec4");
			return 1;
		}

		static int Vec4Div(lua_State* p_luaState)
		{
			glm::vec4* v1 = (glm::vec4*)luaL_checkudata(p_luaState, 1, "Vec4");
			glm::vec4* v2 = (glm::vec4*)luaL_checkudata(p_luaState, 2, "Vec4");
			glm::vec4 v3 = (*v1)/(*v2);
			glm::vec4 *s = (glm::vec4 *)lua_newuserdata(p_luaState, sizeof(glm::vec4));
			*s = glm::vec4(v3);
			luaL_setmetatable(p_luaState, "Vec4");
			return 1;
		}
		//////////////////////////////////////////////////////////////////////////
		//ARRAY
		//////////////////////////////////////////////////////////////////////////
		static int ArrayCreate(lua_State* p_luaState)
		{
			return 0;
		}
		static int ArraySet(lua_State* p_luaState)
		{
			return 0;
		}
		static int ArrayGet(lua_State* p_luaState)
		{
			return 0;
		}
		static int ArrayGetLength(lua_State* p_luaState)
		{
			return 0;
		}
	
		//////////////////////////////////////////////////////////////////////////
		//QUAT
		//////////////////////////////////////////////////////////////////////////
		static int QuatCreate(lua_State* p_luaState)
		{
			NumberOfArgs(4);
			glm::quat *s = (glm::quat *)lua_newuserdata(p_luaState, sizeof(glm::quat));
			float a1 = (float)lua_tonumber(p_luaState, 1);
			float a2 = (float)lua_tonumber(p_luaState, 2);
			float a3 = (float)lua_tonumber(p_luaState, 3);
			float a4 = (float)lua_tonumber(p_luaState, 4);

			*s = glm::quat(a4,a1,a2,a3);
			luaL_setmetatable(p_luaState, "Quat");
			return 1;
		}

		static int QuatGetValue(lua_State* p_luaState)
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

		static int QuatSetValue(lua_State* p_luaState)
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

		//////////////////////////////////////////////////////////////////////////
		//SCRIPT
		//////////////////////////////////////////////////////////////////////////
		static int ScriptCreate(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::Script **s = (RootForce::Script **)lua_newuserdata(p_luaState, sizeof(RootForce::Script*));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::Script>(*e);
			(*s)->Name = g_engineContext.m_resourceManager->GetScript(luaL_checkstring(p_luaState, 2));
			luaL_setmetatable(p_luaState, "Script");
			
			return 1;
		}

		//////////////////////////////////////////////////////////////////////////
		//POINTLIGHT
		//////////////////////////////////////////////////////////////////////////
		static int PointLightCreate(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::PointLight **s = (RootForce::PointLight**)lua_newuserdata(p_luaState, sizeof(RootForce::PointLight*));
			ECS::Entity** e = (ECS::Entity**)luaL_checkudata(p_luaState, 1, "Entity");
			*s = g_world->GetEntityManager()->CreateComponent<RootForce::PointLight>(*e);
			luaL_setmetatable(p_luaState, "PointLight");
			return 1;
		}
		static int PointLightSetColor(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::PointLight **s = (RootForce::PointLight**)luaL_checkudata(p_luaState, 1, "PointLight");
			(*s)->m_color = *(glm::vec4*)luaL_checkudata(p_luaState, 2, "Vec4");
			return 0;
		}
		static int PointLightSetAttenuation(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::PointLight **s = (RootForce::PointLight**)luaL_checkudata(p_luaState, 1, "PointLight");
			(*s)->m_attenuation = *(glm::vec3*)luaL_checkudata(p_luaState, 2, "Vec3");
			return 0;
		}
		static int PointLightSetRange(lua_State* p_luaState)
		{
			NumberOfArgs(2);
			RootForce::PointLight **s = (RootForce::PointLight**)luaL_checkudata(p_luaState, 1, "PointLight");
			(*s)->m_range = (float)luaL_checknumber(p_luaState, 2);
			return 0;
		}
		static int PointLightGetColor(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::PointLight **s = (RootForce::PointLight**)luaL_checkudata(p_luaState, 1, "PointLight");
			glm::vec4 *v1 = (glm::vec4 *)lua_newuserdata(p_luaState, sizeof(glm::vec4));
			*v1 = (*s)->m_color;
			luaL_setmetatable(p_luaState, "Vec4");
			return 1;
		}
		static int PointLightGetAttenuation(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::PointLight **s = (RootForce::PointLight**)luaL_checkudata(p_luaState, 1, "PointLight");
			glm::vec3 *v1 = (glm::vec3 *)lua_newuserdata(p_luaState, sizeof(glm::vec3));
			*v1 = (*s)->m_attenuation;
			luaL_setmetatable(p_luaState, "Vec3");
			return 1;
		}
		static int PointLightGetRange(lua_State* p_luaState)
		{
			NumberOfArgs(1);
			RootForce::PointLight **s = (RootForce::PointLight**)luaL_checkudata(p_luaState, 1, "PointLight");
			lua_pushnumber(p_luaState, (*s)->m_range);
			return 1;
		}

		static const struct luaL_Reg logging_f [] = {
			{"Log", Log},
			{NULL, NULL}
		};

		static const struct luaL_Reg logging_m [] = {
			{NULL, NULL}
		};

		static const struct luaL_Reg entity_f [] = {
			{"New", EntityCreate},
			{"GetEntityByTag", EntityGetByTag},
			{"GetEntityByID", EntityGetByID},
			{NULL, NULL}
		};

		static const struct luaL_Reg entity_m [] = {
			{"GetId", EntityGetId},
			{"GetTransformation", EntityGetTransformation},
			{"GetRenderable", EntityGetRenderable},
			{"GetPhysics", EntityGetPhysics},
			{"GetCollision", EntityGetCollision},
			{"GetCollisionResponder", EntityGetCollisionResponder},
			{NULL, NULL}
		};

		//Renderable functions&methods
		static const struct luaL_Reg renderable_f [] = {
			{"New", RenderableCreate},
			{NULL, NULL}
		};

		static const struct luaL_Reg renderable_m [] = {
			{"SetModel",			RenderableSetModel},
			{"SetMaterial",			RenderableSetMaterial},
			{"SetMaterialDiffuse",	RenderableSetDiffuse},
			{"SetMaterialSpecular", RenderableSetSpecular},
			{"SetMaterialNormal",	RenderableSetNormal},
			{"SetMaterialEffect",	RenderableSetEffect},
			{"GetModel",			RenderableGetModel},
			{"GetMaterial",			RenderableGetMaterial},
			{"GetMaterialDiffuse",	RenderableGetDiffuse},
			{"GetMaterialSpecular", RenderableGetSpecular},
			{"GetMaterialNormal",	RenderableGetNormal},
			{"GetMaterialEffect",	RenderableGetEffect},
			{NULL, NULL}
		};


		//Transform functions & methods
		static const struct luaL_Reg transformation_f [] = {
			{"New", TransformationCreate},
			{NULL, NULL}
		};
		
		static const struct luaL_Reg transformation_m [] = {
			{"GetPos",		TransformationGetPos},
			{"SetPos",		TransformationSetPos},
			{"SetScale",	TransformationSetScale},
			{"GetScale",	TransformationGetScale},
			{"SetOrient",	TransformationSetOrient},
			{"GetOrient",	TransformationGetOrient},
			{NULL, NULL}
		};

		static const struct luaL_Reg collision_f [] = {
			{"New", CollisionCreate},
			{NULL, NULL}
		};

		static const struct luaL_Reg collision_m [] = {
			{"SetMeshHandle", CollisionSetMeshHandle},
			{"CreateHandle", CollisionCreateHandle},
			{"GetHandle", CollisionGetHandle},
			{NULL, NULL}
		};

		//Physics function & methods
		static const struct luaL_Reg physicsaccessor_f [] = {
			{"New", PhysicsCreate},
			{NULL, NULL}
		};

		static const struct luaL_Reg physicsaccessor_m [] = {
			{"SetMass", PhysicsSetMass},
			{"BindShape", PhysicsBindShape},
			{"SetPos", PhysicsSetPos},
			{"SetVelocity", PhysicsSetVelocity},
			{"KnockBack", PhysicsKnockBack},
			{NULL, NULL}
		};

		static const struct luaL_Reg vec3_f [] = {
			{"New",			Vec3Create},
			{"Dot",			Vec3Dot},
			{"Cross",		Vec3Cross},
			{"Distance",	Vec3Distance},
			{"Length",		Vec3Length},
			{"Normalize",	Vec3Normalize},
			{"Reflect",		Vec3Reflect},
			{NULL, NULL}
		};

		static const struct luaL_Reg vec3_m [] = {
			{"__newindex",	Vec3SetValue},
			{"__add",		Vec3Add},
			{"__sub",		Vec3Sub},
			{"__mul",		Vec3Mul},
			{"__div",		Vec3Div},
			{"__index",		Vec3GetValue},
			{"__len",		Vec3Length},
			{NULL, NULL}
		};
		static const struct luaL_Reg vec4_f [] = {
			{"New",			Vec4Create},
			{NULL, NULL}
		};

		static const struct luaL_Reg vec4_m [] = {
			{"__newindex",	Vec4SetValue},
			{"__add",		Vec4Add},
			{"__sub",		Vec4Sub},
			{"__mul",		Vec4Mul},
			{"__div",		Vec4Div},
			{"__index",		Vec4GetValue},
			{NULL, NULL}
		};

		static const struct luaL_Reg array_f [] = {
			{"New", ArrayCreate},
			{NULL, NULL}
		};

		static const struct luaL_Reg array_m [] = {
			{"__newindex",	ArraySet},
			{"__index",		ArrayGet},
			{"__len",		ArrayGetLength},
			{NULL, NULL}
		};
		static const struct luaL_Reg collisionresponder_f [] = {
			{"New", CollisionResponderCreate},
			{NULL, NULL}
		};

		static const struct luaL_Reg collisionresponder_m [] = {
			{"SetContainer", CollisionResponderSetCollisionContainer},
			{NULL, NULL}
		};

		static const struct luaL_Reg quat_f [] = {
			{"New", QuatCreate},
			{NULL, NULL}
		};
		
		static const struct luaL_Reg quat_m [] = {
			{"__newindex", QuatSetValue},
			{"__index", QuatGetValue},
			{NULL, NULL}
		};

		static const struct luaL_Reg orient_f [] = {
			{"New", OrientCreate},
			{NULL, NULL}
		};

		static const struct luaL_Reg orient_m [] = {
			{"GetQuaternion",	OrientGetQuaternion},
			//{"GetMatrix",		OrientGetMatrix},
			{"GetAxis",			OrientGetAxis},
			{"GetAngle",		OrientGetAngle},
			{"GetFront",		OrientGetFront},
			{"GetUp",			OrientGetUp},
			{"GetRight",		OrientGetRight},
			{"SetOrientation",	OrientSetOrientation},
			{"Pitch",	        OrientPitch},
			{"Yaw",				OrientYaw},
			{"Roll",			OrientRoll},
			{"Rotate",			OrientRotate},
			{"PitchGlobal",		OrientPitchGlobal},
			{"YawGlobal",		OrientYawGlobal},
			{"RollGlobal",		OrientRollGlobal},
			{"RotateGlobal",	OrientRotateGlobal},
			{"LookAt",			OrientLookAt},
			{NULL, NULL}
		};

		static const struct luaL_Reg script_f [] = {
			{"New", ScriptCreate},
			{NULL, NULL}
		};

		static const struct luaL_Reg script_m [] = {
			{NULL, NULL}
		};

		static const struct luaL_Reg pointLight_f [] = {
			{"New", PointLightCreate},
			{NULL, NULL}
		};

		static const struct luaL_Reg pointLight_m [] = {
			{"SetColor",		PointLightSetColor},
			{"SetAttenuation",	PointLightSetAttenuation},
			{"SetRange",		PointLightSetRange},
			{"GetColor",		PointLightGetColor},
			{"GetAttenuation",	PointLightGetAttenuation},
			{"GetRange",		PointLightGetRange},
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