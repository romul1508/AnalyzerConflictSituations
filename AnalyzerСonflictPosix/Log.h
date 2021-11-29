#ifndef _LOG_H_
#define _LOG_H_

// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 14.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <string>
#include <ctime>
#include "Singleton.h"

namespace ern{
#define BUFF_LOGGER_INFO	200
/** The log was created using an improved version of Meyers' singleton. 
    However, it should be borne in mind that if more singleton classes are created, 
	for example, for working with a database, 
	then such an implementation can lead to a dangling link problem 
	and for such cases another implementation will be required using the Loki library. 
	Moreover, Meyers' singleton is highly unsuitable for multithreaded applications.
*/

class Log;


class  LoggerDestroyer : public SingletonDestroyer<Log>
{
  protected:
		friend class SingletonDestroyer<Log>;		
  public:
	  LoggerDestroyer() : SingletonDestroyer<Log>() { }    
};

#ifdef __linux__
    //linux code goes here
    //auto Singleton<T>::singletonInstance;
#elif _WIN32
    // windows code goes here
    Log * Singleton<Log>::singletonInstance = 0;
#else
// #error "OS not supported!"
#endif
//----------------------------------------
 class Log : public Singleton<Log>
 {
	private:
		 static LoggerDestroyer destroyer;	

		 Log() : Singleton<Log>()
		 {
			 destroyer.initialize(this);
		 }

		 char * settime(struct tm *u, char* logline) const
		 {
			 char s[200];
			 char *tmp;
			 
			 memset( &s, 0, sizeof(BUFF_LOGGER_INFO) );
			 int length = strftime(s, 40, "%d.%m.%Y %H:%M:%S ", u);
			 strcpy(s+length, ":  ");
			 strcpy(s+length+3, logline);
			 
			 tmp = (char*)malloc(sizeof(s));
			 strcpy(tmp, s);
			 return(tmp);
		 }		 
		 
	 protected:
			friend class Singleton<Log>;			
 
	 public:
		 void Write(char* logline) const
		 {
			struct tm *u;
			char *f;
			const time_t timer = time(NULL);
			u = localtime(&timer);
			f = settime(u, logline);
			// fprintf (log_file, "%s\n", f);
			std::string logMessage = f;
			std::ofstream log_file("logger.txt", std::ios::app);
			log_file << logMessage << std::endl;
			free(f);
		 }	 	
 };
    	
}

#endif /* _LOG_H_ */
