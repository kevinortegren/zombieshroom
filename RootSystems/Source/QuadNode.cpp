#include <RootSystems/Include/QuadNode.h>

namespace RootForce
{
	QuadNode::QuadNode(const AABB& p_bounds)
		: m_bounds(p_bounds), m_id(-1) {}

	void QuadNode::AddChild(QuadNode* p_child)
	{
		assert(m_childs.size() < 4 && "Attempting to add more then 4 childs for a node in quadtree");
		m_childs.push_back(p_child);
	}

	const AABB& QuadNode::GetBounds() const
	{
		return m_bounds;
	}

	const std::vector<QuadNode*>& QuadNode::GetChilds()
	{
		return m_childs;
	}
}