cmake_minimum_required(VERSION 3.10)
project(mqttclient_example)

##################
## set arg info ##
##################
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

#########################
## find depend package ##
#########################
if(BUILD_EXAMPLES)
    set(PACKAGE_DEPEND ${MODULE_NAME})
endif()

# Only find Threads on non-Windows platforms
if(NOT WIN32)
    find_package("Threads")
endif()

###########
## build ##
###########

# 指定头文件搜索目录
set(INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 指定依赖文件搜索目录
set(DEPEND_SRC_DIRS
)

# 指定应用文件搜索目录
set(EXAMPLE_SRC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

foreach(FOREACH_SRC_DIR ${DEPEND_SRC_DIRS})
    aux_source_directory(${FOREACH_SRC_DIR} DEPEND_SOURCES)
endforeach()

foreach(FOREACH_SRC_DIR ${EXAMPLE_SRC_DIRS})
    aux_source_directory(${FOREACH_SRC_DIR} EXAMPLE_SOURCES)
endforeach()

include_directories(${INC_DIRS})

foreach(example_src_name ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_src_name} NAME_WE)
    add_executable(${example_name}_example ${example_src_name} ${DEPEND_SOURCES})
    if(WIN32)
        target_link_libraries(${example_name}_example ${PACKAGE_DEPEND})
    else()
        target_link_libraries(${example_name}_example ${CMAKE_THREAD_LIBS_INIT} ${PACKAGE_DEPEND})
    endif()
    message(STATUS "build example : ${example_name} ")
endforeach()

