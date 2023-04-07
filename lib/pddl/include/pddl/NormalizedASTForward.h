#ifndef __PDDL__NORMALIZED_AST_FORWARD_H
#define __PDDL__NORMALIZED_AST_FORWARD_H

#include <pddl/AST.h>

namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Normalized AST Forward Declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace normalizedAST
{

////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitives
////////////////////////////////////////////////////////////////////////////////////////////////////

using ast::Object;
using ast::ObjectPointer;
using ast::ObjectDeclaration;
using ast::ObjectDeclarationPointer;
using ast::ObjectDeclarations;
using ast::PrimitiveTypePointer;
using ast::PrimitiveType;
using ast::PrimitiveTypePointer;
using ast::PrimitiveTypes;
using ast::PrimitiveTypeDeclaration;
using ast::PrimitiveTypeDeclarationPointer;
using ast::PrimitiveTypeDeclarations;
using ast::Variable;
using ast::VariablePointer;
using ast::Variables;
using ast::VariableDeclaration;
using ast::VariableDeclarationPointer;
using ast::VariableDeclarations;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Compounds
////////////////////////////////////////////////////////////////////////////////////////////////////

struct DerivedPredicate;
using DerivedPredicatePointer = std::unique_ptr<DerivedPredicate>;
using DerivedPredicates = std::vector<DerivedPredicatePointer>;
struct DerivedPredicateDeclaration;
using DerivedPredicateDeclarationPointer = std::unique_ptr<DerivedPredicateDeclaration>;
using DerivedPredicateDeclarations = std::vector<DerivedPredicateDeclarationPointer>;
using ast::Predicate;
using ast::PredicatePointer;
using ast::Predicates;
using ast::PredicateDeclaration;
using ast::PredicateDeclarationPointer;
using ast::PredicateDeclarations;
/*using ast::Function;
using ast::FunctionPointer;
*/
////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////////////////////////

using ast::And;
using ast::AndPointer;
using ast::At;
using ast::AtPointer;
using ast::Either;
using ast::EitherPointer;
using ast::Exists;
using ast::ExistsPointer;
using ast::ForAll;
using ast::ForAllPointer;
using ast::Not;
using ast::NotPointer;
using ast::Or;
using ast::OrPointer;
using ast::When;
using ast::WhenPointer;
/*//Numerical comparisons
using ast::Greater;
using ast::GreaterPointer;
using ast::GreaterEquals;
using ast::GreaterEqualsPointer;
using ast::Smaller;
using ast::SmallerPointer;
using ast::SmallerEquals;
using ast::SmallerEqualsPointer;
// Numerical Operators
using ast::Add;
using ast::AddPointer;
using ast::Subtract;
using ast::SubtractPointer;
using ast::Multiply;
using ast::MultiplyPointer;
using ast::Divide;
using ast::DividePointer;
*/
////////////////////////////////////////////////////////////////////////////////////////////////////
// PDDL Structure
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Action;
using ActionPointer = std::unique_ptr<Action>;
using Actions = std::vector<ActionPointer>;
struct Description;
using DescriptionPointer = std::unique_ptr<Description>;
struct Domain;
using DomainPointer = std::unique_ptr<Domain>;
struct InitialState;
struct Problem;
using ProblemPointer = std::unique_ptr<Problem>;
enum class Requirement;
using Requirements = std::vector<Requirement>;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Variants
////////////////////////////////////////////////////////////////////////////////////////////////////

using ast::Term;
using ast::Terms;

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
using AtomicFormulaT = Variant<
	DerivedPredicatePointer,
	PredicatePointer>;
}

class AtomicFormula : public detail::AtomicFormulaT
{
	AtomicFormula() = delete;

	using detail::AtomicFormulaT::AtomicFormulaT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
using LiteralT = Variant<
	AtomicFormula,
	NotPointer<AtomicFormula>>;
}

class Literal : public detail::LiteralT
{
	Literal() = delete;

	using detail::LiteralT::LiteralT;
};

using Literals = std::vector<Literal>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Precondition;

namespace detail
{
using PreconditionT = Variant<
	Literal,
	AndPointer<Literal>>;
}

class Precondition : public detail::PreconditionT
{
	Precondition() = delete;

	using detail::PreconditionT::PreconditionT;
};

using Preconditions = std::vector<Precondition>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class DerivedPredicatePrecondition;

namespace detail
{
using DerivedPredicatePreconditionT = Variant<
	Literal,
	AndPointer<Literal>,
	OrPointer<Literal>>;
}

class DerivedPredicatePrecondition : public detail::DerivedPredicatePreconditionT
{
	DerivedPredicatePrecondition() = delete;

	using detail::DerivedPredicatePreconditionT::DerivedPredicatePreconditionT;
};

using DerivedPredicatePreconditions = std::vector<DerivedPredicatePrecondition>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class ConditionalEffect;

namespace detail
{
using ConditionalEffectT = Variant<
	Literal,
	AndPointer<Literal>>;
}

class ConditionalEffect : public detail::ConditionalEffectT
{
	ConditionalEffect() = delete;

	using detail::ConditionalEffectT::ConditionalEffectT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class Effect;

namespace detail
{
using EffectT = Variant<
	Literal,
	AndPointer<Effect>,
	ForAllPointer<Effect>,
	WhenPointer<Precondition, ConditionalEffect>>;
}

class Effect : public detail::EffectT
{
	Effect() = delete;

	using detail::EffectT::EffectT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

using Fact = Literal;
using Facts = std::vector<Fact>;

////////////////////////////////////////////////////////////////////////////////////////////////////

using Goal = Precondition;

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
