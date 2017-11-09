add_library(synapse INTERFACE)
target_include_directories(synapse INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}" "${thrust_INCLUDE_DIRS}" "${xassert_INCLUDE_DIRS}")