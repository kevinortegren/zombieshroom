#pragma once

#include "AbilityComponents.h"

namespace AbilityEditorNameSpace
{
	class Condition
	{
	public:
		Condition(QString p_name, QString p_codeLine);
		~Condition();

		void ViewSelectedData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows);
		void SaveSelectedData(QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows);
		void AddEntity(QString p_name);
		void RemoveEntity(QString p_name);
		std::vector<QString> GetEntities() { return m_entities; }
		QString GetName() { return m_name; }
		QString GetCode() { return m_codeLine; }

	private:
		std::vector<QString> m_entities;
		QString m_name;
		QString m_codeLine;
	};
}