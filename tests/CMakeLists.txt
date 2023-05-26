set(test_name dump_tokens_test)

add_executable(${test_name})

include(GoogleTest)
include(CompileOptions)
target_set_compile_options(${test_name})

target_sources(
  ${test_name}
  PRIVATE
    dump_tokens.test.cpp
)

target_include_directories(
	${test_name}
	PUBLIC
		${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
  ${test_name}
  PRIVATE
    libc
    GTest::gtest_main
)

gtest_discover_tests(${test_name})
