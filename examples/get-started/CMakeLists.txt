# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED BCH_PATH)
    set(BCH_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)
endif()

set(EXTRA_COMPONENT_DIRS ${BCH_PATH})

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(get-started)
