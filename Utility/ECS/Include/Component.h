#pragma once

#include <climits>

namespace ECS
{
	static unsigned int s_numComponents;

	class ComponentInterface {};

	template<class T>
	struct Component : public ComponentInterface
	{
		static unsigned int GetTypeId();
		static void SetTypeId(unsigned int p_id);
		static unsigned int s_typeId;
	};

	template<class T>
	unsigned int Component<T>::s_typeId = 0xffffffff;

	template<class T>
	unsigned int Component<T>::GetTypeId()
	{
		return s_typeId;
	}

	template<class T>
	void Component<T>::SetTypeId(unsigned int p_id)
	{
		Component<T>::s_typeId = p_id;

		if(p_id > s_numComponents)
			s_numComponents = p_id;
	}
}