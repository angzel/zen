#pragma once
#include "zen_view_director.h"
#include "zen_view_gl_layer.h"
#include "zen_app_delegate.h"

namespace Zen { namespace Vap2d {

	class _GLScheduleManager
	{

	};

	class GLDirector
	: public virtual Director
	, public virtual Zen::App::InteractionDelegate
	{
	public:
		static GLDirector * GetDefault();

		GLDirector();

		virtual Zen::Pt2 getDesignSize() override;

		virtual void setDesignSize(Zen::Pt2 size) override;

		virtual void _setRealScreenSize(Zen::Pt2 size);

		virtual Zen::Pt2 getRealScreenSize() override;

		virtual ContainerNode * getRootNode() override;

		virtual void setClearColor(Zen::Color4f color) override;

		virtual Zen::Color4f getClearColor() override;

		virtual void setUpdateInterval(float interval) override;

		virtual float getUpdateInterval() override;

		virtual void setScaleMode(ScaleMode) override;

		virtual ScaleMode getScaleMode() override;

		virtual Zen::Pt2 getVap2dSize() override;

		virtual void setInteractionDelegate(Zen::App::InteractionDelegate*) override;

		virtual Zen::App::InteractionDelegate* getInteractionDelegate() override;

		virtual void onTouchDown(Zen::App::Touch const & touch) override;

		virtual void onTouchMove(Zen::App::Touch const & touch) override;

		virtual void onTouchUp(Zen::App::Touch const & touch) override;

		virtual void onTouchCancel(Zen::App::Touch const & touch) override;

		virtual void onMouseMove(Zen::App::Touch const & touch) override;

		virtual void onKeyDown(Zen::App::Key const & key) override;

		virtual void onKeyUp(Zen::App::Key const & key) override;

	public:
		void update(double interval);

		void draw();

		std::shared_ptr<Schedule> schedule(Node * entity, Schedule::Callback callback,
			float interval = 0, int repeat = 1);

		void removeAllSchedules(Node * entity);
	protected:
		void _updateSchedules();

		void _updateVap2dSize();

		Zen::App::Touch _corvertTouch(Zen::App::Touch const & touch);
	protected:

		Zen::Pt2 mDesignSize;

		Zen::Pt2 mScreenSize;

		GLLayer * mRootLayer = nullptr;

		Zen::Color4f mClearColor = Zen::EColor::White;

		float mUpdateInterval = 1.f / 60.f;
		
		ScaleMode mScaleMode = ScaleMode::Resize;

		bool mIsSizeDirty = false;

		Zen::Pt2 mTouchScale = Zen::Pt2(1);

		Zen::Pt2 mTouchShift = Zen::Pt2(0);

		Zen::App::InteractionDelegate* mInteractionDelegate;

		double mRunningTime = 0;
	};
}}
