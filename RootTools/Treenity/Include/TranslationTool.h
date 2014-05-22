#pragma once

#include <RootTools/Treenity/Include/Tool.h>
#include <RootEngine/Render/Include/Material.h>
#include <RootTools/Treenity/Include/EngineInterface.h>

namespace TranslationAxis
{
	enum TranslationAxis
	{
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_NONE,
	};
}

class TranslationTool : public Tool
{
public:
	TranslationTool();
	~TranslationTool();

	void LoadResources(ECS::World* p_world);
	bool Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray);

	void SetPosition(const glm::vec3& p_position);
	void Hide();
	void Show();

private:

	//m_selected axis is set only when LMB is down. When LMB is released m_selectedAxis is always AXIS_NONE. However, the m_hoverAxis is set when the mouse is released. 
	TranslationAxis::TranslationAxis m_selectedAxis;
	TranslationAxis::TranslationAxis m_hoverAxis;

	glm::vec3 m_position_t0;

	float m_axisLength;
	float m_dragOffset;
	bool m_firstDrag;
	
	struct AxisBoundingBox
	{
		AxisBoundingBox(){}
		AxisBoundingBox(glm::vec3 p_lower, glm::vec3 p_upper) : m_lower(p_lower), m_upper(p_upper)
		{}
		glm::vec3 m_lower;
		glm::vec3 m_upper;
	};

	struct PointOnPlane
	{
		PointOnPlane(){}
		PointOnPlane(const glm::vec3& p_point, bool p_hit) : Point(p_point), Hit(p_hit){}

		glm::vec3 Point;
		bool Hit;
	};

	PointOnPlane GetPointOnPlane(const glm::vec3& p_camPos, const glm::vec3& p_worldCamPos,const glm::vec3& p_rayDir );
	float GetDragOffset( float p_pointOnAxis, float p_pointOfEntity );
	float CheckRayVsAABB(const glm::vec3& p_rayDir, const glm::vec3& p_rayOrigin, const glm::vec3& p_bound1, const glm::vec3& p_bound2 );
	void Drag( const glm::vec3& p_ray, const glm::vec3& p_camPos );
	TranslationAxis::TranslationAxis RayVsAxis(const glm::vec3& p_cameraPos, const glm::vec3& p_ray);

	AxisBoundingBox m_axisAABB[3];
};