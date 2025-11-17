cmake_minimum_required(VERSION 3.10)
project(DSPChainPlugin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LV2
find_package(PkgConfig REQUIRED)
pkg_check_modules(LV2 REQUIRED lv2)

# Include directories
include_directories(${LV2_INCLUDE_DIRS})
include_directories(core)

# Add executable/library
add_library(dsp_chain SHARED plugin/dsp_chain.cpp core/dsp_core.c)

# Link libraries
target_link_libraries(dsp_chain ${LV2_LIBRARIES})

# Set output directory
set_target_properties(dsp_chain PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugin
)

# Install
install(TARGETS dsp_chain
    LIBRARY DESTINATION lib/lv2/dsp-chain.lv2/
)
install(FILES plugin/manifest.ttl plugin/dsp_chain.ttl
    DESTINATION lib/lv2/dsp-chain.lv2/
)
