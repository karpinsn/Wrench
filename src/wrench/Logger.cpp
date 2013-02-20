/*
 *  Logger.cpp
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "Logger.h"

wrench::Logger::LogLevel wrench::Logger::m_currentLogLevel = wrench::Logger::Error;

void wrench::Logger::setLogLevel(enum LogLevel level)
{
  m_currentLogLevel = level;
}

bool wrench::Logger::shouldLog(enum LogLevel level)
{
  return level >= m_currentLogLevel;
}

enum wrench::Logger::LogLevel wrench::Logger::getLogLevel(void)
{
  return m_currentLogLevel;
}

void wrench::Logger::logDebug(const string &message)
{
  if(shouldLog(wrench::Logger::Debug))
  {
	wrench::Logger::log(message);
  }
}

void wrench::Logger::logDebug(const char* format, ...)
{
  if(shouldLog(wrench::Logger::Debug))
  {
	va_list argPtr;
	va_start(argPtr, format);
	wrench::Logger::log(format, argPtr);
	va_end(argPtr);
  }
}

void wrench::Logger::logError(const string &message)
{
  if(shouldLog(wrench::Logger::Error))
  {
	wrench::Logger::log(message);
  }
}

void wrench::Logger::logError(const char* format, ... )
{
  if(shouldLog(wrench::Logger::Error))
  {
	va_list argPtr;
	va_start(argPtr, format);
	wrench::Logger::log(format, argPtr);
	va_end(argPtr);
  }
}

void wrench::Logger::log(const string& message)
{
  cout << " -";
  cout << message;
  cout << endl;
}

void wrench::Logger::log(const char* format, va_list argPtr)
{
  cout << " -";
  vprintf(format, argPtr);
  cout << endl;
}

