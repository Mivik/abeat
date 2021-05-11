
add_executable(
		abeat

		animation.cpp
		context.cpp
		fps_printer.cpp
		main.cpp
		mgl.cpp
		object.cpp
		stream.cpp
		transform.cpp
		widget.cpp
		window_dragger.cpp

		streams/FFT.cpp
		streams/gravity.cpp
		streams/pulse_audio.cpp
		streams/smoother.cpp
		streams/spectrum.cpp
		streams/stereo.cpp
		streams/transform.cpp

		widgets/bars.cpp
		widgets/curve.cpp
		widgets/image.cpp
)

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
