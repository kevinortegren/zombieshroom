#pragma once

#define GLM_SWIZZLE
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

		glm::vec3 GetAxis();
		float GetAngle();
		glm::vec3 GetRight();
		glm::vec3 GetUp();
		glm::vec3 GetFront();
		glm::mat3 GetMatrix();

		void SetOrientation(float p_rotationX, float p_rotationY, float p_rotationZ);
		void SetOrientation(float p_angle, glm::vec3 p_axis);
		void SetOrientation(glm::quat p_orientation);

		void Pitch(float p_angle);
		void Yaw(float p_angle);
		void Roll(float p_angle);
		void Rotate(float p_rotationX, float p_rotationY, float p_rotationZ);
		void Rotate(float p_angle, glm::vec3 p_axis);
		void Rotate(glm::quat p_rotation);

		void PitchGlobal(float p_angle);
		void YawGlobal(float p_angle);
		void RollGlobal(float p_angle);
		void RotateGlobal(float p_rotationX, float p_rotationY, float p_rotationZ);
		void RotateGlobal(float p_angle, glm::vec3 p_axis);
		void RotateGlobal(glm::quat p_rotation);

	private:
		glm::quat m_orientation;
	};
}