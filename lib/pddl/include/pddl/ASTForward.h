#ifndef __PDDL__AST_FORWARD_H
#define __PDDL__AST_FORWARD_H

#include <iosfwd>
#include <memory>
#include <experimental/optional>
#include <set>
#include <vector>

#include <pddl/Variant.h>

namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AST Forward Declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ast
{

////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitives
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Object;
using ObjectPointer = std::unique_ptr<Object>;
struct ObjectDeclaration;
using ObjectDeclarationPointer = std::unique_ptr<ObjectDeclaration>;
using ObjectDeclarations = std::vector<ObjectDeclarationPointer>;
struct PrimitiveType;
using PrimitiveTypePointer = std::unique_ptr<PrimitiveType>;
using PrimitiveTypes = std::vector<PrimitiveTypePointer>;
struct PrimitiveTypeDeclaration;
using PrimitiveTypeDeclarationPointer = std::unique_ptr<PrimitiveTypeDeclaration>;
using PrimitiveTypeDeclarations = std::vector<PrimitiveTypeDeclarationPointer>;
struct Variable;
using VariablePointer = std::unique_ptr<Variable>;
using Variables = std::vector<VariablePointer>;
struct VariableDeclaration;
using VariableDeclarationPointer = std::unique_ptr<VariableDeclaration>;
using VariableDeclarations = std::vector<VariableDeclarationPointer>;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Compounds
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Predicate;
using PredicatePointer = std::unique_ptr<Predicate>;
using Predicates = std::vector<PredicatePointer>;
struct PredicateDeclaration;
using PredicateDeclarationPointer = std::unique_ptr<PredicateDeclaration>;
using PredicateDeclarations = std::vector<PredicateDeclarationPointer>;
/*struct Function;
using FunctionPointer = std::unique_ptr<Function>;
using Functions = std::vector<FunctionPointer>;*/

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct And;
template<class Argument>
using AndPointer = std::unique_ptr<And<Argument>>;
template<class Argument>
struct At;
template<class Argument>
using AtPointer = std::unique_ptr<At<Argument>>;
template<class Argument>
struct Either;
template<class Argument>
using EitherPointer = std::unique_ptr<Either<Argument>>;
template<class ArgumentLeft, class ArgumentRight>
struct Equals;
template<class ArgumentLeft, class ArgumentRight>
using EqualsPointer = std::unique_ptr<Equals<ArgumentLeft, ArgumentRight>>;
template<class Argument>
struct Exists;
template<class Argument>
using ExistsPointer = std::unique_ptr<Exists<Argument>>;
template<class Argument>
struct ForAll;
template<class Argument>
using ForAllPointer = std::unique_ptr<ForAll<Argument>>;
template<class Argument>
struct Imply;
template<class Argument>
using ImplyPointer = std::unique_ptr<Imply<Argument>>;
template<class Argument>
struct Not;
template<class Argument>
using NotPointer = std::unique_ptr<Not<Argument>>;
template<class Argument>
struct Or;
template<class Argument>
using OrPointer = std::unique_ptr<Or<Argument>>;
template<class ArgumentLeft, class ArgumentRight>
struct When;
template<class ArgumentLeft, class ArgumentRight>
using WhenPointer = std::unique_ptr<When<ArgumentLeft, ArgumentRight>>;
/*
/// Numerical Comperisons
template<class ArgumentLeft, class ArgumentRight>
struct Greater;
template<class ArgumentLeft, class ArgumentRight>
using GreaterPointer = std::unique_ptr<Greater<ArgumentLeft, ArgumentRight>>;
template<class ArgumentLeft, class ArgumentRight>
struct GreaterEquals;
template<class Argument>
using GreaterEqualsPointer = std::unique_ptr<GreaterEquals<ArgumentLeft, ArgumentRight>>;
template<class Argument>
struct Smaller;
template<class Argument>
using SmallerPointer = std::unique_ptr<Smaller<ArgumentLeft, ArgumentRight>>;
template<class Argument>
struct SmallerEquals;
template<class Argument>
using SmallerEqualsPointer = std::unique_ptr<SmallerEquals<ArgumentLeft, ArgumentRight>>;
// Numerical Operators
template<class Argument>
struct Add;
template<class Argument>
using AddPointer = std::unique_ptr<Add<Argument>>;
template<class Argument>
struct Subtract;
template<class Argument>
using SubtractPointer = std::unique_ptr<Subtract<Argument>>;
template<class Argument>
struct Multiply;
template<class Argument>
using MultiplyPointer = std::unique_ptr<Multiply<Argument>>;
template<class Argument>
struct Divide;
template<class Argument>
using DividePointer = std::unique_ptr<Divide<Argument>>;
template<class Argument>
struct Assign;
template<class Argument>
using AssignPointer = std::unique_ptr<Assign<ArgumentLeft, ArgumentRight>>;
template<class Argument>
struct Increase;
template<class Argument>
using IncreasePointer = std::unique_ptr<Increase<ArgumentLeft, ArgumentRight>>;
template<class Argument>
struct Decrease;
template<class Argument>
using DecreasePointer = std::unique_ptr<Decrease<ArgumentLeft, ArgumentRight>>;
template<class Argument>
struct ScaleUp;
template<class Argument>
using ScaleUpPointer = std::unique_ptr<ScaleUp<ArgumentLeft, ArgumentRight>>;
template<class Argument>
struct ScaleDown;
template<class Argument>
using ScaleDownPointer = std::unique_ptr<ScaleDown<ArgumentLeft, ArgumentRight>>;
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

namespace detail
{
using TermT = Variant<
	ObjectPointer,
	VariablePointer>;
}

class Term : public detail::TermT
{
	Term() = delete;

	using detail::TermT::TermT;
};

using Terms = std::vector<Term>;

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
using AtomicFormulaT = Variant<
	EqualsPointer<Term, Term>,
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
// TODO: add missing types
using PreconditionT = Variant<
	AtomicFormula,
	AndPointer<Precondition>,
	ExistsPointer<Precondition>,
	ForAllPointer<Precondition>,
	ImplyPointer<Precondition>,
	NotPointer<Precondition>,
	OrPointer<Precondition>>;
/*	GreaterPointer<Term,Term>,
	GreaterEqualsPointer<Term, Term>,
	SmallerPointer<Term,Term>,
	SmallerEqualsPointer<Term,Term>*/
	
}

class Precondition : public detail::PreconditionT
{
	Precondition() = delete;

	using detail::PreconditionT::PreconditionT;
};

using Preconditions = std::vector<Precondition>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class ConditionalEffect;

namespace detail
{
// TODO: add missing types
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
// TODO: add missing types
using EffectT = Variant<
	Literal,
	AndPointer<Effect>,
	ForAllPointer<Effect>,
	/*AssignPointer<Term,Term>,
	IncreasePointer<Term,Term>,
	DecreasePointer<Term,Term>,
	ScaleUpPointer<Term,Term>,
	ScaleDownPointer<Term,Term>,*/
	WhenPointer<Precondition, ConditionalEffect>>;
}

class Effect : public detail::EffectT
{
	Effect() = delete;

	using detail::EffectT::EffectT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
using TypeT = Variant<
	EitherPointer<PrimitiveTypePointer>,
	PrimitiveTypePointer>;
}

class Type : public detail::TypeT
{
	Type() = delete;

	using detail::TypeT::TypeT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class Fact;

namespace detail
{
using FactT = Variant<
	AtPointer<Literal>,
	Literal>;
}

class Fact : public detail::FactT
{
	Fact() = delete;

	using detail::FactT::FactT;
};

using Facts = std::vector<Fact>;

////////////////////////////////////////////////////////////////////////////////////////////////////

using Goal = Precondition;

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
