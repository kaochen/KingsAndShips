cmake_minimum_required(VERSION 2.8)

project(kingsandships)

set(CMAKE_BUILD_TYPE Debug)
SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 1)
SET(PATCH_VERSION 1)
set(PROJECT_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
set(PROJECT_NAME kingsandships)

IF (WIN32)
	set(SHARE_FOLDER .)
ELSE()
	set(SHARE_FOLDER share/games/${PROJECT_NAME})
	set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST /usr/local /usr/local/bin /usr/local/share)
ENDIF()
	configure_file("src/config.h.in" "config.h" @ONLY)
	configure_file("kingsandships.desktop.in" "kingsandships.desktop" @ONLY)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set (SRCS
	src/tools.cpp
	src/settings.cpp
	src/coord.cpp
	src/time.cpp
	src/message.cpp
	src/anim.cpp
	src/xml.cpp
	src/wallet.cpp
	src/texture.cpp
	src/textureList.cpp
	src/level/node.cpp
	src/level/pathfinding.cpp
	src/level/zlayer.cpp
	src/level/grid.cpp
	src/level/level.cpp
	src/level/weapons.cpp
	src/level/factory.cpp
	src/level/shooter.cpp
	src/level/towers.cpp
	src/level/barricade.cpp
	src/level/boat.cpp
	src/level/town.cpp
	src/level/landscape.cpp
	src/level/gameUnits.cpp
	src/window.cpp
	src/menu/command.cpp
	src/menu/popup.cpp
	src/menu/menuItems.cpp
	src/menu/tab.cpp
	src/menu/menu.cpp
	src/locator.cpp
	src/main.cpp
)


set (HEADERS
	src/tools.h
	src/settings.h
	src/coord.h
	src/time.h
	src/message.h
	src/anim.h
	src/xml.h
	src/wallet.h
	src/texture.h
	src/textureList.h
	src/level/node.h
	src/level/pathfinding.h
	src/level/zlayer.h
	src/level/grid.h
	src/level/level.h
	src/level/weapons.h
	src/level/factory.h
	src/level/shooter.h
	src/level/towers.h
	src/level/barricade.h
	src/level/boat.h
	src/level/town.h
	src/level/landscape.h
	src/level/gameUnits.h
	src/window.h
	src/menu/command.h
	src/menu/popup.h
	src/menu/menuItems.h
	src/menu/tab.h
	src/menu/menu.h
	src/locator.h
)

add_executable(${PROJECT_NAME}  ${SRCS} ${HEADER})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

find_package(LibXML++ REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR}
                    ${LibXML++_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
                             ${SDL2_IMAGE_LIBRARIES}
                             ${SDL2_TTF_LIBRARIES}
                             ${LibXML++_LIBRARIES})

target_link_libraries(${PROJECT_NAME} stdc++fs)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wno-deprecated-declarations -g -std=gnu++17)


install(TARGETS ${PROJECT_NAME}
	RUNTIME
	DESTINATION bin
	RENAME ${PROJECT_NAME}-${PROJECT_VERSION})

IF (WIN32)
	set(CPACK_GENERATOR "TGZ;NSIS")
ELSE()
	set(CPACK_GENERATOR "RPM;DEB;TGZ")
	##install (FILES icon.png DESTINATION share/pixmaps)
	install (FILES bin/kingsandships.desktop DESTINATION share/applications)
ENDIF()


install (DIRECTORY data DESTINATION ${SHARE_FOLDER})
install (FILES README.md DESTINATION ${SHARE_FOLDER})
install (FILES preferences.ini DESTINATION ${SHARE_FOLDER})



set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A defense Tower game")
set(CPACK_PACKAGE_EXECUTABLES "kingsandships;kingsandships")
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})


## Debian
IF (CPACK_GENERATOR MATCHES "DEB")
	set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kao chen")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-image-2.0-0")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-ttf-2.0-0")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libxml++2.6-2v5")
	set(CPACK_DEBIAN_PACKAGE_SECTION "game")

## RedHat
ELSEIF(CPACK_GENERATOR MATCHES "RPM")
	set(CPACK_RPM_PACKAGE_REQUIRES "libsdl2-2.0-0")
	set(CPACK_RPM_PACKAGE_REQUIRES "libsdl2-image-2.0-0")
	set(CPACK_RPM_PACKAGE_REQUIRES "libsdl2-ttf-2.0-0")
	set(CPACK_RPM_PACKAGE_REQUIRES "libxml++2.6-2v5")
	set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

## Windows
ELSEIF(CPACK_GENERATOR MATCHES "NSIS")
	set (CPACK_NSIS_MODIFY_PATH ON)
	set (CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
ENDIF()

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

include (CPack)

