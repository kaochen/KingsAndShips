project(kingsandships)

SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 1)
SET(PATCH_VERSION 1)
set(PROJECT_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
set(PROJECT_NAME kingsandships)

IF (WIN32)
	set(SHARE_FOLDER .)
ELSE()
	set(LOCALE_FOLDER "/usr/share/locale")
	set(SHARE_FOLDER share/games/${PROJECT_NAME})
	set(BIN_FOLDER games)
	set(DOC_FOLDER share/doc/${PROJECT_NAME})
	set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST
			${CMAKE_INSTALL_PREFIX}
			${CMAKE_INSTALL_PREFIX}/bin
			${CMAKE_INSTALL_PREFIX}/share
			${CMAKE_INSTALL_PREFIX}/share/man
			${CMAKE_INSTALL_PREFIX}/share/man/man1
			${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/256x256/apps
			${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/256x256
			${CMAKE_INSTALL_PREFIX}/share/icons/hicolor
			${CMAKE_INSTALL_PREFIX}/share/icons
			${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps
			${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable
			${CMAKE_INSTALL_PREFIX}/share/applications
			${CMAKE_INSTALL_PREFIX}/share/locale
			${CMAKE_INSTALL_PREFIX}/share/locale/fr
			${CMAKE_INSTALL_PREFIX}/share/locale/fr/LC_MESSAGES
	)
ENDIF()

configure_file("src/config.h.in" "config.h" @ONLY)


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

IF(CMAKE_COMPILER_IS_GNUCXX)
 IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("Generated with config types: ${CMAKE_BUILD_TYPE}")
     	target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wno-deprecated-declarations -g -std=gnu++17)
    ELSE()
	message("Generated with config types: Release")
    	target_compile_options(${PROJECT_NAME} PUBLIC -Wno-error -Wno-deprecated-declarations -O3  -std=gnu++17)
    ENDIF()
ENDIF()


install(TARGETS ${PROJECT_NAME}
	RUNTIME
	DESTINATION ${BIN_FOLDER}
	RENAME ${PROJECT_NAME}-${PROJECT_VERSION})

IF (WIN32)
	set(CPACK_GENERATOR "TGZ;NSIS")
ELSE()
	set(CPACK_GENERATOR "RPM;DEB;TGZ")
	##create the desktop file
	set (DESKTOP_TMP_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop")
	configure_file("${PROJECT_NAME}.desktop.in" "${DESKTOP_TMP_PATH}" @ONLY)
	install (FILES ${DESKTOP_TMP_PATH} DESTINATION share/applications)

	## Install translation files :
	install (FILES po/fr/${PROJECT_NAME}.mo DESTINATION share/locale/fr/LC_MESSAGES)

	##create a man
	set(MAN_FOLDER share/man/man1)
	set(MAN_FILE "${PROJECT_NAME}.1")
	set (MAN_TMP_PATH "${CMAKE_CURRENT_BINARY_DIR}/${MAN_FILE}")
	configure_file("${MAN_FILE}.in" ${MAN_TMP_PATH} @ONLY)
	add_custom_target(man ALL)
	add_custom_command(
	TARGET man
	COMMAND gzip -c "${MAN_TMP_PATH}" > "${MAN_TMP_PATH}.gz"
	DEPENDS ${MAN_FILE}
	)
	install (FILES ${MAN_TMP_PATH}.gz DESTINATION ${MAN_FOLDER})

	##install the icon app
	install (FILES kingsandships_icon.png DESTINATION share/pixmaps)

ENDIF()


install (DIRECTORY data DESTINATION ${SHARE_FOLDER})
install (FILES README.md DESTINATION ${DOC_FOLDER})
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


IF(UNIX AND NOT WINDOWS)
	set (CPACK_SOURCE_IGNORE_FILES "bin/" "build" "logs/" ".git/" ".vscode/" "doc/" "src/img/cache_fluid/" "src/img/render/" "src/img/blendcache_barricade_01/" "valgrind.log" "perf.log")
	set (CPACK_SOURCE_GENERATOR TGZ)
	set (CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}_${CPACK_PACKAGE_VERSION}.orig")
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
	ENDIF()
## Windows
ELSEIF(CPACK_GENERATOR MATCHES "NSIS")
	set (CPACK_NSIS_MODIFY_PATH ON)
	set (CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
ENDIF()

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

include (CPack)

