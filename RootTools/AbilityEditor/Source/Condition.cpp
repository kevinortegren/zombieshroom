#include "Condition.h"

namespace AbilityEditorNameSpace
{

	Condition::Condition(QString p_text)
	{
		m_text = p_text;
	}

	Condition::~Condition()
	{

	}

	void Condition::ViewSelectedData( QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows)
	{

	}

	void Condition::SaveSelectedData( QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows)
	{

	}

	void Condition::AddEntity( QString p_name )
	{
		m_entities.push_back(p_name);
	}

	void Condition::RemoveEntity( QString p_name )
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			if(m_entities.at(i).compare(p_name) == 0)
			{
				m_entities.erase(m_entities.begin() + i);
				return;
			}
		}
	}

	
}

