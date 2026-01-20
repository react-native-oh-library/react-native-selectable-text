cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB rnoh_selectable_text_SRC CONFIGURE_DEPENDS *.cpp)
add_library(rnoh_selectable_text SHARED ${rnoh_selectable_text_SRC})
target_include_directories(rnoh_selectable_text PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rnoh_selectable_text PUBLIC rnoh)
target_link_libraries(rnoh_selectable_text PUBLIC libace_ndk.z.so)