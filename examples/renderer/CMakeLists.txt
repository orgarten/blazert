set(BUILD_TARGET "renderer")

include_directories(${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/examples/common" "/usr/local/include")

file(GLOB SOURCES "*.cc" "*.h" "${CMAKE_SOURCE_DIR}/examples/common/*.cc")
add_executable(${BUILD_TARGET} ${SOURCES} )

if(ENABLE_OMP)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(${BUILD_TARGET} PUBLIC OpenMP::OpenMP_CXX)
    endif()
endif()

source_group("Source Files" FILES ${SOURCES})
