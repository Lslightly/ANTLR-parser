#!/bin/bash -x
# use /bin/bash -x to show the output of shell
# change to your /full/path/to/antlr-4.13.1-complete.jar
export LD_LIBRARY_PATH='.:/usr/local/lib:$LD_LIBRARY_PATH'
export CLASSPATH='.:/usr/local/lib/antlr-4.13.1-complete.jar:$CLASSPATH'
antlr4='java org.antlr.v4.Tool'
grun='java org.antlr.v4.gui.TestRig'

if [[ $# != 1 ]]; then
    echo "Usage: ./run.sh java|cpp"
    echo "Example: cat test/test_cases/simple.c1 | ./run.sh java"
fi

rm -rf build
mkdir -p build

if [[ $1 == "java" ]]; then
    cd grammar
    $antlr4 C1Lexer.g4 -o ../build
    cd ../build
    javac *.java
    $grun C1Lexer tokens -tokens
elif [[ $1 == "cpp" ]]; then
    cd grammar
    $antlr4 C1Lexer.g4 -Dlanguage=Cpp -o ../build
    cd ../build
    # uncomment below if you run this file on your machine
    sudo cp ../Libs_for_c1r_ref/libantlr4-runtime.so.4.13.1 /usr/local/lib/
    sudo ln -snf /usr/local/lib/libantlr4-runtime.so.4.13.1 /usr/local/lib/libantlr4-runtime.so

    g++ -std=c++17 ../test/lexer.cpp C1Lexer.cpp -I../include/antlr4-runtime-4.13.1 -I. -lantlr4-runtime -o c1lexer
    # Generate compile_commands.json for clangd language server.
    # See https://clang.llvm.org/docs/JSONCompilationDatabase.html for the specification of compile_commands.json 
    # See why compile_commands.json takes effect in https://clangd.llvm.org/installation#project-setup
    # You may use Ctrl+Shift+P in VSCode and input `clangd: Restart language server` to make the following setup take effect.
    cat << EOF > compile_commands.json 
[
    {
        "directory": "$(pwd)",
        "arguments": ["g++", "-std=c++17", "../test/lexer.cpp", "C1Lexer.cpp", "-I../include/antlr4-runtime-4.13.1", "-I.", "-lantlr4-runtime", "-o", "c1lexer"],
        "file": "../test/lexer.cpp"
    },
    {
        "directory": "$(pwd)",
        "arguments": ["g++", "-std=c++17", "../test/lexer.cpp", "C1Lexer.cpp", "-I../include/antlr4-runtime-4.13.1", "-I.", "-lantlr4-runtime", "-o", "c1lexer"],
        "file": "C1Lexer.cpp"
    }
]
EOF
    ./c1lexer
fi

