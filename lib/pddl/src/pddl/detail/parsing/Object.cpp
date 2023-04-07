#include <pddl/detail/parsing/Object.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Object
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::ObjectPointer> findObject(const std::string &objectName, ast::ObjectDeclarations &objectDeclarations)
{
	const auto matchingObject = std::find_if(objectDeclarations.begin(), objectDeclarations.end(),
		[&](const auto &objectDeclaration)
		{
			return objectDeclaration->name == objectName;
		});

	if (matchingObject == objectDeclarations.end())
		return std::experimental::nullopt;

	return std::make_unique<ast::Object>(matchingObject->get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::ObjectPointer> findObject(const std::string &objectName, ASTContext &astContext)
{
	auto object = findObject(objectName, astContext.domain->objects);

	if (object)
		return std::move(object.value());

	if (astContext.problem)
	{
		object = findObject(objectName, astContext.problem.value()->objects);

		if (object)
			return std::move(object.value());
	}

	return std::experimental::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::ObjectPointer> parseObject(Context &context, ASTContext &astContext)
{
	auto &tokenizer = context.tokenizer;

	const auto objectName = tokenizer.getIdentifier();
	auto object = findObject(objectName, astContext);

	if (!object)
		return std::experimental::nullopt;

	return std::move(object.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
