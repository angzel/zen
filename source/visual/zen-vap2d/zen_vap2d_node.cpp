
#include "zen_vap2d_node.h"
#include "zen_log.h"

namespace Zen { namespace Vap2d {

	VapObject::VapObject(std::string const & name)
	: m_object_name(name)
	{

	}
	void VapObject::setObjectName(const std::string &name)
	{
		m_object_name = name;
	}
	
	VapObject::~VapObject()
	{
#if ZEN_DEBUG
		Zen::LogV("delete vap-object: %s", m_object_name.data());
#endif
	}

	static void SortNodes(std::vector<Node*> & nodes)
	{
		if(nodes.size() < 2) return;
		for(size_t i = 0; i < nodes.size() - 1; ++i)
		{
			bool swaped = false;
			for(auto j = nodes.size()- 1; j > i; --j)
			{
				auto & a = nodes[j];
				auto & b = nodes[j - 1];
				if(a->getOrder() < b->getOrder())
				{
					std::swap(a, b);
					swaped = true;
				}
			}
			if(!swaped) break;
		}
		return;
	}

	BranchNode::BranchNode(std::string const & name)
	: Node(name)
	{
	}

	Node * BranchNode::onTouchDown(AppTouch const & touch)
	{
		this->clearNodesDirty();
		for(auto iter = m_nodes.rbegin(); iter != m_nodes.rend(); ++iter)
		{
			auto res = (*iter)->onTouchDown(touch);
			if(res) return res;
		}
		return nullptr;
	}

	Node * BranchNode::onTouchMove(AppTouch const & touch)
	{
		return this;
	}

	Node * BranchNode::onTouchUp(AppTouch const & touch)
	{
		return this;
	}

	Node * BranchNode::onTouchCancel(AppTouch const & touch) {
		return this->onTouchUp(touch);
	}
	void BranchNode::draw()
	{
		this->clearNodesDirty();
		for(auto i : m_nodes)
		{
			if(i->isVisible()) i->draw();
		}
	}

	void BranchNode::addNode(Node * node)
	{
		if(node->m_upper) throw "node already in the tree";
		m_nodes.push_back(node);
		node->m_upper = this;
		this->setNodesDirty();
	}

	void BranchNode::removeAllNodes()
	{
		for(auto i : m_nodes)
		{
			i->m_upper = nullptr;
			i->remove();
		}
		m_nodes.clear();

	}
	void BranchNode::setNodesDirty()
	{
		m_nodes_dirty = true;
	}

	void BranchNode::clearNodesDirty()
	{
		if(!m_nodes_dirty) return;
		SortNodes(m_nodes);
		m_nodes_dirty = false;
	}
	/**
	 @ref zen_vap2d_root.cpp
	 void Node::remove() {};
	 void Node::runAction(Action * action);
	 void Node::removeAction(Action * action);
	 */;
	Node::Node(std::string const & name)
	: VapObject(name)
	{
	}	void Node::setOrder(int index)
	{
		m_order = index;
		if(m_upper) m_upper->setNodesDirty();
	}

	void Node::setVisible(bool v)
	{
		m_is_visible = v;
	}

	int Node::getOrder() const
	{
		return m_order;

	}

	bool Node::isVisible() const
	{
		return m_is_visible;
	}

	void Node::setBlend(eBlend blend)
	{
		m_blend = blend;
	}
	eBlend Node::getBlend()
	{
		return m_blend;
	}
	void Node::setAlpha(float alpha)
	{
		m_alpha = alpha;
	}
	float Node::getAlpha()
	{
		return m_alpha;
	}

	FinalNode::FinalNode(std::string const & name)
	: Node(name)
	{

	}

	/**
	 Blend
	 */
	DrawStack * DrawStack::Get()
	{
		static auto single = new DrawStack;
		return single;;
	}
	DrawStack::DrawStack()
	{
		m_performed_blend = eBlend::Inherit;
	}
	DrawStack::~DrawStack()
	{
	}
	void DrawStack::pushMatrix(Matrix4 const * mat, bool dirty)
	{
		this->m_mats.push_back({mat, dirty});
	}

	void DrawStack::popMatrix()
	{
		this->m_mats.pop_back();
	}

	void DrawStack::pushAlpha(float a)
	{
		if(!m_alphas.empty()) a *= getTopAlpha();
		this->m_alphas.push_back(a);
	}

	void DrawStack::popAlpha()
	{
		this->m_alphas.pop_back();
	}

	void DrawStack::pushBlend(eBlend blend)
	{
		if(blend == eBlend::Inherit) blend = m_blends.back();
		m_blends.push_back(blend);
	}

	void DrawStack::popBlend()
	{
		m_blends.pop_back();
	}

	Matrix4 const * DrawStack::getTopMatrix4()
	{
		return this->m_mats.back().mat;
	}

	bool DrawStack::isTopMatrix4Dirty()
	{
		return this->m_mats.back().dirty;
	}

	float DrawStack::getTopAlpha()
	{
		return this->m_alphas.back();
	}
	eBlend DrawStack::getTopBlend()
	{
		return m_blends.back();
	}
	void DrawStack::reset()
	{
		this->m_alphas.clear();
		this->m_mats.clear();
		this->m_blends.clear();
	}

	/**
	 view
	 */
	void View::setPosition(float x, float y)
	{
		m_pos.x = x;
		m_pos.y = y;
		m_is_matrix_dirty = true;
	}
	void View::setX(float x)
	{
		m_pos.x = x;
		m_is_matrix_dirty = true;
	}
	void View::setY(float y)
	{
		m_pos.y = y;
		m_is_matrix_dirty = true;
	}
	void View::setScale(float sx, float sy)
	{
		m_scale = { sx, sy };
		m_is_matrix_dirty = true;
	}
	void View::setScaleX(float sx)
	{
		m_scale.x = sx;
		m_is_matrix_dirty = true;
	}
	void View::setScaleY(float sy)
	{
		m_scale.y = sy;
		m_is_matrix_dirty = true;
	}
	void View::setSkew(float kx, float ky)
	{
		m_skew = { kx, ky };
		m_is_matrix_dirty = true;
	}
	void View::setSkewX(float kx)
	{
		m_skew.x = kx;
		m_is_matrix_dirty = true;
	}
	void View::setSkewY(float ky)
	{
		m_skew.y = ky;
		m_is_matrix_dirty = true;
	}
	void View::setRotation(float radians)
	{
		m_rotation = radians;
		m_is_matrix_dirty = true;
	}
	float View::getX()
	{
		return m_pos.x;
	}
	float View::getY()
	{
		return m_pos.y;
	}
	float View::getScaleX()
	{
		return m_scale.x;
	}
	float View::getScaleY()
	{
		return m_scale.y;
	}
	float View::getSkewX()
	{
		return m_skew.x;
	}
	float View::getSkewY()
	{
		return m_skew.y;
	}
	float View::getRotation()
	{
		return m_rotation;
	}
	Point2 View::getPosition()
	{
		return m_pos;
	}
	Point2 View::getScale()
	{
		return m_scale;
	}
	Point2 View::getSkew()
	{
		return m_skew;
	}
	void View::setPosition(Point2 pos)
	{
		m_pos = pos;
	}
	void View::setScale(Point2 scale)
	{
		m_scale = scale;
	}
	void View::setSkew(Point2 skew)
	{
		m_skew = skew;
	}


	void View::_clearInnerMatrixDirty()
	{
		float cos = cosf(m_rotation);
		float sin = sinf(m_rotation);

		m_matrix.m[0] = cos * m_skew.x * m_scale.x;
		m_matrix.m[1] = sin * m_skew.y * m_scale.x;
		m_matrix.m[4] = -sin * m_skew.x * m_scale.y;
		m_matrix.m[5] = cos * m_skew.y * m_scale.y;
		m_matrix.m[12] = m_pos.x;
		m_matrix.m[13] = m_pos.y;
	}

	void SizeView::setAnchor(float ax, float ay)
	{
		m_anchor = { ax, ay };
		m_is_matrix_dirty = true;
	}
	void SizeView::setAnchorX(float ax)
	{
		m_anchor.x = ax;
		m_is_matrix_dirty = true;
	}
	void SizeView::setAnchorY(float ay)
	{
		m_anchor.y = ay;
		m_is_matrix_dirty = true;
	}
	Point2 SizeView::getAnchor()
	{
		return m_anchor;
	}
	float SizeView::getAnchorX()
	{
		return m_anchor.x;
	}
	float SizeView::getAnchorY()
	{
		return m_anchor.y;
	}
	void SizeView::setAnchor(Point2 anchor)
	{
		m_anchor = anchor;
		m_is_matrix_dirty = true;
	}
	void SizeView::_setScale2(float x, float y)
	{
		m_scale2 = { x, y };
		m_is_matrix_dirty = true;
	}

	void SizeView::_clearInnerMatrixDirty()
	{
		float cos = cosf(m_rotation);
		float sin = sinf(m_rotation);
		float x = m_scale2.w * m_scale.x;
		float y = m_scale2.h * m_scale.y;

		m_matrix.m[0] = cos * m_skew.x * x;
		m_matrix.m[1] = sin * m_skew.y * x;
		m_matrix.m[4] = -sin * m_skew.x * y;
		m_matrix.m[5] = cos * m_skew.y * y;
		m_matrix.m[12] = m_pos.x - m_matrix.m[0] * m_anchor.x - m_matrix.m[4] * m_anchor.y;
		m_matrix.m[13] = m_pos.y - m_matrix.m[1] * m_anchor.x - m_matrix.m[5] * m_anchor.y;
	}
	bool Matrixing::updateMatrix()
	{
		auto rs = DrawStack::Get();
		auto top_m_p = rs->getTopMatrix4();
		if(m_is_matrix_dirty)
		{
			m_is_matrix_dirty = false;

			this->_clearInnerMatrixDirty();

			if(top_m_p)
			{
				m_world_matrix = Matrix4Multiply(*top_m_p, m_matrix);
			}
			else
			{
				m_world_matrix = m_matrix;
			}
			return true;
		}
		else if(top_m_p && rs->isTopMatrix4Dirty())
		{
			m_world_matrix = Matrix4Multiply(*top_m_p, m_matrix);
			return true;
		}
		return false;
	}

	void Colorful::setColor(Zen::Color4f color)
	{
		m_color = color;
	}

	void Colorful::setGreyMode(bool grey)
	{
		m_is_grey_mode = grey;
	}
	bool Colorful::isGreyMode()
	{
		return m_is_grey_mode;
	}
	Zen::Color4f Colorful::getColor()
	{
		return m_color;
	}
	void Flipable::setFlipX(bool x)
	{
		if(x == m_is_flip_x) return;
		m_is_flip_x = x;
		m_is_flip_dirty = true;
	}

	void Flipable::setFlipY(bool y)
	{
		if(y == m_is_flip_y) return;
		m_is_flip_y = y;
		m_is_flip_dirty = true;
	}

	bool Flipable::isFlipX()
	{
		return m_is_flip_x;
	}

	bool Flipable::isFlipY()
	{
		return m_is_flip_y;
	}
}}


