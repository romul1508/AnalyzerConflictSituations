/** author-developer: Roman Ermakov */
#ifndef _LOG_H_
#define _LOG_H_

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <string>
#include <ctime>
#include "Singleton.h"

// using namespace std;

namespace ern{
#define BUFF_LOGGER_INFO	200
/** Лог создан с использованием улучшенной версии синглетона Мейерса
	Однако следует учесть, что если будут созданы еще классы одиночки,
	например, для работы с БД, то такая реализация может привести к
	проблеме висячих ссылок и для таких случаев потребуется другая реализация,
	с использованием библиотеки Loki.
	Более того, синглетон Мейерса является крайне неподходящим для приложений,
	работающих в многопоточном режиме.
*/

class Log;					// опережающее объявление

//template <typename T>
//class  SingletonDestroyer
//{
//  private:
//    // Singleton* p_instance;
//	T* p_instance;
//  public:   
//    ~SingletonDestroyer()
//	{
//		delete p_instance;
//	}
//    void initialize( T* p )
//	{
//		p_instance = p;
//	}
//};
//--------------------------------
//template <typename T>
//class  Singleton
//{
//  /*private:
//    static SingletonDestroyer destroyer;*/
//  protected:
//	static T* singletonInstance;
//    Singleton() { }
//    Singleton( const Singleton& );
//    Singleton& operator=( Singleton& );
//    virtual ~Singleton() { }
//    friend class SingletonsDestroyer;
//  public:
//    static T& getInstance()
//	{
//		if(!singletonInstance)  
//		{
//			singletonInstance = new T();
//			// destroyer.initialize(singletonInstance);
//			
//		}
//		return *singletonInstance;
//	}
//};

// template <typename T> T* Singleton<T>::singletonInstance;
// template <typename T> SingletonsDestroyer Singleton<T>::destroyer;
// Log * Singleton<Log>::singletonInstance = 0;
// SingletonsDestroyer Singleton<Log>::destroyer;

// template <typename T>
class  LoggerDestroyer : public SingletonDestroyer<Log>
{
  protected:
		friend class SingletonDestroyer<Log>;
		// virtual ~LoggerDestroyer() {  }
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
			 // for(int i=0;i<40;i++) 
			 //	 s[i]=0;
			 // почистим память
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
			// friend class SingletonsDestroyer;
			// virtual ~Log() {  }
 
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

    // LoggerDestroyer Log::destroyer;	
}

#endif /* _LOG_H_ */
