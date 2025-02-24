# modified from https://github.com/antlr/antlr4/blob/4.13.1/runtime/Cpp/runtime/CMakeLists.txt#L23
file(GLOB libantlrcpp_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/atn/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/dfa/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/internal/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/misc/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/support/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tree/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tree/pattern/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tree/xpath/*.cpp"
)

add_library(ANTLRRuntime ${libantlrcpp_SRC})
