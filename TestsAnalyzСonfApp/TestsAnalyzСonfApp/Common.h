#ifndef _COMMON_
#define _COMMON_

// #include "unistd.h"
// для Linux
// typedef unsigned short WORD;
// typedef unsigned char BYTE;
typedef unsigned char u_int8_t; 
typedef unsigned int u_int32_t;
// typedef unsigned long long u_int64_t;
#ifdef __linux__
    //linux code goes here
    typedef uint64_t u_int64_t;
#elif _WIN32
    // windows code goes here
    #include "unistd.h"
    typedef unsigned long long u_int64_t;
#else
// #error "OS not supported!"
#endif

// определим и инициализируем наименование конфигурационного файла
// const char* conf_file_name = "conf_app.xml";
#ifdef __linux__
    //linux code goes here

#elif _WIN32
    // windows code goes here
   const char* conf_file_name = "conf_app.xml";
   const char* conf_data_airodrome_name = "conf_airports.xml";
   const char* csv_file_cat11 = "cat11_";	// .csv";
#else
// #error "OS not supported!"
#endif
// определим и инициализируем наименование конфигурационного файла
// const char* conf_data_airodrome_name = "conf_airports.xml";

// определим и инициализируем наименование выходного csv-файла с поступающими данными АСТЕРИКС 11 категории 
// const char* csv_file_cat11 = "cat11_";	// .csv";

//const double MyLAT = 59.800028;			// но все же привяжемся к контрольной точке
//const double MyLON = 30.2625;				// учитывая, что появился свой geocalc
//-----------------------------------------------
// зададим широту и долготу референсной точки приложения в десятичных дробях в системе П3-90.02
// референсная точка приложения (левый верхний угол контролируемой территории, 
// соответствующий левой верхей точки экранного отображения)
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 47;
	//pTargetLocationInMinutes->seconds = 36;
	//Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 15;
	//pTargetLocationInMinutes->seconds = 1.2;

// режим записи в csv согласно выбранному периоду (записывать все записи [all], записывать записи с интервалом 100 мс [standard], записывать записи с интервалом 500 мс[extended period],
// записывать записи с интервалом 1000 мс[long_period],	записывать записи с интервалом 10000 мс[very_long_period], )
enum recording_mode_period {ALL_RECORDS, STANDARD, EXTENDED, LONG_PERIOD, VERY_LONG_PERIOD};
//enum type_system_coordinat { WGS_84, SK_42, main_XYZ, CPA_GAUSE, CPA_MERCATOR_WGS, CPA_MERCATOR_DOMAIN };

// элементы данных 11 категории 
enum elements_data_category11 {
	TYPE_MSG, 				// элемент данных I011/000
	DSI,							// элемент данных I011/010
	SERVICE_ID,				// элемент данных I011/015
	POS_WGS_84,			// элемент данных I011/041
	POS_CART,				// элемент данных I011/042
	MODE_3A,					// элемент данных I011/060
	FLIGHT_LEVEL,			// элемент данных I011/090
	GEO_ALTITUDE,		// элемент данных I011/092
	BARO_ALT,				// элемент данных I011/093
	TIME_OF_TRACK,		// элемент данных I011/140
	TRACK_NUMBER		// элемент данных I011/161		
};

enum ConsoleColor {
    BLACK=0, BLUE=1, GREEN=2, CYAN=3,
    RED=4, MAGENTA=5, BROWN=6, LIGHT_GRAY=7,
    DARK_GRAY=8, LightBLUE=9,   LightGREEN=10,  LightCYAN=11,
    LightRED=12,    LightMAGENTA=13,  YELLOW=14,  WHITE=15 };

enum MSG_TYPE{
	MT_UNKNOWN,
	MT_TRACK_AND_FLIGHT_PLAN,
	MT_MANUAL_ATTACH_FLIGHT_PLAN,
	MT_MANUAL_DETACHMENT_FLIGHT_PLAN,
	MT_INSERT_FLIGHT_PLAN,
	MT_SUPPRESSION_FLIGHT_PLAN,
	MT_MODIFICATION_FLIGHT_PLAN,
	MT_HOLDBAR_STATUS
};


#endif /* _COMMON_ */
