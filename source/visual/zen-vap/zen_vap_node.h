#pragma once

#include "zen_vap_render.h"
#include "zen_matrix.h"
#include "zen_app.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {
	
	class VapObject
	{
	public:
		VapObject(std::string const & name = "object");

		virtual ~VapObject();

		void setObjectName(std::string const & name);
		
		std::string const & getObjectName() { return m_object_name; }
	protected:
		std::string m_object_name;
		VapObject(VapObject&) = delete;
	};
	
	/**
	 @ref zen_vap_action.h
	 */
	class Action;

	class Node : public VapObject
	{
	public:
		/**
		 @function draw
		 - render the node. generally this function will be called from Root automatically.
		 */
		virtual void draw();

		virtual Node * onTouchDown(AppTouch const & touch);

		virtual Node * onTouchMove(AppTouch const & touch);

		virtual Node * onTouchUp(AppTouch const & touch);

		virtual Node * onTouchCancel(AppTouch const & touch);
		
	public:
		Node(std::string const & name = "node");

		virtual ~Node();
	public:
		/**
		 if not visible, the draw will not be called.
		 */
		void setVisible(bool);

		bool isVisible() const;

	public: // as a child
		/**
		 @function remove
		 remove this node from tree.
		 - if the node has already add to other upper <BNode>,  dont <delete> it anymore,
		 you can remove() this node, or remove the upper node, it will be deleted automatically after current Update frame.
		 - or please <delete> if <new>ed.
		 */
		void remove();

		/**
		 @function setOrder
		 - drawing order, just compare with brother-nodes.
		 - less first draw. so larger is in front.
		 */
		void setOrder(int index);

		int getOrder() const;
		
		Node * getUpperNode();
	public: // as a upper
		void addNode(Node * node);

		void removeAllNodes();

		void setNodesOrderDirty();

		void clearNodesOrder();

	public: // action.
		void runAction(std::shared_ptr<Action> action);
		/**
		 @function stopAction
		 @note if the node removed, all actions will stop.
		*/
		void stopAction(std::shared_ptr<Action> action);

		void stopAllActions();

	public:
		void setBlend(eBlend blend);

		eBlend getBlend();

		void setAlpha(float alpha);
		
		float getAlpha();
	protected:

		Node * m_upper = nullptr;
		int m_order = 0;
		bool m_is_visible = true;

		eBlend m_blend = eBlend::Normal;
		float m_alpha = 1.0f;
	protected:
		
		std::vector<Node*> m_nodes;
		bool m_nodes_dirty = false;
	};
//
//	class BNode : public Node
//	{
//		friend class Node;
//	protected:
//
//		std::vector<Node*> m_nodes;
//		bool m_nodes_dirty = false;
//
//	public:
//
//		BNode(std::string const & name = "branch node");
//
//		virtual ~BNode();
//		
//		void onRemoved() override;
//
//		void addNode(Node * node);
//
//		void removeAllNodes();
//
//		void setNodesOrderDirty();
//
//		void clearNodesOrder();
//
//	public:
//
//		virtual void draw() override;
//
//		virtual Node * onTouchDown(AppTouch const & touch) override;
//
//		virtual Node * onTouchMove(AppTouch const & touch) override;
//
//		virtual Node * onTouchUp(AppTouch const & touch) override;
//
//		virtual Node * onTouchCancel(AppTouch const & touch) override;
//	};
typedef Node BNode;
	class Node2d : public Node
	{
	public:
		Node2d(std::string const & name = "leaf node");
	};

	class Matrixing {
	public:
		Matrixing() = default;
		
		virtual ~Matrixing() = default;
		
		Matrix4 const & getMatrix() const { return m_final_matrix; }
		
		virtual int getMatrixVersion() const { return m_matrix_version; }
		
		void setMatrix(Matrix4 const & matrix, int upper_version);
		
		bool isUpperMatrixExpired(int upper_version);
		
		virtual void updateMatrix() {};
	protected:
		Matrix4 m_final_matrix;
		int m_matrix_version = 0;
		int m_upper_matrix_version = 0;
	};

	class View2d : public Matrixing {
	public:
		View2d(bool is_anchor_valid = true);
		
		void updateMatrix() override;
	public:
		void setPosition(float x, float y);
		void setX(float x);
		void setY(float y);
		void setPosition(Point2 pos);
		
		void setScale(float sx, float sy);
		void setScaleX(float sx);
		void setScaleY(float sy);
		void setScale(Point2 scale);
		
		void setSkew(float kx, float ky);
		void setSkewX(float kx);
		void setSkewY(float ky);
		void setSkew(Point2 skew);
		
		void setRotation(float radians);
		
		Point2 getPosition();
		float getX();
		float getY();
		
		Point2 getScale();
		float getScaleX();
		float getScaleY();
		
		Point2 getSkew();
		float getSkewX();
		float getSkewY();
		
		float getRotation();

	protected:
		Point2 m_pos = { 0, 0 };
		Point2 m_scale = { 1, 1 };
		Point2 m_skew = { 1, 1 };
		Point2 m_scale2 = { 1, 1 };
		Point2 m_anchor = { 0, 0 };
		float m_rotation = 0;
		Matrix4 m_matrix0 = Matrix4MakeIdentity();
		bool m_is_matrix_dirty = true;
		bool m_is_anchor_valid = true;
		
	public:
		void setAnchor(float ax, float ay);
		void setAnchorX(float ax);
		void setAnchorY(float ay);
		Point2 getAnchor();
		float getAnchorX();
		float getAnchorY();
		void setAnchor(Point2 anchor);

	protected:
		void setScale2_(float x, float y);
	};

	class Colorful
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
	
	class Flipable 
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
		static DrawStack * Get();

		void reset();

		Matrix4 const * getTopMatrix4();

		int getTopMatrix4Version();

		float getTopAlpha();

		eBlend getTopBlend();

	protected:
		struct MatrixInfo {
			Matrix4 const * mat;
			int version;
		};
//		eBlend m_performed_blend;

		std::vector<MatrixInfo> m_mats;
		std::vector<float> m_alphas;
		std::vector<eBlend> m_blends;

	protected:
		DrawStack();
		virtual ~DrawStack();

		void pushMatrix(Matrix4 const * mat, int version);

		void popMatrix();

		void pushAlpha(float alpha);

		void popAlpha();

		void pushBlend(eBlend);

		void popBlend();
		
	public:

		struct GuardAll
		{
			GuardAll(Matrix4 const * mat, int version, float alpha, eBlend blend)
			{
				Get()->pushMatrix(mat, version);
				Get()->pushAlpha(alpha);
				Get()->pushBlend(blend);
			}
			~GuardAll()
			{
				Get()->popMatrix();
				Get()->popAlpha();
				Get()->popBlend();
			}

		};

		struct GuardMatrix
		{
			GuardMatrix(Matrix4 const * mat, int version)
			{
				Get()->pushMatrix(mat, version);
			}
			~GuardMatrix()
			{
				Get()->popMatrix();
			}
		};

		struct GuardAlpha
		{
			GuardAlpha(float alpha)
			{
				Get()->pushAlpha(alpha);
			}
			~GuardAlpha()
			{
				Get()->popAlpha();
			}
		};

		struct GuardBlend
		{
			GuardBlend(eBlend blend)
			{
				Get()->pushBlend(blend);
			}
			~GuardBlend()
			{
				Get()->popBlend();
			}
		};
	};
}}
