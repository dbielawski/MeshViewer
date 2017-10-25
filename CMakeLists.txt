cmake_minimum_required(VERSION 3.2)

# Project name
project(MeshViewer)

# Tell CMake to compile with C++11
set(CMAKE_CXX_STANDARD 11)

# Activate optimisation when in release mode
# Currently CMake automatically uses -o2 option in CXX flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -o3")

# Tell CMake to run moc when needed.
set(CMAKE_AUTOMOC ON)

# Tell CMake to run uic when needed.
#set(CMAKE_AUTOUIC ON)

# Tell CMake to run rcc when needed
set(CMAKE_AUTORCC ON)

# Moc generated files are located in the current dir so we need to tell CMake to look for them.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find Qt5
find_package(Qt5 COMPONENTS Widgets Core Gui OpenGL REQUIRED)
# Add Qt5 definitions and includes to build libraries. 
# Widgets add Widgets Core and Gui
add_definitions(${Qt5Widgets_DEFINITIONS})
include_directories(${Qt5Widgets_INCLUDES})

add_definitions(${Qt5OpenGL_DEFINITIONS})
include_directories(${Qt5OpenGL_INCLUDES})

# Find OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL not found.")
endif()

# Find CGAL
find_package(CGAL REQUIRED)
if(NOT CGAL_FOUND)
	message(FATAL_ERROR "CGAL not found. You should try to install libcgal-dev libcgal-qt5-dev and libcgal-ipelets")
endif()
include(${CGAL_USE_FILE})

# Set include directories
include_directories(${CMAKE_SOURCE_DIR}/include
			${CMAKE_SOURCE_DIR}/ext/eigen3
			${CMAKE_SOURCE_DIR}/ext/
			${CMAKE_SOURCE_DIR}/ext/tinyobjloader)

# Tell CMake to compile tinyobjloader
add_subdirectory(${CMAKE_SOURCE_DIR}/ext/tinyobjloader)

# Use Qt5 ressources
set(RESOURCE resources.qrc)
# Use Qt5 ui
set(UI mainwindow.ui)
qt5_wrap_ui(UIS_H ${UI})

# Adding sources
set(SOURCES
 	source/main.cpp
    	source/mainwindow.cpp
    	source/octree.cpp
    	source/mesh.cpp
    	source/pgm3d.cpp
    	source/glwidget.cpp
    	source/camera.cpp
    	source/scene.cpp 
    	source/light.cpp 
    	source/obj.cpp 
    	source/alignedbox3f.cpp 
    	source/wireboundingbox.cpp
        source/coctree.cpp

	include/mainwindow.h
	include/utils.h
	include/octree.h
	include/mesh.h
	include/pgm3d.h
	include/glwidget.h
	include/camera.h
	include/scene.h
	include/model3d.h
	include/light.h
	include/obj.h
	include/alignedbox3f.h
        include/wireboundingbox.h
        include/coctree.h
        include/polyhedron.h
)


add_executable(${PROJECT_NAME} ${UIS_H} ${RESOURCE} ${SOURCES} )
target_link_libraries(${PROJECT_NAME} tinyobjloader ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${OPENGL_LIBRARIES})

