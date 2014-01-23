#pragma once

//#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace RootForce
{
	class Orientation
	{
	public:
		Orientation();
		Orientation(float p_rotationX, float p_rotationY, float p_rotationZ);
		Orientation(float p_angle, glm::vec3 p_axis);
		Orientation(glm::quat p_orientation);

		//////////////////////////////////////////////////////////////////////////
		//Getters
		glm::quat GetQuaternion();
		glm::mat3 GetMatrix();
		//Angle-axis representation
		glm::vec3 GetAxis();
		float GetAngle();
		//Object space to world space axis getters (Very useful!)
		glm::vec3 GetRight();
		glm::vec3 GetUp();
		glm::vec3 GetFront();
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//The setters support 4 rotation representations:
		//* 3x3 rotation matrix
		//* Euler angle rotation (ew!)
		//* Angle-axis rotation
		//* Quaternion rotation
		void SetOrientation(const glm::mat3& p_rotationMatrix);
		void SetOrientation(float p_rotationX, float p_rotationY, float p_rotationZ);
		void SetOrientation(float p_angle, glm::vec3 p_axis);
		void SetOrientation(glm::quat p_orientation);
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//Rotation operations:
		//Local
		void Pitch(float p_angle);
		void Yaw(float p_angle);
		void Roll(float p_angle);
		void Rotate(float p_rotationX, float p_rotationY, float p_rotationZ);
		void Rotate(float p_angle, glm::vec3 p_axis);
		void Rotate(glm::quat p_rotation);
		//Global
		void PitchGlobal(float p_angle);
		void YawGlobal(float p_angle);
		void RollGlobal(float p_angle);
		void RotateGlobal(float p_rotationX, float p_rotationY, float p_rotationZ);
		void RotateGlobal(float p_angle, glm::vec3 p_axis);
		void RotateGlobal(glm::quat p_rotation);
		//Others
		void LookAt(glm::vec3 p_direction, glm::vec3 p_upVector);
		//To-do:
		//Interpolation operations
		//////////////////////////////////////////////////////////////////////////

	private:
		glm::quat m_orientation;
	};
}