# brief
zen-app

This is an application model for games.
If you create your own application, dont include this part.

The 'Vap2d' depends this.

# choose graphics engine

 For ios:
 in the 'Build Settings', define one of these preprocessor macros to choose metal/gles supporting:

 ZEN_P_USE_METAL
 ZEN_P_USE_GLES

 eg:
 // use metal
 GCC_PREPROCESSOR_DEFINITIONS = ZEN_P_USE_METAL


# ZenAppMain

 The main entrance was covered.
 Use the function 'ZenAppMain' rather than the 'main' to start your app.

 eg:
######################## begin code
 `
 using namespace Zen;

 class Delegate : public AppRuntimeDelegate
 {
 // override and do your job here.
 };
 
 int ZenAppMain(int argc, const char ** argv)
 {
	auto delegate = std::shared_ptr(new Delegate);
	Zen::App::S()->setRuntimeDelegate(delegate);
 }
 `
######################## end code