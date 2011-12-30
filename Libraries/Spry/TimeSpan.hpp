#ifndef SPRY_TIMESPAN_HPP
#define SPRY_TIMESPAN_HPP

namespace Spry
{
	struct TimeSpan
	{
	public:

		TimeSpan()
		{
		}

		static TimeSpan FromSeconds(int seconds)
		{
			TimeSpan ts;
			ts._seconds = seconds;
			return ts;
		}

		static TimeSpan FromMinutes(int minutes)
		{
			return FromSeconds(minutes * 60);
		}

		static TimeSpan FromHours(int hours)
		{
			return FromMinutes(hours * 60);
		}

	private:
		int _seconds;
	};
}

#endif