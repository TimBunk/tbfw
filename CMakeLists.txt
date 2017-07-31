#CMake entry point
cmake_minimum_required (VERSION 3.8)
project (tbfw)

# delete CMakeCache.txt from the build directory when you edit these options
####################################################################
OPTION(MAKE_START "Make TBFW start applications" ON)
OPTION(MAKE_DEMO_SCENES "Make TBFW demo applications" ON)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like 'build')" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies
find_package(OpenGL REQUIRED)

add_subdirectory (external)

include_directories(
  ${CMAKE_SOURCE_DIR}/external/glfw-3.2.1/include/GLFW
  ${CMAKE_SOURCE_DIR}/external/glew-2.0.0/include
  ${CMAKE_SOURCE_DIR}/external/SOIL2/src
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/tbfw
)

link_directories(
  ${CMAKE_SOURCE_DIR}/external/glfw-3.2.1/src
  ${CMAKE_SOURCE_DIR}/external/glew-2.0.0
  ${CMAKE_SOURCE_DIR}/external/glm-0.9.9.0
  ${CMAKE_SOURCE_DIR}/external
)

set(ALL_GRAPHICS_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

set(TBFWSOURCE
  tbfw/camera.h
  tbfw/camera.cpp
  tbfw/mesh.h
  tbfw/mesh.cpp
  tbfw/OBJloader.h
  tbfw/OBJloader.cpp
  tbfw/shader.h
  tbfw/shader.cpp
  tbfw/texture.h
  tbfw/texture.cpp
)

set(SOILSOURCE
  ##SOIL IMAGE LOADER
  external/SOIL2/src/SOIL2/etc1_utils.c
  external/SOIL2/src/SOIL2/etc1_utils.h
  external/SOIL2/src/SOIL2/image_DXT.c
  external/SOIL2/src/SOIL2/image_DXT.h
  external/SOIL2/src/SOIL2/image_helper.c
  external/SOIL2/src/SOIL2/image_helper.h
  external/SOIL2/src/SOIL2/jo_jpeg.h
  external/SOIL2/src/SOIL2/pkm_helper.h
  external/SOIL2/src/SOIL2/pvr_helper.h
  external/SOIL2/src/SOIL2/SOIL2.c
  external/SOIL2/src/SOIL2/SOIL2.h
  external/SOIL2/src/SOIL2/stb_image_write.h
  external/SOIL2/src/SOIL2/stb_image.h
  external/SOIL2/src/SOIL2/stbi_DDS_c.h
  external/SOIL2/src/SOIL2/stbi_DDS.h
  external/SOIL2/src/SOIL2/stbi_ext_c.h
  external/SOIL2/src/SOIL2/stbi_ext.h
  external/SOIL2/src/SOIL2/stbi_pkm_c.h
  external/SOIL2/src/SOIL2/stbi_pkm.h
  external/SOIL2/src/SOIL2/stbi_pvr_c.h
  external/SOIL2/src/SOIL2/stbi_pvr.h
  ##END SOIL IMAGE LOADER
)

####################################################################
# start                                                            #
####################################################################
IF(MAKE_START)
  add_executable(start
    ${SOILSOURCE}
    ${TBFWSOURCE}
    start/main.cpp
	)
	target_link_libraries(start
		${ALL_GRAPHICS_LIBS}
	)
	# Copy assets to the build directory
	file(
		COPY tbfw/assets
		DESTINATION ${CMAKE_BINARY_DIR}
	)
  file(
		COPY tbfw/shaders
		DESTINATION ${CMAKE_BINARY_DIR}
	)
  file(
		COPY tbfw/textures
		DESTINATION ${CMAKE_BINARY_DIR}
	)
ENDIF()

####################################################################
# DEMO_SCENES                                                      #
####################################################################
IF(MAKE_DEMO_SCENES)
	# Copy assets to the build directory
	file(
		COPY tbfw/assets
		DESTINATION ${CMAKE_BINARY_DIR}
	)
  file(
		COPY tbfw/shaders
		DESTINATION ${CMAKE_BINARY_DIR}
	)
  file(
		COPY tbfw/textures
		DESTINATION ${CMAKE_BINARY_DIR}
	)
  add_executable(3Dscene
    ${SOILSOURCE}
    ${TBFWSOURCE}
    scenes/3dScene_with_Cam/main.cpp
  )
  target_link_libraries(3Dscene
		${ALL_GRAPHICS_LIBS}
	)
  add_executable(3Dcubes
    ${SOILSOURCE}
    ${TBFWSOURCE}
    scenes/3Dcubes/main.cpp
  )
  target_link_libraries(3Dcubes
		${ALL_GRAPHICS_LIBS}
	)
  add_executable(spotLight
    ${SOILSOURCE}
    ${TBFWSOURCE}
    scenes/spotLight/main.cpp
  )
  target_link_libraries(spotLight
		${ALL_GRAPHICS_LIBS}
	)
  add_executable(cppLights
    ${SOILSOURCE}
    ${TBFWSOURCE}
    scenes/cppLights/main.cpp
  )
  target_link_libraries(cppLights
		${ALL_GRAPHICS_LIBS}
	)
  add_executable(spot_dir_pointLightAllIneOne
    ${SOILSOURCE}
    ${TBFWSOURCE}
    scenes/spot_dir_pointLightAllIneOne/main.cpp
  )
  target_link_libraries(spot_dir_pointLightAllIneOne
		${ALL_GRAPHICS_LIBS}
	)
  add_executable(3dModelLoading
    ${SOILSOURCE}
    ${TBFWSOURCE}
    scenes/3dModelLoading/main.cpp
  )
  target_link_libraries(3dModelLoading
		${ALL_GRAPHICS_LIBS}
	)
ENDIF()
