/** author-developer: Roman Ermakov */

#ifndef _PULKOVO_H_
#define _PULKOVO_H_

#include <stdlib.h>
#include <math.h> 
#include "Log.h"
#include "GeoCalc.h"

// namespace airports{

void WgsToLocPr(double Lat, double Lon, double *pX, double *pY);
// void PrLocToWgs(double X, double Y, double *pLat, double *pLon);
//---------------
// Преобразование геодезических координат из системы П3-90.02 в систему WGS-84
// пересчет широты, долготы и высоты
int P3_90_02_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut);
// Преобразование геодезических координат из системы WGS-84 в систему П3-90.02
// пересчет широты, долготы и высоты
int WGS_84_P3_90_02(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut);
//---------------
// Пересчет координат из широты/долготы в проекцию Меркатора/WGS84
std::string LatLong2UTMMerc(double lon, double lat, double east_ref_point, double north_ref_point, double *pEasting, double *pNorthing, double *pX, double *pY);
// переводит UTM в географические координаты WGS-84 (широту и долготу) 
// void UTM2LatLongMerc(std::string longZone, std::string latZone, double easting, double northing, double *pLat, double *pLon);
void UTM2LatLongMerc(double kta_x, double kta_y, double east_ref_point, double north_ref_point, double *pLat, double *pLon);
// void UTM2LatLongMerc(std::string UTM, double *pLat, double *pLon);
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
// квадраты для первой зоны
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
// квадраты для второй зоны
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
// квадраты для третьей зоны
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
// квадраты для четвертой зоны
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