#ifndef __PDDL__DETAIL__PARSING__EXPRESSIONS_H
#define __PDDL__DETAIL__PARSING__EXPRESSIONS_H

#include <pddl/AST.h>
#include <pddl/Context.h>
#include <pddl/Exception.h>
#include <pddl/detail/ASTContext.h>
#include <pddl/detail/VariableStack.h>
#include <pddl/detail/parsing/VariableDeclaration.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::AndPointer<Argument>> parseAnd(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::AtPointer<Argument>> parseAt(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::EitherPointer<Argument>> parseEither(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename ArgumentLeft, typename ArgumentRight, typename ArgumentLeftParser, typename ArgumentRightParser>
std::experimental::optional<ast::EqualsPointer<ArgumentLeft, ArgumentRight>> parseEquals(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentLeftParser parseArgumentLeft, ArgumentRightParser parseArgumentRight);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::ExistsPointer<Argument>> parseExists(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::ForAllPointer<Argument>> parseForAll(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::ImplyPointer<Argument>> parseImply(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::NotPointer<Argument>> parseNot(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::OrPointer<Argument>> parseOr(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename ArgumentLeft, typename ArgumentRight, typename ArgumentLeftParser, typename ArgumentRightParser>
std::experimental::optional<ast::WhenPointer<ArgumentLeft, ArgumentRight>> parseWhen(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentLeftParser parseArgumentLeft, ArgumentRightParser parseArgumentRight);
/*//Numerical comparison
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::GreaterPointer<Argument>> parseGreater(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::GreaterEqualsPointer<Argument>> parseGreaterEquals(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::SmallerPointer<Argument>> parseSmaller(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::SmallerEqualsPointer<Argument>> parseSmallerEquals(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::AddPointer<Argument>> parseAdd(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::SubtractPointer<Argument>> parseSubtract(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::MultiplyPointer<Argument>> parseMultiply(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::DividePointer<Argument>> parseDivide(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument);
*/
//Numerical Operators

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions: Base Classes
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, typename ArgumentLeftParser, typename ArgumentRightParser>
std::experimental::optional<std::unique_ptr<Derived>> parseBinary(Context &context,
	ASTContext &astContext, VariableStack &variableStack, ArgumentLeftParser parseArgumentLeft,
	ArgumentRightParser parseArgumentRight)
{
	auto &tokenizer = context.tokenizer;

	const auto position = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("(")
		|| !tokenizer.testIdentifierAndSkip(Derived::Identifier))
	{
		tokenizer.seek(position);
		return std::experimental::nullopt;
	}

	tokenizer.skipWhiteSpace();

	// Parse arguments of the expression
	auto argumentLeft = parseArgumentLeft(context, astContext, variableStack);

	if (!argumentLeft)
		throw ParserException(tokenizer.location(), "could not parse argument of “" + std::string(Derived::Identifier) + "” expression");

	auto argumentRight = parseArgumentRight(context, astContext, variableStack);

	if (!argumentRight)
		throw ParserException(tokenizer.location(), "could not parse argument of “" + std::string(Derived::Identifier) + "” expression");

	tokenizer.expect<std::string>(")");

	return std::make_unique<Derived>(std::move(argumentLeft.value()), std::move(argumentRight.value()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, typename ArgumentParser>
std::experimental::optional<std::unique_ptr<Derived>> parseNAry(Context &context,
	ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	auto &tokenizer = context.tokenizer;

	const auto position = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("(")
		|| !tokenizer.testIdentifierAndSkip(Derived::Identifier))
	{
		tokenizer.seek(position);
		return std::experimental::nullopt;
	}

	typename Derived::Arguments arguments;

	tokenizer.skipWhiteSpace();

	// Parse arguments of the expression
	while (tokenizer.currentCharacter() != ')')
	{
		auto argument = parseArgument(context, astContext, variableStack);

		if (!argument)
			throw ParserException(tokenizer.location(), "could not parse argument of “" + std::string(Derived::Identifier) + "” expression");

		arguments.emplace_back(std::move(argument.value()));

		tokenizer.skipWhiteSpace();
	}

	if (arguments.empty())
		context.warningCallback(tokenizer.location(), "“" + std::string(Derived::Identifier) + "” expressions should not be empty");

	tokenizer.expect<std::string>(")");

	return std::make_unique<Derived>(std::move(arguments));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, typename ArgumentParser>
std::experimental::optional<std::unique_ptr<Derived>> parseQuantified(Context &context,
	ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	auto &tokenizer = context.tokenizer;

	const auto position = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("(")
		|| !tokenizer.testIdentifierAndSkip(Derived::Identifier))
	{
		tokenizer.seek(position);
		return std::experimental::nullopt;
	}

	// Parse variable list
	tokenizer.expect<std::string>("(");
	auto parameters = parseVariableDeclarations(context, *astContext.domain);
	tokenizer.expect<std::string>(")");

	// Push newly parsed variables to the stack
	variableStack.push(&parameters);

	// Parse argument of the expression
	auto argument = parseArgument(context, astContext, variableStack);

	if (!argument)
		throw ParserException(tokenizer.location(), "could not parse argument of “" + std::string(Derived::Identifier) + "” expression");

	// Clean up variable stack
	variableStack.pop();

	tokenizer.expect<std::string>(")");

	return std::make_unique<Derived>(std::move(parameters), std::move(argument.value()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::AndPointer<Argument>> parseAnd(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseNAry<ast::And<Argument>, ArgumentParser>(context, astContext, variableStack, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::EitherPointer<Argument>> parseEither(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseNAry<ast::Either<Argument>, ArgumentParser>(context, astContext, variableStack, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ArgumentLeft, typename ArgumentRight, typename ArgumentLeftParser, typename ArgumentRightParser>
std::experimental::optional<ast::EqualsPointer<ArgumentLeft, ArgumentRight>> parseEquals(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentLeftParser parseArgumentLeft, ArgumentRightParser parseArgumentRight)
{
	return parseBinary<ast::Equals<ArgumentLeft, ArgumentRight>, ArgumentLeftParser, ArgumentRightParser>(context, astContext, variableStack, parseArgumentLeft, parseArgumentRight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::ExistsPointer<Argument>> parseExists(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseQuantified<ast::Exists<Argument>, ArgumentParser>(context, astContext, variableStack, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::ForAllPointer<Argument>> parseForAll(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseQuantified<ast::ForAll<Argument>, ArgumentParser>(context, astContext, variableStack, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::ImplyPointer<Argument>> parseImply(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseBinary<ast::Imply<Argument>, ArgumentParser, ArgumentParser>(context, astContext, variableStack, parseArgument, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::NotPointer<Argument>> parseNot(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	auto &tokenizer = context.tokenizer;

	const auto position = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("(")
		|| !tokenizer.testIdentifierAndSkip("not"))
	{
		tokenizer.seek(position);
		return std::experimental::nullopt;
	}

	tokenizer.skipWhiteSpace();

	// Parse argument
	auto argument = parseArgument(context, astContext, variableStack);

	if (!argument)
		throw ParserException(tokenizer.location(), "could not parse argument of “not” expression");

	tokenizer.expect<std::string>(")");

	return std::make_unique<ast::Not<Argument>>(std::move(argument.value()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::OrPointer<Argument>> parseOr(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseNAry<ast::Or<Argument>, ArgumentParser>(context, astContext, variableStack, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ArgumentLeft, typename ArgumentRight, typename ArgumentLeftParser, typename ArgumentRightParser>
std::experimental::optional<ast::WhenPointer<ArgumentLeft, ArgumentRight>> parseWhen(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentLeftParser parseArgumentLeft, ArgumentRightParser parseArgumentRight)
{
	return parseBinary<ast::When<ArgumentLeft, ArgumentRight>, ArgumentLeftParser, ArgumentRightParser>(context, astContext, variableStack, parseArgumentLeft, parseArgumentRight);
}

/*
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///	Comparisions
///	@brief returns the result of parseBinary. But if they are not of the e.g. ast::Greater type it returns NULL instead
///
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::GreaterPointer<Argument>> parseGreater(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseBinary<ast::Greater<ArgumentLeft, ArgumentRight>, ArgumentLeftParser, ArgumentRightParser>(context, astContext, variableStack, parseArgumentLeft, parseArgumentRight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::GreaterEqualsPointer<Argument>> parseGreaterEquals(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseBinary<ast::GreaterEquals<ArgumentLeft, ArgumentRight>, ArgumentLeftParser, ArgumentRightParser>(context, astContext, variableStack, parseArgumentLeft, parseArgumentRight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::SmallerPointer<Argument>> parseSmaller(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseBinary<ast::Smaller<ArgumentLeft, ArgumentRight>, ArgumentLeftParser, ArgumentRightParser>(context, astContext, variableStack, parseArgumentLeft, parseArgumentRight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::SmallerEqualsPointer<Argument>> parseSmallerEquals(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument )
{
	return parseBinary<ast::SmallerEquals<ArgumentLeft, ArgumentRight>, ArgumentLeftParser, ArgumentRightParser>(context, astContext, variableStack, parseArgumentLeft, parseArgumentRight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
///	Numeric Operators
///
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::AddPointer<Argument>> parseAdd(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseBinary<ast::Add<Argument>, ArgumentParser, ArgumentParser>(context, astContext, variableStack, parseArgument, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::SubtractPointer<Argument>> parseSubtract(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseBinary<ast::Subtract<Argument>, ArgumentParser, ArgumentParser>(context, astContext, variableStack, parseArgument, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::DividePointer<Argument>> parseDivide(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseBinary<ast::Divide<Argument>, ArgumentParser, ArgumentParser>(context, astContext, variableStack, parseArgument, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Argument, typename ArgumentParser>
std::experimental::optional<ast::MultiplyPointer<Argument>> parseMultiply(Context &context, ASTContext &astContext, VariableStack &variableStack, ArgumentParser parseArgument)
{
	return parseBinary<ast::Multiply<Argument>, ArgumentParser, ArgumentParser>(context, astContext, variableStack, parseArgument, parseArgument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
*/
}
}

#endif
