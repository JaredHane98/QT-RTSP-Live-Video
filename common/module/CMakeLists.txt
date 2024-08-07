cmake_minimum_required(VERSION 3.5)
project(ModuleLibrary VERSION 1.0.0 DESCRIPTION "Thread Safe Communcation Library")



if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14 -Wno-deprecated-declarations")
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIBRARY_DEST ${PROJECT_SOURCE_DIR}/build/)



find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS system thread regex)
    if(NOT Boost_FOUND)
        message(FETAL_ERROR "Boost > 1.65.1 not found")
    else()
        message("BOOST VERSION FOUND: ${Boost_VERSION}")
    endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)


include_directories(${PROJECT_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIRS})


add_library(${PROJECT_NAME} SHARED ${PROJECT_SRC_DIR}/ModuleValue.cpp
                                   ${PROJECT_SRC_DIR}/ModuleValueList.cpp
                                   ${PROJECT_SRC_DIR}/Module.cpp
                                   ${PROJECT_SRC_DIR}/ModuleController.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
                      ${Boost_LIBRARIES})

message(${LIBRARY_DEST})

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${PROJECT_SOURCE_DIR})


