add_executable(matrix_chain_unit_test matrix.cpp chain_tests.cpp)

target_link_libraries(matrix_chain_unit_test
PRIVATE
	GTest::GTest
	GTest::gtest_main
)

target_include_directories(matrix_chain_unit_test PUBLIC ${PROJECT_SOURCE_DIR}/includes)

include(GoogleTest)
gtest_discover_tests(matrix_chain_unit_test)