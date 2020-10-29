
#include "zen_view_gl_director.h"
#include "zen_view_gl_layer.h"
#include "zen_view_gl_schedule.h"
#include "zen_gl.h"
#include "zen_ticker.h"

namespace Zen { namespace Vap2d {
	GLDirector::GLDirector()
	{
		mRootLayer = new GLLayer();
		mRootLayer->setBlendEffect(BlendEffect::Normal);
	}
	void GLDirector::setDesignSize(Zen::Pt2 size)
	{
		mDesignSize = size;
		mIsSizeDirty = true;
	}
	Zen::Pt2 GLDirector::getDesignSize()
	{
		return mDesignSize;
	}
	void GLDirector::_setRealScreenSize(Zen::Pt2 size)
	{
		mScreenSize = size;
		mIsSizeDirty = true;
	}
	Zen::Pt2 GLDirector::getRealScreenSize()
	{
		return mScreenSize;
	}


	void GLDirector::update(double interval)
	{
		{ // check real UPS
			static Zen::HitCountTicker hits;
			hits.hit();
			auto time = Zen::ToSeconds(hits.getDuration());

			if(time > 10)
			{
				Zen::LogV("UPS:%f", (double)hits.getCount() / time);
				hits.reset();
			}
		}

		if(mIsSizeDirty)
		{
			this->_updateVap2dSize();
			mIsSizeDirty = false;
		}
		mRunningTime += interval;
		this->_updateSchedules();
	}
	void GLDirector::_updateSchedules()
	{
		auto sm = GLScheduleManager::GetDefault();
		while(sm->top())
		{
			auto s = sm->top();
			if(s->repeat == 0)
			{
				sm->pop();
				sm->removeSchedule(s);
				continue;
			}
			else if(s->time > mRunningTime)
			{
				break;
			}

			sm->pop();
			s->run();
			if(s->repeat == 0)
			{
				sm->removeSchedule(s);
				continue;
			}
			else
			{
				s->time += s->interval;
				sm->push(s);
			}
		} // while
	}

	std::shared_ptr<Schedule> GLDirector::schedule(Node * entity, Schedule::Callback callback,
		float interval, int repeat)
	{
		if(callback == nullptr || repeat == 0) return nullptr;
		auto p = GLScheduleManager::GetDefault()->createSchedule();
		auto s = p.first;
		s->entity = entity;
		s->callback = callback;
		s->interval = (interval < mUpdateInterval ? mUpdateInterval : interval);
		s->time = mRunningTime + interval;
		s->repeat = repeat;
		GLScheduleManager::GetDefault()->push(s);
		return p.second;
	}

	void GLDirector::removeAllSchedules(Node * entity)
	{
		auto ss = GLScheduleManager::GetDefault()->getAllSchedules();
		for(auto i : ss)
		{
			if(i->entity == entity)
			{
				i->remove();
			}
		}
	}
	void GLDirector::_updateVap2dSize()
	{
		switch (mScaleMode) {
			case ScaleMode::Fill:
			{
				auto view_size = mDesignSize;
				mRootLayer->setScale(Zen::Pt2(2.f/view_size.x, 2.f/view_size.y));
				mRootLayer->setPosition(Zen::Pt2(-1.f));
				mTouchScale.set(view_size.x / mScreenSize.x, view_size.y / mScreenSize.y);
				mTouchShift.set(0);
				break;
			}
			case ScaleMode::Resize:
			{
				auto view_size = mScreenSize;
				mRootLayer->setScale({2.f/view_size.x, 2.f/view_size.y});
				mRootLayer->setPosition(Zen::Pt2(-1.f));
				mTouchShift.set(1);
				mTouchShift.set(0);
				break;
			}
			case ScaleMode::AspectFill:
			{
				auto view_size = mDesignSize;
				float sx = mDesignSize.x / mScreenSize.x;
				float sy = mDesignSize.y / mScreenSize.y;
				float scale = std::min(sx, sy);
				float x = mScreenSize.x * scale;
				float y = mScreenSize.y * scale;
				mRootLayer->setScale({2.f/x, 2.f/y});
				mRootLayer->setPosition({-view_size.x/x, -view_size.y/y});
				mTouchScale.set(scale, scale);
				mTouchShift.set((view_size.x - x)/2.f, (view_size.y - y)/2.f);
				break;
			}
			default:
				break;
		}
	}

	void GLDirector::draw()
	{
		Zen::GL::DrawAPI::ClearWithColor(mClearColor);
		mRootLayer->draw();
	}

	ContainerNode * GLDirector::getRootNode()
	{
		return mRootLayer;
	}
	void GLDirector::setClearColor(Zen::Color4f color)
	{
		mClearColor = color;
	}
	Zen::Color4f GLDirector::getClearColor()
	{
		return mClearColor;
	}

	void GLDirector::setUpdateInterval(float interval)
	{
		mUpdateInterval = interval;
	}

	float GLDirector::getUpdateInterval()
	{
		return mUpdateInterval;
	}


	void GLDirector::setScaleMode(ScaleMode mode)
	{
		if(mScaleMode == mode) return;
		mScaleMode = mode;
		mIsSizeDirty = true;
	}

	ScaleMode GLDirector::getScaleMode()
	{
		return mScaleMode;
	}

	Zen::Pt2 GLDirector::getVap2dSize()
	{
		if(mScaleMode == ScaleMode::Resize) return mScreenSize;
		else return mDesignSize;
	}

	Zen::App::Touch GLDirector::_corvertTouch(const Zen::App::Touch &touch)
	{
		Zen::App::Touch t;
		t.no = touch.no;
		t.x = touch.x * mTouchScale.x + mTouchShift.x;
		t.y = (mScreenSize.y - touch.y) * mTouchScale.y + mTouchShift.y;
		return t;
	}
	void GLDirector::setInteractionDelegate(Zen::App::InteractionDelegate* delegate)
	{
		mInteractionDelegate = delegate;
	}

	Zen::App::InteractionDelegate* GLDirector::getInteractionDelegate()
	{
		return mInteractionDelegate;
	}
	void GLDirector::onTouchDown(Zen::App::Touch const & touch)
	{
		if(!mInteractionDelegate) return;
		mInteractionDelegate->onTouchDown(_corvertTouch(touch));
	}

	void GLDirector::onTouchMove(Zen::App::Touch const & touch)
	{
		if(!mInteractionDelegate) return;
		mInteractionDelegate->onTouchMove(_corvertTouch(touch));
	}

	void GLDirector::onTouchUp(Zen::App::Touch const & touch)
	{
		if(!mInteractionDelegate) return;
		mInteractionDelegate->onTouchUp(_corvertTouch(touch));

	}

	void GLDirector::onTouchCancel(Zen::App::Touch const & touch)
	{
		if(!mInteractionDelegate) return;
		mInteractionDelegate->onTouchCancel(_corvertTouch(touch));

	}

	void GLDirector::onMouseMove(Zen::App::Touch const & touch)
	{

	}

	void GLDirector::onKeyDown(Zen::App::Key const & key)
	{

	}

	void GLDirector::onKeyUp(Zen::App::Key const & key)
	{

	}

	GLDirector * GLDirector::GetDefault()
	{
		static GLDirector * me = new GLDirector;
		return me;
	}

	Director * Director::GetDefault()
	{
		return GLDirector::GetDefault();
	}
}}
