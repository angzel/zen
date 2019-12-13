#pragma once

#include "zen_gles2.h"
#include "zen_matrix.h"
#include "zen_app_runtime.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {
	class VapObject
	{
	public:
		VapObject(std::string const & name = "object");

		virtual ~VapObject();

		std::string const & getVapName() { return m_vap_name; }
	protected:
		std::string m_vap_name;
		VapObject(VapObject&) = delete;
	};

	/**
	 @class BNode
	 a Branch node.
	 can add sub nodes.
	 */
	class BNode;

	/**
	 @class LNode
	 a Leaf Node.
	 this is an end node without sub nodes.
	 */
	class LNode;

	/**
	 @ref zen_vap2d_event.h
	 */
	class Action;

	class Node : public VapObject
	{
		friend class LNode;
		friend class BNode;
	public:

		/**
		 @function setOrder
		 - drawing order, just compare with brother-nodes.
		 - less first draw. so larger is in front.
		 */
		virtual void setOrder(int index);

		/**
		 if not visible, the draw will not be called.
		 */
		virtual void setVisible(bool);

		virtual int getOrder() const;

		virtual bool isVisible() const;

		/**
		 @function draw
		 - render the node. generally this function will be called from Root automatically.
		 */
		virtual void draw() {}
		/**
		 @function remove
		 remove this node from tree.
		 - if the node has already add to other upper <BNode>,  dont <delete> it anymore,
		 you can remove() this node, or remove the upper node, it will be deleted automatically after current Update frame.
		 - or please <delete> if <new>ed.
		 */
		virtual void remove();

		BNode * getUpperNode();
	public:
		virtual Node * onTouchDown(AppTouch const & touch) { return nullptr; }

		virtual Node * onTouchMove(AppTouch const & touch) { return this; }

		virtual Node * onTouchUp(AppTouch const & touch) { return this; }

		virtual Node * onTouchCancel(AppTouch const & touch) { return this; }

	public:
		void runAction(std::shared_ptr<Action> action);
		/**
		 @function stopAction
		 remove action is not required, if the node removed or action end, it will be auto removed
		 */
		void stopAction(std::shared_ptr<Action> action);

		void stopAllActions();
	public:
		Node(std::string const & name = "node");

		virtual ~Node();
	protected:

		BNode * m_upper = nullptr;
		int m_order = 0;
		bool m_is_visible = true;
	};

	class BNode : public Node
	{
		friend class LNode;
		friend class Node;
	protected:
		std::vector<Node*> m_nodes;
		bool m_nodes_dirty = false;

	public:
		BNode(std::string const & name = "branch node");

		virtual ~BNode();

		virtual void draw() override;

		virtual void addNode(Node * node);

		virtual void removeAllNodes();

		virtual void setNodesDirty();

		virtual void clearNodesDirty();

	public:
		virtual Node * onTouchDown(AppTouch const & touch) override;

		virtual Node * onTouchMove(AppTouch const & touch) override;

		virtual Node * onTouchUp(AppTouch const & touch) override;

		virtual Node * onTouchCancel(AppTouch const & touch) override;
	};

	class LNode : public Node
	{
		friend class BNode;
		friend class Node;
	public:
		LNode(std::string const & name = "leaf node");
		~LNode();
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

	class RenderStackGuard
	{
	public:
		RenderStackGuard(Matrix4 const * mat, bool mat_dirty, float alpha, EBlend blend);
		~RenderStackGuard();
	};

	class View {
	public:
		View();
		virtual ~View();
		virtual void setPosition(float x, float y);
		virtual void setX(float x);
		virtual void setY(float y);
		virtual void setScale(float sx, float sy);
		virtual void setScaleX(float sx);
		virtual void setScaleY(float sy);
		virtual void setSkew(float kx, float ky);
		virtual void setSkewX(float kx);
		virtual void setSkewY(float ky);
		virtual void setRotation(float radians);
		virtual Point2 getPosition();
		virtual void setPosition(Point2 pos);
		virtual float getX();
		virtual float getY();
		virtual Point2 getScale();
		virtual void setScale(Point2 scale);
		virtual float getScaleX();
		virtual float getScaleY();
		virtual Point2 getSkew();
		virtual void setSkew(Point2 skew);
		virtual float getSkewX();
		virtual float getSkewY();
		virtual float getRotation();
		virtual void setBlend(EBlend blend);
		virtual EBlend getBlend();
		virtual void setAlpha(float alpha);
		virtual float getAlpha();
	public:
		virtual bool isViewDirty();
		virtual void setViewDirty();
		virtual void clearViewDirty();
		virtual bool updateWorldMatrix();
		
	protected:
		Matrix4 m_world_matrix;
		Matrix4 m_matrix;
		bool m_is_view_dirty = true;
		Point2 m_pos = {0, 0};
		Point2 m_scale = {1, 1};
		Point2 m_skew = {1, 1};
		float m_rotation = 0;
		EBlend m_blend = EBlend::Inherit;
		float m_alpha = 1.0f;
	};
	
	class SizeView : public View {
	public:
		virtual void setAnchor(float ax, float ay);
		virtual void setAnchorX(float ax);
		virtual void setAnchorY(float ay);
		virtual void setSize(float width, float height);
		virtual void setWidth(float w);
		virtual void setHeight(float h);
		virtual Size2 getSize();
		virtual float getWidth();
		virtual float getHeight();
		virtual Point2 getAnchor();
		virtual float getAnchorX();
		virtual float getAnchorY();
		virtual void setSize(Size2 size);
		virtual void setAnchor(Point2 anchor);

	public:
		virtual void clearViewDirty() override;

	protected:
		Size2 m_size = { 0, 0 };
		Point2 m_anchor = { 0, 0 };
	};
}}
