#include <Bullet/LinearMath/btIDebugDraw.h>
#include <vector>
#include <RootEngine/Render/Include/Renderer.h>

class DebugDrawer : public btIDebugDraw
{
	int m_debugMode;

public:
	DebugDrawer();
	virtual ~DebugDrawer();

	 void   drawLine(const btVector3& p_from, const btVector3& p_to, const btVector3& p_color);
	 void   drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {};

	 void   reportErrorWarning(const char* warningString){};

	 void   draw3dText(const btVector3& location, const char* textString){};
	 void   setDebugMode(int p_debugmode) {m_debugMode = p_debugmode ;}

	 int      getDebugMode() const { return m_debugMode; }

	 void ClearDebugVectors();

	 std::vector<glm::vec3> GetDebugVectors()const {return m_debugVectors;}
private:
	std::vector<glm::vec3> m_debugVectors;
};