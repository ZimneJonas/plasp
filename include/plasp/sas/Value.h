#ifndef __SAS__VALUE_H
#define __SAS__VALUE_H

#include <iosfwd>
#include <string>
#include <vector>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Value
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Value
{
	public:
		enum class Sign
		{
			Positive,
			Negative
		};

		static const Value Any;

		static Value fromSAS(std::istream &istream);

	public:
		void printAsSAS(std::ostream &ostream) const;
		void printAsASP(std::ostream &ostream) const;

		Sign sign() const;
		const std::string &name() const;

	private:
		static const Value any();

	private:
		Value();

		Sign m_sign;
		std::string m_name;
		bool m_hasArguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

using Values = std::vector<Value>;

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
