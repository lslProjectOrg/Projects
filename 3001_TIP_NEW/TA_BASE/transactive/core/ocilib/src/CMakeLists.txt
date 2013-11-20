include_directories(${PROJECT_SOURCE_DIR}/oci/include)
aux_source_directory(. DIR_SRCS)
add_library(ocilib ${DIR_SRCS})
target_link_libraries(ocilib libclntsh)
target_link_libraries(ocilib libnnz10)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set_target_properties(ocilib PROPERTIES OUTPUT_NAME "ocilib")
message("=========src==============")
message(${PROJECT_SOURCE_DIR})
message(${PROJECT_BINARY_DIR})
message(${DIR_SRCS})



