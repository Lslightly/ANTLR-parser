
#include <any>
#include <c1recognizer/syntax_tree_builder.h>
#include <memory>
#include <stdexcept>

using namespace c1_recognizer;
using namespace c1_recognizer::syntax_tree;

// You should use std::any_cast instead of dynamic_cast or static_cast
template<typename T>
bool is(std::any operand) {
    return std::any_cast<T>(&operand) != nullptr;
}

// Return pointer to the value of std::any object
template<typename T>
auto as(std::any operand) {
    auto result = std::any_cast<T>(&operand);
    if (result == nullptr) {
        throw std::runtime_error(std::string("bad cast from ") + operand.type().name() + " to " + typeid(T).name());
    }
    return *result;
}

syntax_tree_builder::syntax_tree_builder(error_reporter &_err) : err(_err) {}

antlrcpp::Any syntax_tree_builder::visitCompilationUnit(C1Parser::CompilationUnitContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitDecl(C1Parser::DeclContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitConstdecl(C1Parser::ConstdeclContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitConstdef(C1Parser::ConstdefContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitVardecl(C1Parser::VardeclContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitVardef(C1Parser::VardefContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitFuncdef(C1Parser::FuncdefContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitBlock(C1Parser::BlockContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitStmt(C1Parser::StmtContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitLval(C1Parser::LvalContext *ctx)
{
}

antlrcpp::Any syntax_tree_builder::visitCond(C1Parser::CondContext *ctx)
{
}

// Returns antlrcpp::Any, which is std::any.
// You should be sure you use the same type for packing and depacking the `Any` object.
// This function always returns an `Any` object containing a `expr_syntax *`.
antlrcpp::Any syntax_tree_builder::visitExp(C1Parser::ExpContext *ctx)
{
    // Get all sub-contexts of type `exp`.
    auto expressions = ctx->exp();
    // Two sub-expressions presented: this indicates it's a expression of binary operator, aka `binop`.
    if (expressions.size() == 2)
    {
        auto result = new binop_expr_syntax;
        // Set line and pos.
        result->line = ctx->getStart()->getLine();
        result->pos = ctx->getStart()->getCharPositionInLine();
        // visit(some context) is equivalent to calling corresponding visit method; dispatching is done automatically
        // by ANTLR4 runtime. For this case, it's equivalent to visitExp(expressions[0]).
        // Use reset to set a new pointer to a std::shared_ptr object. DO NOT use assignment; it won't work.
        // Use `as<Type>(std::any)' to get value from antlrcpp::Any object; notice that this Type must match the type used in
        // constructing the Any object, which is constructed from (usually pointer to some derived class of
        // syntax_node, in this case) returning value of the visit call.
        result->lhs.reset(as<expr_syntax *>(visit(expressions[0])));
        // Check if each token exists.
        // Returnd value of the calling will be nullptr (aka NULL in C) if it isn't there; otherwise non-null pointer.
        if (ctx->Plus())
            result->op = binop::plus;
        if (ctx->Minus())
            result->op = binop::minus;
        if (ctx->Multiply())
            result->op = binop::multiply;
        if (ctx->Divide())
            result->op = binop::divide;
        if (ctx->Modulo())
            result->op = binop::modulo;
        result->rhs.reset(as<expr_syntax *>(visit(expressions[1])));
        return static_cast<expr_syntax *>(result);
    }
    // Otherwise, if `+` or `-` presented, it'll be a `unaryop_expr_syntax`.
    if (ctx->Plus() || ctx->Minus())
    {
        auto result = new unaryop_expr_syntax;
        result->line = ctx->getStart()->getLine();
        result->pos = ctx->getStart()->getCharPositionInLine();
        if (ctx->Plus())
            result->op = unaryop::plus;
        if (ctx->Minus())
            result->op = unaryop::minus;
        result->rhs.reset(as<expr_syntax *>(visit(expressions[0])));
        return static_cast<expr_syntax *>(result);
    }
    // In the case that `(` exists as a child, this is an expression like `'(' expressions[0] ')'`.
    if (ctx->LeftParen())
        return visit(expressions[0]); // Any already holds expr_syntax* here, no need for dispatch and re-patch with casting.
    // If `number` exists as a child, we can say it's a literal integer expression.
    if (auto number = ctx->number())
        return visit(number);
}

antlrcpp::Any syntax_tree_builder::visitNumber(C1Parser::NumberContext *ctx)
{
    auto result = new literal_syntax;
    if (auto intConst = ctx->IntConst())
    {
        result->is_int = true;
        result->line = intConst->getSymbol()->getLine();
        result->pos = intConst->getSymbol()->getCharPositionInLine();
        auto text = intConst->getSymbol()->getText();
        if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) // Hexadecimal
            result->intConst = std::stoi(text, nullptr, 16); // std::stoi will eat '0x'
        /* you need to add other situations here */
        
        return static_cast<expr_syntax *>(result);
    }
    // else FloatConst
    else
    {
        return static_cast<expr_syntax *>(result);
    }
}

ptr<syntax_tree_node> syntax_tree_builder::operator()(antlr4::tree::ParseTree *ctx)
{
    auto result = visit(ctx);
    if (is<syntax_tree_node *>(result))
        return ptr<syntax_tree_node>(as<syntax_tree_node *>(result));
    if (is<assembly *>(result))
        return ptr<syntax_tree_node>(as<assembly *>(result));
    if (is<global_def_syntax *>(result))
        return ptr<syntax_tree_node>(as<global_def_syntax *>(result));
    if (is<func_def_syntax *>(result))
        return ptr<syntax_tree_node>(as<func_def_syntax *>(result));
    if (is<cond_syntax *>(result))
        return ptr<syntax_tree_node>(as<cond_syntax *>(result));
    if (is<expr_syntax *>(result))
        return ptr<syntax_tree_node>(as<expr_syntax *>(result));
    if (is<binop_expr_syntax *>(result))
        return ptr<syntax_tree_node>(as<binop_expr_syntax *>(result));
    if (is<unaryop_expr_syntax *>(result))
        return ptr<syntax_tree_node>(as<unaryop_expr_syntax *>(result));
    if (is<lval_syntax *>(result))
        return ptr<syntax_tree_node>(as<lval_syntax *>(result));
    if (is<literal_syntax *>(result))
        return ptr<syntax_tree_node>(as<literal_syntax *>(result));
    if (is<stmt_syntax *>(result))
        return ptr<syntax_tree_node>(as<stmt_syntax *>(result));
    if (is<var_def_stmt_syntax *>(result))
        return ptr<syntax_tree_node>(as<var_def_stmt_syntax *>(result));
    if (is<assign_stmt_syntax *>(result))
        return ptr<syntax_tree_node>(as<assign_stmt_syntax *>(result));
    if (is<func_call_stmt_syntax *>(result))
        return ptr<syntax_tree_node>(as<func_call_stmt_syntax *>(result));
    if (is<block_syntax *>(result))
        return ptr<syntax_tree_node>(as<block_syntax *>(result));
    if (is<if_stmt_syntax *>(result))
        return ptr<syntax_tree_node>(as<if_stmt_syntax *>(result));
    if (is<while_stmt_syntax *>(result))
        return ptr<syntax_tree_node>(as<while_stmt_syntax *>(result));
    return nullptr;
}
