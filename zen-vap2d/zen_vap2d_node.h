#pragma once

#include "zen_vap2d_config.h"
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
	 @class BranchNode
	 a Branch node.
	 can add sub nodes.
	 */
	class BranchNode;

	/**
	 @class FinalNode
	 a Leaf Node.
	 this is an end node without sub nodes.
	 */
	class FinalNode;
	
	/**
	 @ref zen_vap2d_event.h
	 */
	class Action;

	class Node : virtual public VapObject
	{
		friend class FinalNode;
		friend class BranchNode;
	public:
		/**
		 @function draw
		 - render the node. generally this function will be called from Root automatically.
		 */
		virtual void draw() {}

		virtual Node * onTouchDown(AppTouch const & touch) { return nullptr; }

		virtual Node * onTouchMove(AppTouch const & touch) { return this; }

		virtual Node * onTouchUp(AppTouch const & touch) { return this; }

		virtual Node * onTouchCancel(AppTouch const & touch) { return this; }
	public:
		/**
		 @function setOrder
		 - drawing order, just compare with brother-nodes.
		 - less first draw. so larger is in front.
		 */
		void setOrder(int index);

		/**
		 if not visible, the draw will not be called.
		 */
		void setVisible(bool);

		int getOrder() const;

		bool isVisible() const;

		/**
		 @function remove
		 remove this node from tree.
		 - if the node has already add to other upper <BranchNode>,  dont <delete> it anymore,
		 you can remove() this node, or remove the upper node, it will be deleted automatically after current Update frame.
		 - or please <delete> if <new>ed.
		 */
		void remove();

		BranchNode * getUpperNode();

	public:
		void runAction(std::shared_ptr<Action> action);
		/**
		 @function stopAction
		 remove action is not required, if the node removed or action end, it will be auto removed
		 */
		void stopAction(std::shared_ptr<Action> action);

		void stopAllActions();

	public:
		void setBlend(eBlend blend);

		eBlend getBlend();

		void setAlpha(float alpha);
		
		float getAlpha();
	public:
		Node(std::string const & name = "node");

		virtual ~Node();
	protected:

		BranchNode * m_upper = nullptr;
		int m_order = 0;
		bool m_is_visible = true;

		eBlend m_blend = eBlend::Inherit;
		float m_alpha = 1.0f;

	};

	class BranchNode : public Node
	{
		friend class FinalNode;
		friend class Node;
	protected:
		std::vector<Node*> m_nodes;
		bool m_nodes_dirty = false;

	public:
		BranchNode(std::string const & name = "branch node");

		virtual ~BranchNode();

		void addNode(Node * node);

		void removeAllNodes();

		void setNodesDirty();

		void clearNodesDirty();

	public:

		virtual void draw() override;
		virtual Node * onTouchDown(AppTouch const & touch) override;

		virtual Node * onTouchMove(AppTouch const & touch) override;

		virtual Node * onTouchUp(AppTouch const & touch) override;

		virtual Node * onTouchCancel(AppTouch const & touch) override;
	};

	class FinalNode : public Node
	{
		friend class BranchNode;
		friend class Node;
	public:
		FinalNode(std::string const & name = "leaf node");
		virtual ~FinalNode();
	};

	class Matrixing : public virtual VapObject {
	public:
		Matrixing() = default;
		virtual ~Matrixing() = default;

		bool updateMatrix();
	protected:
		virtual void _clearInnerMatrixDirty() = 0;

		Matrix4 m_world_matrix;
		Matrix4 m_matrix = Matrix4MakeIdentity();
		bool m_is_matrix_dirty = true;
	};

	class View : public Matrixing {

	public:
		virtual void _clearInnerMatrixDirty() override;
	public:
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

	protected:
		Point2 m_pos = {0, 0};
		Point2 m_scale = {1, 1};
		Point2 m_skew = {1, 1};
		float m_rotation = 0;	};
	
	class SizeView : public View {
	public:
		virtual void _clearInnerMatrixDirty() override;

	public:
		void setAnchor(float ax, float ay);
		void setAnchorX(float ax);
		void setAnchorY(float ay);
		Point2 getAnchor();
		float getAnchorX();
		float getAnchorY();
		void setAnchor(Point2 anchor);


	protected:
		void _setScale2(float x, float y);

		Size2 m_scale2 = { 1, 1 };
		Point2 m_anchor = { 0, 0 };
	};

	class Colorful : public virtual VapObject
	{
	public:
		Colorful() = default;
		virtual ~Colorful() = default;

		void setColor(Zen::Color4f color);
		Zen::Color4f getColor();

		void setGreyMode(bool);
		bool isGreyMode();
	protected:
		Zen::Color4f m_color = Zen::eColor::White;
		bool m_is_grey_mode = false;
		bool m_is_color_dirty = true;
	};
	class Flipable : public virtual VapObject
	{
	public:
		Flipable() = default;
		virtual ~Flipable() = default;

		void setFlipX(bool x);

		void setFlipY(bool y);

		bool isFlipX();

		bool isFlipY();
	protected:
		bool m_is_flip_x = false;
		bool m_is_flip_y = false;
		bool m_is_flip_dirty = true;
	};

	class DrawStack
	{
	public:
		static DrawStack * S();

		void reset();

		Matrix4 const * getTopMatrix4();

		bool isTopMatrix4Dirty();

		float getTopAlpha();

		eBlend getTopBlend();

	public:

		struct GuardAll
		{
			GuardAll(Matrix4 const * mat, bool mat_dirty, float alpha, eBlend blend)
			{
				S()->pushMatrix(mat, mat_dirty);
				S()->pushAlpha(alpha);
				S()->pushBlend(blend);
			}
			~GuardAll()
			{
				S()->popMatrix();
				S()->popAlpha();
				S()->popBlend();
			}

		};

		struct GuardMatrix
		{
			GuardMatrix(Matrix4 const * mat, bool mat_dirty)
			{
				S()->pushMatrix(mat, mat_dirty);
			}
			~GuardMatrix()
			{
				S()->popMatrix();
			}
		};

		struct GuardAlpha
		{
			GuardAlpha(float alpha)
			{
				S()->pushAlpha(alpha);
			}
			~GuardAlpha()
			{
				S()->popAlpha();
			}
		};

		struct GuardBlend
		{
			GuardBlend(eBlend blend)
			{
				S()->pushBlend(blend);
			}
			~GuardBlend()
			{
				S()->popBlend();
			}
		};

	protected:
		struct MatrixInfo {
			Matrix4 const * mat;
			bool dirty;
		};
		eBlend m_performed_blend;

		std::vector<MatrixInfo> m_mats;
		std::vector<float> m_alphas;
		std::vector<eBlend> m_blends;

	protected:
		DrawStack();
		virtual ~DrawStack();

		void pushMatrix(Matrix4 const * mat, bool dirty);

		void popMatrix();

		void pushAlpha(float alpha);

		void popAlpha();

		void pushBlend(eBlend);

		void popBlend();
	};
}}
