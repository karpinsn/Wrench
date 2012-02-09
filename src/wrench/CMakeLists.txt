#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#
#	Wrench Library for C++
#	---------------------------------------------------------------------------------------

set(NAME wrench)

#	---------------------------------------------------------------------------------------
#	List all of the sources to be compiled
#	---------------------------------------------------------------------------------------

set(GL_SOURCES 
	
	
	gl/Camera.cpp
	gl/FBO.cpp
        gl/ShaderProgram.cpp 
	gl/Shader.cpp
	gl/Texture.cpp
	gl/OGLStatus.cpp
	gl/VBO.cpp
	gl/IBO.cpp
	gl/VAO.cpp
	gl/Converter.cpp

	gl/Camera.h
	gl/FBO.h
        gl/ShaderProgram.h
	gl/Shader.h
	gl/Texture.h
	gl/OGLStatus.h
	gl/VBO.h
	gl/IBO.h
	gl/VAO.h
	gl/Converter.h
)

set(GL_UTILS_SOURCES
	gl/utils/GradientBackground.cpp
	gl/utils/Arcball.cpp
	gl/utils/AxisDisplay.cpp
	gl/utils/TextureDisplay.cpp
	gl/utils/FPSCalculator.cpp
	gl/utils/BoundingBox.cpp
    gl/utils/CoordinateConverter.cpp
    gl/utils/IntersectionCalculator.cpp

	gl/utils/GradientBackground.h
	gl/utils/Arcball.h
	gl/utils/AxisDisplay.h
	gl/utils/TextureDisplay.h
	gl/utils/FPSCalculator.h
	gl/utils/BoundingBox.h
    gl/utils/CoordinateConverter.h
    gl/utils/IntersectionCalculator.h
)

set(SOURCES
	
	Logger.cpp
	Timer.cpp	

	Logger.h
	Timer.h
)

source_group("wrench" FILES ${SOURCES})
source_group("wrench::gl" FILES ${GL_SOURCES})
source_group("wrench::gl::utils" FILES ${GL_UTILS_SOURCES})

include_directories(..)

#	-----------------------------------------------
#   Allow Wrench to integrate with other libraries
#   -----------------------------------------------
option(WRENCH_REACTOR_INTEGRATION "Integrate reactor with the Wrench library" OFF)
if(WRENCH_REACTOR_INTEGRATION)
    SET(USE_REACTOR ON)
else()
    SET(USE_REACTOR OFF)
endif()

option(WRENCH_VRJ_INTEGRATION "Integrate VR Juggler with the Wrench library" OFF)
if(WRENCH_VRJ_INTEGRATION)
    SET(USE_VRJ ON)
else()
    SET(USE_VRJ OFF)
endif()

#	-----------------------------------------------
#   Generate the config file for the code
#   -----------------------------------------------
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config-wrench.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config-wrench.h)
LIST(APPEND SOURCES config-wrench.h)

#	---------------------------------------------------------------------------------------
#	Build Documentation
#	---------------------------------------------------------------------------------------
option(WRENCH_GENERATE_DOC "Build Doxygen Documentation" OFF)
if(WRENCH_GENERATE_DOC)
	add_subdirectory(../../doc ${PROJECT_BINARY_DIR}/doc)
endif(WRENCH_GENERATE_DOC)

#	---------------------------------------------------------------------------------------
#	Build the library
#	---------------------------------------------------------------------------------------

add_library(${NAME} ${SOURCES} ${GL_SOURCES} ${GL_UTILS_SOURCES})

