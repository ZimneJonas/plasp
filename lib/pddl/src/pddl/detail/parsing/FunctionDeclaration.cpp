//#include <pddl/detail/parsing/FunctionsDeclartion.h>

#include <pddl/Exception.h>
#include <pddl/detail/ASTContext.h>
#include <pddl/detail/parsing/VariableDeclaration.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief   parse&add singe imput e.g. (hear_counter ?t - thing)
 */ 


void parseAndAddFunctionDeclaration(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.expect<std::string>("(");

	auto name = tokenizer.getIdentifier();

	tokenizer.skipWhiteSpace();

	auto parameters = parseVariableDeclarations(context, domain);

	tokenizer.expect<std::string>(")");

	domain.functions.emplace_back(std::make_unique<ast::PredicateDeclaration>(std::move(name), std::move(parameters)));
	//domain.functions.emplace_back(std::make_unique<ast::Function>(std::move(name), std::move(parameters)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * @brief   parse&add all function in context
 * @param context		part of filedata with Functions
 * @param domain    	translated domain
 */

void parseAndAddFunctionDeclarations(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.skipWhiteSpace();

	while (tokenizer.currentCharacter() != ')')
	{	
		parseAndAddFunctionDeclaration(context, domain);

		tokenizer.skipWhiteSpace();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////


}
}
