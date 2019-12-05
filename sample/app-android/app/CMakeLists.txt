# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/../../../../zen-external)
set(SOURCE_PATH ../../..)

# libz
add_library(lib_zip STATIC IMPORTED)
set_target_properties(lib_zip
		PROPERTIES IMPORTED_LOCATION
		${EXTERNAL_PATH}/external/zlib/prebuilt/android/armeabi-v7a/libz.a)

# png

add_library(lib_png STATIC IMPORTED)
set_target_properties(lib_png
		PROPERTIES IMPORTED_LOCATION
		${EXTERNAL_PATH}/external/png/prebuilt/android/armeabi-v7a/libpng.a)


# jpeg

add_library(lib_jpeg STATIC IMPORTED)

set_target_properties(lib_jpeg
		PROPERTIES IMPORTED_LOCATION
		${EXTERNAL_PATH}/external/jpeg/prebuilt/android/armeabi-v7a/libjpeg.a)


# freetype2
add_library(lib_freetype STATIC IMPORTED)

set_target_properties(lib_freetype
		PROPERTIES IMPORTED_LOCATION
		${EXTERNAL_PATH}/external/freetype2/prebuilt/android/armeabi-v7a/libfreetype.a)


# openal
add_library(lib_openal STATIC IMPORTED)

set_target_properties(lib_openal
		PROPERTIES IMPORTED_LOCATION
		${EXTERNAL_PATH}/external/openal/prebuilt/android/armeabi-v7a/libopenal.a)

# include
include_directories(
		../cpp
		${SOURCE_PATH}/zen-cpp
		${SOURCE_PATH}/zen-image
		${SOURCE_PATH}/zen-math
		${SOURCE_PATH}/zen-app
		${SOURCE_PATH}/zen-app/android
		${SOURCE_PATH}/zen-string
		${SOURCE_PATH}/zen-system
		${SOURCE_PATH}/zen-system/android
		${SOURCE_PATH}/../zen-external/external/png/include/android/
		${SOURCE_PATH}/../zen-external/external/jpeg/include/android/
		${SOURCE_PATH}/../zen-external/external/freetype2/include/android/
		${SOURCE_PATH}/../zen-external/external/openal/include/android/
)

# source files
file(GLOB sources_cpp
		../cpp/*.cpp
		${SOURCE_PATH}/zen-cpp/*.cpp
		${SOURCE_PATH}/zen-image/*.cpp
		${SOURCE_PATH}/zen-math/*.cpp
		${SOURCE_PATH}/zen-app/*.cpp
		${SOURCE_PATH}/zen-app/android/*.cpp
		${SOURCE_PATH}/zen-string/*.cpp
		${SOURCE_PATH}/zen-system/*.cpp
		${SOURCE_PATH}/zen-system/android/*.cpp
		)

add_library( # Sets the name of the library.
		native-lib
		SHARED
		${sources_cpp}
		)

#find_path(GLES2_INCLUDE_DIR GLES2/gl2.h HINTS ${ANDROID_NDK})              #找到opengl头文件路径

#find_library(GLES2_LIBRARY libGLESv2.so HINTS ${GLES2_INCLUDE_DIR}/../lib)

find_library(log-lib log )

find_library(opensl-lib OpenSLES)

find_library(opengl-lib GLESv2)

target_link_libraries( # Specifies the target library.
		native-lib
		# ${GLES2_LIBRARY}
		${log-lib}
		${opensl-lib}
		${opengl-lib}
		lib_png
		lib_jpeg
		lib_openal
		lib_freetype
		lib_zip
		)

