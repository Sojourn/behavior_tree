file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

add_library(behavior_tree ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(behavior_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
