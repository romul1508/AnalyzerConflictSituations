// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

#ifndef _GEOCALC_H_
#define _GEOCALC_H_

#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include <vector>
// #include "Common.h"
//#include "Log.h"

// namespace airports{
//const double Pi = 3.14159265358979;		
//const double PI_2 = Pi * 0.5;
//const double MAX_LAT = 89.5;
//const double R_MAJOR = 6378137.0;
//const double R_MINOR = 6356752.3142;
//const double ECCENT = sqrt(1 - pow(R_MINOR / R_MAJOR, 2));
//const double ECCNTH = ECCENT * 0.5;
//const double DEG_RAD = Pi/180;
//const double RAD_DEG = 180/Pi;

// but still we will bind to the upper left point of the controlled territory, 
// given that we have our own geocalc

const double MyLAT = 59.8090672;			
const double MyLON = 30.2110079;

enum type_system_coordinat 
{ 
	WGS_84, 
	SK_42, 
	P3_90_02, 
	P3_90_11, 
	main_XYZ, 
	CPA, 
	CPA_GAUSE, 
	CPA_MERCATOR_WGS, 
	CPA_MERCATOR_DOMAIN 
};

// point in a rectangular system with zero at the center of the ellipsoid in meters
typedef struct 
{
    double X;		
    double Y;		
    double Z;
} TXYZSystemPoint;
//-----------------------------
// point in geodetic system SK-42 (longitude and latitude in degrees)
typedef struct 
{
	// longitude in the SK-42 system (in degrees in decimal)
	double sk42_X_lon;						
	
	// latitude in SK-42 system (in degrees in decimal system)
	double sk42_Y_lat;									
	
	// height from ellipsoid surface
	double height;							
	
	// longitude in degrees (whole part)
	int degrees_lon;							
	
	// latitude in degrees (whole part)
	int degrees_lat;							
	
	// longitude in minutes
	unsigned char minutes_x_lon;
	
	// latitude in minutes
	unsigned char minutes_y_lat;		
	
	// longitude in seconds
	double seconds_x_lon;				
	
	// latitude in seconds
	double seconds_y_lat;					
} TSK42Point;
//-----------------------------
// point in the geodetic system PZ-90.02 (longitude and latitude in degrees)
typedef struct 
{
        // longitude in P3-90.02 system (in degrees in decimal system)
	double p3_90_02_X_lon;			
	
	// latitude in P3-90.02 system (in degrees in decimal system)
	double p3_90_02_Y_lat;				
	
	// height from ellipsoid surface
	double height;							
	
	// longitude in degrees (whole part)
	int degrees_lon;
	
	// latitude in degrees (whole part)
	int degrees_lat;
	
	// longitude in minutes
	unsigned char minutes_x_lon;
	
	// latitude in minutes
	unsigned char minutes_y_lat;
	
	// longitude in seconds
	double seconds_x_lon;
	
	// latitude in seconds
	double seconds_y_lat;					
} TP3_90_02Point;
//-----------------------------
// point in the geodetic system P3-90.11 (longitude and latitude in degrees)
typedef struct 
{
        // longitude in P3-90.11 system (in degrees in decimal system)
	double p3_90_11_X_lon;			
	
	// latitude in P3-90.11 system (in degrees in decimal system)
	double p3_90_11_Y_lat;				
	
	// height from ellipsoid surface
	double height;	
	
	// longitude in degrees (whole part)
	int degrees_lon;
	
	// latitude in degrees (whole part)
	int degrees_lat;							
	
	// longitude in minutes
	unsigned char minutes_x_lon;		
	
	// latitude in minutes
	unsigned char minutes_y_lat;		
	
	// longitude in seconds
	double seconds_x_lon;				
	
	// latitude in seconds
	double seconds_y_lat;					
} TP3_90_11Point;
//-----------------------------
// point in the geodetic system WGS-84 (longitude and latitude in degrees)
typedef struct 
{
        // longitude in WGS-84 system (in degrees in decimal system)
	double wgs_X_lon;							
	
	// latitude in WGS-84 system (in degrees in decimal system)
	double wgs_Y_lat;							
	
	// height from ellipsoid surface
	double height;								
	
	// longitude in degrees (whole part)
	int degrees_lon;								
	
	// latitude in degrees (whole part)
	int degrees_lat;
	
	// longitude in minutes
	unsigned char minutes_x_lon;			
	
	// latitude in minutes
	unsigned char minutes_y_lat;
	
	// longitude in seconds
	double seconds_x_lon;
	
	// latitude in seconds
	double seconds_y_lat;						
} TWgsPoint;
//----------------------------
//point in the KTA system in meters (longitude and latitude converted to a flat local coordinate system)
typedef struct 
{
    	double kta_X; 
	double	kta_Y;
	
	// height from the surface of the ellipsoid (in the WGS-84 system)
	double height_84;
	
	// height from the surface of the ellipsoid (in the SK-42 system)
	double height_42;				
} TKtaPoint;
//----------------------------
// point in UTM format
typedef struct
{
	std::string longZone;
	std::string latZone;
	double easting;
	double northing;
} TUtmPoint;
//----------------------------
// Target location in minutes and seconds in latitude 
// or longitude (can be used for various geodetic coordinate systems)
typedef struct 
{
    	int degrees;							
	unsigned char minutes;			
	double seconds;							
} TTargetLocationInMinutes;
//--------------------------------------
template <typename T>
std::string toString(T val)
{
    std::ostringstream oss;
    oss<< val;
    return oss.str();
}
//-------------------------------------
template<typename T> 
T fromString(const std::string& s) 
{
  std::istringstream iss(s);
  T res;
  iss >> res;
  return res;
}
//----------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////UTM2LatLon class (Class for conversions from UTM to latitude / longitude)////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
class UTM2LatLon
{
private:
    double easting;
    double northing;
    int zone;
    double arc;
    double mu;
    double ei;
    double ca;
    double cb;
    double cc;
    double cd;
    double n0;
    double r0;
    double _a1;
    double dd0;
    double t0;
    double Q0;
    double lof1;
    double lof2;
    double lof3;
    double _a2;
    double phi1;
    double fact1;
    double fact2;
    double fact3;
    double fact4;
    double zoneCM;
    double _a3;
    double Pi;		
    double b;		
    double a;		
    double e;		
    double e1sq;	
    double k0;		
   std::string southernHemisphere;	

   double strToDouble(std::string token);	

protected:
	std::string getHemisphere(std::string latZone);
	void setVariables();    

public:
	UTM2LatLon()
	{
		Pi = 3.14159265358979;		
		b  = 6356752.314;
		a  = 6378137;
		e  = 0.081819191;
		e1sq = 0.006739497;
		k0 = 0.9996;
		southernHemisphere = "ACDEFGHJKLM";
	}

	// converts coordinates from UTM format to geographic coordinates WGS-84
	void convertUTMToLatLong(std::string UTM, double *pLat, double *pLon);	
	
	void convertUTMToLatLong(std::string longZone, std::string latZone, double easting, double northing, double *pLat, double *pLon);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Класс	LatZones (вспомогательный класс для вывода информации о зоне по долготе)///////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LatZones
{
private:
	
	char letters[22];		// = { 'A', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Z' };
	int degrees[22];		// = { -90, -84, -72, -64, -56, -48, -40, -32, -24, -16, -8, 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 84 };
	char negLetters[11];	// = { 'A', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M' };
	int negDegrees[11];	// = { -90, -84, -72, -64, -56, -48, -40, -32, -24, -16, -8 };
	char posLetters[11];		// = { 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Z' };
	int posDegrees[11];	// = { 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 84 };
	int arrayLength;	// = 22;
	int posLettersLength;	// = 11;
	int negLettersLength;	// = 11;

	// проверяет на корректность широту и долготу
	// bool validate(double latitude, double longitude);
	

public:
	LatZones(){
		letters[0] = 'A';		letters[1] = 'C';		letters[2] = 'D';		letters[3] = 'E';
		letters[4] = 'F';		letters[5] = 'G';		letters[6] = 'H';		letters[7] = 'J';
		letters[8] = 'K';		letters[9] = 'L';		letters[10] = 'M';		letters[11] = 'N';
		letters[12] = 'P';		letters[13] = 'Q';		letters[14] = 'R';		letters[15] = 'S';
		letters[16] = 'T';		letters[17] = 'U';		letters[18] = 'V';		letters[19] = 'W';
		letters[20] = 'X';		letters[21] = 'Z';
		// = { 'A', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Z' };

		degrees[0] = -90;	degrees[1] = -84;	degrees[2] = -72;	degrees[3] = -64;
		degrees[4] = -56;	degrees[5] = -48;	degrees[6] = -40;	degrees[7] = -32;
		degrees[8] = -24;	degrees[9] = -16;	degrees[10] = -8;	degrees[11] = 0;
		degrees[12] = 8;	degrees[13] = 16;	degrees[14] = 24;	degrees[15] = 32;
		degrees[16] = 40;	degrees[17] = 48;	degrees[18] = 56;	degrees[19] = 64;
		degrees[20] = 72;	degrees[21] = 84;
		//  = { -90, -84, -72, -64, -56, -48, -40, -32, -24, -16, -8, 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 84 };

		negLetters[0] = 'A';		negLetters[1] = 'C';		negLetters[2] = 'D';		negLetters[3] = 'E';
		negLetters[4] = 'F';		negLetters[5] = 'G';		negLetters[6] = 'H';		negLetters[7] = 'J';
		negLetters[8] = 'K';		negLetters[9] = 'L';		negLetters[10] = 'M';		
		// char negLetters[11];	// = { 'A', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M' };

		negDegrees[0] = -90;		negDegrees[1] = -84;		negDegrees[2] = -72;		negDegrees[3] = -64;
		negDegrees[4] = -56;		negDegrees[5] = -48;		negDegrees[6] = -40;		negDegrees[7] = -32;
		negDegrees[8] = -24;		negDegrees[9] = -16;	negDegrees[10] = -8;
		// int negDegrees[11];	// = { -90, -84, -72, -64, -56, -48, -40, -32, -24, -16, -8 };

		posLetters[0] = 'N';		posLetters[1] = 'P';		posLetters[2] = 'Q';		posLetters[3] = 'R';
		posLetters[4] = 'S';		posLetters[5] = 'T';		posLetters[6] = 'U';		posLetters[7] = 'V';
		posLetters[8] = 'W';	posLetters[9] = 'X';		posLetters[10] = 'Z';
		// char posLetters[11];		// = { 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Z' };

		posDegrees[0] = 0;	posDegrees[1] = 8;	posDegrees[2] = 16;	posDegrees[3] = 24;	
		posDegrees[4] = 32;	posDegrees[5] = 40;	posDegrees[6] = 48;	posDegrees[7] = 56;	
		posDegrees[8] = 64;	posDegrees[9] = 72;	posDegrees[10] = 84;
		// int posDegrees[11];		// = { 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 84 };

		arrayLength = 22;
		posLettersLength = 11;
		negLettersLength = 11;
	}

	int getLatZoneDegree(std::string letter);
	std::string getLatZone(double latitude);

};
//---------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////Класс	LatLon2UTM (Класс для преобразований широты/долготы в UTM)///////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
class LatLon2UTM
{
private:
	double Pi;		// число Пи
	
	//-------------------------------------
	// Lat Lon to UTM variables

    // equatorial radius
    double equatorialRadius;		// = 6378137;

    // polar radius
    double polarRadius;			// = 6356752.314;

    // flattening
    double flattening;				// = 0.00335281066474748;// (equatorialRadius-polarRadius)/equatorialRadius;

    // inverse flattening 1/flattening
    double inverseFlattening;		// = 298.257223563;// 1/flattening;

    // Mean radius
    double rm;							// = POW(equatorialRadius * polarRadius, 1 / 2.0);

    // scale factor
    double k0;							// = 0.9996;

    // eccentricity
    double e;							// = Math.sqrt(1 - POW(polarRadius / equatorialRadius, 2));

    double e1sq;						// = e * e / (1 - e * e);

    double n;							// = (equatorialRadius - polarRadius) / (equatorialRadius + polarRadius);

    // r curv 1
    double rho;						// = 6368573.744;

    // r curv 2
    double nu;							// = 6389236.914;

    // Calculate Meridional Arc Length
    // Meridional Arc
    double S;							// = 5103266.421;

    double A0;							// = 6367449.146;

    double B0;							// = 16038.42955;

    double C0;							// = 16.83261333;

    double D0;							// = 0.021984404;

    double E0;							// = 0.000312705;

    // Calculation Constants
    // Delta Long
    double p;							// = -0.483084;

    double sin1;						// = 4.84814E-06;

    // Coefficients for UTM Coordinates
    double K1;							// = 5101225.115;

    double K2;							// = 3750.291596;

    double K3;							// = 1.397608151;

    double K4;							// = 214839.3105;

    double K5;							// = -2.995382942;

    double A6;							// = -1.00541E-07;

	// проверяет на корректность широту и долготу
	bool validate(double latitude, double longitude);

protected:
	void setVariables(double latitude, double longitude);
	std::string getLongZone(double longitude);
	double getEasting() {	return 500000 + (K4 * p + K5 * pow(p, 3));  }
	double getNorthing(double latitude);
    /*{
      double northing = K1 + K2 * p * p + K3 * POW(p, 4);
      if (latitude < 0.0)
      {
        northing = 10000000 + northing;
      }
      return northing;
    }*/

public:
	LatLon2UTM(){
		Pi = 3.14159265358979;		// число Пи

		// Lat Lon to UTM variables

		// equatorial radius
		equatorialRadius = 6378137;

		// polar radius
		polarRadius = 6356752.314;

		// flattening
		flattening = 0.00335281066474748;		// (equatorialRadius-polarRadius)/equatorialRadius;

		// inverse flattening 1/flattening
		inverseFlattening = 298.257223563;		// 1/flattening;

		// Mean radius
		rm = pow(equatorialRadius * polarRadius, 1 / 2.0);

		// scale factor
		k0 = 0.9996;

		// eccentricity
		e = sqrt(1 - pow(polarRadius / equatorialRadius, 2));

		e1sq = e * e / (1 - e * e);

		n = (equatorialRadius - polarRadius) / (equatorialRadius + polarRadius);

		// r curv 1
		rho = 6368573.744;

		// r curv 2
		nu = 6389236.914;

		// Calculate Meridional Arc Length
		// Meridional Arc
		S = 5103266.421;
		A0 = 6367449.146;
		B0 = 16038.42955;
		C0 = 16.83261333;
		D0 = 0.021984404;
		E0 = 0.000312705;

		// Calculation Constants
		// Delta Long
		p = -0.483084;
	    sin1 = 4.84814E-06;

		// Coefficients for UTM Coordinates
		K1 = 5101225.115;
		K2 = 3750.291596;
		K3 = 1.397608151;
		K4 = 214839.3105;
		K5 = -2.995382942;
		A6 = -1.00541E-07;
	}

	// переводит широту и долготу в UTM
	std::string convertLatLonToUTM(double latitude, double longitude, double *pX, double *pY);

	double degreeToRadian(double degree) {	return degree * Pi / 180;  }

};
//---------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Класс	CoordinateUTMConversion (Класс для преобразований широты/долготы в UTM и обратно)///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CoordinateUTMConversion
{
private:
	
	// проверяет на корректность широту и долготу
	bool validate(double latitude, double longitude);
	

public:
	CoordinateUTMConversion(){	}

	// переводит широту и долготу в UTM
	std::string latLon2UTM(double lat, double lon, double* x, double* y);
	// переводит UTM в географические координаты WGS-84 (широту и долготу) 
    void utm2LatLon(std::string longZone, std::string latZone, double easting, double northing, double *pLat, double *pLon);
    void utm2LatLon(std::string UTM, double *pLat, double *pLon);
};
//---------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Класс	GeoCalc (Класс для работы с методами обработки геодезической информации)///////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GeoCalc
{
private:
	double Pi;		// число Пи
	double PI_2;
	double MAX_LAT;
	double R_MAJOR;
	double R_MINOR;
	double ECCENT;
	double ECCNTH;
	double DEG_RAD;
	double RAD_DEG;

	double F1(double lat, double cosLon, double e2, double a, double x)
	{
		double f1, Pi, B, sinLat, sin2Lat, cosLat;
		// e = 0.0066934216;
		// a = 6378245;
		Pi = 3.14159265358979;		// число Пи
		B = lat * DEG_RAD;
		sinLat = sin(B);
		cosLat = cos(B);
		sin2Lat = pow(sinLat, 2);

		// далее рассчитываются вспомогательные переменные
		double var_temp1 = a * (1 - e2);
		double var_temp2 = sqrt(1 - e2 * sin2Lat);
		double var_temp3 = cosLat * cosLon;

		// f1 = ( (a * (1-e2))/sqrt(1-e2*k*k) +h) * k;

		f1 = (var_temp1/var_temp2 +  x/var_temp3 - a/var_temp2) * sinLat;
		// return ((k + h*m)/m)*sqrt( (1- m*m)/e );
		return f1;
	}
	//-------------------------------------------------
	double F2(double z)
    {
		return z;
	}
	//-------------------------------------------------
	double Diff(double lat, double cosLon, double z, double e2, double a, double x)
	{
		// F1(double lat, double cosLon, double e2, double a, double x)
		return F1(lat, cosLon, e2, a, x) - F2(z);
	}
	//-------------------------------------------------
	// находит нужный корень долготы для различных систем координат в основной системе уравнений 
	// int Find_Root(double A, double B, double h, double z, double e2, double a, double* m);

public:
	GeoCalc(){
		Pi = 3.14159265358979;		// число Пи
		PI_2 = Pi * 0.5;
		MAX_LAT = 89.5;
		R_MAJOR = 6378137.0;
		R_MINOR = 6356752.3142;
		ECCENT = sqrt(1 - pow(R_MINOR / R_MAJOR, 2));
		ECCNTH = ECCENT * 0.5;
		DEG_RAD = Pi/180.0;
		RAD_DEG = 180.0/Pi;
	}
// Эллипсоид Красовского

// находит нужный корень долготы для различных систем координат в основной системе уравнений 
int Find_Root(double A, double B, double h, double z, double e2, double a, double x, double* m);
// пересчет координат из системы WGS-84 в прямоугольные (проекция Гаусса-Крюгера)
void WgsToPrGaus(double Lat, double Lon, double *pX, double *pY);
// void PrLocToWgs(double X, double Y, double *pLat, double *pLon);
// пересчет координат из системы CK-42 в прямоугольные (проекция Гаусса-Крюгера)
void SK42ToPrGaus(double Lat, double Lon, double *pX, double *pY);
// преобразование плоских прямоугольных координат в проекции Гаусса - Крюгера
// в геодезические координаты CK-42 на эллипсоиде Красовского
void PrGausToSK42(double X, double Y, double *pLat, double *pLon);
//---------------------------------------
// Пересчет координат из широты/долготы в поперечную проекцию Меркатора/WGS84
void LatLongUTMMerc(double lon, double lat, double* x, double* y);
//---------------------------------------
// пересчет координат из системы WGS-84 в прямоугольные X и Y (проекция Меркатора)
double WgsToPrMercEllipse_X(double Lon)
{
	return Lon * DEG_RAD * R_MAJOR;
}
//---------------------------------------
double PrMercEllipseXToWgs_Lon(double X)
{
	return X * RAD_DEG / R_MAJOR;
}
//---------------------------------------
double WgsToPrMercEllipse_Y(double Lat);
double PrMercEllipseYToWgs_Lat(double Y);
//---------------------------------------
// Пересчет координат из широты/долготы в проекцию Меркатора/WGS84
std::string LatLong2UTMMerc(double lon, double lat, double* pX, double* pY);
// переводит UTM в географические координаты WGS-84 (широту и долготу) 
void UTM2LatLongMerc(std::string longZone, std::string latZone, double easting, double northing, double *pLat, double *pLon);
void UTM2LatLongMerc(std::string UTM, double *pLat, double *pLon);
//---------------------------------------
// для сферы
double WgsToPrMercDomain_X(double Lon)
{
	return Lon * DEG_RAD * R_MAJOR;
}
//----------------------------------------
double PrMercDomainXToWgs_Lon(double X)
{
	double a = 6370997.0;
	return X * RAD_DEG / a;
}
//----------------------------------------
double WgsToPrMercDomain_Y(double Lat);
double PrMercDomainYToWgs_Lat(double Y);
//----------------------------------------
// Преобразование геодезических координат из системы П3-90.02 в систему СК-42
// пересчет широты
// double P3_90_02_SK42_Lat(double Lat, double Lon, double H);
// Преобразование геодезических координат из системы П3-90.02 в систему СК-42
// пересчет долготы
// double P3_90_02_SK42_Lon(double Lat, double Lon, double H);
// пересчет широты, долготы и высоты
int P3_90_02_SK42(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut);
int SK42_P3_90_02(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut);
//----------------------------------------
// Преобразование геодезических координат из системы СК-42 в систему П3-90.02
// пересчет широты
double SK42_P3_90_02_Lat(double Lat, double Lon, double H);
// пересчет долготы
double SK42_P3_90_02_Lon(double Lat, double Lon, double H);
//----------------------------------------
// Преобразование геодезических координат из системы П3-90.02 в систему WGS-84
// пересчет широты, долготы и высоты
int P3_90_02_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut);
int P3_90_02_WGS_84(double Lat, double Lon, double H, double dX, double dY, double dZ, double Wx, double Wy, double Wz, double m, double *ph_wgs, double *pLatOut, double *pLonOut);
int P3_90_02_P3_90_11_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut);
// пересчет широты
// double P3_90_02_WGS_84_Lat(double Lat, double Lon, double H);
// пересчет долготы
// double P3_90_02_WGS_84_Lon(double Lat, double Lon, double H);
//----------------------------------------
// Преобразование геодезических координат из системы WGS-84 в систему П3-90.02
// пересчет широты, долготы и высоты
int WGS_84_P3_90_02(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut);
// пересчет широты
// double WGS_84_P3_90_02_Lat(double Lat, double Lon, double H);
// пересчет долготы
// double WGS_84_P3_90_02_Lon(double Lat, double Lon, double H);
//----------------------------------------
// Более точное преобразование геодезических координат из системы CK-42 в систему WGS-84
// пересчет широты
double SK_42_Sup_WGS_84_Lat(double Lat, double Lon, double H);
// пересчет долготы
double SK_42_Sup_WGS_84_Lon(double Lat, double Lon, double H);
//----------------------------------------
// Более точное преобразование геодезических координат из системы WGS-84 в систему CK-42
// пересчет широты
double WGS_84_Sup_Sk_42_Lat(double Lat, double Lon, double H);
// пересчет долготы
double WGS_84_Sup_Sk_42_Lon(double Lat, double Lon, double H);

//	выдать местоположение цели в минутах и секундах 
void GetTargetLocationInMinutes(TTargetLocationInMinutes* pTargetLocationInMinutes, const double lon_lat_dec);

//	выдать местоположение цели в градусах в десятичных дробях 
double GetDecTargetLocation(TTargetLocationInMinutes* pTargetLocationInMinutes);
};
//---------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Класс	 SimplifiedGeoCalc (Класс для работы с упрощенными методами обработки геодезической информации)///////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimplifiedGeoCalc
{
private:
	TKtaPoint 	kta_point;								// координаты точки в системе КTА
	TXYZSystemPoint xyz_point;					// координаты в прямоугольной системе координат относительно центра элипсоида
	TSK42Point 	sk_42_point;						// координаты точки в системе SK-42					
	TWgsPoint	wgs_84_point;						// координаты точки в системе WGS-84
	double height;										// высота от поверхности элипсоида

	// Эллипсоид Красовского
	double aP;																// Большая полуось
	double alP() { return 1 / 298.3; }								// Сжатие
	double e2P() { return (2 * alP() - alP() * alP() ); }		// Квадрат эксцентриситета
	//--------------------------------------------
	// Эллипсоид WGS84 (GRS80, эти два эллипсоида сходны по большинству параметров)
	double aW;																	// Большая полуось
	double alW() { return 1 / 298.257223563; }						// Сжатие
	double e2W() { return (2 * alW() - alW() *  alW() ); }		// Квадрат эксцентриситета
	//--------------------------------------------
	// Вспомогательные значения для преобразования эллипсоидов
	double a() { 
		double a = (aP + aW) / 2; 
		return a; 
	}
	double e2() { 
		double e2 = (e2P() + e2W() ) / 2; 
		return e2;
	}
	double da() { 
		double da = aW - aP; 
		return da;
	}
	double de2() { 
		double de2 =e2W() - e2P();
		return de2;
	}
	//-------------------------------------------
	// Линейные элементы трансформирования, в метрах
	double dx;  
	double dy;
	double dz; 
	//------------------------------------------ 
	// Угловые элементы трансформирования, в секундах
	double wx;
	double wy;
	double wz;
	//-----------------------------------------
	// Дифференциальное различие масштабов
	double ms;
	//-----------------------------------------
	double dB(double Bd, double Ld, double H);	
	double dL(double Bd, double Ld, double H); 
	
	double Wgs84_SK42_Lat(double Bd, double Ld, double H){		// вычисляет широту в системе Sk42
		double WGS84_SK42_Lat = Bd - dB(Bd, Ld, H) / 3600;
		return WGS84_SK42_Lat;
	}

	double SK42_WGS84_Lat(double	Bd, double Ld, double H) {	// вычисляет широту в системе WGS84
		double SK42_WGS84_Lat = Bd + dB(Bd, Ld, H) / 3600;
		return SK42_WGS84_Lat;
	}

	double WGS84_SK42_Long(double Bd, double Ld, double H){		// вычисляет долготу в системе Sk42
		double WGS84_SK42_Long = Ld - dL(Bd, Ld, H) / 3600;
		return WGS84_SK42_Long;
	}

	double SK42_WGS84_Long(double Bd, double Ld, double H){		// вычисляет долготу в системе WGS84
		double SK42_WGS84_Long = Ld + dL(Bd, Ld, H) / 3600;
		return SK42_WGS84_Long;
	}

	// пересчитывает координаты из прямоугольной (глобальной) XYZ системы в координаты в системе СК42
	int XYZ_SK42(TXYZSystemPoint *ptrXYZSystemPoint, TSK42Point *ptrSK42Point);

	// пересчитывает координаты из системы СК42 в прямоугольную (глобальную) XYZ
	void SK42_XYZ(TSK42Point *ptrSK42Point, TXYZSystemPoint *ptrXYZSystemPoint);

	double F1(double lat, double h)
	{
		double f1, e, a, Pi, B, k;
		e = 0.0066934216;
		a = 6378245;
		Pi = 3.14159265358979;		// число Пи
		B = lat * Pi / 180;
		k = sin(B);
		f1 = ( (a * (1-e))/sqrt(1-e*k*k) +h) * k;
		// return ((k + h*m)/m)*sqrt( (1- m*m)/e );
		return f1;
	}
	//-------------------------------------------------
	double F2(double z)
    {
		return z;
	}
	//-------------------------------------------------
	double Diff(double lat, double h, double z)
	{
		return F1(lat, h) - F2(z);
	}
	//-------------------------------------------------
	int Find_Root(double A, double B, double h, double z, double *m);


public:
	SimplifiedGeoCalc(double lat, double lon, double height, double Z, int type_coord);	
	
	virtual ~SimplifiedGeoCalc() { }

	double getWgs84_Lon() const  { return wgs_84_point.wgs_X_lon; }		// возвращает долготу в формате Wgs84
	double getWgs84_Lat() const  { return wgs_84_point.wgs_Y_lat; }			// возвращает широту в формате Wgs84
	double getSK42_Lon() const	 { return sk_42_point.sk42_X_lon; }			// возвращает долготу в формате CK-42
	double getSK42_Lat() const	 { return sk_42_point.sk42_Y_lat; }				// возвращает широту в формате CK-42
	double getHeight() const     { return height; }										// возвращает высоту от поверхности эллипсоида
	double getTXYZSystem_x()const { return xyz_point.X; }
	double getTXYZSystem_y()const { return xyz_point.Y; }
	double getTXYZSystem_z()const { return xyz_point.Z; }
	double getCPA_x() const	{	return this->kta_point.kta_X; }
	double getCPA_y() const	{	return this->kta_point.kta_Y; }
};
// }
#endif /* _GEOCALC_H_ */
