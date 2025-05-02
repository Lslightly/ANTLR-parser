[TOC]

---

#### 任务描述

本关任务: 理解 ANTLR 的分析树（Parse Tree）结构和编程接口，学习并运用访问者（Visitor）设计模式来构造由分析树生成语法树的 Visitor。请：
1. 理解版本库的整体结构以及编译和运行流程；
2. 理解 Parse Tree 编程接口和 Visitor 模式，通过编写对 Parse Tree 的 Visitor 来构建 AST；
3. 完整地编译和测试能生成 AST 的 C1 解析器。

##### 任务分解与提交要求

1. 了解本实训项目的完整编译和运行方法，编译你的项目并`doc/labAST.md`文件中记录编译过程中遇到的问题与解决方案。
2. 阅读 **分析树（Parse Tree）** 和 **设计模式（Design Pattern）**两小节内容，以理解 Visitor Pattern 及其在 ANTLR 生成的解析器中的应用特点。并学习编写 Parse Tree 的 Visitor 来构建 AST，在`c1recognizer/doc/labAST.md`中总结这些接口与你在[关卡3](./part3.md)中所写文法之间的关系。
3. 为了从 Parse Tree 生成 AST，相关的数据结构位于`include/c1recognizer/syntax_tree.h`。你需要在`src/syntax_tree_builder.cpp`中补充代码，以实现对 AST 的构建。

 在`syntax_tree_builder.cpp`已提供的初始代码中，你需要对其中形如`visitSomeContext`的函数补充相应的实现代码。请确保你理解了以下内容，再开展实际的代码工作：
   - 函数`antlrcpp::Any syntax_tree_builder::visitExp(C1Parser::ExpContext *ctx)`的实现，注意理解`C1Parser::ExpContext`、`result`的含义、与文法的对应关系、使用的编程接口及其含义，查阅`antlrcpp::Any`、访问者模式相关的源码；
   - 函数`ptr<syntax_tree_node> syntax_tree_builder::operator()(antlr4::tree::ParseTree *ctx)`的实现，注意理解其中的`result`、`as<Type>(result)`；
   - 在理解之后，分析 C1 的 Parse Tree 结构，实现通过访问 Parse Tree 来构建 AST。

5. 修改并完善以下文件，使得项目编译后生成的`c1r_test`可执行程序能够解析输入的`c1`文件，并输出其 AST 结构。请确保你在本关卡最终的提交中没有修改除下面四个文件外的任何项目文件，在提交前通过`git status`检查提交涉及的文件将会是一个很好的方法。
  - 将`recognizer.cpp`中解析的起始符号从[`exp`](../src/recognizer.cpp#L45)修改为`compilationUnit`，这样你可以让解析器返回`compilationUnit`为根结点的解析树，否则解析器将返回`exp`为根结点的解析树。你可以利用这一点，基于测试逐步迭代你的开发。
  - 完善`syntax_tree_builder.cpp`中的空白方法。
  - 不建议，如有必要可以调整`C1Lexer.g4`和`C1Parser.g4`，但是你还需要额外修改一些文件，例如`syntax_tree_builder.h`中的`syntax_tree_builder`类和其他的一些函数。
  - 在`doc/labAST.md`文件中，给出实现与调试过程中你所遇到的问题与解决方案。

```
c1recognizer/grammer/C1Lexer.g4
c1recognizer/grammer/C1Parser.g4
c1recognizer/src/recognizer.cpp
c1recognizer/scr/syntax_tree_builder.cpp
```

1. **构建与测试**

使用VSCode的同学推荐使用插件[clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)。助教已经在CMakeLists.txt中配置好了相关选项，并添加了`.clangd`的配置文件。在编写好C1Parser.g4的文法文件，使用下述命令make，会在build文件夹下生成compile_commands.json文件。重启vscode之后就可以有自动补全等功能。

> 由于vscode-clangd在文档支持方面还存在[问题](https://github.com/clangd/clangd/issues/529)，你也可以使用C/C++插件，并在`c_cpp_properties.json`中配置 `compileCommands`路径。

第一次构建时，运行如下命令：

```bash
sudo cp Libs_for_c1r_ref/libantlr4-runtime.so.4.13.1 /usr/local/lib
sudo ln -snf /usr/local/lib/libantlr4-runtime.so.4.13.1 /usr/local/lib/libantlr4-runtime.so
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DANTLR_EXECUTABLE=/path/to/your/antlr.jar ..
make
```
会在build文件夹下生成可执行文件c1r_test。之后如有修改无需重新构建，只需要执行```make -j```即可。

使用你在前面几关编写的测试用例对`c1r_test`程序进行测试。例如:

```bash
cd build 
./c1r_test < ../test/demo/simple.c1
```

可以看到输出的语法树如[simple_ast.json](simple_ast.json)所示。

> 注意每个结点的line, pos都是该语法规则第一个token的line和start position。

  在你的测试用例下生成的 AST 是正确的情况下，可以使用版本库中名为`c1r_ref_static`的二进制文件作为参考来查看生成的语法树。使用命令 ```./c1r_ref_static < testfile```。

所有环境搭建与命令相关都有在代码库中的README.md中涉及，如有问题，请先查看文档。

##### 注意事项

- 最后的检查将通过严格比较参考的二进制文件`c1r_ref_static`输出和你的输出来进行评测，请确保你的输出和二进制文件输出一致；

##### 问题清单

**AST-Q0 项目结构**

分析本实训项目结构，将分析结果记录在`doc/labAST.md`文件中，分析内容至少需要包括以下几项内容：
   - 对项目编译流程的理解，主要是对`CMakeLists.txt`文件内容的理解；
   - 对项目中的文件组织及结构的理解，说明各个文件的主要用途；
   - 项目依赖的外部组件有哪些，这些组件的用途是什么；
   - 项目由哪些主要的功能模块组成，描述模块间的交互关系。

**AST-Q1 理解访问者**

- 请结合`antlrcpp::Any syntax_tree_builder::visitExp(C1Parser::ExpContext *ctx)`的实现 ，说明 `antlrcpp::Any`的定义位置及其意义，说明`C1Parser::ExpContext`、`result`的含义、以及它们与文法的对应关系、使用的编程接口及其含义；

- 请结合函数`ptr<syntax_tree_node> syntax_tree_builder::operator()(antlr4::tree::ParseTree *ctx)`的实现，说明其中的`result`、`as<Type>(result)`的含义；

- 请结合本关实验，阐述 Visitor 的实际运作机制；

- 在`doc/labAST.md`文件中回答上述问题。

#### 基本知识

##### Parse Tree 的结构

以`grammar/C1Parser.g4`生成的`C1Parser.h`（路径为`build/antlr4cpp_generated_src/C1Parser/C1Parser.h`）为例，这里截取部分代码，并在旁边添加了注释说明。

```cpp
class  C1Parser : public antlr4::Parser {
public:
  enum {
    Comma = 1, SemiColon = 2, Assign = 3, LeftBracket = 4, RightBracket = 5, 
    // ...
  };

  enum {
    RuleCompilationUnit = 0, RuleDecl = 1, RuleConstdecl = 2, RuleConstdef = 3, 
    // ...
  };

  //...
  // `ExpContext`是`Parse Tree`中代表`exp`非终结符的节点，其它命名中包含`Context`的类也是类似的
  class  ExpContext : public antlr4::ParserRuleContext {
  public:
    ExpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    
    // 返回该节点的子结构中代表`exp`非终结符的全部节点
    std::vector<ExpContext *> exp();
    // 返回该节点的子结构中代表`exp`非终结符的第i个节点
    ExpContext* exp(size_t i);
    // 生成这些接口的原因在于，`exp`的产生式规则中的右部(`right hand side, RHS`)可能存在多个`exp`非终结符，作为当前节点的子结构。如果最多只有一个`exp`子结构，生成的接口会形如`ExpContext* exp();`，返回唯一一个这样的孩子节点。如果不存在`exp`子结构，则会返回`nullptr`，即`C`语言中的`NULL`。

    // Plus终结符节点，若不存在，返回的是`nullptr`
    antlr4::tree::TerminalNode *Plus();
    // 类似于Plus
    antlr4::tree::TerminalNode *Minus();
    // ...
    // 如果你需要从一个`TerminalNode`指针`p`获取其匹配的原文文本，你应使用`p->getSymbol()->getText()`。这在分析处理`Identifier`和`Number`时会有用。

    // 虚方法 进入该`exp`语法结构时调用参数`listener`提供的实现来进行相应的处理
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    // 虚方法 退出该`exp`语法结构时调用参数`listener`提供的实现来进行相应的处理
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    // 调用参数`visitor`提供的实现来访问该`exp`结构。
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExpContext* exp();
};
```

#### 设计模式（Design Pattern）

`Design Pattern`（设计模式）是软件开发人员在软件开发过程中面临的一般问题的解决方案，是一套被反复使用、经过分类的、代码设计经验的总结。使用设计模式的目的是为了代码的可重用性、让代码更容易被他人理解及保证代码可靠性等。

介绍 Design Pattern 的著名书籍是《 Design Patterns - Elements of Reusable Object-Oriented Software 》（中文译名：设计模式-可复用的面向对象软件元素），不过该书并不太容易理解。你可以阅读网上的 [教程](https://book.douban.com/subject/1052241/) 来理解，为开展本实训，你需要学习 [Factory Pattern](http://www.runoob.com/design-pattern/factory-pattern.html) 以及下面的 [Visitor Pattern](http://www.runoob.com/design-pattern/visitor-pattern.html)。

##### 访问者模式（Visitor Pattern）

Visitor Pattern （访问者模式）是为了解决稳定的数据结构和易变的操作之间的耦合问题而产生的一种设计模式。解决方法就是在被访问的类里面加一个对外提供接待访问者的接口，其关键在于：**在数据基础类里有一个接受访问者的方法，该方法将自身的对象引用传入访问者。**

这里举一个应用实例来帮助理解访问者模式。例如，您在朋友家做客，您是访问者，朋友接受您的访问，您通过朋友的描述，然后对朋友的描述做出一个判断，这就是访问者模式。

有关 Visitor Pattern 的含义、模式和特点可参见 [中文维基百科](https://zh.wikipedia.org/wiki/访问者模式) 和 [英文维基百科](https://en.wikipedia.org/wiki/Visitor_pattern)，其中 [C++ 的示例](https://en.wikipedia.org/wiki/Visitor_pattern#C.2B.2B_example) 值得一看。

##### Visitor Pattern 在本关实验的应用

下面以 C1 中`exp`（表达式）为例来解释 Visitor Pattern。在[`syntax_tree_builder.cpp`](../src/syntax_tree_builder.cpp)中的[`visitExp`](../src/syntax_tree_builder.cpp#L77) 给出了分析 Parse Tree 中的表达式节点 `C1Parser::ExpContext`，构建对应的 AST 的实现。

下面代码给出了当表达式是`number`时，会间接调用`visit`(`number`) 来分析`number`语法结构并为之构造 AST 节点，并返回一个`VisitResult`。

```cpp
// syntax_tree_builder.cpp
VisitResult syntax_tree_builder::visitExp(C1Parser::ExpContext *ctx)
{
  ...
    if (auto number = ctx->number())
        return (*this)(number); // (*this)() 会调用operator()函数
  ...
}
VisitResult syntax_tree_builder::operator()(antlr4::tree::ParseTree *ctx)
{
    auto res = visit(ctx);
    return std::any_cast<VisitResult>(res);
}
```

这里，类[`syntax_tree_builder`](../include/c1recognizer/syntax_tree_builder.h#L42) 是 [ANTLR v4](http://www.antlr.org/) 根据 [`C1Parser.g4`](../grammar/C1Parser.g4) 自动生成的`C1ParserBaseVisitor`类的子类。`C1ParserBaseVisitor`是 [ANTLR v4](http://www.antlr.org/) 为类 `C1ParserVisitor`提供的一个空的实现，用户可以从它派生自己的访问者类，来根据需要实现其中的部分或全部方法。`C1Parser::ExpContext`的代码片段见上面 [Parser Tree 的结构](#parse-tree-的结构)。

在 [ANTLR v4](http://www.antlr.org/) 生成的`C1Parser.cpp`中`C1Parser::ExpContext`的`accept()`方法的如下实现：

```cpp
// C1Parser.cpp
antlrcpp::Any C1Parser::ExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<C1ParserVisitor*>(visitor))
    return parserVisitor->visitNumber(this); // <-这里
  else
    return visitor->visitChildren(this);
}
```

> 其参数类型`tree::ParseTreeVisitor`的定义位于 [ANTLR v4](http://www.antlr.org/) 的[`ParseTreeVisitor.h`](../src/antlr4-runtime-4.13.1/tree/ParseTreeVisitor.h#L21)中。[ANTLR v4](http://www.antlr.org/) 还提供了[`AbstractParseTreeVisitor.h`](../src/antlr4-runtime-4.13.1/tree/AbstractParseTreeVisitor.h#L14)，为类`ParseTreeVisitor`提供了一个缺省的实现类`AbstractParseTreeVisitor`。

从`ExpContext::accept()`的实现中可以看到，它主要是根据`exp`的语法构成，调用访问者对象提供的`visitXXX()`去访问其子结构`XXX`。而`AbstractParseTreeVisitor`实现类中的`antlrcpp::Any visit(ParseTree *tree)`（代码如下）又会调用`accept()`方法来应用访问者`AbstractParseTreeVisitor`处理当前的 Parse Tree 结构。

```cpp
// AbstractParseTreeVisitor.h
namespace antlr4 {
namespace tree {

  class ANTLR4CPP_PUBLIC AbstractParseTreeVisitor : public ParseTreeVisitor {
  public:
    virtual antlrcpp::Any visit(ParseTree *tree) override {
      return tree->accept(this); // <-这里
    }
  ...
  }
}
}
```

综上，`node->accept(visitor)`和`visitor->visitXXX(node)`表明了树和对树进行访问处理的访问者之间的关系, 其中`node`是`tree`中的节点，`XXX`是`node`的子结构。


利用这样的访问者模式，用户可以定义不同的`tree`结构，为`tree`中不同类别的`node`分别实现一个`accept(visitor)`方法。而对`tree`的访问处理，则可以根据不同需求定义不同的`visitor`类，在其中为类`XXX`的`node`实现相应的`visitXXX(node)`方法。

##### Listener模式(了解)

在 [ANTLR v4](http://www.antlr.org/) 生成的`C1Parser.cpp`中还定义了 `enterRule` 和 `exitRule` 两个方法。

```cpp
void C1Parser::ExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<C1ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExp(this);
}

void C1Parser::ExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<C1ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExp(this);
}
```

这两个方法为 `ParseTreeListener` 服务，分别在进入结点前和退出结点后发挥作用。这两个函数被调用的代码逻辑见 [`ParseTreeWalker::walk`](../src/antlr4-runtime-4.13.1/tree/ParseTreeWalker.cpp#L31)

```cpp
  enterRule(listener, t);
  for (auto &child : t->children) {
    walk(listener, child);
  }
  exitRule(listener, t);
```

### 你可能使用的API

`TerminalNode`类。TerminalNode是终结符结点基类。
- `Token* getSymbol()`可以返回一个`Token`实例，包含C1Lexer.g4中定义的终结符的相关信息。

`Token`类，
- `std::string getText()`可以获取原始文本。
- `size_t getLine()`可以获取终结符所在行。
- `size_t getCharPositionInLine()`可以获取终结符的第一个字符所在的列数。
- `size_t getTokenIndex()`可以获取这个token在所有token流中的下标，从0开始。

`ParserRuleContext`类，全部非终结符`XXXContext`的基类。
- `Token* getStart()`用于获取语法规则中的第一个token。

