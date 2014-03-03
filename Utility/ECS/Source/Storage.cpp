#include <Utility/ECS/Include/Storage.h>
#include <sstream>

namespace ECS
{
	void Storage::SetStringValue(const std::string& p_key, const std::string& p_value)
	{
		m_values[p_key] = p_value;
	}

	void Storage::SetValue(const std::string& p_key, const glm::vec4& p_value)
	{
		std::stringstream ss;
		ss << p_value.x << " " << p_value.y << " " << p_value.z << " " << p_value.w;
		SetStringValue(p_key, ss.str());
	}

	void Storage::SetValue(const std::string& p_key, const glm::vec3& p_value)
	{
		std::stringstream ss;
		ss << p_value.x << " " << p_value.y << " " << p_value.z;
		SetStringValue(p_key, ss.str());
	}

	void Storage::SetValue(const std::string& p_key, const glm::vec2& p_value)
	{
		std::stringstream ss;
		ss << p_value.x << " " << p_value.y;
		SetStringValue(p_key, ss.str());
	}

	void Storage::SetValue(const std::string& p_key, const float p_value)
	{
		std::stringstream ss;
		ss << p_value;
		SetStringValue(p_key, ss.str());
	}

	void Storage::SetValue(const std::string& p_key, const int p_value)
	{
		std::stringstream ss;
		ss << p_value;
		SetStringValue(p_key, ss.str());
	}

	void Storage::SetValue(const std::string& p_key, const bool p_value)
	{
		if(p_value) 
		{
			SetStringValue(p_key, "true");
		}
		else 
		{
			SetStringValue(p_key, "false");
		}
	}

	bool Storage::GetValueAsBool(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			if( m_values[p_key] == "True" || m_values[p_key] == "true" || m_values[p_key] == "1" )
			{
				return true;
			}
		return false;
	}

	int Storage::GetValueAsInt(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			return 0;

		int r;
		std::stringstream ss( m_values[p_key] );
		ss >> std::dec >> r;
		return r;
	}

	float Storage::GetValueAsFloat(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			return -99999.0f;

		float r;
		std::stringstream ss( m_values[p_key] );
		ss >> std::defaultfloat >> r;
		return r;
	}

	std::string Storage::GetValueAsString(const std::string& p_key)
	{
		if(m_values.find(p_key) != m_values.end())
			return m_values[p_key];
		return std::string("");
	}

	glm::vec4 Storage::GetValueAsVec4(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			return glm::vec4(1.0f);

		glm::vec4 vec;
		std::stringstream ss( m_values[p_key] );
		ss >> std::defaultfloat >> vec.x >> vec.y >> vec.z >> vec.w;
		return vec;
	}

	glm::vec3 Storage::GetValueAsVec3(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			return glm::vec3(1.0f);

		glm::vec3 vec;
		std::stringstream ss( m_values[p_key] );
		ss >> std::defaultfloat >> vec.x >> vec.y >> vec.z;
		return vec;
	}

	glm::vec2 Storage::GetValueAsVec2(const std::string& p_key)
	{
		if(m_values.find(p_key) == m_values.end())
			return glm::vec2(1.0f);

		glm::vec2 vec;
		std::stringstream ss( m_values[p_key] );
		ss >> std::defaultfloat >> vec.x >> vec.y;
		return vec;
	}
}

	