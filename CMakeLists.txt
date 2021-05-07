
cmake_minimum_required(VERSION 3.16)
project(abeat)

add_subdirectory(external/fmt)
add_subdirectory(external/glad)
add_subdirectory(external/stb_image)

find_package(PkgConfig REQUIRED)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)
find_package(PulseAudio REQUIRED)

pkg_search_module(FFTW3 REQUIRED fftw3f libfftw3f)

add_definitions(${OPENGL_DEFINITIONS})
include_directories(
		include

		external/fmt/include
		external/glad/include
		external/stb_image/include

		${PULSEAUDIO_INCLUDE_DIRS}
		${OPENGL_INCLUDE_DIRS}
)

set(CMAKE_CXX_STANDARD 17)

set(PULSE_LIBS ${PULSE_LIBS} "pulse")

add_subdirectory(main)
