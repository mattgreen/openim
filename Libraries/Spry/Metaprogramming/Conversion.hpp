#ifndef SPRY_METAPROGRAMMING_CONVERSION_HPP
#define SPRY_METAPROGRAMMING_CONVERSION_HPP

namespace Spry
{
namespace Metaprogramming
{
	template<typename T, typename U>
	struct Conversion
	{
	private:
		static T		Create();
		static char		Test(U);
		static short	Test(...);
	public:
		enum
		{
			Exists = sizeof(Test(Create())) == sizeof(char)
		};
	};

	template<typename T>
	struct Conversion<T, T>
	{
		enum
		{
			Exists = true
		};
	};
}
}

#endif