#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <QtCore/QStringList>
#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantEditorFactory>
#include <Qt/QtGui/QVector3D>
#include <Qt/QtWidgets/QMessageBox>
namespace AbilityEditorNameSpace
{


	namespace AbilityComponents
	{
		namespace ComponentType
		{
			enum ComponentType
			{
				TRANSFORM,
				COLLISION,
				ABILITYMODEL,
				COLLISIONSHAPE,
				ABILITYPARTICLE,
				PHYSICSCONTROLLED,
				OFFENSIVEABILITY,
				EXPLOSIVE,
				END_OF_ENUM
			};
		}
		const struct ComponentNameList
		{
			QStringList m_compNames;
			ComponentNameList()
			{
				m_compNames.append("Transform");
				m_compNames.append("Collision");
				m_compNames.append("Ability Model");
				m_compNames.append("Collision Shape");
				m_compNames.append("Ability Particle");
				m_compNames.append("Physics Controlled");
				m_compNames.append("Offensive Ability");
				m_compNames.append("Explosive");
			}
		} static g_componentNameList;
		//All components should inherit from the MainComponent struct
		struct MainComponent
		{
			ComponentType::ComponentType m_type;
			MainComponent(ComponentType::ComponentType p_type)
			{
				m_type = p_type;
			}
			virtual void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory) = 0;			
			virtual void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory) = 0;
		};
	
		////////////////////////////////////////////////////////On create components ////////////////////////////////////////////////////////
		struct Transform: MainComponent
		{ 
			QVector3D m_rotation;
			QVector3D m_scale;
			Transform(QVector3D p_rotation = QVector3D(0,0,0), QVector3D p_scale = QVector3D(1,1,1)): MainComponent(ComponentType::TRANSFORM)
			{
				m_rotation = p_rotation;
				m_scale = p_scale;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* rotation, *scale , *x, *y, *z;
				QList<QtProperty*> list;
				QString combinedValue;
				//Show rotation vector
				rotation = p_propMan->addProperty(QVariant::String, "Rotation");
				x = p_propMan->addProperty(QVariant::Double, "x");
				y = p_propMan->addProperty(QVariant::Double, "y");
				z = p_propMan->addProperty(QVariant::Double, "z");
				p_propMan->setValue(x, m_rotation.x());
				p_propMan->setValue(y, m_rotation.y());
				p_propMan->setValue(z, m_rotation.z());
				rotation->addSubProperty(x);
				rotation->addSubProperty(y);
				rotation->addSubProperty(z);
				list = rotation->subProperties();
				combinedValue = "(" + list.at(0)->valueText() + ", " + list.at(1)->valueText() + ", " + list.at(2)->valueText() + ")";
				p_propMan->setValue(rotation,combinedValue);

				//show Scale vector
				scale = p_propMan->addProperty(QVariant::String, "Scale");
				x = p_propMan->addProperty(QVariant::Double, "x");
				y = p_propMan->addProperty(QVariant::Double, "y");
				z = p_propMan->addProperty(QVariant::Double, "z");
				p_propMan->setValue(x, m_scale.x());
				p_propMan->setValue(y, m_scale.y());
				p_propMan->setValue(z, m_scale.z());		
				scale->addSubProperty(x);
				scale->addSubProperty(y);
				scale->addSubProperty(z);
				list = scale->subProperties();
				combinedValue = "(" + list.at(0)->valueText() + ", " + list.at(1)->valueText() + ", " + list.at(2)->valueText() + ")";
				p_propMan->setValue(scale,combinedValue);

				//Add to browser widget
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(rotation);
				p_propBrows->addProperty(scale);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				
				//0 skräp 1 2 3 x y z, 4 SKRÄP. 5 6 7
				subprops = props.at(0)->subProperties();
				for(int i = 0; i < subprops.size(); i++)
				{
					m_rotation[i] = atof(subprops.at(i)->valueText().toStdString().c_str());
				}
				subprops = props.at(1)->subProperties();
				for(int i = 0; i < subprops.size(); i++)
				{
					m_scale[i] = atof(subprops.at(i)->valueText().toStdString().c_str());
				}
			

			}
		};

		struct Collision : MainComponent
		{
			bool m_collidesWithWorld;
			Collision(bool p_colWithWorld = true) : MainComponent(ComponentType::COLLISION)
			{
				m_collidesWithWorld = p_colWithWorld;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* collidesWithWorld;

				collidesWithWorld = p_propMan->addProperty(QVariant::Bool, "Collides with world" );
				p_propMan->setValue(collidesWithWorld, m_collidesWithWorld);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(collidesWithWorld);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				m_collidesWithWorld = props.at(0)->valueText().compare("True") == 0 ? true : false;
			}
		};


		struct AbilityModel : MainComponent
		{
			std::string m_modelName;
			std::string m_material;
			std::string m_materialDiffuse;
			std::string m_materialSpecular;
			std::string m_materialNormal;
			std::string m_materialEffect;
			AbilityModel(std::string p_modelName = "", std::string p_material = "",
						std::string p_materialDiffuse = "", std::string p_materialSpecular = "",
						std::string p_materialNormal = "", std::string p_materialEffect = "") 
						: MainComponent(ComponentType::ABILITYMODEL)
			{
				m_modelName = p_modelName;
				m_material = p_material;
				m_materialDiffuse = p_materialDiffuse;
				m_materialSpecular = p_materialSpecular;
				m_materialNormal = p_materialNormal;
				m_materialEffect = p_materialEffect;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* modelname, *material, *diffuse, *specular, *normal, *effect ;

				modelname = p_propMan->addProperty(QVariant::String, "Model name" );
				p_propMan->setValue(modelname, m_modelName.c_str());
				material = p_propMan->addProperty(QVariant::String, "Material name");
				p_propMan->setValue(material, m_material.c_str());
				diffuse = p_propMan->addProperty(QVariant::String, "Diffuse name");
				p_propMan->setValue(diffuse, m_materialDiffuse.c_str());
				specular = p_propMan->addProperty(QVariant::String, "Specular name");
				p_propMan->setValue(specular, m_materialSpecular.c_str());
				normal = p_propMan->addProperty(QVariant::String, "normal name");
				p_propMan->setValue(normal,m_materialNormal.c_str());
				effect = p_propMan->addProperty(QVariant::String, "Effect name");
				p_propMan->setValue(effect, m_materialEffect.c_str());
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(modelname);
				p_propBrows->addProperty(material);
				p_propBrows->addProperty(diffuse);
				p_propBrows->addProperty(specular);
				p_propBrows->addProperty(normal);
				p_propBrows->addProperty(effect);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				m_modelName = props.at(0)->valueText().toStdString();
				m_material = props.at(1)->valueText().toStdString();
				m_materialDiffuse = props.at(2)->valueText().toStdString();
				m_materialSpecular = props.at(3)->valueText().toStdString();
				m_materialNormal = props.at(4)->valueText().toStdString();
				m_materialEffect =props.at(5)->valueText().toStdString();
			}
		};

		struct CollisionShape : MainComponent
		{
			enum shape
			{
				CONE,
				CYLINDER,
				SPHERE,
				MESH,
				END_OF_ENUM
			};
			
			std::map<shape, QString> m_enumToText;
			shape m_CollisionShape;
			float m_radius;
			float m_height;
			std::string m_collisionModelShapeName;
			CollisionShape(shape p_collisionShape = shape::CONE, float p_radius = 0.0f, float p_height = 0.0f, std::string p_collisionModelShapeName = "") : MainComponent(ComponentType::COLLISIONSHAPE)
			{
				m_CollisionShape = p_collisionShape;
				m_radius = p_radius;
				m_height = p_height;
				m_collisionModelShapeName = p_collisionModelShapeName;
				m_enumToText[CONE] = "Cone";
				m_enumToText[CYLINDER] = "Cylinder";
				m_enumToText[SPHERE] = "Sphere";
				m_enumToText[MESH] = "Mesh";
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* collisionShape, *radius, *height, *collisionModelname;				
				collisionShape = p_propMan->addProperty(QVariant::String, "CollisionShape" );
				p_propMan->setValue(collisionShape, m_enumToText[m_CollisionShape] );
				collisionShape->setToolTip("Collision shape can be Cone, Cylinder, Sphere or Mesh");
				radius = p_propMan->addProperty(QVariant::Double, "Radius");
				p_propMan->setValue(radius, m_radius);
				height = p_propMan->addProperty(QVariant::Double, "Height");
				p_propMan->setValue(height, m_height);
				collisionModelname = p_propMan->addProperty(QVariant::String, "CollisionModel name");
				p_propMan->setValue(collisionModelname, m_collisionModelShapeName.c_str());
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(collisionShape);
				p_propBrows->addProperty(radius);
				p_propBrows->addProperty(height);
				p_propBrows->addProperty(collisionModelname);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				bool foundShape = false;
				for(int i = 0; i < shape::END_OF_ENUM; i++)
				{
					if(props.at(0)->valueText().compare(m_enumToText[(shape)i]) == 0)
					{
						m_CollisionShape = (shape)i;
						foundShape = true;
					}
				}
				if(!foundShape)
				{
					QMessageBox msgbox;
					msgbox.setText("Invalid collision shape : " + props.at(0)->valueText() + ". Valid collision shapes are : 'Cone', 'Cylinder', 'Sphere' and 'Mesh' ");
					msgbox.exec();
				}
				m_radius = atof(props.at(1)->valueText().toStdString().c_str());
				m_height = atof(props.at(2)->valueText().toStdString().c_str());;
				m_collisionModelShapeName = props.at(3)->valueText().toStdString();
			}

		};

		struct AbilityParticle : MainComponent
		{
			std::string m_particleName;
			float m_size;
			AbilityParticle(float p_size = 1.0f) : MainComponent(ComponentType::ABILITYPARTICLE)
			{
				m_particleName = "";
				m_size = p_size;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* particleName, *size;

				particleName = p_propMan->addProperty(QVariant::String, "ParticleName" );
				p_propMan->setValue(particleName, m_particleName.c_str());
				size = p_propMan->addProperty(QVariant::Double, "Size");
				p_propMan->setValue(size, m_size);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(particleName);
				p_propBrows->addProperty(size);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				m_particleName = props.at(0)->valueText().toStdString();
				m_size = atof(props.at(1)->valueText().toStdString().c_str());
			}
		};

		struct PhysicsControlled : MainComponent
		{
			float m_speed;
			float m_mass;
			PhysicsControlled(float p_speed = 0.0f, float p_mass = 1.0f) : MainComponent(ComponentType::PHYSICSCONTROLLED)
			{
				m_speed = p_speed;
				m_mass = p_mass;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* speed, *mass;

				speed = p_propMan->addProperty(QVariant::Double, "Speed" );
				p_propMan->setValue(speed, m_speed);
				mass = p_propMan->addProperty(QVariant::Double, "Mass");
				p_propMan->setValue(mass, m_mass);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(mass);
				p_propBrows->addProperty(speed);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				m_mass = atof(props.at(0)->valueText().toStdString().c_str());
				m_speed = atof(props.at(1)->valueText().toStdString().c_str());
			}
		};

		//////////////////////////////////////////////On collide components////////////////////////////////////////////////////////
		struct OffensiveAbility: MainComponent
		{
			float m_damage;
			float m_knockbackPower;
			OffensiveAbility(float p_damage = 0.0f, float p_knockbackPower = 0.0f) : MainComponent(ComponentType::OFFENSIVEABILITY)
			{
				m_damage = p_damage;
				m_knockbackPower = p_knockbackPower;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* damage, *knockbackpower;

				damage = p_propMan->addProperty(QVariant::Double, "Damage" );
				p_propMan->setValue(damage, m_damage);
				knockbackpower = p_propMan->addProperty(QVariant::Double, "KnockbackPower" );
				p_propMan->setValue(knockbackpower, m_knockbackPower);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(damage);
				p_propBrows->addProperty(knockbackpower);

			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				m_damage = atof(props.at(0)->valueText().toStdString().c_str());
				m_knockbackPower =  atof(props.at(1)->valueText().toStdString().c_str());
			}
		};

		struct Explosive : MainComponent
		{
			float m_radius;
			Explosive(float p_radius = 0.0f) : MainComponent(ComponentType::EXPLOSIVE)
			{
				m_radius = p_radius;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* radius;

				radius = p_propMan->addProperty(QVariant::String, "Radius" );
				p_propMan->setValue(radius, m_radius);
			
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(radius);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				m_radius = atof(props.at(0)->valueText().toStdString().c_str());
			}
		};	
	}
	namespace AbilityEntity
	{
		//The one struct to rule them all
		struct Entity
		{
			QString m_name;
			std::vector<AbilityComponents::MainComponent*>* m_components;
			std::map<QString, AbilityComponents::ComponentType::ComponentType> m_nameToEnumMapper;
			Entity(QString p_name)
			{
				for(int i = 0 ; i < AbilityComponents::ComponentType::END_OF_ENUM; i++)
				{
					m_nameToEnumMapper[AbilityComponents::g_componentNameList.m_compNames.at(i)] = (AbilityComponents::ComponentType::ComponentType)i;
				}
				m_name = p_name;
				m_components = new std::vector<AbilityComponents::MainComponent*>();
			}
			~Entity()
			{
				if(m_components->size() > 0)
				{
					for(unsigned int i = m_components->size() -1 ; i > 0; i --)
					{
						delete m_components->at(i);
						m_components->pop_back();
					}
				}
				delete m_components;
			}
			void AddComponent(QString p_componentName)
			{
				AddComponent((AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper.at(p_componentName));
			}
			void AddComponent(AbilityComponents::ComponentType::ComponentType p_type)
			{
				switch (p_type)
				{
					
				case AbilityComponents::ComponentType::TRANSFORM:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::Transform();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::COLLISION:			
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::Collision();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::ABILITYMODEL:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::AbilityModel();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::COLLISIONSHAPE:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::CollisionShape();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::PHYSICSCONTROLLED:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::PhysicsControlled();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::ABILITYPARTICLE:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::AbilityParticle();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::OFFENSIVEABILITY:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::OffensiveAbility();
						AddComponent(temp);
					}
					break;
				case AbilityComponents::ComponentType::EXPLOSIVE:
					{
						AbilityComponents::MainComponent* temp = new AbilityComponents::Explosive();
						AddComponent(temp);
					}
					break;
				default:
					break; 
				}
			}
			void AddComponent(AbilityComponents::MainComponent* p_component)
			{
				m_components->push_back(p_component);
			}
			void RemoveComponent(QString p_componentName)
			{
				RemoveComponent((AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper.at(p_componentName));
			}
			void RemoveComponent(AbilityComponents::ComponentType::ComponentType p_type)
			{
				for(unsigned int i = 0; i < m_components->size(); i++)
					if(m_components->at(i)->m_type == p_type)
					{
						delete m_components->at(i);
						m_components->erase(m_components->begin() + i);
						break;
					}
			}
			bool DoesComponentExist(QString p_componentName)
			{
				return DoesComponentExist((AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper.at(p_componentName));
			}
			bool DoesComponentExist(AbilityComponents::ComponentType::ComponentType p_type)
			{
				for(unsigned int i = 0; i < m_components->size(); i++)
				{
					if(m_components->at(i)->m_type == p_type)
						return true;
				}
				return false;
			}
			void ViewComponentData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory ,  QString p_name)
			{
				AbilityComponents::ComponentType::ComponentType type = (AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper[p_name];
				for(unsigned int i = 0 ; i < m_components->size(); i++)
				{
					if(m_components->at(i)->m_type == type)
					{
						m_components->at(i)->ViewData(p_propMan, p_propBrows, p_factory);
					}
				}
			}
			void SaveComponentData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory ,  QString p_name)
			{
				AbilityComponents::ComponentType::ComponentType type = (AbilityComponents::ComponentType::ComponentType)m_nameToEnumMapper[p_name];
				for(unsigned int i = 0 ; i < m_components->size(); i++)
				{
					if(m_components->at(i)->m_type == type)
					{
						m_components->at(i)->SaveData(p_propMan,p_propBrows, p_factory);
					}
				}
			}
			QString GetComponentNameFromId(unsigned int p_id)
			{
				AbilityComponents::ComponentType::ComponentType findType = m_components->at(p_id)->m_type;
				std::map<QString, AbilityComponents::ComponentType::ComponentType>::const_iterator it;
				for(it = m_nameToEnumMapper.begin(); it != m_nameToEnumMapper.end(); ++it)
				{
					if(it->second == findType)
					{
						return it->first;
					}
				}
				return "";
			}
			void RemoveAllComponents()
			{
				for(unsigned int i = 0; i < m_components->size(); i++)
				{	
					delete m_components->at(i);
					m_components->erase(m_components->begin() + i);
				}
			}
		};
	}
}