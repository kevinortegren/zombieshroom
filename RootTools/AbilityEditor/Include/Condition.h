#pragma once

#include "AbilityComponents.h"

namespace AbilityEditorNameSpace
{
	class Condition
	{
	public:
		Condition();
		~Condition();

		void ViewSelectedData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory);
		void SaveSelectedData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_factory);

	private:
		std::vector<AbilityEditorNameSpace::AbilityEntity::Entity*> m_entities;
	};
}