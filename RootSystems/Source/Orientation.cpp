#include <Orientation.h>

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
		return mat3(-GetRight(), GetUp(), GetFront());
	}

	void Orientation::SetOrientation(const glm::mat3& p_rotationMatrix)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".

		float trace = p_rotationMatrix[0][0]+p_rotationMatrix[1][1]+p_rotationMatrix[2][2];
		float root;

		if ( trace > 0.0 )
		{
			// |w| > 1/2, may as well choose w > 1/2
			root = glm::sqrt(trace + 1.0f);  // 2w
			m_orientation.w = 0.5f*root;
			root = 0.5f/root;  // 1/(4w)
			m_orientation.x = (p_rotationMatrix[2][1]-p_rotationMatrix[1][2])*root;
			m_orientation.y = (p_rotationMatrix[0][2]-p_rotationMatrix[2][0])*root;
			m_orientation.z = (p_rotationMatrix[1][0]-p_rotationMatrix[0][1])*root;
		}
		else
		{
			// |w| <= 1/2
			static size_t next[3] = { 1, 2, 0 };
			size_t i = 0;
			if ( p_rotationMatrix[1][1] > p_rotationMatrix[0][0] )
				i = 1;
			if ( p_rotationMatrix[2][2] > p_rotationMatrix[i][i] )
				i = 2;
			size_t j = next[i];
			size_t k = next[j];

			root = glm::sqrt(p_rotationMatrix[i][i]-p_rotationMatrix[j][j]-p_rotationMatrix[k][k] + 1.0f);
			float* apkQuat[3] = { &m_orientation.x, &m_orientation.y, &m_orientation.z };
			*apkQuat[i] = 0.5f*root;
			root = 0.5f/root;
			m_orientation.w = (p_rotationMatrix[k][j]-p_rotationMatrix[j][k])*root;
			*apkQuat[j] = (p_rotationMatrix[j][i]+p_rotationMatrix[i][j])*root;
			*apkQuat[k] = (p_rotationMatrix[k][i]+p_rotationMatrix[i][k])*root;
		}
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
		//	axis = vec3(1.0f, 0.0f, 0.0f);
		}
		float angle = glm::degrees(glm::acos(glm::dot(direction, vec3(0.0f, 0.0f, 1.0f))));
		vec3 third = glm::cross(axis, vec3(0.0f, 0.0f, 1.0f));
		if (glm::dot(third, direction) < 0)
		{
			angle = -angle;
		}
		Rotate(angle, axis);

		vec3 targetRight = normalize(glm::cross(direction, up));
		vec3 myRight = normalize(glm::cross(direction, GetUp()));
		float angle2 = glm::degrees(glm::acos(glm::dot(targetRight, myRight)));
		vec3 third2 = glm::cross(direction, myRight);
		if (glm::dot(third2, targetRight) < 0)
		{
			angle2 = -angle2;
		}
		Roll(angle2);

		//vec3 left = glm::cross(up, direction);
		//up = glm::cross(direction, left);
		//mat3 mat(left, up, direction);

		//SetOrientation(mat);
	}

	glm::quat Orientation::GetQuaterion()
	{
		return m_orientation;
	}

}