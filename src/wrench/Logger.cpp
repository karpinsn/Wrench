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

void wrench::Logger::logError(const string &message)
{
  if(shouldLog(wrench::LogLevel::Error))
  {
    clog << "Error: " << message << endl;
  }
}

void wrench::Logger::logError(const char* Format, ... )
{
  if(shouldLog(wrench::LogLevel::Error))
  {
    va_list Arguments;
    va_start(Arguments, Format);
    double FArg;
    int IArg;
    for(int i = 0; Format[i] != '\0'; ++i )
    {
        if (Format[i] == 'f')
        {
            clog << va_arg(Arguments, double);

        }
        else if (Format[i] == 'i')
        {
            clog << va_arg(Arguments, int);
        }

        switch(Format[i])
        {
            // string
        case 's':

            clog << va_arg( Arguments, char * );
            break;
            // character
        case 'c':

            clog << ((char) va_arg( Arguments, int ));

            break;

            // integer
        case 'd':

            clog << va_arg( Arguments, int );
            break;
        case 'f':
            clog << va_arg(Arguments, double);
            break;
        default:
            break;


        }
    }
    va_end(Arguments);
  }
}

