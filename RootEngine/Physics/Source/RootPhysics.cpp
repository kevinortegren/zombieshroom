#include "RootPhysics.h"
#include <stdio.h>
#include <iostream>
#include <Bullet/BulletCollision/CollisionShapes/btShapeHull.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <Bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
extern ContactAddedCallback		gContactAddedCallback;

//#include "vld.h"
namespace RootEngine
{


namespace Physics
{

	struct RayAgainstStaticCast : public btCollisionWorld::ClosestRayResultCallback
	{
		RayAgainstStaticCast() : btCollisionWorld::ClosestRayResultCallback(btVector3(0.f, 0.f, 0.f), btVector3(0.f, 0.f, 0.f))
		{

		}
		btScalar addSingleResult(btCollisionWorld::LocalRayResult& p_rayResult, bool p_normalInWorldSpace)
		{
			CustomUserPointer* ptr = (CustomUserPointer*)(p_rayResult.m_collisionObject->getUserPointer());
			if (ptr != nullptr)
			{
				
				if (ptr->m_type == PhysicsType::TYPE_STATIC)
				{
					return btCollisionWorld::ClosestRayResultCallback::addSingleResult(p_rayResult, p_normalInWorldSpace);
				}
			}
				
			return 1.0f;
				
		}
	};

	struct RayAbilityCast : public btCollisionWorld::ClosestRayResultCallback
	{
		CustomUserPointer* m_hit;
		RayAbilityCast() : btCollisionWorld::ClosestRayResultCallback(btVector3(0.f, 0.f, 0.f), btVector3(0.f, 0.f, 0.f))
		{

		}
		btScalar addSingleResult(btCollisionWorld::LocalRayResult& p_rayResult, bool p_normalInWorldSpace)
		{
			//p_rayResult.
			
			m_hit = (CustomUserPointer*)(p_rayResult.m_collisionObject->getUserPointer());
			
			return btCollisionWorld::ClosestRayResultCallback::addSingleResult(p_rayResult, p_normalInWorldSpace);
			//return 1.0f;
		}
	};

	RootEngine::SubsystemSharedContext g_context;
	RootPhysics* RootPhysics::s_physicsInstance = nullptr;
	Render::RendererInterface* g_renderer;
	RootEngine::ResourceManagerInterface* g_resourceManager;
	const int MAX_STEPSIMULATION_SUBSTEPS = 20;

	RootPhysics::RootPhysics()
	{

	}

	RootPhysics::~RootPhysics()
	{

	
	}
	void RootPhysics::Startup()
	{
		Init();
	}
	void RootPhysics::Shutdown()
	{
		//Magic loop of deleting, might not work with ghostobjects(players). Check this before release.
		RemoveAll();
		delete m_debugDrawer;
		delete m_dynamicWorld;
		delete m_solver;
		delete m_broadphase;
		delete m_dispatcher;
		delete m_collisionConfig;
		g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Shutdown complete");
		
	}

	///Must be global, used to check custom collision events, NOTE : Don't ever ever use this yourself!
	bool callbackFunc(btManifoldPoint& p_cp,const btCollisionObjectWrapper * p_obj1 , int p_id1, int p_index1, const btCollisionObjectWrapper * p_obj2 , int p_id2, int p_index2 )
	{

		CustomUserPointer* pointer1 = (CustomUserPointer*)(p_obj1->getCollisionObject()->getUserPointer());
		CustomUserPointer* pointer2 = (CustomUserPointer*)(p_obj2->getCollisionObject()->getUserPointer());
		if(pointer1 == nullptr|| pointer1->m_id == nullptr)
			return false;
		if(pointer2 == nullptr || pointer2->m_id == nullptr)
			return false;
		if(pointer1->m_type == PhysicsType::TYPE_STATIC)
			btAdjustInternalEdgeContacts(p_cp,p_obj1,p_obj2, p_id1,p_index1);
		else if(pointer2->m_type == PhysicsType::TYPE_STATIC)
			btAdjustInternalEdgeContacts(p_cp,p_obj2,p_obj1, p_id2,p_index2);
		if(pointer1->m_collisions != nullptr)
		{
			if(pointer1->m_collisions->count(pointer2->m_entity) == 0)
			{
				btVector3 temp = p_cp.getPositionWorldOnA();
				RootForce::CollisionInfo info;
				info.m_collisionPosition = glm::vec3(temp.x(), temp.y(), temp.z());
				pointer1->m_collisions->insert(std::make_pair(pointer2->m_entity, info));
			}
			
		}
			
		if(pointer2->m_collisions != nullptr)
		{
			if(pointer2->m_collisions->count(pointer1->m_entity) == 0)
			{
				btVector3 temp = p_cp.getPositionWorldOnA();
				RootForce::CollisionInfo info;
				info.m_collisionPosition = glm::vec3(temp.x(), temp.y(), temp.z());
				pointer2->m_collisions->insert(std::make_pair(pointer1->m_entity, info));
			}
				//pointer2->m_collidedEntities->insert(pointer1->m_entity);
		}


		return true;
	}


	void RootPhysics::Init()
	{
		m_collisionConfig= new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
		m_broadphase = new btDbvtBroadphase();
		//m_broadphase = new btAxisSweep3(btVector3(-1000,-1000,-1000), btVector3(1000,1000,1000));
		m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback()); //TODO:: We should send in a member variable so we can delete it in deconstructor
		m_solver = new btSequentialImpulseConstraintSolver();
		m_dynamicWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfig);
		m_dynamicWorld->setGravity(btVector3(0.0f, -9.82f, 0.0f));
		gContactAddedCallback = callbackFunc;
		g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::INIT_PRINT, "Physics subsystem initialized!");
		m_debugDrawer = new DebugDrawer();
		m_debugDrawer->setDebugMode(m_debugDrawer->getDebugMode() | btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints /*| btIDebugDraw::DBG_DrawAabb*/ |btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits);
		m_dynamicWorld->setDebugDrawer(m_debugDrawer);
		m_dynamicWorld->debugDrawWorld();
		m_debugDrawEnabled = false;
		m_dynamicWorld->getDispatchInfo().m_allowedCcdPenetration=0.0001f;
		m_dynamicWorld->getSolverInfo().m_numIterations = 20;
		m_dynamicWorld->setForceUpdateAllAabbs(false);
		//m_dynamicWorld->getSolverInfo().m_numIterations = 4;
		//btSetDebugDrawer(m_debugDrawer);

	}


	//////////////////////////////////////////////////////////////////////////
	
	void RootPhysics::Update(float p_dt)
	{
		
		m_dt = p_dt;
		//if(m_debugDrawEnabled == false)
		
		m_dynamicWorld->stepSimulation(m_dt,MAX_STEPSIMULATION_SUBSTEPS, btScalar(1.)/btScalar(120.));
		for(unsigned int i = 0; i < m_playerObjects.size(); i++)
		{
			m_playerObjects.at(i)->Update(m_dt);
		}

		if(m_debugDrawEnabled)
			m_dynamicWorld->debugDrawWorld();
		
	}
	void RootPhysics::RemoveObject( int p_objectHandle)
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		
		CustomUserPointer* userPointer = m_userPointer.at(p_objectHandle);
		if(userPointer->m_type == PhysicsType::TYPE_PLAYER || userPointer->m_type == PhysicsType::TYPE_RAGDOLL)
		{
			//Player object
			int removedIndex = userPointer->m_vectorIndex;
			m_playerObjects.at(removedIndex)->RemovePlayer();
			delete m_playerObjects.at(removedIndex);
			m_playerObjects.erase(m_playerObjects.begin() + removedIndex);
			//Ragdoll
			int ragdollRemovedIndex = userPointer->m_ragdollIndex;
			if(ragdollRemovedIndex != -1)
			{
				
				delete m_ragdolls.at(ragdollRemovedIndex);
				m_ragdolls.erase(m_ragdolls.begin() + ragdollRemovedIndex);
			}
			delete userPointer;
			m_userPointer.erase(m_userPointer.begin() + p_objectHandle);
		
			for(unsigned int i = p_objectHandle; i < m_userPointer.size(); i++)
			{
				m_userPointer.at(i)->m_id[0] --;
				if(m_userPointer.at(i)->m_type == PhysicsType::TYPE_PLAYER ||m_userPointer.at(i)->m_type == PhysicsType::TYPE_RAGDOLL)
				{					
					if(m_userPointer.at(i)->m_vectorIndex > removedIndex)
						m_userPointer.at(i)->m_vectorIndex--;
					if(m_userPointer.at(i)->m_ragdollIndex > ragdollRemovedIndex)
						m_userPointer.at(i)->m_ragdollIndex--;
				}
				
			}
			

		}
		else //TODO : Remove the shapeless ability!
		{
			int removedIndex = userPointer->m_vectorIndex;
			if (userPointer->m_shape == PhysicsShape::SHAPE_NONE)
			{
				delete m_shapelessObjects.at(removedIndex);
				m_shapelessObjects.erase(m_shapelessObjects.begin() + removedIndex);

				delete userPointer;
				m_userPointer.erase(m_userPointer.begin() + p_objectHandle);

				for(unsigned int i = p_objectHandle; i < m_userPointer.size(); i++)
				{
					m_userPointer.at(i)->m_id[0] --;
					if(m_userPointer.at(i)->m_shape == PhysicsShape::SHAPE_NONE)
					{
						if(m_userPointer.at(i)->m_vectorIndex > removedIndex)
							m_userPointer.at(i)->m_vectorIndex--;
					}
				}
			}
			else if (userPointer->m_type == PhysicsType::TYPE_DYNAMIC || (userPointer->m_type == PhysicsType::TYPE_ABILITY && userPointer->m_externalControlled == false))
			{
				//unsigned int removedIndex = userPointer->m_vectorIndex;
				m_dynamicWorld->removeRigidBody(m_dynamicObjects.at(removedIndex));
				delete m_dynamicObjects.at(removedIndex);
				m_dynamicObjects.erase(m_dynamicObjects.begin() + removedIndex);
			
				delete userPointer;
				m_userPointer.erase(m_userPointer.begin() + p_objectHandle);

				for(unsigned int i = p_objectHandle; i < m_userPointer.size(); i++)
				{

					m_userPointer.at(i)->m_id[0] --;
					if(m_userPointer.at(i)->m_type != PhysicsType::TYPE_PLAYER && m_userPointer.at(i)->m_shape != PhysicsShape::SHAPE_NONE && m_userPointer.at(i)->m_externalControlled == false)
					{
						if(m_userPointer.at(i)->m_vectorIndex > removedIndex)
							m_userPointer.at(i)->m_vectorIndex--;
					}
				}
			}
			else if(userPointer->m_externalControlled == true)
			{
				delete m_externallyControlled.at(removedIndex);
				m_externallyControlled.erase(m_externallyControlled.begin() + removedIndex);

				delete userPointer;
				m_userPointer.erase(m_userPointer.begin() + p_objectHandle);

				for(unsigned int i = p_objectHandle; i < m_userPointer.size(); i++)
				{

					m_userPointer.at(i)->m_id[0] --;
					if(m_userPointer.at(i)->m_externalControlled == true && m_userPointer.at(i)->m_type != PhysicsType::TYPE_PLAYER && m_userPointer.at(i)->m_type != PhysicsType::TYPE_RAGDOLL && m_userPointer.at(i)->m_shape != PhysicsShape::SHAPE_NONE )
					{
						if(m_userPointer.at(i)->m_vectorIndex > removedIndex)
							m_userPointer.at(i)->m_vectorIndex--;
					}
				}
			}
		}
	}


	void RootPhysics::RemoveAll()
	{
		for(unsigned int i = 0; i < m_ragdolls.size(); i++)
		{
			Ragdoll::Ragdoll* temp = m_ragdolls[i];
			delete temp;
		}
		for(unsigned int i = 0; i < m_shapelessObjects.size(); i++)
		{
			ShapelessObject* temp = m_shapelessObjects[i];
			delete temp;
		}
		for(unsigned int i = 0; i < m_externallyControlled.size(); i++)
		{
			ObjectController* temp = m_externallyControlled[i];
			delete temp;
		}
		for(int i = m_dynamicWorld->getNumCollisionObjects()-1; i>=0; i--)
		{
			btCollisionObject* obj = m_dynamicWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			if(body && body->getCollisionShape())
			{
				btCollisionShape* temp = body->getCollisionShape();
				delete temp;
			}
			m_dynamicWorld->removeCollisionObject( obj );
			delete obj;
		}
		for(unsigned int i = 0; i < m_playerObjects.size(); i++)
		{
			KinematicController* temp = m_playerObjects[i];
			delete temp;
		}
		for(unsigned int i = 0; i < m_userPointer.size(); i++)
		{
			CustomUserPointer* temp = m_userPointer[i];
			delete temp;
		}
		m_ragdolls.clear();
		m_externallyControlled.clear();
		m_shapelessObjects.clear();
		m_dynamicObjects.clear();
		m_playerObjects.clear();
		m_userPointer.clear();

	}

	int* RootPhysics::CreateHandle( void* p_entity, PhysicsType::PhysicsType p_physicsType, bool p_externalControlled )
	{
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex = -1;
		m_userPointer.push_back(userPointer);
		userPointer->m_type = p_physicsType;
		userPointer->m_id = new int();
		userPointer->m_externalControlled = p_externalControlled;
		userPointer->m_entity = p_entity;
		*(userPointer->m_id) = m_userPointer.size()-1;

		return userPointer->m_id;
	}

	


	void RootPhysics::BindSphereShape( int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation, float p_radius, float p_mass, bool p_collideWithWorld, bool p_collidesWithStatic)
	{
		
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btTransform trans = btTransform(quat, pos );
		btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
		ShapeStruct temp;
		temp.m_radius = p_radius;
		temp.m_type = PhysicsShape::SHAPE_SPHERE;
		btCollisionShape* shape = CreateShape(temp);

		m_userPointer.at(p_objectHandle)->m_shape = temp.m_type;

		btVector3 fallInertia = btVector3(0,0,0);
		if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_STATIC)
		{
			shape->calculateLocalInertia(p_mass, fallInertia);
		}
		if(!m_userPointer.at(p_objectHandle)->m_externalControlled) //if physics driven, i.e a rigidbody 
		{
			btRigidBody* body = new btRigidBody(p_mass, motionstate , shape, fallInertia);


			AddRigidBody(p_objectHandle, body, p_collideWithWorld, p_collidesWithStatic);	

			return;
		}
		else //Create a controller
		{
			AddController(p_objectHandle, shape, p_collideWithWorld, p_collidesWithStatic, trans);
		}
	}

	void RootPhysics::BindCylinderShape( int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation, float p_height, float p_radius, float p_mass, bool p_collideWithWorld, bool p_collidesWithStatic)
	{
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btTransform trans = btTransform(quat, pos );
		btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
		ShapeStruct temp;
		temp.m_radius = p_radius;
		temp.m_height = p_height;
		temp.m_type = PhysicsShape::SHAPE_CYLINDER;
		btCollisionShape* shape = CreateShape(temp);

		m_userPointer.at(p_objectHandle)->m_shape = temp.m_type;

		btVector3 fallInertia = btVector3(0,0,0);
		if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_STATIC)
		{
			shape->calculateLocalInertia(p_mass, fallInertia);
		}
		if(!m_userPointer.at(p_objectHandle)->m_externalControlled) //if physics driven, i.e a rigidbody 
		{

			btRigidBody* body = new btRigidBody(p_mass, motionstate , shape,fallInertia);
			AddRigidBody(p_objectHandle, body, p_collideWithWorld, p_collidesWithStatic); 
			return;
		}
		else //Create a controller
		{
			AddController(p_objectHandle, shape, p_collideWithWorld, p_collidesWithStatic, trans);
		}
	}

	void RootPhysics::BindConeShape( int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation, float p_height, float p_radius, float p_mass, bool p_collideWithWorld, bool p_collidesWithStatic)
	{
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btTransform trans = btTransform(quat, pos );
		btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
		ShapeStruct temp;
		temp.m_radius = p_radius;
		temp.m_height = p_height;
		temp.m_type = PhysicsShape::SHAPE_CONE;
		btCollisionShape* shape = CreateShape(temp);
		btVector3 fallInertia = btVector3(0,0,0);

		m_userPointer.at(p_objectHandle)->m_shape = temp.m_type;

		if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_STATIC)
		{
			shape->calculateLocalInertia(p_mass, fallInertia);
		}
		if(!m_userPointer.at(p_objectHandle)->m_externalControlled) //if physics driven, i.e a rigidbody 
		{

			btRigidBody* body = new btRigidBody(p_mass, motionstate , shape,fallInertia);
			AddRigidBody(p_objectHandle, body, p_collideWithWorld, p_collidesWithStatic); 
			return;
		}
		else //Create a controller
		{
			AddController(p_objectHandle, shape, p_collideWithWorld, p_collidesWithStatic, trans);
		}
	}

	void RootPhysics::BindMeshShape( int p_objectHandle, std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, glm::vec3 p_scale, float p_mass, bool p_collideWithWorld, bool p_collidesWithStatic)
	{
		btVector3 pos;
		pos.setX( p_position[0]);
		pos.setY( p_position[1]);
		pos.setZ( p_position[2]);
		btQuaternion quat = btQuaternion(p_rotation[0], p_rotation[1], p_rotation[2],p_rotation[3]);
		btTransform trans = btTransform(quat, pos );
		btDefaultMotionState* motionstate = new btDefaultMotionState(trans);
		ShapeStruct temp;
		temp.m_modelHandle = p_modelHandle;
		temp.m_scale = p_scale;
		temp.m_type = PhysicsShape::SHAPE_CUSTOM_MESH;
		btCollisionShape* shape = CreateShape(temp);

		m_userPointer.at(p_objectHandle)->m_shape = temp.m_type;

		btVector3 fallInertia = btVector3(0,0,0);
		if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_STATIC)
		{
			shape->calculateLocalInertia(p_mass, fallInertia);
		}
		if(!m_userPointer.at(p_objectHandle)->m_externalControlled) //if physics driven, i.e a rigidbody 
		{

			btRigidBody* body = new btRigidBody(p_mass, motionstate , shape, fallInertia);
			AddRigidBody(p_objectHandle, body, p_collideWithWorld, p_collidesWithStatic); 
			return;
		}
		else //Create a controller
		{
			AddController(p_objectHandle, shape, p_collideWithWorld, p_collidesWithStatic, trans);
		}
	}

	void RootPhysics::BindHullShape( int p_objectHandle, std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, float p_mass, bool p_collideWithWorld, bool p_collidesWithStatic)
	{
		btRigidBody* body = CreateMesh(p_modelHandle, p_position, p_rotation, p_mass);
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_STATIC)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		m_dynamicWorld->addRigidBody(body);
		m_dynamicObjects.push_back(body);
		m_userPointer.at(p_objectHandle)->m_vectorIndex = m_dynamicObjects.size()-1;
		body->setUserPointer((void*)m_userPointer.at(p_objectHandle));
		m_userPointer.at(p_objectHandle)->m_modelHandle = p_modelHandle;
	}
	void RootPhysics::BindNoShape( int p_objectHandle, glm::vec3 p_position, glm::quat p_rotation )
	{
		ShapelessObject* object = new ShapelessObject();
		object->SetPos(p_position);
		object->SetOrientation(p_rotation);

		m_userPointer.at(p_objectHandle)->m_shape = PhysicsShape::SHAPE_NONE;

		m_shapelessObjects.push_back(object);
		m_userPointer.at(p_objectHandle)->m_vectorIndex = m_shapelessObjects.size()-1;
	}

	//Creates an impassable plane
	void RootPhysics::CreatePlane(glm::vec3 p_normal, glm::vec3 p_position)
	{
		btCollisionShape* plane = new btStaticPlaneShape(btVector3(p_normal[0],p_normal[1],p_normal[2]), 0);
		btDefaultMotionState* planeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(p_position[0],p_position[1],p_position[2])));
		btRigidBody::btRigidBodyConstructionInfo planeRigidbodyCI(0, planeMotionState, plane, btVector3(0, 0, 0));
		btRigidBody* planeBody = new btRigidBody(planeRigidbodyCI);
		planeBody->setCollisionFlags(planeBody->getCollisionFlags() /*| btRigidBody::CF_DISABLE_VISUALIZE_OBJECT*/);
		m_dynamicWorld->addRigidBody(planeBody);
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex = -1;
		m_userPointer.push_back(userPointer);
		userPointer->m_type = PhysicsType::TYPE_STATIC;
		userPointer->m_id = new int();
		userPointer->m_entity = nullptr;
		*(userPointer->m_id) = m_userPointer.size()-1;
		planeBody->setUserPointer((void*)userPointer);

	}


	btRigidBody* RootPhysics::CreateMesh( std::string p_modelHandle, glm::vec3 p_position, glm::quat p_rotation, float p_mass )
	{
		PhysicsMeshInterface* tempMesh = g_resourceManager->GetPhysicsMesh(p_modelHandle);
		btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(tempMesh->GetNrOfFaces(), tempMesh->GetIndices(), 3*sizeof(int), tempMesh->GetNrOfPoints() , (btScalar*) tempMesh->GetMeshPoints(), 3*sizeof(float));

		btConvexShape* objectMeshShape = new btConvexTriangleMeshShape(indexVertexArray);
		//Cull unneccesary vertices to improve performance 
		btShapeHull* objectHull = new btShapeHull(objectMeshShape);

		btScalar margin = objectMeshShape->getMargin();
		objectHull->buildHull(margin);
		btConvexHullShape* simplifiedObject = new btConvexHullShape();
		for(int i = 0; i < objectHull->numVertices(); i++)
		{
			simplifiedObject->addPoint(objectHull->getVertexPointer()[i]);
		}	
		simplifiedObject->recalcLocalAabb();
		////Set Inertia
		btVector3 fallInertia =  btVector3(0,0,0);
		simplifiedObject->calculateLocalInertia(p_mass,fallInertia);

		//Set startpos and start rotation and bind them to a motionstate
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(p_position[0],p_position[1],p_position[2]));
		startTransform.setRotation(btQuaternion(p_rotation[0],p_rotation[1], p_rotation[2],p_rotation[3]));
		btDefaultMotionState* motionstate = new btDefaultMotionState(startTransform);
	
		//create a body
		btRigidBody::btRigidBodyConstructionInfo objectBodyInfo(p_mass, motionstate,simplifiedObject, fallInertia );
		btRigidBody* objectBody = new btRigidBody(objectBodyInfo);
		return objectBody;
	}

	btCollisionShape* RootPhysics::CreateShape( ShapeStruct p_shapeStruct )
	{
		if(p_shapeStruct.m_type == PhysicsShape::SHAPE_SPHERE)
		{
			btCollisionShape* sphere = new btSphereShape(p_shapeStruct.m_radius);
			return sphere;
		}
		else if(p_shapeStruct.m_type == PhysicsShape::SHAPE_CONE)
		{
			btCollisionShape* cone = new btConeShape(p_shapeStruct.m_radius, p_shapeStruct.m_height);
			return cone;
		}
		else if(p_shapeStruct.m_type == PhysicsShape::SHAPE_CYLINDER)
		{
			btCollisionShape* cylinder = new btCylinderShape(btVector3(p_shapeStruct.m_radius, p_shapeStruct.m_height, p_shapeStruct.m_radius));
			return cylinder;
		}
		else if(p_shapeStruct.m_type == PhysicsShape::SHAPE_HULL)
		{
			PhysicsMeshInterface* tempMesh = g_resourceManager->GetPhysicsMesh(p_shapeStruct.m_modelHandle);
			btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(tempMesh->GetNrOfFaces(), tempMesh->GetIndices(), 3*sizeof(int), tempMesh->GetNrOfPoints() , (btScalar*) tempMesh->GetMeshPoints(), 3*sizeof(float));

			btConvexShape* objectMeshShape = new btConvexTriangleMeshShape(indexVertexArray);
			//Cull unneccesary vertices to improve performance 
			btShapeHull* objectHull = new btShapeHull(objectMeshShape);
			btScalar margin = objectMeshShape->getMargin();
			objectHull->buildHull(margin);
			btConvexHullShape* simplifiedObject = new btConvexHullShape();
			for(int i = 0; i < objectHull->numVertices(); i++)
			{
				simplifiedObject->addPoint(objectHull->getVertexPointer()[i]);
			}	
			simplifiedObject->recalcLocalAabb();
			simplifiedObject->setLocalScaling(btVector3(p_shapeStruct.m_scale.x, p_shapeStruct.m_scale.y, p_shapeStruct.m_scale.z));
			
			return (btCollisionShape*)simplifiedObject;
		}
		else if(p_shapeStruct.m_type == PhysicsShape::SHAPE_CUSTOM_MESH)
		{
			PhysicsMeshInterface* tempMesh = g_resourceManager->GetPhysicsMesh(p_shapeStruct.m_modelHandle);
			if(tempMesh == NULL)
			{
				g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Mesh for %s not found", p_shapeStruct.m_modelHandle);
				return nullptr;
			}
			btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(tempMesh->GetNrOfFaces(), tempMesh->GetIndices(), 3* sizeof(int), tempMesh->GetNrOfPoints() , (btScalar*) tempMesh->GetMeshPoints(), 3*sizeof(btScalar));
			btScalar mass = 0; //mass is always 0 for static objects

			btBvhTriangleMeshShape* objectMeshShape = new btBvhTriangleMeshShape(indexVertexArray, true);
			objectMeshShape->setLocalScaling(btVector3(p_shapeStruct.m_scale.x, p_shapeStruct.m_scale.y, p_shapeStruct.m_scale.z));
			btTriangleInfoMap* trinfoMap = new btTriangleInfoMap();
			trinfoMap->m_edgeDistanceThreshold = 0.01f;
			trinfoMap->m_equalVertexThreshold = 0.01f;
			btGenerateInternalEdgeInfo(objectMeshShape, trinfoMap);
			return (btCollisionShape*)objectMeshShape;
		}
		else
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Shape %d not specified", p_shapeStruct.m_type);
			return nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Use this to add a static object to the World, i.e trees, rocks and the ground. Both position and rotation are vec3
	

	

	int* RootPhysics::AddPlayerObjectToWorld(std::string p_modelHandle, void* p_entity, glm::vec3 p_position, glm::quat p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight, std::map<void*, RootForce::CollisionInfo>* p_collisions)
	{
		KinematicController* player = new KinematicController();
		player->Init(m_dynamicWorld, 0, 0, 3*sizeof(int), 
			0 , (btScalar*) 0, 3*sizeof(float), p_position, p_rotation, p_mass, p_maxSpeed, p_modelHeight, p_stepHeight );
		m_playerObjects.push_back(player);
		CustomUserPointer* userPointer = new CustomUserPointer();
		userPointer->m_vectorIndex =  m_playerObjects.size()-1;
		m_userPointer.push_back(userPointer);
		userPointer->m_id = new int();
		*(userPointer->m_id) = m_userPointer.size()-1;
		userPointer->m_type = PhysicsType::TYPE_PLAYER;
		userPointer->m_modelHandle = p_modelHandle;
		userPointer->m_entity = p_entity;
		userPointer->m_collisions = p_collisions;
		userPointer->m_externalControlled = true;
		player->SetUserPointer((void*)userPointer);
		player->SetDebugDrawer(m_debugDrawer);
		return userPointer->m_id;
	}
	

	

	void RootPhysics::PlayerJump( int p_objectHandle, float p_jumpForce )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		
		m_playerObjects.at(index)->Jump(p_jumpForce);
	}
	
	void RootPhysics::PlayerJumpBoost( int p_objectHandle, float p_boostPower )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		m_playerObjects.at(index)->JumpBoost(p_boostPower);
	}

	void RootPhysics::KnockbackObject( int p_objectHandle, glm::vec3 p_pushDirection, float p_pushForce )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		btVector3 temp = btVector3(p_pushDirection[0], p_pushDirection[1], p_pushDirection[2]);
		temp.normalize();
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->Knockback(temp, p_pushForce);
		}
		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			m_dynamicObjects.at(index)->setLinearVelocity(temp*p_pushForce);
		}
		else 
		{
			return; //Controlled object, how to knockback? Velocity variable?
		}
	}

	void RootPhysics::StopPlayerKnockback( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->Knockback(btVector3(0,0,0), 0);
		}
	}

	bool RootPhysics::DoesObjectExist( int p_objectHandle )
	{
		if(m_userPointer.size() == 0 || (unsigned int)p_objectHandle >= m_userPointer.size() || p_objectHandle < 0)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing user pointer at index %d", p_objectHandle);
			return false;
		}
		else
		{
			if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
			{
				if(m_playerObjects.size() == 0 || (unsigned int)m_userPointer.at(p_objectHandle)->m_vectorIndex > m_playerObjects.size()-1 || m_userPointer.at(p_objectHandle)->m_vectorIndex < 0)
				{
					g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing player object at index %d", m_userPointer.at(p_objectHandle)->m_vectorIndex);
					return false;
				}
			}
			else if (m_userPointer.at(p_objectHandle)->m_shape == PhysicsShape::SHAPE_NONE)
			{
				if(m_shapelessObjects.size() == 0 || (unsigned int)m_userPointer.at(p_objectHandle)->m_vectorIndex > m_shapelessObjects.size()-1 || m_userPointer.at(p_objectHandle)->m_vectorIndex < 0)
				{
					g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing shapeless object at index %d", m_userPointer.at(p_objectHandle)->m_vectorIndex);
					return false;
				}
			}
			else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_RAGDOLL)
			{
				if(m_ragdolls.size() == 0 || (unsigned int)m_userPointer.at(p_objectHandle)->m_ragdollIndex > m_ragdolls.size()-1 ||  m_userPointer.at(p_objectHandle)->m_ragdollIndex < 0)
				{
					g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing ragdoll  at index %d", m_userPointer.at(p_objectHandle)->m_ragdollIndex);
					return false;
				}
			}
			else if(m_userPointer.at(p_objectHandle)->m_externalControlled)
			{
				if(m_externallyControlled.size() == 0  || (unsigned int)m_userPointer.at(p_objectHandle)->m_vectorIndex > m_externallyControlled.size()-1 || m_userPointer.at(p_objectHandle)->m_vectorIndex < 0)
				{
					g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing externally controlled object at index %d", m_userPointer.at(p_objectHandle)->m_vectorIndex);
					return false;
				}
			}
			
			else
			{
				if(m_dynamicObjects.size() == 0 || (unsigned int)m_userPointer.at(p_objectHandle)->m_vectorIndex > m_dynamicObjects.size()-1 || m_userPointer.at(p_objectHandle)->m_vectorIndex < 0)
				{
					g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing dynamic object at index %d", m_userPointer.at(p_objectHandle)->m_vectorIndex);
					return false;
				}
			}
		}
		return true;
	}

	bool RootPhysics::DoesUserPointerExist( int p_objectHandle )
	{
		if(m_userPointer.size() == 0 || (unsigned int)p_objectHandle >= m_userPointer.size() || p_objectHandle < 0)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attemting to access non existing user pointer at index %d", p_objectHandle);
			return false;
		}
		return true;
	}

	void RootPhysics::EnableDebugDraw( bool p_enabled )
	{
		if(p_enabled == true)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Turned physics debugdraw on ");
			
		}
		else
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Turned physics debugdraw off");
			
		}
		
		m_debugDrawEnabled = p_enabled;
	}

	glm::vec3 RootPhysics::GetPos(int p_objectHandle)
	{
		if(!DoesObjectExist(p_objectHandle))
			return glm::vec3(0,0,0);

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		glm::vec3 retVal;
		btVector3 temp;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			temp = m_playerObjects.at(index)->GetPosition();
			retVal[0] = temp.getX();
			retVal[1] = temp.getY();
			retVal[2] = temp.getZ();
		}
		
		else if (m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_RAGDOLL)
		{
			unsigned int indexrag = m_userPointer.at(p_objectHandle)->m_ragdollIndex;
			temp = m_ragdolls[indexrag]->GetPos();
			retVal[0] = temp.getX();
			retVal[1] = temp.getY();
			retVal[2] = temp.getZ();	 
		}
		else if (m_userPointer.at(p_objectHandle)->m_shape == PhysicsShape::SHAPE_NONE)
		{
			retVal = m_shapelessObjects.at(m_userPointer.at(p_objectHandle)->m_vectorIndex)->GetPos();
		}
		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled )
		{
			temp = m_dynamicObjects.at(index)->getWorldTransform().getOrigin();
			retVal[0] = temp.getX();
			retVal[1] = temp.getY();
			retVal[2] = temp.getZ();		
		}
		else
		{
			retVal = m_externallyControlled.at(index)->GetPos();
			
			
		}
		
		return retVal;

	}

	float RootPhysics::GetMass( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return -1;
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;

		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			return m_playerObjects.at(index)->GetMass();
		}

		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			return 1/m_dynamicObjects.at(index)->getInvMass();
		}
		else
		{
			return 0;
		}
	
		return -1 ;
		

	}
	int RootPhysics::GetType(int p_objectHandle)
	{
		if(!DoesUserPointerExist(p_objectHandle))
			return -1;
		return m_userPointer.at(p_objectHandle)->m_type;

	}

	float RootPhysics::GetStepHeight( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return -1;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			return m_playerObjects.at(index)->GetStepHeight();
		}
		return -1;
	}
	glm::quat RootPhysics::GetOrientation( int p_objectHandle)
	{
		if(!DoesObjectExist(p_objectHandle))
			return glm::quat(0,0,0,0);

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		glm::quat retVal;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			btQuaternion temp = m_playerObjects.at(index)->GetOrientation();
			retVal[0] = temp.x();
			retVal[1] = temp.y();
			retVal[2] = temp.z();
			retVal[3] = temp.w();
			
		}
		
		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			btRigidBody* body = m_dynamicObjects.at(index);
			btTransform transform;
			transform = body->getWorldTransform();
			
			retVal[0] = transform.getRotation().x();
			retVal[1] = transform.getRotation().y();
			retVal[2] = transform.getRotation().z();
			retVal[3] = transform.getRotation().w();
		}
		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_RAGDOLL)
		{
			index = m_userPointer.at(p_objectHandle)->m_ragdollIndex;
			btQuaternion temp = m_ragdolls.at(index)->GetOrientation();
			retVal[0] = temp.w();
			retVal[1] = temp.x();
			retVal[2] = temp.y();
			retVal[3] = temp.z();
			//return glm::quat(0,0,0,1);
		}
		else if (m_userPointer.at(p_objectHandle)->m_shape == PhysicsShape::SHAPE_NONE)
		{
			retVal = m_shapelessObjects.at(m_userPointer.at(p_objectHandle)->m_vectorIndex)->GetOrientation();
		}
		else //Todo: return externallycontrolled orientation if needed.
		{
			btQuaternion temp = m_externallyControlled.at(index)->GetOrientation();
			
			retVal[0] = temp.y();
			retVal[1] = temp.z();
			retVal[2] = temp.w();
			retVal[3] = temp.x();
		}
		return retVal;
	}

	float RootPhysics::GetMaxSpeed( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return -1;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			return m_playerObjects.at(index)->GetMaxSpeed();
		}
		return -1;
	}
	float RootPhysics::GetModelHeight( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return -1;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			unsigned index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			return m_playerObjects.at(index)->GetModelHeight();
		}
		return -1;
	}	

	std::string RootPhysics::GetPhysicsModelHandle( int p_objectHandle )
	{
		if(!DoesUserPointerExist(p_objectHandle))
			return "";
		return m_userPointer.at(p_objectHandle)->m_modelHandle;
		
	}

	std::map<void*, RootForce::CollisionInfo>* RootPhysics::GetCollisionVector( int p_objectHandle )
	{
		return (m_userPointer.at(p_objectHandle)->m_collisions);
	}


	glm::vec3 RootPhysics::GetVelocity( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return glm::vec3(0,0,0);
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			//No, player doesn't have a constant velocity, and i'm gonna go ahead and guess that ability controllers won't either
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Player don't have a velocity in physics engine");
			return glm::vec3(0,0,0);
		}
		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled)
		{

			btVector3 temp = m_dynamicObjects.at(index)->getLinearVelocity();

			return glm::vec3(temp[0], temp[1], temp[2]);
		}
		else if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_RAGDOLL && m_userPointer.at(p_objectHandle)->m_shape != PhysicsShape::SHAPE_NONE)
		{
			//g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Externally controlled objects don't have a velocity in physics engine");
			return glm::vec3(0,0,0);
		}
		//return glm::vec3(0,0,0);
	}
	glm::vec3 RootPhysics::GetPlayerKnockbackVector( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return glm::vec3(0,0,0);

		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			btVector3 temp = m_playerObjects.at(index)->GetKnockbackVector();
			return glm::vec3(temp.x(), temp.y() , temp.z());
		}
		return glm::vec3(0, 0, 0);  //Only players have a knockbackvector
	}

	float RootPhysics::GetPlayerVerticalVelocity( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return 0;

		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			return m_playerObjects.at(index)->GetVerticalVelocity();			
		}
		return 0;
	}

	RootPhysics* RootPhysics::GetInstance()
	{
		if(!s_physicsInstance)
		{
			s_physicsInstance = new RootPhysics();
		}
		return s_physicsInstance;
	}

	void RootPhysics::SetVelocity( int p_objectHandle, glm::vec3 p_velocity )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		btVector3 temp (p_velocity[0], p_velocity[1], p_velocity[2]);
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->Walk(p_velocity, m_dt);
		}
		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			
			m_dynamicObjects.at(index)->setLinearVelocity(temp);
		}
		else if(m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_RAGDOLL && m_userPointer.at(p_objectHandle)->m_shape != PhysicsShape::SHAPE_NONE)
		{
			m_externallyControlled.at(index)->Move(p_velocity, m_dynamicWorld);
		}
	}

	void RootPhysics::SetOrientation( int p_objectHandle, glm::quat p_objectOrientation )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->SetOrientation(p_objectOrientation);
		}
		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			float x,y,z, w;
			btRigidBody* body = m_dynamicObjects.at(index);
		
			x = p_objectOrientation[0];
			y = p_objectOrientation[1];
			z = p_objectOrientation[2];
			w = p_objectOrientation[3];
			body->getMotionState()->setWorldTransform(btTransform(btQuaternion(x,y,z, w), body->getWorldTransform().getOrigin()));
			
		}
		else if (m_userPointer.at(p_objectHandle)->m_shape == PhysicsShape::SHAPE_NONE)
		{
			m_shapelessObjects.at(m_userPointer.at(p_objectHandle)->m_vectorIndex)->SetOrientation(p_objectOrientation);
		}
		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_RAGDOLL)
		{
			index = m_userPointer.at(p_objectHandle)->m_ragdollIndex;
			//	m_ragdolls.at(index)->SetOrientation(p_objectOrientation);
		}
		else if(m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			glm::quat rot = glm::angleAxis(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)); //here be trouble
			
			m_externallyControlled.at(index)->SetOrientation(p_objectOrientation/**rot*/);
		}
		
	}


	void RootPhysics::SetGravity( int p_objectHandle, glm::vec3 p_gravity )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->SetGravity(p_gravity[1]);		
		}
		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled && m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_RAGDOLL)
		{
			
			btVector3 gravity;
			gravity.setX(p_gravity[0]);
			gravity.setY(p_gravity[1]);
			gravity.setZ(p_gravity[2]);
			m_dynamicObjects.at(index)->setGravity(gravity);
			m_dynamicObjects.at(index)->applyGravity();
		}
		else if(m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			//Gravity doesn't exist in objectcontroller now, add if needed
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Changing gravity is not supported for externally controlled objects");
		}
		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_RAGDOLL)
		{
			btVector3 gravity;	
			gravity.setX(p_gravity[0]);
			gravity.setY(p_gravity[1]);
			gravity.setZ(p_gravity[2]);
			index = m_userPointer.at(p_objectHandle)->m_ragdollIndex;
			if(index != -1)
				m_ragdolls.at(index)->SetGravity(gravity);
		}
	}

	//TODO : Remake to work for all objecttypes
	void RootPhysics::SetMass( int p_objectHandle, float p_mass )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		if (m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_STATIC)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Can't change mass for static objects");
			return;
		}
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
			m_playerObjects.at(index)->SetMass(p_mass);
		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			btVector3 fallInertia =  btVector3(0,0,0);
			m_dynamicObjects.at(index)->getCollisionShape()->calculateLocalInertia(p_mass, fallInertia);
			m_dynamicObjects.at(index)->setMassProps(p_mass, fallInertia);
		}
		else
		{
			//Mass doesn't exist in objectcontroller now, add if needed
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Changing mass is not supported for externally controlled objects");
		}
		
		
	}

	void RootPhysics::SetRestitution( int p_objectHandle, float p_resitution )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY || m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_STATIC)
		{
			int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			m_dynamicObjects.at(index)->setRestitution(p_resitution);
		}
	}

	void RootPhysics::Move( int p_objectHandle , glm::vec3 p_position, float p_dt )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;

		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		btVector3 temp (p_position[0], p_position[1], p_position[2]);
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->Move(p_position, p_dt);
		}
		else if (m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_RAGDOLL)
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Moving ragdolls not supported");
			//index = m_userPointer.at(p_objectHandle)->m_ragdollIndex;
			//m_ragdolls.at(index)->
		}
		else if(!m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			m_dynamicObjects.at(index)->getWorldTransform().setOrigin(temp);
		}
		else
		{
			m_externallyControlled.at(index)->Move(p_position, m_dynamicWorld);
		}
	}

	void RootPhysics::SetCollisionContainer( int p_objectHandle ,std::map<void*, RootForce::CollisionInfo>* p_collisions )
	{
		m_userPointer.at(p_objectHandle)->m_collisions = p_collisions;
	}

	void RootPhysics::SetPosition( int p_objectHandle, glm::vec3 p_position )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		btVector3 temp (p_position[0], p_position[1], p_position[2]);
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			m_playerObjects.at(index)->SetPosition(temp);
		}
		else if (m_userPointer.at(p_objectHandle)->m_shape == PhysicsShape::SHAPE_NONE)
		{
			m_shapelessObjects.at(m_userPointer.at(p_objectHandle)->m_vectorIndex)->SetPos(p_position);
		}
		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY && !m_userPointer.at(p_objectHandle)->m_externalControlled)
		{
			m_dynamicObjects.at(index)->getWorldTransform().setOrigin(temp);
		}
		else if(m_userPointer.at(p_objectHandle)->m_externalControlled && m_userPointer.at(p_objectHandle)->m_type != PhysicsType::TYPE_RAGDOLL)
		{
			m_externallyControlled.at(index)->SetPosition(p_position);
		}
		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_RAGDOLL)
		{
			/*m_userPointer.at(p_objectHandle)->m_type = PhysicsType::TYPE_PLAYER;
			m_playerObjects.at(index)->SetPosition(temp);*/
			
		}
	}


	bool RootPhysics::IsOnGround( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return false;
		unsigned int index = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_PLAYER)
		{
			return m_playerObjects.at(index)->IsOnGround();
		} 
		return false;
	}

	void RootPhysics::BuildRagdoll( int p_objectHandle, glm::mat4 p_bones[20], aiNode* p_rootNode, std::map<std::string, int>  p_nameToIndex, glm::mat4 p_boneOffset[20], glm::vec3 p_right)
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		int index  = m_userPointer.at(p_objectHandle)->m_ragdollIndex;
		int indexplayer = m_userPointer.at(p_objectHandle)->m_vectorIndex;
		
		if(index  == -1)
		{
			
			Ragdoll::Ragdoll* ragdoll = new Ragdoll::Ragdoll(m_dynamicWorld);
			btTransform trans = m_playerObjects.at(indexplayer)->GetTransform();
			float x,y,z,w;
			x = trans.getRotation().w();
			y = trans.getRotation().x();
			z = trans.getRotation().y();
			w = trans.getRotation().z();
			trans.setRotation(btQuaternion(x,y,z,w));
			trans.setOrigin(trans.getOrigin() + btVector3(0, -0.5f, 0));
			
			float data[16];
			glm::mat4 matrix;
			trans.getOpenGLMatrix(data);
			matrix = glm::make_mat4(data);
			ragdoll->BuildRagdoll(p_bones, p_rootNode, p_nameToIndex, matrix, p_boneOffset, p_right);
			//Deactivate the player and 
			m_ragdolls.push_back(ragdoll);
			m_userPointer.at(p_objectHandle)->m_ragdollIndex = m_ragdolls.size()-1;
			m_userPointer.at(p_objectHandle)->m_type = PhysicsType::TYPE_RAGDOLL;
			m_playerObjects.at(indexplayer)->Deactivate();
			index  = m_userPointer.at(p_objectHandle)->m_ragdollIndex;
			btVector3 knockback = m_playerObjects.at(indexplayer)->GetKnockbackVector();
			if(knockback.length() > 10)
				m_ragdolls.at(index)->SetVelocity(knockback );
			else
			{
				m_ragdolls.at(index)->SetVelocity(btVector3(knockback.x() * 10, knockback.y(), knockback.z() * 10));
			}
			
		}	
		
	}

	glm::mat4* RootPhysics::GetBones( int p_objectHandle)
	{
		if(!DoesObjectExist(p_objectHandle))
			return nullptr;
		int index  = m_userPointer.at(p_objectHandle)->m_ragdollIndex;
		if(index  != -1)
		{
			return m_ragdolls.at(index)->GetBones();
		}
		else
		{
			g_context.m_logger->LogText(LogTag::PHYSICS, LogLevel::WARNING, "Attempting to get bones from nonexisting ragdoll %d with objecthandle: %d", index, p_objectHandle);
		}
		return nullptr;
	}

	void RootPhysics::DeactivateRagdoll( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		int index = m_userPointer.at(p_objectHandle)->m_ragdollIndex;
		//Ragdoll
		
		if(index != -1 && m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_RAGDOLL)
		{
			
			delete m_ragdolls.at(index);
			m_ragdolls.erase(m_ragdolls.begin() + index);
			m_userPointer.at(p_objectHandle)->m_ragdollIndex = -1;
			int playerIndex = m_userPointer.at(p_objectHandle)->m_vectorIndex;
			m_userPointer.at(p_objectHandle)->m_type = PhysicsType::TYPE_PLAYER;
			m_playerObjects.at(playerIndex)->Activate();

			for(unsigned int i = 0; i < m_userPointer.size(); i++)
			{
				if(m_userPointer.at(i)->m_type == PhysicsType::TYPE_PLAYER ||m_userPointer.at(i)->m_type == PhysicsType::TYPE_RAGDOLL)
				{
					
					if(m_userPointer.at(i)->m_ragdollIndex > index)
						m_userPointer.at(i)->m_ragdollIndex--;
				}

			}
		}
	}

	bool RootPhysics::IsRagdoll( int p_objecthandle )
	{
		assert(DoesObjectExist(p_objecthandle));
		return m_userPointer.at(p_objecthandle)->m_type == PhysicsType::TYPE_RAGDOLL;
	}

	float RootPhysics::RayTest( glm::vec3 p_startPos, glm::vec3 p_endPos )
	{
		RayAgainstStaticCast rayResult;
		m_dynamicWorld->rayTest(btVector3(p_startPos[0], p_startPos[1], p_startPos[2]), btVector3(p_endPos[0], p_endPos[1], p_endPos[2]), rayResult);

		return rayResult.m_closestHitFraction;
	}

	void* RootPhysics::CastRay( int p_objectHandle, glm::vec3 p_startPos, glm::vec3 p_direction, float p_length, glm::vec3* p_hitPos, bool p_isAbility )
	{
		RayAbilityCast rayResult;
		glm::vec3 end = p_startPos + glm::normalize(p_direction) * p_length;
		m_dynamicWorld->rayTest(btVector3(p_startPos[0], p_startPos[1], p_startPos[2]), btVector3(end[0], end[1], end[2]), rayResult);

		if(!rayResult.hasHit())
		{
			*p_hitPos = p_startPos + p_direction * p_length;
			return nullptr;
		}
		
		glm::vec3 castVector = end - p_startPos;
		glm::vec3 relativePosition = castVector * rayResult.m_closestHitFraction;
		*p_hitPos = p_startPos + relativePosition;
		glm::vec3 hitPos = p_startPos + relativePosition;
		

		if(p_isAbility)
		{
			RootForce::CollisionInfo info;
			info.m_collisionPosition = hitPos;
			m_userPointer.at(p_objectHandle)->m_collisions->insert(std::make_pair(rayResult.m_hit->m_entity, info));

			return rayResult.m_hit->m_entity;
		}
		else
		{
			if (rayResult.m_hit->m_type == PhysicsType::TYPE_PLAYER)
			{
				return rayResult.m_hit->m_entity;
			}
			else
			{
				return nullptr;
			}
		}
	}

	void RootPhysics::RadiusCheck( int p_objectHandle, glm::vec3 p_pos, float p_radius )
	{
		//if(!DoesObjectExist(p_objectHandle))
			//return;
		btVector3 pos = btVector3(p_pos[0], p_pos[1], p_pos[2]);
		for (unsigned int i = 0; i < m_playerObjects.size(); i++)
		{
			if (m_playerObjects.at(i)->GetPosition().distance(pos) < p_radius)
			{
				glm::vec3 pos(m_playerObjects.at(i)->GetPosition().x(), m_playerObjects.at(i)->GetPosition().y(), m_playerObjects.at(i)->GetPosition().z());
				RootForce::CollisionInfo info;
				info.m_collisionPosition = pos;
				m_userPointer.at(p_objectHandle)->m_collisions->insert(std::make_pair(((CustomUserPointer*)(m_playerObjects.at(i)->GetUserPointer()))->m_entity, info));
			}
		}
	}

	void* RootPhysics::GetPlayerAtAim( int p_objectHandle, glm::vec3 p_startPos, glm::vec3 p_direction, float p_length )
	{
		RayAbilityCast rayResult;
		//rayResult.m_caster = m_userPointer.at(p_objectHandle);
		//rayResult.from = p_startPos;
		glm::vec3 end = p_startPos + glm::normalize(p_direction) * p_length;
		//rayResult.to = end;
		m_dynamicWorld->rayTest(btVector3(p_startPos[0], p_startPos[1], p_startPos[2]), btVector3(end[0], end[1], end[2]), rayResult);
		if (rayResult.m_hit->m_type == PhysicsType::TYPE_PLAYER)
		{
			return rayResult.m_hit->m_entity;
		}
		return nullptr;
	}

	void RootPhysics::AddRigidBody( int p_objectHandle, btRigidBody* p_body, bool p_collideWithWorld, bool p_collidesWithStatic )
	{
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_STATIC)
		{
				p_body->setRestitution(0.8f);
				p_body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
		}
		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
		{
				p_body->setCollisionFlags( btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
				p_body->setActivationState(DISABLE_DEACTIVATION);
				p_body->setRestitution(0.7f);
				p_body->setCcdMotionThreshold(3);
				p_body->setCcdSweptSphereRadius(1.f);
		}
		
		
		if(!p_collideWithWorld)
			p_body->setCollisionFlags(p_body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		if(!p_collidesWithStatic)
		{
			m_dynamicWorld->addRigidBody(p_body, 1, -35); //DO NOT QUESTION WHY THIS WORKS
		}
		else
		{
			m_dynamicWorld->addRigidBody(p_body);
		}

		

		m_dynamicObjects.push_back(p_body);
		m_userPointer.at(p_objectHandle)->m_vectorIndex = m_dynamicObjects.size()-1;
		/*if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY)
			body->setActivationState(DISABLE_DEACTIVATION);*/
		p_body->setUserPointer((void*)m_userPointer.at(p_objectHandle));
	}
	
	void RootPhysics::AddController(int p_objectHandle, btCollisionShape* p_collisionShape, bool p_collideWithWorld, bool p_collidesWithStatic, const btTransform& p_transform)
	{
		btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
		ghostObject->setCollisionShape(p_collisionShape);
		ghostObject->setWorldTransform(p_transform);
		ghostObject->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK | btCollisionObject::CF_CHARACTER_OBJECT);
		
		
		if(!p_collideWithWorld)
			ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		
		
		ObjectController* controller = new ObjectController();
		controller->Init(ghostObject, (btConvexShape*)p_collisionShape, m_dynamicWorld);
		if(!p_collidesWithStatic)
		{
			m_dynamicWorld->addCollisionObject(ghostObject, btBroadphaseProxy::DefaultFilter, -35);
		}
		else if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITYSPAWN)
		{
			m_dynamicWorld->addCollisionObject(ghostObject, btBroadphaseProxy::DefaultFilter, -3);
		}
		else
		{
			m_dynamicWorld->addCollisionObject(ghostObject);
		}
		m_externallyControlled.push_back(controller);
		m_userPointer.at(p_objectHandle)->m_vectorIndex = m_externallyControlled.size()-1;
		ghostObject->setUserPointer((void*)m_userPointer.at(p_objectHandle));
		return;
	}

	void RootPhysics::LockYOrientation( int p_objectHandle )
	{
		if(!DoesObjectExist(p_objectHandle))
			return;
		if(m_userPointer.at(p_objectHandle)->m_type == PhysicsType::TYPE_ABILITY && !(m_userPointer.at(p_objectHandle)->m_externalControlled))
			m_dynamicObjects.at(m_userPointer.at(p_objectHandle)->m_vectorIndex)->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
	}

}
}

RootEngine::Physics::PhysicsInterface* CreatePhysics( RootEngine::SubsystemSharedContext p_context, Render::RendererInterface* p_renderer , RootEngine::ResourceManagerInterface* p_resourceManager)
{
	RootEngine::Physics::g_context = p_context;
	RootEngine::Physics::g_renderer = p_renderer;
	RootEngine::Physics::g_resourceManager = p_resourceManager;
	return RootEngine::Physics::RootPhysics::GetInstance();
}
