/*
 *  Logger.cpp
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "Logger.h"

wrench::LogLevel wrench::Logger::m_currentLogLevel = wrench::LogLevel::Error;

void wrench::Logger::setLogLevel(enum LogLevel level)
{
  m_currentLogLevel = level;
}

bool wrench::Logger::shouldLog(enum LogLevel level)
{
  return level >= m_currentLogLevel;
}

enum wrench::LogLevel wrench::Logger::getLogLevel(void)
{
  return m_currentLogLevel;
}

void wrench::Logger::logDebug(const string &message)
{
  if(shouldLog(wrench::LogLevel::Debug))
  {
	clog << "Debug: " << message << endl;
  }
}

void wrench::Logger::logDebug(const char* format, ...)
{
  if(shouldLog(wrench::LogLevel::Debug))
  {
	va_list argPtr;
	va_start(argPtr, format);
	wrench::Logger::log(format, argPtr);
	va_end(argPtr);
  }
}

void wrench::Logger::logError(const string &message)
{
  if(shouldLog(wrench::LogLevel::Error))
  {
	clog << "Error: " << message << endl;
  }
}

void wrench::Logger::logError(const char* format, ... )
{
  if(shouldLog(wrench::LogLevel::Error))
  {
	va_list argPtr;
	va_start(argPtr, format);
	wrench::Logger::log(format, argPtr);
	va_end(argPtr);
  }
}

void wrench::Logger::log(const char* format, va_list argPtr)
{
  double FArg;
  int IArg;
  for(int i = 0; format[i] != '\0'; ++i )
  {
	  if (format[i] == 'f')
	  {
		  clog << va_arg(argPtr, double);

	  }
	  else if (format[i] == 'i')
	  {
		  clog << va_arg(argPtr, int);
	  }

	  switch(format[i])
	  {
	  case 's':
		  clog << va_arg(argPtr, char * );
		  break;
			
	  case 'c':
		  clog << ((char) va_arg(argPtr, int ));
		  break;

	  case 'd':
		  clog << va_arg(argPtr, int );
		  break;

	  case 'f':
		  clog << va_arg(argPtr, double);
		  break;

	  default:
		  break;
	  }
  }
}

