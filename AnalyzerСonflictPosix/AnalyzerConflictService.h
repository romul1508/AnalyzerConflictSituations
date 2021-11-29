#ifndef ANALYZER_CONFLICT_SERVICE_H_
#define ANALYZER_CONFLICT_SERVICE_H_

// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 12.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

#ifdef __linux__ 
	//linux code goes here
    #define __STDC_FORMAT_MACROS
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
    #include <inttypes.h>
    #include <unistd.h>
#elif _WIN32
	// windows code goes here
	// #include <windows.h>
	#include <winsock2.h>
	#include <conio.h>
#else
// #error "OS not supported!"
#endif

#include "asterix_cat_11.h"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <vector>
#include <set>
#include <algorithm>
#include "Common.h"
#include "Log.h"
#include "Config_app.h"
#include "Config_airports.h"
#include "Pulkovo.h"



# define MAX_SIZE_MSG_ASTERIX_011		20000

// Error message buffer
#define MAX_ERR_MESSAGE					240

// Maximum length of incoming data 
#define BUFF_MESSAGE_INFO				512		

// maximum number of entries in the CSV file
#define	MAX_NUMBER_ENTRIES_CSV			30000	


#define STR_STATUS_TARGET 80

const int MAX = 2000;


typedef struct 
{
	uint8_t sac;
	uint8_t sic;

	uint16_t to_raw() const
	{
		return sac << 8 | sic;
	}

	void from_raw(uint16_t raw)
	{
		sac = raw >> 8;
		sic = raw & 0xFF;
	}
}data_source_id;

typedef struct
{
	double wgs84_lon;
	double wgs84_lat;
}position_wgs84;

typedef struct
{
	double x;
	double y;
}position_cartesian;

typedef struct 
{
	bool   qnh;
	double altitude;
}baro_altitude;

 // Data structure for transmission of the trace status
typedef struct 
{
	 // Trace trace using data (MON)
	 //          1 - one source
	 //          0 - multiradar tracking
	 uint8_t monosensor;
	
	 // Sign of receiving SPI signal from hardware (SPI)
	 //          1 - received signal
	 //          0 - no signal received
	 uint8_t spi_sign;
	
	 // Height preference (more reliable) (MRH)
	 //          1 - geometric height
	 //          0 - barometric altitude (Mode C)
	 uint8_t most_reliable_height;
	
	 // Geometric height source attribute (SRC)
	 //          according to the definition ALTITUDE_SRC_хххххх
	 uint8_t geometric_altitude_source;
	
	 // Data confirmation (update) sign (CNF)
	 //          0 - data confirmed
	 //          1 - data are tentative (extrapolation of elevation)
	 uint8_t confirm;
	
	 // Sign of the reference (simulated) trace (SIM)
	 //          0 - valid track
	 //          1 - simulated track
	 uint8_t simulate;
	
	 // Sign of the last message about the track (TSE)
	 //          0 - default, update information
	 //          1 - last message, track reset from tracking
	 uint8_t last_message;
	
	 // Sign of the first message about the track	(TSB)
	 // 0 - default, update information
	 // 1 - first message, new track accompanied
	 uint8_t first_message;
	
	 // Confirmation Attribute with Planning Information (FPC)
	 // 0 - track out of plan, no confirmation
	 // 1 - the route is confirmed by the planning information
	 uint8_t flight_in_plan;
	
	 // Sign of a contradiction between observation data and data obtained via the ADS channel (AFF)
	 // 0 - default, no contradiction
	 // 1 - there is a discrepancy in the data
	 uint8_t ads_inconsist;
	
	 // Sign of the trace broadcast from subordinate sources
	 //           0 - default value, own trace is output
	 //           1 - slave source trail retransmission
	 uint8_t slave_track;
	
	 // Sign of priority maintenance of the track (letter board)
	 //           0 - priority track
	 //           1 - normal service, standard traffic
	 uint8_t background_serv;
	
	 // Identified trace attribute (the result of tertiary processing of several traces from different sources)(AMA)
	 //           0 - default, no trace identified
	 //           1 - identified track
	 uint8_t amalgamation;
	
	 // Sign of recognition result in Mode 4 (FRI/FOE)
	 uint8_t mode4_interrogate;
	
	 // Sign of receipt of a distress from a military aircraft	(ME)
	 //          0 - default, no distress
	 //          1 - distress signal received
	 uint8_t military_emrg;
	
	 // Sign of identification of the aircraft belonging to the military department (MI)
	 //          0 - default, no distress
	 //          1 - distress signal received
	 uint8_t military_id;
	
	 
	 // Sign of recognition result in Mode 5
	 uint8_t mode5_interrogate;
	
	 // The sign of obsolescence of the time of the last received update (CST) 
	 // of the track is greater than the threshold of the working time (inertia) of the system
	 //           0 - default, the trace is updated
	 //           1 - trace has not been updated, trace extrapolation
	 uint8_t coast_track;
	
	 // Sign of obsolescence of track data received via the PSR channel
	 //           more than the threshold of the working time (inertia) of the system
	 //           0 - default, the trace is updated
	 //           1 - the track has not been updated, there is no PRL data
	 uint8_t psr_coast;
	
	 // Sign of staleness of track data received via SSR channel
	 //           more than the threshold of the working time (inertia) of the system
	 //           0 - default, the trace is updated
	 //           1 - route has not been updated, no SSR data
	 uint8_t ssr_coast;
	
	 // Sign of staleness of track data received via Mode S channel (MDS)
	 //           more than the threshold of the working time (inertia) of the system
	 //           0 - default, the trace is updated
	 //           1 - the track has not been updated, there is no data Mode S
	 uint8_t mode_s_coast;
	
	 // Sign of staleness of track data received via ADS-B channel	(ADS)	
	 //           more than the threshold of the working time (inertia) of the system
	 //           0 - default, the trace is updated
	 //           1 - track has not been updated, no ADS-B data
	 uint8_t ads_coast;
	
	 // Sign of using a special mode code Mode A (board number)
	 //           0 - default value, special board number is not used
	 //           1 - a special board number is used
	 uint8_t suc_set;
	
	 // Sign of the presence of a conflict of numbers in Mode A (aircraft number) (AAC)
	 //            0 - default, no conflict
	 //            1 - there is a track with a similar board number code
	 uint8_t aac_conflict;                             // assigned mode A Code conflict                             // assigned mode A Code conflict
	
	 // Ground observation data status indication
	 uint8_t sds_status;
	
	// Distress type symptom
	//           according to the definition EMERGENCY_STATUS_хххххх
	uint8_t emergency_status;
	
	 // Sign of setting the on-board ADS transmitter (Mode S) on the ground (GBS)
	 //           0 - default, not set
	 //           1 - bit set "on the ground"
	 uint8_t ground_bit_set;

	
	 // Horizontal maneuver sign (по Х, У)
	 //           0 - no maneuver
	 //           1 - there is a maneuver
	 uint8_t horizontal_manoeuvre;

	
	 // Sign of recalculation of inclined coordinates of the target position
	 //        on the horizontal plane (for X, Y)
	 //           0 - no recalculation
	 //           1 - there is a recount	 
	uint8_t slant_correction;

	 // Sign of transformation of coordinates of target position (for X, Y) (для Х, У)
	 //        0 - measurement data
	 //        1 - smoothed data
	uint8_t smooth_position;
}track_status;

typedef struct 
{
	double vx;
	double vy;
}track_velocity;

typedef struct 
{
	double ax;
	double ay;
}calc_acceleration;

 

typedef struct 
{
	static const int CALLSIGN_SIZE = 9;
	int sti;
	char callsign[CALLSIGN_SIZE];
}call_sign;

typedef struct 
{
	double length;
	double width;
	double orientation;
}target_size_orient;

typedef struct 
{
	double psr_age;
	double ssr_age;
	double mda_age;
	double mfl_age;
	double mds_age;
	double ads_age;
	double ads_es_age;
	double md1_age;
	double md2_age;
	double loop_age;
	double trk_age;
	double mlt_age;
}ages;

typedef struct 
{
	bool    trouble : 1;
	uint8_t type    : 7;
}prog_msg;


// Determination of the data structure received via the ModeS / Ads-B channel
// regarding the tracked object

typedef struct 
{

	// 24-bit ICAO Airplane Address Value (Subfield # 2)
	u_int32_t target_address;
	
	// Number of Mode S data records (REP field in subfield No. 1) 
	u_int8_t mode_s_data_count;
	
	// Pointer to Mode S messages (for repeating subfields # 1)
    // Each 64-bit message contains:
	// 64 - 9 bit - 56 bit of the Mode S codogram (MB Data)
	// 8 - 5 bits - BDS data 1
	// 4 - 1 bit - BDS 2 data
	u_int64_t* mode_s_msg;

	//	Aircraft type - ASCII characters (subfield No. 8)
	char aircraft_type[5];

	// Transmitter category (ECAT parameter of subfield No. 9)
	u_int8_t emitter_category;
	
	// Communication capability (COM parameter of subfield no. 4)
	u_int8_t communication_capability;
	
    // Flight status (STAS parameter of subfield No. 4)
	u_int8_t flight_status;
	
	// Special service sign (service priority, SSC parameter of subfield No. 4)
	// 0 - normal flight
	// 1 - priority service
	u_int8_t specific_service;

	 // Sign of the specified height determination accuracy (ARC parameter of subfield No. 4)
	 // 0 - 100 feet (30.48 m)
	 // 1 - 25 feet (7.62 m)
	 u_int8_t altitude_report_capability;

	 // Identification capability attribute (AIC parameter of subfield No. 4)
	 //          0 - нет
	 //          1 - да
	 u_int8_t aircraft_ident_capability;

	// B1A value (parameter B1A of subfield No. 4)
	u_int8_t bit_b1a;

	// В1В value (parameter В1В of subfield No. 4)
	u_int8_t bits_b1b;

	// Sign of the ACAS system operability (AC parameter of subfield No. 4)
    // 0 - no
    // 1 - yes
	u_int8_t acas_operational;

	// Sign of the presence of complex navigation support (parameter MN of subfield No. 4)
	// 0 - no
	// 1 - yes
	u_int8_t multiple_navigational;
	

	//  Sign of the presence of differential correction (DC parameter of the subfield No. 4)
	// 0 - no
	// 1 - yes
	u_int8_t differencial_correction;
	
	// Sign of the presence of VDL equipment (parameter VDL subfield No. 11)
	// 0 - no
	// 1 - yes
	u_int8_t vdl_available;
	
	//  Sign of the presence of Mode S equipment (MDS parameter of subfield No. 11)
	// 0 - no
	// 1 - yes
	 u_int8_t mode_s_available;

	 // Sign of the presence of UAT equipment
	 // 0 - no
	 // 1 - yes
	 u_int8_t uat_available;

}TAdsBRelatedData;

typedef struct 
{
	uint8_t   type_save_data;					    // data type for the current save
	uint8_t msg_type;								// data item I011/000
	data_source_id dsi;								// data item I011/010
	uint8_t service_id;								// data item I011/015
	position_wgs84 pos_wgs84;						// data item I011/041
	position_cartesian pos_cart;					// data item I011/042
	uint16_t mode_3A;								// data item I011/060
	double flight_level;							// data item I011/090
	double geo_altitude;							// data item I011/092
	baro_altitude baro_alt;							// data item I011/093
	double time_of_track;							// data item I011/140
	uint16_t track_number;							// data item I011/161
	track_status tr_status;							// data item I011/170
	track_velocity tr_velocity;						// data item I011/202
	calc_acceleration acceleration;					// data item I011/210
	double calc_vert_rate;							// data item I011/215
	call_sign _call_sign;							// data item I011/245
	target_size_orient target_size;					// data item I011/270
	ages _ages;										// data item I011/290
	int fleet;										// data item I011/300
	prog_msg _prog_msg;								// data item I011/310
	TAdsBRelatedData adsBRelatedData;				// data item I011/380
}TRecCategory_11;
//----------------------------------------------------------------------------------
typedef struct
{
	uint8_t str_status[STR_STATUS_TARGET];
	uint8_t target_koord[STR_STATUS_TARGET];
}TTargetEnvironment;
//----------------------------------------------------------------------------------
// gets a string, changes ',' to '.', converts the string to double
double strToDouble(std::string token);				

// initializes the Winsock module (for Windows operating systems)
bool StartWinsock();								

// puts the Service into the mode of receiving messages and performing computational tasks
// int server();									

//////////test functions/////////
// verifies loading of configuration data
void testLoadAppSetXML();

// to check the correctness of the loaded aerodrome tuning data
void testLoadAirportsSetXML();						

// determination of the correctness of the calculation of the coordinates 
// of the vertices of the polygon
void testDetermCorrectnCalcCoordPolygonvertices();  

// testing the exact algorithm for converting from the WGS-84 system to a rectangular one
void testCalcPrCoord();								
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Класс	 Point (Point on the surface of an aerodrome)////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Point
{
private:
	TKtaPoint 	kta_point;					// point coordinates in the local KTA system
	// TXYZSystemPoint xyz_point;			// coordinates in a rectangular coordinate system relative 
											// to the center of the ellipsoid

	// TSK42Point 	sk_42_point;			// point coordinates in SK-42 system					
	TWgsPoint	wgs_84_point;				// point coordinates in WGS-84 system
	TP3_90_02Point pz_90_02_point;			// point coordinates in the system П3-90.02
	TUtmPoint utm_point;					// point coordinates in the system UTM
	std::string str_utm;					// formatted string with UTM coordinates
	double height;							// height from the surface of the ellipsoid (in the WGS-84 system)	

public:
	Point(double lat, double lon, double height, double Z, int type_coord);	
	
	virtual ~Point() { }

	// returns longitude in Wgs84 system
	double getWgs84_Lon() const  { return wgs_84_point.wgs_X_lon - 0.001852; }									
	
	// returns degrees of longitude in Wgs84 system
	int getWgs84_Degrees_Lon() const  { return wgs_84_point.degrees_lon; }							
	
	// returns minutes of longitude in Wgs84 system
	unsigned char getWgs84_Minutes_Lon() const  { return wgs_84_point.minutes_x_lon; }		
	
	// returns seconds of longitude in Wgs84 system
	double getWgs84_Seconds_Lon() const  { return wgs_84_point.seconds_x_lon; }					

	// returns latitude in Wgs84 system
	double getWgs84_Lat() const  { return wgs_84_point.wgs_Y_lat - 0.00006575; }										
	
	// returns degrees of latitude in Wgs84 system
	int getWgs84_Degrees_Lat() const  { return wgs_84_point.degrees_lat; }
	
	// returns minutes of latitude in Wgs84 system
	unsigned char getWgs84_Minutes_Lat() const  { return wgs_84_point.minutes_y_lat; }

	// returns minutes of latitude in Wgs84 system
	double getWgs84_Seconds_Lat() const  { return wgs_84_point.seconds_y_lat; }					

	// returns longitude in the CK-42 system
	// double getSK42_Lon() const	 { return sk_42_point.sk42_X_lon; }							
	
	// returns latitude in the CK-42 system
	// double getSK42_Lat() const	 { return sk_42_point.sk42_Y_lat; }								

    // returns the height of a point in the P3-90.02 system
	double getP3_90_02_Height() const  { return pz_90_02_point.height; }	

	// returns longitude in P3-90.02 system
	double getP3_90_02_Lon() const  { return pz_90_02_point.p3_90_02_X_lon; }


	int getP3_90_02_Degrees_Lon() const  { return pz_90_02_point.degrees_lon; }							
	
	// returns degrees of longitude in the P3-90.02 system
	unsigned char getP3_90_02_Minutes_Lon() const  { return pz_90_02_point.minutes_x_lon; }		
	
	// returns minutes of longitude in the P3-90.02 system
	double getP3_90_02_Seconds_Lon() const  { return pz_90_02_point.seconds_x_lon; }					

	// returns latitude in P3-90.02 system
	double getP3_90_02_Lat() const  { return pz_90_02_point.p3_90_02_Y_lat; }								
	
	// returns degrees of latitude in the P3-90.02 system
	int getP3_90_02_Degrees_Lat() const  { return pz_90_02_point.degrees_lat; }								
	
	// returns latitude minutes in P3-90.02 system
	unsigned char getP3_90_02_Minutes_Lat() const  { return pz_90_02_point.minutes_y_lat; }			
	
	// returns latitude seconds in P3-90.02 system
	double getP3_90_02_Seconds_Lat() const  { return pz_90_02_point.seconds_y_lat; }					

	// returns the height from the surface of an ellipsoid in the WGS-84 system
	double getHeight() const     { return height; }														

	// returns east offset in UTM system
	double getUTM_easting()const { return utm_point.easting; }									
	
	// returns north offset in UTM system
	double getUTM_northing()const { return utm_point.northing; }								
	
	// returns a UTM formatted string
	std::string getUTM_string()const { return str_utm; }												
	
	double getCPA_x() const	{	return this->kta_point.kta_X; }
	double getCPA_y() const	{	return this->kta_point.kta_Y; }

	void setCPA_x( double kta_X) 	{	this->kta_point.kta_X = kta_X; }
	void setCPA_y( double kta_Y) 	{	this->kta_point.kta_Y = kta_Y; }
};

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Line class (Line [between polygon points]) ////////////////
/////////////////////////////////////////////////////////////////////////////////////
class Line
{
private:
	Point* ptrPoint1;									// pointer to point 1
	Point* ptrPoint2;									// pointer to point 2		
	//-------------------------------------------
	// Parameters of the equation of the straight line y = kx + m (in the local system)
	double k;  
	double m;
	//------------------------------------------ 
	void calcParamLine();

public:
	Line(Point* ptrPoint1, Point* ptrPoint2); 
	virtual ~Line() { }
	
	// returns the value of the function y = kx + m at x == SK42_X in KTA format	
	double getIntersectionCPA_Y(double CPA_X) 	{ return k * CPA_X + m; }			
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Runway Class (Runway) //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
class Runway
{
private:
	int 	id_vpp;							// Runway ID					
	std::string		caption_vpp;			// Runway name
	std::string		cat_vpp;				// Runway category
	double	angle_vpp;						// Runway corner
	int lenght_vpp;							// Runway length in meters
	int width_vpp;							// Runway width in meters
	bool isRunwayEmploymentStatus_;			// Busy status Runway (true - busy, 
											// false - free)
	
	Point* ptrCentralPoint;					// center point of Runway
	Point* ptrNorthPoint;					// north point of Runway
	Point* ptrSouthPoint;					// south point of Runway
	Point* ptrWesternPoint;					// west point of Runway
	Point* ptrEasternPoint;					// east point of Runway

	Line* ptrWN;							// line passing through the west and north point
	Line* ptrNE;							// line passing through the north and east point
	Line* ptrES;							// line passing through the east and south point
	Line* ptrSW;							// line passing through the south and west point

	// returns the runway occupied status for a specific target location
	bool runway_employment_status(Point* ptrTargetPoint);		
	
	// recalculates the coordinates of points ptrNorthPoint, ptrSouthPoint, 
	// ptrWesternPoint, ptrEasternPoint
	void to_recalculate_coords();								

public:
	Runway(int 	id_vpp, double height_cta); 
	
	virtual ~Runway() { 
		delete ptrCentralPoint; delete ptrNorthPoint; delete ptrSouthPoint; delete ptrWesternPoint; delete ptrEasternPoint; 
		delete ptrWN; delete ptrNE; delete ptrES, delete ptrSW;
	}

	// returns the runway occupied status for a specific target location
	bool isRunwayEmploymentStatus(Point* ptrTargetPoint)			
	{
		if(runway_employment_status(ptrTargetPoint))
			return true;
		else
			return false; 		
	}

	// sets runway employment status
	void setRunwayEmploymentStatus(bool isRunwayEmploymentStatus)  { isRunwayEmploymentStatus_ 
		= isRunwayEmploymentStatus; }
	
	bool getRunwayEmploymentStatus() const { return isRunwayEmploymentStatus_; }
	
	// returns a pointer to the northernmost point of the runway
	Point* getPtrNorthPoint() const	{ return ptrNorthPoint; }				
	
	// returns a pointer to the southernmost point of the runway
	Point* getPtrSouthPoint() const	{ return ptrSouthPoint; }				

	// returns a pointer to the westernmost point of the runway
	Point* getPtrWestPoint() const	{ return ptrWesternPoint; }				
	
	// returns a pointer to the westernmost point of the runway
	Point* getPtrEastPoint() const	{ return ptrEasternPoint; }				
	
	// sets the name of the information array
	// void setNameInfoArraj(string name_file);			
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Airdrome class ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Airdrome
{
private:
	uint8_t	count_vpp;							// number of runways					
	std::string  name_airdrome;					// aerodrome name
	Point* ptrAirRefPoint;						// aerodrome reference point pointer
	// unsigned int CalcHash(char *str);		// calculates the hash code of the string
	Runway* ptrRunway_1;						// runway 1
	Runway* ptrRunway_2;						// runway 2
public:
	Airdrome(); 
	
	// Airdrome(unsigned int num_info_arraj, string name_file) : id_info_arraj(num_info_arraj), name_info_arraj(name_file) { }
	// infoArjRecord(string name_file) : name_info_arraj(name_file) { id_info_arraj = CalcHash((char*)name_info_arraj.c_str()); }
	virtual ~Airdrome() { delete ptrRunway_1; delete ptrRunway_2; delete ptrAirRefPoint; }

	Runway* getPtrRunway(uint8_t	number_vpp) const		// returns a pointer to a runway
	{	
		if(number_vpp == 1)
			return ptrRunway_1;
		else if(number_vpp == 2)
			return ptrRunway_2;
		else
		{
			ern::Log::getInstance().Write("Aerodromes with more than two runways in our country are not present");
			return 0;
		}
	}	
	
	// returns a pointer to an aerodrome checkpoint
	Point* getPtrAirRefPoint() const { return ptrAirRefPoint; }     
	
	// sets the identification (ordinal) number of the information array
	void setIdInfoArraj(uint16_t num_info_arraj);   	
	
	// returns the name of the aerodrome
	std::string getNameAirdrome() const	{ return name_airdrome; }				
	
	// establishes the name of the aerodrome
	void setNameAirdrome(std::string name_airdrome)	{	this->name_airdrome = name_airdrome;	}			
};
//////////////////////////////////////////////////////////////////////////////////////
/////////////////  class RecCategory_11 - record category 11 Asterix	//////////////
/////////////////  identifier (index) - the name of the information array  ////////////

class RecCategory_11
{
private:	
	// unsigned int CalcHash(char *str);		// calculates the hash code of the string

public:
	uint8_t   type_save_data;					// data type for the current save
	uint8_t msg_type;							// data item I011 / 000
	data_source_id dsi;							// data item I011 / 010
	uint8_t service_id;							// data item I011 / 015
	position_wgs84 pos_wgs84;					// data item I011 / 041
	position_cartesian pos_cart;				// data item I011/042
	uint16_t mode_3A;							// data item I011/060
	double flight_level;						// data item I011/090
	double geo_altitude;						// data item I011/092
	baro_altitude baro_alt;						// data item I011/093
	double time_of_track;						// data item I011/140
	uint16_t track_number;						// data item I011/161
	track_status tr_status;						// data item I011/170
	track_velocity tr_velocity;					// data item I011/202
	calc_acceleration acceleration;				// data item I011/210
	double calc_vert_rate;						// data item I011/215
	call_sign _call_sign;						// data item I011/245
	target_size_orient target_size;				// data item I011/270
	ages _ages;									// data item I011/290
	int fleet;									// data item I011/300
	prog_msg _prog_msg;							// data item I011/310
	TAdsBRelatedData adsBRelatedData;			// data item I011/380

	RecCategory_11() { }
	
	~RecCategory_11() 
	{ 		
			delete adsBRelatedData.mode_s_msg;
	}	
};
/////////////////////////////////////////////////////////////////////////////////
///////////////// class Cat11Records - list of records of category 11 Asterix ///
/////////////////////////////////////////////////////////////////////////////////
class Cat11Records
{
private:
	std::vector<RecCategory_11> vectCat11Records;	

public:
	// adding a new entry
	void addCat11Record(RecCategory_11);					
	
	// returns the number of elements in the list
	uint16_t getCountElemList() const						
	{
		return vectCat11Records.size();
	}

	// sets the iterator to the beginning of the RecordCat11List
	// list<RecCategory_11>::iterator begin_iter();			
	
	// moves the iterator forward one index in the list
	// list<RecCategory_11>::iterator next_iter();			
	
	// clears the list of stored records
	void clearAll() { vectCat11Records.clear(); }				

	~Cat11Records() { vectCat11Records.clear(); }

	// returns a copy of the class instance numInfoArrRec 
	// numbered in the vector
	RecCategory_11 getCat11Record(uint16_t numCat11Rec);		
};
//////////////////////////////////////////////////////////////////////
///////////////Track class////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class Track
{
private:
	uint16_t  track_number;				// track number
	Cat11Records* ptrCat11Records;		// list of traces records
	bool isLocatedRunway1;				// true - is on runway1, false - is not on runway1
	bool isLocatedRunway2;				// true - located on runway2, 
										// false - not on runway2
public:
	Track() { ptrCat11Records = new Cat11Records(); isLocatedRunway1 = false; isLocatedRunway2 = false; }
	Track(uint16_t  track_number) 
	{ 
		this->track_number = track_number;
		ptrCat11Records = new Cat11Records();
		isLocatedRunway1 = false;
		isLocatedRunway2 = false;
	}
	Track(RecCategory_11 cat_11) 
	{ 
		this->track_number = cat_11.track_number;
		ptrCat11Records = new Cat11Records();
		ptrCat11Records->addCat11Record(cat_11);
		isLocatedRunway1 = false;
		isLocatedRunway2 = false;
	}
	
	// destructor, pre-deletes all entries from the list
	~Track() { ptrCat11Records->clearAll();	delete ptrCat11Records; }		
	
	// adding an entry to the list (new entry to the end of the list !!!)
	void addCat11Asterix(RecCategory_11);												

	void setTrackNumber(uint16_t  track_number) { this->track_number = track_number; }
	
	// return track number
	uint16_t getTrackNumber() const { return track_number; }						
	
	// returns a pointer to the list of records associated with the trace
	Cat11Records* getCat11Record() { return ptrCat11Records; }            		

	void setLocatedRunway1(bool isLocatedRunway1)	{ this->isLocatedRunway1 = isLocatedRunway1; }
	bool getLocatedRunway1() const  { return isLocatedRunway1; }

	void setLocatedRunway2(bool isLocatedRunway2)	{ this->isLocatedRunway2 = isLocatedRunway2; }
	bool getLocatedRunway2() const  { return isLocatedRunway2; }

	friend bool operator < (const Track&, const Track&);
	friend bool operator == (const Track&, const Track&);	
};
/////////////////////////////////////////////////////////////////////////////
//////////////////класс compareTrack/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// functional object for comparing traces (by their numbers)														
class compareTrack																			
{
public:
	bool operator() (Track*, Track*) const;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////TrackList class (list [set] of tracks)//////////////////////
//////////////// provides high-speed access to stored inf. arrays //////////////
////////////////////////////////////////////////////////////////////////////////
class TrackList
{
private:
	// Set pointers to trails
	std::set<Track*, compareTrack> setPtrsTrack;
	std::set<Track*, compareTrack>::iterator iter;

public:
	~TrackList();	// removing all traces from the list (freeing up resources)
		
	// adding a track to the list (a new track at the end of the list !!!)	
	int addTrack(Track* track);				
	
	// deleting a trace from the list according to its number
	int deleteTrack(uint16_t);												
	
	// returns a pointer to the trace by its number in the list
	Track* getTrack(uint16_t);		
	
	
	// returns the trace pointed to by the starting iterator
	Track* getBegTrack();			
	
	// returns the next trace according to the sequence of iterators
	Track* getNextTrack();		
	
	// returns the total number of traces
	uint16_t getNumberTraces() const;		

	// writes data about traces to a binary file
	// int saveTracesFile(const char* name_file);	
	
	// checks if a trace exists in the set with the specified number
	bool isTrackWithNumber(uint16_t num_track);					
	
	// checks the actualization of all traces, 
	// if the trace has not been updated for more than 15 seconds, 
	// then it must be deleted, where the actual_time attribute is the time 
	// in seconds relative to the beginning of the day
	void checkActualizationTraces(double actual_time);			
																						
	//  checks the runway occupied status by traversing all current (active) routes
	bool isRunwayEmploymentStatus(uint8_t number_vpp);		
	
	// clears the storage of traces
	void clearAllTracks();	
};
/////////////////////////////////////////////////////////////////////////////////////
///////the AsterixWorker class is designed to work with the 11th category ASTERIX////
/////////////////////////////////////////////////////////////////////////////////////
class AsterixWorker
{
private:
	
	// CSV file record counter 
	// is incremented until their number exceeds 30000 (MAX_NUMBER_ENTRIES_CSV), 
	// then it is zeroed
	int record_count;	
	
	// counter of generated csv files
	int table_count;	
	
	// calculation mode and recording in csv, 
	// taking into account a specified time period (write all records [all], 
	// record records with an interval of 100 ms [standard], 
	// write records with an interval of 500 ms [extended_period], etc.)
	int recording_mode_to_csv;		 
																												
	// size of the received ASTERIX-datagram of category 011
	int size_buff_Asterix_MsgCat011_;															
	
	// Buffer for incoming datagrams with information in the ASTERIX category 011
	char buffAsterixMsgCat011[BUFF_MESSAGE_INFO];		
	
	// the number of received traces for a one-second observation period 
	// for the analysis of conflict situations
	int count_msg_asterix_cat011;	
	
	// the name of the current csv file in which the work is in progress
	std::string name_csv_file;																		
	
	// time of receipt of the last message in seconds
	// double the_last_message;																	
	
	// start time of observation (time of receipt of the first message [dateagram]) 
	// after the completion of the last calculation or during the first period, 
	// the beginning of the time count (needed to determine the start of the calculation part)
	double the_first_message_arrives;															
																												
	// shows whether the start of the time count from the beginning of the observation 
	// is set from the beginning 
	// of the observation (the start of the service or after the end of the last calculation),
	// if false, the start of the observation has not been fixed yet
	bool isBegObservPeriodSpecified;	

	// list of traces
	TrackList* ptrTrackList; 		 
	
	// pointer to airfield (s)
	Airdrome* ptrAirdrome;																			
		
	// allocates the required amount of memory for ASTERIX category 11 data
	void init_sourse_data_cat011();																
	
	// parses a new record of category 11 Asterix, 
	// the data is added to the corresponding fields of the class, 
	// and the line is added to the csv file 
	void handleCat11(Cat11::Cat11* cat11);							
																												 
	
	void handleCat11(TRecCategory_11 *ptrCat11);
	
	// parses the received ASTERIX string of category 011 [buffAsterixMsgInfo]
	// void parseAsterixMsgCat11();																
	
	// loads the retrieved category 11 items into online storage
	bool isStartCalculation(RecCategory_11 &cat_11);										
	
	// the settlement part begins here						
	int StartCalculation() { isBegObservPeriodSpecified = false;	return 1; }
	
	void addStrToCSV(RecCategory_11 &cat_11);

public:
	AsterixWorker() {
		char name_file[20];
		isBegObservPeriodSpecified = false;
		// record_count = 1;
		record_count = ern::ConfiguratorApp::getInstance().getRecordCount();
		// table_count = 1;
		table_count = ern::ConfiguratorApp::getInstance().getTableCount();
		// recording_mode_to_csv = ALL_RECORDS;		// by default we write all records
		recording_mode_to_csv = ern::ConfiguratorApp::getInstance().getRecordingMode();
		size_buff_Asterix_MsgCat011_ = 0;
		memset(this->buffAsterixMsgCat011, 0, BUFF_MESSAGE_INFO);
		count_msg_asterix_cat011 = 0;

        #ifdef __linux__
        //linux code goes here
             const char* csv_file_cat11 = "cat11_";
        #elif _WIN32
        // windows code goes here

        #else
        // #error "OS not supported!"
        #endif

		name_csv_file = csv_file_cat11;
		sprintf(name_file, "%s%d", name_csv_file.c_str(), table_count);
		std::ofstream out_csv_file;
		out_csv_file.open(name_file, std::ios::app | std::ios::out);

		if(record_count == 1)
		{			
			std::string csv_string 
				= "msg_type;SAC;SIC;ServiceID;wgs84_lat;wgs84_lon;X;Y;Mode3A;flight_level;geo_altitude;QNH;baro_altitude;time_of_track;track_number;";
			csv_string 
				+= "MON;GBS;MRH;SRC;CNF;SIM;TSE;TSB;FRI/FOE;ME;MI;AMA;SPI;CST;FPC;AFF;PSR;SSR;MDS;ADS;SUC;AAC;VX;VY;AX;AY;RATE;STI;CALLSIGN;LENGTH;WIDTH;ORIENTATION;";
			csv_string 
				+= "PSR_Ages;SSR_Ages;MDA;MFL;MDS_Ages;ADS_Ages;ADB;MD1;MD2;LOP;TRK;MUL;VFI;TRB;MSG;REP;MB_DATA;ADDRESS;COM;STAS;SSC;ARC;AIC;B1A;B1B;AC;VVP1;VVP2\n";
			out_csv_file << csv_string;
			record_count++;
			ern::ConfiguratorApp::getInstance().setRecordCount(record_count);
		}

		ptrAirdrome = new Airdrome();
		ptrTrackList = new TrackList();
	}
	
	virtual ~AsterixWorker(){
		this->clearAllDataCat011(); delete ptrAirdrome; delete ptrTrackList; 
	}
	
	// updates the buffAsterixMsgInfo array with new data  
	void setAsterixMSG(const char* ptrBuffAsterixMsg, int size_buff_Asterix_Msg);								
	
	// clears the online storage of category 011 data for traces
	void clearAllDataCat011();																			

	void setRecordingMode(int recording_mode_to_csv) 
	{
		this->recording_mode_to_csv = recording_mode_to_csv;
	}
	
	int getRecordingMode() const { return recording_mode_to_csv;}
	
	// parses the received ASTERIX string of category 011 [buffAsterixMsgInfo]
	void parseAsterixMsgCat11();																									
	
	
	void parseAsterixMsgCat11(TRecCategory_11 *ptrCat11);
	
	
	int getRecordCount() const { return record_count; }
	
	
	int getTableCount() const { return this->table_count; }
	
	
	void drawTargetSituation();
};

int server(AsterixWorker* ptrAsterixWorker);

// allows testing the mechanism when working with local data
int testLocalMeh(AsterixWorker* ptrAsterixWorker);																																

#endif /* ANALYZER_CONFLICT_SERVICE_H_ */
