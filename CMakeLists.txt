cmake_minimum_required(VERSION 3.5)

project(TobiiProNano LANGUAGES CXX VERSION 1.0.0)

include(Findliblsl.cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(TOBII_INCL_DIR "/usr/local/include/tobii_research")
find_library(TOBII_LIB "/usr/local/lib/tobii_research/libtobii_research.dylib")

find_package(Qt5 REQUIRED COMPONENTS Widgets)

if(WIN32)
	SET(Boost_USE_STATIC_LIBS OFF)
	SET(Boost_USE_MULTITHREAD ON)

	find_package(Boost REQUIRED)

	IF(Boost_FOUND)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
	ENDIF(Boost_FOUND)

	SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
else()
	find_package(Boost REQUIRED COMPONENTS thread)
endif()

add_executable(${PROJECT_NAME} MACOSX_BUNDLE WIN32
	src/main.cpp
	src/mainwindow.cpp
	src/recording.cpp
	include/mainwindow.hpp
	include/recording.hpp
	${TOBII_INCL_DIR}/tobii_research.h
	${TOBII_INCL_DIR}/tobii_research_eyetracker.h
	${TOBII_INCL_DIR}/tobii_research_calibration.h
	${TOBII_INCL_DIR}/tobii_research_streams.h
	window/mainwindow.ui
)

target_include_directories(${PROJECT_NAME} PRIVATE ${TOBII_INCL_DIR})

if(WIN32)
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
		Qt5::Widgets
		LSL::lsl
		ws2_32
		${USED_LIBS}
		${TOBII_LIB}
	)
else()
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
		Qt5::Widgets
		LSL::lsl
		Boost::thread
		${TOBII_LIB}
	)
endif()

installLSLApp(${PROJECT_NAME})

LSLGenerateCPackConfig()