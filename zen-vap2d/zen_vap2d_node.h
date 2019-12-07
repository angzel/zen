#pragma once

#include "zen_gles2.h"
#include "zen_matrix.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {
	/**
	@class BNode
	Branch node
	 */
	class BNode;
	/**
	Leaf node.
	*/
	class LNode;

	class Node
	{
		friend class LNode;
		friend class BNode;
	public:
		virtual void setAlpha(float alpha);
		virtual void setOrder(int index);

		virtual float getAlpha() const { return m_alpha; }
		virtual int getOrder() const { return m_order; }
		
		virtual void draw() = 0;
		/**
			@function remove
			remove this node from tree. and delete
		 */
		virtual void remove();
		virtual ~Node() = default;
	protected:
		float m_alpha = 1.0f;
		int m_order = 0;
	protected:
		BNode * m_parent = nullptr;
	};

	class BNode : public Node
	{
		friend class LNode;
		friend class Node;
	protected:
		std::vector<Node*> m_nodes;
		bool m_nodes_dirty = false;

		void _remove_node(Node * node);
	public:
		virtual void draw() override;
		virtual void addNode(Node * node);
		virtual void removeAllNodes();
		virtual void setNodesDirty();
		virtual ~BNode();
	};

	class LNode : public Node
	{
		friend class BNode;
		friend class Node;
	public:
	};

	class RNode : public BNode
	{
	protected:
		Matrix4 m_matrix;
		Size2 m_design_view_size;
		bool m_is_view_dirty = false;
	public:
		static RNode * GetDefault();

		void setDesignViewSize(float width, float height);

		Size2 getDesignViewSize();

		virtual void draw() override;

	protected:
		virtual void remove() override;
	};

	enum class EBlend {
		Inherit,
		Normal,
		Add,
		Reverse,
	};
	

	class RenderStack
	{
	protected:
		struct MatrixInfo {
			Matrix4 const * mat;
			bool dirty;
		};
		EBlend m_performed_blend;
		
		std::vector<MatrixInfo> m_mats;
		std::vector<float> m_alphas;
		std::vector<EBlend> m_blends;

		RenderStack();
	public:
		static RenderStack * GetDefault();

		void reset();

		void pushMatrix(Matrix4 const * mat, bool dirty);

		void popMatrix();

		void pushAlpha(float a);

		void popAlpha();

		void pushBlend(EBlend blend);

		void popBlend();

		Matrix4 const * getTopMatrix4();

		bool isTopMatrix4Dirty();

		float getTopAlpha();

		EBlend getTopBlend();

		void performBlend(EBlend blend);
	};

	class RenderNode
	{
	public:
		RenderNode(Matrix4 const * mat, bool mat_dirty, float alpha, EBlend blend);
		~RenderNode();
	};

	class View {
	protected:
		Matrix4 m_world_matrix;
		Matrix4 m_matrix;
		bool m_is_view_dirty = true;
		Point2 m_pos = {0, 0};
		Point2 m_scale = {1, 1};
		Point2 m_skew = {1, 1};
		float m_rotation = 0;
		EBlend m_blend = EBlend::Inherit;
	public:
		View();
		virtual ~View();
		void setPosition(float x, float y);
		void setX(float x);
		void setY(float y);
		void setScale(float sx, float sy);
		void setScaleX(float sx);
		void setScaleY(float sy);
		void setSkew(float kx, float ky);
		void setSkewX(float kx);
		void setSkewY(float ky);
		void setRotation(float radians);
		Point2 getPosition();
		void setPosition(Point2 pos);
		float getX();
		float getY();
		Point2 getScale();
		void setScale(Point2 scale);
		float getScaleX();
		float getScaleY();
		Point2 getSkew();
		void setSkew(Point2 skew);
		float getSkewX();
		float getSkewY();
		float getRotation();
		void setBlend(EBlend blend);
		EBlend getBlend();
	public:
		virtual bool isViewDirty();
		virtual void setViewDirty();
		virtual void clearViewDirty();
		bool updateWorldMatrix();
	};
	
	class SizeView : public View {
	protected:
		Size2 m_size = {1, 1};
		Point2 m_anchor = { 0, 0};
	public:
		void setAnchor(float ax, float ay);
		void setAnchorX(float ax);
		void setAnchorY(float ay);
		void setSize(float width, float height);
		void setWidth(float w);
		void setHeight(float h);
		Size2 getSize();
		float getWidth();
		float getHeight();
		Point2 getAnchor();
		float getAnchorX();
		float getAnchorY();
		void setSize(Size2 size);
		void setAnchor(Point2 anchor);

	public:
		virtual void clearViewDirty() override;
	};
}}
