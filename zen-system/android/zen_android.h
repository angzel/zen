#include "zen_os.h"

#if defined(ZEN_OS_ANDROID)

#include <thread>
#include <string>

namespace Zen { namespace Android {

	JNIEnv * GetEnv();

	void SetEnv(JNIEnv *);

}}

#endif
