
#include "zen_vap2d_node.h"

namespace Zen { namespace Vap2d {
	
	static void SortNodes(std::vector<Node*> & nodes)
	{
		if(nodes.size() <= 1) return;
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
	
	void BNode::draw()
	{
		if(m_nodes_dirty)
		{
			SortNodes(m_nodes);
			m_nodes_dirty = false;
		}
		for(auto i : m_nodes) i->draw();
	}
	void BNode::addNode(Node * node)
	{
		if(node->m_parent) throw "node already in the tree";
		m_nodes.push_back(node);
		node->m_parent = this;
		this->setNodesDirty();
	}
	void BNode::_remove_node(Node * node)
	{
		for(auto i = m_nodes.begin(); i != m_nodes.end(); ++i)
		{
			if(*i == node)
			{
				m_nodes.erase(i);
				delete node;
			}
		}
	}
	void BNode::removeAllNodes()
	{
		for(auto i : m_nodes)
		{
			delete i;
		}
		m_nodes.clear();
		
	}
	void BNode::setNodesDirty()
	{
		m_nodes_dirty = true;
	}
	
	BNode::~BNode()
	{
		this->removeAllNodes();
	}
	void Node::remove()
	{
		if(m_parent) m_parent->_remove_node(this);
		delete this;
	}
	void Node::setAlpha(float alpha)
	{
		m_alpha = alpha;
	}
	void Node::setOrder(int index)
	{
		m_order = index;
		if(m_parent) m_parent->setNodesDirty();
	}
	

	RNode * RNode::GetDefault()
	{
		static auto me = new RNode;
		return me;
	}

	void RNode::setDesignViewSize(float width, float height)
	{
		m_design_view_size = { width, height };
		m_is_view_dirty = true;
	}

	Size2 RNode::getDesignViewSize()
	{
		return m_design_view_size;
	}

	void RNode::draw()
	{
		bool dirty = false;
		if(m_is_view_dirty)
		{
			m_matrix = {
				2 / m_design_view_size.w, 0, 0, 0,
				0, 2 / m_design_view_size.h, 0, 0,
				0, 0, 1, 0,
				-1, -1, 0, 1
			};
			dirty = true;
			m_is_view_dirty = false;
		}
		RenderNode rn(&m_matrix, dirty, 1.0f, EBlend::Normal);
		BNode::draw();
	}
	void RNode::remove()
	{
		throw "Root node cannot be removed";
	}
	/**
	 Blend
	 */
	void RenderStack::performBlend(EBlend blend)
	{
		if(blend == EBlend::Inherit)
		{
			blend = m_blends.back();
		}
		if(blend == m_performed_blend) return;

		using namespace Zen::GL;
		switch (blend) {
			case EBlend::Normal:
				Render::SetBlendFunc
				(Zen::GL::EBlendSrc::SrcAlpha,
				 Zen::GL::EBlendDst::OneMinusSrcAlpha);
				break;
			case EBlend::Add:
				Render::SetBlendFunc
				(Zen::GL::EBlendSrc::SrcAlpha,
				 Zen::GL::EBlendDst::One);
				break;
			case EBlend::Reverse:
				Render::SetBlendFunc
				(Zen::GL::EBlendSrc::Zero,
				 Zen::GL::EBlendDst::OneMinusSrcColor);
				break;
			default:
				break;
		}
		m_performed_blend = blend;
	}

	RenderStack * RenderStack::GetDefault()
	{
		static auto me = new RenderStack;
		return me;
	}
	RenderStack::RenderStack()
	{
		m_performed_blend = EBlend::Inherit;
	}
	void RenderStack::pushMatrix(Matrix4 const * mat, bool dirty)
	{
		this->m_mats.push_back({mat, dirty});
	}
	
	void RenderStack::popMatrix()
	{
		this->m_mats.pop_back();
	}
	
	void RenderStack::pushAlpha(float a)
	{
		this->m_alphas.push_back(a);
	}

	void RenderStack::popAlpha()
	{
		this->m_alphas.pop_back();
	}

	void RenderStack::pushBlend(EBlend blend)
	{
		if(blend == EBlend::Inherit) blend = m_blends.back();
		m_blends.push_back(blend);
	}

	void RenderStack::popBlend()
	{
		m_blends.pop_back();
	}

	Matrix4 const * RenderStack::getTopMatrix4()
	{
		return this->m_mats.back().mat;
	}
	
	bool RenderStack::isTopMatrix4Dirty()
	{
		return this->m_mats.back().dirty;
	}
	
	float RenderStack::getTopAlpha()
	{
		return this->m_alphas.back();
	}
	EBlend RenderStack::getTopBlend()
	{
		return m_blends.back();
	}
	void RenderStack::reset()
	{
		this->m_alphas.clear();
		this->m_mats.clear();
		this->m_blends.clear();
	}

	RenderNode::RenderNode(Matrix4 const * mat, bool mat_dirty, float alpha, EBlend blend)
	{
		RenderStack::GetDefault()->pushMatrix(mat, mat_dirty);
		RenderStack::GetDefault()->pushAlpha(alpha);
		RenderStack::GetDefault()->pushBlend(blend);
	}
	RenderNode::~RenderNode()
	{
		RenderStack::GetDefault()->popMatrix();
		RenderStack::GetDefault()->popAlpha();
		RenderStack::GetDefault()->popBlend();
	}

	/**
	 view implament
	 */
	View::View()
	{
		m_matrix = Matrix4MakeIdentity();
	}
	View::~View()
	{

	}
	void View::setPosition(float x, float y)
	{
		m_pos.x = x;
		m_pos.y = y;
		this->setViewDirty();
	}
	void View::setX(float x)
	{
		m_pos.x = x;
		this->setViewDirty();
	}
	void View::setY(float y)
	{
		m_pos.y = y;
		this->setViewDirty();
	}
	void View::setScale(float sx, float sy)
	{
		m_scale = { sx, sy };
		this->setViewDirty();
	}
	void View::setScaleX(float sx)
	{
		m_scale.x = sx;
		this->setViewDirty();
	}
	void View::setScaleY(float sy)
	{
		m_scale.y = sy;
		this->setViewDirty();
	}
	void View::setSkew(float kx, float ky)
	{
		m_skew = { kx, ky };
		this->setViewDirty();
	}
	void View::setSkewX(float kx)
	{
		m_skew.x = kx;
		this->setViewDirty();
	}
	void View::setSkewY(float ky)
	{
		m_skew.y = ky;
		this->setViewDirty();
	}
	void View::setRotation(float radians)
	{
		m_rotation = radians;
		this->setViewDirty();
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
	bool View::isViewDirty()
	{
		return m_is_view_dirty;
	}
	void View::setViewDirty()
	{
		m_is_view_dirty = true;
	}
	void View::clearViewDirty()
	{
		if(!m_is_view_dirty) return;
		m_is_view_dirty = false;

		float cos = cosf(m_rotation);
		float sin = sinf(m_rotation);

		m_matrix.m[0] = cos * m_skew.x * m_scale.x;
		m_matrix.m[1] = sin * m_skew.y * m_scale.x;
		m_matrix.m[4] = -sin * m_skew.x * m_scale.y;
		m_matrix.m[5] = cos * m_skew.y * m_scale.y;
		m_matrix.m[12] = m_pos.x;
		m_matrix.m[13] = m_pos.y;
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

	void View::setBlend(EBlend blend)
	{
		m_blend = blend;
	}
	EBlend View::getBlend()
	{
		return m_blend;
	}
	
	void SizeView::setSize(float width, float height)
	{
		m_size = { width, height };
		this->setViewDirty();
	}
	void SizeView::setWidth(float w)
	{
		m_size.w = w;
		this->setViewDirty();
	}
	void SizeView::setHeight(float h)
	{
		m_size.h = h;
		this->setViewDirty();
	}
	float SizeView::getWidth()
	{
		return m_size.w;
	}
	float SizeView::getHeight()
	{
		return m_size.h;
	}

	void SizeView::setAnchor(float ax, float ay)
	{
		m_anchor = { ax, ay };
		this->setViewDirty();
	}
	void SizeView::setAnchorX(float ax)
	{
		m_anchor.x = ax;
		this->setViewDirty();
	}
	void SizeView::setAnchorY(float ay)
	{
		m_anchor.y = ay;
		this->setViewDirty();
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
	void SizeView::setSize(Size2 size)
	{
		m_size = size;
		this->setViewDirty();
	}
	void SizeView::setAnchor(Point2 anchor)
	{
		m_anchor = anchor;
		this->setViewDirty();
	}
	void SizeView::clearViewDirty()
	{
		if(!m_is_view_dirty) return;
		m_is_view_dirty = false;

		float cos = cosf(m_rotation);
		float sin = sinf(m_rotation);
		float x = m_size.w * m_scale.x;
		float y = m_size.h * m_scale.y;
		m_matrix.m[0] = cos * m_skew.x * x;
		m_matrix.m[1] = sin * m_skew.y * x;
		m_matrix.m[4] = -sin * m_skew.x * y;
		m_matrix.m[5] = cos * m_skew.y * y;

		m_matrix.m[12] = m_pos.x - m_matrix.m[0] * m_anchor.x - m_matrix.m[4] * m_anchor.y;
		m_matrix.m[13] = m_pos.y - m_matrix.m[1] * m_anchor.x - m_matrix.m[5] * m_anchor.y;
	}
	bool View::updateWorldMatrix()
	{
		auto rs = RenderStack::GetDefault();
		auto top_m_p = rs->getTopMatrix4();
		if(this->isViewDirty())
		{
			this->clearViewDirty();
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

}}

