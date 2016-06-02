#include <plasp/pddl/expressions/OrExpression.h>

#include <plasp/pddl/ExpressionVisitor.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// OrExpression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void OrExpression::accept(plasp::pddl::ExpressionVisitor &expressionVisitor) const
{
	expressionVisitor.visit(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
