#include <Utility\ECS\Include\Shared\Orientation.h>

using glm::vec3;
using glm::mat3;
using glm::quat;
using glm::degrees;
using glm::radians;
using glm::normalize;

namespace RootForce
{
	Orientation::Orientation()
	{
		m_orientation = quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	Orientation::Orientation(float p_rotationX, float p_rotationY, float p_rotationZ)
	{
		SetOrientation(p_rotationX, p_rotationY, p_rotationZ);
	}

	Orientation::Orientation(float p_angle, vec3 p_axis)
	{
		SetOrientation(p_angle, p_axis);
	}

	Orientation::Orientation(quat p_orientation)
	{
		SetOrientation(p_orientation);
	}

	vec3 Orientation::GetAxis()
	{
		if (m_orientation.w >= 1 || m_orientation.w <= -1)
		{
			return vec3(1.0f, 0.0f, 0.0f);
		}
		float tmp = sqrt(1 - m_orientation.w * m_orientation.w);
		return normalize(vec3(	m_orientation.x / tmp,
								m_orientation.y / tmp,
								m_orientation.z / tmp));
	}

	float Orientation::GetAngle()
	{
		if(m_orientation.w > 1)
		{
			m_orientation.w = 1;
		}
		else if (m_orientation.w < -1)
		{
			m_orientation.w = -1;
		}
		return 2 * degrees(acos(m_orientation.w));
	}

	vec3 Orientation::GetRight()
	{
		return normalize(-vec3(	1 -	2 * (m_orientation.y * m_orientation.y + m_orientation.z * m_orientation.z),
									2 * (m_orientation.x * m_orientation.y + m_orientation.z * m_orientation.w),
									2 * (m_orientation.x * m_orientation.z - m_orientation.y * m_orientation.w)));
	}

	vec3 Orientation::GetUp()
	{
		return normalize(vec3(		2 * (m_orientation.x * m_orientation.y - m_orientation.z * m_orientation.w),
								1 -	2 * (m_orientation.x * m_orientation.x + m_orientation.z * m_orientation.z),
									2 * (m_orientation.y * m_orientation.z + m_orientation.x * m_orientation.w)));
	}

	vec3 Orientation::GetFront()
	{
		return normalize(vec3(		2 * (m_orientation.x * m_orientation.z + m_orientation.y * m_orientation.w),
									2 * (m_orientation.y * m_orientation.z - m_orientation.x * m_orientation.w),
								1 -	2 * (m_orientation.x * m_orientation.x + m_orientation.y * m_orientation.y)));
	}

	mat3 Orientation::GetMatrix()
	{
		return mat3(GetRight(), GetUp(), GetFront());
	}


	void Orientation::SetOrientation(float p_rotationX, float p_rotationY, float p_rotationZ)
	{
		m_orientation = quat(1.0f, 0.0f, 0.0f, 0.0f);
		RotateGlobal(p_rotationX, p_rotationY, p_rotationZ);
	}

	void Orientation::SetOrientation(float p_angle, vec3 p_axis)
	{
		m_orientation = quat(1.0f, 0.0f, 0.0f, 0.0f);
		RotateGlobal(p_angle, p_axis);
	}

	void Orientation::SetOrientation(quat p_orientation)
	{
		m_orientation = p_orientation;
	}

	void Orientation::Pitch(float p_angle)
	{
		if(p_angle == 0.0f)
			return;
		quat rotation(cos(radians(p_angle) / 2), sin(radians(p_angle) / 2), 0, 0);
		Rotate(rotation);
	}

	void Orientation::Yaw(float p_angle)
	{
		if(p_angle == 0.0f)
			return;
		quat rotation(cos(radians(p_angle) / 2), 0, sin(radians(p_angle) / 2), 0);
		Rotate(rotation);
	}

	void Orientation::Roll(float p_angle)
	{
		if(p_angle == 0.0f)
			return;
		quat rotation(cos(radians(p_angle) / 2), 0, 0, sin(radians(p_angle) / 2));
		Rotate(rotation);
	}

	void Orientation::Rotate(float p_rotationX, float p_rotationY, float p_rotationZ)
	{
		Pitch(p_rotationX);
		Yaw(p_rotationY);
		Roll(p_rotationZ);
	}

	void Orientation::Rotate(float p_angle, vec3 p_axis)
	{
		if(p_angle == 0.0f)
			return;
		p_axis = normalize(p_axis);
		float tmp = sin(radians(p_angle) / 2);
		quat rotation(cos(radians(p_angle) / 2), p_axis.x * tmp, p_axis.y * tmp, p_axis.z * tmp);
		Rotate(rotation);
	}

	void Orientation::Rotate(quat p_orientation)
	{
		m_orientation = normalize(glm::cross(m_orientation, p_orientation));
	}

	void Orientation::PitchGlobal(float p_angle)
	{
		if(p_angle == 0.0f)
			return;
		quat rotation(cos(radians(p_angle)/2), sin(radians(p_angle)/2), 0, 0);
		RotateGlobal(rotation);
	}

	void Orientation::YawGlobal(float p_angle)
	{
		if(p_angle == 0.0f)
			return;
		quat rotation(cos(radians(p_angle)/2), 0, sin(radians(p_angle)/2), 0);
		RotateGlobal(rotation);
	}

	void Orientation::RollGlobal(float p_angle)
	{
		if(p_angle == 0.0f)
			return;
		quat rotation(cos(radians(p_angle)/2), 0, 0, sin(radians(p_angle)/2));
		RotateGlobal(rotation);
	}

	void Orientation::RotateGlobal(float p_rotationX, float p_rotationY, float p_rotationZ)
	{
		PitchGlobal(p_rotationX);
		YawGlobal(p_rotationY);
		RollGlobal(p_rotationZ);
	}

	void Orientation::RotateGlobal(float p_angle, vec3 p_axis)
	{
		if(p_angle == 0)
			return;
		p_axis = normalize(p_axis);
		float tmp = sin(radians(p_angle)/2);
		quat rotation(cos(radians(p_angle) / 2), p_axis.x * tmp, p_axis.y * tmp, p_axis.z * tmp);
		RotateGlobal(rotation);
	}

	void Orientation::RotateGlobal(quat p_orientation)
	{
		m_orientation = glm::cross(p_orientation, m_orientation);
	}

	void Orientation::LookAt(glm::vec3 p_direction, glm::vec3 p_upVector)
	{
		m_orientation = quat(1.0f, 0.0f, 0.0f, 0.0f);
		vec3 direction = normalize(p_direction);
		vec3 up = normalize(p_upVector);
		vec3 axis = glm::cross(direction, vec3(0.0f, 0.0f, 1.0f));
		if(glm::length(axis) == 0)
		{
			axis = vec3(1.0f, 0.0f, 0.0f);
		}
		float angle = glm::degrees(glm::acos(glm::dot(direction, vec3(0.0f, 0.0f, 1.0f))));
		vec3 third = glm::cross(axis, vec3(0.0f, 0.0f, 1.0f));
		if (glm::dot(third, direction) < 0)
		{
			angle = -angle;
		}
		Rotate(angle, axis);

		//vec3 side1 = glm::cross(direction, GetUp());
		//vec3 side2 = glm::cross(direction, up);
		//vec3 newUp = glm::cross(side2, direction);
		//float angle2 = glm::degrees(glm::acos(glm::dot(side2, side1)));
		//RollGlobal(angle2);
	}

	glm::quat Orientation::GetQuaterion()
	{
		return m_orientation;
	}

}