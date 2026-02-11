cmake_minimum_required(VERSION 3.10)

project(mqtt-client)

set(TARGETS "mqtt-client")
set(SUBDIRS  "common" "mqtt" "platform" "network" "mqttclient")

# 库文件前缀
set(LIBRARY_PREFIX "mc")

# 编译debug版本
set(CMAKE_BUILD_TYPE "Debug")

# 设置版本信息
set(PROJECT_MAJOR_VERSION 1) # 架版本信息
set(PROJECT_MINOR_VERSION 2) # 主版本信息
set(PROJECT_PATCH_VERSION 0) # 次版本信息
set(PROJECT_VERSION ${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION})

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/build/bin)       #设置可执行文件的输出目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/build/lib)           #设置库文件的输出目录
link_directories(${LIBRARY_OUTPUT_PATH})

# Platform-specific compiler flags
if(MSVC)
    # MSVC compiler flags for Windows
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Od /Zi")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    # GCC/Clang compiler flags for Linux/Unix
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -lpthread")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -no-pie -ggdb")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")
    
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -lpthread")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -no-pie -ggdb")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") 
    endif(CMAKE_COMPILER_IS_GNUCXX)
endif()


#########################################
## load cmake config and load 3rdparty ##
#########################################
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/color.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/module.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/install.cmake)

# foreach(incdir ${INCDIRS})
#     include_directories(${incdir})
# endforeach()

include_directories(${PROJECT_ROOT_DIR}/config)

foreach(subdir ${SUBDIRS})
    add_subdirectory(${PROJECT_ROOT_DIR}/${subdir})
endforeach()


get_module_info()
show_module_info()
install_project()

option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
