set(target_name c-compiler)

add_executable(${target_name})

include(CompileOptions)
target_set_compile_options(${target_name})

target_sources(${target_name} PRIVATE main.cpp)

target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${target_name}
    PRIVATE
    libc
    CLexer
    fmt
    cxxopts
)
