#ifndef _COMMON_
#define _COMMON_

//---------------------------------------------------
// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 14.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved
//---------------------------------------------------

// #include "unistd.h"
// for Linux
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

// define and initialize the name of the configuration file
#ifdef __linux__
    //linux code goes here

#elif _WIN32
    // windows code goes here
   const char* conf_file_name = "conf_app.xml";
   const char* conf_data_airodrome_name = "conf_airports.xml";
   const char* csv_file_cat11 = "cat11_";
#else
// #error "OS not supported!"
#endif

// define and initialize the name of the configuration file
// const char* conf_data_airodrome_name = "conf_airports.xml";

// define and initialize the name of the output csv file 
// with incoming data of ASTERIX 11 category
// const char* csv_file_cat11 = "cat11_";	// .csv";

//const double MyLAT = 59.800028;			// but still bind to the control point
//const double MyLON = 30.2625;				// considering that there is a geocalc
//-----------------------------------------------
// set the latitude and longitude of the application reference point 
// in decimal fractions in the P3-90.02 system

// application reference point (upper left corner of the controlled area,
// corresponding to the top left of the screen display point)
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 47;
	//pTargetLocationInMinutes->seconds = 36;
	//Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 15;
	//pTargetLocationInMinutes->seconds = 1.2;

// CSV recording mode according to the selected period (record all records [all], 
// record records with an interval of 100 ms [standard], 
// record records with an interval of 500 ms [extended period],
// write records with an interval of 1000 ms [long_period], 
// write records with an interval of 10000 ms [very_long_period])
enum recording_mode_period {ALL_RECORDS, STANDARD, EXTENDED, LONG_PERIOD, VERY_LONG_PERIOD};
//enum type_system_coordinat { WGS_84, SK_42, main_XYZ, CPA_GAUSE, CPA_MERCATOR_WGS, CPA_MERCATOR_DOMAIN };

// 11 category items 
enum elements_data_category11 {
	TYPE_MSG, 						// data item I011/000
	DSI,							// data item I011/010
	SERVICE_ID,						// data item I011/015
	POS_WGS_84,						// data item I011/041
	POS_CART,						// data item I011/042
	MODE_3A,						// data item I011/060
	FLIGHT_LEVEL,					// data item I011/090
	GEO_ALTITUDE,					// data item I011/092
	BARO_ALT,						// data item I011/093
	TIME_OF_TRACK,					// data item I011/140
	TRACK_NUMBER					// data item I011/161		
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
