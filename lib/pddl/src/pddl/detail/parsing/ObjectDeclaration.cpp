#include <pddl/detail/parsing/ObjectDeclaration.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/detail/ASTCopy.h>
#include <pddl/detail/parsing/PrimitiveType.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ObjectDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddUntypedObjectDeclaration(Context &context, ast::ObjectDeclarations &objectDeclarations)
{
	auto &tokenizer = context.tokenizer;

	auto objectName = tokenizer.getIdentifier();
	assert(objectName != "-");

	objectDeclarations.emplace_back(std::make_unique<ast::ObjectDeclaration>(std::move(objectName)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddObjectDeclarations(Context &context, ast::Domain &domain, ast::ObjectDeclarations &objectDeclarations)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.skipWhiteSpace();

	// Index on the first element of the current inheritance list
	size_t inheritanceIndex = objectDeclarations.size();

	while (tokenizer.currentCharacter() != ')')
	{
		parseAndAddUntypedObjectDeclaration(context, objectDeclarations);

		tokenizer.skipWhiteSpace();

		if (!tokenizer.testAndSkip<char>('-'))
			continue;

		// If existing, parse and store parent type
		auto parentType = parsePrimitiveType(context, domain);

		for (size_t i = inheritanceIndex; i < objectDeclarations.size(); i++)
			objectDeclarations[i]->type = ast::deepCopy(parentType);

		// All types up to now are labeled with their parent types
		inheritanceIndex = objectDeclarations.size();

		tokenizer.skipWhiteSpace();
	}

	const bool isTypingUsed = !domain.types.empty();

	if (isTypingUsed && !objectDeclarations.empty() && !objectDeclarations.back()->type)
		throw ParserException(tokenizer.location(), "missing type declaration for object “" + objectDeclarations.back()->name + "”");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddObjectDeclarations(Context &context, ast::Domain &domain)
{
	parseAndAddObjectDeclarations(context, domain, domain.objects);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddObjectDeclarations(Context &context, ast::Problem &problem)
{
	parseAndAddObjectDeclarations(context, *problem.domain, problem.objects);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
