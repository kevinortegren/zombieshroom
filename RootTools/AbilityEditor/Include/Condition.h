#pragma once

#include "AbilityComponents.h"

namespace AbilityEditorNameSpace
{
	class Condition
	{
	public:
		Condition(QString p_text);
		~Condition();

		void ViewSelectedData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows);
		void SaveSelectedData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows);
		void AddEntity(QString p_name);
		void RemoveEntity(QString p_name);
		std::vector<QString> GetEntities() { return m_entities; }
		QString GetText() { return m_text; }

	private:
		std::vector<QString> m_entities;
		QString m_text;
	};
}