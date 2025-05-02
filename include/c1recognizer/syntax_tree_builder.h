
#ifndef _C1_SYNTAX_TREE_BUILDER_H_
#define _C1_SYNTAX_TREE_BUILDER_H_

#include "c1recognizer/syntax_tree.h"
#include "C1ParserBaseVisitor.h"
#include <c1recognizer/error_reporter.h>
#include <variant>
#include <vector>

namespace c1_recognizer
{
namespace syntax_tree
{

/*
  [variant](https://stanford-cs242.github.io/f19/lectures/03-2-algebraic-data-types#variants)([sum type](https://stanford-cs242.github.io/f19/lectures/03-2-algebraic-data-types#sum-types))

  You can add type of visiting result to std::variant<syntax_tree_node*, vector<...>, ...>
*/
class VisitResult: public std::variant<syntax_tree_node*,
                                      std::vector<syntax_tree_node*>> {
public:
  VisitResult(syntax_tree_node* opnd): std::variant<syntax_tree_node*, std::vector<syntax_tree_node*>>(opnd) {}
  VisitResult(std::vector<syntax_tree_node*> opnd): std::variant<syntax_tree_node*, std::vector<syntax_tree_node*>>(opnd) {}

  /**
    as vector or other types you defined
   */
  template<typename T>
  T as() {
    return std::get<T>(*this);
  }
  
  /**
    cast to syntax_tree_node* and then cast to the subtype
   */
  template<typename T>
  T as_node() {
    return dynamic_cast<T>(std::get<syntax_tree_node*>(*this));
  }
};

class syntax_tree_builder : public C1ParserBaseVisitor
{
  public:
    syntax_tree_builder(error_reporter &_err);

    virtual antlrcpp::Any visitCompilationUnit(C1Parser::CompilationUnitContext *ctx) override;
    virtual antlrcpp::Any visitDecl(C1Parser::DeclContext *ctx) override;
    virtual antlrcpp::Any visitConstdecl(C1Parser::ConstdeclContext *ctx) override;
    virtual antlrcpp::Any visitConstdef(C1Parser::ConstdefContext *ctx) override;
    virtual antlrcpp::Any visitVardecl(C1Parser::VardeclContext *ctx) override;
    virtual antlrcpp::Any visitVardef(C1Parser::VardefContext *ctx) override;
    virtual antlrcpp::Any visitFuncdef(C1Parser::FuncdefContext *ctx) override;
    virtual antlrcpp::Any visitBlock(C1Parser::BlockContext *ctx) override;
    virtual antlrcpp::Any visitStmt(C1Parser::StmtContext *ctx) override;
    virtual antlrcpp::Any visitLval(C1Parser::LvalContext *ctx) override;
    virtual antlrcpp::Any visitCond(C1Parser::CondContext *ctx) override;
    virtual antlrcpp::Any visitExp(C1Parser::ExpContext *ctx) override;
    virtual antlrcpp::Any visitNumber(C1Parser::NumberContext *ctx) override;

    VisitResult operator()(antlr4::tree::ParseTree *ctx);

  private:
    error_reporter &err;
};
}
}

#endif
