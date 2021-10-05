
FILE(GLOB_RECURSE ABEAT_MAIN_SOURCE_FILES *.cpp)

message(ERROR ${ABEAT_MAIN_SOURCE_FILES})

add_executable(abeat ${ABEAT_MAIN_SOURCE_FILES})

target_link_libraries(
		abeat
		${CMAKE_THREAD_LIBS_INIT}
		${FFTW3_LIBRARIES}
		${OPENGL_LIBRARIES}
		${PULSE_LIBS}
		dl
		fmt::fmt
		glad
		glfw
		stb_image
)
