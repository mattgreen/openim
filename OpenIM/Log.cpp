#include "Log.hpp"

#include "Interfaces/ILog.hpp"

extern "C"
{
	ILog* GetLogger()
	{
		static ILog* logger = ComponentManager::Get<ILog>();
		Assert(logger != 0);

		return logger;
	}

	void LogError(const wchar_t* source, const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);

		GetLogger()->WriteError(source, format, args);

		va_end(args);
	}

	void LogInfo(const wchar_t* source, const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);

		GetLogger()->WriteInfo(source, format, args);

		va_end(args);
	}

	void LogWarning(const wchar_t* source, const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);

		GetLogger()->WriteWarning(source, format, args);

		va_end(args);
	}

}