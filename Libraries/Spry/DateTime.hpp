#ifndef SPRY_DATETIME_HPP
#define SPRY_DATETIME_HPP

#include <ctime>

namespace Spry
{
	struct DateTime
	{
		DateTime() :
			Year(1900),
			Month(1),
			Day(1),
			Hours(0),
			Minutes(0),
			Seconds(0)
		{
		}

		explicit DateTime(const time_t& secondsSinceEpoch)
		{
			tm* then = localtime(&secondsSinceEpoch);

			Year = then->tm_year;
			Month = then->tm_mon;
			Day = then->tm_mday;
			Hours = then->tm_hour;
			Minutes = then->tm_min;
			Seconds = then->tm_sec;
		}

		static DateTime GetCurrentDateTime()
		{
			time_t now = time(0);

			return DateTime(now);
		}

		int Year;
		int Month;
		int Day;

		int Hours;
		int Minutes;
		int Seconds;
	};
}

#endif