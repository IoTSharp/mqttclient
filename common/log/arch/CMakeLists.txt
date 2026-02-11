# Select platform-specific arch directory
if(WIN32)
    set(SUBDIRS "windows")
else()
    set(SUBDIRS "linux")
endif()

foreach(subdir ${SUBDIRS})
    add_subdirectory(${subdir})
endforeach()

