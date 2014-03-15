#include <RootSystems/Include/Shapes/AABB.h>

namespace RootForce
{
	class QuadNode
	{
	public:
		friend class QuadTree;
		QuadNode(const AABB& p_bounds);
		~QuadNode();
		void AddChild(QuadNode* p_child);
		const std::vector<QuadNode*>& GetChilds();
		const AABB& GetBounds() const;

		int m_id;

		// Indices to entity vector.
		std::vector<unsigned> m_indices;

	private:
		const AABB m_bounds;
		std::vector<QuadNode*> m_childs;	
	};
}