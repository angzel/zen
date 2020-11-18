
need libraries

libpng, libjpeg


Android Studio
CMakeLists.txt
[****************

#[......] your code

add_library(lib_zip STATIC IMPORTED)
set_target_properties(lib_zip
		PROPERTIES IMPORTED_LOCATION
		${CMAKE_SOURCE_DIR}/../../../../../../../zen-external/external/zlib/prebuilt/android/armeabi-v7a/libz.a)

add_library(lib_png STATIC IMPORTED)
set_target_properties(lib_png
		PROPERTIES IMPORTED_LOCATION
		${CMAKE_SOURCE_DIR}/../../../../../../../zen-external/external/png/prebuilt/android/armeabi-v7a/libpng.a)


add_library(lib_jpeg STATIC IMPORTED)

set_target_properties(lib_jpeg
		PROPERTIES IMPORTED_LOCATION
		${CMAKE_SOURCE_DIR}/../../../../../../../zen-external/external/jpeg/prebuilt/android/armeabi-v7a/libjpeg.a)

#[......]
#[......] your code

include_directories(
		../../../../../../../zen-external/external/png/include/android/
		../../../../../../../zen-external/external/jpeg/include/android/
)

#[......]
#[......] your code

target_link_libraries( 
		your-app-lib
		${log-lib} #other lib
		lib_png
		lib_jpeg
		lib_zip
		)


		*******************]
