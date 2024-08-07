cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(QT_PROGRAM VERSION 1.0.0 LANGUAGES CXX)


# you may have to run this with -DCMAKE_PREFIX_PATH=/usr/local/Qt-5.15.2/lib/cmake/Qt5
# cmake ./ -DCMAKE_PREFIX_PATH=/usr/local/Qt-5.15.2/lib/cmake/Qt5

add_compile_options(-fPIC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -fPIC")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OPENCV_MIN_VERSION "4.4") 

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()


find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS system thread regex)
    if(NOT Boost_FOUND)
        message(FETAL_ERROR "Boost > 1.65.1 not found")
    else()
        message("BOOST VERSION FOUND: ${Boost_VERSION}")
    endif()

find_package(OpenCV ${OPENCV_MIN_VERSION} REQUIRED)
    if(NOT OpenCV_FOUND)
        message(FATAL_ERROR "OpenCV > 4.4 not found.")
    else()
        message("OPENCV VERSION FOUND: ${OpenCV_VERSION}")
    endif()


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Multimedia REQUIRED)
find_package(Qt5 COMPONENTS MultimediaWidgets REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gstreamer REQUIRED gstreamer-1.0)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0 >= 1.4)
pkg_search_module(gstreamer-sdp REQUIRED IMPORTED_TARGET gstreamer-sdp-1.0 >= 1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0 >= 1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0 >= 1.4)
pkg_check_modules(GLIB REQUIRED glib-2.0)


add_executable(${PROJECT_NAME}
                 main.cpp
                 MainWindow.cpp
                 WindowWorker.cpp
                 VideoWidget.cpp
                 ConsoleWidget.cpp
                 TreeWidget.cpp
                 TCPManager.cpp
                 StreamInterface.cpp
                 ${PROJECT_SOURCE_DIR}/../common/gstream/src/GStreamerPipeline.cpp
                 ${PROJECT_SOURCE_DIR}/../common/module/src/ModuleValue.cpp
                 ${PROJECT_SOURCE_DIR}/../common/module/src/ModuleValueList.cpp
                 ${PROJECT_SOURCE_DIR}/../common/module/src/Module.cpp
                 ${PROJECT_SOURCE_DIR}/../common/module/src/ModuleController.cpp)



include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/../common/module/inc
                    ${PROJECT_SOURCE_DIR}/../common/gstream/inc
                    ${GLIB_INCLUDE_DIRS}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${GSTREAMER_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})


link_directories(${PROJECT_NAME} ${GLIB_LIBRARY_DIRS} 
                            ${GSTREAMER_LIBRARY_DIRS} )

target_link_libraries(${PROJECT_NAME} 
                                 Qt5::Widgets
                                 Qt5::Gui
                                 Qt5::Core
                                 Qt5::Multimedia
                                 Qt5::MultimediaWidgets
                                 ${GLIB_LIBRARIES}
                                 ${OpenCV_LIBS}
                                 PkgConfig::gstreamer
                                 PkgConfig::gstreamer-sdp
                                 PkgConfig::gstreamer-app
                                 PkgConfig::gstreamer-video
                                 ${Boost_LIBRARIES})



