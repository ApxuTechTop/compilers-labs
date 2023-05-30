set(lib_name libc)

add_library(${lib_name})

include(CompileOptions)
target_set_compile_options(${lib_name})

target_include_directories(${lib_name} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_sources(
  ${lib_name}
  PRIVATE
    dump_tokens.cpp
    ast/Ast.cpp
    ast/XmlSerializer.cpp
    ast/detail/Builder.cpp
    parser.cpp
)

target_link_libraries(
  ${lib_name}
  PRIVATE
    C
    fmt
    pugixml
)


