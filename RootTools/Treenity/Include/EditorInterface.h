#pragma once

#include <set>
#include <Utility/ECS/Include/Entity.h>

class EditorInterface
{
public:
	virtual void Select(ECS::Entity* p_entity) = 0;
	virtual void AddToSelection(ECS::Entity* p_entity) = 0;
	virtual void ClearSelection() = 0;
	virtual const std::set<ECS::Entity*>& GetSelection() const = 0;
	virtual void RenameEntity(ECS::Entity* p_entity, const std::string& p_name) = 0;
};