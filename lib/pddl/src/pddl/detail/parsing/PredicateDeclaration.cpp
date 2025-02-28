#include <pddl/detail/parsing/PredicateDeclaration.h>

#include <pddl/Exception.h>
#include <pddl/detail/ASTContext.h>
#include <pddl/detail/parsing/VariableDeclaration.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PredicateDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Constructs a new PredicateDeclaration object.	
 */
void parseAndAddPredicateDeclaration(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.expect<std::string>("(");

	auto name = tokenizer.getIdentifier();

	tokenizer.skipWhiteSpace();

	auto parameters = parseVariableDeclarations(context, domain);

	tokenizer.expect<std::string>(")");

	domain.predicates.emplace_back(std::make_unique<ast::PredicateDeclaration>(std::move(name), std::move(parameters)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief	parses imput e.g.:
 * 				predicates
 * 					(can_hear ?t - thing)
 * 					(on ?x - obj))
 */
void parseAndAddPredicateDeclarations(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.skipWhiteSpace();

	while (tokenizer.currentCharacter() != ')')
	{	
		// parse&add singe imput e.g. (can_hear ?t - thing)
		parseAndAddPredicateDeclaration(context, domain);

		tokenizer.skipWhiteSpace();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
