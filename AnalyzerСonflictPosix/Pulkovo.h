#ifndef _PULKOVO_H_
#define _PULKOVO_H_

// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 14.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

#include <stdlib.h>
#include <math.h> 
#include "Log.h"
#include "GeoCalc.h"


void WgsToLocPr(double Lat, double Lon, double *pX, double *pY);
//---------------
// Conversion of geodetic coordinates from the P3-90.02 system to the WGS-84 system
// recalculation of latitude, longitude and altitude
int P3_90_02_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, 
	double *pLonOut);

// Conversion of geodetic coordinates from the WGS-84 system to the P3-90.02 system
// пересчет широты, долготы и высоты
int WGS_84_P3_90_02(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, 
	double *pLonOut);
//---------------
// Conversion of coordinates from latitude / longitude to Mercator projection / WGS84
std::string LatLong2UTMMerc(double lon, double lat, double east_ref_point, 
	double north_ref_point, double *pEasting, double *pNorthing, double *pX, double *pY);

// converts UTM to WGS-84 geographic coordinates (latitude and longitude)
void UTM2LatLongMerc(double kta_x, double kta_y, double east_ref_point, 
	double north_ref_point, double *pLat, double *pLon);
//---------------
int FuzzyWgsToLocPlkv(double Lat, double Lon, double *pX, double *pY);
int FuzzyPlkvLocToWgs(double X, double Y, double *pLat, double *pLon);
//---------------------------------------------
int WgsToLocPlkv_zone_1(double Lat, double Lon, double *pX, double *pY);
int WgsToLocPlkv_zone_2(double Lat, double Lon, double *pX, double *pY);
int WgsToLocPlkv_zone_3(double Lat, double Lon, double *pX, double *pY);
int WgsToLocPlkv_zone_4(double Lat, double Lon, double *pX, double *pY);
//-------------------------------------------
int PlkvLocToWgs_zone_1(double X, double Y, double *pLat, double *pLon);
int PlkvLocToWgs_zone_2(double X, double Y, double *pLat, double *pLon);
int PlkvLocToWgs_zone_3(double X, double Y, double *pLat, double *pLon);
int PlkvLocToWgs_zone_4(double X, double Y, double *pLat, double *pLon);
//---------------------------------------------
// squares for the first zone
void WgsToLocPlkv_zone_1_square_11(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_1_square_25(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_1_square_26(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_1_square_27(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_1_square_34(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_1_square_35(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_1_square_36(double Lat, double Lon, double *pX, double *pY);
//--------------------------------------------
void PlkvLocToWgs_zone_1_square_11(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_1_square_25(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_1_square_26(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_1_square_27(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_1_square_34(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_1_square_35(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_1_square_36(double X, double Y, double *pLat, double *pLon);
//--------------------------------------------
// squares for the second zone
void WgsToLocPlkv_zone_2_square_1(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_2_square_2(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_2_square_14(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_2_square_76(double Lat, double Lon, double *pX, double *pY);
//--------------------------------------------
void PlkvLocToWgs_zone_2_square_1(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_2_square_2(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_2_square_14(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_2_square_76(double X, double Y, double *pLat, double *pLon);
//--------------------------------------------
// squares for the third zone
void WgsToLocPlkv_zone_3_square_1(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_3_square_14(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_3_square_15(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_3_square_29(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_3_square_31(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_3_square_32(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_3_square_33(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_3_square_45(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_3_square_46(double Lat, double Lon, double *pX, double *pY);
//----------------------------------------------
void PlkvLocToWgs_zone_3_square_1(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_3_square_14(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_3_square_15(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_3_square_29(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_3_square_31(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_3_square_32(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_3_square_33(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_3_square_45(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_3_square_46(double X, double Y, double *pLat, double *pLon);
//----------------------------------------------
// squares for the fourth zone
void WgsToLocPlkv_zone_4_square_29(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_4_square_30(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_4_square_35(double Lat, double Lon, double *pX, double *pY);
void WgsToLocPlkv_zone_4_square_36(double Lat, double Lon, double *pX, double *pY);
//----------------------------------------------
void PlkvLocToWgs_zone_4_square_29(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_4_square_30(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_4_square_35(double X, double Y, double *pLat, double *pLon);
void PlkvLocToWgs_zone_4_square_36(double X, double Y, double *pLat, double *pLon);
// }
#endif /* _PULKOVO_H_ */