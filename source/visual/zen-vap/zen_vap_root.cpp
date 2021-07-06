#include "zen_vap_root.h"
#include "zen_vap_action.h"
#include "zen_app.h"
#include "zen_ticker.h"
#include "zen_log.h"
#include <vector>
#include <mutex>
#include <queue>
#include <map>

namespace Zen { namespace Vap {

	struct ActionItem
	{
		Node * node;
		std::shared_ptr<Action> action;
		double next_time;
		double time;
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

		std::map<uint32_t/*AppTouch::no*/, Node*> m_touch_nodes;

	protected:
		Size2 m_real_size;

		Node * m_root_node = nullptr;
		std::shared_ptr<RootDelegate> m_delegate = nullptr;
		Zen::Color4f m_color;

		double m_running_duration; // running duration
		std::chrono::microseconds m_start_time;
	public:

		void queueAction(ActionItem * item);

		void deleteNodeAndClean(Node * node);

		void runAction(Node * node, std::shared_ptr<Action> action);

		void stopAllActions(Node * node);

		void stopAction(Node * node, std::shared_ptr<Action> action);

		void updateActions();

		RootInner() = default;

#define s_root RootInner::Get()
		static std::shared_ptr<RootInner> Get();
	public: // override
		virtual void run(std::shared_ptr<RootDelegate> delegate) override;
		virtual std::shared_ptr<RootDelegate> getDelegate() override;
		virtual Size2 getRealViewSize() override;
		virtual void setBackgroundColor(Zen::Color4f color) override;
		virtual Zen::Color4f getBackgroundColor() override;
		virtual double getRunningTime() override;
		virtual void replaceRootNode(Node * node) override;
		virtual Node * getRootNode() override;

		virtual double getTotalTime() override;
	public:
		virtual void onLaunch(Zen::Size2 view_size) override;

		virtual void onResize(Zen::Size2 view_size) override;

		virtual void onExit() override;

		virtual void onPause() override;

		virtual void onResume() override;

		virtual void onUpdate() override;

		virtual void onDraw() override;

		void _fixTouch(AppTouch & touch);

		virtual void onTouchDown(AppTouch touch) override;

		virtual void onTouchMove(AppTouch touch) override;

		virtual void onTouchUp(AppTouch touch) override;

		virtual void onTouchCancel(AppTouch touch) override;
		
		void clean();
	};

}}


namespace Zen { namespace Vap {

	void RootInner::onLaunch(Zen::Size2 view_size)
	{
		Zen::LogV("%s", __FUNCTION__);
		this->setBackgroundColor(Zen::Color4f(0x333355ff));

		m_real_size = view_size;
		
		m_running_duration = 0.0; //std::chrono::microseconds::zero();
		
		if(m_delegate) m_delegate->onLaunch();
		
	}

	void RootInner::onResize(Zen::Size2 view_size)
	{
		Zen::LogV("%s", __FUNCTION__);

		m_real_size = view_size;
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
	}

	void RootInner::onResume()
	{
		Zen::LogV("%s", __FUNCTION__);

		if(m_delegate) m_delegate->onResume();
		
		std::cout << m_touch_nodes.size() << std::endl;
	}

	void RootInner::clean()
	{
		for(auto i : m_removed_nodes)
		{
			for(auto iter = m_touch_nodes.begin(); iter != m_touch_nodes.end(); ++iter)
			{
				if(iter->second == i) iter->second = nullptr;
			}
			delete i;
		}
		m_removed_nodes.clear();
	}
	void RootInner::onUpdate()
	{
		auto interval = 1.f / App::Get()->getFramesPerSecond(); // m_ticker.tick();

		m_running_duration += interval;

		this->updateActions();

		if(m_delegate) m_delegate->onUpdate((float)interval);
		
		this->clean();
	}

	void RootInner::onDraw()
	{
		GRenderGuard r;
		
		if(!m_root_node || !m_root_node->isVisible()) return;

		m_root_node->draw();
		
		this->clean();
	}

	void RootInner::_fixTouch(AppTouch & touch)
	{
//		printf("~~ %.2f %.2f\n", touch.x, touch.y);
	}
	void RootInner::onTouchDown(AppTouch touch) {
		if(!m_root_node) return;
		
		_fixTouch(touch);
		auto node = m_root_node->onTouchDown(touch);
		if(node) {
			m_touch_nodes[touch.no] = node;
		}
		
		this->clean();
 	}

	void RootInner::onTouchMove(AppTouch touch) {
		auto iter = m_touch_nodes.find(touch.no);
		if(iter == m_touch_nodes.end()) return;
		
		_fixTouch(touch);
		
		if(iter->second) iter->second = iter->second->onTouchMove(touch);
		
		if(!iter->second) m_touch_nodes.erase(iter);
		
		this->clean();
	}

	void RootInner::onTouchUp(AppTouch touch) {
		auto iter = m_touch_nodes.find(touch.no);
		if(iter == m_touch_nodes.end()) return;
		
		_fixTouch(touch);
		if(iter->second) iter->second->onTouchUp(touch);
		m_touch_nodes.erase(iter);
		
		this->clean();
	}

	void RootInner::onTouchCancel(AppTouch touch) {
		auto iter = m_touch_nodes.find(touch.no);
		if(iter == m_touch_nodes.end()) return;
		
		_fixTouch(touch);
		if(iter->second) iter->second->onTouchCancel(touch);
		m_touch_nodes.erase(iter);
		
		this->clean();
	}
}}

namespace Zen { namespace Vap {

		std::shared_ptr<RootInner> RootInner::Get()
		{
			static std::shared_ptr<RootInner> root(new RootInner);
			return root;
		}
	Root * Root::Get()
	{
			return RootInner::Get().get();
	}
	void RootInner::run(std::shared_ptr<RootDelegate> delegate)
	{
		m_delegate = delegate;
		Zen::App::Get()->setRuntimeDelegate(s_root);
	}

	void RootInner::setBackgroundColor(Zen::Color4f color)
	{
		m_color = color;
		Helper::SetClearColor(color);
	}

	Zen::Color4f RootInner::getBackgroundColor()
	{
		return m_color;
	}
	
	Size2 RootInner::getRealViewSize()
	{
		return m_real_size;
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
	double RootInner::getRunningTime()
	{
		return m_running_duration;
	}
	double RootInner::getTotalTime()
	{
		return Zen::ToSeconds(Now() - m_start_time);
	}
	std::shared_ptr<RootDelegate> RootInner::getDelegate()
	{
		return m_delegate;
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

//	void RootInner::clearNode(Node * node)
//	{
//		this->stopAllActions(node);
//	}
	void RootInner::deleteNodeAndClean(Node * node)
	{
		stopAllActions(node);
		
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
			if(i->next_time > m_running_duration) break;
			
			if(i->valid)
			{
				auto interval = m_running_duration - i->time;
				
				auto delay = i->action->run((float)interval);

				/**
				 here check valid again, in action->run() the stopAction() function maybe be called.
				 */
				if(delay < 0) i->valid = false;
				
				if(i->valid) // running
				{
					i->time = m_running_duration;
					i->next_time = m_running_duration + std::max((double)delay, 1/120.0);

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
				auto delay = i->action->start();
				if(delay >= 0 && i->valid)
				{
					i->time = m_running_duration;
					i->next_time = m_running_duration + delay;

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

namespace Zen { namespace Vap {

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
		this->removeAllNodes();
		
		s_root->deleteNodeAndClean(this);
	}

	void Node::runAction(std::shared_ptr<Action> action)
	{
		s_root->runAction(this, action);
	}
	void Node::stopAction(std::shared_ptr<Action> action)
	{
		s_root->stopAction(this, action);
	}
	void Node::stopAllActions()
	{
		s_root->stopAllActions(this);
	}
}}
