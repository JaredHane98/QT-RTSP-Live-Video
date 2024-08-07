cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(QT_SERVER_APPLICATION)


add_compile_options(-fPIC)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -fPIC")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()


find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS system thread regex)
    if(NOT Boost_FOUND)
        message(FETAL_ERROR "Boost > 1.65.1 not found")
    else()
        message("BOOST VERSION FOUND: ${Boost_VERSION}")
    endif()

    
add_executable(${PROJECT_NAME} 
                main.cpp
                TCPManager.cpp
               ${PROJECT_SOURCE_DIR}/../common/gstream/src/GStreamerPipeline.cpp
               ${PROJECT_SOURCE_DIR}/../common/module/src/ModuleValue.cpp
               ${PROJECT_SOURCE_DIR}/../common/module/src/ModuleValueList.cpp
               ${PROJECT_SOURCE_DIR}/../common/module/src/Module.cpp
               ${PROJECT_SOURCE_DIR}/../common/module/src/ModuleController.cpp)


find_package(PkgConfig REQUIRED)
pkg_check_modules(gstreamer REQUIRED gstreamer-1.0)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0 >= 1.4)
pkg_search_module(gstreamer-sdp REQUIRED IMPORTED_TARGET gstreamer-sdp-1.0 >= 1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0 >= 1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0 >= 1.4)
pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/../common/module/inc
                    ${PROJECT_SOURCE_DIR}/../common/gstream/inc
                    ${GLIB_INCLUDE_DIRS}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${GSTREAMER_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})



link_directories(${PROJECT_NAME} 
                                "/usr/lib/aarch64-linux-gnu"
                                ${GLIB_LIBRARY_DIRS} 
                                ${GSTREAMER_LIBRARY_DIRS} )



target_link_libraries(${PROJECT_NAME} 
                      ${GLIB_LIBRARIES}
                      PkgConfig::gstreamer
                      PkgConfig::gstreamer-sdp
                      PkgConfig::gstreamer-app
                      PkgConfig::gstreamer-video
                      ${Boost_LIBRARIES})


# must manually link these
target_link_libraries(${PROJECT_NAME}  "/usr/lib/aarch64-linux-gnu/libgstvideo-1.0.so")
target_link_libraries(${PROJECT_NAME}  "/usr/lib/aarch64-linux-gnu/libgstrtspserver-1.0.so")
target_link_libraries(${PROJECT_NAME}  "/usr/lib/aarch64-linux-gnu/libgstbase-1.0.so")
target_link_libraries(${PROJECT_NAME}  "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/libgstrtsp.so")
target_link_libraries(${PROJECT_NAME}  "/usr/lib/aarch64-linux-gnu/libgio-2.0.so")