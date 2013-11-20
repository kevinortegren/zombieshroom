#pragma once

namespace ECS
{
	static unsigned int s_count;

	class ComponentInterface {};

	template<class T>
	class Component : public ComponentInterface
	{
	public:
		static unsigned int GetTypeId();
		static unsigned int s_typeId;
	};

	template<class T>
	unsigned int Component<T>::s_typeId = s_count++;

	template<class T>
	unsigned int Component<T>::GetTypeId()
	{
		return s_typeId;
	}
}