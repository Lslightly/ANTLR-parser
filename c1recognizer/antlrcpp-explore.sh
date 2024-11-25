#!/bin/bash -x
# bash script for setting up environment to explore ANTLR

# set ANTLR_LOCATION environment
ANTLR_LOCATION=/usr/local/lib/antlr-4.13.1-complete.jar # you can change the path

# clone ANTLR source code
cd ../../ # download antlr4 source code in another directory.
git clone --branch 4.13.1 https://github.com/antlr/antlr4.git
cd antlr4/runtime/Cpp

# generate .clangd for clangd language server(need to install clangd VSCode extension).
# See https://clangd.llvm.org/config#files and https://clangd.llvm.org/config#compilationdatabase for reasons
cat << EOF > .clangd
CompileFlags:
  CompilationDatabase: build/       # Search build/ directory for compile_commands.json
EOF

# build ANTLR source code
# see the guide https://github.com/antlr/antlr4/tree/811b7fda58bd14d7f0abc496b6fd651dfa01ed97/runtime/Cpp#compiling-on-linux for details.
# The following script is a modified version of the instructions in the guide.
rm -rf build run
mkdir build && mkdir run && cd build
# The next command is `cmake .. -DANTLR_JAR_LOCATION=$ANTLR_LOCATION -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DANTLR_BUILD_CPP_TESTS=0`
# use DANTLR_BUILD_CPP_TESTS to disable downloading googletest because of restricted network issues. See https://github.com/antlr/antlr4/blob/7ed420ff2c78d62883875c442d75f32e73bc86c8/runtime/Cpp/runtime/CMakeLists.txt#L64
# use CMAKE_EXPORT_COMPILE_COMMANDS=1 to generate compile_commands.json. See https://clangd.llvm.org/installation#compile_commandsjson
cmake .. -DANTLR_JAR_LOCATION=$ANTLR_LOCATION -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DANTLR_BUILD_CPP_TESTS=0
make -j 4
# Now you can use VSCode to open the directory ../../antlr4/runtime/Cpp to see the source code.
