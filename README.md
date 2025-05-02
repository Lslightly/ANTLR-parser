# 使用 ANTLR-4.13.1 为 C1 语言构造分析器

在本实验中，你将学习并逐步掌握如何为一个编程语言构建一个能生成语法树的分析器。你将通过编写描述语言文法的描述文件，使用 [ANTLR v4](http://www.antlr.org/) 生成相应的分析器源码，该分析器能为合法的输入程序输出对应的分析树(`parse tree`)。然后，你会继续编写为分析树生成相应抽象语法树(`abstract syntax tree, AST`)的访问者(`visitor`)，从而最终实现一个能为合法的输入程序输出 AST 的分析器。

本实训要实现的编程语言是 C1 语言，你需要先阅读 **第 1 关 预备知识：C1 语言** 来了解 C1 语言的特点，尝试编写一些 C1 程序，再利用现有的 C 语言编译器来编译。然后，你将通过逐级递进的闯关任务，逐步完成对 C1 语言分析器的构造，它将为正确的 C1 程序输出对应的语法分析树。

## 版本库结构

```
.
├── CMakeLists.txt                  # c1recognizer项目的cmake脚本
├── Doxyfile                        # doxygen配置文件
├── README.md
├── build
│   ├── antlr4cpp_generated_src     # 由grammar生成的ANTLR解析器文件
│   ├── c1r_test                    # c1recognizer
│   ├── compile_commands.json       # VSCode Clangd编译数据库
├── c1r_ref_static                  # 参考编译器
├── cmake
│   └── FindANTLR.cmake             # 定义ANTLR宏
├── doc                             # 关卡文档
├── explore                         # 从源码构建ANTLR，探索ANTLR
│   └── antlrcpp-explore.sh
├── grammar                         # !!!存放文法文件
│   ├── C1Lexer.g4
│   └── C1Parser.g4
├── include                         # !!!
│   ├── c1recognizer                # c1recognizer头文件
│   ├── rapidjson                   # rapidjson运行时头文件
│   └── syntax_tree_serializer.hpp  # 生成序列化树的头文件
├── run_lexer.sh                    # 执行lexer的脚本
├── src                             # !!!
│   ├── antlr4-runtime-4.13.1       # ANTLR源代码，已通过CMakeLists.txt集成到该项目中
│   ├── error_listener.cpp          # 语法错误Listener
│   ├── error_reporter.cpp          # 语法错误Reporter
│   ├── lexer.cpp                   # lexer驱动程序
│   ├── main.cpp                    # c1recognizer驱动程序
│   ├── recognizer.cpp              # 语法树生成器
│   ├── syntax_tree.cpp
│   └── syntax_tree_builder.cpp     # 构造语法树
├── test                            # !!!阶段性测例
│   ├── exp
│   └── incomplete_cases
```

你需要在上述标记有`!!!`的目录中增加或修改文件，来完成本实训的实践任务。

在每个实践任务中，会有进一步的实践任务检查说明，助教会借助脚本实现对实践任务的半自动检查，请一定严格按照各实践任务的要求组织实训项目的目录和文件，否则会影响本实训的成绩。

## 环境配置

> 头歌平台已配置好实训项目所需的各种软件，其中`antlr-4.13.1-complete.jar`在`/usr/local/lib`目录下：

**本地实验的环境配置方法**

**1. Java环境安装**

ANTLR 工具需要 JVM 才能执行，此外为了方便使用 ANTLR 的`grun`，你需要一个能够编译`java`源文件的环境。因此，你需要一个完整的`Java Development Kit`。

  - 如果你使用 Linux，推荐通过包管理器安装 OpenJDK 21。在你的包管理器中通过搜索来确定包名，如`Ubuntu`下包名为`openjdk-21-jdk`，安装之即可；

  - 如果你使用 Mac，你需要手动安装一个 JDK。

**2. ANTLR下载与安装**
  - 你需要从[`ANTLR Download`](https://github.com/antlr/website-antlr4/tree/gh-pages/download)下载`antlr-4.13.1-complete.jar`；

  - 你需要将该`jar`包的存放路径加入到环境变量`CLASSPATH`中，即可以在`Bash`中执行`export CLASSPATH=".:/path/to/your/antlr-4.13.1-complete.jar:$CLASSPATH"`；

  - 你可以考虑将这一命令加入`.bashrc`（对于`Bash`），以省去你每次配置的麻烦。

**3. 定义`antlr4`和`grun`工具**

  - 可以定义别名`antlr4`表示 ANTLR 工具，即`alias antlr4='java org.antlr.v4.Tool'`；

  - `grun`本质上是一个别名，可以定义如下：`alias grun='java org.antlr.v4.runtime.misc.TestRig'`或`alias grun='java org.antlr.v4.gui.TestRig'`；

  - 同样的，你可以将这些别名命令加入到`.bashrc`，以节省你配置和使用的时间。

四个关卡的文档在[doc](./doc/)目录下。

## 参考

引用的外部库：
- [`rapidjson`](https://github.com/Tencent/rapidjson)
- [`antlr4-runtime(cpp)`](https://github.com/antlr/antlr4/tree/master/runtime/Cpp)。
