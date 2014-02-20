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
				STARTPOS, //done
				TARGPOS, //Needs system
				VELOCITY, //done
				ABILITYMODEL, //done
				COLLISIONSHAPE, //done
				ABILITYPARTICLE, //done
				DAMAGE, //done
				KNOCKBACK, //done
				STATCHANGECASTER, //Still needs script implementation
				STATCHANGETARGET, //Still needs script implementation
				PHYSICS, //add collide w/ world
				CHARGEVARIABLES, //done
				END_OF_ENUM //TODO : EntityChange
			};
		}
		const struct ComponentNameList
		{
			QStringList m_compNames;
			ComponentNameList()
			{
				m_compNames.append("Start Position");
				m_compNames.append("Target Position");
				m_compNames.append("Velocity");
				m_compNames.append("Ability Model");
				m_compNames.append("Collision Shape");
				m_compNames.append("Ability Particle");
				m_compNames.append("Damage");
				m_compNames.append("Knockback");
				m_compNames.append("Stat Change (Caster)");
				m_compNames.append("Stat Change (Target)");
				m_compNames.append("Physics");
				m_compNames.append("Charge Variables"); 
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
	

		struct StartPos: MainComponent
		{ 
			enum pos
			{
				ONCASTER,
				UNDERCASTER,
				ENEMYPLAYER,
				FRIENDPLAYER,
				ABSOLUTE,
				END_OF_ENUM
			};

			std::map<pos, QString> m_enumToText;
			pos m_startPos;
			QVector3D m_absolutePos;
			StartPos(pos p_pos = pos::ONCASTER, QVector3D p_startPos = QVector3D(0,0,0)): MainComponent(ComponentType::STARTPOS)
			{
				m_enumToText[ONCASTER] = "On Caster";
				m_enumToText[UNDERCASTER] = "Under Caster";
				m_enumToText[ENEMYPLAYER] = "Enemy Player";
				m_enumToText[FRIENDPLAYER] = "Friendly Player";
				m_enumToText[ABSOLUTE] = "Absolute";
				m_absolutePos = p_startPos;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory) 
			{
				QtVariantProperty* position, *x, *y, *z; 
				QList<QtProperty*> list;
				QString combinedValue;

				QtProperty* enumPos;
				QStringList enumTypes;
				QtEnumPropertyManager* enumMan = new QtEnumPropertyManager;
				QtEnumEditorFactory* enumFac = new QtEnumEditorFactory;
				for(unsigned int i = 0; i < m_enumToText.size(); i++)
				{
					enumTypes << m_enumToText[(pos)i];
				}
				enumPos = enumMan->addProperty("Start position");
				enumMan->setEnumNames(enumPos,enumTypes);
				enumMan->setValue(enumPos, m_startPos);

				position = p_propMan->addProperty(QVariant::String, "Absolute (if selected)");
				x = p_propMan->addProperty(QVariant::Double, "x");
				y = p_propMan->addProperty(QVariant::Double, "y");
				z = p_propMan->addProperty(QVariant::Double, "z");
				p_propMan->setValue(x, m_absolutePos.x());
				p_propMan->setValue(y, m_absolutePos.y());
				p_propMan->setValue(z, m_absolutePos.z());
				position->addSubProperty(x);
				position->addSubProperty(y);
				position->addSubProperty(z);
				list = position->subProperties();
				combinedValue = "(" + list.at(0)->valueText() + ", " + list.at(1)->valueText() + ", " + list.at(2)->valueText() + ")";
				p_propMan->setValue(position,combinedValue);

				//Add to browser widget
				p_propBrows->setFactoryForManager(enumMan, enumFac);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(enumPos);
				p_propBrows->addProperty(position);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				std::map<pos, QString>::const_iterator it;
				for(it = m_enumToText.begin(); it != m_enumToText.end(); ++it)
				{
					if(it->second == props.at(0)->valueText())
					{
						m_startPos = it->first;
					}
				}
				subprops = props.at(1)->subProperties();
				for(int i = 0; i < subprops.size(); i++)
				{
					m_absolutePos[i] = p_propMan->variantProperty(subprops.at(i))->value().toFloat();
				}
			}
		};

		struct TargetPos: MainComponent
		{ 
			enum pos
			{
				ENEMYPLAYER,
				FRIENDLYPLAYER,
				ONAIM,
				ABSOLUTE,
				END_OF_ENUM
			};

			std::map<pos, QString> m_enumToText;
			pos m_targPos;
			QVector3D m_absolutePos;
			TargetPos(pos p_pos = pos::ENEMYPLAYER, QVector3D p_targPos = QVector3D(0,0,0)): MainComponent(ComponentType::TARGPOS)
			{
				m_enumToText[ENEMYPLAYER] = "Enemy Player";
				m_enumToText[FRIENDLYPLAYER] = "Friendly Player";
				m_enumToText[ONAIM] = "On Aim";
				m_enumToText[ABSOLUTE] = "Absolute";
				m_absolutePos = p_targPos;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory) 
			{
				QtVariantProperty* position, *x, *y, *z; 
				QList<QtProperty*> list;
				QString combinedValue;

				QtProperty* enumPos;
				QStringList enumTypes;
				QtEnumPropertyManager* enumMan = new QtEnumPropertyManager;
				QtEnumEditorFactory* enumFac = new QtEnumEditorFactory;
				for(unsigned int i = 0; i < m_enumToText.size(); i++)
				{
					enumTypes << m_enumToText[(pos)i];
				}
				enumPos = enumMan->addProperty("Target position");
				enumMan->setEnumNames(enumPos,enumTypes);
				enumMan->setValue(enumPos, m_targPos);

				position = p_propMan->addProperty(QVariant::String, "Absolute (if selected)");
				x = p_propMan->addProperty(QVariant::Double, "x");
				y = p_propMan->addProperty(QVariant::Double, "y");
				z = p_propMan->addProperty(QVariant::Double, "z");
				p_propMan->setValue(x, m_absolutePos.x());
				p_propMan->setValue(y, m_absolutePos.y());
				p_propMan->setValue(z, m_absolutePos.z());
				position->addSubProperty(x);
				position->addSubProperty(y);
				position->addSubProperty(z);
				list = position->subProperties();
				combinedValue = "(" + list.at(0)->valueText() + ", " + list.at(1)->valueText() + ", " + list.at(2)->valueText() + ")";
				p_propMan->setValue(position,combinedValue);

				//Add to browser widget
				p_propBrows->setFactoryForManager(enumMan, enumFac);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(enumPos);
				p_propBrows->addProperty(position);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				std::map<pos, QString>::const_iterator it;
				for(it = m_enumToText.begin(); it != m_enumToText.end(); ++it)
				{
					if(it->second == props.at(0)->valueText())
					{
						m_targPos = it->first;
					}
				}
				subprops = props.at(1)->subProperties();
				for(int i = 0; i < subprops.size(); i++)
				{
					m_absolutePos[i] = p_propMan->variantProperty(subprops.at(i))->value().toFloat();
				}
			}
		};

		struct Velocity: MainComponent
		{ 
			enum dir
			{
				FORWARD,
				BACKWARD,
				UP,
				DOWN,
				LEFT,
				RIGHT,
				END_OF_ENUM
			};

			std::map<dir, QString> m_enumToText;
			dir m_direction;
			float m_speed;
			Velocity(dir p_dir = dir::FORWARD, float p_speed = 1.0f): MainComponent(ComponentType::VELOCITY)
			{
				m_enumToText[FORWARD] = "Forward";
				m_enumToText[BACKWARD] = "Backward";
				m_enumToText[UP] = "Up";
				m_enumToText[DOWN] = "Down";
				m_enumToText[LEFT] = "Left";
				m_enumToText[RIGHT] = "Right";
				m_speed = p_speed;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory) 
			{
				QtVariantProperty* speed;

				QtProperty* enumDir;
				QStringList enumTypes;
				QtEnumPropertyManager* enumMan = new QtEnumPropertyManager;
				QtEnumEditorFactory* enumFac = new QtEnumEditorFactory;
				for(unsigned int i = 0; i < m_enumToText.size(); i++)
				{
					enumTypes << m_enumToText[(dir)i];
				}
				enumDir = enumMan->addProperty("Direction");
				enumMan->setEnumNames(enumDir,enumTypes);
				enumMan->setValue(enumDir, m_direction);

				speed = p_propMan->addProperty(QVariant::Double, "Speed");
				p_propMan->setValue(speed, m_speed);

				//Add to browser widget
				p_propBrows->setFactoryForManager(enumMan, enumFac);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(enumDir);
				p_propBrows->addProperty(speed);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				std::map<dir, QString>::const_iterator it;
				for(it = m_enumToText.begin(); it != m_enumToText.end(); ++it)
				{
					if(it->second == props.at(0)->valueText())
					{
						m_direction = it->first;
					}
				}
				m_speed = p_propMan->variantProperty(props.at(1))->value().toFloat();
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

		struct Damage : MainComponent
		{
			enum affected
			{
				ENEMIES,
				FRIENDLIES,
				EVERYONE,
				END_OF_ENUM
			};

			std::map<affected, QString> m_enumToText;
			affected m_affectedPlayers;
			float m_damage;
			Damage(affected p_collisionShape = affected::ENEMIES, float p_damage = 1.0f) : MainComponent(ComponentType::DAMAGE)
			{
				m_affectedPlayers = p_collisionShape;
				m_damage = p_damage;
				m_enumToText[ENEMIES] = "Enemies";
				m_enumToText[FRIENDLIES] = "Friendlies";
				m_enumToText[EVERYONE] = "Everyone";
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* damage;
				QtProperty* affectedPlayers;
				QStringList enumTypes;
				QtEnumPropertyManager* enumMan = new QtEnumPropertyManager;
				QtEnumEditorFactory* enumFac = new QtEnumEditorFactory;
				for(unsigned int i = 0; i < m_enumToText.size(); i++)
				{
					enumTypes << m_enumToText[(affected)i];
				}
				affectedPlayers = enumMan->addProperty("Affected Players");
				enumMan->setEnumNames(affectedPlayers,enumTypes);
				enumMan->setValue(affectedPlayers, m_affectedPlayers);

				damage = p_propMan->addProperty(QVariant::Double, "Damage");
				p_propMan->setValue(damage, m_damage);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->setFactoryForManager(enumMan, enumFac);
				p_propBrows->addProperty(affectedPlayers);
				p_propBrows->addProperty(damage);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				bool foundShape = false;
				std::map<affected, QString>::const_iterator it;
				for(it = m_enumToText.begin(); it != m_enumToText.end(); ++it)
				{
					if(it->second == props.at(0)->valueText())
					{
						m_affectedPlayers = it->first;
					}
				}
				m_damage = p_propMan->variantProperty(props.at(1))->value().toFloat();
			}
		};

		struct Knockback : MainComponent
		{
			enum affected
			{
				ENEMIES,
				FRIENDLIES,
				EVERYONE,
				END_OF_ENUM
			};

			std::map<affected, QString> m_enumToText;
			affected m_affectedPlayers;
			float m_knockback;
			Knockback(affected p_collisionShape = affected::ENEMIES, float p_knockback = 1.0f) : MainComponent(ComponentType::KNOCKBACK)
			{
				m_affectedPlayers = p_collisionShape;
				m_knockback = p_knockback;
				m_enumToText[ENEMIES] = "Enemies";
				m_enumToText[FRIENDLIES] = "Friendlies";
				m_enumToText[EVERYONE] = "Everyone";
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* knockback;
				QtProperty* affectedPlayers;
				QStringList enumTypes;
				QtEnumPropertyManager* enumMan = new QtEnumPropertyManager;
				QtEnumEditorFactory* enumFac = new QtEnumEditorFactory;
				for(unsigned int i = 0; i < m_enumToText.size(); i++)
				{
					enumTypes << m_enumToText[(affected)i];
				}
				affectedPlayers = enumMan->addProperty("Affected Players");
				enumMan->setEnumNames(affectedPlayers,enumTypes);
				enumMan->setValue(affectedPlayers, m_affectedPlayers);

				knockback = p_propMan->addProperty(QVariant::Double, "Knockback");
				p_propMan->setValue(knockback, m_knockback);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->setFactoryForManager(enumMan, enumFac);
				p_propBrows->addProperty(affectedPlayers);
				p_propBrows->addProperty(knockback);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				bool foundShape = false;
				std::map<affected, QString>::const_iterator it;
				for(it = m_enumToText.begin(); it != m_enumToText.end(); ++it)
				{
					if(it->second == props.at(0)->valueText())
					{
						m_affectedPlayers = it->first;
					}
				}
				m_knockback = p_propMan->variantProperty(props.at(1))->value().toFloat();
			}
		};

		struct StatChangeCaster : MainComponent
		{
			float m_speed, m_jumpHeight, m_knockbackResistance;
			StatChangeCaster(float p_speed = 1.0f, float p_jumpHeight = 1.0f, float p_knockbackResistance = 1.0f) : MainComponent(ComponentType::STATCHANGECASTER)
			{
				m_speed = p_speed;
				m_jumpHeight = p_jumpHeight;
				m_knockbackResistance = p_knockbackResistance;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* speed, *jumpHeight, *knockbackResistance;

				speed = p_propMan->addProperty(QVariant::Double, "Speed" );
				p_propMan->setValue(speed, m_speed);

				jumpHeight = p_propMan->addProperty(QVariant::Double, "JumpHeight" );
				p_propMan->setValue(jumpHeight, m_jumpHeight);

				knockbackResistance = p_propMan->addProperty(QVariant::Double, "Knockback Resistance" );
				p_propMan->setValue(knockbackResistance, m_knockbackResistance);

				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(speed);
				p_propBrows->addProperty(jumpHeight);
				p_propBrows->addProperty(knockbackResistance);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				m_speed = p_propMan->variantProperty(props.at(0))->value().toFloat();
				m_jumpHeight = p_propMan->variantProperty(props.at(1))->value().toFloat();
				m_knockbackResistance = p_propMan->variantProperty(props.at(2))->value().toFloat();
			}
		};

		struct StatChangeTarget : MainComponent
		{
			float m_speed, m_jumpHeight, m_knockbackResistance;
			StatChangeTarget(float p_speed = 1.0f, float p_jumpHeight = 1.0f, float p_knockbackResistance = 1.0f) : MainComponent(ComponentType::STATCHANGETARGET)
			{
				m_speed = p_speed;
				m_jumpHeight = p_jumpHeight;
				m_knockbackResistance = p_knockbackResistance;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* speed, *jumpHeight, *knockbackResistance;

				speed = p_propMan->addProperty(QVariant::Double, "Speed" );
				p_propMan->setValue(speed, m_speed);

				jumpHeight = p_propMan->addProperty(QVariant::Double, "JumpHeight" );
				p_propMan->setValue(jumpHeight, m_jumpHeight);

				knockbackResistance = p_propMan->addProperty(QVariant::Double, "Knockback Resistance" );
				p_propMan->setValue(knockbackResistance, m_knockbackResistance);

				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(speed);
				p_propBrows->addProperty(jumpHeight);
				p_propBrows->addProperty(knockbackResistance);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				m_speed = p_propMan->variantProperty(props.at(0))->value().toFloat();
				m_jumpHeight = p_propMan->variantProperty(props.at(1))->value().toFloat();
				m_knockbackResistance = p_propMan->variantProperty(props.at(2))->value().toFloat();
			}
		};

		struct Physics : MainComponent
		{
			float m_mass;
			QVector3D m_gravity;
			bool m_collide;
			Physics(float p_mass = 1.0f, QVector3D p_gravity = QVector3D(0.0f, -9.82f, 0.0f), bool p_collide = true) : MainComponent(ComponentType::PHYSICS)
			{
				m_mass = p_mass;
				m_gravity =	p_gravity;
				m_collide = p_collide;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty *mass, *gravity, *x, *y, *z, *collide;
				QList<QtProperty*> list;
				QString combinedValue;
				mass = p_propMan->addProperty(QVariant::Double, "Mass");
				p_propMan->setValue(mass, m_mass);
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(mass);

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

				collide = p_propMan->addProperty(QVariant::Bool, "Collide with world");
				p_propMan->setValue(collide, m_collide);
				p_propBrows->addProperty(collide);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;

				props = p_propBrows->properties();
				m_mass = p_propMan->variantProperty(props.at(0))->value().toFloat();
				
				subprops = props.at(1)->subProperties();
				for(int i = 0; i < subprops.size(); i++)
				{
					m_gravity[i] = p_propMan->variantProperty(subprops.at(i))->value().toFloat();
				}

				m_collide = p_propMan->variantProperty(props.at(2))->value().toBool();
			}
		};

		struct ChargeVariables : MainComponent
		{
			float m_chargeReq, m_chargeFactor;
			ChargeVariables(float p_chargeReq = 1.0f, float p_chargeFactor = 1.0f) : MainComponent(ComponentType::CHARGEVARIABLES)
			{
				m_chargeReq = p_chargeReq;
				m_chargeFactor = p_chargeFactor;
			}
			void ViewData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QtVariantProperty* chargeReq, *chargeFactor;

				chargeReq = p_propMan->addProperty(QVariant::Double, "Charge Required" );
				p_propMan->setValue(chargeReq, m_chargeReq);
				chargeReq->setToolTip("Factor of charge time required for the ability to activate [0.0 : 1.0]");
				
				chargeFactor = p_propMan->addProperty(QVariant::Double, "Charge Effect Factor" );
				p_propMan->setValue(chargeFactor, m_chargeFactor);
				chargeFactor->setToolTip("Increase in effect over time [1.0 : 2.0]");
			
				p_propBrows->setFactoryForManager(p_propMan,p_factory);
				p_propBrows->addProperty(chargeReq);
				p_propBrows->addProperty(chargeFactor);
			}
			void SaveData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory)
			{
				QList<QtProperty*> props, subprops;
				props = p_propBrows->properties();
				m_chargeReq = p_propMan->variantProperty(props.at(0))->value().toFloat();
				m_chargeFactor = p_propMan->variantProperty(props.at(1))->value().toFloat();
			}
		};	
	}
	
}