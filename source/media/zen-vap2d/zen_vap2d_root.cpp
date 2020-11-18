#include "zen_vap2d_root.h"
#include "zen_vap2d_event.h"
#include "zen_app.h"
#include "zen_ticker.h"
#include "zen_log.h"
#include <vector>
#include <mutex>
#include <queue>

namespace Zen { namespace Vap2d {

	struct ActionItem
	{
		Node * node;
		std::shared_ptr<Action> action;
		Zen::Microseconds next_time;
		Zen::Microseconds time;
		bool valid = true;
	};

	class RootInner : public Root, public Zen::AppRuntimeDelegate  {
	protected:
		/**
		 @member m_removed_nodes
		 cache removed node. after update, they will be delete.
		 */
		std::vector<Node*> m_removed_nodes;
		std::deque<ActionItem*> m_running_actions;
		std::vector<ActionItem*> m_adding_actions;
		Node * m_touch_node = nullptr;


	protected:
		Matrix4 m_matrix;
		Size2 m_design_size;
		Size2 m_real_size;
		float m_view_scale = 1.f;

		bool m_is_view_dirty = false;

		Node * m_root_node = nullptr;
		std::shared_ptr<RootDelegate> m_delegate = nullptr;
		Zen::Color4f m_color;

		Zen::Microseconds m_start_time;
		Zen::Microseconds m_duration; // running duration
		Zen::Ticker m_ticker;
		std::chrono::microseconds m_pause_cut_duration__;

	public:
		void setViewDirty();

		void queueAction(ActionItem * item);

		void clearNode(Node * node);

		void removeNode(Node * node);

		void runAction(Node * node, std::shared_ptr<Action> action);

		void stopAllActions(Node * node);

		void stopAction(Node * node, std::shared_ptr<Action> action);

		void updateActions();

		RootInner() = default;

	public: // override
		virtual void run(std::shared_ptr<RootDelegate> delegate) override;
		virtual std::shared_ptr<RootDelegate> getDelegate() override;
		virtual void setViewSize(Size2 size) override;
		virtual Size2 getViewSize() override;
		virtual Size2 getRealViewSize() override;
		virtual void setBackgroundColor(Zen::Color4f color) override;
		virtual Zen::Color4f getBackgroundColor() override;
		virtual std::chrono::microseconds getRunningTime() override;
		virtual void replaceRootNode(Node * node) override;
		virtual Node * getRootNode() override;

		virtual float getViewScale() override;

		std::chrono::microseconds getTotalTime() override;
	public:
		virtual void onLaunch(Zen::Size2 view_size) override;

		virtual void onResize(Zen::Size2 view_size) override;

		virtual void onExit() override;

		virtual void onPause() override;

		virtual void onResume() override;

		virtual void onUpdate() override;

		virtual void onDraw() override;

		virtual void onBack() override;
		
		virtual void onClose() override;

		void _fixTouch(AppTouch & touch);

		virtual void onTouchDown(AppTouch touch) override;

		virtual void onTouchMove(AppTouch touch) override;

		virtual void onTouchUp(AppTouch touch) override;

		virtual void onTouchCancel(AppTouch touch) override;

		virtual void onMouseMove(AppTouch touch) override;

		virtual void onKeyDown(AppKey key) override;

		virtual void onKeyUp(AppKey key) override;
	};

}}


namespace Zen { namespace Vap2d {

	void RootInner::onLaunch(Zen::Size2 view_size)
	{
		Zen::LogV("%s", __FUNCTION__);

		m_duration = std::chrono::microseconds::zero();
		m_start_time = Zen::Now();
		m_ticker.tick();

		m_real_size = view_size;

		m_design_size = view_size;

		this->setBackgroundColor(Zen::Color4f(0x333355ff));

		if(m_delegate) m_delegate->onLaunch();
	}

	void RootInner::onResize(Zen::Size2 view_size)
	{
		Zen::LogV("%s", __FUNCTION__);

		m_real_size = view_size;
		this->setViewDirty();
		if(m_delegate) m_delegate->onResize();
	}

	void RootInner::onExit()
	{
		Zen::LogV("%s", __FUNCTION__);

		if(m_delegate) m_delegate->onExit();
	}

	void RootInner::onPause()
	{
		Zen::LogV("%s", __FUNCTION__);
		if(m_delegate) m_delegate->onPause();
		m_pause_cut_duration__ = m_ticker.tick();
	}

	void RootInner::onResume()
	{
		Zen::LogV("%s", __FUNCTION__);

		if(m_delegate) m_delegate->onResume();
		m_ticker.tick(m_pause_cut_duration__);
	}

	void RootInner::onUpdate()
	{
		auto interval = m_ticker.tick();

		m_duration += interval;

		this->updateActions();

		for(auto i : m_removed_nodes)
		{
			delete i;
		}
		m_removed_nodes.clear();

		if(m_delegate) m_delegate->onUpdate((float)Zen::ToSeconds(interval));
	}

	void RootInner::onDraw()
	{
		_StartRender();

		if(!m_root_node || !m_root_node->isVisible()) return;

		bool dirty = m_is_view_dirty;

		if(dirty)
		{
			m_is_view_dirty = false;
			m_matrix = {
				2 / m_design_size.w, 0, 0, 0,
				0, 2 / m_design_size.h, 0, 0,
				0, 0, 1, 0,
				-1, -1, 0, 1
			};
		}

		DrawStack::GuardAll rg(&m_matrix, dirty, 1.0f, eBlend::Normal);
		m_root_node->draw();

		_EndRender();
	}

	/**
	 @function onBack
	 Android back action
	 */
	void RootInner::onBack()
	{
		Zen::LogV("%s", __FUNCTION__);

		if(m_delegate) m_delegate->onBack();
	}
	/**
	 @function onBack
	 Windows <alt+f4> action
	 */
	void RootInner::onClose()
	{
		Zen::LogV("%s", __FUNCTION__);

		if(m_delegate) m_delegate->onClose();
	}

	void RootInner::_fixTouch(AppTouch & touch)
	{
		touch.x *= m_design_size.w;
		touch.y *= m_design_size.h;
//		printf("~~ %.2f %.2f\n", touch.x, touch.y);
	}
	void RootInner::onTouchDown(AppTouch touch) {
		if(!m_root_node) return;
		_fixTouch(touch);
		m_touch_node = m_root_node->onTouchDown(touch);
	}

	void RootInner::onTouchMove(AppTouch touch) {
		if(!m_touch_node) return;
		_fixTouch(touch);
		m_touch_node = m_touch_node->onTouchMove(touch);
	}

	void RootInner::onTouchUp(AppTouch touch) {
		if(!m_touch_node) return;
		_fixTouch(touch);
		m_touch_node = m_touch_node->onTouchUp(touch);
	}

	void RootInner::onTouchCancel(AppTouch touch) {
		if(!m_touch_node) return;
		_fixTouch(touch);
		m_touch_node = m_touch_node->onTouchCancel(touch);
	}

	void RootInner::onMouseMove(AppTouch touch)
	{
		Zen::LogV("[%s]not supported on this version", __FUNCTION__);
	}

	void RootInner::onKeyDown(AppKey key)
	{
		Zen::LogV("[%s]not supported on this version", __FUNCTION__);
	}

	void RootInner::onKeyUp(AppKey key)
	{
		Zen::LogV("[%s]not supported on this version", __FUNCTION__);
	}
}}

namespace Zen { namespace Vap2d {

	static std::shared_ptr<RootInner> S_root(new RootInner);

	Root * Root::S()
	{
		return S_root.get();
	}
	void RootInner::run(std::shared_ptr<RootDelegate> delegate)
	{
		m_delegate = delegate;
		Zen::App::S()->setRuntimeDelegate(S_root);
	}

	void RootInner::setBackgroundColor(Zen::Color4f color)
	{
		m_color = color;
		_SetClearColor(color);
	}

	Zen::Color4f RootInner::getBackgroundColor()
	{
		return m_color;
	}
	void RootInner::setViewSize(Size2 size)
	{
		m_design_size = size;
		this->setViewDirty();
	}

	Size2 RootInner::getViewSize()
	{
		return m_design_size;
	}
	Size2 RootInner::getRealViewSize()
	{
		return m_real_size;
	}
	float RootInner::getViewScale()
	{
		return m_view_scale;
	}
	
	void RootInner::replaceRootNode(Node *node)
	{
		if(m_root_node) m_root_node->remove();
		m_root_node = node;
	}
	Node * RootInner::getRootNode()
	{
		return m_root_node;
	}
	std::chrono::microseconds RootInner::getRunningTime()
	{
		return m_duration;
	}
	std::chrono::microseconds RootInner::getTotalTime()
	{
		return Now() - m_start_time;
	}

	std::shared_ptr<RootDelegate> RootInner::getDelegate()
	{
		return m_delegate;
	}

	// node operation

	void RootInner::setViewDirty()
	{
		m_is_view_dirty = true;
		m_view_scale = m_design_size.w / m_real_size.w;
	}
	void RootInner::queueAction(ActionItem * item)
	{
		for(auto iter = m_running_actions.begin(); iter != m_running_actions.end(); ++iter)
		{
			if((*iter)->next_time >= item->next_time)
			{
				m_running_actions.insert(iter, item);
				return;
			}
		}
		m_running_actions.insert(m_running_actions.end(), item);
	}

	void RootInner::clearNode(Node * node)
	{
		if(m_touch_node == node) m_touch_node = nullptr;
		this->stopAllActions(node);
	}
	void RootInner::removeNode(Node * node)
	{
		m_removed_nodes.push_back(node);
	}

	void RootInner::runAction(Node * node, std::shared_ptr<Action> action)
	{
		auto i = new ActionItem;
		i->node = node;
		i->action = action;
		m_adding_actions.push_back(i);
	}

	void RootInner::stopAllActions(Node * node)
	{
		for(auto i : m_running_actions)
		{
			if(i->node == node) i->valid = false;
		}
		for(auto i : m_adding_actions)
		{
			if(i->node == node) i->valid = false;
		}
	}

	void RootInner::stopAction(Node * node, std::shared_ptr<Action> action)
	{
		for(auto i : m_running_actions)
		{
			if(i->node == node && i->action == action)
			{
				i->valid = false;
				break;
			}
		}
		for(auto & i : m_adding_actions)
		{
			if(i->node == node && i->action == action)
			{
				i->valid = false;
				break;
			}
		}
	}

	void RootInner::updateActions()
	{
		while(m_running_actions.size())
		{
			auto i = m_running_actions.front();
			if(i->next_time > m_duration) break;

			if(i->valid)
			{
				float interval = Zen::ToSeconds(m_duration - i->time);
				auto delay = i->action->run(interval);

				/**
				 here check valid again, in action->run() the stopAction() function maybe be called.
				 */
				if(delay >= 0 && i->valid) // running
				{
					auto forward = Zen::ToMicroseconds(delay);
					i->time = m_duration;
					i->next_time = m_duration + (forward.count()>0?forward:Zen::Microseconds(1));

					m_running_actions.pop_front();
					queueAction(i);
					continue;
				}
			}
			m_running_actions.pop_front();
			delete i;
		}

		for(auto i : m_adding_actions)
		{
			if(i->valid)
			{
				auto delay = i->action->begin();
				if(delay >= 0 && i->valid)
				{
					auto forward = Zen::ToMicroseconds(delay);

					i->time = m_duration;
					i->next_time = m_duration + (forward.count()>0?forward:Zen::Microseconds(1));

					queueAction(i);
					continue;
				}
			}
			delete i;
		}
		m_adding_actions.clear();
	}

}}

/**
 @ref part of node
 */

namespace Zen { namespace Vap2d {
	Node::~Node()
	{
		S_root->clearNode(this);
	}
	BranchNode::~BranchNode()
	{
		for(auto i : m_nodes) delete i;
	}
	FinalNode::~FinalNode()
	{
	}

	void Node::remove()
	{
		if(m_upper)
		{
			for(auto i = m_upper->m_nodes.begin(); i != m_upper->m_nodes.end(); ++i)
			{
				if(*i == this)
				{
					m_upper->m_nodes.erase(i);
					break;
				}
			}
			m_upper = nullptr;
		}
		S_root->removeNode(this);
	}

	void Node::runAction(std::shared_ptr<Action> action)
	{
		S_root->runAction(this, action);
	}
	void Node::stopAction(std::shared_ptr<Action> action)
	{
		S_root->stopAction(this, action);
	}
	void Node::stopAllActions()
	{
		S_root->stopAllActions(this);
	}
}}
