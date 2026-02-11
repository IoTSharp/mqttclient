string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_LIB_NAME ${CMAKE_CURRENT_SOURCE_DIR}) 

set(CURRENT_LIB_NAME ${LIBRARY_PREFIX}-${CURRENT_LIB_NAME})

# Select platform-specific arch directory
if(WIN32)
    set(ARCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/arch/windows)
else()
    set(ARCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/arch/linux)
endif()

# 指定头文件搜索目录
set(${CURRENT_LIB_NAME}_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

# 源文件目录
set(${CURRENT_LIB_NAME}_SRC_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${ARCH_DIR}
)

set_module_info(${CURRENT_LIB_NAME} "${${CURRENT_LIB_NAME}_INC_DIRS}")
get_module_info()

foreach(FOREACH_SRC_DIR ${${CURRENT_LIB_NAME}_SRC_DIRS})
    aux_source_directory(${FOREACH_SRC_DIR} ${CURRENT_LIB_NAME}_SOURCES)
endforeach()


# 生成动态库
add_library(${CURRENT_LIB_NAME}        SHARED ${${CURRENT_LIB_NAME}_SOURCES})
# 生成静态库
add_library(${CURRENT_LIB_NAME}_static STATIC ${${CURRENT_LIB_NAME}_SOURCES})

# Link threading library only on non-Windows platforms
if(NOT WIN32)
    target_link_libraries(${CURRENT_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(${CURRENT_LIB_NAME}_static ${CMAKE_THREAD_LIBS_INIT})
endif()

# 进行必要的配置，以生成动态库和静态库
# 指定静态库的输出名称
set_target_properties(${CURRENT_LIB_NAME}_static PROPERTIES OUTPUT_NAME ${CURRENT_LIB_NAME})
# 使动态库和静态库同时存在
set_target_properties(${CURRENT_LIB_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${CURRENT_LIB_NAME}_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# 指定动态库版本
set_target_properties(${CURRENT_LIB_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# 打包相关 install package
install(TARGETS ${CURRENT_LIB_NAME} ${CURRENT_LIB_NAME}_static
    EXPORT ${CURRENT_LIB_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)
