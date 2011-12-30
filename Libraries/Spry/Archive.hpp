#ifndef SPRY_SERIALIZATION_ARCHIVE_HPP
#define SPRY_SERIALIZATION_ARCHIVE_HPP

namespace Spry
{
namespace Serialization
{
	template<typename DerivedArchive>
	class Archive
	{
	public:
		bool IsReading
		template<typename T>
		Archive<DerivedArchive>& operator&(const T& object)
		{
			return DerivedArchive
		}
	};
}
}

#endif