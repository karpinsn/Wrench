/*
 Filename:		Logger.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <string>
#include <stdarg.h>

using namespace std;

namespace wrench
{
	class Logger
	{
	public:
	  enum LogLevel
	  {
		All = 0,
		Info = 1,
		Debug = 2,
		Error = 3,
		None = 4
	  };

	private:
	  static enum LogLevel m_currentLogLevel;

	public:
		static void setLogLevel(enum LogLevel level);
		static bool shouldLog(enum LogLevel level);
		static enum LogLevel getLogLevel(void);
		static void logDebug(const string &message);
		static void logDebug(const char* format, ... );
		static void logError(const string &message);
		static void logError(const char* format, ... );
	
	private:
		static void log(const string& message);
		static void log(const char* format, va_list argPtr);
	};
}

#endif	// _LOGGER_H_
