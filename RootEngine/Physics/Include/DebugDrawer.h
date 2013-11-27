#include <Bullet/LinearMath/btIDebugDraw.h>

class DebugDrawer : public btIDebugDraw
{
	int m_debugMode;

public:
	DebugDrawer();
	virtual ~DebugDrawer();

	void   drawLine(const btVector3& p_from, const btVector3& p_to, const btVector3& p_color);
	virtual void   drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {};

	virtual void   reportErrorWarning(const char* warningString){};

	virtual void   draw3dText(const btVector3& location, const char* textString){};
	virtual void   setDebugMode(int p_debugmode) {m_debugMode = p_debugmode ;}

	virtual int      getDebugMode() const { return m_debugMode; }
};