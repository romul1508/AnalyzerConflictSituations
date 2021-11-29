#include "AnalyzerConflictService.h"

#ifdef __linux__ 
	//linux code goes here
	
#elif _WIN32
	// windows code goes here
	#include <commctrl.h>
	#pragma comment(lib, "comctl32.lib")
	#pragma comment(lib, "ws2_32.lib")	
#else
// #error "OS not supported!"
#endif

// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

ern::LoggerDestroyer ern::Log::destroyer;
ern::ConfiguratorAppDestroyer ern::ConfiguratorApp::conf_app_destroyer;
ern::ConfiguratorAirportsDestroyer ern::ConfiguratorAirports::conf_airports_destroyer;

//-------------------------------------------------------------------------
/*
unsigned int CalcHash(char *str)
{
	unsigned int hash = 0;
	char* copystr = str;

	while(*copystr)
    {
		hash = ((hash << 7) & (DWORD)(-1))|(hash >> (32-7));
		hash = hash^(*copystr);
		copystr++;
    }

	return hash;
}
*/
//--------------------------------------------------------------------------
double strToDouble(std::string token)
{
	// получает строку, меняет ',' на '.', переводит строку в double
	int n;
	double value_token;
	std::string temp_str; 
	
	temp_str = token;
    n = temp_str.find(',');
	if(n != -1)
		temp_str.replace(n, 1, ".");
	value_token = atof( temp_str.c_str() );
	return value_token;
}
//--------------------------------------------------------------------------
Point::Point(double lat, double lon, double height, double Z, int system_coord ) 
{ 
	double  easting_x, northing_y, height0, X, Y;
	double LatP3_90_02, LonP3_90_02, LatWGS_84, LonWGS_84, H0;
	double *pLat;
	double *pLon;
	double *pH;
	double *pX;
	double *pY;
	double *ptr_easting_x;
	double *ptr_northing_y;

	TWgsPoint	wgs_84_cpa_ref_point;
	// TSK42Point sk_42_cpa_ref_point;
	TP3_90_02Point pz_90_02_ref_point;
	TKtaPoint xy_cpa_ref_point;
	TUtmPoint utm_ref_point;							// координаты точки в системе UTM
	std::string str_utm_ref_point;						// форматированная строка с координатами UTM

	GeoCalc geo_calc;
	////////////////////////////////////////////////////////
	// п.1 переход от WGS-84 к П3-90.02 (через переход к пространственным координатам)
    // точка соответствует левому верхнему углу контролируемой территории
	////////////////////////////////////////////////////////

	TTargetLocationInMinutes targetLocationInMinutes;
	TTargetLocationInMinutes* pTargetLocationInMinutes;

	pTargetLocationInMinutes = &targetLocationInMinutes;
	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, MyLAT);
		
	// easting_x = 0;		//  восточное смещение
	// northing_y = 0;		//  северное смещение		// однако нужно учитывать, что в геодизии принято наоборот, поэтому нужно внимательно смотреть на функции!
	this->height = height;
	
	// wgs_84_point_cpa.wgs_Y_lat = ern::ConfiguratorAirports::getInstance().getLatCTA();     // задаeтся референсная точка (как задано у Лаврентия)
	// wgs_84_point_cpa.wgs_X_lon = ern::ConfiguratorAirports::getInstance().getLonCTA();	// задается референсная точка (как задано у Лаврентия)

	// wgs_84_point_cpa.wgs_Y_lat = 59.800095;		// задается широта референсной точки приложения
	// wgs_84_point_cpa.wgs_X_lon = 30.264818;		// задается долгота референсной точки приложения 
	wgs_84_cpa_ref_point.wgs_Y_lat = MyLAT;			// задается широта референсной точки приложения
	wgs_84_cpa_ref_point.wgs_X_lon = MyLON;		// задается долгота референсной точки приложения
	wgs_84_cpa_ref_point.degrees_lat = pTargetLocationInMinutes->degrees;
	wgs_84_cpa_ref_point.minutes_y_lat = pTargetLocationInMinutes->minutes;
	wgs_84_cpa_ref_point.seconds_y_lat = pTargetLocationInMinutes->seconds;

	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, MyLON);
	wgs_84_cpa_ref_point.degrees_lon = pTargetLocationInMinutes->degrees;
	wgs_84_cpa_ref_point.minutes_x_lon = pTargetLocationInMinutes->minutes;
	wgs_84_cpa_ref_point.seconds_x_lon = pTargetLocationInMinutes->seconds;

	LatP3_90_02 = 0;
	pLat = &LatP3_90_02;

	LonP3_90_02 = 0;
	pLon = &LonP3_90_02;

	height0 = 20;
	H0 = 0;
	pH = &H0;
	WGS_84_P3_90_02(MyLAT, MyLON, height0, pH, pLat, pLon);

	pz_90_02_ref_point.degrees_lat = LatP3_90_02;
	pz_90_02_ref_point.degrees_lon = LonP3_90_02;
	pz_90_02_ref_point.height = H0;

	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatP3_90_02);
	pz_90_02_ref_point.degrees_lat = pTargetLocationInMinutes->degrees;
	pz_90_02_ref_point.minutes_y_lat = pTargetLocationInMinutes->minutes;
	pz_90_02_ref_point.seconds_y_lat = pTargetLocationInMinutes->seconds;

	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonP3_90_02);
	pz_90_02_ref_point.degrees_lon = pTargetLocationInMinutes->degrees;
	pz_90_02_ref_point.minutes_x_lon = pTargetLocationInMinutes->minutes;
	pz_90_02_ref_point.seconds_x_lon = pTargetLocationInMinutes->seconds;
	////////////////////////////////////////////////////////////////////////////////
	// должно получиться приблизительно так:
	//  LatP3_90_02_0 =  59.81097394
	//  LonP3_90_02_0 =  30.21199235

	//  deg_lat0 = 59;  min_x_lat0 = 48;	sec_x_lat0 = 39.506192;
	//  deg_lon0 = 30;  min_x_lon0 = 12;	sec_x_lon0 = 43.172455;
	/////////////////////////////////////////////////////////////////////////////////
	// п.1 переход от географических координат WGS-84 к координатам в системе UTM
   // точка соответствует левому верхнему углу контролируемой территории
	easting_x = 0;
	northing_y = 0;

	ptr_easting_x = &easting_x;
	ptr_northing_y = &northing_y;

	str_utm_ref_point = geo_calc.LatLong2UTMMerc(wgs_84_cpa_ref_point.wgs_X_lon, wgs_84_cpa_ref_point.wgs_Y_lat, ptr_easting_x, ptr_northing_y); 
	
	utm_ref_point.easting = easting_x;
	utm_ref_point.northing = northing_y;

	xy_cpa_ref_point.kta_X = 0;
	xy_cpa_ref_point.kta_Y = 0;

	if(system_coord == WGS_84)
	{
		wgs_84_point.wgs_X_lon = lon;
		wgs_84_point.wgs_Y_lat = lat;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, lon);
		wgs_84_point.degrees_lon = pTargetLocationInMinutes->degrees;
		wgs_84_point.minutes_x_lon = pTargetLocationInMinutes->minutes;
		wgs_84_point.seconds_x_lon = pTargetLocationInMinutes->seconds;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, lat);
		wgs_84_point.degrees_lat = pTargetLocationInMinutes->degrees;
		wgs_84_point.minutes_y_lat	= pTargetLocationInMinutes->minutes;
		wgs_84_point.seconds_y_lat	= pTargetLocationInMinutes->seconds;
		wgs_84_point.height = height;
		//---------------------------------------------------------
		LatP3_90_02 = 0;
		// pLat = &LatP3_90_02;

		LonP3_90_02 = 0;
		// pLon = &LonP3_90_02;

		H0 = 0;
		// pH = &H0;
		WGS_84_P3_90_02(lat, lon, height, pH, pLat, pLon);

		pz_90_02_point.degrees_lat = LatP3_90_02;
		pz_90_02_point.degrees_lon = LonP3_90_02;
		pz_90_02_point.height = H0;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatP3_90_02);
		pz_90_02_point.degrees_lat = pTargetLocationInMinutes->degrees;
		pz_90_02_point.minutes_y_lat = pTargetLocationInMinutes->minutes;
		pz_90_02_point.seconds_y_lat = pTargetLocationInMinutes->seconds;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonP3_90_02);
		pz_90_02_point.degrees_lon = pTargetLocationInMinutes->degrees;
		pz_90_02_point.minutes_x_lon = pTargetLocationInMinutes->minutes;
		pz_90_02_point.seconds_x_lon = pTargetLocationInMinutes->seconds;
		//------------------------
		X = 0;
		Y = 0;
		//geo_calc.WgsToPrGaus(lat, lon, &X, &Y);
		//// xyz_point.X = X;		// восточное смещение
		//// xyz_point.Y = Y;		// северное смещение
		//kta_point.kta_Y = Y - xy_cpa_ref_point.kta_Y;
		//kta_point.kta_X = X - xy_cpa_ref_point.kta_X; 

		easting_x = 0;
		northing_y = 0;

		ptr_easting_x = &easting_x;
		ptr_northing_y = &northing_y;

		pX = &X;
		pY = &Y;

	    str_utm = LatLong2UTMMerc(wgs_84_point.wgs_X_lon, wgs_84_point.wgs_Y_lat,
			utm_ref_point.easting, utm_ref_point.northing, ptr_easting_x, ptr_northing_y, pX, pY); 
	
		utm_point.easting = easting_x;		// восточное смещение
		utm_point.northing = northing_y;	// северное смещение
		//---------------------------
		// определим точку в локальной системе координат
		// kta_point
		// для теста используем приближенный алгоритм
		// FuzzyWgsToLocPlkv(lat, lon, &X, &Y);
		kta_point.kta_X = X;
		kta_point.kta_Y = Y;

		// а теперь попробуем воспользоваться вариантом перевода из СК-42, и сравним результаты ...
		/*geo_calc.SK42ToPrGaus(lat, lon, &X, &Y);
		kta_point.kta_Y = Y - xy_cpa_ref_point.kta_Y;
		kta_point.kta_X = X - xy_cpa_ref_point.kta_X;*/ 
	}
	else if(system_coord == CPA)
	{
		double *pLat;
		double *pLon;

		double dLat = 0;
		double dLon = 0;

		pLat = &dLat;
		pLon = &dLon;

		kta_point.kta_Y = lat;
		kta_point.kta_X = lon;
		//-------------------
		// Y = kta_point.kta_Y + xy_cpa_ref_point.kta_Y;
		// X = kta_point.kta_X + xy_cpa_ref_point.kta_X;
		UTM2LatLongMerc(kta_point.kta_X, kta_point.kta_Y, utm_ref_point.easting, utm_ref_point.northing, pLat, pLon);
		//----------------------		
		
		//// сперва попробуем неточным способом
		// FuzzyPlkvLocToWgs(kta_point.kta_X, kta_point.kta_Y, pLat, pLon);
		wgs_84_point.wgs_X_lon = dLon;
		wgs_84_point.wgs_Y_lat = dLat;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, dLon);
		wgs_84_point.degrees_lon = pTargetLocationInMinutes->degrees;
		wgs_84_point.minutes_x_lon = pTargetLocationInMinutes->minutes;
		wgs_84_point.seconds_x_lon = pTargetLocationInMinutes->seconds;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, dLat);
		wgs_84_point.degrees_lat = pTargetLocationInMinutes->degrees;
		wgs_84_point.minutes_y_lat	= pTargetLocationInMinutes->minutes;
		wgs_84_point.seconds_y_lat	= pTargetLocationInMinutes->seconds;
		wgs_84_point.height = height;

		//// переведем результаты в СК-42
		// sk_42_point.sk42_Y_lat = geo_calc.WGS_84_Sup_Sk_42_Lat(dLat, dLon, height);
		// sk_42_point.sk42_X_lon = geo_calc.WGS_84_Sup_Sk_42_Lon(dLat, dLon, height);
		//---------------------
		// а теперь наиболее точным (глобальным) в систему CK-42 и сравним...
		// dLat = 0;
		// dLon = 0;
		// geo_calc.PrGausToSK42(X, Y, pLat, pLon);
		//// определим точку в системе СК-42
		// sk_42_point.sk42_Y_lat = dLat;
		// sk_42_point.sk42_X_lon = dLon; 

		// wgs_84_point.wgs_Y_lat = geo_calc.SK_42_Sup_WGS_84_Lat(dLat, dLon, height);
		// wgs_84_point.wgs_X_lon = geo_calc.SK_42_Sup_WGS_84_Lon(dLat, dLon, height);
		//----------------------------------------
		LatP3_90_02 = 0;
		pLat = &LatP3_90_02;

		LonP3_90_02 = 0;
		pLon = &LonP3_90_02;

		H0 = 0;
		// pH = &H0;
		WGS_84_P3_90_02(dLat, dLon, height, pH, pLat, pLon);

		pz_90_02_point.degrees_lat = LatP3_90_02;
		pz_90_02_point.degrees_lon = LonP3_90_02;
		pz_90_02_point.height = H0;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatP3_90_02);
		pz_90_02_point.degrees_lat = pTargetLocationInMinutes->degrees;
		pz_90_02_point.minutes_y_lat = pTargetLocationInMinutes->minutes;
		pz_90_02_point.seconds_y_lat = pTargetLocationInMinutes->seconds;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonP3_90_02);
		pz_90_02_point.degrees_lon = pTargetLocationInMinutes->degrees;
		pz_90_02_point.minutes_x_lon = pTargetLocationInMinutes->minutes;
		pz_90_02_point.seconds_x_lon = pTargetLocationInMinutes->seconds;
		//------------------------
	}
	else if(system_coord == P3_90_02)		
	{
		pz_90_02_point.p3_90_02_X_lon = lon;
		pz_90_02_point.p3_90_02_Y_lat = lat;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, lon);
		pz_90_02_point.degrees_lon = pTargetLocationInMinutes->degrees;
		pz_90_02_point.minutes_x_lon = pTargetLocationInMinutes->minutes;
		pz_90_02_point.seconds_x_lon = pTargetLocationInMinutes->seconds;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, lat);
		pz_90_02_point.degrees_lat = pTargetLocationInMinutes->degrees;
		pz_90_02_point.minutes_y_lat	= pTargetLocationInMinutes->minutes;
		pz_90_02_point.seconds_y_lat	= pTargetLocationInMinutes->seconds;
		pz_90_02_point.height = height;
		//---------------------------------------------------------
		LatWGS_84 = 0;
		pLat = &LatWGS_84;

		LonWGS_84 = 0;
		pLon = &LatWGS_84;

		H0 = 0;
		// pH = &H0;
		P3_90_02_WGS_84(lat, lon, height, pH, pLat, pLon);

		wgs_84_point.degrees_lat = LatWGS_84;
		wgs_84_point.degrees_lon = LonWGS_84;
		wgs_84_point.height = H0;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatWGS_84);
		wgs_84_point.degrees_lat = pTargetLocationInMinutes->degrees;
		wgs_84_point.minutes_y_lat = pTargetLocationInMinutes->minutes;
		wgs_84_point.seconds_y_lat = pTargetLocationInMinutes->seconds;

		memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
		geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonWGS_84);
		wgs_84_point.degrees_lon = pTargetLocationInMinutes->degrees;
		wgs_84_point.minutes_x_lon = pTargetLocationInMinutes->minutes;
		wgs_84_point.seconds_x_lon = pTargetLocationInMinutes->seconds;
		//------------------------
		X = 0;
		Y = 0;
	
		easting_x = 0;
		northing_y = 0;

		ptr_easting_x = &easting_x;
		ptr_northing_y = &northing_y;

		pX = &X;
		pY = &Y;

	    str_utm = LatLong2UTMMerc(wgs_84_point.wgs_X_lon, wgs_84_point.wgs_Y_lat,
			utm_ref_point.easting, utm_ref_point.northing, ptr_easting_x, ptr_northing_y, pX, pY); 
	
		utm_point.easting = easting_x;		// восточное смещение
		utm_point.northing = northing_y;	// северное смещение
		//---------------------------
		// теперь можно задать точку в локальной системе координат
		kta_point.kta_X = X;
		kta_point.kta_Y = Y;
	}
	else
	{
		ern::Log::getInstance().Write("Unknown coordinate system");
		return;
	}
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//-------------------------------------------------------------------------
//--------------------------------------------------------------------------
Line::Line(Point* ptrPoint1, Point* ptrPoint2)
{
	if(!ptrPoint1 || !ptrPoint2)
	{
		ern::Log::getInstance().Write("No line points are specified");
		return;
	}
	this->ptrPoint1 = ptrPoint1;
	this->ptrPoint2 = ptrPoint2;
	calcParamLine();
}
//--------------------------------------------------------------------------
void Line::calcParamLine()
{
	double dY, multiplier_k;
	dY = ptrPoint1->getCPA_y() - ptrPoint2->getCPA_y();
	multiplier_k = ptrPoint1->getCPA_x() - ptrPoint2->getCPA_x();
	k = dY / multiplier_k;
	m = ptrPoint1->getCPA_y() - k * ptrPoint1->getCPA_x();
}
//--------------------------------------------------------------------------
Runway::Runway(int 	id_vpp, double height_cta) 
{ 
	double runway_lat_vpp,  runway_lon_vpp;
	double north_wgs_lat_vpp, north_wgs_lon_vpp;
	double south_wgs_lat_vpp, south_wgs_lon_vpp;
	double west_wgs_lat_vpp, west_wgs_lon_vpp;
	double east_wgs_lat_vpp, east_wgs_lon_vpp;

	char cap_vpp[BUFF_CAPTION_VPP];
	memset(cap_vpp, 0, BUFF_CAPTION_VPP);
	this->id_vpp = id_vpp;

	if(	id_vpp==0){		// это означает, что ВПП №1
		ern::ConfiguratorAirports::getInstance().getCaptionVPP1(cap_vpp, BUFF_CAPTION_VPP);
		cat_vpp = ern::ConfiguratorAirports::getInstance().getCatVPP1();
		angle_vpp = ern::ConfiguratorAirports::getInstance().getAngleVPP1();
		runway_lat_vpp = ern::ConfiguratorAirports::getInstance().getRunwayLatVPP1();
		runway_lat_vpp = runway_lat_vpp - 0.0000031;
		runway_lon_vpp = ern::ConfiguratorAirports::getInstance().getRunwayLonVPP1();
		runway_lon_vpp = runway_lon_vpp + 0.000341;
		lenght_vpp = ern::ConfiguratorAirports::getInstance().getLenghtVPP1();
		lenght_vpp = lenght_vpp * 1.001322055;
		// lenght_vpp = 3787;
		width_vpp = ern::ConfiguratorAirports::getInstance().getWidthVPP1();
		north_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getNorthWgsLatVPP1();
		north_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getNorthWgsLonVPP1();
		south_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getSouthWgsLatVPP1();
		south_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getSouthWgsLonVPP1();
		west_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getWestWgsLatVPP1();
		west_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getWestWgsLonVPP1();
		east_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getEastWgsLatVPP1();
		east_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getEastWgsLonVPP1();
		ptrCentralPoint = new Point(runway_lat_vpp, runway_lon_vpp, height_cta, 0, WGS_84);
		// ptrCentralPoint->setCPA_y(ptrCentralPoint->getCPA_y() - 5.533);
		ptrCentralPoint->setCPA_y(ptrCentralPoint->getCPA_y() - 11.066);
		// ptrCentralPoint->setCPA_x(ptrCentralPoint->getCPA_x() + 18.613);
		ptrCentralPoint->setCPA_x(ptrCentralPoint->getCPA_x() + 37.226);
	}else if(id_vpp==1){	// это означает, что ВПП №2
		ern::ConfiguratorAirports::getInstance().getCaptionVPP2(cap_vpp, BUFF_CAPTION_VPP);
		cat_vpp = ern::ConfiguratorAirports::getInstance().getCatVPP2();
		angle_vpp = ern::ConfiguratorAirports::getInstance().getAngleVPP2();
		runway_lat_vpp = ern::ConfiguratorAirports::getInstance().getRunwayLatVPP2();
		runway_lon_vpp = ern::ConfiguratorAirports::getInstance().getRunwayLonVPP2() - 0.0000689;
		lenght_vpp = ern::ConfiguratorAirports::getInstance().getLenghtVPP2();
		// lenght_vpp = lenght_vpp * 0.9988211;
		lenght_vpp = lenght_vpp * 1.00058945;
		// lenght_vpp = 3395;
		width_vpp = ern::ConfiguratorAirports::getInstance().getWidthVPP2();
		north_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getNorthWgsLatVPP2();
		north_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getNorthWgsLonVPP2();
		south_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getSouthWgsLatVPP2();
		south_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getSouthWgsLonVPP2();
		west_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getWestWgsLatVPP2();
		west_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getWestWgsLonVPP2();
		east_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getEastWgsLatVPP2();
		east_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getEastWgsLonVPP2();
		ptrCentralPoint = new Point(runway_lat_vpp, runway_lon_vpp, height_cta, 0, WGS_84);
	}else
	{
		ern::Log::getInstance().Write("The number of runways is more than two");
		return;
	}
	caption_vpp = cap_vpp;
	isRunwayEmploymentStatus_ = false;

	// теперь можно задать центральную точку и крайние точки ВПП, которые ее определяют  
	/*ptrCentralPoint = new Point(runway_lat_vpp, runway_lon_vpp, height_cta, 0, WGS_84);*/
	
	// вычисление координат точек ptrNorthPoint, ptrSouthPoint, ptrWesternPoint, ptrEasternPoint  в системе WGS84 относительно центральной точки ВПП
	to_recalculate_coords();

	// далее необходимо задать функциональные параметры прямых, обрамляющих ВПП (WN, NE, ES, SW) 
	ptrWN = new Line(ptrWesternPoint, ptrNorthPoint);
	ptrNE = new Line(ptrNorthPoint, ptrEasternPoint);
	ptrES = new Line(ptrEasternPoint, ptrSouthPoint);
	ptrSW = new Line(ptrSouthPoint, ptrWesternPoint);
}
//--------------------------------------------------------------------------
bool Runway::runway_employment_status(Point* ptrTargetPoint)			// возвращает статус занятости ВПП для местоположения конкретной цели
{
	bool runway_employment_status = false;
	if(!ptrTargetPoint)
		ern::Log::getInstance().Write("Target point is not set");
	else
	{
		// если попадает в диапазон по шкале X
		if(ptrTargetPoint->getCPA_x() > ptrWesternPoint->getCPA_x() && ptrTargetPoint->getCPA_x() < ptrEasternPoint->getCPA_x() ) 
		{
			// если координата X северной точки меньше координаты X южной точки
			if(ptrNorthPoint->getCPA_x() < ptrSouthPoint->getCPA_x() ) 
			{
				 if(ptrTargetPoint->getCPA_x() > ptrWesternPoint->getCPA_x() && ptrTargetPoint->getCPA_x() < ptrNorthPoint->getCPA_x() )
				 {
					// координата цели в интервале [W, N]
					 if( ptrWN->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) > ptrTargetPoint->getCPA_y() && ptrSW->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) < ptrTargetPoint->getCPA_y() )
						runway_employment_status = true;		// если между значениями по Y сторон WN и SW то цель находится на ВПП (ВПП занята)
				 }
				 else if(ptrTargetPoint->getCPA_x() > ptrNorthPoint->getCPA_x() && ptrTargetPoint->getCPA_x() < ptrSouthPoint->getCPA_x() )
				 {
					// координата цели в интервале [N, S]
					if( ptrNE->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) > ptrTargetPoint->getCPA_y() && ptrSW->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) < ptrTargetPoint->getCPA_y() )
						runway_employment_status = true;		// если между значениями по Y сторон NE и SW то цель находится на ВПП (ВПП занята)
				 }
				 else
				 {
					// координата цели в интервале [S, E]
					if( ptrNE->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) > ptrTargetPoint->getCPA_y() && ptrES->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) < ptrTargetPoint->getCPA_y() )
					  	runway_employment_status = true;		// если между значениями по Y сторон NE и ES то цель находится на ВПП (ВПП занята)
				 }
			}
			else	// значение координаты X северной точки больше значения координаты X южной точки
			{
				if(ptrTargetPoint->getCPA_x() > ptrWesternPoint->getCPA_x() && ptrTargetPoint->getCPA_x() < ptrSouthPoint->getCPA_x() )
				{
					// координата в интервале [W, S]
					if( ptrWN->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) > ptrTargetPoint->getCPA_y() && ptrSW->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) < ptrTargetPoint->getCPA_y() )
							runway_employment_status = true;		// если между значениями по Y сторон WN и SW то цель находится на ВПП (ВПП занята)
				}
				else if(ptrTargetPoint->getCPA_x() > ptrNorthPoint->getCPA_x() && ptrTargetPoint->getCPA_x() < ptrSouthPoint->getCPA_x() )
				 {
					// координата цели в интервале [S, N]
					if( ptrWN->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) > ptrTargetPoint->getCPA_y() && ptrES->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) < ptrTargetPoint->getCPA_y() )
						runway_employment_status = true;		// если между значениями по Y сторон NE и SW то цель находится на ВПП (ВПП занята)
				 }
				else
				{
					// координата цели в интервале [N, E]
					if( ptrNE->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) > ptrTargetPoint->getCPA_y() && ptrES->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) < ptrTargetPoint->getCPA_y() )
					  	runway_employment_status = true;		// если между значениями по Y сторон NE и ES то цель находится на ВПП (ВПП занята)
				}
			}
		}
	}
			
	return runway_employment_status;
}
//--------------------------------------------------------------------------
// пересчитывает координаты точек ptrNorthPoint, ptrSouthPoint, ptrWesternPoint, ptrEasternPoint
void Runway::to_recalculate_coords()
{
	double L, W, R, B, north_x, north_y, south_x, south_y, east_x, east_y, west_x, west_y;
	double d_north_x, d_north_y, d_south_x, d_south_y, d_east_x, d_east_y, d_west_x, d_west_y;
	double Pi = 3.14159265358979;		// число Пи
	double angle_rad = angle_vpp * Pi / 180;
	
	L = 0.5 * lenght_vpp;
	W = 0.5 * width_vpp;	// + 1.9;	// 20;
	R = sqrt(L*L + W*W);
	B = atan(W/L);

	if(angle_vpp > 0 && angle_vpp < 90)
	{
		d_north_x = R * sin(angle_rad - B);
		north_x = ptrCentralPoint->getCPA_x() + d_north_x;
		south_x = ptrCentralPoint->getCPA_x() - d_north_x;
		//-------------------
		d_north_y = R * cos(angle_rad - B);
		north_y = ptrCentralPoint->getCPA_y() + d_north_y;
		south_y = ptrCentralPoint->getCPA_y() - d_north_y;
		//------------------
		d_east_x = R * sin(angle_rad + B);
		east_x = ptrCentralPoint->getCPA_x() + d_east_x;
		west_x = ptrCentralPoint->getCPA_x() - d_east_x;
		//-----------------
		d_east_y = R * cos(angle_rad + B);
		east_y = ptrCentralPoint->getCPA_y() + d_east_y;
		west_y = ptrCentralPoint->getCPA_y() - d_east_y;
	}
	else if(angle_vpp > 90 && angle_vpp < 180)
	{
		d_north_x = R * cos(angle_rad - Pi/2 + B);
		north_x = ptrCentralPoint->getCPA_x() - d_north_x;
		south_x = ptrCentralPoint->getCPA_x() + d_north_x;
		//-------------------
		d_north_y = R * sin(angle_rad - Pi/2 + B);
		north_y = ptrCentralPoint->getCPA_y() + d_north_y;
		south_y = ptrCentralPoint->getCPA_y() - d_north_y;
		//-------------------
		d_east_x = R * cos(angle_rad - Pi/2 - B);
		east_x = ptrCentralPoint->getCPA_x() + d_east_x;
		west_x = ptrCentralPoint->getCPA_x() - d_east_x;
		//-------------------
		d_east_y = R * sin(angle_rad - Pi/2 - B);
		east_y = ptrCentralPoint->getCPA_y() - d_east_y;
		west_y = ptrCentralPoint->getCPA_y() + d_east_y;
	}
	else
	{
		ern::Log::getInstance().Write("Cases with an angle of 180 degrees to 360 degrees are not implemented");
		return;
	}

	ptrNorthPoint = new Point(north_y, north_x, ptrCentralPoint->getHeight(), 0, CPA);
	ptrSouthPoint = new Point(south_y, south_x, ptrCentralPoint->getHeight(), 0, CPA);
	ptrWesternPoint = new Point(west_y, west_x, ptrCentralPoint->getHeight(), 0, CPA);
	ptrEasternPoint = new Point(east_y, east_x, ptrCentralPoint->getHeight(), 0, CPA);
}
//--------------------------------------------------------------------------
Airdrome::Airdrome() 
{ 
	char name_aird [BUFF_NAME_AIRPORTS];
	memset(name_aird, 0, BUFF_NAME_AIRPORTS);
	ern::ConfiguratorAirports::getInstance().getNameSection(name_aird, BUFF_NAME_AIRPORTS);
	name_airdrome = name_aird;
	count_vpp = 2;
	ptrAirRefPoint = new Point(MyLAT, MyLON, ern::ConfiguratorAirports::getInstance().getHeightCTA(), 0, WGS_84);
	ptrRunway_1 = new Runway(ern::ConfiguratorAirports::getInstance().getIdVpp1(), ern::ConfiguratorAirports::getInstance().getHeightCTA() );
	ptrRunway_2 = new Runway(ern::ConfiguratorAirports::getInstance().getIdVPP2(), ern::ConfiguratorAirports::getInstance().getHeightCTA() );	
}
//--------------------------------------------------------------------------
//bool operator < (const RecCategory_11& record_1, const RecCategory_11& record_2)
//{
//	return record_1.time_of_track < record_2.time_of_track;
//}
//-------------------------------------------------
//bool operator == (const RecCategory_11& record_1, const RecCategory_11& record_2)
//{
//	double dTimes;
//	double eps = 0.00000001;
//	dTimes = record_1.time_of_track - record_2.time_of_track;
//	if( dTimes > -eps  && dTimes < eps)
//		return true;
//	else
//		return false;
//}
////-------------------------------------------------
//bool operator != (const RecCategory_11& record_1, const RecCategory_11& record_2)
//{
//	double dTimes;
//	double eps = 0.00000001;
//	dTimes = record_1.time_of_track - record_2.time_of_track;
//	if( dTimes > -eps  && dTimes < eps)
//		return false;
//	else
//		return true;
//}
////-------------------------------------------------
//bool operator > (const RecCategory_11& record_1, const RecCategory_11& record_2)
//{
//	return record_1.time_of_track > record_2.time_of_track;
//}
//--------------------------------------------------------------------------
void Cat11Records::addCat11Record(RecCategory_11 rec_11)
{
	if( find(vectCat11Records.begin(), vectCat11Records.end(), rec_11) == vectCat11Records.end() )
		vectCat11Records.push_back(rec_11);	
}
//--------------------------------------------------------------------------
RecCategory_11 Cat11Records::getCat11Record(uint16_t numCat11Rec)
{
	if(this->vectCat11Records.size() < numCat11Rec)
	{
		ern::Log::getInstance().Write("Error, In the track refer to a nonexistent record");
		numCat11Rec = vectCat11Records.size();
	}
	return vectCat11Records[numCat11Rec];
}
//--------------------------------------------------------------------------
// добавление записи в список (новой записи в конец списка!!!)
void Track::addCat11Asterix(RecCategory_11 rec_cat_11)			
{
	if(track_number == rec_cat_11.track_number)
		ptrCat11Records->addCat11Record(rec_cat_11);
	else
		ern::Log::getInstance().Write("Error when adding an entry to the route, does not correspond to the number");
}
//--------------------------------------------------------------------------
bool operator < (const Track& track1, const Track& track2)
{
	return track1.track_number < track2.track_number;
}
//-------------------------------------------------
bool operator == (const Track& track1, const Track& track2)
{
	return track1.track_number == track2.track_number;
}
//-------------------------------------------------
bool compareTrack::operator () (Track* ptrT1, Track* ptrT2) const
{
	return *ptrT1 < *ptrT2;
}
//-------------------------------------------------
TrackList::~TrackList()
{
	// cout << "Start destructor:  TrackList::~TrackList()" << endl;
	// Удаление всех трасс из списка, удаление указателей 
	while(!setPtrsTrack.empty()){
		iter = setPtrsTrack.begin();
		delete *iter;
		setPtrsTrack.erase(iter);
		//cout << "Track is removed" << endl;
	}
	//cout << "Finish destructor:  TrackList::~operationList()" << endl;
}
//------------------------------------------------
void TrackList::clearAllTracks()
{
	while(!setPtrsTrack.empty()){
		iter = setPtrsTrack.begin();
		delete *iter;
		setPtrsTrack.erase(iter);
		//cout << "Track is removed" << endl;
	}
}
//-------------------------------------------------
// добавление трассы в список (новой трассы в конец списка!!!)
int TrackList::addTrack(Track* ptrTrack)
{
	// cout << "Entered in metod: void TrackList::addTrack(Track* track)" << endl;
	int res = 1; // пока все Ок!

	if(setPtrsTrack.empty() ) // трасс еще нет
		setPtrsTrack.insert(ptrTrack);
	else  // трассы уже есть, добавить только в том случае, если трасс с таким номером в списке нет
	{
		Track track(ptrTrack->getTrackNumber() );
		
		iter = setPtrsTrack.begin();

		while(iter != setPtrsTrack.end())
		{
			// cout << "number track in first elements of the set: " << (**iter).getTrackNumber() << endl;
			if(track == **iter)
			{
				//cout << "Elements are the same." << endl;
				// char str[20];
				//sprintf(str, "track number in first elements of the set:   %d", (**iter).getNumberOperation());
				//cout << str << endl;
				break;
			}
			iter++;
		}

		if(iter == setPtrsTrack.end())	// не нашли трассу с таким номером
			setPtrsTrack.insert(ptrTrack);
		else
		{
			ern::Log::getInstance().Write("Error when adding a new route, do not match the number");	
			res = 0;
		}
	}
	
	// cout << "method: void TrackList::addTrack(Track* track) complected!" << endl;
	return res;
}
//-------------------------------------------------
// удаление трассы из списка в соответствии с ее номером
int TrackList::deleteTrack(uint16_t track_number)								  
{
	int res = 0;
	char str[20];

	Track track( track_number );
	// iter = setPtrsTrack.find(ptrTrack);

	iter = setPtrsTrack.begin();
	while(iter != setPtrsTrack.end())
	{
		if(track == **iter)
		{
			delete *iter;
			setPtrsTrack.erase(iter);
			res = 1;

			break;
		}
		iter++;
	}	

	// cout << "Output from the method."  << endl;
	return res;
}
//-------------------------------------------------
// возвращает указатель на трассу по ее идентификационному номеру
Track* TrackList::getTrack(uint16_t num_track)									  
{
	Track track( num_track );

	int isTrackWithNumber = 0;

	iter = setPtrsTrack.begin();

	while(iter != setPtrsTrack.end())
	{
		// cout << "number track in first elements of the set: " << (**iter).getNumberTrack() << endl;
		if(track == **iter)
		{
			isTrackWithNumber = 1;
			// cout << "Elements are the same." << endl;
			break;
		}
		iter++;
	}

	if(isTrackWithNumber == 0)
		return 0;
	else
		return (*iter);
}
//-------------------------------------------------
// возвращает трассу на которую указывает начальный итератор
Track* TrackList::getBegTrack()										 
{
	iter = setPtrsTrack.begin();
	if(iter != setPtrsTrack.end())
		return (*iter);
	else 
		return 0;
}
//-------------------------------------------------
Track* TrackList::getNextTrack()
{
	iter++;
	if(iter != setPtrsTrack.end())
		return (*iter);
	else 
		return 0;
}
//------------------------------------------------
// возвращает общее количество трасс
uint16_t TrackList::getNumberTraces() const						  
{
	return setPtrsTrack.size();
}
//-------------------------------------------------
// проверяет - существует ли трасса в наборе с указанным номером
bool TrackList::isTrackWithNumber(uint16_t num_track)			  
{
	bool status = true;

	// iter = setPtrsTrack.find(ptrTrack);
	Track track( num_track );
	
	// iter = setPtrsTrack.find(track);
	iter = setPtrsTrack.begin();

	while(iter != setPtrsTrack.end())
	{
		// cout << "number operation in first elements of the set: " << (**iter).getNumberOperation() << endl;
		if(track == **iter)
		{
			//cout << "Elements are the same." << endl;
			// char str[20];
			//sprintf(str, "number operation in first elements of the set:   %d", (**iter).getNumberOperation());
			//cout << str << endl;
			break;
		}
		iter++;
	}
	
	std::set<Track*, compareTrack>::iterator iter_;
	iter_ = setPtrsTrack.end();

	if(iter == iter_)
		status = false;

	return status;
}
//-------------------------------------------------
// проверяет актуализацию всех трасс, если трасса не обновлялась более 15 сек, то она подлежит удалению
void TrackList::checkActualizationTraces(double actual_time)
{
	uint16_t count_records;
	iter = setPtrsTrack.begin();
	while(iter != setPtrsTrack.end())
	{
		// cout << "number operation in first elements of the set: " << (**iter).getNumberOperation() << endl;
		count_records = (*iter)->getCat11Record()->getCountElemList();
		count_records--;
		// if( actual_time - (*iter)->getCat11Record()->getCat11Record(count_records).time_of_track > 15){		// указывает на последний элемент списка записей, если настоящ. время больше на 15 сек
		if( actual_time - (*iter)->getCat11Record()->getCat11Record(count_records).time_of_track > 160){		// указывает на последний элемент списка записей, если настоящ. время больше на 20 сек
			delete *iter;
			setPtrsTrack.erase(iter);
			//cout << "Track is removed" << endl;
		}
		iter++;
	}	
}
//------------------------------------------------
//  проверяет состояние занятости ВПП путем обхода всех актуальных (действующих) трасс
bool TrackList::isRunwayEmploymentStatus(uint8_t number_vpp)
{
	bool isRunwayEmploymentStatus = false;
	iter = setPtrsTrack.begin();

	if(number_vpp == 1)
	{
		while(iter != setPtrsTrack.end())
		{
			if( (*iter)->getLocatedRunway1() )		// если находится на ВПП1
				return true;
			iter++;
		}
	}
	else if(number_vpp == 2)
	{
		while(iter != setPtrsTrack.end())
		{
			if( (*iter)->getLocatedRunway2() )		// если находится на ВПП2
				return true;
			iter++;
		}
	}
	else
	{
		ern::Log::getInstance().Write("Error, bool TrackList::isRunwayEmploymentStatus(uint8_t number_vpp)");
		ern::Log::getInstance().Write("Aerodromes with more than two runways in our country are not present");
	}

	return isRunwayEmploymentStatus;
}
//-------------------------------------------------
// выделяет необходимое количество памяти для АСТЕРИКС-данных категории 11
void AsterixWorker::init_sourse_data_cat011()
{
	// count_msg_asterix_cat011
	// this->ptrMessageType = new BYTE(this->count_msg_asterix_cat011);

}
//--------------------------------------------------------------------------
// обновляет массив buffAsterixMsgInfo новыми данными
void AsterixWorker::setAsterixMSG(const char* ptrBuffAsterixMsg, int size_buff_Asterix_Msg)	
{
	// почистим память
	// memset(this->buffAsterixMsgInfo, 0, this->size_buff_Asterix_Msg_);
	this->size_buff_Asterix_MsgCat011_ = size_buff_Asterix_Msg;
	memcpy(this->buffAsterixMsgCat011, ptrBuffAsterixMsg, size_buff_Asterix_Msg );
}
//-------------------------------------------------------------------------------------
// очищает оперативное хранилище данных категории 011 для трасс
void AsterixWorker::clearAllDataCat011()
{
	/*msg_MessageType.clear();
	msg_DataSourceId.clear();
	msg_ServiceId.clear();
	msg_Position_wgs84.clear();
	msg_PositionCartesian.clear();
	msg_Mode3A.clear();
	msg_FlightLevel.clear();
	msg_GeoAltitude.clear();
	msg_BaroAltitude.clear();
	msg_TimeOfTrack.clear();
	msg_TrackNumber.clear();
	msg_TrackStatus.clear();
	msg_TrackVelocity.clear();
	msg_CalcAcceleration.clear();
	msg_CalcVertRate.clear();
	msg_CallSign.clear();
	msg_TargetSizeOrient.clear();
	msg_Ages.clear();
	msg_Fleet.clear();
	msg_Prog.clear();
	for(int i=0; i<msg_AdsBRelatedData.size(); i++)
		delete msg_AdsBRelatedData[i].mode_s_msg;
	msg_AdsBRelatedData.clear();*/
	//..
	/*memset(this->buffAsterixMsgCat011, 0, this->size_buff_Asterix_MsgCat011_);
	size_buff_Asterix_MsgCat011_ = 0;*/
	count_msg_asterix_cat011 = 0;
	ptrTrackList->clearAllTracks();
}
//-------------------------------------------------------------------------------------
void AsterixWorker::addStrToCSV(RecCategory_11 &cat_11)
{
	char name_file[20];
	char str_csv[2000];
	std::string csv_string;				// cvs-строка для записи в файл

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
	out_csv_file.open(name_file, std::ios::app | std::ios::out );
		
	if(record_count > MAX_NUMBER_ENTRIES_CSV)
	{
		record_count = 1;
		ern::ConfiguratorApp::getInstance().setRecordCount(record_count);

		table_count++;
		ern::ConfiguratorApp::getInstance().setTableCount(table_count);
		// out_csv_file.close();

		std::string cvs_string = "msg_type;SAC;SIC;ServiceID;wgs84_lat;wgs84_lon;X;Y;Mode3A;flight_level;geo_altitude;QNH;baro_altitude;time_of_track;track_number;";
		cvs_string += "MON;GBS;MRH;SRC;CNF;SIM;TSE;TSB;FRI/FOE;ME;MI;AMA;SPI;CST;FPC;AFF;PSR;SSR;MDS;ADS;SUC;AAC;VX;VY;AX;AY;RATE;STI;CALLSIGN;LENGTH;WIDTH;ORIENTATION;";
		cvs_string += "PSR_Ages;SSR_Ages;MDA;MFL;MDS_Ages;ADS_Ages;ADB;MD1;MD2;LOP;TRK;MUL;VFI;TRB;MSG;REP;MB_DATA;ADDRESS;COM;STAS;SSC;ARC;AIC;B1A;B1B;AC;VVP1;VVP2\n";
		out_csv_file << cvs_string;
	}
	
	sprintf(str_csv, "%d", cat_11.msg_type);
	csv_string = str_csv;
	csv_string += ";";
	//-----------------------------
	sprintf(str_csv, "%d;", cat_11.dsi.sac);
	csv_string += str_csv;
	
	sprintf(str_csv, "%d;", cat_11.dsi.sic);
	csv_string += str_csv;
	//----------------------------
	sprintf(str_csv, "%d;", cat_11.service_id);
	csv_string += str_csv;
	//----------------------------
	sprintf(str_csv, "%8.6f;", cat_11.pos_wgs84.wgs84_lat);
	csv_string += str_csv;
	
	sprintf(str_csv, "%8.6f;", cat_11.pos_wgs84.wgs84_lon);
	csv_string += str_csv;
	//---------------------------
	sprintf(str_csv, "%8.4f;", cat_11.pos_cart.x);
	csv_string += str_csv;
	
	sprintf(str_csv, "%8.4f;", cat_11.pos_cart.y);
	csv_string += str_csv;
	//--------------------------
	sprintf(str_csv, "%d;", cat_11.mode_3A);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%8.4f;", cat_11.flight_level);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%8.4f;", cat_11.geo_altitude);
	csv_string += str_csv;
	//-------------------------
	if(cat_11.baro_alt.qnh)
		csv_string += "true;";	
	else	
		csv_string += "false;";
	//-------------------------
	sprintf(str_csv, "%8.4f", cat_11.baro_alt.altitude);
	csv_string += str_csv;
	csv_string += ";";
	//-------------------------
	sprintf(str_csv, "%8.4f;", cat_11.time_of_track);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%d;", cat_11.track_number);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%d;", cat_11.tr_status.monosensor);
	csv_string += str_csv;
	
	sprintf(str_csv, "%d;", cat_11.tr_status.ground_bit_set);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.most_reliable_height);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.geometric_altitude_source);
	csv_string += str_csv;
	
	sprintf(str_csv, "%d;", cat_11.tr_status.confirm);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.simulate);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.last_message);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.first_message);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.mode4_interrogate);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.military_emrg);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.military_id);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.amalgamation);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.spi_sign);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.coast_track);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.flight_in_plan);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.ads_inconsist);
	csv_string += str_csv;
	
	sprintf(str_csv, "%d;", cat_11.tr_status.psr_coast);
	csv_string += str_csv;
	
	sprintf(str_csv, "%d;", cat_11.tr_status.ssr_coast);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.mode_s_coast);
	csv_string += str_csv;
	
	sprintf(str_csv, "%d;", cat_11.tr_status.ads_coast);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.suc_set);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.tr_status.aac_conflict);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%8.4f;", cat_11.tr_velocity.vx);
	csv_string += str_csv;
	
	sprintf(str_csv, "%8.4f;", cat_11.tr_velocity.vy);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%8.4f;", cat_11.acceleration.ax);
	csv_string += str_csv;
		
	sprintf(str_csv, "%8.4f;", cat_11.acceleration.ay);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%8.4f;", cat_11.calc_vert_rate);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%d;", cat_11._call_sign.sti);
	csv_string += str_csv;
	csv_string += cat_11._call_sign.callsign;	
	csv_string += ";";
	//-------------------------
	sprintf(str_csv, "%8.4f;", cat_11.target_size.length);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11.target_size.width);
	csv_string += str_csv;
		
	sprintf(str_csv, "%8.4f;", cat_11.target_size.orientation);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%8.4f;", cat_11._ages.psr_age);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11._ages.ssr_age);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11._ages.mda_age);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11._ages.mfl_age);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11._ages.mds_age);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11._ages.ads_age);
	csv_string += str_csv;
			
	sprintf(str_csv, "%8.4f;", cat_11._ages.ads_es_age);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11._ages.md1_age);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11._ages.md2_age);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11._ages.loop_age);
	csv_string += str_csv;
			
	sprintf(str_csv, "%8.4f;", cat_11._ages.trk_age);
	csv_string += str_csv;
				
	sprintf(str_csv, "%8.4f;", cat_11._ages.mlt_age);
	csv_string += str_csv;
	//-------------------------
	sprintf(str_csv, "%d;", cat_11.fleet);
	csv_string += str_csv;
	//--------------------------
	if(cat_11._prog_msg.trouble)
		// sprintf(str_csv, "%d", cat_11._prog_msg.trouble);
		csv_string += "true;";
	else
		csv_string += "false;";
	sprintf(str_csv, "%d;", cat_11._prog_msg.type);
	csv_string += str_csv;
	//--------------------------
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.mode_s_data_count);
	csv_string += str_csv;
	//---------------------
	if(cat_11.adsBRelatedData.mode_s_data_count == 0)
		csv_string += ";";			
	else
	{
		for(int i =0; i<cat_11.adsBRelatedData.mode_s_data_count; i++)
		{
			#ifdef __linux__ 
			//linux code goes here
			sprintf(str_csv, "%" PRIu64 "|", cat_11.adsBRelatedData.mode_s_msg[i]);
			#elif _WIN32
			// windows code goes here
			sprintf(str_csv, "%llu|", cat_11.adsBRelatedData.mode_s_msg[i]);
			#else
				// #error "OS not supported!"
			#endif
			// csv_string += "|";			
		}
		csv_string += str_csv;
		csv_string += ";";
	}

	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.target_address);
	csv_string += str_csv;
	 
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.communication_capability);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.flight_status);
	csv_string += str_csv;
	 
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.specific_service);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.altitude_report_capability);
	csv_string += str_csv;
			
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.aircraft_ident_capability);
	csv_string += str_csv;
			
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.bit_b1a);
	csv_string += str_csv;
			
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.bits_b1b);
	csv_string += str_csv;
		
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.acas_operational);
	csv_string += str_csv;
	//-------------------------
	// здесь промежуточные результаты
	//--------------------------
	// статус занятости ВВП1 и статус за
	if( ptrAirdrome->getPtrRunway(1)->getRunwayEmploymentStatus() )
		csv_string +=	"busy;";	
	else
		csv_string +=	"free;	";
	//-----------------------------
	if( ptrAirdrome->getPtrRunway(2)->getRunwayEmploymentStatus() )
		csv_string += "busy;\n";	
	else
		csv_string += "free;\n";	
	//--------------------------
	int x = csv_string.find('.');
	if(x != -1)
		csv_string.replace(x, 1, ",");						
			
	record_count++;
	ern::ConfiguratorApp::getInstance().setRecordCount(record_count);
	out_csv_file << csv_string;
}
//------------------------------------------------------------------------------------
// определяет, нужно ли начинать расчет, а также по необходимости записывает данные в csv-файл
bool AsterixWorker::isStartCalculation(RecCategory_11 &cat_11)
{
	// если начало цикла наблюдения еще не задано, то нужно его инициализировать полученным текущим временем трассы
	// и выйти из метода c возвратом false (начинать расчет еще не нужно, начало наблюдения!!!)
	// данные пишутся в файл
 	if(!this->isBegObservPeriodSpecified)
	{
		this->the_first_message_arrives = cat_11.time_of_track;
		isBegObservPeriodSpecified = true;

		// здесь запись в файл
		addStrToCSV(cat_11);
		return false;
	}
	//-----------------------------
	// режим работы: через каждые 50 мс после последнего расчета начинается новый расчет,
	// в CSV-файл пишутся все записи 
	if(recording_mode_to_csv == ALL_RECORDS)
	{
		addStrToCSV(cat_11);
		if((cat_11.time_of_track - the_first_message_arrives) > 0.05)  
			return true;			
		else
			return false;
	}
	//-------------------------------
	// режим работы: через каждые 100 мс после последнего расчета начинается новый расчет,
	// в CSV-файл запись начинается через каждые 50 мс
	if(this->recording_mode_to_csv == STANDARD)
	{
		if((cat_11.time_of_track - the_first_message_arrives) > 0.1)  
		{
			addStrToCSV(cat_11);		
			return true;
		}
		else if((cat_11.time_of_track - the_first_message_arrives) > 0.05)  
		{
			addStrToCSV(cat_11);		
			return false;
		}
		else
			return false;
	}
	//---------------------------------
	// режим работы: через каждые 500 мс после последнего расчета начинается новый расчет,
	// в CSV-файл запись начинается через каждые 450 мс
	if(this->recording_mode_to_csv == EXTENDED)
	{
		if((cat_11.time_of_track - the_first_message_arrives) > 0.5)  
		{
			addStrToCSV(cat_11);		
			return true;
		}
		else if((cat_11.time_of_track - the_first_message_arrives) > 0.450)  
		{
			addStrToCSV(cat_11);		
			return false;
		}
		else
			return false;
	}
	//---------------------------------
	// режим работы: через каждые 1000 мс после последнего расчета начинается новый расчет,
	// в CSV-файл запись начинается через каждые 900 мс
	if(this->recording_mode_to_csv == LONG_PERIOD)
	{
		if((cat_11.time_of_track - the_first_message_arrives) > 1)  
		{
			addStrToCSV(cat_11);		
			return true;
		}
		else if((cat_11.time_of_track - the_first_message_arrives) > 0.9)  
		{
			addStrToCSV(cat_11);		
			return false;
		}
		else
			return false;
	}
	//---------------------------------
	// режим работы: через каждые 10000 мс после последнего расчета начинается новый расчет,
	// в CSV-файл запись начинается через каждые 9800 мс
	if(this->recording_mode_to_csv == VERY_LONG_PERIOD)
	{
		if((cat_11.time_of_track - the_first_message_arrives) > 10)  
		{
			addStrToCSV(cat_11);		
			return true;
		}
		else if((cat_11.time_of_track - the_first_message_arrives) > 9.8)  
		{
			addStrToCSV(cat_11);		
			return false;
		}
		else
			return false;
	}
}
//-------------------------------------------------------------------------------------
// разбирает новую запись категории 11 Астерикс, 
// данные добавляются в соответствующие поля класса, 
// а строка дозаписывается в csv-файл
void AsterixWorker::handleCat11(Cat11::Cat11* cat11)
{
	using namespace Asterix;

	uint8_t msg_type;	// элемент данных I011/000
	
	const uint8_t* ptrMsgType = cat11->get_message_type();
	const DataSourceId*      id       = cat11->get_dsi();
	const double*            ptrTime     = cat11->get_time();
	/*const PositionCartesian* pos      = cat11->get_cart_pos();
	const uint16_t*          tn       = cat11->get_track_number();*/
		
	if (!ptrMsgType || !id || !time)
	{
		if (!id)
			//mm_log(AST, ERROR, "missing some fields in cat11: dsi\n");
			ern::Log::getInstance().Write("Error, missing some fields in cat11: dsi\n");
		if (!ptrTime)
			//mm_log(AST, ERROR, "missing some fields in cat11: time\n");
			ern::Log::getInstance().Write("Error, missing some fields in cat11: time\n");
		if (!ptrMsgType)
			// mm_log(AST, ERROR, "missing some fields in cat11: msg_type\n");
			ern::Log::getInstance().Write("Error, missing some fields in cat11: msg_type\n");
		return;
	}

	msg_type = *ptrMsgType;

	switch (msg_type)
	{
		case Cat11::MT_UNKNOWN:
			break;
		case Cat11::MT_TRACK_AND_FLIGHT_PLAN:
		{
			int x;
			RecCategory_11 cat_11;
			
			cat_11.msg_type = msg_type;
			//-----------------------------
			cat_11.dsi.sac = id->sac;
			cat_11.dsi.sic = id->sic;
			//----------------------------
			const uint8_t* ptrServiceID = cat11->get_service();
			if(!ptrServiceID)
			{
				cat_11.service_id = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Service Identification\n");
			}
			else
				cat_11.service_id = *ptrServiceID;
			//----------------------------
			const Position* ptrPos = cat11->get_position();	 ///< Position in WGS-84 Coordinates
			if(!ptrPos)
			{ 
				cat_11.pos_wgs84.wgs84_lat = 0;	cat_11.pos_wgs84.wgs84_lon = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Position in WGS-84 Coordinates\n");
			}	
			else
			{
				cat_11.pos_wgs84.wgs84_lat = ptrPos->wgs84_lat;
				cat_11.pos_wgs84.wgs84_lon = ptrPos->wgs84_lon;
			}
			//---------------------------
			const PositionCartesian* ptrPosCart = cat11->get_cart_pos();	 ///< Position in Cartesian Co-ordinates
			if(!ptrPosCart)
			{
				cat_11.pos_cart.x = 100000.0;
				cat_11.pos_cart.y = 100000.0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Position in Cartesian Co-ordinates\n");
			}
			else
			{
				// double Conf_North_Y = 0;
				// double Conf_East_X = 0;
				// WgsToLocPr(ern::ConfiguratorAirports::getInstance().getLatCTA(), ern::ConfiguratorAirports::getInstance().getLonCTA(), &Conf_East_X, &Conf_North_Y);

				// получим координаты точки на плоскости в проекции Меркатора
				double conf_northing_y = 0;
				double conf_easting_x = 0;

				GeoCalc geo_calc;		// добавляется смещение, которое имеет место быть в данной версии (эту поправку стоит учитывать и при обратном преобразовании)
				geo_calc.LatLong2UTMMerc(ern::ConfiguratorAirports::getInstance().getLonCTA() + 0.001852, ern::ConfiguratorAirports::getInstance().getLatCTA() + 0.00006575, &conf_easting_x, &conf_northing_y); 

				// double My_North_Y = 0;
				// double My_East_X = 0;
				// WgsToLocPr(MyLAT, MyLON, &My_East_X, &My_North_Y);

				double my_northing_y = 0;
				double my_easting_x = 0;
				geo_calc.LatLong2UTMMerc(MyLON, MyLAT, &my_easting_x, &my_northing_y); 

				/*cat_11.pos_cart.x = ptrPosCart->x + (conf_easting_x - my_easting_x) * 1.03;
				if(abs(conf_northing_y - my_northing_y) > 1080)
					cat_11.pos_cart.y = ptrPosCart->y + (conf_northing_y - my_northing_y) / 1.06623;
				else
					cat_11.pos_cart.y = ptrPosCart->y + (conf_northing_y - my_northing_y) / 1.29;*/
				
				// cat_11.pos_cart.x = ptrPosCart->x + Conf_East_X - My_East_X;
				// cat_11.pos_cart.y = ptrPosCart->y + Conf_North_Y - My_North_Y;
				// cat_11.pos_cart.x = ptrPosCart->x + (conf_easting_x - my_easting_x) * 1.01941 - 34.3;
				// cat_11.pos_cart.x = ptrPosCart->x + (conf_easting_x - my_easting_x) * 0.968176 + 25 + 40;
				// cat_11.pos_cart.x = ptrPosCart->x + (conf_easting_x - my_easting_x) * 0.968176 + 25 + 42;
				// cat_11.pos_cart.x = ptrPosCart->x + (conf_easting_x - my_easting_x) * 0.968176 + 25 + 43;
				// cat_11.pos_cart.x = ptrPosCart->x + (conf_easting_x - my_easting_x) * 0.968176 + 25 - 2;
				cat_11.pos_cart.x = ptrPosCart->x + (conf_easting_x - my_easting_x) * 0.968176 + 25 + 1;
				// cat_11.pos_cart.y = ptrPosCart->y + (conf_northing_y - my_northing_y) * 1.03356 + 1.95;
				cat_11.pos_cart.y = ptrPosCart->y + (conf_northing_y - my_northing_y) * 1.0557432 - 2;
			}
			//--------------------------
			const Mode3A* ptrMode3A = cat11->get_mode_3a();	///< Mode 3/A Code
			if(!ptrMode3A)
			{
				cat_11.mode_3A = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Mode 3/A Code\n");
			}
			else
				cat_11.mode_3A = ptrMode3A->code;
			//-------------------------
			const double* ptrFlightLevel = cat11->get_flight_level();	// Measured Flight Level
			if(!ptrFlightLevel)
			{
				cat_11.flight_level = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Measured Flight Level\n");
			}
			else
				cat_11.flight_level = *ptrFlightLevel;
			//-------------------------
			const double* ptrGeoAltitude = cat11->get_geo_altitude();	// Calculated Track Geometric Altitude
			if(!ptrGeoAltitude)
			{
				cat_11.geo_altitude = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11:_geo_altitude\n");
			}
			else
				cat_11.geo_altitude = *ptrGeoAltitude;
			//-------------------------
			const Cat11::BaroAltitude* ptrBaroAltitude = cat11->get_baro_altitude();	//  Calculated Track Barometric Altitude
			if(!ptrBaroAltitude)
			{
				cat_11.baro_alt.qnh = 0;
				cat_11.baro_alt.altitude = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Track Barometric Altitude\n");
			}
			else
			{
				cat_11.baro_alt.qnh = ptrBaroAltitude->qnh;
				cat_11.baro_alt.altitude = ptrBaroAltitude->altitude;
			}
			//-------------------------
			cat_11.time_of_track = *ptrTime;	//  Time of Track
			// the_last_message = cat_11.time_of_track;
			//-------------------------
			const uint16_t* ptrTN = cat11->get_track_number();		// Track Number
			if(!ptrTN)
			{
				cat_11.track_number = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Track Number\n");
			}
			else
				cat_11.track_number = *ptrTN;
			//-------------------------
			const Cat11::TrackStatus* ptrTrackStatus = cat11->get_track_status(); // Track Status
			if(!ptrTrackStatus)
			{
				cat_11.tr_status.monosensor = 0;			cat_11.tr_status.spi_sign = 0;
				cat_11.tr_status.most_reliable_height = 0;	cat_11.tr_status.geometric_altitude_source = 0;
				cat_11.tr_status.confirm = 0;				cat_11.tr_status.simulate = 0;
				cat_11.tr_status.last_message = 0;			cat_11.tr_status.first_message = 0;
				cat_11.tr_status.flight_in_plan = 0;		cat_11.tr_status.ads_inconsist = 0;
				cat_11.tr_status.slave_track = 0;			cat_11.tr_status.background_serv = 0;
				cat_11.tr_status.amalgamation = 0;			cat_11.tr_status.mode4_interrogate = 0;
				cat_11.tr_status.military_emrg = 0;			cat_11.tr_status.military_id = 0;
				cat_11.tr_status.mode5_interrogate = 0;		cat_11.tr_status.coast_track = 0;
				cat_11.tr_status.psr_coast = 0;				cat_11.tr_status.ssr_coast = 0;
				cat_11.tr_status.mode_s_coast = 0;			cat_11.tr_status.ads_coast = 0;
				cat_11.tr_status.suc_set = 0;				cat_11.tr_status.aac_conflict = 0;
				cat_11.tr_status.sds_status = 0;			cat_11.tr_status.emergency_status = 0;
	            cat_11.tr_status.ground_bit_set = 0;		cat_11.tr_status.horizontal_manoeuvre = 0;                
				cat_11.tr_status.slant_correction = 0;		cat_11.tr_status.smooth_position = 0;
	 			ern::Log::getInstance().Write("Error, missing some fields in cat11: Track Status\n");
			}
			else
			{
				cat_11.tr_status.monosensor = ptrTrackStatus->monosensor;					cat_11.tr_status.spi_sign = ptrTrackStatus->spi_sign;
				cat_11.tr_status.most_reliable_height = ptrTrackStatus->most_reliable_height;	cat_11.tr_status.geometric_altitude_source = ptrTrackStatus->geometric_altitude_source;
				cat_11.tr_status.confirm = ptrTrackStatus->confirm;							cat_11.tr_status.simulate = ptrTrackStatus->simulate;
				cat_11.tr_status.last_message = ptrTrackStatus->last_message;				cat_11.tr_status.first_message = ptrTrackStatus->first_message;
				cat_11.tr_status.flight_in_plan = ptrTrackStatus->flight_in_plan;			cat_11.tr_status.ads_inconsist = ptrTrackStatus->ads_inconsist;
				cat_11.tr_status.slave_track = ptrTrackStatus->slave_track;					cat_11.tr_status.background_serv = ptrTrackStatus->background_serv;
				cat_11.tr_status.amalgamation = ptrTrackStatus->amalgamation;				cat_11.tr_status.mode4_interrogate = ptrTrackStatus->mode4_interrogate;
				cat_11.tr_status.military_emrg = ptrTrackStatus->military_emrg;				cat_11.tr_status.military_id = ptrTrackStatus->military_id;
				cat_11.tr_status.mode5_interrogate = ptrTrackStatus->mode5_interrogate;		cat_11.tr_status.coast_track = ptrTrackStatus->coast_track;
				cat_11.tr_status.psr_coast = ptrTrackStatus->psr_coast;						cat_11.tr_status.ssr_coast = ptrTrackStatus->ssr_coast;
				cat_11.tr_status.mode_s_coast = ptrTrackStatus->mode_s_coast;				cat_11.tr_status.ads_coast = ptrTrackStatus->ads_coast;
				cat_11.tr_status.suc_set = ptrTrackStatus->suc_set;							cat_11.tr_status.aac_conflict = ptrTrackStatus->aac_conflict;
				cat_11.tr_status.sds_status = ptrTrackStatus->sds_status;					cat_11.tr_status.emergency_status = ptrTrackStatus->emergency_status;
	            cat_11.tr_status.ground_bit_set = ptrTrackStatus->ground_bit_set;			cat_11.tr_status.horizontal_manoeuvre = ptrTrackStatus->horizontal_manoeuvre;                
				cat_11.tr_status.slant_correction = ptrTrackStatus->slant_correction;		cat_11.tr_status.smooth_position = ptrTrackStatus->smooth_position;
			}
			//-------------------------
			const Velocity* ptrVelocity = cat11->get_velocity();	//< Calculated Track Velocity in Cartesian Coord.
			if(!ptrVelocity)
			{
				cat_11.tr_velocity.vx = 0;
				cat_11.tr_velocity.vy = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Calculated Track Velocity\n");
			}
			else
			{
				cat_11.tr_velocity.vx = ptrVelocity->vx;
				cat_11.tr_velocity.vy = ptrVelocity->vy;
			}
			//-------------------------
			const Acceleration* ptrAcceleration = cat11->get_accel();	//< Calculated Acceleration
			if(!ptrAcceleration)
			{
				cat_11.acceleration.ax = 0;
				cat_11.acceleration.ay = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Calculated Acceleration\n");
			}
			else
			{
				cat_11.acceleration.ax = ptrAcceleration->ax;
				cat_11.acceleration.ay = ptrAcceleration->ay;
			}
			//-------------------------
			const double* ptrCalcVertRate = cat11->get_vert_rate();		// Calculated Rate Of Climb/Descent
			if(!ptrCalcVertRate)
			{
				cat_11.calc_vert_rate = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11:_vert_rate\n");
			}
			else
				cat_11.calc_vert_rate = *ptrCalcVertRate;
			//-------------------------
			const Callsign* ptrCallsign = cat11->get_callsign();		//< Target Identification
			if(!ptrCallsign)
			{
				// cat_11._call_sign.sti = 0;
				memset(cat_11._call_sign.callsign, 0, cat_11._call_sign.CALLSIGN_SIZE);
				cat_11._call_sign.callsign[0] = '\0';
				// ern::Log::getInstance().Write("Error, missing some fields in cat11: Target Identification\n");
			}
			else
			{
				cat_11._call_sign.sti = ptrCallsign->sti;
				strncpy(cat_11._call_sign.callsign, ptrCallsign->callsign, cat_11._call_sign.CALLSIGN_SIZE);
			}
			//-------------------------
			const Size* ptrSize = cat11->get_size();		//< Target Size & Orientation
			if(!ptrCallsign)
			{
				cat_11.target_size.length = 0;
				cat_11.target_size.width = 0;
				cat_11.target_size.orientation = 0;
				// ern::Log::getInstance().Write("Error, missing some fields in cat11: Target Size & Orientation\n");
			}
			else
			{
				/*cat_11.target_size.length = ptrSize->length;
				cat_11.target_size.width = ptrSize->width;
				cat_11.target_size.orientation = ptrSize->orientation;*/
				cat_11.target_size.length = 0;
				cat_11.target_size.width = 0;
				cat_11.target_size.orientation = 0;
			}
			//-------------------------
			const Cat11::Ages* ptrAges = cat11->get_ages();		//< System Track Update Ages
			if(!ptrAges)
			{
				cat_11._ages.psr_age = 0;			cat_11._ages.ssr_age = 0;
				cat_11._ages.mda_age = 0;		cat_11._ages.mfl_age = 0;
				cat_11._ages.mds_age = 0;			cat_11._ages.ads_age = 0;
				cat_11._ages.ads_es_age = 0;		cat_11._ages.md1_age = 0;
				cat_11._ages.md2_age = 0;		cat_11._ages.loop_age = 0;
				cat_11._ages.trk_age = 0;			cat_11._ages.mlt_age = 0;
				// ern::Log::getInstance().Write("Error, missing some fields in cat11: System Track Update Ages\n");
			}
			else
			{
				cat_11._ages.psr_age = ptrAges->psr_age;				cat_11._ages.ssr_age = ptrAges->ssr_age;
				cat_11._ages.mda_age = ptrAges->mda_age;			cat_11._ages.mfl_age = ptrAges->mfl_age;
				cat_11._ages.mds_age = ptrAges->mds_age;			cat_11._ages.ads_age = ptrAges->ads_age;
				cat_11._ages.ads_es_age = ptrAges->ads_es_age;	cat_11._ages.md1_age = ptrAges->md1_age;
				cat_11._ages.md2_age = ptrAges->md2_age;			cat_11._ages.loop_age = ptrAges->loop_age;
				cat_11._ages.trk_age = ptrAges->trk_age;				cat_11._ages.mlt_age = ptrAges->mlt_age;
			}
			//-------------------------
			const int* ptr_fleet_type = (int*)cat11->get_fleet_type();		// Vehicle Fleet Identification 
			if(!ptr_fleet_type)
			{
				cat_11.fleet = 0;			
				// ern::Log::getInstance().Write("Error, missing some fields in cat11: Vehicle Fleet Identification\n");
			}
			else
			{
				cat_11.fleet = *ptr_fleet_type;
			}
			//-------------------------
			// Pre-programmed Message
			const ProggramedMessage* ptrProggramedMessage = cat11->get_proggramed_message();
			if(!ptrProggramedMessage)
			{
				cat_11._prog_msg.trouble = 0;
				cat_11._prog_msg.type = 0;
				// ern::Log::getInstance().Write("Error, missing some fields in cat11: Pre-programmed Message\n");
			}
			else
			{
				cat_11._prog_msg.trouble = ptrProggramedMessage->trouble;
				cat_11._prog_msg.type = ptrProggramedMessage->type;
			}
			//------------------------
			// Mode-S / ADS-B Related Data
	        const Cat11::ModeS* ptrModeS = cat11->get_mode_s();	// Mode-S / ADS-B Related Data
			if(!ptrModeS)
			{
				cat_11.adsBRelatedData.mode_s_data_count = 0;				// Количество переданных восьмибайтовых сообщений (субполе №1 [REP])
				cat_11.adsBRelatedData.mode_s_msg = 0;						// Указатель на сообщения Mode S (для повторяющихся субполей №1 [MB_DATA] )
				cat_11.adsBRelatedData.target_address = 0;						// Значение 24-бит адреса ИКАО для самолетов (субполе №2 [ADDRESS])
				cat_11.adsBRelatedData.communication_capability = 0;		// Возможность коммуникации (параметр субполя №4 [COM])
				cat_11.adsBRelatedData.flight_status = 0;							// Статус полета	(параметр субполя №4 [STAS])
				cat_11.adsBRelatedData.specific_service = 0;						// Признак спецобслуживания (приоритет обслуживания,	параметр субполя №4 [SSC] )
				cat_11.adsBRelatedData.altitude_report_capability = 0;		// Признак установленной точности определения высоты (параметр субполя №4 [ARC]) 
				cat_11.adsBRelatedData.aircraft_ident_capability = 0;			// Признак возможности идентификации (параметр субполя №4 [AIC])
				cat_11.adsBRelatedData.bit_b1a = 0;								// Значение В1А (параметр субполя №4 [В1А])
				cat_11.adsBRelatedData.bits_b1b = 0;								// Значение В1В (параметр субполя №4 [В1В])
				cat_11.adsBRelatedData.acas_operational = 0;					// Признак работоспособности системы ACAS (параметр субполя №4 [AC])
				
	 			// ern::Log::getInstance().Write("Error, missing some fields in cat11: Mode-S / ADS-B Related Data\n");
			}
			else
			{
				cat_11.adsBRelatedData.mode_s_data_count = ptrModeS->mode_s_data_count;
				/*cat_11.adsBRelatedData.mode_s_msg = new u_int64_t[cat_11.adsBRelatedData.mode_s_data_count];
				for(int i = 0; i<cat_11.adsBRelatedData.mode_s_data_count; i++)
					cat_11.adsBRelatedData.mode_s_msg[i] = ptrModeS->mode_s_msg[i];*/
				cat_11.adsBRelatedData.mode_s_msg = 0;
				cat_11.adsBRelatedData.target_address = ptrModeS->target_address;
				cat_11.adsBRelatedData.communication_capability = ptrModeS->communication_capability;
				cat_11.adsBRelatedData.flight_status = ptrModeS->flight_status;
				cat_11.adsBRelatedData.specific_service = ptrModeS->specific_service;
				cat_11.adsBRelatedData.altitude_report_capability = ptrModeS->altitude_report_capability;
				cat_11.adsBRelatedData.aircraft_ident_capability = ptrModeS->aircraft_ident_capability;
				cat_11.adsBRelatedData.bit_b1a = ptrModeS->bit_b1a;
				cat_11.adsBRelatedData.bits_b1b = ptrModeS->bits_b1b;
				cat_11.adsBRelatedData.acas_operational = ptrModeS->acas_operational;
			}
			//-----------------------
			//	далее требуется осуществить проверку занятости ВПП и актуализацию трасс с учетом меток времени
			// отсечем все устаревшие трассы
			ptrTrackList->checkActualizationTraces(cat_11.time_of_track);
			// проверим занятость ВПП на основе данных трасс, и обновим состояния ВПП аэродромов
			//-----------------------
			// полученную запись нужно прикрепить к одной из трасс
			Track* ptrTrack;
			ptrTrack = ptrTrackList->getTrack(cat_11.track_number);
			if(ptrTrack == 0)	// с таким номером трассы еще нет
			{
				// тогда нужно создать новую трассу и прикрепить к ней запись,
				// а новую трассу добавить в список трасс
				ptrTrack = new Track(cat_11);
				ptrTrackList->addTrack(ptrTrack);
			}
			else
				ptrTrack->addCat11Asterix(cat_11);
			//-----------------------
			// проверка местоположения цели для которой получена запись (не оказалась ли цель на одной из ВПП)
			Point temp_point(cat_11.pos_cart.y, cat_11.pos_cart.x, ptrAirdrome->getPtrAirRefPoint()->getHeight(), 0, CPA);
			if(ptrAirdrome->getPtrRunway(1)->isRunwayEmploymentStatus(&temp_point) )
			{
				// если цель на ВПП1
				ptrTrack->setLocatedRunway1(true);
				ptrAirdrome->getPtrRunway(1)->setRunwayEmploymentStatus(true);
			}
			else
			{
				ptrTrack->setLocatedRunway1(false);
				// проверим занятость ВПП на основе данных трасс, и обновим состояния ВПП аэродромов
				if(ptrTrackList->isRunwayEmploymentStatus(1) )
					ptrAirdrome->getPtrRunway(1)->setRunwayEmploymentStatus(true);
				else
					ptrAirdrome->getPtrRunway(1)->setRunwayEmploymentStatus(false);
			}
			//-----------------------				
			if(ptrAirdrome->getPtrRunway(2)->isRunwayEmploymentStatus(&temp_point))
			{
				// если цель на ВПП2
				ptrTrack->setLocatedRunway2(true);
				ptrAirdrome->getPtrRunway(2)->setRunwayEmploymentStatus(true);
			}
			else
			{
				ptrTrack->setLocatedRunway2(false);
				if(ptrTrackList->isRunwayEmploymentStatus(2) )
					ptrAirdrome->getPtrRunway(2)->setRunwayEmploymentStatus(true);
				else
					ptrAirdrome->getPtrRunway(2)->setRunwayEmploymentStatus(false);
			}
			//------------------------
			// Вывод на консоль результат состояния ВПП
			/*Получение дискриптора*/
			#ifdef __linux__ 
				//linux code goes here
	
			#elif _WIN32
				// windows code goes here
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
			#else
				// #error "OS not supported!"
			#endif

			/*Цвет всего фона - белый. Цвет всего текста - черный */
			// system("color F0");
			// puts("Hello World!");

			if( ptrAirdrome->getPtrRunway(1)->getRunwayEmploymentStatus() )
			{
				#ifdef __linux__ 
				//linux code goes here
				std::cout << "Runway1 busy		    ";	
				#elif _WIN32
				// windows code goes here
					/*Цвет символов - белый. Цвет фона - красный*/
					SetConsoleTextAttribute(hConsole, (WORD) ((RED << 4) | WHITE));	
					printf("Runway1 busy		    ");
				#else
				// #error "OS not supported!"
				#endif			
				// std::cout << "Runway1 busy		";	
			}
			else
			{
				#ifdef __linux__ 
				//linux code goes here
					std::cout <<	"Runway1 is free        ";	
				#elif _WIN32
				// windows code goes here
				/*Цвет символов - белый. Цвет фона - зеленый*/
					SetConsoleTextAttribute(hConsole, (WORD) ((GREEN << 4) | WHITE));	
					printf("Runway1 is free		    ");
				#else
				// #error "OS not supported!"
				#endif	
				// std::cout <<	"Runway1 is free		";
			}

			if( ptrAirdrome->getPtrRunway(2)->getRunwayEmploymentStatus() )
			{
				#ifdef __linux__ 
				//linux code goes here
				std::cout << "	Runway2 busy";	
				#elif _WIN32
				// windows code goes here
					/*Цвет символов - белый. Цвет фона - красный*/
					SetConsoleTextAttribute(hConsole, (WORD) ((RED << 4) | WHITE));	
					puts("	        Runway2 busy");
				#else
				// #error "OS not supported!"
				#endif
				// std::cout << "	Runway2 busy"	<< std::endl;	
			}
			else
			{
				#ifdef __linux__ 
				//linux code goes here
					std::cout <<	"	       Runway2 is free";	
				#elif _WIN32
				// windows code goes here
				/*Цвет символов - белый. Цвет фона - зеленый*/
					SetConsoleTextAttribute(hConsole, (WORD) ((GREEN << 4) | WHITE));	
					puts("	        Runway2 is free");
				#else
				// #error "OS not supported!"
				#endif	
				// std::cout << "	Runway2 is free"	<< std::endl;	
			}

			// drawTargetSituation();
			//------------------------
			if(this->isStartCalculation(cat_11))
				// isBegObservPeriodSpecified = false;
				StartCalculation();
			break;
		}
	case Cat11::MT_MODIFICATION_FLIGHT_PLAN:
		break;
	case Cat11::MT_MANUAL_ATTACH_FLIGHT_PLAN:
	case Cat11::MT_MANUAL_DETACHMENT_FLIGHT_PLAN:
	case Cat11::MT_INSERT_FLIGHT_PLAN:
	case Cat11::MT_SUPPRESSION_FLIGHT_PLAN:		
	case Cat11::MT_HOLDBAR_STATUS:
		break;
	}
}
//-------------------------------------------------------------------------------------------------
// разбирает новую запись категории 11 Астерикс, 
// данные добавляются в соответствующие поля класса, 
// а строка дозаписывается в csv-файл
void AsterixWorker::handleCat11(TRecCategory_11 *ptrCat11)
{
	double eps = 0.000001;
	uint8_t msg_type;				// элемент данных I011/000
	
	msg_type =	ptrCat11->msg_type;	

	switch (msg_type)
	{
		case MT_UNKNOWN:
			break;
		case MT_TRACK_AND_FLIGHT_PLAN:
		{
			int x;
			RecCategory_11 cat_11;
			
			cat_11.msg_type = msg_type;
			//-----------------------------
			cat_11.dsi.sac = ptrCat11->dsi.sac;
			cat_11.dsi.sic = ptrCat11->dsi.sic;
			//----------------------------
			if(ptrCat11->service_id == 0)
			{
				cat_11.service_id = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Service Identification\n");
			}
			else
				cat_11.service_id = ptrCat11->service_id;
			//----------------------------
			cat_11.pos_wgs84.wgs84_lat = ptrCat11->pos_wgs84.wgs84_lat;
			cat_11.pos_wgs84.wgs84_lon =  ptrCat11->pos_wgs84.wgs84_lon;
			//---------------------------
			if(abs(ptrCat11->pos_cart.x) < eps  &&  abs(ptrCat11->pos_cart.y) < eps)
			{
				cat_11.pos_cart.x = 10000;
				cat_11.pos_cart.y = 10000;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Position in Cartesian Co-ordinates\n");
			}
			else
			{
				// координаты X и Y заданы относительно референсной точки, заданной в конфигурационном файле
				// wgs_84_point_cpa.wgs_Y_lat = ern::ConfiguratorAirports::getInstance().getLatCTA();     // задаeтся референсная точка (как задано у Лаврентия)
				// wgs_84_point_cpa.wgs_X_lon = ern::ConfiguratorAirports::getInstance().getLonCTA();	// задается референсная точка (как задано у Лаврентия)
				// получим координаты точки на плоскости в системе Гаусса-Крюгера
				/*double Conf_North_Y = 0;
				double Conf_East_X = 0;
				WgsToLocPr(ern::ConfiguratorAirports::getInstance().getLatCTA(), ern::ConfiguratorAirports::getInstance().getLonCTA(), &Conf_East_X, &Conf_North_Y);

				double My_North_Y = 0;
				double My_East_X = 0;
				WgsToLocPr(MyLAT, MyLON, &My_East_X, &My_North_Y);

				cat_11.pos_cart.x = ptrCat11->pos_cart.x + Conf_East_X - My_East_X;
				cat_11.pos_cart.y = ptrCat11->pos_cart.y + Conf_North_Y - My_North_Y;*/

				// получим координаты точки на плоскости в проекции Меркатора
				double conf_northing_y = 0;
				double conf_easting_x = 0;
				// WgsToLocPr(ern::ConfiguratorAirports::getInstance().getLatCTA(), ern::ConfiguratorAirports::getInstance().getLonCTA(), &Conf_East_X, &Conf_North_Y);
				
				// GeoCalc geo_calc;
				// geo_calc.LatLong2UTMMerc(ern::ConfiguratorAirports::getInstance().getLonCTA(), ern::ConfiguratorAirports::getInstance().getLatCTA(), &conf_easting_x, &conf_northing_y); 
				
				GeoCalc geo_calc;		// добавляется смещение, которое имеет место быть в данной версии (эту поправку стоит учитывать и при обратном преобразовании)
				geo_calc.LatLong2UTMMerc(ern::ConfiguratorAirports::getInstance().getLonCTA() + 0.001852, ern::ConfiguratorAirports::getInstance().getLatCTA() + 0.00006575, &conf_easting_x, &conf_northing_y);
	
				
				double my_northing_y = 0;
				double my_easting_x = 0;
				// WgsToLocPr(MyLAT, MyLON, &My_East_X, &My_North_Y);
				geo_calc.LatLong2UTMMerc(MyLON, MyLAT, &my_easting_x, &my_northing_y); 

				// Point temp_point(cat_11.pos_cart.y + Conf_North_Y - My_North_Y, cat_11.pos_cart.x + Conf_East_X - My_East_X, ptrAirdrome->getPtrAirRefPoint()->getHeight(), 0, CPA);
				/*cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 1.03;
				if(abs(conf_northing_y - my_northing_y) > 1080)
					cat_11.pos_cart.y = ptrCat11->pos_cart.y + (conf_northing_y - my_northing_y) / 1.06623;
				else
					cat_11.pos_cart.y = ptrCat11->pos_cart.y + (conf_northing_y - my_northing_y) / 1.29;*/
			
				// cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 1.01941 - 34.3;
				// cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 0.968176;
				// cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 0.968176 + 25;
				// cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 0.968176 + 25 + 40;
				// cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 0.968176 + 25 + 42;
				// cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 0.968176 + 25 + 43;
				cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 0.968176 + 25 - 2;
				// cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 0.968176 + 25 - 40;
				// cat_11.pos_cart.y = ptrCat11->pos_cart.y + (conf_northing_y - my_northing_y) * 1.03356 + 1.95;
				cat_11.pos_cart.y = ptrCat11->pos_cart.y + (conf_northing_y - my_northing_y) * 1.0557432 - 2; // + 1.95;
			}
			//--------------------------
			// Mode 3/A Code
			if(ptrCat11->mode_3A == 0)
			{
				cat_11.mode_3A = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Mode 3/A Code\n");
			}
			else
				cat_11.mode_3A = ptrCat11->mode_3A;
			//-------------------------
			// Measured Flight Level
			cat_11.flight_level = ptrCat11->flight_level;
			//-------------------------
			// Calculated Track Geometric Altitude
			cat_11.geo_altitude = ptrCat11->geo_altitude;
			//-------------------------
			//  Calculated Track Barometric Altitude
			cat_11.baro_alt.qnh = ptrCat11->baro_alt.qnh;
			cat_11.baro_alt.altitude = ptrCat11->baro_alt.altitude;
			//-------------------------
			//	Time of Track
			cat_11.time_of_track = ptrCat11->time_of_track;	
			//-------------------------
			// Track Number
			cat_11.track_number = ptrCat11->track_number;
			//-------------------------
			cat_11.tr_status.monosensor = ptrCat11->tr_status.monosensor;								cat_11.tr_status.spi_sign = ptrCat11->tr_status.spi_sign;
			cat_11.tr_status.most_reliable_height = ptrCat11->tr_status.most_reliable_height;		cat_11.tr_status.geometric_altitude_source = ptrCat11->tr_status.geometric_altitude_source;
			cat_11.tr_status.confirm = ptrCat11->tr_status.confirm;												cat_11.tr_status.simulate = ptrCat11->tr_status.simulate;
			cat_11.tr_status.last_message = ptrCat11->tr_status.last_message;								cat_11.tr_status.first_message = ptrCat11->tr_status.first_message;
			cat_11.tr_status.flight_in_plan = ptrCat11->tr_status.flight_in_plan;							cat_11.tr_status.ads_inconsist = ptrCat11->tr_status.ads_inconsist;
			cat_11.tr_status.slave_track = ptrCat11->tr_status.slave_track;									cat_11.tr_status.background_serv = ptrCat11->tr_status.background_serv;
			cat_11.tr_status.amalgamation = ptrCat11->tr_status.amalgamation;							cat_11.tr_status.mode4_interrogate = ptrCat11->tr_status.mode4_interrogate;
			cat_11.tr_status.military_emrg = ptrCat11->tr_status.military_emrg;							cat_11.tr_status.military_id = ptrCat11->tr_status.military_id;
			cat_11.tr_status.mode5_interrogate = ptrCat11->tr_status.mode5_interrogate;			cat_11.tr_status.coast_track = ptrCat11->tr_status.coast_track;
			cat_11.tr_status.psr_coast = ptrCat11->tr_status.psr_coast;										cat_11.tr_status.ssr_coast = ptrCat11->tr_status.ssr_coast;
			cat_11.tr_status.mode_s_coast = ptrCat11->tr_status.mode_s_coast;							cat_11.tr_status.ads_coast = ptrCat11->tr_status.ads_coast;
			cat_11.tr_status.suc_set = ptrCat11->tr_status.suc_set;												cat_11.tr_status.aac_conflict = ptrCat11->tr_status.aac_conflict;
			cat_11.tr_status.sds_status = ptrCat11->tr_status.sds_status;										cat_11.tr_status.emergency_status = ptrCat11->tr_status.emergency_status;
			cat_11.tr_status.ground_bit_set = ptrCat11->tr_status.ground_bit_set;						cat_11.tr_status.horizontal_manoeuvre = ptrCat11->tr_status.horizontal_manoeuvre;                
			cat_11.tr_status.slant_correction = ptrCat11->tr_status.slant_correction;						cat_11.tr_status.smooth_position = ptrCat11->tr_status.smooth_position;
			//-------------------------
			// Calculated Track Velocity in Cartesian Coord.
			cat_11.tr_velocity.vx = ptrCat11->tr_velocity.vx;
			cat_11.tr_velocity.vy = ptrCat11->tr_velocity.vy;
			//-------------------------
			// Calculated Acceleration
			cat_11.acceleration.ax = ptrCat11->acceleration.ax;
			cat_11.acceleration.ay = ptrCat11->acceleration.ay;
			//-------------------------
			// Calculated Rate Of Climb/Descent
			if( abs(ptrCat11->calc_vert_rate) < eps)
			{
				cat_11.calc_vert_rate = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11:_vert_rate\n");
			}
			else
				cat_11.calc_vert_rate = ptrCat11->calc_vert_rate;
			//-------------------------
			// Target Identification
			cat_11._call_sign.sti = ptrCat11->_call_sign.sti;
			strncpy(cat_11._call_sign.callsign, ptrCat11->_call_sign.callsign, cat_11._call_sign.CALLSIGN_SIZE);
			//-------------------------
			// Target Size & Orientation
			if( abs(ptrCat11->target_size.length) < eps && abs(ptrCat11->target_size.width) < eps && abs(ptrCat11->target_size.orientation) < eps )
			{
				cat_11.target_size.length = 0;
				cat_11.target_size.width = 0;
				cat_11.target_size.orientation = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Target Size & Orientation\n");
			}
			else
			{
				cat_11.target_size.length = ptrCat11->target_size.length;
				cat_11.target_size.width =  ptrCat11->target_size.width;
				cat_11.target_size.orientation = ptrCat11->target_size.orientation;
			}
			//-------------------------
			// System Track Update Ages
			cat_11._ages.psr_age = ptrCat11->_ages.psr_age;				cat_11._ages.ssr_age = ptrCat11->_ages.ssr_age;
			cat_11._ages.mda_age = ptrCat11->_ages.mda_age;			cat_11._ages.mfl_age = ptrCat11->_ages.mfl_age;
			cat_11._ages.mds_age = ptrCat11->_ages.mds_age;				cat_11._ages.ads_age = ptrCat11->_ages.ads_age;
			cat_11._ages.ads_es_age = ptrCat11->_ages.ads_es_age;		cat_11._ages.md1_age = ptrCat11->_ages.md1_age;
			cat_11._ages.md2_age = ptrCat11->_ages.md2_age;			cat_11._ages.loop_age = ptrCat11->_ages.loop_age;
			cat_11._ages.trk_age = ptrCat11->_ages.trk_age;					cat_11._ages.mlt_age = ptrCat11->_ages.mlt_age;
			//-------------------------
			// Vehicle Fleet Identification
			if(ptrCat11->fleet == 0)
			{
				cat_11.fleet = 0;			
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Vehicle Fleet Identification\n");
			}
			else
				cat_11.fleet = ptrCat11->fleet;
			//-------------------------
			// Pre-programmed Message
			cat_11._prog_msg.trouble =	ptrCat11->_prog_msg.trouble;
			cat_11._prog_msg.type =		ptrCat11->_prog_msg.type;
			
			cat_11.adsBRelatedData.mode_s_data_count = ptrCat11->adsBRelatedData.mode_s_data_count;
			/*cat_11.adsBRelatedData.mode_s_msg = new u_int64_t[cat_11.adsBRelatedData.mode_s_data_count];
			for(int i = 0; i<cat_11.adsBRelatedData.mode_s_data_count; i++)
				cat_11.adsBRelatedData.mode_s_msg[i] = ptrCat11->adsBRelatedData.mode_s_msg[i];			*/
			cat_11.adsBRelatedData.mode_s_msg = 0;
			cat_11.adsBRelatedData.target_address = ptrCat11->adsBRelatedData.target_address;
			cat_11.adsBRelatedData.communication_capability = ptrCat11->adsBRelatedData.communication_capability;
			cat_11.adsBRelatedData.flight_status = ptrCat11->adsBRelatedData.flight_status;
			cat_11.adsBRelatedData.specific_service = ptrCat11->adsBRelatedData.specific_service;
			cat_11.adsBRelatedData.altitude_report_capability = ptrCat11->adsBRelatedData.altitude_report_capability;
			cat_11.adsBRelatedData.aircraft_ident_capability = ptrCat11->adsBRelatedData.aircraft_ident_capability;
			cat_11.adsBRelatedData.bit_b1a = ptrCat11->adsBRelatedData.bit_b1a;
			cat_11.adsBRelatedData.bits_b1b = ptrCat11->adsBRelatedData.bits_b1b;
			cat_11.adsBRelatedData.acas_operational = ptrCat11->adsBRelatedData.acas_operational;
			//-----------------------
			//	далее требуется осуществить проверку занятости ВПП и актуализацию трасс с учетом меток времени
			// отсечем все устаревшие трассы
			ptrTrackList->checkActualizationTraces(cat_11.time_of_track);
			// проверим занятость ВПП на основе данных трасс, и обновим состояния ВПП аэродромов
			//-----------------------
			// полученную запись нужно прикрепить к одной из трасс
			Track* ptrTrack;
			ptrTrack = ptrTrackList->getTrack(cat_11.track_number);
			if(ptrTrack == 0)	// с таким номером трассы еще нет
			{
				// тогда нужно создать новую трассу и прикрепить к ней запись,
				// а новую трассу добавить в список трасс
				ptrTrack = new Track(cat_11);
				ptrTrackList->addTrack(ptrTrack);
			}
			else
				ptrTrack->addCat11Asterix(cat_11);
			//-----------------------
			Point temp_point(cat_11.pos_cart.y, cat_11.pos_cart.x, ptrAirdrome->getPtrAirRefPoint()->getHeight(), 0, CPA);
			if(ptrAirdrome->getPtrRunway(1)->isRunwayEmploymentStatus(&temp_point) )
			{
				// если цель на ВПП1
				ptrTrack->setLocatedRunway1(true);
				ptrAirdrome->getPtrRunway(1)->setRunwayEmploymentStatus(true);
			}
			else
			{
				ptrTrack->setLocatedRunway1(false);
				// проверим занятость ВПП на основе данных трасс, и обновим состояния ВПП аэродромов
				if(ptrTrackList->isRunwayEmploymentStatus(1) )
					ptrAirdrome->getPtrRunway(1)->setRunwayEmploymentStatus(true);
				else
					ptrAirdrome->getPtrRunway(1)->setRunwayEmploymentStatus(false);
			}
			//-----------------------				
			if(ptrAirdrome->getPtrRunway(2)->isRunwayEmploymentStatus(&temp_point))
			{
				// если цель на ВПП2
				ptrTrack->setLocatedRunway2(true);
				ptrAirdrome->getPtrRunway(2)->setRunwayEmploymentStatus(true);
			}
			else
			{
				ptrTrack->setLocatedRunway2(false);
				if(ptrTrackList->isRunwayEmploymentStatus(2) )
					ptrAirdrome->getPtrRunway(2)->setRunwayEmploymentStatus(true);
				else
					ptrAirdrome->getPtrRunway(2)->setRunwayEmploymentStatus(false);
			}
			//------------------------
			// Вывод на консоль результат состояния ВПП
			/*Получение дискриптора*/
			#ifdef __linux__ 
				//linux code goes here
	
			#elif _WIN32
				// windows code goes here
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
			#else
				// #error "OS not supported!"
			#endif

			/*Цвет всего фона - белый. Цвет всего текста - черный */
			// system("color F0");
			// puts("Hello World!");	

			if( ptrAirdrome->getPtrRunway(1)->getRunwayEmploymentStatus() )
			{
				#ifdef __linux__ 
				//linux code goes here
				std::cout << "Runway1 busy		    ";	
				#elif _WIN32
				// windows code goes here
					/*Цвет символов - белый. Цвет фона - красный*/
					SetConsoleTextAttribute(hConsole, (WORD) ((RED << 4) | WHITE));	
					printf("Runway1 busy		    ");
				#else
				// #error "OS not supported!"
				#endif	
				// std::cout << "Runway1 busy		";
			}
			else
			{
				#ifdef __linux__ 
				//linux code goes here
					std::cout <<	"Runway1 is free        ";	
				#elif _WIN32
				// windows code goes here
				/*Цвет символов - белый. Цвет фона - зеленый*/
					SetConsoleTextAttribute(hConsole, (WORD) ((GREEN << 4) | WHITE));	
					printf("Runway1 is free		    ");
				#else
				// #error "OS not supported!"
				#endif
				//	std::cout <<	"Runway1 is free		";
			}

			if( ptrAirdrome->getPtrRunway(2)->getRunwayEmploymentStatus() )
			{
				#ifdef __linux__ 
				//linux code goes here
				std::cout << "	Runway2 busy";	
				#elif _WIN32
				// windows code goes here
					/*Цвет символов - белый. Цвет фона - красный*/
					SetConsoleTextAttribute(hConsole, (WORD) ((RED << 4) | WHITE));	
					puts("	        Runway2 busy");
				#else
				// #error "OS not supported!"
				#endif	
				// std::cout << "	Runway2 busy"	<< std::endl;	
			}
			else
			{
				#ifdef __linux__ 
				//linux code goes here
					std::cout <<	"	       Runway2 is free";	
				#elif _WIN32
				// windows code goes here
				/*Цвет символов - белый. Цвет фона - зеленый*/
					SetConsoleTextAttribute(hConsole, (WORD) ((GREEN << 4) | WHITE));	
					puts("	        Runway2 is free");
				#else
				// #error "OS not supported!"
				#endif		
				// std::cout << "	Runway2 is free"	<< std::endl;	
			}

			drawTargetSituation();
			//------------------------
			if(this->isStartCalculation(cat_11))
				// isBegObservPeriodSpecified = false;
				StartCalculation();
			break;
		}
	case MT_MODIFICATION_FLIGHT_PLAN:
		break;
	case MT_MANUAL_ATTACH_FLIGHT_PLAN:
	case MT_MANUAL_DETACHMENT_FLIGHT_PLAN:
	case MT_INSERT_FLIGHT_PLAN:
	case MT_SUPPRESSION_FLIGHT_PLAN:		
	case MT_HOLDBAR_STATUS:
		break;
	}
}
//-------------------------------------------------------------------------------------------------------------
// разбирает полученную строку АСТЕРИКС категории 011 [buffAsterixMsgInfo]
// и помещаем в соответствующие элементы хранения данных в классе
void AsterixWorker::parseAsterixMsgCat11()
{
	char errINFO[80];
	unsigned char* data = (unsigned char*) buffAsterixMsgCat011;
	// std::ofstream out_csv_file;
	// out_csv_file.open(csv_file_cat11, std::ofstream::out | std::ofstream::app);

	AsterixDecoder decoder(A_DECODE_DELIMITED, false);
	AsterixDecoder* dec;		
	dec = &decoder;
	// AsterixCoder   coder(A_CODE_SINGLE);

	using Cat11::Cat11;
	using namespace Cat11;

	dec->feed(data, size_buff_Asterix_MsgCat011_);
	// закодированную полученную строку необходимо раскодировать и записи трасс считать в цикле
	/*AsterixRecord* r = decoder.decode();
	Cat11* cat_11 = dynamic_cast<Cat11*>(r);*/
	while (AsterixRecord* r = dec->decode())
	{
		using namespace Cat11;
		
		if(struct Cat11* cat11 =  dynamic_cast<struct Cat11*>(r))
			handleCat11(cat11);
		else
		{
			sprintf(errINFO, "Unknown category: %d \n", r->category);
			ern::Log::getInstance().Write(errINFO);
		}			
		delete r;
	}
	memset(this->buffAsterixMsgCat011, 0, this->size_buff_Asterix_MsgCat011_);
	size_buff_Asterix_MsgCat011_ = 0;
}
//-------------------------------------------------------------------------------------------------------------
// разбирает полученную строку АСТЕРИКС категории 011 [buffAsterixMsgInfo]
// и помещаем в соответствующие элементы хранения данных в классе
void AsterixWorker::parseAsterixMsgCat11(TRecCategory_11 *ptrCat11)
{
	handleCat11(ptrCat11);	
	size_buff_Asterix_MsgCat011_ = 0;
}
//------------------------------------------------------------------------------------------------------
// перересовывает текущую целевую обстановку
void AsterixWorker::drawTargetSituation()
{
	int ch, num_simb_x, num_simb_y;
	double doble_num_simb_x, doble_num_simb_y, ostatok_x, ostatok_y;
	double ref_point_x, ref_point_y, ref_point_cta_x, ref_point_cta_y;
	double x, y;
	TTargetEnvironment target_envs[24];
		
	// ref_point_cta_x = 2982.225;
	// ref_point_cta_y = - 1263.06;

	memset(target_envs, 0, sizeof(TTargetEnvironment)*24 );

	// 1 шаг: метим данные для отрисовки взлетно-посадочных полос
	// ВПП2
	target_envs[0].str_status[25] = 2;
	target_envs[0].str_status[26] = 2;
	target_envs[1].str_status[26] = 2;
	target_envs[1].str_status[27] = 2;
	target_envs[1].str_status[28] = 2;
	target_envs[1].str_status[29] = 2;
	target_envs[1].str_status[30] = 2;
	target_envs[2].str_status[31] = 2;
	target_envs[2].str_status[32] = 2;
	target_envs[2].str_status[33] = 2;
	target_envs[2].str_status[34] = 2;
	target_envs[2].str_status[35] = 2;
	target_envs[2].str_status[36] = 2;
	target_envs[3].str_status[36] = 2;
	target_envs[3].str_status[37] = 2;
	target_envs[3].str_status[38] = 2;
	target_envs[3].str_status[39] = 2;
	target_envs[3].str_status[40] = 2;
	target_envs[3].str_status[41] = 2;
	target_envs[3].str_status[42] = 2;
	target_envs[4].str_status[42] = 2;
	target_envs[4].str_status[43] = 2;
	target_envs[4].str_status[44] = 2;
	target_envs[4].str_status[45] = 2;
	target_envs[4].str_status[46] = 2;
	target_envs[5].str_status[47] = 2;
	target_envs[5].str_status[48] = 2;
	target_envs[5].str_status[49] = 2;
	target_envs[5].str_status[50] = 2;
	target_envs[5].str_status[51] = 2;
	target_envs[5].str_status[52] = 2;
	target_envs[6].str_status[52] = 2;
	target_envs[6].str_status[53] = 2;
	target_envs[6].str_status[54] = 2;
	target_envs[6].str_status[55] = 2;
	target_envs[6].str_status[56] = 2;
	target_envs[7].str_status[56] = 2;
	target_envs[7].str_status[57] = 2;
	target_envs[7].str_status[58] = 2;
	target_envs[7].str_status[59] = 2;
	target_envs[7].str_status[60] = 2;
	target_envs[7].str_status[61] = 2;
	target_envs[7].str_status[62] = 2;
	target_envs[8].str_status[63] = 2;
	target_envs[8].str_status[62] = 2;
	target_envs[8].str_status[63] = 2;
	target_envs[8].str_status[64] = 2;
	target_envs[8].str_status[65] = 2;
	target_envs[8].str_status[66] = 2;
	target_envs[8].str_status[67] = 2;
	target_envs[9].str_status[66] = 2;
	target_envs[9].str_status[67] = 2;
	target_envs[9].str_status[68] = 2;
	target_envs[9].str_status[69] = 2;
	target_envs[9].str_status[70] = 2;
	target_envs[9].str_status[71] = 2;
	target_envs[10].str_status[70] = 2;
	target_envs[10].str_status[71] = 2;
	target_envs[10].str_status[72] = 2;
	// ВПП1
	target_envs[11].str_status[3] = 2;
	target_envs[11].str_status[4] = 2;
	target_envs[11].str_status[5] = 2;
	target_envs[11].str_status[6] = 2;
	target_envs[12].str_status[6] = 2;
	target_envs[12].str_status[7] = 2;
	target_envs[12].str_status[8] = 2;
	target_envs[12].str_status[9] = 2;
	target_envs[12].str_status[10] = 2;
	target_envs[13].str_status[11] = 2;
	target_envs[13].str_status[12] = 2;
	target_envs[13].str_status[13] = 2;
	target_envs[13].str_status[14] = 2;
	target_envs[13].str_status[15] = 2;
	target_envs[13].str_status[16] = 2;
	target_envs[13].str_status[17] = 2;
	target_envs[14].str_status[17] = 2;
	target_envs[14].str_status[18] = 2;
	target_envs[14].str_status[19] = 2;
	target_envs[14].str_status[20] = 2;
	target_envs[14].str_status[21] = 2;
	target_envs[14].str_status[22] = 2;
	target_envs[15].str_status[21] = 2;
	target_envs[15].str_status[22] = 2;
	target_envs[15].str_status[23] = 2;
	target_envs[15].str_status[24] = 2;
	target_envs[15].str_status[25] = 2;
	target_envs[15].str_status[26] = 2;
	target_envs[15].str_status[27] = 2;
	target_envs[16].str_status[26] = 2;
	target_envs[16].str_status[27] = 2;
	target_envs[16].str_status[28] = 2;
	target_envs[16].str_status[29] = 2;
	target_envs[16].str_status[30] = 2;
	target_envs[16].str_status[31] = 2;
	target_envs[16].str_status[32] = 2;	
	target_envs[17].str_status[33] = 2;
	target_envs[17].str_status[34] = 2;
	target_envs[17].str_status[35] = 2;
	target_envs[17].str_status[34] = 2;
	target_envs[17].str_status[35] = 2;
	target_envs[17].str_status[36] = 2;
	target_envs[17].str_status[37] = 2;
	target_envs[18].str_status[37] = 2;
	target_envs[18].str_status[38] = 2;
	target_envs[18].str_status[39] = 2;
	target_envs[18].str_status[40] = 2;
	target_envs[18].str_status[41] = 2;
	target_envs[18].str_status[42] = 2;
	target_envs[19].str_status[41] = 2;
	target_envs[19].str_status[42] = 2;
	target_envs[19].str_status[43] = 2;
	target_envs[19].str_status[44] = 2;
	target_envs[19].str_status[45] = 2;
	target_envs[19].str_status[46] = 2;
	target_envs[19].str_status[47] = 2;
	target_envs[20].str_status[46] = 2;
	target_envs[20].str_status[47] = 2;
	target_envs[20].str_status[48] = 2;
	target_envs[20].str_status[49] = 2;
	target_envs[20].str_status[50] = 2;
	target_envs[20].str_status[51] = 2;
	target_envs[20].str_status[52] = 2;
	
	target_envs[21].str_status[51] = 2;
	target_envs[21].str_status[52] = 2;
	target_envs[21].str_status[53] = 2;
	target_envs[21].str_status[54] = 2;
	target_envs[21].str_status[55] = 2;	
	//-------------------------------
	// контрольная точка
	target_envs[10].str_status[39] = 3;
	// -----------------------------

	// цели
	if( ptrTrackList->getNumberTraces() > 0)
	{
		Track* ptrTrack = ptrTrackList->getBegTrack();
		x = ptrTrack->getCat11Record()->getCat11Record(ptrTrack->getCat11Record()->getCountElemList()-1).pos_cart.x;
		y = ptrTrack->getCat11Record()->getCat11Record(ptrTrack->getCat11Record()->getCountElemList()-1).pos_cart.y;
		
		// if(y < 1263.06 && y > - 1263.06 && x < 2806.785 && x > -2806.785)
		if(y < 0 && y > - 2526.12 && x < 5613.57 && x > 0)
		{
			// x += ref_point_cta_x;
			num_simb_x = x / 70.17;
			doble_num_simb_x = x / 70.17;
			ostatok_x = doble_num_simb_x - num_simb_x;

			num_simb_y = abs( y / 105.255 );
			doble_num_simb_y = abs(y / 105.255);
			ostatok_y = doble_num_simb_y - num_simb_y;
			// num_simb_y = 11 - num_simb_y;
			target_envs[num_simb_y].str_status[num_simb_x] = 1;

			if(ostatok_x > 0.333333 && ostatok_x < 0.6666667 && ostatok_y > 0.333333 && ostatok_y < 0.6666667)
					target_envs[num_simb_y].target_koord[num_simb_x] = 0;
				else if(ostatok_x > 0.5 && ostatok_y > 0.5)
					target_envs[num_simb_y].target_koord[num_simb_x] = 1;
				else if(ostatok_x < 0.5 && ostatok_y > 0.5)
					target_envs[num_simb_y].target_koord[num_simb_x] = 2;
				else if(ostatok_x < 0.5 && ostatok_y < 0.5)
					target_envs[num_simb_y].target_koord[num_simb_x] = 3;
				else
					target_envs[num_simb_y].target_koord[num_simb_x] = 4;
		}

		for(int i=1; i< ptrTrackList->getNumberTraces(); i++)
		{
			ptrTrack = ptrTrackList->getNextTrack();
			x = ptrTrack->getCat11Record()->getCat11Record(ptrTrack->getCat11Record()->getCountElemList()-1).pos_cart.x;
			y = ptrTrack->getCat11Record()->getCat11Record(ptrTrack->getCat11Record()->getCountElemList()-1).pos_cart.y;
			
			// if(y < 1263.06 && y > - 1263.06 && x < 2806.785 && x > -2806.785)
			if(y < 0 && y > - 2526.12 && x < 5613.57 && x > 0)
			{
				// x += ref_point_cta_x;
				num_simb_x = x / 70.17;
				doble_num_simb_x = x / 70.17;
				ostatok_x = doble_num_simb_x - num_simb_x;

				num_simb_y = abs( y / 105.255 );
				doble_num_simb_y = abs(y / 105.255);
				ostatok_y = doble_num_simb_y - num_simb_y;
				// num_simb_y = 11 - num_simb_y;
				target_envs[num_simb_y].str_status[num_simb_x] = 1;

				if(ostatok_x > 0.333333 && ostatok_x < 0.6666667 && ostatok_y > 0.333333 && ostatok_y < 0.6666667)
					target_envs[num_simb_y].target_koord[num_simb_x] = 0;
				else if(ostatok_x > 0.5 && ostatok_y > 0.5)
					target_envs[num_simb_y].target_koord[num_simb_x] = 1;
				else if(ostatok_x < 0.5 && ostatok_y > 0.5)
					target_envs[num_simb_y].target_koord[num_simb_x] = 2;
				else if(ostatok_x < 0.5 && ostatok_y < 0.5)
					target_envs[num_simb_y].target_koord[num_simb_x] = 3;
				else
					target_envs[num_simb_y].target_koord[num_simb_x] = 4;
			}
		}
	}
	// теперь выведем в консоль все элементы
	/*Получение дискриптора*/
	#ifdef __linux__ 
		//linux code goes here
	
	#elif _WIN32
		// windows code goes here
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	#else
		// #error "OS not supported!"
	#endif

	//-------------------------------------------------	
	for(int i = 0; i < 24; i++)
	{
		for(int j = 0; j < 80; j++)
		{
			if( target_envs[i].str_status[j] == 0 ) // это - фон! Он зеленый
			{
				#ifdef __linux__ 
				//linux code goes here
					printf(" ");
				#elif _WIN32
				// windows code goes here
					/*Цвет символов - белый. Цвет фона - зеленый*/
					SetConsoleTextAttribute(hConsole, (WORD) ((GREEN << 4) | WHITE));	
					printf(" ");
				#else
				// #error "OS not supported!"
				#endif
			}
			else if( target_envs[i].str_status[j] == 1 ) // здесь находится цель
			{
				#ifdef __linux__ 
				//linux code goes here
					// printf("0");
					printf("%d", target_envs[i].target_koord[j]);
				#elif _WIN32
				// windows code goes here
					/*Цвет символов - белый. Цвет фона - синий*/
					SetConsoleTextAttribute(hConsole, (WORD) ((BLUE << 4) | WHITE));	
					printf("%d", target_envs[i].target_koord[j]);
				#else
				// #error "OS not supported!"
				#endif				
			}
			else if( target_envs[i].str_status[j] == 2 ) // здесь находится кусочек ВПП
			{
				#ifdef __linux__ 
				//linux code goes here
					printf("X");
				#elif _WIN32
				// windows code goes here
					/*Цвет символов - белый. Цвет фона - серый*/
					SetConsoleTextAttribute(hConsole, (WORD) ((LIGHT_GRAY << 4) | WHITE));	
					printf(" ");
				#else
				// #error "OS not supported!"
				#endif				
			}
			else if( target_envs[i].str_status[j] == 3 ) // здесь находится контрольная точка
			{
				#ifdef __linux__ 
				//linux code goes here
					printf("C");
				#elif _WIN32
				// windows code goes here
					/*Цвет символов - черный. Цвет фона - серый*/
					SetConsoleTextAttribute(hConsole, (WORD) ((WHITE << 4) | BLACK));	
					printf(" ");
				#else
				// #error "OS not supported!"
				#endif				
			}
		}		
	}	

	/*
	#ifdef __linux__ 
	//linux code goes here
		sleep(1000);
	#elif _WIN32
	// windows code goes here
		ch = getch();
	#else
	// #error "OS not supported!"
	#endif	
	// ch = getch();
	*/
}
//------------------------------------------------------------------------------------------------------------
//Функция StartWinsock вызывается из WinMain непосредственно перед вхождением
//в главный цикл обработки сообщений программы. Она инициализирует модуль Winsock
//при помощи WSAStartup. Если при загрузке Winsock происходит ошибка, возвращается
//код ошибки и выводится соответствующее сообщение. Если ошибок не было, 
//функция StartWinsock вызывает функцию PaintWindow для вывода сообщения о Winsock DLL
//[платформазависимая функция]
bool StartWinsock()
{
	bool status;
	status = true;
#ifdef __linux__ 
	//linux code goes here
	
#elif _WIN32
	// windows code goes here
	uint16_t wVersionRequested;
	WSADATA wsaData;  //Сведения о реализации Winsock
	int iErr;        //Код ошибки
	wVersionRequested = MAKEWORD(2, 0);
	//wVersionRequested = MAKEWORD(1, 1);

	char szErrMessage[MAX_ERR_MESSAGE]; //Буфер сообщения об ошибке
	// char msg_WSASYSNOTREADY[] = "Основная сетевая подсистема не готова к сетевой коммуникации.";
	char msg_WSASYSNOTREADY[] = "The main network subsystem is not ready for network communication.\n";

	// char msg_WSAVERNOTSUPPORTED[] = "Версия поддержки Сокетов Windows, которую требуют, \n не обеспечивается этим специфическим выполнением Windows Сокетов.";
	char msg_WSAVERNOTSUPPORTED[] = "Version support Windows sockets that require \n is not provided by this particular Windows Sockets implementation.\n";

	// char msg_WSAEPROCLIM[] = "Предел по количеству задач, поддержанных выполнением \n Сокетов Windows был достигнут.";
	char msg_WSAEPROCLIM[] = "The limit on the number of tasks \n used Windows Sockets implementation has been reached.\n";

	// char msg_WSAEFAULT[] = "lpWSAData не допустимый указатель. Значение параметра namelen \n меньше размера структуры sockaddr";
	char msg_WSAEFAULT[] = "lpWSAData not a valid pointer.\n The value of namelen smaller size of struct sockaddr\n";

	if(iErr = WSAStartup(wVersionRequested, &wsaData))
	{
		// MessageBeep(0);
		switch(iErr)
		{
		case WSASYSNOTREADY:
			// lstrcpy(((LPTSTR)szErrMessage, (LPTSTR)msg_WSANOTINITIALISED);
			sprintf(szErrMessage, "%s\n", msg_WSASYSNOTREADY);
			break;
		case WSAVERNOTSUPPORTED:
			sprintf(szErrMessage, "%s\n", msg_WSAVERNOTSUPPORTED);
			break;
		case WSAEPROCLIM:
			sprintf(szErrMessage, "%s\n", msg_WSAEPROCLIM);
			break;
		case WSAEFAULT:
			sprintf(szErrMessage, "%s\n", msg_WSAEFAULT);
			break;
		}
		// MessageBox(NULL, szErrMessage, "Error", MB_OK);
		// printf(szErrMessage);
		ern::Log::getInstance().Write(szErrMessage);
		// Log& logger = Log::getInstance();
		// logger.Write(szErrMessage);	
		return false;
	}
	else
  		return true;    
#else
// #error "OS not supported!"
#endif
  
	return status;
}
//------------------------------------------------------------------------------------------------------------
// если отработал нормально, то возвращается 1, в противном случае любое другое число
int server(AsterixWorker* ptrAsterixWorker)
{
	char szBuf[512];
    char HostName[20];										//Имя своего хоста
    int err, err1;													// Коды ошибки
	int cycle_number;											// номер цикла	полученного сообщения, если больше 5, то он обнуляется, что является сигналом к проверке файла настроек с
																		// последующим его обновлением (!)
	double cycle_counter;										// счетчик циклов
    int nSize;														//размер структуры настроек
    unsigned int nSizeLin;														//размер структуры настроек
    char szErrMessage [MAX_ERR_MESSAGE];		//Буфер сообщения об ошибке
    
	char szBuffMessageInfo[BUFF_MESSAGE_INFO]; //Буфер №1 для поступающих данных
	unsigned short usPort;
        
	err = 1;
    // char msg_WSAEFAULT[] = "lpWSAData не допустимый указатель. Значение параметра namelen меньше размера структуры sockaddr";
	char msg_WSAEFAULT[] = "lpWSAData not a valid pointer.\n The value of namelen smaller size of struct sockaddr\n";

    // char msg_WSANOTINITIALISED[] = "Перед использование функции необходимо вызвать функцию WSAStartup";
	char msg_WSANOTINITIALISED[] = "Before you use the function you must call the WSAStartup function\n";

    // char msg_WSAENETDOWN[] = "Сбой в сети";
	char msg_WSAENETDOWN[] = "Network failure\n";

    char msg_WSAEADDRINUSE[] = "The specified address already is used\n";//Указанный адрес уже используется";

    // char msg_WSAEINPROGRESS[] = "Выполняется блокирующая функция интерфейса Windows Sockets";
	char msg_WSAEINPROGRESS[] = "At this time, perform a blocking function Windows Sockets Interface\n";

    // char msg_WSAEAFNOSUPPORT[] = "Выбранный протокол не может работать с указанным семейством адресов";
	char msg_WSAEAFNOSUPPORT[] = "The selected protocol may not work with the specified address family\n";

    // char msg_WSAEINVAL[] = "Сокет уже привязан к адресу";
	char msg_WSAEINVAL[] = "The socket is already bound to an address\n";

    // char msg_WSAENOBUFS[] = "Установлено слишком много соединений";
	char msg_WSAENOBUFS[] = "Too many connections\n";

    // har msg_WSAENOTSOCK[] = "Указанный в параметре дескриптор не является дескриптором сокета";
	char msg_WSAENOTSOCK[] = "This parameter descriptor is not a socket handle\n";

	struct sockaddr_in src_recv_addr, dst_arm_send_addr;
    struct sockaddr_in src_addr, dst_addr;

    #ifdef __linux__
        //linux code goes here

    #elif _WIN32
        // windows code goes here
        SOCKET src_sock;
        // socket dst_sock;
        // struct hostent *ent;
    #else
        // #error "OS not supported!"
    #endif

   union a
	{
       int z;
	   struct
	   {
            unsigned char a1;
		    unsigned char a2;
		    unsigned char a3;
		    unsigned char a4;
		}b;
	}val;  

	#ifdef __linux__ 
	//linux code goes here
	int fd;
	if( (fd = socket(PF_INET, SOCK_DGRAM, 0) ) == -1) 
	{
		// perror("error in function socket(...)");
	    ern::Log::getInstance().Write("error in function socket(...)");
		return 0;
	}
	#elif _WIN32
	// windows code goes here
	src_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (src_sock == INVALID_SOCKET)
	{
		 // lstrcpy(szBuf, "Не удалось установить связь с сокетом.\n");
		 // printf("Unable to establish a connection with a socket\n");	
		 ern::Log::getInstance().Write("Unable to establish a connection with a socket\n");
		 return 0; 
	}	
	#else
		// #error "OS not supported!"
	#endif	
       
	
    // получим размер структуры настроек
    #ifdef __linux__
        //linux code goes here
        nSizeLin = sizeof ( sockaddr_in );
    #elif _WIN32
        // windows code goes here
        nSize = sizeof ( SOCKADDR_IN );
    #else
        // #error "OS not supported!"
    #endif
                   
    // почистим память
    //	memset(&src_recv_addr, 0, nSize);
    #ifdef __linux__
        //linux code goes here
        memset(&src_recv_addr, 0, nSizeLin);
    #elif _WIN32
        // windows code goes here
        memset(&src_recv_addr, 0, nSize);
    #else
        // #error "OS not supported!"
    #endif

    // сформируем необходимые данные
    src_recv_addr.sin_family = AF_INET;
    // src_recv_addr.sin_port = htons(59399);
	// atoi(argv[2])
    // src_recv_addr.sin_port = htons(sPort);
	//ConfiguratorApp
    src_recv_addr.sin_port = htons(ern::ConfiguratorApp::getInstance().getPortTestClientCons());
    // src_recv_addr.sin_port = htons(34322);


    // src_recv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    #ifdef __linux__
        //linux code goes here
        src_recv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    #elif _WIN32
        // windows code goes here
        src_recv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    #else
        // #error "OS not supported!"
    #endif

    // char szErrMessage [MAX_ERR_MESSAGE]; //Буфер сообщения об ошибке
    szErrMessage[0] = '\0';

	#ifdef __linux__ 
		//linux code goes here
		 // привяжем сокет к нашим настройкам адреса и порта
         if (bind(fd, (const sockaddr*) &src_recv_addr, sizeof(src_recv_addr)) == -1)
         {
              // printf("Error function Bind: %d\n", err1);
			  sprintf(szErrMessage, "Error function Bind(...)");
			  ern::Log::getInstance().Write(szErrMessage);
				
			  szErrMessage[0] = '\0';
              
              close(fd);
              return 0;
       }
	#elif _WIN32
		// windows code goes here
		// привяжем сокет к нашим настройкам адреса и порта
		 if (bind(src_sock, (SOCKADDR*) &src_recv_addr, sizeof(src_recv_addr)) == SOCKET_ERROR)
         {
              //printf("Ошибка функции Bind: %d\n", WSAGetLastError()); 
			  err1 = WSAGetLastError();
			  // printf("Ошибка функции Bind: %d\n", err1);
			  // printf("Error function Bind: %d\n", err1);
			  sprintf(szErrMessage, "Error function Bind: %d\n", err1);
			  ern::Log::getInstance().Write(szErrMessage);
				
			  szErrMessage[0] = '\0';
              switch(err1)//switch(10014)//
			  {
				  case WSANOTINITIALISED:
					  // lstrcpy((LPTSTR)szErrMessage, (LPTSTR)msg_WSANOTINITIALISED);
					  sprintf(szErrMessage, "%s\n", msg_WSANOTINITIALISED);
					  break;
				  case WSAENETDOWN:
					  // lstrcpy(szErrMessage, msg_WSAENETDOWN);
					  sprintf(szErrMessage, "%s\n", msg_WSAENETDOWN);
		              break;
                  case WSAEADDRINUSE:
		                // lstrcpy(szErrMessage, msg_WSAEADDRINUSE);
						sprintf(szErrMessage, "%s\n", msg_WSAEADDRINUSE);
		                break;
				  case WSAEFAULT:
		                // lstrcpy(szErrMessage, msg_WSAEFAULT);
						sprintf(szErrMessage, "%s\n", msg_WSAEFAULT);
		                break;
                  case WSAEINPROGRESS:
		                // lstrcpy(szErrMessage, msg_WSAEINPROGRESS);
						sprintf(szErrMessage, "%s\n", msg_WSAEINPROGRESS);
		                break;
                  case WSAEAFNOSUPPORT:
		                sprintf(szErrMessage, "%s\n", msg_WSAEAFNOSUPPORT);
		                break;
                  case WSAEINVAL:
		                // lstrcpy(szErrMessage, msg_WSAEINVAL);
						sprintf(szErrMessage, "%s\n", msg_WSAEINVAL);
		                break;
                  case WSAENOBUFS:
		                // lstrcpy(szErrMessage, msg_WSAENOBUFS);
						sprintf(szErrMessage, "%s\n", WSAENOBUFS);
		                break;
                  case WSAENOTSOCK:
		                // lstrcpy(szErrMessage, msg_WSAENOTSOCK);
						sprintf(szErrMessage, "%s\n", msg_WSAENOTSOCK);
		                break;
			  }
			  // printf(szErrMessage);                          
			  ern::Log::getInstance().Write(szErrMessage);
              closesocket(src_sock);
              WSACleanup();
              return 0;
       }	
	#else
		// #error "OS not supported!"
	#endif

	/////////////////////////////
	// начинается серверный цикл приема и обработки и отправления поступающей информации
	cycle_number = 0;
	cycle_counter = 0;
	while(true)
	   {
			int nCharRecv;        //возвращает результат выполнения функции recvfrom;
            //char szBuffMessageInfo[BUFF_MESSAGE_INFO]; //Буфер для поступающих данных
            // char * ptrSzBuffMessageInfo;
			// ptrSzBuffMessageInfo = szBuffMessageInfo;

            // очистим память для принятия информации
            // memset(&dst_arm_send_addr, 0, nSize);
            #ifdef __linux__
                //linux code goes here
                memset(&dst_arm_send_addr, 0, nSizeLin);
            #elif _WIN32
                // windows code goes here
                memset(&dst_arm_send_addr, 0, nSize);
            #else
                // #error "OS not supported!"
            #endif

			memset(szBuffMessageInfo, 0, BUFF_MESSAGE_INFO);
            //MessageBox(hwnd, "Все готово для recvfrom", "Debug step1", MB_OK);    
			// nLength = 0;				 
			// number_buff = 1;	// c szBuffMessageInfo1 никто не работает, тогда работать будем с ним
			// status_msg_1 = 1;
            // nCharRecv = recvfrom(src_sock, szBuffMessageInfo,	BUFF_MESSAGE_INFO, 0, (LPSOCKADDR)&dst_arm_send_addr, &nSize );

            #ifdef __linux__
                //linux code goes here
                nCharRecv = recvfrom(fd, szBuffMessageInfo,	BUFF_MESSAGE_INFO, 0, (sockaddr*)&dst_arm_send_addr, &nSizeLin );
            #elif _WIN32
                // windows code goes here
                nCharRecv = recvfrom(src_sock, szBuffMessageInfo,	BUFF_MESSAGE_INFO, 0, (LPSOCKADDR)&dst_arm_send_addr, &nSize );
            #else
                // #error "OS not supported!"
            #endif
            //----------------------
            #ifdef __linux__
				//linux code goes here
				if(nCharRecv == -1)
                {
                      // printf("Error recvfrom\n");
					   ern::Log::getInstance().Write("Error recvfrom(...)\n");
                       close(fd);
					   return 0;
				}
			#elif _WIN32
				// windows code goes here
				//MessageBox(hwnd, "recvfrom отработал", "Debug step2", MB_OK);
				//char mesgErr[80];
                //sprintf(mesgErr, "%-20s: %d\0", "recvfrom отработал", nCharRecv);
                        
				if(nCharRecv == SOCKET_ERROR)
                {
                      //MessageBox(hwnd, "Ошибка recvfrom", "Ответ", MB_OK);
                      // lstrcpy(szErrMessage, "Ошибка recvfrom");
					  // printf("Error recvfrom\n");
					  ern::Log::getInstance().Write("Error recvfrom\n");
                      closesocket(src_sock);
					  WSACleanup();
					  return nCharRecv;
				 }
			#else
				// #error "OS not supported!"
			#endif

			//char buff[120];
            /*
                  sprintf(buff, "Get broadcast info Ok!\nSystemUTL Host IP:\t%s\nSystemUTL TCP/IP Port:\t%d\n\n",
			      inet_ntoa(dst_arm_send_addr.sin_addr),
			      //dst_arm_send_addr.szServerName,
			      //ntohl(dst_arm_send_addr.sin_port));
                  htons(dst_arm_send_addr.sin_port));
                  MessageBox(hwnd, buff, "Ответ", MB_OK);
		    */
             szBuffMessageInfo[nCharRecv] = '\0';
			 cycle_counter++;
             //MessageBox(NULL, szBuffMessageInfo, "Принятые данные", MB_OK);					
			// AsterixWorker* ptrAsterixWorker = new AsterixWorker();
			ptrAsterixWorker->setAsterixMSG(szBuffMessageInfo, nCharRecv);
			ptrAsterixWorker->parseAsterixMsgCat11();

            #ifdef __linux__
                //linux code goes here
                const char* conf_file_name = "conf_app.xml";
                // const char* conf_data_airodrome_name = "conf_airports.xml";
                // const char* csv_file_cat11 = "cat11_";
            #elif _WIN32
            // windows code goes here

            #else
                // #error "OS not supported!"
            #endif

			// delete ptrAsterixWorker;
			ern::ConfiguratorApp::getInstance().setRecordCount(ptrAsterixWorker->getRecordCount());
			ern::ConfiguratorApp::getInstance().setTableCount(ptrAsterixWorker->getTableCount());
			ern::ConfiguratorApp::getInstance().UpdateXML(conf_file_name);

			cycle_number++;
			if(cycle_number > 5)
			{
				cycle_number = 0;
																
				// сохраним текущее состояние настроек (Ненужно сохранять!!! Необходимые данные уже есть в ptrAsterixWorker, их можно там взять!)
				
				// заливаем конфигуратор данными из текущего файла настроек
				ern::ConfiguratorApp::getInstance().LoadXML(conf_file_name);
				int run_service = ern::ConfiguratorApp::getInstance().getRunService();
																
				

				#ifdef __linux__ 
					//linux code goes here
					if(!run_service)	// пользователь системы решил завершить работу сервиса
					{
							// освобождение ресурсов и выход
							// delete ptrAsterixWorker;
                            close(fd);
							return 1;
					}	
				#elif _WIN32
					// windows code goes here
					if(!run_service)	// пользователь системы решил завершить работу сервиса
						{
							// освобождение ресурсов и выход
							// delete ptrAsterixWorker;
							closesocket(src_sock);
							WSACleanup();
							return 1;
						}	
				#else
					// #error "OS not supported!"
				#endif				
			}

			// delete ptrAsterixWorker;

                        //closesocket(src_sock);
						//return szBuffMessageInfo;
                        /////////////////////////////////////////////////////////////////////////////////////////////////////////
						//  Далее код, обеспечивающий отправку обработанной информации на другой узел в категории АСТЕРИКС 004 //
						//  пока закоментируем его (!)																			   //
						/////////////////////////////////////////////////////////////////////////////////////////////////////////
                        /*   //enum { max = 80 };
                        //char buffErr[max];
		                //BOOL status = TRUE;
                        //int value = 1;

                        //Определяем и заполняем строку для передачи
	                    //char szBuf[1492];
                        //Определим порт "по умолчанию"
	                    usPort = htons(DEF_IP_PORT);

                        //struct sockaddr_in src_addr, dst_addr;
                        //SOCKET dst_sock;

                        //char HostName[20];	                        

                        //int err, nSize;
                        //lstrcpy(szBuf, szBuffMessageInfo);
                        //ZeroMemory( &buffErr, max );
                        dst_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                        if (dst_sock == INVALID_SOCKET)
						{
		                    sprintf(szErrMessage, "Failed to create source socket, error: %d",          
                            WSAGetLastError());
                            WSACleanup();			   
			                //status = FALSE;
                            return szErrMessage;
						}

                        if(gethostname(HostName, 20)!=0)
						{
                            lstrcpy(szErrMessage, "GetHostName Error!");
                            closesocket(dst_sock);
                            WSACleanup();
                            //exit(-1);
							return szErrMessage;
						}
	 
                        ent = gethostbyname(HostName);
	                    /////
	                    ////printf("name = %s   addr=%d:%d:%d:%d", HostName, (unsigned char)ent->h_addr[0], (unsigned char)ent->h_addr[1],
                        ////         (unsigned char)ent->h_addr[2], (unsigned char)ent->h_addr[3]);
	                    ////

                            //почистим память
						ZeroMemory(&src_addr, nSize);
                        ZeroMemory(&dst_addr, nSize);

						//сформируем необходимые данные
						src_addr.sin_family = AF_INET;
                        src_addr.sin_port = usPort;

						val.b.a1 = (unsigned char)ent->h_addr[0];
                        val.b.a2 = (unsigned char)ent->h_addr[1];
                        val.b.a3 = (unsigned char)ent->h_addr[2];
						val.b.a4 = (unsigned char)ent->h_addr[3];

                        src_addr.sin_addr.S_un.S_addr = val.z;

                        dst_addr.sin_port = htons(PortSending);
                        dst_addr.sin_family = AF_INET;
                        dst_addr.sin_addr.S_un.S_addr =  htonl(INADDR_BROADCAST);
							 
						//Привяжем сокет к нашим настройкам адреса и порта
		                err = bind(dst_sock, (SOCKADDR FAR *)&src_addr, sizeof(src_addr));
                        if (err)
						{
                                sprintf(szErrMessage, "Failed to bind source socket, error: %d",          
                                WSAGetLastError());
			                    closesocket(dst_sock);
			                    WSACleanup();                    
                                //status = FALSE;
                                return szErrMessage;
						} 

                            //произведем дополнительные настройки
			            err = setsockopt(dst_sock, SOL_SOCKET, SO_BROADCAST, (char *) &value, sizeof(value)); 
                        if(err)
						{
                            sprintf(szErrMessage, "setsockopt, error %d",          
                            WSAGetLastError());
                            closesocket(dst_sock);
				            WSACleanup();                       
                            //status = FALSE;
                            return szErrMessage; 
						}

                        err = sendto(dst_sock, szBuffMessageInfo, lstrlen(szBuffMessageInfo), 0, 
                                (SOCKADDR FAR *)&dst_addr, sizeof(dst_addr));

                        if(err < 0)
						{
                            sprintf(szErrMessage, "sendto failed, error code %d",          
                            WSAGetLastError());
                            closesocket(dst_sock);
			                WSACleanup();                       
			                //	status = FALSE;
                            return szErrMessage;
						}
						//
						*/
						//////////////////////////////////////////////////
						// конец else условия if(nCharRecv == SOCKET_ERROR)
	            sprintf(szErrMessage, "\n cycle_counter = %6.2f\n", cycle_counter);
				std::cout <<	szErrMessage << std::endl;    
					 
				 if(szBuffMessageInfo != NULL)
				 {
                        // ZeroMemory( &szBuffMessageInfo, BUFF_MESSAGE_INFO );
						memset(&szBuffMessageInfo, 0, BUFF_MESSAGE_INFO);
                        szBuffMessageInfo[0] = '\0';
				 }
		} // конец while

    // closesocket(src_sock);
    
	#ifdef __linux__ 
	//linux code goes here
        close(fd);
	#elif _WIN32
	// windows code goes here
        closesocket(src_sock);
	WSACleanup();	
	#else
		// #error "OS not supported!"
	#endif
	
	return err;	
}
//-------------------------------------------------
// тест с локальными данными 
// если отработал нормально, то возвращается 1, в противном случае любое другое число
int testLocalMeh(AsterixWorker* ptrAsterixWorker)
{
	// char szBuf[512];
    int n_simb;
    int err, err1;							// Коды ошибки
	int cycle_number;					// номер цикла	полученного сообщения, если больше 5, то он обнуляется, что является сигналом к проверке файла настроек с
												// последующим его обновлением (!)
	double cycle_counter;				// счетчик циклов, если номер извлеченной строки меньше двух, то не обрабатывать строку
	
    char szErrMessage [MAX_ERR_MESSAGE];			//Буфер сообщения об ошибке
    char szBuffMessageInfo[BUFF_MESSAGE_INFO];	//Буфер №1 для поступающих данных
	
	std::string str_temp;
	std::vector<std::string> delim_n;
	std::vector<std::string>::iterator iter;

	TRecCategory_11 *ptrCat11;
	err = 1;

	std::string input_file_name = "cat11_1_test";    
    szErrMessage[0] = '\0';	

	/////////////////////////////
	// начинается серверный цикл приема и обработки и отправления поступающей информации
	cycle_number = 0;
	cycle_counter = 0;

	// Step 1. Записи будут извлекаться из входного cvs-файла построчно
	// т.к. размер файла очень большой
	std::ifstream infile( input_file_name.c_str() );

	while( infile )		
	{
		std::string min_str_temp;
		char strTemp[MAX];
		memset(strTemp, 0, MAX);
			
		infile.getline(strTemp, MAX);
		min_str_temp = strTemp;
		cycle_counter += 1;
		
		if(cycle_counter>1.0001)
		{
			if(min_str_temp == "" || min_str_temp == " " || min_str_temp == "  " || min_str_temp == "   " || min_str_temp == "    ")
				return 0;

			// cout << "step 2: min_str_temp = " << min_str_temp << endl;

			while(min_str_temp.size() > 0)
			{
				std::string token;
				n_simb = min_str_temp.find_first_of(";\r\n");

				// cout << "step 3: n_simb = " << n_simb << endl;
				if(n_simb == -1)
					break;

				min_str_temp.copy(strTemp, n_simb, 0);
				strTemp[n_simb] = 0;

				token = strTemp;
				// cout << "step 3: token = " << token << endl;

				delim_n.push_back(token);
				min_str_temp.erase(0, n_simb+1);
			}
						
			//---------------------------
			if(delim_n.size() < 73)
			{
				while(!delim_n.empty()){
					iter = delim_n.begin();		
					delim_n.erase(iter);
				}		
				return 0;
			}
			
			// вместо следующих закоментированных инструкций нужно произвести упаковку 
			// извлеченной из csv-файла записи в структуру категории 11
			///-----------------------------------
			// упаковка идет здесь!
			
			ptrCat11 = new TRecCategory_11();
			memset(ptrCat11, 0, sizeof(TRecCategory_11) );
			//------------------------------------
			str_temp = delim_n[0];			
			ptrCat11->msg_type = atoi(str_temp.c_str());			
			//-----------------------------
			str_temp = delim_n[1];
			ptrCat11->dsi.sac = atoi(str_temp.c_str() );
			str_temp = delim_n[2];
			ptrCat11->dsi.sic = atoi(str_temp.c_str() );
			//----------------------------
			str_temp = delim_n[3];
			ptrCat11->service_id = atoi(str_temp.c_str() );
			//----------------------------
			//  Position in WGS-84 Coordinates
			ptrCat11->pos_wgs84.wgs84_lat = strToDouble(delim_n[4]);
			ptrCat11->pos_wgs84.wgs84_lon =  strToDouble(delim_n[5]);
			//---------------------------
			//  Position in Cartesian Co-ordinates
			ptrCat11->pos_cart.x = strToDouble(delim_n[6]); 
			ptrCat11->pos_cart.y = strToDouble(delim_n[7]);
			//--------------------------
			//  Mode 3/A Code
			str_temp = delim_n[8];
			ptrCat11->mode_3A = atoi(str_temp.c_str() );
			//-------------------------
			// Measured Flight Level
			ptrCat11->flight_level = strToDouble(delim_n[9]);
			//-------------------------
			ptrCat11->geo_altitude = strToDouble(delim_n[10]);
			//-------------------------
			//  Calculated Track Barometric Altitude
			str_temp = delim_n[11];
			if(str_temp == "false")
				ptrCat11->baro_alt.qnh = false;
			else
				ptrCat11->baro_alt.qnh = true;
			
			ptrCat11->baro_alt.altitude = strToDouble(delim_n[12]);
			//-------------------------
			//  Time of Track
			ptrCat11->time_of_track = strToDouble(delim_n[13]);	
			//-------------------------
			//  Track Number
			str_temp = delim_n[14];
			ptrCat11->track_number = atoi(str_temp.c_str() );
			//-------------------------
			str_temp = delim_n[15];
			ptrCat11->tr_status.monosensor = atoi(str_temp.c_str() );	
			//-------------------------
			str_temp = delim_n[16];
			ptrCat11->tr_status.ground_bit_set = atoi(str_temp.c_str() );
			//------------------------
			str_temp = delim_n[17];
			ptrCat11->tr_status.most_reliable_height = atoi(str_temp.c_str() );
			//-----------------------
			str_temp = delim_n[18];			
			ptrCat11->tr_status.geometric_altitude_source = atoi(str_temp.c_str() );
			//-----------------------
			str_temp = delim_n[19];	
			ptrCat11->tr_status.confirm = atoi(str_temp.c_str() );
			//----------------------
			str_temp = delim_n[20];	
			ptrCat11->tr_status.simulate = atoi(str_temp.c_str() );
			//---------------------
			str_temp = delim_n[21];
			ptrCat11->tr_status.last_message = atoi(str_temp.c_str() );
			//----------------------
			str_temp = delim_n[22];
			ptrCat11->tr_status.first_message = atoi(str_temp.c_str() );
			//----------------------
			str_temp = delim_n[23];
			ptrCat11->tr_status.mode4_interrogate = atoi(str_temp.c_str() );
			//--------------------
			str_temp = delim_n[24];
			ptrCat11->tr_status.military_emrg = atoi(str_temp.c_str() );
			//--------------------
			str_temp = delim_n[25];
			ptrCat11->tr_status.military_id = atoi(str_temp.c_str() );
			//--------------------
			str_temp = delim_n[26];
			ptrCat11->tr_status.amalgamation = atoi(str_temp.c_str() );
			//---------------------
			str_temp = delim_n[27];
			ptrCat11->tr_status.spi_sign = atoi(str_temp.c_str() );
			//---------------------
			str_temp = delim_n[28];
			ptrCat11->tr_status.coast_track = atoi(str_temp.c_str() );
			//----------------------
			str_temp = delim_n[29];
			ptrCat11->tr_status.flight_in_plan = atoi(str_temp.c_str() );
			//------------------
			str_temp = delim_n[30];
			ptrCat11->tr_status.ads_inconsist = atoi(str_temp.c_str() );
			//-------------------
			str_temp = delim_n[31];
			ptrCat11->tr_status.psr_coast = atoi(str_temp.c_str() );
			//-----------------
			str_temp = delim_n[32];
			ptrCat11->tr_status.ssr_coast = atoi(str_temp.c_str() );
			//-------------------
			str_temp = delim_n[33];
			ptrCat11->tr_status.mode_s_coast = atoi(str_temp.c_str() );
			//-------------------
			str_temp = delim_n[34];
			ptrCat11->tr_status.ads_coast = atoi(str_temp.c_str() );
			//-------------------
			str_temp = delim_n[35];
			ptrCat11->tr_status.suc_set = atoi(str_temp.c_str() );
			//--------------------
			str_temp = delim_n[36];
			ptrCat11->tr_status.aac_conflict = atoi(str_temp.c_str() );
			//-------------------------
			// Calculated Track Velocity in Cartesian Coord.
			ptrCat11->tr_velocity.vx = strToDouble(delim_n[37]);
			ptrCat11->tr_velocity.vy = strToDouble(delim_n[38]);
			//-------------------------
			// Calculated Acceleration
			ptrCat11->acceleration.ax = strToDouble(delim_n[39]);
			ptrCat11->acceleration.ay = strToDouble(delim_n[40]);
			//-------------------------
			// Calculated Rate Of Climb/Descent
			ptrCat11->calc_vert_rate = strToDouble(delim_n[41]);
			//-------------------------
			// Target Identification
			str_temp = delim_n[42];
			ptrCat11->_call_sign.sti = atoi(str_temp.c_str() );
			// str_temp = delim_n[43];
			// strncpy(ptrCat11->_call_sign.callsign, str_temp.c_str(), ptrCat11->_call_sign.CALLSIGN_SIZE);
			//-------------------------
			// Target Size & Orientation
			ptrCat11->target_size.length = strToDouble(delim_n[44]);
			ptrCat11->target_size.width =  strToDouble(delim_n[45]);
			ptrCat11->target_size.orientation = strToDouble(delim_n[46]);
			//-------------------------
			ptrCat11->_ages.psr_age =	 strToDouble(delim_n[47]);
			//------------------------
			ptrCat11->_ages.ssr_age = strToDouble(delim_n[48]);
			//------------------------
			ptrCat11->_ages.mda_age =	strToDouble(delim_n[49]);
			//-------------------------
			ptrCat11->_ages.mfl_age = strToDouble(delim_n[50]);
			//----------------------------------
			ptrCat11->_ages.mds_age =	strToDouble(delim_n[51]);
			//----------------------------------
			ptrCat11->_ages.ads_age = strToDouble(delim_n[52]);
			//---------------------------------
			ptrCat11->_ages.ads_es_age =	strToDouble(delim_n[53]);
			//--------------------------
			ptrCat11->_ages.md1_age = strToDouble(delim_n[54]);
			//---------------------------------------------
			ptrCat11->_ages.md2_age =	strToDouble(delim_n[55]);	
			//---------------------------
			ptrCat11->_ages.loop_age = strToDouble(delim_n[56]);
			//------------
			ptrCat11->_ages.trk_age =	strToDouble(delim_n[57]);	
			//---------------
			ptrCat11->_ages.mlt_age = strToDouble(delim_n[58]);
			//-------------------------
			// Vehicle Fleet Identification
			str_temp = delim_n[59];
			ptrCat11->fleet = atoi(str_temp.c_str() );
			//--------------------------
			// Pre-programmed Message
			str_temp = delim_n[60];
			if(str_temp == "false")
				ptrCat11->_prog_msg.trouble = false;
			else
				ptrCat11->_prog_msg.trouble = false;
				
			str_temp = delim_n[61];
			ptrCat11->_prog_msg.type = atoi(str_temp.c_str() );
			//--------------------------
			// Mode-S / ADS-B Related Data
	       ptrCat11->adsBRelatedData.mode_s_data_count = 0;
		   //--------------------------
		   ptrCat11->adsBRelatedData.mode_s_msg = 0;
		   //-------------------------
			/*for(int i = 0; i<cat_11.adsBRelatedData.mode_s_data_count; i++)
					cat_11.adsBRelatedData.mode_s_msg[i] = ptrCat11->adsBRelatedData.mode_s_msg[i];*/	
		   //-------------------------
		   str_temp = delim_n[64];
		   ptrCat11->adsBRelatedData.target_address = atoi(str_temp.c_str() );
		   //-------------------------
		   str_temp = delim_n[65];
		   ptrCat11->adsBRelatedData.communication_capability = atoi(str_temp.c_str() );
		   //--------------------------
		   str_temp = delim_n[66];
		   ptrCat11->adsBRelatedData.flight_status = atoi(str_temp.c_str() );
		   //-------------------------
		   str_temp = delim_n[67];
		   ptrCat11->adsBRelatedData.specific_service = atoi(str_temp.c_str() );
		   //-------------------------
		   str_temp = delim_n[68];
		   ptrCat11->adsBRelatedData.altitude_report_capability = atoi(str_temp.c_str() );
			//-----------------------
		   str_temp = delim_n[69];
		   ptrCat11->adsBRelatedData.aircraft_ident_capability = atoi(str_temp.c_str() );
		   //------------------------
		   str_temp = delim_n[70];
		   ptrCat11->adsBRelatedData.bit_b1a = atoi(str_temp.c_str() );
		   //-------------------------
		   str_temp = delim_n[71];
		   ptrCat11->adsBRelatedData.bits_b1b = atoi(str_temp.c_str() );
		   //-------------------------
		   str_temp = delim_n[72];
		   ptrCat11->adsBRelatedData.acas_operational = atoi(str_temp.c_str() );
		
			///------------------------------------------------------------------------
			ptrAsterixWorker->parseAsterixMsgCat11(ptrCat11);

            #ifdef __linux__
                //linux code goes here
                const char* conf_file_name = "conf_app.xml";

            #elif _WIN32
            // windows code goes here

            #else
                // #error "OS not supported!"
            #endif



            // delete ptrAsterixWorker;
			ern::ConfiguratorApp::getInstance().setRecordCount(ptrAsterixWorker->getRecordCount());
			ern::ConfiguratorApp::getInstance().setTableCount(ptrAsterixWorker->getTableCount());
			ern::ConfiguratorApp::getInstance().UpdateXML(conf_file_name);

			cycle_number++;
			if(cycle_number > 5)
			{
				cycle_number = 0;
																
				// заливаем конфигуратор данными из текущего файла настроек
				ern::ConfiguratorApp::getInstance().LoadXML(conf_file_name);
				int run_service = ern::ConfiguratorApp::getInstance().getRunService();
			}                        
			
			sprintf(szErrMessage, "\n cycle_counter = %6.2f\n", cycle_counter);
			std::cout <<	szErrMessage << std::endl;

			while(!delim_n.empty()){
				iter = delim_n.begin();		
				delim_n.erase(iter);
			}		
			delete ptrCat11;
		}
	} // конец while	
}
//-------------------------------------------------
// тестирование точного алгоритма перевода из системы WGS-84 в прямоугольную
void testCalcPrCoord()
{
	double X, Y;
	double res;
	char info[80];

	double *pX;
	double *pY; 

	// инициализируем координаты в метрах
	X = 0;
	Y = 0;
	res = 0;

	// Географические координаты точки (в градусах)
	double dLon = 37.618;
	double dLat = 55.752;
	pX = &X;
	pY = &Y;

	// using namespace airports;

	WgsToLocPr(dLat, dLon, pX, pY);
	sprintf(info, "X =  %8.4f\n", X);
	ern::Log::getInstance().Write(info);

	sprintf(info, "Y =  %8.4f\n", Y);
	ern::Log::getInstance().Write(info);
	res = 1; 
}
//-------------------------------------------------
// для проверки правильности загруженных настроечных данных аэродромов
void testLoadAirportsSetXML()						
{
	char str_temp[80];
	int size_str_temp = 80;
	int fact_size_str_temp;
	char info[80];
	double height_cta;
	double lat_cta;
	double lon_cta;
	int mag_decl_cta;
	int id_vpp1;
	char cat_vpp1;
	char strCatVPP1[2];
	double angle_vpp1;
	double runway_lat_vpp1;
	double runway_lon_vpp1;
	int lenght_vpp1;
	int width_vpp1;
	double north_wgs_lat_vpp1;
	double north_wgs_lon_vpp1;
	double south_wgs_lat_vpp1;
	double south_wgs_lon_vpp1;
	double west_wgs_lat_vpp1;
	double west_wgs_lon_vpp1;
	double east_wgs_lat_vpp1;
	double east_wgs_lon_vpp1;
	int id_vpp2;
	char cat_vpp2;
	char strCatVPP2[2];
	double angle_vpp2;
	double runway_lat_vpp2;
	double runway_lon_vpp2;
	int lenght_vpp2;
	int width_vpp2;
	double north_wgs_lat_vpp2;
	double north_wgs_lon_vpp2;
	double south_wgs_lat_vpp2;
	double south_wgs_lon_vpp2;
	double west_wgs_lat_vpp2;
	double west_wgs_lon_vpp2;
	double east_wgs_lat_vpp2;
	double east_wgs_lon_vpp2;
	//----------------------------------------
	//  <name>Pulkovo</name>
	fact_size_str_temp = ern::ConfiguratorAirports::getInstance().getNameSection(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "airport_name_en =  %s\n", str_temp);	
	ern::Log::getInstance().Write(info);
	//---------------------------------------
    // <height_cta>24</height_cta>
	height_cta = ern::ConfiguratorAirports::getInstance().getHeightCTA();
	sprintf(info, "height_cta =  %10.4f\n", height_cta);
	ern::Log::getInstance().Write(info);
	//--------------------------------------
    // <lat_cta>59.800028</lat_cta>
	lat_cta = ern::ConfiguratorAirports::getInstance().getLatCTA();
	sprintf(info, "lat_cta =  %8.5f\n", lat_cta);
	ern::Log::getInstance().Write(info);
	//-------------------------------------
    // <lon_cta>30.2625</lon_cta>  
	lon_cta = ern::ConfiguratorAirports::getInstance().getLonCTA();
	sprintf(info, "lon_cta =  %8.5f\n", lon_cta);
	ern::Log::getInstance().Write(info);
	//------------------------------------
    // <mag_decl_cta>10</mag_decl_cta>    
	mag_decl_cta = ern::ConfiguratorAirports::getInstance().getMagDeclCTA();
	sprintf(info, "mag_decl_cta =  %d\n", mag_decl_cta);
	ern::Log::getInstance().Write(info);
	//-----------------------------------
    // <caption_vpp1>ВПП-1</caption_vpp1>
	memset(str_temp, 0, 80);
	fact_size_str_temp = ern::ConfiguratorAirports::getInstance().getCaptionVPP1(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "caption_vpp1 =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);
	//------------------------------------
    // <id_vpp1>0</id_vpp1>
	id_vpp1 = ern::ConfiguratorAirports::getInstance().getIdVpp1();
	sprintf(info, "id_vpp1 =  %d\n", id_vpp1);
	ern::Log::getInstance().Write(info);
	//-----------------------------------
    // <cat_vpp1>B</cat_vpp1>
	cat_vpp1 = ern::ConfiguratorAirports::getInstance().getCatVPP1();
	strCatVPP1[0] = cat_vpp1;
	strCatVPP1[1] = '\0';
	sprintf(info, "cat_vpp1 =  %s\n", strCatVPP1);
	ern::Log::getInstance().Write(info);
	//-----------------------------------
    // <angle_vpp1>106.419</angle_vpp1>
	angle_vpp1 = ern::ConfiguratorAirports::getInstance().getAngleVPP1();
	sprintf(info, "angle_vpp1 =  %8.4f\n", angle_vpp1);
	ern::Log::getInstance().Write(info);
	//------------------------------------
    // <runway_lat_vpp1>59.795142</runway_lat_vpp1> 
	runway_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getRunwayLatVPP1();
	sprintf(info, "runway_lat_vpp1 =  %8.5f\n", runway_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//-----------------------------------
    // <runway_lon_vpp1>30.250639</runway_lon_vpp1>
	runway_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getRunwayLonVPP1();
	sprintf(info, "runway_lon_vpp1 =  %8.5f\n", runway_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//-----------------------------------
    // <lenght_vpp1>3782</lenght_vpp1>
	lenght_vpp1 = ern::ConfiguratorAirports::getInstance().getLenghtVPP1();
	sprintf(info, "lenght_vpp1 =  %d\n", lenght_vpp1);
	ern::Log::getInstance().Write(info);
	//----------------------------------
    // <width_vpp1>60</width_vpp1>
	width_vpp1 = ern::ConfiguratorAirports::getInstance().getWidthVPP1();
	sprintf(info, "width_vpp1 =  %d\n", width_vpp1);
	ern::Log::getInstance().Write(info);
	//----------------------------------
    // <sign_left_vpp1>10R</sign_left_vpp1>
	memset(str_temp, 0, 80);
	fact_size_str_temp = ern::ConfiguratorAirports::getInstance().getSignLeftVPP1(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "sign_left_vpp1 =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);
	//----------------------------------
    // <sign_right_vpp1>28L</sign_right_vpp1>    
	memset(str_temp, 0, 80);
	fact_size_str_temp = ern::ConfiguratorAirports::getInstance().getSignRightVPP1(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "sign_right_vpp1 =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);
	//---------------------------------
    // <north_wgs_lat_vpp1>59.800193</north_wgs_lat_vpp1>
	north_wgs_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getNorthWgsLatVPP1();
	sprintf(info, "north_wgs_lat_vpp1 =  %8.5f\n", north_wgs_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//---------------------------------
    // <north_wgs_lon_vpp1>30.218491</north_wgs_lon_vpp1>
	north_wgs_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getNorthWgsLonVPP1();
	sprintf(info, "north_wgs_lon_vpp1 =  %8.5f\n", north_wgs_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    // <south_wgs_lat_vpp1>59.790093</south_wgs_lat_vpp1>
	south_wgs_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getSouthWgsLatVPP1();
	sprintf(info, "south_wgs_lat_vpp1 =  %8.5f\n", south_wgs_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <south_wgs_lon_vpp1>30.282778</south_wgs_lon_vpp1>
	south_wgs_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getSouthWgsLonVPP1();
	sprintf(info, "south_wgs_lon_vpp1 =  %8.5f\n", south_wgs_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <west_wgs_lat_vpp1>59.799652</west_wgs_lat_vpp1>
	west_wgs_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getWestWgsLatVPP1();
	sprintf(info, "west_wgs_lat_vpp1 =  %8.5f\n", west_wgs_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//------------------------------
    // <west_wgs_lon_vpp1>30.218233</west_wgs_lon_vpp1>
	west_wgs_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getWestWgsLonVPP1();
	sprintf(info, "west_wgs_lon_vpp1 =  %8.5f\n", west_wgs_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//-----------------------------
    // <east_wgs_lat_vpp1>59.79059</east_wgs_lat_vpp1>
	east_wgs_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getEastWgsLatVPP1();
	sprintf(info, "east_wgs_lat_vpp1 =  %8.5f\n", east_wgs_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//----------------------------
    // <east_wgs_lon_vpp1>30.283035</east_wgs_lon_vpp1>    
	east_wgs_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getEastWgsLonVPP1();
	sprintf(info, "east_wgs_lon_vpp1 =  %8.5f\n", east_wgs_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//---------------------------
    // <caption_vpp2>ВПП-2</caption_vpp2>
	memset(str_temp, 0, 80);
	fact_size_str_temp = ern::ConfiguratorAirports::getInstance().getCaptionVPP2(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "caption_vpp2 =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);
	//---------------------------
    // <id_vpp2>1</id_vpp2>
	id_vpp2 = ern::ConfiguratorAirports::getInstance().getIdVPP2();
	sprintf(info, "id_vpp2 =  %d\n", id_vpp2);
	ern::Log::getInstance().Write(info);
	//---------------------------
    // <cat_vpp2>A</cat_vpp2>
	cat_vpp2 = ern::ConfiguratorAirports::getInstance().getCatVPP2();
	strCatVPP2[0] = cat_vpp2;
	strCatVPP2[1] = '\0';
	sprintf(info, "cat_vpp2 =  %s\n", strCatVPP2);
	ern::Log::getInstance().Write(info);
	//-----------------------------
    // <angle_vpp2>106.45</angle_vpp2>
	angle_vpp2 = ern::ConfiguratorAirports::getInstance().getAngleVPP2();
	sprintf(info, "angle_vpp2 =  %8.4f\n", angle_vpp2);
	ern::Log::getInstance().Write(info);
	//-----------------------------
    // <runway_lat_vpp2>59.805453</runway_lat_vpp2>
	runway_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getRunwayLatVPP2();
	sprintf(info, "runway_lat_vpp2 =  %8.5f\n", runway_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//----------------------------
    // <runway_lon_vpp2>30.274379</runway_lon_vpp2>
	runway_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getRunwayLonVPP2();
	sprintf(info, "runway_lon_vpp2 =  %8.5f\n", runway_lon_vpp2);
	ern::Log::getInstance().Write(info);
	//---------------------------
    // <lenght_vpp2>3393</lenght_vpp2>
	lenght_vpp2 = ern::ConfiguratorAirports::getInstance().getLenghtVPP2();
	sprintf(info, "lenght_vpp2 =  %d\n", lenght_vpp2);
	ern::Log::getInstance().Write(info);
	//----------------------------
    // <width_vpp2>60</width_vpp2>
	width_vpp2 = ern::ConfiguratorAirports::getInstance().getWidthVPP2();
	sprintf(info, "width_vpp2 =  %d\n", width_vpp2);
	ern::Log::getInstance().Write(info);	
	//-------------------------------
    // <sign_left_vpp2>10R</sign_left_vpp2>
	memset(str_temp, 0, 80);
	fact_size_str_temp = ern::ConfiguratorAirports::getInstance().getSignLeftVPP2(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "sign_left_vpp2 =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <sign_right_vpp2>28L</sign_right_vpp2>    
	memset(str_temp, 0, 80);
	fact_size_str_temp = ern::ConfiguratorAirports::getInstance().getSignRightVPP2(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "sign_right_vpp2 =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <north_wgs_lat_vpp2>59.810008</north_wgs_lat_vpp2>
	north_wgs_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getNorthWgsLatVPP2();
	sprintf(info, "north_wgs_lat_vpp2 =  %8.5f\n", north_wgs_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <north_wgs_lon_vpp2>30.245613</north_wgs_lon_vpp2>
	north_wgs_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getNorthWgsLonVPP2();
	sprintf(info, "north_wgs_lon_vpp2 =  %8.5f\n", north_wgs_lon_vpp2);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    // <south_wgs_lat_vpp2>59.800885</south_wgs_lat_vpp2>
	south_wgs_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getSouthWgsLatVPP2();
	sprintf(info, "south_wgs_lat_vpp2 =  %8.5f\n", south_wgs_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//---------------------------------
    // <south_wgs_lon_vpp2>30.303377</south_wgs_lon_vpp2>
	south_wgs_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getSouthWgsLonVPP2();
	sprintf(info, "south_wgs_lon_vpp2 =  %8.5f\n", south_wgs_lon_vpp2);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    //<west_wgs_lat_vpp2>59.809511</west_wgs_lat_vpp2>
	west_wgs_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getWestWgsLatVPP2();
	sprintf(info, "west_wgs_lat_vpp2 =  %8.5f\n", west_wgs_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    // <west_wgs_lon_vpp2>30.24527</west_wgs_lon_vpp2>
	west_wgs_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getWestWgsLonVPP2();
	sprintf(info, "west_wgs_lon_vpp2 =  %8.5f\n", west_wgs_lon_vpp2);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    // <east_wgs_lat_vpp2>59.801339</east_wgs_lat_vpp2>
	east_wgs_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getEastWgsLatVPP2();
	sprintf(info, "east_wgs_lat_vpp2 =  %8.5f\n", east_wgs_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <east_wgs_lon_vpp2>30.303635</east_wgs_lon_vpp2>    
	east_wgs_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getEastWgsLonVPP2();
	sprintf(info, "east_wgs_lon_vpp2 =  %8.5f\n", east_wgs_lon_vpp2);
	ern::Log::getInstance().Write(info);
}
//-------------------------------------------------
// тест №1: проверка загрузки настроечных данных приложения
void testLoadAppSetXML()
{
	char str_temp[80];
	int size_str_temp = 80;
	int fact_size_str_temp;
	char info[80];
	int port_main_client;
	int port_test_client_cons;
	int port_test_client_gui;
	char recording_mode;
	int table_count;
	int record_count;
	int run_service;

	// <airport_name_en>Pulkovo</airport_name_en>
	fact_size_str_temp = ern::ConfiguratorApp::getInstance().getAirportNameEn(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "airport_name_en =  %s\n", str_temp);	
	ern::Log::getInstance().Write(info);
	//-----------------------------------------------------
	// <port_main_client>0</port_main_client>
	port_main_client = ern::ConfiguratorApp::getInstance().getPortMainClient();
	sprintf(info, "port_main_client =  %d\n", port_main_client);
	ern::Log::getInstance().Write(info);
	//-----------------------------------------------------
	// <port_test_client_cons>54632</port_test_client_cons>
	port_test_client_cons = ern::ConfiguratorApp::getInstance().getPortTestClientCons();
	sprintf(info, "port_test_client_cons =  %d\n", port_test_client_cons);
	ern::Log::getInstance().Write(info);
	//----------------------------------------------------
	// <port_test_client_gui>54633</port_test_client_gui>
	port_test_client_gui = ern::ConfiguratorApp::getInstance().getPortTestClientGui();
	sprintf(info, "port_test_client_gui =  %d\n", port_test_client_gui);
	ern::Log::getInstance().Write(info);
	//----------------------------------------------------
	// <ip_main_client>192.168.2.209</ip_main_client>
	memset(str_temp, 0, 80);
	// size_str_temp = 0;
	fact_size_str_temp = ern::ConfiguratorApp::getInstance().getIpMainClient(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "ip_main_client =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);
	//------------------------------------------
	// <ip_test_client_cons>192.168.2.209</ip_test_client_cons>
	memset(str_temp, 0, 80);
	//size_str_temp = 0;
	fact_size_str_temp = ern::ConfiguratorApp::getInstance().getIpTestClientCons(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "ip_test_client_cons =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);
	//------------------------------------------
	// <ip_test_client_gui>192.168.2.209</ip_test_client_gui>
	memset(str_temp, 0, 80);
	// size_str_temp = 0;
	fact_size_str_temp = ern::ConfiguratorApp::getInstance().getIpTestClientGui(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "ip_test_client_gui =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);
	//------------------------------------------
	// <recording_mode>0</recording_mode>
	recording_mode = ern::ConfiguratorApp::getInstance().getRecordingMode();
	sprintf(info, "recording_mode =  %d\n", recording_mode);
	ern::Log::getInstance().Write(info);
	//------------------------------------------
	// <table_count>1</table_count>
	table_count = ern::ConfiguratorApp::getInstance().getTableCount();
	sprintf(info, "table_count =  %d\n", table_count);
	ern::Log::getInstance().Write(info);
	//-----------------------------------------
	// <record_count>1</record_count>
	record_count = ern::ConfiguratorApp::getInstance().getRecordCount();
	sprintf(info, "record_count =  %d\n", record_count);
	ern::Log::getInstance().Write(info);
	//----------------------------------------
	// <run_service>0</run_service>
	run_service = ern::ConfiguratorApp::getInstance().getRunService();
	sprintf(info, "run_service =  %d\n", run_service);
	ern::Log::getInstance().Write(info);
}
//------------------------------------------------
// определение правильности расчета координат вершин полигона
void testDetermCorrectnCalcCoordPolygonvertices()
{
	char str_temp[80];
	int size_str_temp = 80;
	int fact_size_str_temp;
	char info[80];
	double height_cta;
	// double lat_cta;
	// double lon_cta;
	double angle_vpp1;
	double runway_lat_vpp1;
	double runway_lon_vpp1;
	int lenght_vpp1;
	int width_vpp1;

	double north_wgs_lat_vpp1;
	double north_wgs_lon_vpp1;
	double south_wgs_lat_vpp1;
	double south_wgs_lon_vpp1;
	double west_wgs_lat_vpp1;
	double west_wgs_lon_vpp1;
	double east_wgs_lat_vpp1;
	double east_wgs_lon_vpp1;

	double angle_vpp2;
	double runway_lat_vpp2;
	double runway_lon_vpp2;
	int lenght_vpp2;
	int width_vpp2;

	double north_wgs_lat_vpp2;
	double north_wgs_lon_vpp2;
	double south_wgs_lat_vpp2;
	double south_wgs_lon_vpp2;
	double west_wgs_lat_vpp2;
	double west_wgs_lon_vpp2;
	double east_wgs_lat_vpp2;
	double east_wgs_lon_vpp2;

	//---------------------------------
	// расчетные показатели
	double northPointRunway1_lat;
	double northPointRunway1_lon;
	double southPointRunway1_lat;
	double southPointRunway1_lon;
	double westPointRunway1_lat;
	double westPointRunway1_lon;
	double eastPointRunway1_lat;
	double eastPointRunway1_lon;

	double northPointRunway2_lat;
	double northPointRunway2_lon;
	double southPointRunway2_lat;
	double southPointRunway2_lon;
	double westPointRunway2_lat;
	double westPointRunway2_lon;
	double eastPointRunway2_lat;
	double eastPointRunway2_lon;
	//----------------------------------------
	//  сравним настроечные данные приложения с расчетными
	ern::Log::getInstance().Write("Runway configuration data list\n");
	//---------------------------------------
    // <height_cta>24</height_cta>
	height_cta = ern::ConfiguratorAirports::getInstance().getHeightCTA();
	sprintf(info, "height_cta =  %10.4f\n", height_cta);
	ern::Log::getInstance().Write(info);
	//--------------------------------------
    // <lat_cta>59.800028</lat_cta>
	/*lat_cta = ern::ConfiguratorAirports::getInstance().getLatCTA();
	sprintf(info, "lat_cta =  %8.5f\n", lat_cta);
	ern::Log::getInstance().Write(info);*/
	//-------------------------------------
    // <lon_cta>30.2625</lon_cta>  
	/*lon_cta = ern::ConfiguratorAirports::getInstance().getLonCTA();
	sprintf(info, "lon_cta =  %8.5f\n", lon_cta);
	ern::Log::getInstance().Write(info);*/
	//------------------------------------
    // <caption_vpp1>ВПП-1</caption_vpp1>
	/*memset(str_temp, 0, 80);
	fact_size_str_temp = ern::ConfiguratorAirports::getInstance().getCaptionVPP1(str_temp, size_str_temp);
	str_temp[fact_size_str_temp] = '\0';
	sprintf(info, "caption_vpp1 =  %s\n", str_temp);
	ern::Log::getInstance().Write(info);*/
	//------------------------------------
    // <angle_vpp1>106.419</angle_vpp1>
	angle_vpp1 = ern::ConfiguratorAirports::getInstance().getAngleVPP1();
	sprintf(info, "angle_vpp1 =  %9.5f\n", angle_vpp1);
	ern::Log::getInstance().Write(info);
	//------------------------------------
    // <runway_lat_vpp1>59.795142</runway_lat_vpp1> 
	runway_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getRunwayLatVPP1();
	sprintf(info, "runway_lat_vpp1 =  %9.6f\n", runway_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//-----------------------------------
    // <runway_lon_vpp1>30.250639</runway_lon_vpp1>
	runway_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getRunwayLonVPP1();
	sprintf(info, "runway_lon_vpp1 =  %9.6f\n", runway_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//-----------------------------------
    // <lenght_vpp1>3782</lenght_vpp1>
	lenght_vpp1 = ern::ConfiguratorAirports::getInstance().getLenghtVPP1();
	sprintf(info, "lenght_vpp1 =  %d\n", lenght_vpp1);
	ern::Log::getInstance().Write(info);
	//----------------------------------
    // <width_vpp1>60</width_vpp1>
	width_vpp1 = ern::ConfiguratorAirports::getInstance().getWidthVPP1();
	sprintf(info, "width_vpp1 =  %d\n", width_vpp1);
	ern::Log::getInstance().Write(info);
	//----------------------------------
    // <north_wgs_lat_vpp1>59.800193</north_wgs_lat_vpp1>
	north_wgs_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getNorthWgsLatVPP1();
	sprintf(info, "north_wgs_lat_vpp1 =  %9.6f\n", north_wgs_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//---------------------------------
    // <north_wgs_lon_vpp1>30.218491</north_wgs_lon_vpp1>
	north_wgs_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getNorthWgsLonVPP1();
	sprintf(info, "north_wgs_lon_vpp1 =  %9.6f\n", north_wgs_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    // <south_wgs_lat_vpp1>59.790093</south_wgs_lat_vpp1>
	south_wgs_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getSouthWgsLatVPP1();
	sprintf(info, "south_wgs_lat_vpp1 =  %9.6f\n", south_wgs_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <south_wgs_lon_vpp1>30.282778</south_wgs_lon_vpp1>
	south_wgs_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getSouthWgsLonVPP1();
	sprintf(info, "south_wgs_lon_vpp1 =  %9.6f\n", south_wgs_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <west_wgs_lat_vpp1>59.799652</west_wgs_lat_vpp1>
	west_wgs_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getWestWgsLatVPP1();
	sprintf(info, "west_wgs_lat_vpp1 =  %9.6f\n", west_wgs_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//------------------------------
    // <west_wgs_lon_vpp1>30.218233</west_wgs_lon_vpp1>
	west_wgs_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getWestWgsLonVPP1();
	sprintf(info, "west_wgs_lon_vpp1 =  %9.6f\n", west_wgs_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//-----------------------------
    // <east_wgs_lat_vpp1>59.79059</east_wgs_lat_vpp1>
	east_wgs_lat_vpp1 = ern::ConfiguratorAirports::getInstance().getEastWgsLatVPP1();
	sprintf(info, "east_wgs_lat_vpp1 =  %9.6f\n", east_wgs_lat_vpp1);
	ern::Log::getInstance().Write(info);
	//----------------------------
    // <east_wgs_lon_vpp1>30.283035</east_wgs_lon_vpp1>    
	east_wgs_lon_vpp1 = ern::ConfiguratorAirports::getInstance().getEastWgsLonVPP1();
	sprintf(info, "east_wgs_lon_vpp1 =  %9.6f\n", east_wgs_lon_vpp1);
	ern::Log::getInstance().Write(info);
	//-----------------------------
    // <angle_vpp2>106.45</angle_vpp2>
	angle_vpp2 = ern::ConfiguratorAirports::getInstance().getAngleVPP2();
	sprintf(info, "angle_vpp2 =  %9.5f\n", angle_vpp2);
	ern::Log::getInstance().Write(info);
	//-----------------------------
    // <runway_lat_vpp2>59.805453</runway_lat_vpp2>
	runway_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getRunwayLatVPP2();
	sprintf(info, "runway_lat_vpp2 =  %9.6f\n", runway_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//----------------------------
    // <runway_lon_vpp2>30.274379</runway_lon_vpp2>
	runway_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getRunwayLonVPP2();
	sprintf(info, "runway_lon_vpp2 =  %9.6f\n", runway_lon_vpp2);
	ern::Log::getInstance().Write(info);
	//---------------------------
    // <lenght_vpp2>3393</lenght_vpp2>
	lenght_vpp2 = ern::ConfiguratorAirports::getInstance().getLenghtVPP2();
	sprintf(info, "lenght_vpp2 =  %d\n", lenght_vpp2);
	ern::Log::getInstance().Write(info);
	//----------------------------
    // <width_vpp2>60</width_vpp2>
	width_vpp2 = ern::ConfiguratorAirports::getInstance().getWidthVPP2();
	sprintf(info, "width_vpp2 =  %d\n", width_vpp2);
	ern::Log::getInstance().Write(info);	
	//-------------------------------
    // <north_wgs_lat_vpp2>59.810008</north_wgs_lat_vpp2>
	north_wgs_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getNorthWgsLatVPP2();
	sprintf(info, "north_wgs_lat_vpp2 =  %9.6f\n", north_wgs_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <north_wgs_lon_vpp2>30.245613</north_wgs_lon_vpp2>
	north_wgs_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getNorthWgsLonVPP2();
	sprintf(info, "north_wgs_lon_vpp2 =  %9.6f\n", north_wgs_lon_vpp2);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    // <south_wgs_lat_vpp2>59.800885</south_wgs_lat_vpp2>
	south_wgs_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getSouthWgsLatVPP2();
	sprintf(info, "south_wgs_lat_vpp2 =  %9.6f\n", south_wgs_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//---------------------------------
    // <south_wgs_lon_vpp2>30.303377</south_wgs_lon_vpp2>
	south_wgs_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getSouthWgsLonVPP2();
	sprintf(info, "south_wgs_lon_vpp2 =  %9.6f\n", south_wgs_lon_vpp2);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    //<west_wgs_lat_vpp2>59.809511</west_wgs_lat_vpp2>
	west_wgs_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getWestWgsLatVPP2();
	sprintf(info, "west_wgs_lat_vpp2 =  %9.6f\n", west_wgs_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    // <west_wgs_lon_vpp2>30.24527</west_wgs_lon_vpp2>
	west_wgs_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getWestWgsLonVPP2();
	sprintf(info, "west_wgs_lon_vpp2 =  %9.6f\n", west_wgs_lon_vpp2);
	ern::Log::getInstance().Write(info);
	//--------------------------------
    // <east_wgs_lat_vpp2>59.801339</east_wgs_lat_vpp2>
	east_wgs_lat_vpp2 = ern::ConfiguratorAirports::getInstance().getEastWgsLatVPP2();
	sprintf(info, "east_wgs_lat_vpp2 =  %9.6f\n", east_wgs_lat_vpp2);
	ern::Log::getInstance().Write(info);
	//-------------------------------
    // <east_wgs_lon_vpp2>30.303635</east_wgs_lon_vpp2>    
	east_wgs_lon_vpp2 = ern::ConfiguratorAirports::getInstance().getEastWgsLonVPP2();
	sprintf(info, "east_wgs_lon_vpp2 =  %9.6f\n", east_wgs_lon_vpp2);
	ern::Log::getInstance().Write(info);	

	// расчетные показатели
	ern::Log::getInstance().Write("Calculated indicators\n");
	// создадим два экземпляра класса ВПП для аэродрома "Пулково" (впп1 и впп2) и вывдем в лог крайние точки в системе WGS84
	Runway* ptrRunway_1;							// впп 1
	Runway* ptrRunway_2;							// впп 2

	ptrRunway_1 = new Runway(ern::ConfiguratorAirports::getInstance().getIdVpp1(), ern::ConfiguratorAirports::getInstance().getHeightCTA() );
	ptrRunway_2 = new Runway(ern::ConfiguratorAirports::getInstance().getIdVPP2(), ern::ConfiguratorAirports::getInstance().getHeightCTA() );	
	//------------vpp1---------------
	northPointRunway1_lat = ptrRunway_1->getPtrNorthPoint()->getWgs84_Lat();
	sprintf(info, "northPointRunway1_lat =  %9.6f\n", northPointRunway1_lat);
	ern::Log::getInstance().Write(info);
	//--------------------------
	northPointRunway1_lon = ptrRunway_1->getPtrNorthPoint()->getWgs84_Lon();
	sprintf(info, "northPointRunway1_lon =  %9.6f\n", northPointRunway1_lon);
	ern::Log::getInstance().Write(info);
	//--------------------------
	southPointRunway1_lat = ptrRunway_1->getPtrSouthPoint()->getWgs84_Lat();
	sprintf(info, "southPointRunway1_lat =  %9.6f\n", southPointRunway1_lat);
	ern::Log::getInstance().Write(info);
	//--------------------------
	southPointRunway1_lon = ptrRunway_1->getPtrSouthPoint()->getWgs84_Lon();
	sprintf(info, "southPointRunway1_lon =  %9.6f\n", southPointRunway1_lon);
	ern::Log::getInstance().Write(info);
	//--------------------------
	westPointRunway1_lat = ptrRunway_1->getPtrWestPoint()->getWgs84_Lat();
	sprintf(info, "westPointRunway1_lat =  %9.6f\n", westPointRunway1_lat);
	ern::Log::getInstance().Write(info);
	//--------------------------
	westPointRunway1_lon = ptrRunway_1->getPtrWestPoint()->getWgs84_Lon();
	sprintf(info, "westPointRunway1_lon =  %9.6f\n", westPointRunway1_lon);
	ern::Log::getInstance().Write(info);
	//-------------------------
	eastPointRunway1_lat = ptrRunway_1->getPtrEastPoint()->getWgs84_Lat();
	sprintf(info, "eastPointRunway1_lat =  %9.6f\n", eastPointRunway1_lat);
	ern::Log::getInstance().Write(info);
	//-------------------------
	eastPointRunway1_lon = ptrRunway_1->getPtrEastPoint()->getWgs84_Lon();
	sprintf(info, "eastPointRunway1_lon =  %9.6f\n", eastPointRunway1_lon);
	ern::Log::getInstance().Write(info);
	//----------vpp2--------------
	northPointRunway2_lat = ptrRunway_2->getPtrNorthPoint()->getWgs84_Lat();
	sprintf(info, "northPointRunway2_lat =  %9.6f\n", northPointRunway2_lat);
	ern::Log::getInstance().Write(info);
	//--------------------------
	northPointRunway2_lon = ptrRunway_2->getPtrNorthPoint()->getWgs84_Lon();
	sprintf(info, "northPointRunway2_lon =  %9.6f\n", northPointRunway2_lon);
	ern::Log::getInstance().Write(info);
	//--------------------------
	southPointRunway2_lat = ptrRunway_2->getPtrSouthPoint()->getWgs84_Lat();
	sprintf(info, "southPointRunway2_lat =  %9.6f\n", southPointRunway2_lat);
	ern::Log::getInstance().Write(info);
	//--------------------------
	southPointRunway2_lon = ptrRunway_2->getPtrSouthPoint()->getWgs84_Lon();
	sprintf(info, "southPointRunway2_lon =  %9.6f\n", southPointRunway2_lon);
	ern::Log::getInstance().Write(info);
	//--------------------------
	westPointRunway2_lat = ptrRunway_2->getPtrWestPoint()->getWgs84_Lat();
	sprintf(info, "westPointRunway2_lat =  %9.6f\n", westPointRunway2_lat);
	ern::Log::getInstance().Write(info);
	//--------------------------
	westPointRunway2_lon = ptrRunway_2->getPtrWestPoint()->getWgs84_Lon();
	sprintf(info, "westPointRunway2_lon =  %9.6f\n", westPointRunway2_lon);
	ern::Log::getInstance().Write(info);
	//-------------------------
	eastPointRunway2_lat = ptrRunway_2->getPtrEastPoint()->getWgs84_Lat();
	sprintf(info, "eastPointRunway2_lat =  %9.6f\n", eastPointRunway2_lat);
	ern::Log::getInstance().Write(info);
	//-------------------------
	eastPointRunway2_lon = ptrRunway_2->getPtrEastPoint()->getWgs84_Lon();
	sprintf(info, "eastPointRunway2_lon =  %9.6f\n", eastPointRunway2_lon);
	ern::Log::getInstance().Write(info);
	//------------------------
	delete ptrRunway_1; 
	delete ptrRunway_2;
}
// atoi(argv[1]) порт на серверной части (т.е. на сервисе обработки и анализе конфликтных ситуаций)
//-------------------------------------------
/*
int main(int argc, char *argv[]) {
	
	////////////////////////////////////////////////////
	int err = 0;
	printf("Start service \"Analyzer conflict\"!\n");
	ern::Log::getInstance().Write("Start service \"Analyzer conflict\"!\n");

	ern::ConfiguratorApp::getInstance().LoadXML(conf_file_name);
	ern::ConfiguratorAirports::getInstance().LoadXML(conf_data_airodrome_name);

	// testLoadAppSetXML();		// проверка правильности загрузки насроечных данных
	// testLoadAirportsSetXML();	// проверка правильности загрузки насроечных данных аэродромов
	// testDetermCorrectnCalcCoordPolygonvertices();		// определение правильности расчета координат вершин полигона

	ern::ConfiguratorApp::getInstance().setRunService(1);
	ern::ConfiguratorApp::getInstance().UpdateXML(conf_file_name);

	if(StartWinsock())
	{
		AsterixWorker* ptrAsterixWorker = new AsterixWorker();
		if(server(ptrAsterixWorker)!=1)
		{
			delete ptrAsterixWorker;
			return 1;
		}
		delete ptrAsterixWorker;
	}	
	else
		err = 1;

	
	return err;	
}
*/
//------------------------------------------------


