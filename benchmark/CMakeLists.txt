add_executable(matrix_chain_benchmark
			   bench.cpp)

target_compile_options(matrix_chain_benchmark PRIVATE -O3)

target_link_libraries(matrix_chain_benchmark
PRIVATE
    benchmark::benchmark
)

target_include_directories(matrix_chain_benchmark PUBLIC ${PROJECT_SOURCE_DIR}/includes)
target_include_directories(matrix_chain_benchmark PUBLIC ${PROJECT_SOURCE_DIR}/src)