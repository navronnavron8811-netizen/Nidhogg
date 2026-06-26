#include "pch.h"
#include "Logger.h"

/*
* Description:
* Log is responsible for logging messages to the debugger.
*
* Parameters:
* @logLevel [_In_ LogLevel]    -- The log level.
* @format   [_In_ const char*] -- The format string.
* @argList  [_In_ va_list]     -- The argument list.
*
* Returns:
* There is no return value.
*/
void Logger::Log(_In_ LogLevel logLevel, _In_ const char* format, _In_ va_list argList) const {
    if (logLevel < LogLevel::Debug || logLevel > LogLevel::Error)
        return;

    if (level <= logLevel) {
        char prefixBuffer[sizeof(LOGGING_PREFIX) + MAX_LOG_LEVEL_PREFIX_LEN] = { 0 };
        NTSTATUS status = RtlStringCbPrintfA(prefixBuffer, sizeof(prefixBuffer),
            LOGGING_PREFIX "%s]: ",
            LogLevelPrefixes[static_cast<int>(logLevel)]);

        NT_SUCCESS(status) ? vDbgPrintExWithPrefix(prefixBuffer, DPFLTR_IHVDRIVER_ID, TraceLevels[static_cast<int>(logLevel)], format, argList) :
            vDbgPrintExWithPrefix(NO_PREFIX_LOGGING, DPFLTR_IHVDRIVER_ID, TraceLevels[static_cast<int>(logLevel)], format, argList);
    }
}

/*
* Description:
* Debug is responsible for logging debug messages to the debugger.
*
* Parameters:
* @format [_In_ const char*] -- The format string.
* @...    [_In_]             -- The arguments.
*
* Returns:
* There is no return value.
*/
void Logger::Debug(const char* format, ...) const {
    va_list argList;
    va_start(argList, format);
    Log(LogLevel::Debug, format, argList);
    va_end(argList);
}

/*
* Description:
* Info is responsible for logging info messages to the debugger.
*
* Parameters:
* @format [_In_ const char*] -- The format string.
* @...    [_In_]             -- The arguments.
*
* Returns:
* There is no return value.
*/
void Logger::Info(const char* format, ...) const {
    va_list argList;
    va_start(argList, format);
    Log(LogLevel::Info, format, argList);
    va_end(argList);
}

/*
* Description:
* Warning is responsible for logging warning messages to the debugger.
*
* Parameters:
* @format [_In_ const char*] -- The format string.
* @...    [_In_]             -- The arguments.
*
* Returns:
* There is no return value.
*/
void Logger::Warning(const char* format, ...) const {
    va_list argList;
    va_start(argList, format);
    Log(LogLevel::Warning, format, argList);
    va_end(argList);
}

/*
* Description:
* Error is responsible for logging error messages to the debugger.
*
* Parameters:
* @format [_In_ const char*] -- The format string.
* @...    [_In_]             -- The arguments.
*
* Returns:
* There is no return value.
*/
void Logger::Error(const char* format, ...) const {
    va_list argList;
    va_start(argList, format);
    Log(LogLevel::Error, format, argList);
    va_end(argList);
}
