
#include "zen_vap_node.h"
#include "zen_log.h"

#define BNode Node
namespace Zen { namespace Vap {

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

	Node * BNode::onTouchDown(AppTouch const & touch)
	{
		this->clearNodesOrder();
		for(auto iter = m_nodes.rbegin(); iter != m_nodes.rend(); ++iter)
		{
			auto res = (*iter)->onTouchDown(touch);
			if(res) return res;
		}
		return nullptr;
	}

	Node * BNode::onTouchMove(AppTouch const & touch)
	{
		return nullptr;
	}

	Node * BNode::onTouchUp(AppTouch const & touch)
	{
		return nullptr;
	}

	Node * BNode::onTouchCancel(AppTouch const & touch) {
		return this->onTouchUp(touch);
	}
	
	void BNode::draw()
	{
		this->clearNodesOrder();
		
		for(auto i : m_nodes)
		{
			if(i->isVisible()) i->draw();
		}
	}

	void BNode::addNode(Node * node)
	{
		if(node->m_upper) throw "node already in the tree";
		m_nodes.push_back(node);
		node->m_upper = this;
		this->setNodesOrderDirty();
	}

	void BNode::removeAllNodes()
	{
		for(auto i : m_nodes)
		{
			i->m_upper = nullptr;
			i->remove();
		}
		m_nodes.clear();

	}
	void BNode::setNodesOrderDirty()
	{
		m_nodes_dirty = true;
	}

	void BNode::clearNodesOrder()
	{
		if(!m_nodes_dirty) return;
		SortNodes(m_nodes);
		m_nodes_dirty = false;
	}
	/**
	 @ref zen_vap_root.cpp
	 void Node::remove() {};
	 void Node::runAction(Action * action);
	 void Node::removeAction(Action * action);
	 */;
	Node::Node(std::string const & name)
	: VapObject(name)
	{
	}

	Node::~Node()
	{
	}

	void Node::setVisible(bool v)
	{
		m_is_visible = v;
	}
	
	void Node::setOrder(int index)
	{
		if(m_order == index) return;
		m_order = index;
		if(m_upper) m_upper->setNodesOrderDirty();
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

	Node2d::Node2d(std::string const & name)
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
//		m_performed_blend = eBlend::Inherit;
	}
	DrawStack::~DrawStack()
	{
	}
	void DrawStack::pushMatrix(Matrix4 const * mat, int version)
	{
		this->m_mats.push_back({mat, version});
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
		if(this->m_mats.empty()) return nullptr;
		return this->m_mats.back().mat;
	}

	int DrawStack::getTopMatrix4Version()
	{
		if(this->m_mats.empty()) return 0;
		return this->m_mats.back().version;
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
		
		this->m_blends.push_back(eBlend::None);
		this->m_mats.push_back(MatrixInfo{nullptr, 0 });
		this->m_alphas.push_back(1.f);
	}

	void Matrixing::setMatrix(const Matrix4 &matrix, int upper_version)
	{
		m_final_matrix = matrix;
		m_upper_matrix_version = upper_version;
		++m_matrix_version;
	}
	bool Matrixing::isUpperMatrixExpired(int upper_version)
	{
		return (upper_version != m_upper_matrix_version);
	}
	/**
	 view
	 */
	void View2d::setPosition(float x, float y)
	{
		m_pos.x = x;
		m_pos.y = y;
		m_is_matrix_dirty = true;
	}
	void View2d::setX(float x)
	{
		m_pos.x = x;
		m_is_matrix_dirty = true;
	}
	void View2d::setY(float y)
	{
		m_pos.y = y;
		m_is_matrix_dirty = true;
	}
	void View2d::setScale(float sx, float sy)
	{
		m_scale = { sx, sy };
		m_is_matrix_dirty = true;
	}
	void View2d::setScaleX(float sx)
	{
		m_scale.x = sx;
		m_is_matrix_dirty = true;
	}
	void View2d::setScaleY(float sy)
	{
		m_scale.y = sy;
		m_is_matrix_dirty = true;
	}
	void View2d::setSkew(float kx, float ky)
	{
		m_skew = { kx, ky };
		m_is_matrix_dirty = true;
	}
	void View2d::setSkewX(float kx)
	{
		m_skew.x = kx;
		m_is_matrix_dirty = true;
	}
	void View2d::setSkewY(float ky)
	{
		m_skew.y = ky;
		m_is_matrix_dirty = true;
	}
	void View2d::setRotation(float radians)
	{
		m_rotation = radians;
		m_is_matrix_dirty = true;
	}
	float View2d::getX()
	{
		return m_pos.x;
	}
	float View2d::getY()
	{
		return m_pos.y;
	}
	float View2d::getScaleX()
	{
		return m_scale.x;
	}
	float View2d::getScaleY()
	{
		return m_scale.y;
	}
	float View2d::getSkewX()
	{
		return m_skew.x;
	}
	float View2d::getSkewY()
	{
		return m_skew.y;
	}
	float View2d::getRotation()
	{
		return m_rotation;
	}
	Point2 View2d::getPosition()
	{
		return m_pos;
	}
	Point2 View2d::getScale()
	{
		return m_scale;
	}
	Point2 View2d::getSkew()
	{
		return m_skew;
	}
	void View2d::setPosition(Point2 pos)
	{
		m_pos = pos;
	}
	void View2d::setScale(Point2 scale)
	{
		m_scale = scale;
	}
	void View2d::setSkew(Point2 skew)
	{
		m_skew = skew;
	}

	void View2d::setAnchor(float ax, float ay)
	{
		m_anchor = { ax, ay };
		m_is_matrix_dirty = true;
	}
	void View2d::setAnchorX(float ax)
	{
		m_anchor.x = ax;
		m_is_matrix_dirty = true;
	}
	void View2d::setAnchorY(float ay)
	{
		m_anchor.y = ay;
		m_is_matrix_dirty = true;
	}
	Point2 View2d::getAnchor()
	{
		return m_anchor;
	}
	float View2d::getAnchorX()
	{
		return m_anchor.x;
	}
	float View2d::getAnchorY()
	{
		return m_anchor.y;
	}
	void View2d::setAnchor(Point2 anchor)
	{
		m_anchor = anchor;
		m_is_matrix_dirty = true;
	}
	void View2d::setScale2_(float x, float y)
	{
		m_scale2 = { x, y };
		m_is_matrix_dirty = true;
	}
	
	View2d::View2d(bool is_anchor_valid)
	{
		m_is_anchor_valid = is_anchor_valid;
	}
	void View2d::updateMatrix()
	{
		auto rs = DrawStack::Get();
		auto upper = rs->getTopMatrix4();
		auto version = rs->getTopMatrix4Version();
		
		if(m_is_matrix_dirty)
		{
			m_is_matrix_dirty = false;
			
			Matrix4 & m = m_matrix0;
			if(!m_is_anchor_valid)
			{
				float cos = cosf(m_rotation);
				float sin = sinf(m_rotation);
				float x = m_scale2.x * m_scale.x;
				float y = m_scale2.y * m_scale.y;

				m[0] = cos * m_skew.x * x;
				m[1] = sin * m_skew.y * x;
				m[4] = -sin * m_skew.x * y;
				m[5] = cos * m_skew.y * y;
				m[12] = m_pos.x;
				m[13] = m_pos.y;
			}
			else {
				float cos = cosf(m_rotation);
				float sin = sinf(m_rotation);
				float x = m_scale2.x * m_scale.x;
				float y = m_scale2.y * m_scale.y;

				m[0] = cos * m_skew.x * x;
				m[1] = sin * m_skew.y * x;
				m[4] = -sin * m_skew.x * y;
				m[5] = cos * m_skew.y * y;
				m[12] = m_pos.x - m[0] * m_anchor.x - m[4] * m_anchor.y;
				m[13] = m_pos.y - m[1] * m_anchor.x - m[5] * m_anchor.y;
			}

			if(upper)
			{
				setMatrix(Matrix4Multiply(*upper, m_matrix0), version);
			}
			else
			{
				setMatrix(m_matrix0, 0);
			}
		}
		else if(upper && isUpperMatrixExpired(version))
		{
			setMatrix(Matrix4Multiply(*upper, m_matrix0), version);
		}
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


