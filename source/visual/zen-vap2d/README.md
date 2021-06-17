# Vap2d game engine
This is a simple 2D game engine.
The tree mode was used and you can start from the 'Root' node (see "zen_vap2d_root.h").

You can use these nodes:
-- Layer
-- Sprite
-- Label
-- Particle
-- Doodle (drawable things)

## include

'zen-app', 'zen-image', and the basic parts.

and third parts:
'zen-gles2', 'zen-metal'(iOS only),  'zen-freetype', 'zen-jpg', 'zen-png'

also import freetype, png, jpg libraries from zen-external.git (or other place if you have got them).

## usage

start your game like this:

```
	// file name: main.cpp
	#include "zen_vap2d.h"

	class MyDelegate : public Zen::Vap2d::RootDelegate
	{
	public:

		virtual void onLaunch() override
		{
			Zen::App::Get()->setFramesPerSecond(60);
			Zen::App::Get()->setRotatable(true);

			// start your game logic.
		}
		// ... other overrides
	};

	int ZenAppMain(int argc, const char ** argv)
	{
		auto root = Zen::Vap2d::Root::Get();
		root->run(std::shared_ptr<RootDelegate>(new MyDelegate));
		return 0;
	}
```

