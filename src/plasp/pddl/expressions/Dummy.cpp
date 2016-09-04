#include <plasp/pddl/expressions/Dummy.h>

#include <plasp/pddl/IO.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Dummy
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Dummy::Dummy(std::string name)
:	m_name{name}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Dummy::print(std::ostream &ostream) const
{
	ostream << "(" << m_name << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
