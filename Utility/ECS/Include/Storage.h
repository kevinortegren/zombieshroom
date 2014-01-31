#pragma once

#include <map>
#include <glm/glm.hpp>

namespace ECS
{
	class Storage
	{
	public:
		friend class EntityExporter;
		void SetStringValue(const std::string& p_key, const std::string& p_value);
		void SetValue(const std::string& p_key, const glm::vec4& p_value);
		void SetValue(const std::string& p_key, const glm::vec3& p_value);
		void SetValue(const std::string& p_key, const glm::vec2& p_value);
		void SetValue(const std::string& p_key, const float p_value);
		void SetValue(const std::string& p_key, const int p_value);
		void SetValue(const std::string& p_key, const bool p_value);

		bool GetValueAsBool(const std::string& p_key);
		int GetValueAsInt(const std::string& p_key);
		float GetValueAsFloat(const std::string& p_key);
		std::string GetValueAsString(const std::string& p_key);
		glm::vec4 GetValueAsVec4(const std::string& p_key);
		glm::vec3 GetValueAsVec3(const std::string& p_key);
		glm::vec2 GetValueAsVec2(const std::string& p_key);

	private:
		std::map<std::string, std::string> m_values;
	};
}