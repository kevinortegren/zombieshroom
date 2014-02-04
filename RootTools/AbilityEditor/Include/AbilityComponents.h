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
#include <QtPropertyBrowser/QtEnumPropertyManager>
#include <QtPropertyBrowser/QtEnumEditorFactory>
#include <Qt/QtGui/QVector3D>
#include <Qt/QtWidgets/QMessageBox>
#include <Qt/QtWidgets/QSpinBox>

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
					m_rotation[i] = p_propMan->variantProperty(subprops.at(i))->value().toFloat();
				}
				subprops = props.at(1)->subProperties();
				for(int i = 0; i < subprops.size(); i++)
				{
					m_scale[i] = p_propMan->variantProperty(subprops.at(i))->value().toFloat();
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
				RAY,
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
				m_enumToText[RAY] = "Ray";
				m_enumToText[MESH] = "Mesh";
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* radius, *height, *collisionModelname;	
				QtProperty* collisionShape;
				QStringList enumTypes;
				QtEnumPropertyManager* enumMan = new QtEnumPropertyManager;
				QtEnumEditorFactory* enumFac = new QtEnumEditorFactory;
				for(unsigned int i = 0; i < m_enumToText.size(); i++)
				{
					enumTypes << m_enumToText[(shape)i];
				}
				collisionShape = enumMan->addProperty("Collision shape");
				enumMan->setEnumNames(collisionShape,enumTypes);
				enumMan->setValue(collisionShape, m_CollisionShape);
				//collisionShape = p_propMan->addProperty(QVariant::String, "CollisionShape" );
				//p_propMan->setValue(collisionShape, m_enumToText[m_CollisionShape] );
				collisionShape->setToolTip("Collision shape can be Cone, Cylinder, Sphere, Ray or Mesh");
				radius = p_propMan->addProperty(QVariant::Double, "Radius");
				p_propMan->setValue(radius, m_radius);
				height = p_propMan->addProperty(QVariant::Double, "Height/Length");
				p_propMan->setValue(height, m_height);
				collisionModelname = p_propMan->addProperty(QVariant::String, "CollisionModel name");
				p_propMan->setValue(collisionModelname, m_collisionModelShapeName.c_str());
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->setFactoryForManager(enumMan, enumFac);
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
				std::map<shape, QString>::const_iterator it;
				for(it = m_enumToText.begin(); it != m_enumToText.end(); ++it)
				{
					if(it->second == props.at(0)->valueText())
					{
						 m_CollisionShape = it->first;
					}
				}
				m_radius = p_propMan->variantProperty(props.at(1))->value().toFloat();
				m_height = p_propMan->variantProperty(props.at(2))->value().toFloat();
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
				m_size = p_propMan->variantProperty(props.at(1))->value().toFloat();
			}
		};

		struct PhysicsControlled : MainComponent
		{
			float m_speed;
			float m_mass;
			QVector3D m_gravity;
			PhysicsControlled(float p_speed = 0.0f, float p_mass = 1.0f, QVector3D p_gravity = QVector3D(0.0f, -9.82f, 0.0f)) : MainComponent(ComponentType::PHYSICSCONTROLLED)
			{
				m_speed = p_speed;
				m_mass = p_mass;
				m_gravity =	p_gravity;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* speed, *mass, *gravity, *x, *y, *z;
				QList<QtProperty*> list;
				QString combinedValue;
				speed = p_propMan->addProperty(QVariant::Double, "Speed" );
				p_propMan->setValue(speed, m_speed);
				mass = p_propMan->addProperty(QVariant::Double, "Mass");
				p_propMan->setValue(mass, m_mass);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(mass);
				p_propBrows->addProperty(speed);

				//Show gravity vector
				gravity = p_propMan->addProperty(QVariant::String, "Gravity");
				x = p_propMan->addProperty(QVariant::Double, "x");
				y = p_propMan->addProperty(QVariant::Double, "y");
				z = p_propMan->addProperty(QVariant::Double, "z");
				p_propMan->setValue(x, m_gravity.x());
				p_propMan->setValue(y, m_gravity.y());
				p_propMan->setValue(z, m_gravity.z());
				gravity->addSubProperty(x);
				gravity->addSubProperty(y);
				gravity->addSubProperty(z);
				list = gravity->subProperties();
				combinedValue = "(" + list.at(0)->valueText() + ", " + list.at(1)->valueText() + ", " + list.at(2)->valueText() + ")";
				p_propMan->setValue(gravity,combinedValue);
				p_propBrows->addProperty(gravity);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				//QList<QVariant*> props, subprops;

				props = p_propBrows->properties();
				//m_mass = atof(props.at(0)->valueText().toStdString().c_str());
				//m_speed = atof(props.at(1)->valueText().toStdString().c_str());
				m_mass = p_propMan->variantProperty(props.at(0))->value().toFloat();
				m_speed = p_propMan->variantProperty(props.at(1))->value().toFloat();
				
				//0 skräp 1 2 3 x y z, 4 SKRÄP. 5 6 7    <---- SKRÄPKommentar
				subprops = props.at(2)->subProperties();
				for(int i = 0; i < subprops.size(); i++)
				{
					m_gravity[i] = p_propMan->variantProperty(subprops.at(i))->value().toFloat();
				}
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
				m_damage = p_propMan->variantProperty(props.at(0))->value().toFloat();
				m_knockbackPower = p_propMan->variantProperty(props.at(1))->value().toFloat();
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
				m_radius = p_propMan->variantProperty(props.at(0))->value().toFloat();
			}
		};	
	}
	
}