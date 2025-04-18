cmake_minimum_required(VERSION 3.10)

project(ANTLR-parser)

project(c1recognizer)

add_definitions(-DFULL_IMPL)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
       PROPERTY EP_UPDATE_DISCONNECTED 1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# c++ standard >= 17
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# set variable pointing to the antlr tool that supports C++
set(ANTLR_EXECUTABLE "NOT-FOUND" CACHE STRING "ANTLR v4 JAR file location. Used by antlr4cpp.")

# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(C1Lexer ${CMAKE_CURRENT_SOURCE_DIR}/grammar/C1Lexer.g4 LEXER)
antlr_target(C1Parser ${CMAKE_CURRENT_SOURCE_DIR}/grammar/C1Parser.g4 PARSER LISTENER VISITOR
             DEPENDS_ANTLR C1Lexer
             COMPILE_FLAGS -lib ${ANTLR_C1Lexer_OUTPUT_DIR})

# include generated files in project environment
include_directories(${ANTLR_C1Lexer_OUTPUT_DIR})
include_directories(${ANTLR_C1Parser_OUTPUT_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# modified from https://github.com/antlr/antlr4/blob/4.13.1/runtime/Cpp/runtime/CMakeLists.txt#L10
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1
  ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1/atn
  ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1/dfa
  ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1/internal
  ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1/misc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1/support
  ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1/tree
  ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1/tree/pattern
  ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1/tree/xpath
)

# build ANTLRRuntime
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4-runtime-4.13.1)

# add generated grammar to c1_recognizer binary target
add_library(c1recognizer
  src/error_listener.cpp
  src/error_reporter.cpp
  src/syntax_tree.cpp
  src/syntax_tree_builder.cpp
  src/recognizer.cpp
  ${ANTLR_C1Lexer_CXX_OUTPUTS}
  ${ANTLR_C1Parser_CXX_OUTPUTS})

target_link_libraries(c1recognizer ANTLRRuntime)

include_directories(${rapidjson_include_dirs})
add_executable(c1r_test test/main.cpp)
add_dependencies(c1r_test c1recognizer)
target_link_libraries(c1r_test c1recognizer)

install(
  TARGETS c1recognizer
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(DIRECTORY include/c1recognizer DESTINATION include)

configure_file(c1recognizer-config.cmake.in c1recognizer-config.cmake @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/c1recognizer-config.cmake DESTINATION cmake)
