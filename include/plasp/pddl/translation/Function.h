#ifndef __PLASP__PDDL__TRANSLATION__FUNCTION_H
#define __PLASP__PDDL__TRANSLATION__FUNCTION_H

#include <colorlog/Formatting.h>

#include <pddl/NormalizedAST.h>
#include <pddl/Parse.h>

#include <plasp/pddl/translation/Primitives.h>
#include <plasp/pddl/translation/Variables.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function #TODO only Copy of predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void translateFunction(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Predicate &predicate, VariableIDMap &variableIDs);
void translateFunctionDeclaration(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::PredicateDeclaration &predicateDeclaration, VariableIDMap &variableIDs);

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateFunction(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Predicate &predicate, VariableIDMap &variableIDs)
{
	const auto &arguments = predicate.arguments;

	if (arguments.empty())
	{
		outputStream << *predicate.declaration;
		return;
	}

	outputStream << "(" << *predicate.declaration;

	for (const auto &argument : arguments)
	{
		outputStream << ", ";

		const auto handleObject =
			[&](const ::pddl::normalizedAST::ObjectPointer &object)
			{
				outputStream << colorlog::Keyword("object") << "(" << *object << ")";
			};

		const auto handleVariable =
			[&](const ::pddl::normalizedAST::VariablePointer &variable)
			{
				translateVariable(outputStream, *variable, variableIDs);
			};

		argument.match(handleObject, handleVariable);
	}

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateFunctionDeclaration(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::PredicateDeclaration &predicateDeclaration, VariableIDMap &variableIDs)
{
	outputStream << colorlog::Keyword("numericVariable") << "(";

	if (predicateDeclaration.parameters.empty())
	{
		outputStream << predicateDeclaration << ")";
		return;
	}

	outputStream << "(" << predicateDeclaration;
	translateVariablesForRuleHead(outputStream, predicateDeclaration.parameters, variableIDs);
	outputStream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void translateFunctionToVariable(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Predicate &predicate, VariableIDMap &variableIDs, bool isPositive = true)
{
	outputStream << colorlog::Keyword("variable") << "(";

	translateFunction(outputStream, predicate, variableIDs);

	outputStream
		<< "), "
		<< colorlog::Keyword("value") << "("
		<< colorlog::Keyword("variable") << "(";

	translateFunction(outputStream, predicate, variableIDs);

	outputStream << "), ";

	if (isPositive)
		outputStream << colorlog::Boolean("true");
	else
		outputStream << colorlog::Boolean("false");

	outputStream << ")";
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
