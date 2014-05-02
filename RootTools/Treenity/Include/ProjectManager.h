#pragma once

#include <map>
#include <QString>
#include <Utility/ECS/Include/Entity.h>
#include <Utility/ECS/Include/World.h>

class ProjectManager
{
public:
	ProjectManager(ECS::World* p_world);
	~ProjectManager();

	const QString& GetCurrentProjectName() const;
	const QString& GetEntityName(ECS::Entity* p_entity) const;

	void SetEntityName(ECS::Entity* p_entity, const QString& p_name);

	void Export(const QString& p_name);
	void Import(const QString& p_name);
private:
	ECS::World* m_world;

	QString m_projectName;
	std::map<ECS::Entity*, std::string> m_entityNames;
};