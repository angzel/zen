#pragma once

#include "zen_app_runtime.h"
#include "zen_ticker.h"
#include <vector>
#include <mutex>
#include "zen_vap2d_node.h"

namespace Zen { namespace Vap2d {

	class RootDelegate {
	public:
		virtual void onLaunch() {}

		virtual void onResize() {}

		virtual void onExit() {}

		virtual void onPause() {}

		virtual void onResume() {}

		virtual void onUpdate(float interval) {}
		/**
		 @function onBack
		 Android back action
		 */
		virtual void onBack() {}
		/**
		 @function onBack
		 Windows <alt+f4> action
		 */
		virtual void onClose() {}
	};
	
	class Root {

	public:
		static Root * GetDefault();

		/**
		 @function run
		 - call this in Main function.
		 */
		virtual void run() = 0;
		/**
		 @function setDelegate
		 set the delegate in the function ZenAppMain so that onLaunch could be called rightly.
		 if reset a new one, the original delegate is not deleted here.
		 */
		virtual void setDelegate(RootDelegate * delegate) = 0;
		virtual RootDelegate * getDelegate() = 0;

	public: // below functions not valid before onLaunch.

		/**
		 @setViewSize
		 - design size
		 */
		virtual void setViewSize(Size2 size) = 0;
		virtual Size2 getViewSize() = 0;

		/**
		 @function getRealViewSize
		 get the application view size.
		 */
		virtual Size2 getRealViewSize() = 0;
		virtual float getViewScale() = 0;
		/**
		 gl clear color
		 */
		virtual void setBackgroundColor(Zen::Color4f color) = 0;
		virtual Zen::Color4f getBackgroundColor() = 0;

		/**
		 total running time. ( not include paused time)
		 */
		virtual double getRunningTime() = 0;

		/**
		 @function replaceRootNode
		 the original root node will auto remove(). dont remove() or delete any more.
		 */
		virtual void replaceRootNode(Node * node) = 0;
		virtual Node * getRootNode() = 0;

	protected:
		Root() = default;

		virtual ~Root() = default;
	};
}}
