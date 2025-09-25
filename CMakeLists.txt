cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Tell IDF to look into drv/ for extra components
set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/drv)

project(envirotrack)
