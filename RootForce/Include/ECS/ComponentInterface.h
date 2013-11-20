#pragma once

namespace ECS
{
	static int s_count;

	class ComponentInterface {};

	template<class T>
	class Component : public ComponentInterface
	{
	public:
		static int GetTypeId();
		static int s_typeId;
	};

	template<class T>
	int Component<T>::s_typeId = s_count++;

	template<class T>
	int Component<T>::GetTypeId()
	{
		return s_typeId;
	}
}