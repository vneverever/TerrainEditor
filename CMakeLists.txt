cmake_minimum_required(VERSION 2.8)

project(TerrainEditor)

set(TOF_LIB_PREFIX src)

#Cmake Policy
cmake_policy(SET CMP0020 OLD)
cmake_policy(SET CMP0043 OLD)


#QT5
set(QT_REQUIRED_VERSION 5.7)

find_package(Qt5 ${QT_REQUIRED_VERSION} REQUIRED Core Gui Widgets concurrent) 

qt5_add_resources(RESOURCES qtmdi.qrc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)



#Includes (for uic and moc files)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Add glbase and glm
include_directories(
    ${CMAKE_SOURCE_DIR}
	glbase
)

#Source Files
set(${PROJECT_NAME}_SRC

    ${TOF_LIB_PREFIX}/main.cpp
	${TOF_LIB_PREFIX}/MainWindow.cpp
	${TOF_LIB_PREFIX}/RenderWindow.cpp
	${TOF_LIB_PREFIX}/SkyBox.cpp
	${TOF_LIB_PREFIX}/Terrain.cpp
	${TOF_LIB_PREFIX}/Controls.cpp
	${TOF_LIB_PREFIX}/Loader.cpp
	${TOF_LIB_PREFIX}/TextureUtil.cpp
	${TOF_LIB_PREFIX}/Transform.cpp
	${TOF_LIB_PREFIX}/PainterWindow.cpp
	${TOF_LIB_PREFIX}/Manager.cpp
)

#Header Files
set(${PROJECT_NAME}_HDR
	${TOF_LIB_PREFIX}/MainWindow.h
	${TOF_LIB_PREFIX}/RenderWindow.h
	${TOF_LIB_PREFIX}/SkyBox.h
	${TOF_LIB_PREFIX}/Terrain.h
	${TOF_LIB_PREFIX}/Controls.h
	${TOF_LIB_PREFIX}/Loader.h
	${TOF_LIB_PREFIX}/TextureUtil.h
	${TOF_LIB_PREFIX}/Transform.h
	${TOF_LIB_PREFIX}/Manager.h
	${TOF_LIB_PREFIX}/PainterWindow.h
)

#Compiler Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Quick_EXECUTABLE_COMPILE_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS} ${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}")

#Add Executable
IF(UNIX)
  ADD_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_HDR} ${${PROJECT_NAME}_SRC} ${RESOURCES})
ELSEIF(APPLE)
  ADD_EXECUTABLE(${PROJECT_NAME} MACOSX_BUNDLE ${${PROJECT_NAME}_HDR} ${${PROJECT_NAME}_SRC} ${RESOURCES})
ELSEIF(WIN32)
  ADD_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_HDR} ${${PROJECT_NAME}_SRC} ${RESOURCES})
ENDIF()

qt5_use_modules(${PROJECT_NAME} Core Gui OpenGL)

#Link OpenGL
target_link_libraries(${PROJECT_NAME}
	${OPENGL_LIBRARIES}
)
