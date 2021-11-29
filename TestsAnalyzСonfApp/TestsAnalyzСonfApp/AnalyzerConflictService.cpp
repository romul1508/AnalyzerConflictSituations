// TestsAnalyzСonfApp implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 17.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

// The TestsAnalyzConfApp program is a console technology software. 
// TestsAnalyzСonfApp is designed to reproduce 
// a recording (from a text CSV file) of a changing target 
// situation (aircraft route) on the territory of an aerodrome.
// The main task of TestsAnalyzСonfApp is to develop a recognition 
// scenario - whether the plane is on the runway or not.
// TestsAnalyzConfApp helps to implement some of the functions 
// of the A-SMGCS (Advanced Surface Movement Guidance and Control System).  
// To a large extent, the program is intended for educational purposes. 
// TestsAnalyzСonfApp is implemented in the C ++ programming language 
// in the development environment - Microsoft Visual Studio 2005. 
// The program functions in Windows 7/8/10 (x86-64).


// Программа TestsAnalyzСonfApp является консольным технологическим
// программным обеспечением. TestsAnalyzСonfApp предназначена для
// воспроизведения записи (из текстового CSV-файла) изменяющейся
// целевой обстановки (трассы движения воздушных судов)на территории
// аэродрома. Основной задачей TestsAnalyzСonfApp является отработка 
// распознавательного сценария - самолет находится на взлетно-посадочной
// полосе или нет.
// TestsAnalyzСonfApp реализует некоторые функции A-SMGCS 
// (Advanced Surface Movement Guidance and Control System).  
// В большей степени программа предназначена для учебных целей. 
// TestsAnalyzСonfApp реализована на языке программирования  C++ 
// в среде разработки — Microsoft Visual Studio 2005. 
// Программа функционирует в линейке ОС Windows 7-10 (x86-64).


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

ern::LoggerDestroyer ern::Log::destroyer;
ern::ConfiguratorAppDestroyer ern::ConfiguratorApp::conf_app_destroyer;
ern::ConfiguratorAirportsDestroyer ern::ConfiguratorAirports::conf_airports_destroyer;

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

	//TTargetLocationInMinutes targetLocationInMinutes;
	//TTargetLocationInMinutes *pTargetLocationInMinutes;
	//pTargetLocationInMinutes = &targetLocationInMinutes;
	////// memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	//// точка 0 - левому верхнему углу контролируемой территории
	//// зададим широту и долготу нулевой точки в десятичных дробях в системе WGS-84
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 48;
	//pTargetLocationInMinutes->seconds = 39.59;
	//Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 12;
	//pTargetLocationInMinutes->seconds = 44.24;
	//Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	////////------------------------------------------------------------
	
	// результаты варианта 9
	//  LatP3_90_02_0 =  59.81097394
	//  LonP3_90_02_0 =  30.21199235

	//  deg_lat0 = 59;  min_x_lat0 = 48;	sec_x_lat0 = 39.506192;
	//  deg_lon0 = 30;  min_x_lon0 = 12;	sec_x_lon0 = 43.172455;
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
		runway_lon_vpp = ern::ConfiguratorAirports::getInstance().getRunwayLonVPP1();
		lenght_vpp = ern::ConfiguratorAirports::getInstance().getLenghtVPP1();
		lenght_vpp = lenght_vpp * 1.0036277;
		width_vpp = ern::ConfiguratorAirports::getInstance().getWidthVPP1();
		north_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getNorthWgsLatVPP1();
		north_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getNorthWgsLonVPP1();
		south_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getSouthWgsLatVPP1();
		south_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getSouthWgsLonVPP1();
		west_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getWestWgsLatVPP1();
		west_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getWestWgsLonVPP1();
		east_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getEastWgsLatVPP1();
		east_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getEastWgsLonVPP1();
		// следующие пять строчек для отладки
		// ptrCentralPoint = new Point(-552.3, -615.7, height_cta, 0, CPA);
		// ptrNorthPoint = new Point(north_y, north_x, ptrCentralPoint->getHeight(), 0, CPA);
		///////////////
		/*ptrNorthPoint = new Point(-1171, 320, height_cta, 0, CPA);
		ptrSouthPoint = new Point(-2292, 3944.5, height_cta, 0, CPA);
		ptrWesternPoint = new Point(-1228.5, 300, height_cta, 0, CPA);
		ptrEasternPoint = new Point(-2234.5, 3984.5, height_cta, 0, CPA);*/
	}else if(id_vpp==1){	// это означает, что ВПП №2
		ern::ConfiguratorAirports::getInstance().getCaptionVPP2(cap_vpp, BUFF_CAPTION_VPP);
		cat_vpp = ern::ConfiguratorAirports::getInstance().getCatVPP2();
		angle_vpp = ern::ConfiguratorAirports::getInstance().getAngleVPP2();
		runway_lat_vpp = ern::ConfiguratorAirports::getInstance().getRunwayLatVPP2();
		runway_lon_vpp = ern::ConfiguratorAirports::getInstance().getRunwayLonVPP2();
		lenght_vpp = ern::ConfiguratorAirports::getInstance().getLenghtVPP2();
		lenght_vpp = lenght_vpp * 1.00276646;
		width_vpp = ern::ConfiguratorAirports::getInstance().getWidthVPP2();
		north_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getNorthWgsLatVPP2();
		north_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getNorthWgsLonVPP2();
		south_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getSouthWgsLatVPP2();
		south_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getSouthWgsLonVPP2();
		west_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getWestWgsLatVPP2();
		west_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getWestWgsLonVPP2();
		east_wgs_lat_vpp = ern::ConfiguratorAirports::getInstance().getEastWgsLatVPP2();
		east_wgs_lon_vpp = ern::ConfiguratorAirports::getInstance().getEastWgsLonVPP2();
		// следующие пять строчек для отладки
		//ptrCentralPoint = new Point(621.7, 626.8, height_cta, 0, CPA);
		//ptrNorthPoint = new Point(-74, 1860, height_cta, 0, CPA);
		//ptrSouthPoint = new Point(-1098.5, 5108, height_cta, 0, CPA);
		//// ptrWesternPoint = new Point(-112.8, 1842, height_cta, 0, CPA);
		//ptrWesternPoint = new Point(-136.8, 1842, height_cta, 0, CPA);
		//ptrEasternPoint = new Point(1041, 5128, height_cta, 0, CPA);
	}else
	{
		ern::Log::getInstance().Write("The number of runways is more than two");
		return;
	}
	caption_vpp = cap_vpp;
	isRunwayEmploymentStatus_ = false;

	// теперь можно задать центральную точку и крайние точки ВПП, которые ее определяют  
	ptrCentralPoint = new Point(runway_lat_vpp, runway_lon_vpp, height_cta, 0, WGS_84);
	/*ptrNorthPoint = new Point(north_wgs_lat_vpp, north_wgs_lon_vpp, height_cta, 0, WGS_84);
	ptrSouthPoint = new Point(south_wgs_lat_vpp, south_wgs_lon_vpp, height_cta, 0, WGS_84);
	ptrWesternPoint = new Point(west_wgs_lat_vpp, west_wgs_lon_vpp, height_cta, 0, WGS_84);
	ptrEasternPoint = new Point(east_wgs_lat_vpp, east_wgs_lon_vpp, height_cta, 0, WGS_84);*/
	// после задания точек, определяющих ВПП, неплохо было бы организовать проверку.......(потом нужно будет реализовать!!!) 
		
	// вычисление координат точек ptrNorthPoint, ptrSouthPoint, ptrWesternPoint, ptrEasternPoint  в системе WGS84 относительно референсной точки приложения
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
		// if(ptrTargetPoint->getSK42_Lon() > ptrWesternPoint->getSK42_Lon() && ptrTargetPoint->getSK42_Lon() < ptrEasternPoint->getSK42_Lon() ) // если попадает в диапазон по шкале X
		if(ptrTargetPoint->getCPA_x() > ptrWesternPoint->getCPA_x() && ptrTargetPoint->getCPA_x() < ptrEasternPoint->getCPA_x() ) // если попадает в диапазон по шкале X
		{
			// if(ptrNorthPoint->getSK42_Lon() < ptrSouthPoint->getSK42_Lon() ) // если координата X северной точки меньше координаты X южной точки
			if(ptrNorthPoint->getCPA_x() < ptrSouthPoint->getCPA_x() ) // если координата X северной точки меньше координаты X южной точки
			{
				 // if(ptrTargetPoint->getSK42_Lon() > ptrWesternPoint->getSK42_Lon() && ptrTargetPoint->getSK42_Lon() < ptrNorthPoint->getSK42_Lon() )
				 if(ptrTargetPoint->getCPA_x() > ptrWesternPoint->getCPA_x() && ptrTargetPoint->getCPA_x() < ptrNorthPoint->getCPA_x() )
				 {
					// координата цели в интервале [W, N]
					 if( ptrWN->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) > ptrTargetPoint->getCPA_y() && ptrSW->getIntersectionCPA_Y(ptrTargetPoint->getCPA_x()) < ptrTargetPoint->getCPA_y() )
						runway_employment_status = true;		// если между значениями по Y сторон WN и SW то цель находится на ВПП (ВПП занята)
				 }
				 // else if(ptrTargetPoint->getSK42_Lon() > ptrNorthPoint->getSK42_Lon() && ptrTargetPoint->getSK42_Lon() < ptrSouthPoint->getSK42_Lon() )
				 else if(ptrTargetPoint->getCPA_x() > ptrNorthPoint->getCPA_x() && ptrTargetPoint->getCPA_x() < ptrSouthPoint->getCPA_x() )
				 {
					// координата цели в интервале [N, S]
					// if( ptrNE->getIntersectionSK42_Y(ptrTargetPoint->getSK42_Lon()) > ptrTargetPoint->getSK42_Lat() && ptrSW->getIntersectionSK42_Y(ptrTargetPoint->getSK42_Lon()) < ptrTargetPoint->getSK42_Lat() )
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
	// double dX, X2, X3, dY, Y2, Y3;
	double L, W, R, B, north_x, north_y, south_x, south_y, east_x, east_y, west_x, west_y;
	double d_north_x, d_north_y, d_south_x, d_south_y, d_east_x, d_east_y, d_west_x, d_west_y;
	double Pi = 3.14159265358979;		// число Пи
	double angle_rad = angle_vpp * Pi / 180;
	
	L = 0.5 * lenght_vpp;
	W = 0.5 * width_vpp + 0.8;		// + 1.9;		// + 20;
	R = sqrt(L*L + W*W);
	B = atan(W/L);

	if(angle_vpp > 0 && angle_vpp < 90)
	{
		//dX = L * sin(angle_vpp);
		//X2 = ptrCentralPoint->getWgs84_Lon() + dX;
		//X3 = ptrCentralPoint->getWgs84_Lon() - dX;
		////-------------------
		//dY = L * cos(angle_vpp);
		//Y2 = ptrCentralPoint->getWgs84_Lat() + dY;
		//Y3 = ptrCentralPoint->getWgs84_Lat() - dY;
		//------------------
		d_north_x = R * sin(angle_rad - B);
		// north_x = ptrCentralPoint->getWgs84_Lon() + d_north_x;
		north_x = ptrCentralPoint->getCPA_x() + d_north_x;
		// south_x = ptrCentralPoint->getWgs84_Lon() - d_north_x;
		south_x = ptrCentralPoint->getCPA_x() - d_north_x;
		//-------------------
		d_north_y = R * cos(angle_rad - B);
		// north_y = ptrCentralPoint->getWgs84_Lat() + d_north_y;
		north_y = ptrCentralPoint->getCPA_y() + d_north_y;
		// south_y = ptrCentralPoint->getWgs84_Lat() - d_north_y;
		south_y = ptrCentralPoint->getCPA_y() - d_north_y;
		//------------------
		d_east_x = R * sin(angle_rad + B);
		// east_x = ptrCentralPoint->getWgs84_Lon() + d_east_x;
		east_x = ptrCentralPoint->getCPA_x() + d_east_x;
		// west_x = ptrCentralPoint->getWgs84_Lon() - d_east_x;
		west_x = ptrCentralPoint->getCPA_x() - d_east_x;
		//-----------------
		d_east_y = R * cos(angle_rad + B);
		// east_y = ptrCentralPoint->getWgs84_Lat() + d_east_y;
		east_y = ptrCentralPoint->getCPA_y() + d_east_y;
		// west_y = ptrCentralPoint->getWgs84_Lat() - d_east_y;
		west_y = ptrCentralPoint->getCPA_y() - d_east_y;
	}
	else if(angle_vpp > 90 && angle_vpp < 180)
	{
		// dX = L * cos(angle_vpp - 90);
		// X2 = ptrCentralPoint->getWgs84_Lon() + dX;
		// X3 = ptrCentralPoint->getWgs84_Lon() - dX;
		////-------------------
		// dY = L * sin(angle_vpp - 90);
		// Y2 = ptrCentralPoint->getWgs84_Lat() - dY;
		// Y3 = ptrCentralPoint->getWgs84_Lat() + dY;
		//--------------------
		d_north_x = R * cos(angle_rad - Pi/2 + B);
		// north_x = ptrCentralPoint->getWgs84_Lon() - d_north_x;
		north_x = ptrCentralPoint->getCPA_x() - d_north_x;
		// south_x = ptrCentralPoint->getWgs84_Lon() + d_north_x;
		south_x = ptrCentralPoint->getCPA_x() + d_north_x;
		//-------------------
		d_north_y = R * sin(angle_rad - Pi/2 + B);
		// north_y = ptrCentralPoint->getWgs84_Lat() + d_north_y;
		north_y = ptrCentralPoint->getCPA_y() + d_north_y;
		// south_y = ptrCentralPoint->getWgs84_Lat() - d_north_y;
		south_y = ptrCentralPoint->getCPA_y() - d_north_y;
		//-------------------
		d_east_x = R * cos(angle_rad - Pi/2 - B);
		// east_x = ptrCentralPoint->getWgs84_Lon() + d_east_x;
		east_x = ptrCentralPoint->getCPA_x() + d_east_x;
		// west_x = ptrCentralPoint->getWgs84_Lon() - d_east_x;
		west_x = ptrCentralPoint->getCPA_x() - d_east_x;
		//-------------------
		d_east_y = R * sin(angle_rad - Pi/2 - B);
		// east_y = ptrCentralPoint->getWgs84_Lat() - d_east_y;
		east_y = ptrCentralPoint->getCPA_y() - d_east_y;
		// west_y = ptrCentralPoint->getWgs84_Lat() + d_east_y;
		west_y = ptrCentralPoint->getCPA_y() + d_east_y;
	}
	else
	{
		ern::Log::getInstance().Write("Cases with an angle of 180 degrees to 360 degrees are not implemented");
		return;
	}

	// ptrNorthPoint = new Point(north_y, north_x, ptrCentralPoint->getHeight(), WGS_84);
	ptrNorthPoint = new Point(north_y, north_x, ptrCentralPoint->getHeight(), 0, CPA);
	// ptrSouthPoint = new Point(south_y, south_x, ptrCentralPoint->getHeight(), WGS_84);
	ptrSouthPoint = new Point(south_y, south_x, ptrCentralPoint->getHeight(), 0, CPA);
	// ptrWesternPoint = new Point(west_y, west_x, ptrCentralPoint->getHeight(), WGS_84);
	ptrWesternPoint = new Point(west_y, west_x, ptrCentralPoint->getHeight(), 0, CPA);
	// ptrEasternPoint = new Point(east_y, east_x, ptrCentralPoint->getHeight(), WGS_84);
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
	// ptrAirRefPoint = new Point(ern::ConfiguratorAirports::getInstance().getLatCTA(), ern::ConfiguratorAirports::getInstance().getLonCTA(), ern::ConfiguratorAirports::getInstance().getHeightCTA(), 0, WGS_84);
	ptrAirRefPoint = new Point(MyLAT, MyLON, ern::ConfiguratorAirports::getInstance().getHeightCTA(), 0, WGS_84);
	ptrRunway_1 = new Runway(ern::ConfiguratorAirports::getInstance().getIdVpp1(), ern::ConfiguratorAirports::getInstance().getHeightCTA() );
	ptrRunway_2 = new Runway(ern::ConfiguratorAirports::getInstance().getIdVPP2(), ern::ConfiguratorAirports::getInstance().getHeightCTA() );	
}
//--------------------------------------------------------------------------
//bool operator < (const RecCategory_11& record_1, const RecCategory_11& record_2)
//{
//	return record_1.time_of_track < record_2.time_of_track;
//}
////-------------------------------------------------
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
	
	/*bool isRecord;
	isRecord = false;*/
	
	if( find(vectCat11Records.begin(), vectCat11Records.end(), rec_11) == vectCat11Records.end() )
	{
		vectCat11Records.push_back(rec_11);
		// sort(vectCat11Records.begin(), vectCat11Records.end());
		// isRecord = true;
	}
	
	// vectCat11Records.push_back(rec_11);
	// return isRecord;
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
	// WORD size = setPtrsOperation.size() + 1;
	// oper->setNumberOperation(size);

	// cout << "number of the operation " << size << endl;
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
// возвращает следующую трассу согласно последовательности итераторов
Track* TrackList::getNextTrack()
{
	iter++;
	if(iter != setPtrsTrack.end())
		return (*iter);
	else 
		return 0;
}
//-------------------------------------------------
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
		if( actual_time - (*iter)->getCat11Record()->getCat11Record(count_records).time_of_track > 15){		// указывает на последний элемент списка записей, если настоящ. время больше на 15 сек
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
// пишет полученное АСТЕРИКС-сообщение в файл с расширением csv
//int AsterixWorker::writeCSV() const
//{
//	// int buff_len, num_simb, n_simb, i, j, x;
//
//	double vpp;
//	double pvo;
//	double po;
//	double tppp;
//	double ptv;
//	double vp;
//	double rp;
//	double gzs;
//
//	//double complAss;
//	//char ch;
//	//char str[140];
//	//char msgCvsText[MAX_CVC_TEXT];
//	//std::string str_msg_text;
//
//	//std::string cvs_string;
//
//	//std::vector<std::string> delim_n;
//	//std::vector<std::string>::iterator iter;
//
//	//// Step 1. Выгрузка всего содержимого из входного cvs-файла
//	//std::ifstream infile( input_file_name.c_str() );
//	//std::ofstream outfile( output_file_name.c_str() );
//
//	//buff_len = 0;
//	//while(infile)
//	//{
//	//	infile.get(ch);
//	//	msgCvsText[buff_len] = ch;
//	//	buff_len++;
//	//}
//
//	//msgCvsText[buff_len] = '\0';
//
//	//str_msg_text = msgCvsText;
//
//	// cout << "step 1: msgCsvText = " << str_msg_text << endl;
//
//	// Step 2. 
//	// cout << "step 2: " << endl;
//
//	while(str_msg_text.size() > 0)
//	{
//		std::string min_str_temp;
//		char strTemp[MAX];
//
//		num_simb = str_msg_text.find_first_of("\n");
//
//		// cout << "step 2: num_simb = " << num_simb << endl;
//
//		if(num_simb == -1)
//			break;
//
//		str_msg_text.copy(strTemp, num_simb+1, 0);
//		strTemp[num_simb+1] = 0;
//
//		min_str_temp = strTemp;
//
//		if(min_str_temp == "" || min_str_temp == " " || min_str_temp == "  " || min_str_temp == "   " || min_str_temp == "    ")
//			break;
//
//		// cout << "step 2: min_str_temp = " << min_str_temp << endl;
//
//		while(min_str_temp.size() > 0)
//		{
//			std::string token;
//			n_simb = min_str_temp.find_first_of(";\r\n");
//
//			// cout << "step 3: n_simb = " << n_simb << endl;
//			if(n_simb == -1)
//				break;
//
//			min_str_temp.copy(strTemp, n_simb, 0);
//		    strTemp[n_simb] = 0;
//
//			token = strTemp;
//			// cout << "step 3: token = " << token << endl;
//
//			delim_n.push_back(token);
//			min_str_temp.erase(0, n_simb+1);
//		}
//
//		if(delim_n.size() < 9)
//		{
//			while(!delim_n.empty()){
//				iter = delim_n.begin();		
//				delim_n.erase(iter);
//			}		
//			break;
//		}
//
//		// перед расчетами произведем замену запятых на точки в токенах
//		vpp = strToDouble(delim_n[1]);
//		pvo = strToDouble(delim_n[2]);
//		po = strToDouble(delim_n[3]);
//		tppp = strToDouble(delim_n[4]);
//		ptv = strToDouble(delim_n[5]);
//		vp = strToDouble(delim_n[6]);
//		rp = strToDouble(delim_n[7]);
//		gzs = strToDouble(delim_n[8]);
//		
//		complAss = calcComplAssConditions(vpp, pvo, po, tppp, ptv, vp, rp, gzs);
//		//-----------------------------------
//		cvs_string = delim_n[0] + ";" + delim_n[1] + ";" + delim_n[2] + ";" + delim_n[3] + ";" +
//			delim_n[4] + ";" + delim_n[5] + ";" + delim_n[6] + ";" + delim_n[7] + ";" + delim_n[8] + ";";
//		sprintf(str, "%s%4.4f;\n", cvs_string.c_str(), complAss);
//		cvs_string = str;
//		x = cvs_string.find('.');
//		if(x != -1)
//		  	cvs_string.replace(x, 1, ",");
//
//	    outfile << cvs_string;
//		//-------------------------------------
//		while(!delim_n.empty()){
//			iter = delim_n.begin();		
//			delim_n.erase(iter);
//		}
//
//		str_msg_text.erase(0, num_simb+1);
//	}
//}
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
	// out_csv_file.open(name_file, std::ofstream::out | std::ofstream::app);
	out_csv_file.open(name_file, std::ios::app | std::ios::out );
	// ptrOutCSVFile = &out_csv_file;
	
	if(record_count > MAX_NUMBER_ENTRIES_CSV)
	{
		record_count = 1;
		ern::ConfiguratorApp::getInstance().setRecordCount(record_count);

		table_count++;
		ern::ConfiguratorApp::getInstance().setTableCount(table_count);
		// out_csv_file.close();

		/*name_csv_file = csv_file_cat11 + table_count;
		out_csv_file.open(name_csv_file, std::ofstream::out | std::ofstream::app);*/
		std::string cvs_string = "msg_type;SAC;SIC;ServiceID;wgs84_lat;wgs84_lon;X;Y;Mode3A;flight_level;geo_altitude;QNH;baro_altitude;time_of_track;track_number;";
		cvs_string += "MON;GBS;MRH;SRC;CNF;SIM;TSE;TSB;FRI/FOE;ME;MI;AMA;SPI;CST;FPC;AFF;PSR;SSR;MDS;ADS;SUC;AAC;VX;VY;AX;AY;RATE;STI;CALLSIGN;LENGTH;WIDTH;ORIENTATION;";
		cvs_string += "PSR_Ages;SSR_Ages;MDA;MFL;MDS_Ages;ADS_Ages;ADB;MD1;MD2;LOP;TRK;MUL;VFI;TRB;MSG;REP;MB_DATA;ADDRESS;COM;STAS;SSC;ARC;AIC;B1A;B1B;AC;VVP1;VVP2\n";
		out_csv_file << cvs_string;
	}
	
	
	// str_csv
	// cvs_string = cat_11.msg_type;
	sprintf(str_csv, "%d", cat_11.msg_type);
	csv_string = str_csv;
	csv_string += ";";
	//-----------------------------
	// cvs_string += cat_11.dsi.sac;
	sprintf(str_csv, "%d;", cat_11.dsi.sac);
	csv_string += str_csv;
	// csv_string += ";";
	// cvs_string += cat_11.dsi.sic;
	sprintf(str_csv, "%d;", cat_11.dsi.sic);
	csv_string += str_csv;
	//----------------------------
	// cvs_string += cat_11.service_id;
	sprintf(str_csv, "%d;", cat_11.service_id);
	csv_string += str_csv;
	//----------------------------
	//	cvs_string += cat_11.pos_wgs84.wgs84_lat;
	sprintf(str_csv, "%8.6f;", cat_11.pos_wgs84.wgs84_lat);
	csv_string += str_csv;
	// cvs_string += cat_11.pos_wgs84.wgs84_lon;
	sprintf(str_csv, "%8.6f;", cat_11.pos_wgs84.wgs84_lon);
	csv_string += str_csv;
	//---------------------------
	// cvs_string += cat_11.pos_cart.x;
	sprintf(str_csv, "%8.4f;", cat_11.pos_cart.x * 0.99795);
	csv_string += str_csv;
	// cvs_string += cat_11.pos_cart.y;
	sprintf(str_csv, "%8.4f;", cat_11.pos_cart.y * 1.041856); //  * 1.13);
	csv_string += str_csv;
	//--------------------------
	// cvs_string += cat_11.mode_3A;
	sprintf(str_csv, "%d;", cat_11.mode_3A);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11.flight_level;
	sprintf(str_csv, "%8.4f;", cat_11.flight_level);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11.geo_altitude;
	sprintf(str_csv, "%8.4f;", cat_11.geo_altitude);
	csv_string += str_csv;
	//-------------------------
	if(cat_11.baro_alt.qnh)
		// cvs_string += cat_11.baro_alt.qnh;
		csv_string += "true;";	
	else	
		csv_string += "false;";
	//-------------------------
	// cvs_string += cat_11.baro_alt.altitude;
	sprintf(str_csv, "%8.4f", cat_11.baro_alt.altitude);
	csv_string += str_csv;
	csv_string += ";";
	//-------------------------
	// cvs_string += cat_11.time_of_track;
	sprintf(str_csv, "%8.4f;", cat_11.time_of_track);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11.track_number;
	sprintf(str_csv, "%d;", cat_11.track_number);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11.tr_status.monosensor;
	sprintf(str_csv, "%d;", cat_11.tr_status.monosensor);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.ground_bit_set;
	sprintf(str_csv, "%d;", cat_11.tr_status.ground_bit_set);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.most_reliable_height;	
	sprintf(str_csv, "%d;", cat_11.tr_status.most_reliable_height);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.geometric_altitude_source;	
	sprintf(str_csv, "%d;", cat_11.tr_status.geometric_altitude_source);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.confirm;
	sprintf(str_csv, "%d;", cat_11.tr_status.confirm);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.simulate;	
	sprintf(str_csv, "%d;", cat_11.tr_status.simulate);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.last_message;	
	sprintf(str_csv, "%d;", cat_11.tr_status.last_message);
	csv_string += str_csv;
	//	cvs_string += cat_11.tr_status.first_message;	
	sprintf(str_csv, "%d;", cat_11.tr_status.first_message);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.mode4_interrogate;	
	sprintf(str_csv, "%d;", cat_11.tr_status.mode4_interrogate);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.military_emrg;	
	sprintf(str_csv, "%d;", cat_11.tr_status.military_emrg);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.military_id;	
	sprintf(str_csv, "%d;", cat_11.tr_status.military_id);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.amalgamation;	
	sprintf(str_csv, "%d;", cat_11.tr_status.amalgamation);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.spi_sign;	
	sprintf(str_csv, "%d;", cat_11.tr_status.spi_sign);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.coast_track;	
	sprintf(str_csv, "%d;", cat_11.tr_status.coast_track);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.flight_in_plan;	
	sprintf(str_csv, "%d;", cat_11.tr_status.flight_in_plan);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.ads_inconsist;	
	sprintf(str_csv, "%d;", cat_11.tr_status.ads_inconsist);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.psr_coast;	
	sprintf(str_csv, "%d;", cat_11.tr_status.psr_coast);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.ssr_coast;	
	sprintf(str_csv, "%d;", cat_11.tr_status.ssr_coast);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.mode_s_coast;	
	sprintf(str_csv, "%d;", cat_11.tr_status.mode_s_coast);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.ads_coast;	
	sprintf(str_csv, "%d;", cat_11.tr_status.ads_coast);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.suc_set;	
	sprintf(str_csv, "%d;", cat_11.tr_status.suc_set);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_status.aac_conflict;	
	sprintf(str_csv, "%d;", cat_11.tr_status.aac_conflict);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11.tr_velocity.vx;	
	sprintf(str_csv, "%8.4f;", cat_11.tr_velocity.vx);
	csv_string += str_csv;
	// cvs_string += cat_11.tr_velocity.vy;	cvs_string += ";";
	sprintf(str_csv, "%8.4f;", cat_11.tr_velocity.vy);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11.acceleration.ax;	
	sprintf(str_csv, "%8.4f;", cat_11.acceleration.ax);
	csv_string += str_csv;
	// cvs_string += cat_11.acceleration.ay;	
	sprintf(str_csv, "%8.4f;", cat_11.acceleration.ay);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11.calc_vert_rate;
	sprintf(str_csv, "%8.4f;", cat_11.calc_vert_rate);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11._call_sign.sti;	
	sprintf(str_csv, "%d;", cat_11._call_sign.sti);
	csv_string += str_csv;
	csv_string += cat_11._call_sign.callsign;	
	csv_string += ";";
	//-------------------------
	// cvs_string += cat_11.target_size.length;		
	sprintf(str_csv, "%8.4f;", cat_11.target_size.length);
	csv_string += str_csv;
	// cvs_string += cat_11.target_size.width;			
	sprintf(str_csv, "%8.4f;", cat_11.target_size.width);
	csv_string += str_csv;
	// cvs_string += cat_11.target_size.orientation;	
	sprintf(str_csv, "%8.4f;", cat_11.target_size.orientation);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11._ages.psr_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.psr_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.ssr_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.ssr_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.mda_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.mda_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.mfl_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.mfl_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.mds_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.mds_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.ads_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.ads_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.ads_es_age;		
	sprintf(str_csv, "%8.4f;", cat_11._ages.ads_es_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.md1_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.md1_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.md2_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.md2_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.loop_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.loop_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.trk_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.trk_age);
	csv_string += str_csv;
	// cvs_string += cat_11._ages.mlt_age;			
	sprintf(str_csv, "%8.4f;", cat_11._ages.mlt_age);
	csv_string += str_csv;
	//-------------------------
	// cvs_string += cat_11.fleet;							
	sprintf(str_csv, "%d;", cat_11.fleet);
	csv_string += str_csv;
	//--------------------------
	// cvs_string += cat_11._prog_msg.trouble;
	if(cat_11._prog_msg.trouble)
		// sprintf(str_csv, "%d", cat_11._prog_msg.trouble);
		csv_string += "true;";
	else
		csv_string += "false;";
	// cvs_string += cat_11._prog_msg.type;			
	sprintf(str_csv, "%d;", cat_11._prog_msg.type);
	csv_string += str_csv;
	//--------------------------
	// cvs_string += cat_11.adsBRelatedData.mode_s_data_count;   
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

	// cvs_string += cat_11.adsBRelatedData.target_address; 
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.target_address);
	csv_string += str_csv;
	// cvs_string += cat_11.adsBRelatedData.communication_capability; 
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.communication_capability);
	csv_string += str_csv;
	// cvs_string += cat_11.adsBRelatedData.flight_status;	
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.flight_status);
	csv_string += str_csv;
	// cvs_string += cat_11.adsBRelatedData.specific_service; 
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.specific_service);
	csv_string += str_csv;
	// cvs_string += cat_11.adsBRelatedData.altitude_report_capability;	
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.altitude_report_capability);
	csv_string += str_csv;
	// cvs_string += cat_11.adsBRelatedData.aircraft_ident_capability;		
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.aircraft_ident_capability);
	csv_string += str_csv;
	// cvs_string += cat_11.adsBRelatedData.bit_b1a;		
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.bit_b1a);
	csv_string += str_csv;
	// cvs_string += cat_11.adsBRelatedData.bits_b1b;		
	sprintf(str_csv, "%d;", cat_11.adsBRelatedData.bits_b1b);
	csv_string += str_csv;
	// cvs_string += cat_11.adsBRelatedData.acas_operational;	
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
void AsterixWorker::handleCat11(TRecCategory_11 *ptrCat11)
{
	// using namespace Asterix;

	// char str[512];
	double eps = 0.000001;
	// std::string cvs_string;		// cvs-строка для записи в файл

	uint8_t msg_type;	// элемент данных I011/000
	// const DataSourceId* id;	// элемент данных I011/010
	// const double*  time;		// элемент данных I011/140

	// const uint8_t* ptrMsgType; 
	msg_type =	ptrCat11->msg_type;
	// ptrMsgType = &msg_type;

	// const DataSourceId*      ptrId;
	// DataSourceId DSI;
	// DSI.sac = ptrCat11->dsi.sac;
	// DSI.sic = ptrCat11->dsi.sic;
	// ptrId = &DSI;

	// const double*            ptrTime     = cat11->get_time();
	/*const PositionCartesian* pos      = cat11->get_cart_pos();
	const uint16_t*          tn       = cat11->get_track_number();*/
		
	//if (!ptrMsgType || !id || !time)
	//{
	//	if (!id)
	//		//mm_log(AST, ERROR, "missing some fields in cat11: dsi\n");
	//		ern::Log::getInstance().Write("Error, missing some fields in cat11: dsi\n");
	//	if (!ptrTime)
	//		//mm_log(AST, ERROR, "missing some fields in cat11: time\n");
	//		ern::Log::getInstance().Write("Error, missing some fields in cat11: time\n");
	//	if (!ptrMsgType)
	//		// mm_log(AST, ERROR, "missing some fields in cat11: msg_type\n");
	//		ern::Log::getInstance().Write("Error, missing some fields in cat11: msg_type\n");
	//	return;
	//}

	// msg_type = *ptrMsgType;

	switch (msg_type)
	{
		case MT_UNKNOWN:
			break;
		case MT_TRACK_AND_FLIGHT_PLAN:
		{
			int x;
			RecCategory_11 cat_11;
			
			cat_11.msg_type = msg_type;
			// msg_MessageType.push_back(msg_type);
			//-----------------------------
			cat_11.dsi.sac = ptrCat11->dsi.sac;
			cat_11.dsi.sic = ptrCat11->dsi.sic;
			// msg_DataSourceId.push_back(cat_11.dsi);
			//----------------------------
			// const uint8_t* ptrServiceID = cat11->get_service();
			if(ptrCat11->service_id == 0)
			{
				cat_11.service_id = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Service Identification\n");
			}
			else
				cat_11.service_id = ptrCat11->service_id;
			// msg_ServiceId.push_back(cat_11.service_id);
			//----------------------------
			// const Position* ptrPos = cat11->get_position();	 ///< Position in WGS-84 Coordinates
			cat_11.pos_wgs84.wgs84_lat = ptrCat11->pos_wgs84.wgs84_lat;
			cat_11.pos_wgs84.wgs84_lon =  ptrCat11->pos_wgs84.wgs84_lon;
			
			// msg_Position_wgs84.push_back(cat_11.pos_wgs84);
			//---------------------------
			// const PositionCartesian* ptrPosCart = cat11->get_cart_pos();	 ///< Position in Cartesian Co-ordinates
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
				// получим координаты точки на плоскости в проекции Меркатора
				double conf_northing_y = 0;
				double conf_easting_x = 0;
				// WgsToLocPr(ern::ConfiguratorAirports::getInstance().getLatCTA(), ern::ConfiguratorAirports::getInstance().getLonCTA(), &Conf_East_X, &Conf_North_Y);
				
				GeoCalc geo_calc;
				geo_calc.LatLong2UTMMerc(ern::ConfiguratorAirports::getInstance().getLonCTA(), ern::ConfiguratorAirports::getInstance().getLatCTA(), &conf_easting_x, &conf_northing_y); 
	
				
				double my_northing_y = 0;
				double my_easting_x = 0;
				// WgsToLocPr(MyLAT, MyLON, &My_East_X, &My_North_Y);
				geo_calc.LatLong2UTMMerc(MyLON, MyLAT, &my_easting_x, &my_northing_y); 

				// Point temp_point(cat_11.pos_cart.y + Conf_North_Y - My_North_Y, cat_11.pos_cart.x + Conf_East_X - My_East_X, ptrAirdrome->getPtrAirRefPoint()->getHeight(), 0, CPA);
				// *pX = dX * 1.01941 + Bx;
				// cat_11.pos_cart.x = ptrCat11->pos_cart.x + (conf_easting_x - my_easting_x) * 1.01941 - 34.3;
				cat_11.pos_cart.x = ptrCat11->pos_cart.x * 1.002054 + (conf_easting_x - my_easting_x);


				/*if(abs(conf_northing_y - my_northing_y) > 1080)
					cat_11.pos_cart.y = ptrCat11->pos_cart.y + (conf_northing_y - my_northing_y) / 1.06623;
				else
					cat_11.pos_cart.y = ptrCat11->pos_cart.y + (conf_northing_y - my_northing_y) / 1.29;*/
				/*cat_11.pos_cart.x = ptrCat11->pos_cart.x + conf_easting_x - my_easting_x;*/
				// cat_11.pos_cart.y = ptrCat11->pos_cart.y + (conf_northing_y - my_northing_y) * 1.03356 + 1.95;
				cat_11.pos_cart.y = ptrCat11->pos_cart.y * 0.95983 + (conf_northing_y - my_northing_y - 108);
			}
			// msg_PositionCartesian.push_back(cat_11.pos_cart);			
			//--------------------------
			// const Mode3A* ptrMode3A = cat11->get_mode_3a();	///< Mode 3/A Code
			if(ptrCat11->mode_3A == 0)
			{
				cat_11.mode_3A = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Mode 3/A Code\n");
			}
			else
				cat_11.mode_3A = ptrCat11->mode_3A;
			// msg_Mode3A.push_back(cat_11.mode_3A);
			//-------------------------
			// const double* ptrFlightLevel = cat11->get_flight_level();	// Measured Flight Level
			/*if(abs(ptrCat11->flight_level)<0)
			{
				cat_11.flight_level = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Measured Flight Level\n");
			}
			else*/
				cat_11.flight_level = ptrCat11->flight_level;
			// msg_FlightLevel.push_back(cat_11.flight_level);			
			//-------------------------
			// const double* ptrGeoAltitude = cat11->get_geo_altitude();	// Calculated Track Geometric Altitude
			/*if(!ptrGeoAltitude)
			{
				cat_11.geo_altitude = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11:_geo_altitude\n");
			}
			else*/
				cat_11.geo_altitude = ptrCat11->geo_altitude;
			// msg_GeoAltitude.push_back(cat_11.geo_altitude);
			//-------------------------
			//const Cat11::BaroAltitude* ptrBaroAltitude = cat11->get_baro_altitude();	//  Calculated Track Barometric Altitude
			//if(!ptrBaroAltitude)
			//{
			//	cat_11.baro_alt.qnh = 0;
			//	cat_11.baro_alt.altitude = 0;
			//	ern::Log::getInstance().Write("Error, missing some fields in cat11: Track Barometric Altitude\n");
			//}
			//else
			//{
				cat_11.baro_alt.qnh = ptrCat11->baro_alt.qnh;
				cat_11.baro_alt.altitude = ptrCat11->baro_alt.altitude;
			// }
			// msg_BaroAltitude.push_back(cat_11.baro_alt);
			//-------------------------
				cat_11.time_of_track = ptrCat11->time_of_track;	//  Time of Track
			// the_last_message = cat_11.time_of_track;
			// msg_TimeOfTrack.push_back(cat_11.time_of_track);			
			//-------------------------
			// const uint16_t* ptrTN = cat11->get_track_number();		// Track Number
				cat_11.track_number = ptrCat11->track_number;
				// pos_cart.y = ptrPosCart->y;
			// msg_TrackNumber.push_back(cat_11.track_number);
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
			
			// msg_TrackStatus.push_back(cat_11.tr_status);
			//-------------------------
			// const Velocity* ptrVelocity = cat11->get_velocity();	//< Calculated Track Velocity in Cartesian Coord.
				cat_11.tr_velocity.vx = ptrCat11->tr_velocity.vx;
				cat_11.tr_velocity.vy = ptrCat11->tr_velocity.vy;
			
			// this->msg_TrackVelocity.push_back(cat_11.tr_velocity);
			//-------------------------
			//const Acceleration* ptrAcceleration = cat11->get_accel();	//< Calculated Acceleration
			//if(!ptrAcceleration)
			//{
			//	cat_11.acceleration.ax = 0;
			//	cat_11.acceleration.ay = 0;
			//	ern::Log::getInstance().Write("Error, missing some fields in cat11: Calculated Acceleration\n");
			//}
			
				cat_11.acceleration.ax = ptrCat11->acceleration.ax;
				cat_11.acceleration.ay = ptrCat11->acceleration.ay;
			
			// this->msg_CalcAcceleration.push_back(cat_11.acceleration);
			//-------------------------
			// const double* ptrCalcVertRate = cat11->get_vert_rate();		// Calculated Rate Of Climb/Descent
			if( abs(ptrCat11->calc_vert_rate) < eps)
			{
				cat_11.calc_vert_rate = 0;
				ern::Log::getInstance().Write("Error, missing some fields in cat11:_vert_rate\n");
			}
			else
				cat_11.calc_vert_rate = ptrCat11->calc_vert_rate;
			// msg_CalcVertRate.push_back(cat_11.calc_vert_rate);
			//-------------------------
			// const Callsign* ptrCallsign = cat11->get_callsign();		//< Target Identification
				cat_11._call_sign.sti = ptrCat11->_call_sign.sti;
				strncpy(cat_11._call_sign.callsign, ptrCat11->_call_sign.callsign, cat_11._call_sign.CALLSIGN_SIZE);
			// this->msg_CallSign.push_back(cat_11._call_sign);
			//-------------------------
			// const Size* ptrSize = cat11->get_size();		//< Target Size & Orientation
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
			// this->msg_TargetSizeOrient.push_back(cat_11.target_size);
			//-------------------------
			// const Cat11::Ages* ptrAges = cat11->get_ages();		//< System Track Update Ages
			
			cat_11._ages.psr_age = ptrCat11->_ages.psr_age;				cat_11._ages.ssr_age = ptrCat11->_ages.ssr_age;
			cat_11._ages.mda_age = ptrCat11->_ages.mda_age;			cat_11._ages.mfl_age = ptrCat11->_ages.mfl_age;
			cat_11._ages.mds_age = ptrCat11->_ages.mds_age;				cat_11._ages.ads_age = ptrCat11->_ages.ads_age;
			cat_11._ages.ads_es_age = ptrCat11->_ages.ads_es_age;		cat_11._ages.md1_age = ptrCat11->_ages.md1_age;
			cat_11._ages.md2_age = ptrCat11->_ages.md2_age;			cat_11._ages.loop_age = ptrCat11->_ages.loop_age;
			cat_11._ages.trk_age = ptrCat11->_ages.trk_age;					cat_11._ages.mlt_age = ptrCat11->_ages.mlt_age;
			
			// this->msg_Ages.push_back(cat_11._ages);
			//-------------------------
			// Vehicle Fleet Identification
			// const int* ptr_fleet_type = (int*)cat11->get_fleet_type();		// Vehicle Fleet Identification 
			if(ptrCat11->fleet == 0)
			{
				cat_11.fleet = 0;			
				ern::Log::getInstance().Write("Error, missing some fields in cat11: Vehicle Fleet Identification\n");
			}
			else
				cat_11.fleet = ptrCat11->fleet;
			
			// this->msg_Fleet.push_back(cat_11.fleet);			
			//-------------------------
			// Pre-programmed Message
			// const ProggramedMessage* ptrProggramedMessage = cat11->get_proggramed_message();
				cat_11._prog_msg.trouble =	ptrCat11->_prog_msg.trouble;
				cat_11._prog_msg.type =		ptrCat11->_prog_msg.type;
			
			// this->msg_Prog.push_back(cat_11._prog_msg);	
			//------------------------
			// Mode-S / ADS-B Related Data
	        // const Cat11::ModeS* ptrModeS = cat11->get_mode_s();	// Mode-S / ADS-B Related Data
			//if(!ptrModeS)
			//{
			//	cat_11.adsBRelatedData.mode_s_data_count = 0;				// Количество переданных восьмибайтовых сообщений (субполе №1 [REP])
			//	cat_11.adsBRelatedData.mode_s_msg = 0;						// Указатель на сообщения Mode S (для повторяющихся субполей №1 [MB_DATA] )
			//	cat_11.adsBRelatedData.target_address = 0;						// Значение 24-бит адреса ИКАО для самолетов (субполе №2 [ADDRESS])
			//	cat_11.adsBRelatedData.communication_capability = 0;		// Возможность коммуникации (параметр субполя №4 [COM])
			//	cat_11.adsBRelatedData.flight_status = 0;							// Статус полета	(параметр субполя №4 [STAS])
			//	cat_11.adsBRelatedData.specific_service = 0;						// Признак спецобслуживания (приоритет обслуживания,	параметр субполя №4 [SSC] )
			//	cat_11.adsBRelatedData.altitude_report_capability = 0;		// Признак установленной точности определения высоты (параметр субполя №4 [ARC]) 
			//	cat_11.adsBRelatedData.aircraft_ident_capability = 0;			// Признак возможности идентификации (параметр субполя №4 [AIC])
			//	cat_11.adsBRelatedData.bit_b1a = 0;								// Значение В1А (параметр субполя №4 [В1А])
			//	cat_11.adsBRelatedData.bits_b1b = 0;								// Значение В1В (параметр субполя №4 [В1В])
			//	cat_11.adsBRelatedData.acas_operational = 0;					// Признак работоспособности системы ACAS (параметр субполя №4 [AC])
			//	
	 	//		// ern::Log::getInstance().Write("Error, missing some fields in cat11: Mode-S / ADS-B Related Data\n");
			//}			
			
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
			
			// msg_AdsBRelatedData.push_back(cat_11.adsBRelatedData);
			//-----------------------
			//	далее требуется осуществить проверку занятости ВПП и актуализацию трасс с учетом меток времени
			// отсечем все устаревшие трассы
			ptrTrackList->checkActualizationTraces(cat_11.time_of_track);
			// проверим занятость ВПП на основе данных трасс, и обновим состояния ВПП аэродромов
			/*if(ptrTrackList->isRunwayEmploymentStatus(1) )
				ptrAirdrome->getPtrRunway(1)->setRunwayEmploymentStatus(true);
			else
				ptrAirdrome->getPtrRunway(1)->setRunwayEmploymentStatus(false);*/
			//----------------------------------------------
			/*if(ptrTrackList->isRunwayEmploymentStatus(2) )
				ptrAirdrome->getPtrRunway(2)->setRunwayEmploymentStatus(true);
			else
				ptrAirdrome->getPtrRunway(2)->setRunwayEmploymentStatus(false);*/			
			//-----------------------
			// полученную запись нужно прикрепить к одной из трасс
			Track* ptrTrack;
			// Track* ptrNewTrack;
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
			// координаты X и Y заданы относительно референсной точки, заданной в конфигурационном файле
			// wgs_84_point_cpa.wgs_Y_lat = ern::ConfiguratorAirports::getInstance().getLatCTA();     // задаeтся референсная точка (как задано у Лаврентия)
			// wgs_84_point_cpa.wgs_X_lon = ern::ConfiguratorAirports::getInstance().getLonCTA();	// задается референсная точка (как задано у Лаврентия)
			// получим координаты точки в глобальной прямоугольной системе
			/*double Conf_North_Y = 0;
			double Conf_East_X = 0;
			WgsToLocPr(ern::ConfiguratorAirports::getInstance().getLatCTA(), ern::ConfiguratorAirports::getInstance().getLonCTA(), &Conf_East_X, &Conf_North_Y);

			double My_North_Y = 0;
			double My_East_X = 0;
			WgsToLocPr(MyLAT, MyLON, &My_East_X, &My_North_Y);*/

			// Point temp_point(cat_11.pos_cart.y + Conf_North_Y - My_North_Y, cat_11.pos_cart.x + Conf_East_X - My_East_X, ptrAirdrome->getPtrAirRefPoint()->getHeight(), 0, CPA);
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
void AsterixWorker::parseAsterixMsgCat11(TRecCategory_11 *ptrCat11)
{
	// char errINFO[80];
	// unsigned char data[BUFF_MESSAGE_INFO];
	// memcpy(data, buffAsterixMsgCat011, size_buff_Asterix_MsgCat011_ );

	// AsterixDecoder decoder(A_DECODE_SINGLE, false);
	// AsterixDecoder decoder(A_DECODE_DELIMITED, false);

	// AsterixDecoder* dec;		
	// dec = &decoder;
	// AsterixCoder   coder(A_CODE_SINGLE);

	// using Cat11::Cat11;
	// using namespace Cat11;

	// dec->feed(data, size_buff_Asterix_MsgCat011_);
	// закодированную полученную строку необходимо раскодировать и записи трасс считать в цикле
	/*AsterixRecord* r = decoder.decode();
	Cat11* cat_11 = dynamic_cast<Cat11*>(r);*/
	/*while (AsterixRecord* r = dec->decode())
	{
		using namespace Cat11;*/
		
		// if(struct Cat11* cat11 =  dynamic_cast<struct Cat11*>(r))
			handleCat11(ptrCat11);
		/*else
		{
			sprintf(errINFO, "Unknown category: %d \n", r->category);
			ern::Log::getInstance().Write(errINFO);
		}			
		delete r;*/
	// }
	// memset(this->buffAsterixMsgCat011, 0, this->size_buff_Asterix_MsgCat011_);
	size_buff_Asterix_MsgCat011_ = 0;
}
//-----------------------------------------------
// перересовывает текущую целевую обстановку
void AsterixWorker::drawTargetSituation()
{
	int ch, num_simb_x, num_simb_y;
	double doble_num_simb_x, doble_num_simb_y, ostatok_x, ostatok_y;
	double ref_point_x, ref_point_y, ref_point_cta_x, ref_point_cta_y;
	double x, y;
	TTargetEnvironment target_envs[24];

	// ref_point_x = -118;	// 30.2625	
	// ref_point_y = -400;	// 59.7965498641

	/////////////для теста////////////////////////////////
	//double Conf_North_Y = 0;
	//double Conf_East_X = 0;
	//WgsToLocPr(ern::ConfiguratorAirports::getInstance().getLatCTA(), ern::ConfiguratorAirports::getInstance().getLonCTA(), &Conf_East_X, &Conf_North_Y);

	//double My_North_Y = 0;
	//double My_East_X = 0;
	//WgsToLocPr(MyLAT, MyLON, &My_East_X, &My_North_Y);

	//ref_point_x = Conf_East_X - My_East_X;	// должно быть: -118
	//ref_point_y = Conf_North_Y - My_North_Y; // -400
	//////////конец теста///////////////////////
	
	// ref_point_cta_x = 2982.225;
	// ref_point_cta_x = 2806.785; //(примем за референсную точку - точку под КТА)
	// ref_point_cta_y = - 1263.06;
	// ref_point_cta_y = - 1600.00;

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
	// -----------------------------

	// цели
	if( ptrTrackList->getNumberTraces() > 0)
	{
		// int temp;
		Track* ptrTrack = ptrTrackList->getBegTrack();
		x = ptrTrack->getCat11Record()->getCat11Record(ptrTrack->getCat11Record()->getCountElemList()-1).pos_cart.x;
		y = ptrTrack->getCat11Record()->getCat11Record(ptrTrack->getCat11Record()->getCountElemList()-1).pos_cart.y;
		/*x += ref_point_x;
		y += ref_point_y;*/
		// if(y < 1263.06 && y > - 1263.06 && x < 2806.785 && x > -2806.785)
		if(y < 0 && y > - 2526.12 && x < 5613.57 && x > 0)
		{
			// x += ref_point_cta_x;
			// y += ref_point_cta_y;
			num_simb_x = x / 70.17;
			doble_num_simb_x = x / 70.17;
			ostatok_x = doble_num_simb_x - num_simb_x;

			num_simb_y = abs(y / 105.255);
			doble_num_simb_y = abs(y / 105.255);
			ostatok_y = doble_num_simb_y - num_simb_y;

			// num_simb_y = y / 105.255;
			// num_simb_y = 11 - num_simb_y;
			target_envs[num_simb_y+1].str_status[num_simb_x] = 1;

			if(ostatok_x > 0.333333 && ostatok_x < 0.6666667 && ostatok_y > 0.333333 && ostatok_y < 0.6666667)
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 0;
				else if(ostatok_x > 0.5 && ostatok_y > 0.5)
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 1;
				else if(ostatok_x < 0.5 && ostatok_y > 0.5)
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 2;
				else if(ostatok_x < 0.5 && ostatok_y < 0.5)
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 3;
				else
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 4;
		}

		for(int i=1; i< ptrTrackList->getNumberTraces(); i++)
		{
			ptrTrack = ptrTrackList->getNextTrack();
			x = ptrTrack->getCat11Record()->getCat11Record(ptrTrack->getCat11Record()->getCountElemList()-1).pos_cart.x;
			y = ptrTrack->getCat11Record()->getCat11Record(ptrTrack->getCat11Record()->getCountElemList()-1).pos_cart.y;
			/*x += ref_point_x;
			y += ref_point_y;*/
			// if(y < 1263.06 && y > - 1263.06 && x < 2806.785 && x > -2806.785)
			if(y < 0 && y > - 2526.12 && x < 5613.57 && x > 0)
			{
				// x += ref_point_cta_x;
				// y += ref_point_cta_y;
				num_simb_x = x / 70.17;
				doble_num_simb_x = x / 70.17;
				ostatok_x = doble_num_simb_x - num_simb_x;

				num_simb_y = abs(y / 105.255);
				doble_num_simb_y = abs(y / 105.255);
				ostatok_y = doble_num_simb_y - num_simb_y;
				// num_simb_y = y / 105.255;
				// num_simb_y = 11 - num_simb_y;
				target_envs[num_simb_y+1].str_status[num_simb_x] = 1;

				/*if(ostatok_x > 23.39 && ostatok_x < 46.78 && ostatok_y > 35.085 && ostatok_y < 70.17)
					target_envs[num_simb_y].target_koord[num_simb_x] = 0;
				else if(ostatok_x > 35.085 && ostatok_y > 52.6275)
					target_envs[num_simb_y].target_koord[num_simb_x] = 1;
				else if(ostatok_x < 35.085 && ostatok_y > 52.6275)
					target_envs[num_simb_y].target_koord[num_simb_x] = 2;
				else if(ostatok_x < 35.085 && ostatok_y < 52.6275)
					target_envs[num_simb_y].target_koord[num_simb_x] = 3;
				else
					target_envs[num_simb_y].target_koord[num_simb_x] = 4;*/

				if(ostatok_x > 0.333333 && ostatok_x < 0.6666667 && ostatok_y > 0.333333 && ostatok_y < 0.6666667)
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 0;
				else if(ostatok_x > 0.5 && ostatok_y > 0.5)
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 1;
				else if(ostatok_x < 0.5 && ostatok_y > 0.5)
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 2;
				else if(ostatok_x < 0.5 && ostatok_y < 0.5)
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 3;
				else
					target_envs[num_simb_y+1].target_koord[num_simb_x] = 4;
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

	/*Цвет всего фона - белый. Цвет всего текста - черный */
	// system("color F0");
	// puts("Hello World!");

	
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
		//------------------------------------------
		// последний символ в строке
		/*
		if( target_envs[i].str_status[79] == 0 ) // это - фон! Он зеленый
		{
				#ifdef __linux__ 
				//linux code goes here
					puts(" ");
				#elif _WIN32
				// windows code goes here
					//Цвет символов - белый. Цвет фона - зеленый
					SetConsoleTextAttribute(hConsole, (WORD) ((GREEN << 4) | WHITE));	
					puts(" ");
				#else
				// #error "OS not supported!"
				#endif
		}
		else if( target_envs[i].str_status[79] == 1 ) // здесь находится цель
		{
				#ifdef __linux__ 
				//linux code goes here
					puts("0");
				#elif _WIN32
				// windows code goes here
					//Цвет символов - белый. Цвет фона - синий
					SetConsoleTextAttribute(hConsole, (WORD) ((BLUE << 4) | WHITE));	
					puts(" ");
				#else
				// #error "OS not supported!"
				#endif				
		}
		else if( target_envs[i].str_status[79] == 2 ) // здесь находится кусочек ВПП
		{
				#ifdef __linux__ 
				//linux code goes here
					puts("X");
				#elif _WIN32
				// windows code goes here
				//Цвет символов - белый. Цвет фона - серый
					SetConsoleTextAttribute(hConsole, (WORD) ((LIGHT_GRAY << 4) | WHITE));	
					puts(" ");
				#else
				// #error "OS not supported!"
				#endif				
		}
		else if( target_envs[i].str_status[79] == 3 ) // здесь находится контрольная точка
		{
				#ifdef __linux__ 
				//linux code goes here
					puts("C");
				#elif _WIN32
				// windows code goes here
					//Цвет символов - черный. Цвет фона - серый
					SetConsoleTextAttribute(hConsole, (WORD) ((WHITE << 4) | BLACK));	
					puts(" ");
				#else
				// #error "OS not supported!"
				#endif				
		}
		*/
	}
	

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
}
//-------------------------------------------------------------------------------------------------------------
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
    
	int n_simb;
    int err, err1;			// Коды ошибки
	int cycle_number;	// номер цикла	полученного сообщения, если больше 5, то он обнуляется, что является сигналом к проверке файла настроек с
								// последующим его обновлением (!)
	double cycle_counter;										// счетчик циклов, если номер извлеченной строки меньше двух, то не обрабатывать строку
	
    char szErrMessage [MAX_ERR_MESSAGE]; //Буфер сообщения об ошибке
    // static char szBuffMessageInfo[BUFF_MESSAGE_INFO]; //Буфер №1 для поступающих данных
	char szBuffMessageInfo[BUFF_MESSAGE_INFO]; //Буфер №1 для поступающих данных
	
	std::string str_temp;
	std::vector<std::string> delim_n;
	std::vector<std::string>::iterator iter;

	TRecCategory_11 *ptrCat11;
	
	// int number_buff;			  // номер используемого буфера (для удобства при асинхронной обработке с использование switch() )

	// char szBuffMessageInfo[BUFF_MESSAGE_INFO]; //Буфер для поступающих данных
    // unsigned short usPort;
    //enum { max = 80 };
    //char buffErr[max];
	//static char
    // int value = 1; 
    
	err = 1;

	std::string input_file_name = "cat11_1_test";

    // char msg_WSAEFAULT[] = "lpWSAData не допустимый указатель. Значение параметра namelen меньше размера структуры sockaddr";
	//char msg_WSAEFAULT[] = "lpWSAData not a valid pointer.\n The value of namelen smaller size of struct sockaddr\n";

 //   // char msg_WSANOTINITIALISED[] = "Перед использование функции необходимо вызвать функцию WSAStartup";
	//char msg_WSANOTINITIALISED[] = "Before you use the function you must call the WSAStartup function\n";

 //   // char msg_WSAENETDOWN[] = "Сбой в сети";
	//char msg_WSAENETDOWN[] = "Network failure\n";

 //   char msg_WSAEADDRINUSE[] = "The specified address already is used\n";//Указанный адрес уже используется";

 //   // char msg_WSAEINPROGRESS[] = "Выполняется блокирующая функция интерфейса Windows Sockets";
	//char msg_WSAEINPROGRESS[] = "At this time, perform a blocking function Windows Sockets Interface\n";

 //   // char msg_WSAEAFNOSUPPORT[] = "Выбранный протокол не может работать с указанным семейством адресов";
	//char msg_WSAEAFNOSUPPORT[] = "The selected protocol may not work with the specified address family\n";

 //   // char msg_WSAEINVAL[] = "Сокет уже привязан к адресу";
	//char msg_WSAEINVAL[] = "The socket is already bound to an address\n";

 //   // char msg_WSAENOBUFS[] = "Установлено слишком много соединений";
	//char msg_WSAENOBUFS[] = "Too many connections\n";

 //   // har msg_WSAENOTSOCK[] = "Указанный в параметре дескриптор не является дескриптором сокета";
	//char msg_WSAENOTSOCK[] = "This parameter descriptor is not a socket handle\n";

	// struct sockaddr_in src_recv_addr, dst_arm_send_addr;
    // struct sockaddr_in src_addr, dst_addr;

	// SOCKET src_sock;
    // SOCKET dst_sock;

	// struct hostent *ent;

   /*union a
	{
       int z;
	   struct
	   {
            unsigned char a1;
		    unsigned char a2;
		    unsigned char a3;
		    unsigned char a4;
		}b;
	}val;  */

	//#ifdef __linux__ 
	////linux code goes here
	//int fd;
	//if( (fd = socket(PF_INET, SOCK_DGRAM, 0) ) == -1) 
	//{
	//	// perror("error in function socket(...)");
	//    ern::Log::getInstance().Write("error in function socket(...)");
	//	return 0;
	//}
	//#elif _WIN32
	//// windows code goes here
	//src_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//if (src_sock == INVALID_SOCKET)
	//{
	//	 // lstrcpy(szBuf, "Не удалось установить связь с сокетом.\n");
	//	 // printf("Unable to establish a connection with a socket\n");	
	//	 ern::Log::getInstance().Write("Unable to establish a connection with a socket\n");
	//	 return 0; 
	//}	
	//#else
	//	// #error "OS not supported!"
	//#endif	
       
	
    // получим размер структуры настроек 
	// nSize = sizeof ( SOCKADDR_IN );
                   
    // почистим память
	// memset(&src_recv_addr, 0, nSize);

    // сформируем необходимые данные
    // src_recv_addr.sin_family = AF_INET;
    // src_recv_addr.sin_port = htons(59399);
	// atoi(argv[2])
    // src_recv_addr.sin_port = htons(sPort);
	//ConfiguratorApp
	// src_recv_addr.sin_port = htons(ern::ConfiguratorApp::getInstance().getPortTestClientCons());
	// src_recv_addr.sin_port = htons(24322);


    // src_recv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    // char szErrMessage [MAX_ERR_MESSAGE]; //Буфер сообщения об ошибке
    szErrMessage[0] = '\0';

	//#ifdef __linux__ 
	//	//linux code goes here
	//	 // привяжем сокет к нашим настройкам адреса и порта
	//	 if (bind(src_sock, (SOCKADDR*) &src_recv_addr, sizeof(src_recv_addr)) == -1)
 //        {
 //             // printf("Error function Bind: %d\n", err1);
	//		  sprintf(szErrMessage, "Error function Bind(...)");
	//		  ern::Log::getInstance().Write(szErrMessage);
	//			
	//		  szErrMessage[0] = '\0';
 //             
 //             closesocket(src_sock);
 //             return 0;
 //      }
	//#elif _WIN32
	//	// windows code goes here
	//	// привяжем сокет к нашим настройкам адреса и порта
	//	 if (bind(src_sock, (SOCKADDR*) &src_recv_addr, sizeof(src_recv_addr)) == SOCKET_ERROR)
 //        {
 //             //printf("Ошибка функции Bind: %d\n", WSAGetLastError()); 
	//		  err1 = WSAGetLastError();
	//		  // printf("Ошибка функции Bind: %d\n", err1);
	//		  // printf("Error function Bind: %d\n", err1);
	//		  sprintf(szErrMessage, "Error function Bind: %d\n", err1);
	//		  ern::Log::getInstance().Write(szErrMessage);
	//			
	//		  szErrMessage[0] = '\0';
 //             switch(err1)//switch(10014)//
	//		  {
	//			  case WSANOTINITIALISED:
	//				  // lstrcpy((LPTSTR)szErrMessage, (LPTSTR)msg_WSANOTINITIALISED);
	//				  sprintf(szErrMessage, "%s\n", msg_WSANOTINITIALISED);
	//				  break;
	//			  case WSAENETDOWN:
	//				  // lstrcpy(szErrMessage, msg_WSAENETDOWN);
	//				  sprintf(szErrMessage, "%s\n", msg_WSAENETDOWN);
	//	              break;
 //                 case WSAEADDRINUSE:
	//	                // lstrcpy(szErrMessage, msg_WSAEADDRINUSE);
	//					sprintf(szErrMessage, "%s\n", msg_WSAEADDRINUSE);
	//	                break;
	//			  case WSAEFAULT:
	//	                // lstrcpy(szErrMessage, msg_WSAEFAULT);
	//					sprintf(szErrMessage, "%s\n", msg_WSAEFAULT);
	//	                break;
 //                 case WSAEINPROGRESS:
	//	                // lstrcpy(szErrMessage, msg_WSAEINPROGRESS);
	//					sprintf(szErrMessage, "%s\n", msg_WSAEINPROGRESS);
	//	                break;
 //                 case WSAEAFNOSUPPORT:
	//	                sprintf(szErrMessage, "%s\n", msg_WSAEAFNOSUPPORT);
	//	                break;
 //                 case WSAEINVAL:
	//	                // lstrcpy(szErrMessage, msg_WSAEINVAL);
	//					sprintf(szErrMessage, "%s\n", msg_WSAEINVAL);
	//	                break;
 //                 case WSAENOBUFS:
	//	                // lstrcpy(szErrMessage, msg_WSAENOBUFS);
	//					sprintf(szErrMessage, "%s\n", WSAENOBUFS);
	//	                break;
 //                 case WSAENOTSOCK:
	//	                // lstrcpy(szErrMessage, msg_WSAENOTSOCK);
	//					sprintf(szErrMessage, "%s\n", msg_WSAENOTSOCK);
	//	                break;
	//		  }
	//		  // printf(szErrMessage);                          
	//		  ern::Log::getInstance().Write(szErrMessage);
 //             closesocket(src_sock);
 //             WSACleanup();
 //             return 0;
 //      }	
	//#else
	//	// #error "OS not supported!"
	//#endif

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
			
			// TRecCategory_11 *ptrCat11;
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
			
			// int x;
			// TRecCategory_11 cat_11;
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
			// msg_ServiceId.push_back(cat_11.service_id);
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
			//// cvs_string += cat_11.adsBRelatedData.mode_s_data_count;   
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.mode_s_data_count);
			//csv_string += str_csv;
			////---------------------
			//if(cat_11.adsBRelatedData.mode_s_data_count == 0)
			//	csv_string += ";";			
			//else
			//{
			//	for(int i =0; i<cat_11.adsBRelatedData.mode_s_data_count; i++)
			//	{
			//		#ifdef __linux__ 
			//		//linux code goes here
			//		sprintf(str_csv, "%" PRIu64 "|", cat_11.adsBRelatedData.mode_s_msg[i]);
			//		#elif _WIN32
			//		// windows code goes here
			//		sprintf(str_csv, "%llu|", cat_11.adsBRelatedData.mode_s_msg[i]);
			//		#else
			//			// #error "OS not supported!"
			//		#endif
			//		// csv_string += "|";			
			//	}
			//	csv_string += str_csv;
			//	csv_string += ";";
			//}

			//// cvs_string += cat_11.adsBRelatedData.target_address; 
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.target_address);
			//csv_string += str_csv;
			//// cvs_string += cat_11.adsBRelatedData.communication_capability; 
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.communication_capability);
			//csv_string += str_csv;
			//// cvs_string += cat_11.adsBRelatedData.flight_status;	
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.flight_status);
			//csv_string += str_csv;
			//// cvs_string += cat_11.adsBRelatedData.specific_service; 
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.specific_service);
			//csv_string += str_csv;
			//// cvs_string += cat_11.adsBRelatedData.altitude_report_capability;	
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.altitude_report_capability);
			//csv_string += str_csv;
			//// cvs_string += cat_11.adsBRelatedData.aircraft_ident_capability;		
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.aircraft_ident_capability);
			//csv_string += str_csv;
			//// cvs_string += cat_11.adsBRelatedData.bit_b1a;		
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.bit_b1a);
			//csv_string += str_csv;
			//// cvs_string += cat_11.adsBRelatedData.bits_b1b;		
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.bits_b1b);
			//csv_string += str_csv;
			//// cvs_string += cat_11.adsBRelatedData.acas_operational;	
			//sprintf(str_csv, "%d;", cat_11.adsBRelatedData.acas_operational);
			//csv_string += str_csv;
			//////////////////////////////////////////
						
			//-------------------------
			// Vehicle Fleet Identification
			// str_temp = delim_n[59];
			// ptrCat11->fleet = atoi(str_temp.c_str() );
			//-------------------------
			// Pre-programmed Message
			/*str_temp = delim_n[60];
			if(str_temp == "false")
				ptrCat11->_prog_msg.trouble = false;
			else
				ptrCat11->_prog_msg.type = true;*/
			//------------------------
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
		// }
		
		//////////////////////////////////////////////////////////////////////
		// int nCharRecv;        //возвращает результат выполнения функции recvfrom;
            //char szBuffMessageInfo[BUFF_MESSAGE_INFO]; //Буфер для поступающих данных
            // char * ptrSzBuffMessageInfo;
			// ptrSzBuffMessageInfo = szBuffMessageInfo;

            // очистим память для принятия информации
	        // memset(&dst_arm_send_addr, 0, nSize);
			// memset(szBuffMessageInfo, 0, BUFF_MESSAGE_INFO);
            //MessageBox(hwnd, "Все готово для recvfrom", "Debug step1", MB_OK);    
			// nLength = 0;				 
			// number_buff = 1;	// c szBuffMessageInfo1 никто не работает, тогда работать будем с ним
			// status_msg_1 = 1;
			// nCharRecv = recvfrom(src_sock, szBuffMessageInfo,	BUFF_MESSAGE_INFO, 0, (LPSOCKADDR)&dst_arm_send_addr, &nSize );
			//#ifdef __linux__ 
			//	//linux code goes here
			//	if(nCharRecv == -1)
   //             {
   //                   // printf("Error recvfrom\n");
			//		   ern::Log::getInstance().Write("Error recvfrom(...)\n");
   //                    closesocket(src_sock);
			//		   return 0;
			//	}
			//#elif _WIN32
			//	// windows code goes here
			//	//MessageBox(hwnd, "recvfrom отработал", "Debug step2", MB_OK);
			//	//char mesgErr[80];
   //             //sprintf(mesgErr, "%-20s: %d\0", "recvfrom отработал", nCharRecv);
   //                     
			//	if(nCharRecv == SOCKET_ERROR)
   //             {
   //                   //MessageBox(hwnd, "Ошибка recvfrom", "Ответ", MB_OK);
   //                   // lstrcpy(szErrMessage, "Ошибка recvfrom");
			//		  // printf("Error recvfrom\n");
			//		  ern::Log::getInstance().Write("Error recvfrom\n");
   //                   closesocket(src_sock);
			//		  WSACleanup();
			//		  return nCharRecv;
			//	 }
			//#else
			//	// #error "OS not supported!"
			//#endif

			//char buff[120];
            /*
                  sprintf(buff, "Get broadcast info Ok!\nSystemUTL Host IP:\t%s\nSystemUTL TCP/IP Port:\t%d\n\n",
			      inet_ntoa(dst_arm_send_addr.sin_addr),
			      //dst_arm_send_addr.szServerName,
			      //ntohl(dst_arm_send_addr.sin_port));
                  htons(dst_arm_send_addr.sin_port));
                  MessageBox(hwnd, buff, "Ответ", MB_OK);
		    */
             // szBuffMessageInfo[nCharRecv] = '\0';
             //MessageBox(NULL, szBuffMessageInfo, "Принятые данные", MB_OK);	


////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// AsterixWorker* ptrAsterixWorker = new AsterixWorker();
			// ptrAsterixWorker->setAsterixMSG(szBuffMessageInfo, nCharRecv);

		  	ptrAsterixWorker->parseAsterixMsgCat11(ptrCat11);

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
				//sax_pars_xml::TConfAppSection* pConfAppSection;
				//pConfAppSection = new sax_pars_xml::TConfAppSection;
				//// pConfAppSection
				//// strncpy(pConfAppSection[0].airport_name_en, ern, BUFF_NAME_AIRPORTS);
				//int lenStrAirportNameEn = ern::ConfiguratorApp::getInstance().getAirportNameEn(pConfAppSection[0].airport_name_en, BUFF_NAME_AIRPORTS);
				//pConfAppSection[0].port_main_client = ern::ConfiguratorApp::getInstance().getPortMainClient();
				//pConfAppSection[0].port_test_client_cons = ern::ConfiguratorApp::getInstance().getPortTestClientCons();
				//pConfAppSection[0].port_test_client_gui = ern::ConfiguratorApp::getInstance().getPortTestClientGui();
				//int lenStrIpMainClient = ern::ConfiguratorApp::getInstance().getIpMainClient(pConfAppSection[0].ip_main_client, BUFF_IP_CLIENT);
				//int lenStrIpTestClientCons = ern::ConfiguratorApp::getInstance().getIpTestClientCons(pConfAppSection[0].ip_test_client_cons, BUFF_IP_CLIENT);
				//. . . . .
				//delete pConfAppSection; 

				// заливаем конфигуратор данными из текущего файла настроек
				ern::ConfiguratorApp::getInstance().LoadXML(conf_file_name);
				int run_service = ern::ConfiguratorApp::getInstance().getRunService();
																
				/*ern::ConfiguratorApp::getInstance().setRecordCount(ptrAsterixWorker->getRecordCount());
				ern::ConfiguratorApp::getInstance().setTableCount(ptrAsterixWorker->getTableCount());
				ern::ConfiguratorApp::getInstance().UpdateXML(conf_file_name);*/

				//#ifdef __linux__ 
				//	//linux code goes here
				//	if(!run_service)	// пользователь системы решил завершить работу сервиса
				//	{
				//			// освобождение ресурсов и выход
				//			delete ptrAsterixWorker;
				//			closesocket(src_sock);
				//			return 1;
				//	}	
				//#elif _WIN32
				//	// windows code goes here
				//	if(!run_service)	// пользователь системы решил завершить работу сервиса
				//		{
				//			// освобождение ресурсов и выход
				//			delete ptrAsterixWorker;
				//			closesocket(src_sock);
				//			WSACleanup();
				//			return 1;
				//		}	
				//#else
				//	// #error "OS not supported!"
				//#endif	
				// delete ptrAsterixWorker;
				//			closesocket(src_sock);
				//			WSACleanup();
				// return 1;
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
	                 
					 
				 //if(szBuffMessageInfo != NULL)
				 //{
     //                   // ZeroMemory( &szBuffMessageInfo, BUFF_MESSAGE_INFO );
					//	memset(&szBuffMessageInfo, 0, BUFF_MESSAGE_INFO);
     //                   szBuffMessageInfo[0] = '\0';
				 //}

				// double cycle_counter;										// счетчик циклов, если номер извлеченной строки меньше двух, то не обрабатывать строку
	
				// char szErrMessage [MAX_ERR_MESSAGE];
				sprintf(szErrMessage, "\n cycle_counter = %6.2f\n", cycle_counter);
				std::cout <<	szErrMessage << std::endl;

				while(!delim_n.empty()){
					iter = delim_n.begin();		
					delim_n.erase(iter);
				}		
				delete ptrCat11;
				}
		} // конец while

	// closesocket(src_sock);
    
	//#ifdef __linux__ 
	////linux code goes here
	//
	//#elif _WIN32
	//// windows code goes here
	//WSACleanup();	
	//#else
	//	// #error "OS not supported!"
	//#endif
	//
	//return err;
	/*return 0;*/
}
//-------------------------------------------------
// тестирует поиск географических координат нулевой референсной точки 
// и оптимального коэффициента масштабирования
void testRefPointAndKoeffMasshtab()
{
	/*
	int i, j, k, t, n, v, s;
	double X0, Y0, X1, Y1, X2, Y2, H0, H1, H2, Lat0, Lat1, Lat2, Lon0, Lon1, Lon2, hight0, hight1, hight2, dX, dY, dZ, Sum_opt, number;
	double dX0, dY0, dZ0, Wx0, Wx, Wy0, Wy, Wz0, Wz, m0, m, Wx_opt, Wy_opt, Wz_opt, m_opt, dX_opt, dY_opt, dZ_opt, Sum; 
	double res, dLat, dLon, dH, Etalon_Lat0, Etalon_Lon0, Etalon_H0, Etalon_Lat1, Etalon_Lon1, Etalon_H1, Etalon_Lat2, Etalon_Lon2, Etalon_H2;
	char info[140];

	double *pX;
	double *pY; 
	double *pH;

	TWgsPoint wgs_point_0;
	TWgsPoint wgs_point_1;
	*/

	int i, j, k, t;
	double X0, Y0, X1, Y1, X2, Y2, number, res;
	double Lat0, Lon0, Etalon_X0, Etalon_Y0, Etalon_H0;
	double Lat1, Lon1, Etalon_X1, Etalon_Y1, Etalon_H1;
	double Lat2, Lon2, Etalon_X2, Etalon_Y2, Etalon_H2;
	double Sum_dX, Sum_dY, Sum, Sum_opt;
	double my_northing_y, my_easting_x;
	double conf_northing_y, conf_easting_x;
	double MyLON_, MyLON_0, MyLAT_, MyLAT_0;
	double MyLON_opt, MyLAT_opt;
	double koeff_masshtab_x, koeff_masshtab_x0;
	double koeff_masshtab_x_opt, koeff_masshtab_y_opt;
	double koeff_masshtab_y, koeff_masshtab_y0;
	double easting_x0,	northing_y0, easting_x1,	northing_y1, easting_x2,	northing_y2;
	char info[140];
	
	Sum_opt = 100000.0;
	Sum = 100000.0;

	MyLON_opt = 0;
	MyLAT_opt = 0;
	// koeff_masshtab_x_opt = 0;
	// koeff_masshtab_y_opt = 0;

	res = 1;

	GeoCalc geo_calc;

	// Зададим географические координаты трех точек (в градусах) в системе WGS-84
	// и их эталоные измерения в локальной системе координат
	TTargetLocationInMinutes targetLocationInMinutes;
	TTargetLocationInMinutes *pTargetLocationInMinutes;
	pTargetLocationInMinutes = &targetLocationInMinutes;
	// memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	// точка 0 - референсная точка (как сделано в интерфейсе Лаврентия)
	// зададим широту и долготу нулевой точки в десятичных дробях в системе WGS-84
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 47;
	pTargetLocationInMinutes->seconds = 50.98;
	Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 15;
	pTargetLocationInMinutes->seconds = 49.03;
	Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	// получим восточное и северное смещение проекции Меркатора
	easting_x0 = 0;
	northing_y0 = 0;

	geo_calc.LatLong2UTMMerc(Lon0, Lat0, &easting_x0, &northing_y0);
	//------------------
	// определим коэффициенты масштабирования
	koeff_masshtab_x =  1.002054;
	koeff_masshtab_y =  0.95983;
	// зададим эталоны нулевой точки в локальной системе координат
	// Etalon_X0 = 2805.0;
	Etalon_X0 = 2866.0 * koeff_masshtab_x;
	// Etalon_X0 = 2754.0;
	// Etalon_Y0 = -1192.0;
	Etalon_Y0 = -1483.0 * koeff_masshtab_y;
	Etalon_H0 = 24.0;
	//----------------
	// точка 1- Центр ВПП1 	
	// зададим широту и долготу первой точки в десятичных дробях в системе WGS-84
	// Lat1 = 59.795142;
	// Lon1 = 30.250639;
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 47;
	pTargetLocationInMinutes->seconds = 42.26;
	Lat1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 15;
	pTargetLocationInMinutes->seconds = 02.42;
	Lon1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	// получим восточное и северное смещение проекции Меркатора
	easting_x1 = 0;
	northing_y1 = 0;

	geo_calc.LatLong2UTMMerc(Lon1, Lat1, &easting_x1, &northing_y1);

	// зададим эталоны первой точки в локальной системе координат
	// Etalon_X1 = 2158.867;
	Etalon_X1 = 2133.0 * koeff_masshtab_x;
	// Etalon_X1 = 2107.867;
	Etalon_Y1 = -1734.56 * koeff_masshtab_y;

	Etalon_H1 = 22.5;
	
	//----------------
	// точка 2- Центр ВПП2 	
	// зададим широту и долготу второй точки в десятичных дробях в системе WGS-84
	// Lat2 = 59.805453;
	// Lon2 = 30.274379;
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 48;
	pTargetLocationInMinutes->seconds = 19.15;
	Lat2 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 16;
	pTargetLocationInMinutes->seconds = 29.57;
	Lon2 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	// получим восточное и северное смещение проекции Меркатора
	easting_x2 = 0;
	northing_y2 = 0;

	geo_calc.LatLong2UTMMerc(Lon2, Lat2, &easting_x2, &northing_y2);

	// зададим эталоны второй точки в локальной системе координат
	// Etalon_X2 = 3563.7882;
	Etalon_X2 = 3534.9 * koeff_masshtab_x;
	// Etalon_X2 = 3512.7882;
	Etalon_Y2 = -604.736 * koeff_masshtab_y;
	
	// (центр ВПП2)
	Etalon_H2 = 20.6;

	//-----------------------
	conf_northing_y = 0;
	conf_easting_x = 0;
	// WgsToLocPr(ern::ConfiguratorAirports::getInstance().getLatCTA(), ern::ConfiguratorAirports::getInstance().getLonCTA(), &Conf_East_X, &Conf_North_Y);
	// geo_calc.LatLong2UTMMerc(ern::ConfiguratorAirports::getInstance().getLonCTA(), ern::ConfiguratorAirports::getInstance().getLatCTA(), &conf_easting_x, &conf_northing_y); 
	geo_calc.LatLong2UTMMerc(Lon0, Lat0, &conf_easting_x, &conf_northing_y); 
	//-----------------------

	// зададим искомую точку левого верхнего угла экрана в WGS-84
	// MyLON_0 = 30.2117889;
	// MyLON_0 = 30.2112889;
	// MyLON_0 = 30.2107889;
	// MyLON_0 = 30.2108889;
	// MyLON_0 = 30.2109089;
	// MyLON_0 = 30.2109989;
	// MyLON_0 = 30.2109079;
	// MyLON_0 = 30.2100079;
	// MyLON_0 = 30.2105029;
	MyLON_0 = 30.2113079;
	// MyLON_ = MyLON_0 + 0.00005 * i;
	// MyLON_0 = 30.2119989;
	// MyLAT_0 = 59.8104972;
	// MyLAT_0 = 59.8099972;
	// MyLAT_0 = 59.8094972;
	// MyLAT_0 = 59.8089472;
	// MyLAT_0 = 59.8090572;
	// MyLAT_0 = 59.8089672;
	// MyLAT_0 = 59.8080672;
	// MyLAT_0 = 59.8083072;
	// MyLAT_0 = 59.8085422;
	MyLAT_0 = 59.8090972;
	// MyLAT_0 = 59.8093872;
	// koeff_masshtab_x0 = 1.00356;
	// koeff_masshtab_x0 = 0.96556;
	// koeff_masshtab_x0 = 0.97556;
	// koeff_masshtab_x0 = 1.02756;
	// koeff_masshtab_x0 = 1.02786;
	// koeff_masshtab_x0 =  1.03276;
	// koeff_masshtab_x0 =  1.02776;

	// koeff_masshtab_x0 = 0.98066;
	// koeff_masshtab_y0 = 1.01623;
	// koeff_masshtab_y0 = 1.01523;
	// koeff_masshtab_y0 = 0.97423;
	// koeff_masshtab_y0 = 0.94713;
	// koeff_masshtab_y0 = 0.94213;
	// koeff_masshtab_y0 = 0.96223;
	// koeff_masshtab_y0 = 0.96263;
	// koeff_masshtab_y0 = 0.96753;
		
	 for(i = 0; i < 200; i++)
	// for(i = 0; i < 5; i++)
	// int GeoCalc::P3_90_02_P3_90_11_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
	// if(geo_calc.P3_90_02_WGS_84(Lat0, Lon0, hight0, pH, pLat, pLon))
	{
		// MyLON_ = MyLON_0 + 0.000005 * i;
		MyLON_ = MyLON_0 + 0.000001 * i;
		// MyLON_ = MyLON_0 + 0.00001 * i;
		// MyLON_ = MyLON_0 + 0.000001 * i;

		for(j = 0; j < 200; j++)
		{
			// MyLAT_ = MyLAT_0 + 0.000005 * j;
			MyLAT_ = MyLAT_0 + 0.000001 * j;
			// MyLAT_ = MyLAT_0 + 0.00001 * j;
			// MyLAT_ = MyLAT_0 + 0.000001 * j;

			// for(k = 0; k < 100; k++)
			// {
				// koeff_masshtab_x = koeff_masshtab_x0 + 0.001 * k;	
				// koeff_masshtab_x = koeff_masshtab_x0 + 0.0001 * k;
				// koeff_masshtab_x = koeff_masshtab_x0 + 0.00001 * k;
				
				// for(t = 0; t < 100; t++)
				// {
					// koeff_masshtab_y = koeff_masshtab_y0 + 0.001 * t;	
					// koeff_masshtab_y = koeff_masshtab_y0 + 0.0001 * t;
					// koeff_masshtab_y = koeff_masshtab_y0 + 0.00001 * t;

					/*for(n = 0; n < 10; n++)
					{
						dX = dX0 + 0.001 * n;*/
						// dX = dX0 + 0.002 * n;
						// dX = dX0 + 0.01 * n;
						// dX = dX0 + 0.1 * n;
						// dX = dX0 + 0.5 * n;
						// dX = dX0 + 1 * n;
						// dX = dX0 + 2 * n;
						
						//--------------------------------------	
						my_northing_y = 0;
						my_easting_x = 0;
				
						// WgsToLocPr(MyLAT, MyLON, &My_East_X, &My_North_Y);
						geo_calc.LatLong2UTMMerc(MyLON_, MyLAT_, &my_easting_x, &my_northing_y); 

						// X0 = (easting_x0  - my_easting_x) / koeff_masshtab_x;
						X0 = easting_x0  - my_easting_x;
						// Y0 = (northing_y0  - my_northing_y) / koeff_masshtab_y;
						Y0 = northing_y0  - my_northing_y;
																							
						ern::Log::getInstance().Write("//-------------------------"); 
						sprintf(info, "X0 = %9.5f;  Y0 = %9.5f;", X0, Y0);
						ern::Log::getInstance().Write(info);
						//-------------------------------------
						// X1 = (easting_x1  - my_easting_x) / koeff_masshtab_x;
						X1 = easting_x1  - my_easting_x;
						// Y1 = (northing_y1  - my_northing_y) / koeff_masshtab_y;
						Y1 = northing_y1  - my_northing_y;
																							
						ern::Log::getInstance().Write("//-------------------------"); 
						sprintf(info, "X1 = %9.5f;  Y1 = %9.5f;", X1, Y1);
						ern::Log::getInstance().Write(info);		
						//-----------------------------------			
						// X2 = (easting_x2  - my_easting_x) / koeff_masshtab_x;
						X2 = easting_x2  - my_easting_x;
						// Y2 = (northing_y2  - my_northing_y) / koeff_masshtab_y;
						Y2 = northing_y2  - my_northing_y;
																							
						ern::Log::getInstance().Write("//-------------------------"); 
						sprintf(info, "X2 = %9.5f;  Y2 = %9.5f;", X2, Y2);
						ern::Log::getInstance().Write(info);		
						//-----------------------------------
						Sum_dX = sqrt( (pow(Etalon_X0 - X0, 2) + pow(Etalon_X1 - X1, 2) + pow(Etalon_X2 - X2, 2) ) / 3.0 );				
						Sum_dY = sqrt( (pow(Etalon_Y0 - Y0, 2) + pow(Etalon_Y1 - Y1, 2) + pow(Etalon_Y2 - Y2, 2) ) / 3.0 );		
						Sum = Sum_dX + Sum_dY;	// + dH;
						
						if(Sum < Sum_opt)
						{
							Sum_opt = Sum;
							MyLON_opt = MyLON_;
							MyLAT_opt = MyLAT_;
							// koeff_masshtab_x_opt = koeff_masshtab_x;
							// koeff_masshtab_y_opt = koeff_masshtab_y;									
						}

						// number =  t + 100 * k + 2000 * j + i * 40000;
						number =  j + i * 200;

						sprintf(info, "number = %10.2f; Sum_dX = %9.5f;  Sum_dY = %9.5f;	Sum =	%9.5f;	Sum_opt = %9.5f;", number, Sum_dX, Sum_dY, Sum, Sum_opt);
						ern::Log::getInstance().Write(info);

						// sprintf(info, "MyLON_ = %11.8f;  MyLAT_ = %11.8f;	koeff_masshtab_x = %11.8f;	koeff_masshtab_y =	%11.8f;	", MyLON_, MyLAT_, koeff_masshtab_x, koeff_masshtab_y);
						sprintf(info, "MyLON_ = %11.8f;  MyLAT_ = %11.8f;	koeff_masshtab_x = %11.8f;	koeff_masshtab_y =	%11.8f;	", MyLON_, MyLAT_, koeff_masshtab_x, koeff_masshtab_y);
						ern::Log::getInstance().Write(info);

						// sprintf(info, "MyLON_opt = %11.8f;  MyLAT_opt = %11.8f;	koeff_masshtab_x_opt = %11.8f;	koeff_masshtab_y_opt =	%11.8f;", MyLON_opt, MyLAT_opt, koeff_masshtab_x_opt, koeff_masshtab_y_opt);
						sprintf(info, "MyLON_opt = %11.8f;  MyLAT_opt = %11.8f;	koeff_masshtab_x_opt = %11.8f;	koeff_masshtab_y_opt =	%11.8f;", MyLON_opt, MyLAT_opt, koeff_masshtab_x, koeff_masshtab_y);
						ern::Log::getInstance().Write(info);
						//---------------------------------------
						res = 1;								
				// }						
			// }			
		}
	}
}
//------------------------------------------------
// тестирование перевода из одной системы координат в другую (поиск оптимальных коэффициентов)
void testTransitionToAnotherCoordinateSystem()
{
	int i, j, k, t, n, v, s;
	double X0, Y0, X1, Y1, X2, Y2, H0, H1, H2, Lat0, Lat1, Lat2, Lon0, Lon1, Lon2, hight0, hight1, hight2, dX, dY, dZ, Sum_opt, number;
	double dX0, dY0, dZ0, Wx0, Wx, Wy0, Wy, Wz0, Wz, m0, m, Wx_opt, Wy_opt, Wz_opt, m_opt, dX_opt, dY_opt, dZ_opt, Sum; 
	double res, dLat, dLon, dH, Etalon_Lat0, Etalon_Lon0, Etalon_H0, Etalon_Lat1, Etalon_Lon1, Etalon_H1, Etalon_Lat2, Etalon_Lon2, Etalon_H2;
	char info[140];

	double *pX;
	double *pY; 
	double *pH;

	TWgsPoint wgs_point_0;
	TWgsPoint wgs_point_1;

	GeoCalc geo_calc;

	// инициализируем координаты в метрах
	X0 = 0;
	Y0 = 0;
	X1 = 0;
	Y1 = 0;
	X2 = 0;
	Y2 = 0;
	H0 = 0;
	H1 = 0;
	H2 = 0;
	dX0 = -0.035;
	
	dX = 0.0;
	dY = 0.0;
	dY0 = -0.275;
	
	dZ = 0.0;
	dZ0 = -0.93;
	
	res = 0;
	hight0 = 22.25;
	hight1 = 19.5;
	hight2 = 22.0;
	Wx = 0.0;
	Wx0 = -0.00000005;
	
	Wy = 0.0;
	Wy0 = -0.00000005;			  
	
	Wz = 0.0;
	// Wz0 = -0.00000012;
	Wz0 = -0.0000012;
	
	m = 0.0;
	//   m = 0.000001;
	m0 = 0.0000005;
	// m0 = 0.00000005;
	// m0 = 0.00000001;	
	
	Wx_opt = 0.0;
	Wy_opt = 0.0;
	Wz_opt = 0.0;
	m_opt = 0.0;
	dX_opt = 0.0;
	dY_opt = 0.0;
	dZ_opt = 0.0;

	Sum_opt = 100000.0;
	Sum = 100000.0;

	// Зададим географические координаты трех точек (в градусах) в системе П3-90.02
	// и их эталоны в системе WGS-84
	TTargetLocationInMinutes targetLocationInMinutes;
	TTargetLocationInMinutes *pTargetLocationInMinutes;
	pTargetLocationInMinutes = &targetLocationInMinutes;
	// memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	// точка 0 - референсная точка (как задано у Лаврентия, и Д.П. сделал рядом замеры)
	// зададим широту и долготу нулевой точки в десятичных дробях в системе П3-90.02
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 47;
	pTargetLocationInMinutes->seconds = 46.56;
	Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 15;
	pTargetLocationInMinutes->seconds = 45.00;
	Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	// зададим эталоны нулевой точки в системе WGS-84
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 47;
	pTargetLocationInMinutes->seconds = 47.66;
	Etalon_Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 15;
	pTargetLocationInMinutes->seconds = 43.84;
	Etalon_Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	Etalon_H0 = 24.0;
	//----------------
	// точка 1- чуть севернее и чуть западнее контрольной точки аэропорта 
	// угол перрона возле стоящего рядом здания

	// зададим широту и долготу первой точки в десятичных дробях в системе П3-90.02
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 48;
	pTargetLocationInMinutes->seconds = 7.68;
	Lat1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 15;
	pTargetLocationInMinutes->seconds = 40.8;
	Lon1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	// зададим эталоны первой точки в системе WGS-84
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 48;
	pTargetLocationInMinutes->seconds = 7.97;
	Etalon_Lat1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 15;
	pTargetLocationInMinutes->seconds = 40.14;
	Etalon_Lon1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	// Etalon_H1 = 21.8;	// (центр ВПП2)
	Etalon_H1 = 24.0;
	//----------------
	// точка 2- самый западный поворот на ВПП1 с параллельной рулежной дорожки  
	// центр рулежной дорожки на повороте

	// зададим широту и долготу второй точки в десятичных дробях в системе П3-90.02
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 48;
	pTargetLocationInMinutes->seconds = 4.08;
	Lat2 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 13;
	pTargetLocationInMinutes->seconds = 13.2;
	Lon2 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	// зададим эталоны второй точки в системе WGS-84
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 48;
	pTargetLocationInMinutes->seconds = 4.17;
	Etalon_Lat2 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 13;
	pTargetLocationInMinutes->seconds = 16.54;
	Etalon_Lon2 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	// Etalon_H2 = 24.0;	// (центр ВПП2)
	Etalon_H2 = 20.6;

	pX = &X0;
	pY = &Y0;	
	pH = &H0;
	
	double *pLat;
	double *pLon;
	double LonWGS_84_0 = 0;
	double LatWGS_84_0 = 0;
	double LonWGS_84_1 = 0;
	double LatWGS_84_1 = 0;
	double LonWGS_84_2 = 0;
	double LatWGS_84_2 = 0;

	pLat = &LatWGS_84_0;
	pLon = &LonWGS_84_0;

	////// переход в WGS-84
	
	 for(i = 0; i < 10; i++)
	// for(i = 0; i < 5; i++)
	// int GeoCalc::P3_90_02_P3_90_11_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
	// if(geo_calc.P3_90_02_WGS_84(Lat0, Lon0, hight0, pH, pLat, pLon))
	{
		Wx = Wx0 + 0.00000001 * i;
		// Wx = Wx0 + 0.000001 * i;
		// Wx = Wx0 + 0.000002 * i;
		// Wx = Wx0 + 0.000005 * i;
		// Wx = Wx0 + 0.00001 * i;
		for(j = 0; j < 10; j++)
		{
			Wy = Wy0 +  0.00000001 * j;
			// Wy = Wy0 + 0.000001 * j;
			// Wy = Wy0 + 0.000002 * j;
			// Wy = Wy0 + 0.000005 * j;
			// Wy = Wy0 + 0.00001 * j;
			for(k = 0; k < 10; k++)
			//  for(k = 0; k < 5; k++)
			{
				Wz = Wz0 + 0.0000001 * k;	// 0.00000002; -0.0000008;
				// Wz = Wz0 + 0.000001 * k;
				// Wz = Wz0 + 0.000002 * k;
				// Wz = Wz0 + 0.000005 * k;
				// Wz = Wz0 + 0.00001 * k;
				// for(t = 0; t < 10; t++)
				for(t = 0; t < 10; t++)
				{
					// m = m0 + 0.00000001 * t;	
					m = m0 + 0.0000001 * t;	
					// m = m0 + 0.000001 * t;
					// m = m0 + 0.000002 * t;
					// m = m0 + 0.000005 * t;
					// m = m0 + 0.00001 * t;
					for(n = 0; n < 10; n++)
					{
						dX = dX0 + 0.001 * n;
						// dX = dX0 + 0.002 * n;
						// dX = dX0 + 0.01 * n;
						// dX = dX0 + 0.1 * n;
						// dX = dX0 + 0.5 * n;
						// dX = dX0 + 1 * n;
						// dX = dX0 + 2 * n;
						for(v = 0; v < 10; v++)
						{
							dY = dY0 + 0.001 * v;
							// dY = dY0 + 0.002 * v;
							// dY = dY0 + 0.01 * v;
							// dY = dY0 + 0.1 * v;
							// dY = dY0 + 0.2 * v;
							// dY = dY0 + 0.5 * v;
							// dY = dY0 + 1 * v;
							for(s = 0; s < 10; s++)
							// for(s = 0; s < 5; s++)
							{
								dZ = dZ0 + 0.002 * s;
								// dZ = dZ0 + 0.01 * s;
								// dZ = dZ0 + 0.1 * s;
								// dZ = dZ0 + 0.2 * s;
								// dZ = dZ0 + 0.5 * s;
								// dZ = dZ0 + 1 * s;

								LonWGS_84_0 = 0;
								LatWGS_84_0 = 0;
								pLat = &LatWGS_84_0;
								pLon = &LonWGS_84_0;
								H0 = 0;
								pH = &H0;
								if(geo_calc.P3_90_02_WGS_84(Lat0, Lon0, hight0, dX, dY, dZ, Wx, Wy, Wz, m, pH, pLat, pLon))
								{
									ern::Log::getInstance().Write("//-------------------------"); 
									sprintf(info, "LatWGS_84_0 = %11.8f;  LonWGS_84_0 = %11.8f;	H0 = %11.4f;", LatWGS_84_0, LonWGS_84_0, H0);
									ern::Log::getInstance().Write(info);

									memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
									geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatWGS_84_0);
									
									sprintf(info, "deg_lat0 = %d;  min_x_lat0 = %d;	sec_x_lat0 = %9.6f;", pTargetLocationInMinutes->degrees, 
										pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
									ern::Log::getInstance().Write(info);
									////-----------------------------
									memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
									geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonWGS_84_0);

									sprintf(info, "deg_lon0 = %d;  min_x_lon0 = %d;	sec_x_lon0 = %9.6f;", pTargetLocationInMinutes->degrees, 
										pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
									ern::Log::getInstance().Write(info);
								}
								////------------------------------------------------------------
								pX = &X1;
								pY = &Y1;
								LonWGS_84_1 = 0;
								LatWGS_84_1 = 0;
								pLat = &LatWGS_84_1;
								pLon = &LonWGS_84_1;
								H1 = 0;
								pH = &H1;

								// if(geo_calc.P3_90_02_WGS_84(Lat1, Lon1, hight1, pH, pLat, pLon))
								if(geo_calc.P3_90_02_WGS_84(Lat1, Lon1, hight1, dX, dY, dZ, Wx, Wy, Wz, m, pH, pLat, pLon))
								{
									sprintf(info, "LatWGS_84_1 = %11.8f;  LonWGS_84_1 = %11.8f;	H1 = %11.4f;", LatWGS_84_1, LonWGS_84_1, H1);
									ern::Log::getInstance().Write(info);

									memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
									geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatWGS_84_1);
									
									sprintf(info, "deg_lat1 = %d;  min_x_lat1 = %d;	sec_x_lat1 = %9.6f;", pTargetLocationInMinutes->degrees, 
										pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
									ern::Log::getInstance().Write(info);
									////-----------------------------
									memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
									geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonWGS_84_1);

									sprintf(info, "deg_lon1 = %d;  min_x_lon1 = %d;	sec_x_lon1 = %9.6f;", pTargetLocationInMinutes->degrees, 
										pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
									ern::Log::getInstance().Write(info);
								}
								//////-----------------------------------
								pX = &X2;
								pY = &Y2;
								LonWGS_84_2 = 0;
								LatWGS_84_2 = 0;
								pLat = &LatWGS_84_2;
								pLon = &LonWGS_84_2;
								H2 = 0;
								pH = &H2;

								// if(geo_calc.P3_90_02_WGS_84(Lat2, Lon2, hight2, pH, pLat, pLon))
								if(geo_calc.P3_90_02_WGS_84(Lat2, Lon2, hight2, dX, dY, dZ, Wx, Wy, Wz, m, pH, pLat, pLon))
								{
									
									sprintf(info, "LatWGS_84_2 = %11.8f;  LonWGS_84_2 = %11.8f;	H2 = %11.4f;", LatWGS_84_2, LonWGS_84_2, H2);
									ern::Log::getInstance().Write(info);

									memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
									geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatWGS_84_2);
									
									sprintf(info, "deg_lat2 = %d;  min_x_lat2 = %d;	sec_x_lat2 = %9.6f;", pTargetLocationInMinutes->degrees, 
										pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
									ern::Log::getInstance().Write(info);
									////-----------------------------
									memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
									geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonWGS_84_2);

									sprintf(info, "deg_lon2 = %d;  min_x_lon2 = %d;	sec_x_lon2 = %9.6f;", pTargetLocationInMinutes->degrees, 
										pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
									ern::Log::getInstance().Write(info);
								}
								//---------------------------------------
								dLat = sqrt( pow(Etalon_Lat0 - LatWGS_84_0, 2) + pow(Etalon_Lat1 - LatWGS_84_1, 2) + pow(Etalon_Lat2 - LatWGS_84_2, 2) );				// / 0.000009163;
								dLon = sqrt( pow(Etalon_Lon0 - LonWGS_84_0, 2) + pow(Etalon_Lon1 - LonWGS_84_1, 2) + pow(Etalon_Lon2 - LonWGS_84_2, 2) );		// / 0.000009163;
								dH = sqrt(pow(Etalon_H0 - H0, 2) + pow(Etalon_H1 - H1, 2) + pow(Etalon_H2 - H2, 2) ) / 109134.563;

								Sum = dLat + dLon + dH;
								if(Sum < Sum_opt)
								{
									Sum_opt = Sum;
									Wx_opt = Wx;
									Wy_opt = Wy;
									Wz_opt = Wz;
									m_opt = m;
									dX_opt = dX;
									dY_opt = dY;
									dZ_opt = dZ;
								}

								number = s + 10 * v + 100 * n + 1000 * t + 10000 * k + 100000 * j + i * 1000000;

								sprintf(info, "number = %10.2f; dLat = %12.8f;  dLon = %12.8f;	dH = %12.8f;	Sum =	%14.8f;		Sum_opt = %14.8f;", number, dLat, dLon, dH, Sum, Sum_opt);
								ern::Log::getInstance().Write(info);

								sprintf(info, "Wx = %11.8f;  Wy = %11.8f;	Wz = %11.8f;	m =	%11.8f;		dX = %11.7f;		dY = %11.7f;	dZ = %11.7f;", Wx, Wy, Wz, m, dX, dY, dZ);
								ern::Log::getInstance().Write(info);

								sprintf(info, "Wx_opt = %11.8f;  Wy_opt = %11.8f;	Wz_opt = %11.8f;	m_opt =	%11.8f;		dX_opt = %11.7f;	dY_opt = %11.7f;	dZ_opt = %11.7f;", Wx_opt, Wy_opt, Wz_opt, m_opt, dX_opt, dY_opt, dZ_opt);
								ern::Log::getInstance().Write(info);
								//---------------------------------------
								res = 1;
								
							}
						}
					}
				}
			}
		}
	}
}
//-------------------------------------------------
// тестирование точного алгоритма перевода из систем П3-90.02, WGS-84 и СК-42 в плоскую проекцию Гаусса или Меркатора
// определение наиболее точного способа перехода к плоским координатам
void testCalcPrCoord()
{
	int i, j, k, t, n, v, s;
	double X0, Y0, X1, Y1, X2, Y2, H0, H1, H2, Lat0, Lat1, Lat2, Lon0, Lon1, Lon2, hight0, hight1, hight2, dX, dY, dZ, Sum_opt, number;
	double dX0, dY0, dZ0, Wx0, Wx, Wy0, Wy, Wz0, Wz, m0, m, Wx_opt, Wy_opt, Wz_opt, m_opt, dX_opt, dY_opt, dZ_opt, Sum; 
	double res, dLat, dLon, dH, Etalon_Lat0, Etalon_Lon0, Etalon_H0, Etalon_Lat1, Etalon_Lon1, Etalon_H1, Etalon_Lat2, Etalon_Lon2, Etalon_H2;
	char info[140];

	double *pX;
	double *pY; 
	double *pH;

	TWgsPoint wgs_point_0;
	TWgsPoint wgs_point_1;

	// TSK42Point sk42_point_0;
	// TSK42Point sk42_point_1;

	GeoCalc geo_calc;

	// инициализируем координаты в метрах
	X0 = 0;
	Y0 = 0;
	X1 = 0;
	Y1 = 0;
	
	H0 = 0;
	H1 = 0;
	H2 = 0;
		
	res = 0;
	hight0 = 22.25;
	hight1 = 19.5;
	// hight2 = 22.0;
	
	// проанализируем варианты переходов
	// Задача
	// Дано: координаты референсной точки (центр ВПП1) в системе П3-90.02 в градусах
	// Lat0 = 59.477,		Lon0 = 30.1502, высота над поверхностью эллипсоида hight0 = 22.25 в метрах;
	// и координаты центра ВПП2 в системе П3-90.02 в градусах
	// Lat1 = 59.4832,		Lon1 = 30.1652, высота над поверхностью эллипсоида hight1 = 19.5 в метрах;
	// известно, что в плоской системе расстояние от координаты X0 до координаты X1 примерно 1380 метров,
	// а расстояние от координаты Y0 до координаты Y1 соответственно 1160 метров
	// требуется найти оптимальный способ перехода, чтобы погрешность была минимальной

	// Вариант №2
	// п.1 переход от П3-90.02 в WGS-84 (делается oдна итерация)
	// п.2 переход из системы WGS-84 к проекции Гаусса


	// Географические координаты точки (в градусах) в системе П3-90.02
	//Lat0 = 59.477;
	//Lon0 = 30.1502;
	////----------------
	//Lat1 = 59.4832;
	//Lon1 = 30.1652;

	//pX = &X0;
	//pY = &Y0;	

	//// переход в WGS-84
	/*wgs_point_0.wgs_Y_lat = geo_calc.P3_90_02_WGS_84_Lat(Lat0, Lon0, hight0);
	wgs_point_0.wgs_X_lon = geo_calc.P3_90_02_WGS_84_Lon(Lat0, Lon0, hight0);
	wgs_point_1.wgs_Y_lat = geo_calc.P3_90_02_WGS_84_Lat(Lat1, Lon1, hight1);
	wgs_point_1.wgs_X_lon = geo_calc.P3_90_02_WGS_84_Lon(Lat1, Lon1, hight1);

	WgsToLocPr(wgs_point_0.wgs_Y_lat, wgs_point_0.wgs_X_lon, pX, pY);

	sprintf(info, "X0 =  %11.4f\n", X0);
	ern::Log::getInstance().Write(info);

	sprintf(info, "Y0 =  %11.4f\n", Y0);
	ern::Log::getInstance().Write(info);*/
	////-----------------------------
	/*pX = &X1;
	pY = &Y1;

	WgsToLocPr(wgs_point_1.wgs_Y_lat, wgs_point_1.wgs_X_lon, pX, pY);

	sprintf(info, "X1 =  %11.4f\n", X1);
	ern::Log::getInstance().Write(info);

	sprintf(info, "Y1 =  %11.4f\n", Y1);
	ern::Log::getInstance().Write(info);*/
	//-----------------------------------
	/*dX = X1 - X0;
	dY = Y1 - Y0;

	sprintf(info, "dX =  %11.4f\n", dX);
	ern::Log::getInstance().Write(info);

	sprintf(info, "dY =  %11.4f\n", dY);
	ern::Log::getInstance().Write(info);*/

	// результаты варианта 2
	// WGS_LON_0 = 33.366;			// ошибка
	// WGS_LAT_0 = 53.935;			// ошибка

	// WGS_LON_1 = 33.381;			// ошибка
	// WGS_LAT_1 = 53.9429;			// ошибка

	//  X0 =  5654685.1175
	//  Y0 =  5981456.5115

	//  X1 =  5655644.2694
	//  Y1 =  5982274.6643

	//  dX =     959.1519
	//  dY =     818.1529							// отклонение 250-300 метров
	
	// итог: неудовлетворительно
	// вывод: точные методы преобразования geo_calc.P3_90_02_WGS_84_Lat(...)  и  geo_calc.P3_90_02_WGS_84_Lon(...)
	// нужно реализовывать самостоятельно, через переход к пространственным координатам X, Y, Z
	///////////////////////////////////////////////////////////
	//----------------------------------
	// Вариант №3
	// п.1 переход от П3-90.02 в WGS-84 (делается через переход к пространственным координатам)
	// п.2 переход из системы WGS-84 к проекции Гаусса

	
	// Географические координаты точки (в градусах) в системе П3-90.02
	// Lat0 =	59.79333;		//	59-47-36	(59 градусов, 47 минут, 36 секунд)
	// Lon0 = 30.250333;	// 30-15-1.2	(30 градусов, 15 минут, 1.2 секунды)
	//TTargetLocationInMinutes targetLocationInMinutes;
	//TTargetLocationInMinutes *pTargetLocationInMinutes;
	//pTargetLocationInMinutes = &targetLocationInMinutes;
	//// memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	//// точка 0 - центр ВПП1
	//// зададим широту и долготу нулевой точки в десятичных дробях в системе П3-90.02
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 47;
	//pTargetLocationInMinutes->seconds = 36;
	//Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 15;
	//pTargetLocationInMinutes->seconds = 1.2;
	//Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//// Etalon_H0 = 24.0;
	////----------------
	//// Lat1 = 59.4832;				// (центр ВПП2)
	//// Etalon_Lat1 = 59.805453;		// (центр ВПП2)
	//// Lon1 = 30.1652;				// (центр ВПП2)
	//// Etalon_Lon1 = 30.274379;		// (центр ВПП2)

	//// точка 1- центр ВПП2 
	//// угол перрона возле стоящего рядом здания

	//// зададим широту и долготу первой точки в десятичных дробях в системе П3-90.02
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 48;
	//pTargetLocationInMinutes->seconds = 19.0;
	//Lat1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 16;
	//pTargetLocationInMinutes->seconds = 31.0;
	//Lon1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//// Etalon_H1 = 21.8;	// (центр ВПП2)
	////----------------
	//pX = &X0;
	//pY = &Y0;	
	//pH = &H0;
	
	//double *pLat;
	//double *pLon;
	//double LonWGS_84_0 = 0;
	//double LatWGS_84_0 = 0;
	//double LonWGS_84_1 = 0;
	//double LatWGS_84_1 = 0;
	//// double LonWGS_84_2 = 0;
	//// double LatWGS_84_2 = 0;

	//pLat = &LatWGS_84_0;
	//pLon = &LonWGS_84_0;

	//////// переход в WGS-84
	//pX = &X0;
	//pY = &Y0;	
	//pH = &H0;
	
	
	////// переход в WGS-84
	
	// int GeoCalc::P3_90_02_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
	// if(geo_calc.P3_90_02_WGS_84(Lat0, Lon0, hight0, pH, pLat, pLon))
	//if(P3_90_02_WGS_84(Lat0, Lon0, hight0, pH, pLat, pLon))
	//{
	//	wgs_point_0.wgs_Y_lat = LatWGS_84_0;
	//	wgs_point_0.wgs_X_lon = LonWGS_84_0;

	//	WgsToLocPr(wgs_point_0.wgs_Y_lat, wgs_point_0.wgs_X_lon, pX, pY);
	//	 
	//	sprintf(info, "X0 =  %11.4f\n", X0);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "Y0 =  %11.4f\n", Y0);
	//	ern::Log::getInstance().Write(info);
	////	////-----------------------------
	////	pX = &X1;
	////	pY = &Y1;
	//}
	//pX = &X1;
	//pY = &Y1;
	////////------------------------------------------------------------
	//LonWGS_84_1 = 0;
	//LatWGS_84_1 = 0;
	//pLat = &LatWGS_84_1;
	//pLon = &LonWGS_84_1;
	//pH = &H1;

	// if(geo_calc.P3_90_02_WGS_84(Lat1, Lon1, hight1, pH, pLat, pLon))
	/*if(P3_90_02_WGS_84(Lat1, Lon1, hight1, pH, pLat, pLon))
	{
		wgs_point_1.wgs_Y_lat = LatWGS_84_1;
		wgs_point_1.wgs_X_lon = LonWGS_84_1;
		
		WgsToLocPr(wgs_point_1.wgs_Y_lat, wgs_point_1.wgs_X_lon, pX, pY);

		sprintf(info, "X1 =  %11.4f\n", X1);
		ern::Log::getInstance().Write(info);

		sprintf(info, "Y1 =  %11.4f\n", Y1);
		ern::Log::getInstance().Write(info);
	}*/
	////////-----------------------------------
	/*dX = X1 - X0;
	dY = Y1 - Y0;

	sprintf(info, "dX =  %11.4f\n", dX);
	ern::Log::getInstance().Write(info);

	sprintf(info, "dY =  %11.4f\n", dY);
	ern::Log::getInstance().Write(info);*/

	// результаты варианта 3
	// WGS_LON_0 = 30.15023;		// ошибка
	// WGS_LAT_0 = 59.47697;		// ошибка
	// H0 = 24.4957;

	// WGS_LON_1 = 30.165;			// ошибка
	// WGS_LAT_1 = 59.48317;		// ошибка
	// H1 = 21.758;

	// X0 =  6345642.7845
	// Y0 =  6634362.9827

	// X1 =  6347097.2953
	// Y1 =  6635635.1399

	// dX =  1454.5108
	// dY =  1272.1572								// отклонение 100-120 метров
	
	// итог: неудовлетворительно
	// вывод: переход на основе указанных коэффициентов из ПЗ-90.02 в WGS-84 неэффективен

	////////////////////////////////////////////////////////
	// Вариант №4
	// п.1 переход от П3-90.02 к СК-42 (через переход к пространственным координатам)
	// п.2 переход из системы СK-42 к проекции Гаусса
	
	//// Географические координаты точки (в градусах) в системе П3-90.02
	////Lat0 = 59.477;
	////Lon0 = 30.1502;
	////////----------------
	////Lat1 = 59.4832;
	////Lon1 = 30.1652;

	////pX = &X0;
	////pY = &Y0;
	//// Географические координаты точки (в градусах) в системе П3-90.02
	//// Lat0 =	59.79333;		//	59-47-36	(59 градусов, 47 минут, 36 секунд)
	//// Lon0 = 30.250333;	// 30-15-1.2	(30 градусов, 15 минут, 1.2 секунды)
	//TTargetLocationInMinutes targetLocationInMinutes;
	//TTargetLocationInMinutes *pTargetLocationInMinutes;
	//pTargetLocationInMinutes = &targetLocationInMinutes;
	//// memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	//// точка 0 - центр ВПП1
	//// зададим широту и долготу нулевой точки в десятичных дробях в системе П3-90.02
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 47;
	//pTargetLocationInMinutes->seconds = 36;
	//Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 15;
	//pTargetLocationInMinutes->seconds = 1.2;
	//Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//// Etalon_H0 = 24.0;
	////----------------
	//// Lat1 = 59.4832;				// (центр ВПП2)
	//// Etalon_Lat1 = 59.805453;		// (центр ВПП2)
	//// Lon1 = 30.1652;				// (центр ВПП2)
	//// Etalon_Lon1 = 30.274379;		// (центр ВПП2)

	//// точка 1- центр ВПП2 
	//// угол перрона возле стоящего рядом здания

	//// зададим широту и долготу первой точки в десятичных дробях в системе П3-90.02
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 48;
	//pTargetLocationInMinutes->seconds = 19.0;
	//Lat1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 16;
	//pTargetLocationInMinutes->seconds = 31.0;
	//Lon1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//////// переход в СК-42
	//pX = &X0;
	//pY = &Y0;	
	//pH = &H0;
	//
	//double *pLat;
	//double *pLon;
	//double LonSK_42_0 = 0;
	//double LatSK_42_0 = 0;
	//double LonSK_42_1 = 0;
	//double LatSK_42_1 = 0;
	//
	//pLat = &LatSK_42_0;
	//pLon = &LonSK_42_0;

	//	
	//if(geo_calc.P3_90_02_SK42(Lat0, Lon0, hight0, pH, pLat, pLon))
	//{
	//	sk42_point_0.sk42_Y_lat = LatSK_42_0;
	//	sk42_point_0.sk42_X_lon = LonSK_42_0;

	//	geo_calc.SK42ToPrGaus(sk42_point_0.sk42_Y_lat, sk42_point_0.sk42_X_lon, pX, pY);
	//	 
	//	sprintf(info, "X0 =  %11.4f\n", X0);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "Y0 =  %11.4f\n", Y0);
	//	ern::Log::getInstance().Write(info);
	////	////-----------------------------
	////	pX = &X1;
	////	pY = &Y1;
	//}
	//pX = &X1;
	//pY = &Y1;
	////////------------------------------------------------------------
	//LonSK_42_1 = 0;
	//LatSK_42_1 = 0;
	//pLat = &LatSK_42_1;
	//pLon = &LonSK_42_1;
	//pH = &H1;

	//// if(geo_calc.P3_90_02_WGS_84(Lat1, Lon1, hight1, pH, pLat, pLon))
	//if(geo_calc.P3_90_02_SK42(Lat1, Lon1, hight1, pH, pLat, pLon))
	//{
	//	sk42_point_1.sk42_Y_lat = LatSK_42_1;
	//	sk42_point_1.sk42_X_lon = LonSK_42_1;
	//	
	//	geo_calc.SK42ToPrGaus(sk42_point_1.sk42_Y_lat, sk42_point_1.sk42_X_lon, pX, pY);

	//	sprintf(info, "X1 =  %11.4f\n", X1);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "Y1 =  %11.4f\n", Y1);
	//	ern::Log::getInstance().Write(info);
	//}
	//////////-----------------------------------
	//dX = X1 - X0;
	//dY = Y1 - Y0;

	//sprintf(info, "dX =  %11.4f\n", dX);
	//ern::Log::getInstance().Write(info);

	//sprintf(info, "dY =  %11.4f\n", dY);
	//ern::Log::getInstance().Write(info);

	// результаты варианта 4
	
	//  X0 =  6634346.6089
	//  Y0 =  495737.4534

	//  X1 =  6635618.7225
	//  Y1 =  497191.9465

	//  dX =   1272.1136
	//  dY =    1454.4931			// отклонение 100-120 метров,  результаты для dX и  dY перепутаны между собой
											// должно быть наоборот (!)
	
	// итог:	результат неэффективный, но радует, что переход в СК-42 правильный, 
	// проекция Гаусса-Крюгера расчитывается правильно двумя способами, результат приблизительно одинаков!
	// становится очевидно, что проекция Гаусса-Крюгера не решает проблему

	// Вариант №5
	// п.1 переход от П3-90.02 к WGS-84 (через переход к пространственным координатам)
	// п.2 переход из системы WGS-84 к проекции Меркатора (эллипс)

	//TTargetLocationInMinutes targetLocationInMinutes;
	//TTargetLocationInMinutes *pTargetLocationInMinutes;
	//pTargetLocationInMinutes = &targetLocationInMinutes;
	//
	//// точка 0 - центр ВПП1
	//// зададим широту и долготу нулевой точки в десятичных дробях в системе П3-90.02
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 47;
	//pTargetLocationInMinutes->seconds = 36;
	//Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 15;
	//pTargetLocationInMinutes->seconds = 1.2;
	//Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	////// точка 1- центр ВПП2 
	////// угол перрона возле стоящего рядом здания

	////// зададим широту и долготу первой точки в десятичных дробях в системе П3-90.02
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 48;
	//pTargetLocationInMinutes->seconds = 19.0;
	//Lat1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 16;
	//pTargetLocationInMinutes->seconds = 31.0;
	//Lon1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//////----------------
	//double *pLat;
	//double *pLon;
	//double LonWGS_84_0 = 0;
	//double LatWGS_84_0 = 0;
	//double LonWGS_84_1 = 0;
	//double LatWGS_84_1 = 0;
	//
	//pLat = &LatWGS_84_0;
	//pLon = &LonWGS_84_0;

	////////// переход в WGS-84
	//pX = &X0;
	//pY = &Y0;	
	//pH = &H0;
	//
	//if(P3_90_02_WGS_84(Lat0, Lon0, hight0, pH, pLat, pLon))
	//{
	//	wgs_point_0.wgs_Y_lat = LatWGS_84_0;
	//	wgs_point_0.wgs_X_lon = LonWGS_84_0;

	//	//*pY = geo_calc. WgsToPrMercEllipse_Y(wgs_point_0.wgs_Y_lat);
	//	//*pX = geo_calc.WgsToPrMercEllipse_X(wgs_point_0.wgs_X_lon);
	//   geo_calc.LatLong2Merc(wgs_point_0.wgs_X_lon, wgs_point_0.wgs_Y_lat, pX, pY); 
	//	 
	//	sprintf(info, "X0 =  %11.4f\n", X0);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "Y0 =  %11.4f\n", Y0);
	//	ern::Log::getInstance().Write(info);
	//	////-----------------------------	
	//}
	//pX = &X1;
	//pY = &Y1;
	//////////------------------------------------------------------------
	//LonWGS_84_1 = 0;
	//LatWGS_84_1 = 0;
	//pLat = &LatWGS_84_1;
	//pLon = &LonWGS_84_1;
	//pH = &H1;

	//if(P3_90_02_WGS_84(Lat1, Lon1, hight1, pH, pLat, pLon))
	//{
	//	wgs_point_1.wgs_Y_lat = LatWGS_84_1;
	//	wgs_point_1.wgs_X_lon = LonWGS_84_1;
	//	
	//	// *pY = geo_calc. WgsToPrMercEllipse_Y(wgs_point_1.wgs_Y_lat);
	//	// *pX = geo_calc.WgsToPrMercEllipse_X(wgs_point_1.wgs_X_lon);
	// geo_calc.LatLong2Merc(wgs_point_1.wgs_X_lon, wgs_point_1.wgs_Y_lat, pX, pY); 

	//	sprintf(info, "X1 =  %11.4f\n", X1);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "Y1 =  %11.4f\n", Y1);
	//	ern::Log::getInstance().Write(info);
	//}
	//////////-----------------------------------
	//dX = X1 - X0;
	//dY = Y1 - Y0;

	//sprintf(info, "dX =  %11.4f\n", dX);
	//ern::Log::getInstance().Write(info);

	//sprintf(info, "dY =  %11.4f\n", dY);
	//ern::Log::getInstance().Write(info);

	// результаты варианта 5	
	//	 X0 =  3367458.7402;
	//  Y0 =  8316901.8746

	//  X1 =  3370235.5433
	//  Y1 =  8319540.6443

	//  dX =    2776.8031
	//  dY =    2638.7696
	// результат:	результат неприемлемый, очевидно, имеет место быть какая-то ошибка 
	// нужно перепроверять и переделывать....
////////////////////////////////////////////////////////////////////////////////////////

	// Вариант №6
	// п.1 переход от П3-90.02 к WGS-84 (через переход к пространственным координатам)
	// п.2 переход из системы WGS-84 к проекции Меркатора (сфера)

	//TTargetLocationInMinutes targetLocationInMinutes;
	//TTargetLocationInMinutes *pTargetLocationInMinutes;
	//pTargetLocationInMinutes = &targetLocationInMinutes;
	////// memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	//// точка 0 - центр ВПП1
	//// зададим широту и долготу нулевой точки в десятичных дробях в системе П3-90.02
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 47;
	//pTargetLocationInMinutes->seconds = 36;
	//Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 15;
	//pTargetLocationInMinutes->seconds = 1.2;
	//Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	// Etalon_H0 = 24.0;
	////----------------
	// Lat1 = 59.4832;				// (центр ВПП2)
	// Etalon_Lat1 = 59.805453;		// (центр ВПП2)
	// Lon1 = 30.1652;				// (центр ВПП2)
	// Etalon_Lon1 = 30.274379;		// (центр ВПП2)

	//// точка 1- центр ВПП2 
	//// угол перрона возле стоящего рядом здания

	//// зададим широту и долготу первой точки в десятичных дробях в системе П3-90.02
	/*pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 48;
	pTargetLocationInMinutes->seconds = 19.0;
	Lat1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 16;
	pTargetLocationInMinutes->seconds = 31.0;
	Lon1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);*/

	//// Etalon_H1 = 21.8;	// (центр ВПП2)
	////----------------
	//double *pLat;
	//double *pLon;
	//double LonWGS_84_0 = 0;
	//double LatWGS_84_0 = 0;
	//double LonWGS_84_1 = 0;
	//double LatWGS_84_1 = 0;
	//// double LonWGS_84_2 = 0;
	//// double LatWGS_84_2 = 0;

	//pLat = &LatWGS_84_0;
	//pLon = &LonWGS_84_0;

	////////// переход в WGS-84
	//pX = &X0;
	//pY = &Y0;	
	//pH = &H0;
	
	// int GeoCalc::P3_90_02_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
	// if(geo_calc.P3_90_02_WGS_84(Lat0, Lon0, hight0, pH, pLat, pLon))
	//if(P3_90_02_WGS_84(Lat0, Lon0, hight0, pH, pLat, pLon))
	//{
	//	wgs_point_0.wgs_Y_lat = LatWGS_84_0;
	//	wgs_point_0.wgs_X_lon = LonWGS_84_0;

	//	// WgsToLocPr(wgs_point_0.wgs_Y_lat, wgs_point_0.wgs_X_lon, pX, pY);
	//	*pY = geo_calc.WgsToPrMercDomain_Y(wgs_point_0.wgs_Y_lat);
	//	*pX = geo_calc.WgsToPrMercDomain_X(wgs_point_0.wgs_X_lon);
	//	// geo_calc.LatLong2Merc(wgs_point_0.wgs_X_lon, wgs_point_0.wgs_Y_lat, pX, pY); 
	//	 
	//	sprintf(info, "X0 =  %11.4f\n", X0);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "Y0 =  %11.4f\n", Y0);
	//	ern::Log::getInstance().Write(info);
	////	////-----------------------------
	////	pX = &X1;
	////	pY = &Y1;
	//}
	/*pX = &X1;
	pY = &Y1;*/
	////////------------------------------------------------------------
	//LonWGS_84_1 = 0;
	//LatWGS_84_1 = 0;
	//pLat = &LatWGS_84_1;
	//pLon = &LonWGS_84_1;
	//pH = &H1;

	//// if(geo_calc.P3_90_02_WGS_84(Lat1, Lon1, hight1, pH, pLat, pLon))
	//if(P3_90_02_WGS_84(Lat1, Lon1, hight1, pH, pLat, pLon))
	//{
	//	wgs_point_1.wgs_Y_lat = LatWGS_84_1;
	//	wgs_point_1.wgs_X_lon = LonWGS_84_1;
	//	
	//	// WgsToLocPr(wgs_point_1.wgs_Y_lat, wgs_point_1.wgs_X_lon, pX, pY);
	//	*pY = geo_calc.WgsToPrMercDomain_Y(wgs_point_1.wgs_Y_lat);
	//	*pX = geo_calc.WgsToPrMercDomain_X(wgs_point_1.wgs_X_lon);
	//	// geo_calc.LatLong2Merc(wgs_point_1.wgs_X_lon, wgs_point_1.wgs_Y_lat, pX, pY); 

	//	sprintf(info, "X1 =  %11.4f\n", X1);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "Y1 =  %11.4f\n", Y1);
	//	ern::Log::getInstance().Write(info);
	//}
	////////-----------------------------------
	/*dX = X1 - X0;
	dY = Y1 - Y0;

	sprintf(info, "dX =  %11.4f\n", dX);
	ern::Log::getInstance().Write(info);

	sprintf(info, "dY =  %11.4f\n", dY);
	ern::Log::getInstance().Write(info);*/

	// результаты варианта 6
	// X0 =  3367458.7402
	// Y0 =  8344511.8467

	//  X1 =  3370235.5433
	//  Y1 =  8347152.1574

	//  dX =    2776.8031
	//  dY =    2640.3107
	///////////////////////////////////////////////////////////////////////////////////////

	// Вариант №7
	// п.1 переход от П3-90.02 к WGS-84 (через переход к пространственным координатам)
	// п.2 переход из системы WGS-84 к поперечной проекции Меркатора (UTM)

	/*TTargetLocationInMinutes targetLocationInMinutes;
	TTargetLocationInMinutes *pTargetLocationInMinutes;
	pTargetLocationInMinutes = &targetLocationInMinutes;*/
	//// memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	// точка 0 - центр ВПП1
	// зададим широту и долготу нулевой точки в десятичных дробях в системе П3-90.02
	/*pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 47;
	pTargetLocationInMinutes->seconds = 36;
	Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 15;
	pTargetLocationInMinutes->seconds = 1.2;
	Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);*/

	// Etalon_H0 = 24.0;
	////----------------
	// Lat1 = 59.4832;				// (центр ВПП2)
	// Etalon_Lat1 = 59.805453;		// (центр ВПП2)
	// Lon1 = 30.1652;				// (центр ВПП2)
	// Etalon_Lon1 = 30.274379;		// (центр ВПП2)

	//// точка 1- центр ВПП2 
	//// угол перрона возле стоящего рядом здания

	//// зададим широту и долготу первой точки в десятичных дробях в системе П3-90.02
	/*pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 48;
	pTargetLocationInMinutes->seconds = 19.0;
	Lat1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 16;
	pTargetLocationInMinutes->seconds = 31.0;
	Lon1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);
*/
	//// Etalon_H1 = 21.8;	// (центр ВПП2)
	////----------------
	//double *pLat;
	//double *pLon;
	//double LonWGS_84_0 = 0;
	//double LatWGS_84_0 = 0;
	//double LonWGS_84_1 = 0;
	//double LatWGS_84_1 = 0;
	//// double LonWGS_84_2 = 0;
	//// double LatWGS_84_2 = 0;

	//pLat = &LatWGS_84_0;
	//pLon = &LonWGS_84_0;

	////////// переход в WGS-84
	//pX = &X0;
	//pY = &Y0;	
	//pH = &H0;
	//std::string  str_utm;
	
	// int GeoCalc::P3_90_02_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
	// if(geo_calc.P3_90_02_WGS_84(Lat0, Lon0, hight0, pH, pLat, pLon))
	//if(P3_90_02_WGS_84(Lat0, Lon0, hight0, pH, pLat, pLon))
	//{
	//	wgs_point_0.wgs_Y_lat = LatWGS_84_0;
	//	wgs_point_0.wgs_X_lon = LonWGS_84_0;

	//	// WgsToLocPr(wgs_point_0.wgs_Y_lat, wgs_point_0.wgs_X_lon, pX, pY);
	//	// *pY = geo_calc.WgsToPrMercDomain_Y(wgs_point_0.wgs_Y_lat);
	//	// *pX = geo_calc.WgsToPrMercDomain_X(wgs_point_0.wgs_X_lon);
	//	str_utm = geo_calc.LatLong2UTMMerc(wgs_point_0.wgs_X_lon, wgs_point_0.wgs_Y_lat, pX, pY); 
	//	 
	//	sprintf(info, "X0 =  %11.4f\n", X0);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "Y0 =  %11.4f\n", Y0);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "UTM0:  %s\n", str_utm.c_str() );
	//	ern::Log::getInstance().Write(info);
	////	////-----------------------------
	////	pX = &X1;
	////	pY = &Y1;
	//}
	/*pX = &X1;
	pY = &Y1;*/
	////////------------------------------------------------------------
	/*LonWGS_84_1 = 0;
	LatWGS_84_1 = 0;
	pLat = &LatWGS_84_1;
	pLon = &LonWGS_84_1;
	pH = &H1;*/

	// if(geo_calc.P3_90_02_WGS_84(Lat1, Lon1, hight1, pH, pLat, pLon))
	/*if(P3_90_02_WGS_84(Lat1, Lon1, hight1, pH, pLat, pLon))
	{
		wgs_point_1.wgs_Y_lat = LatWGS_84_1;
		wgs_point_1.wgs_X_lon = LonWGS_84_1;
		
		str_utm = geo_calc.LatLong2UTMMerc(wgs_point_1.wgs_X_lon, wgs_point_1.wgs_Y_lat, pX, pY); 

		sprintf(info, "X1 =  %11.4f\n", X1);
		ern::Log::getInstance().Write(info);

		sprintf(info, "Y1 =  %11.4f\n", Y1);
		ern::Log::getInstance().Write(info);

		sprintf(info, "UTM1:  %s\n", str_utm.c_str() );
		ern::Log::getInstance().Write(info);
	}*/
	////////-----------------------------------
	/*dX = X1 - X0;
	dY = Y1 - Y0;

	sprintf(info, "dX =  %11.4f\n", dX);
	ern::Log::getInstance().Write(info);

	sprintf(info, "dY =  %11.4f\n", dY);
	ern::Log::getInstance().Write(info);*/

	// результаты варианта 7
	// X0 =  345706.9657
	// Y0 =  6631593.4188
	// UTM0:  36 V 345707 6.63159e+006

	// X1 =  347160.8724
	//  Y1 =  6632865.0456
	//  UTM1:  36 V 347161 6.63287e+006

	// dX =    1453.9067
	// dY =    1271.6268
	////////////////////////////////////////////////////
	// Вариант №8
	// п.1 переход от поперечной проекции Меркатора (UTM) в систему географических координат WGS-84 (широта и долгота)									
	// п.2 переход из системы WGS-84 в систему П3-90.02

	//double *pLat; 
	//double *pLon;

	//TTargetLocationInMinutes targetLocationInMinutes;
	//TTargetLocationInMinutes *pTargetLocationInMinutes;
	//pTargetLocationInMinutes = &targetLocationInMinutes;

	//std::string long_zone = "36";
	//std::string lat_zone = "V";

	//X0 =  345706.9657;
	//Y0 = 6631593.4188;

	//double LatWGS_84_0 = 0;
	//pLat = &LatWGS_84_0;

	//double LonWGS_84_0 = 0;
	//pLon = &LonWGS_84_0;
	//
	//geo_calc.UTM2LatLongMerc(long_zone, lat_zone, X0, Y0, pLat, pLon);

	//wgs_point_0.wgs_Y_lat = LatWGS_84_0;
	//wgs_point_0.wgs_X_lon = LonWGS_84_0;

	//hight0 = 0;
	//pH = &hight0;

	//double LatP3_90_02_0 = 0;
	//pLat = &LatP3_90_02_0;

	//double LonP3_90_02_0 = 0;
	//pLon = &LonP3_90_02_0;

	//H0 = 24;

 //  if( WGS_84_P3_90_02(LatWGS_84_0, LonWGS_84_0, H0, pH, pLat, pLon) )
 //  {
	//	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
	//	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatP3_90_02_0);
	//								
	//	sprintf(info, "deg_lat0 = %d;  min_x_lat0 = %d;	sec_x_lat0 = %9.6f;", pTargetLocationInMinutes->degrees, 
	//									pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
	//	ern::Log::getInstance().Write(info);
	//	//------------------------
	//	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
	//	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonP3_90_02_0);
	//								
	//	sprintf(info, "deg_lon0 = %d;  min_x_lon0 = %d;	sec_x_lon0 = %9.6f;", pTargetLocationInMinutes->degrees, 
	//									pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
	//	ern::Log::getInstance().Write(info);
	//   
	//    // sprintf(info, "UTM0:  36 V 345706.9657 6.3159e+006\n", str_utm.c_str() );
	//	ern::Log::getInstance().Write("UTM0:  36 V 345706.9657 6.3159e+006\n");
 //  }

   // Результаты варианта №8
   // deg_lat0 = 59;  min_x_lat0 = 47;	sec_x_lat0 = 35.693221;
   // deg_lon0 = 30;  min_x_lon0 = 15;	sec_x_lon0 =  1.192182;
   // UTM0:  36 V 345706.9657 6.3159e+006
   // Итог: все Ok!
   ////////////////////////////////////////////////////////////////

   // Вариант №9
   // п.1 переход от WGS-84 к П3-90.02 (через переход к пространственным координатам)
   // точка соответствует левому верхнему углу контролируемой территории

	//TTargetLocationInMinutes targetLocationInMinutes;
	//TTargetLocationInMinutes *pTargetLocationInMinutes;
	//pTargetLocationInMinutes = &targetLocationInMinutes;
	////// memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	//// точка 0 - левому верхнему углу контролируемой территории
	//// зададим широту и долготу нулевой точки в десятичных дробях в системе WGS-84
	//pTargetLocationInMinutes->degrees = 59;
	//pTargetLocationInMinutes->minutes = 48;
	//pTargetLocationInMinutes->seconds = 39.59;
	//Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	//pTargetLocationInMinutes->degrees = 30;
	//pTargetLocationInMinutes->minutes = 12;
	//pTargetLocationInMinutes->seconds = 44.24;
	//Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	///*Lat0 = 59.812408;
	//Lon0 = 30.211367;*/

	//////----------------
	//double *pLat;
	//double *pLon;
	//

	//double LatP3_90_02_0 = 0;
	//pLat = &LatP3_90_02_0;

	//double LonP3_90_02_0 = 0;
	//pLon = &LonP3_90_02_0;

	//hight0 = 20;

	//H0 = 0;
	//pH = &H0;
	//
	//if(WGS_84_P3_90_02(Lat0, Lon0, hight0, pH, pLat, pLon) )
	//{
	//	
	//	sprintf(info, "LatP3_90_02_0 =  %11.8f\n", LatP3_90_02_0);
	//	ern::Log::getInstance().Write(info);

	//	sprintf(info, "LonP3_90_02_0 =  %11.8f\n", LonP3_90_02_0);
	//	ern::Log::getInstance().Write(info);

	//	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
	//	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatP3_90_02_0);
	//								
	//	sprintf(info, "deg_lat0 = %d;  min_x_lat0 = %d;	sec_x_lat0 = %9.6f;", pTargetLocationInMinutes->degrees, 
	//									pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
	//	ern::Log::getInstance().Write(info);
	//	//------------------------
	//	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
	//	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonP3_90_02_0);
	//								
	//	sprintf(info, "deg_lon0 = %d;  min_x_lon0 = %d;	sec_x_lon0 = %9.6f;", pTargetLocationInMinutes->degrees, 
	//									pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
	//	ern::Log::getInstance().Write(info);
	//}
	
	////////------------------------------------------------------------
	
	// результаты варианта 9
	//  LatP3_90_02_0 =  59.81097394
	//  LonP3_90_02_0 =  30.21199235

	//  deg_lat0 = 59;  min_x_lat0 = 48;	sec_x_lat0 = 39.506192;
	//  deg_lon0 = 30;  min_x_lon0 = 12;	sec_x_lon0 = 43.172455;
	/////////////////////////////////////////
	// Вариант №10
   // п.1 переход от географических координат WGS-84 к координатам в системе UTM
   // точка соответствует левому верхнему углу контролируемой территории

	//wgs_point_0.wgs_Y_lat = 59.8109972;
	//wgs_point_0.wgs_X_lon = 30.2122889;

	//pX = &X0;
	//pY = &Y0;

	//std::string str_utm = geo_calc.LatLong2UTMMerc(wgs_point_0.wgs_X_lon, wgs_point_0.wgs_Y_lat, pX, pY); 
	////	 
	//sprintf(info, "X0 =  %11.4f\n", X0);
	//ern::Log::getInstance().Write(info);

	//sprintf(info, "Y0 =  %11.4f\n", Y0);
	//ern::Log::getInstance().Write(info);

	//sprintf(info, "UTM0:  %s\n", str_utm.c_str() );
	//ern::Log::getInstance().Write(info);
	// нужно проверить и получить результат....
	
	//--------------------------------------
	/////////////////////////////////////////
	// Вариант 11
	/////////////////////////////////
	// п.1 переход из проекции Гаусса-Крюггера в систему географических координат СК-42 (широта и долгота)									
	// п.2 переход из системы СК-42 в систему П3-90.02

	// Исходные данные (результаты варианта 4)
	
	////  X0 =  6634346.6089
	////  Y0 =  495737.4534

	////  X1 =  6635618.7225
	////  Y1 =  497191.9465

	////  dX =   1272.1136
	////  dY =    1454.4931

	//double *pLat; 
	//double *pLon;

	//TTargetLocationInMinutes targetLocationInMinutes;
	//TTargetLocationInMinutes *pTargetLocationInMinutes;
	//pTargetLocationInMinutes = &targetLocationInMinutes;

	//X0 =  495737.4534;
	//Y0 = 6634346.6089;

	//double LatCK_42_0 = 0;
	//pLat = &LatCK_42_0;

	//double LonCK_42_0 = 0;
	//pLon = &LonCK_42_0;
	//
	//geo_calc.PrGausToSK42(X0, Y0, pLat, pLon);

	////wgs_point_0.wgs_Y_lat = LatWGS_84_0;
	////wgs_point_0.wgs_X_lon = LonWGS_84_0;

	//hight0 = 0;
	//pH = &hight0;

	//double LatP3_90_02_0 = 0;
	//pLat = &LatP3_90_02_0;

	//double LonP3_90_02_0 = 0;
	//pLon = &LonP3_90_02_0;

	//H0 = 23;

 //  if( geo_calc.SK42_P3_90_02(LatCK_42_0, LonCK_42_0, H0, pH, pLat, pLon) )
 //  {
	//	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
	//	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LatP3_90_02_0);
	//								
	//	sprintf(info, "deg_lat0 = %d;  min_x_lat0 = %d;	sec_x_lat0 = %9.6f;", pTargetLocationInMinutes->degrees, 
	//									pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
	//	ern::Log::getInstance().Write(info);
	//	//------------------------
	//	memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );
	//	geo_calc.GetTargetLocationInMinutes(pTargetLocationInMinutes, LonP3_90_02_0);
	//								
	//	sprintf(info, "deg_lon0 = %d;  min_x_lon0 = %d;	sec_x_lon0 = %9.6f;", pTargetLocationInMinutes->degrees, 
	//									pTargetLocationInMinutes->minutes, pTargetLocationInMinutes->seconds);
	//	ern::Log::getInstance().Write(info);
	//   
	//    // sprintf(info, "UTM0:  36 V 345706.9657 6.3159e+006\n", str_utm.c_str() );
	//	// ern::Log::getInstance().Write("UTM0:  36 V 345706.9657 6.3159e+006\n");
 //  }

   // Результаты варианта №8
   // deg_lat0 = 59;  min_x_lat0 = 47;	sec_x_lat0 = 35.693221;
   // deg_lon0 = 30;  min_x_lon0 = 15;	sec_x_lon0 =  1.192182;
   // UTM0:  36 V 345706.9657 6.3159e+006
   // Итог: все Ok!
   //----------------------------
   // Результаты варианта №11 (должны быть приблизительно такими же)
   // проверить на 64-разрядной машине ....
   //(на 32-х разрядной получилась туфта)....

   ////////////////////////////////////////
	// Вариант №12
	// п.2 переход из системы WGS-84 к поперечной проекции Меркатора (UTM)

	TTargetLocationInMinutes targetLocationInMinutes;
	TTargetLocationInMinutes *pTargetLocationInMinutes;
	pTargetLocationInMinutes = &targetLocationInMinutes;
	//// memset(pTargetLocationInMinutes, 0, sizeof(TTargetLocationInMinutes) );

	// точка 0 - центр ВПП1
	// зададим широту и долготу нулевой точки в десятичных дробях в системе WGS-84
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 47;
	pTargetLocationInMinutes->seconds = 42.26;
	Lat0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 15;
	pTargetLocationInMinutes->seconds = 2.42;
	Lon0 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	Etalon_H0 = 24.0;
	////----------------
	// Lat1 = 59.4832;				// (центр ВПП2)
	// Etalon_Lat1 = 59.805453;		// (центр ВПП2)
	// Lon1 = 30.1652;				// (центр ВПП2)
	// Etalon_Lon1 = 30.274379;		// (центр ВПП2)

	//// точка 1- центр ВПП2 
	//// зададим широту и долготу первой точки в десятичных дробях в системе WGS-84
	pTargetLocationInMinutes->degrees = 59;
	pTargetLocationInMinutes->minutes = 48;
	pTargetLocationInMinutes->seconds = 19.15;
	Lat1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	pTargetLocationInMinutes->degrees = 30;
	pTargetLocationInMinutes->minutes = 16;
	pTargetLocationInMinutes->seconds = 29.57;
	Lon1 = geo_calc.GetDecTargetLocation(pTargetLocationInMinutes);

	Etalon_H1 = 21.8;	// (центр ВПП2)
	////----------------
	double *pLat;
	double *pLon;
	
	pLat = &Lat0;
	pLon = &Lon0;

	////////// переход в WGS-84
	pX = &X0;
	pY = &Y0;	
	pH = &H0;
	std::string  str_utm;
	
	wgs_point_0.wgs_Y_lat = Lat0;
	wgs_point_0.wgs_X_lon = Lon0;

	
	str_utm = geo_calc.LatLong2UTMMerc(wgs_point_0.wgs_X_lon, wgs_point_0.wgs_Y_lat, pX, pY); 
		 
	sprintf(info, "X0 =  %11.4f\n", X0);
	ern::Log::getInstance().Write(info);

	sprintf(info, "Y0 =  %11.4f\n", Y0);
	ern::Log::getInstance().Write(info);

	sprintf(info, "UTM0:  %s\n", str_utm.c_str() );
	ern::Log::getInstance().Write(info);
	////-----------------------------
	
	pX = &X1;
	pY = &Y1;
	////////------------------------------------------------------------
	pLat = &Lat1;
	pLon = &Lon1;
	pH = &H1;

	wgs_point_1.wgs_Y_lat = Lat1;
	wgs_point_1.wgs_X_lon = Lon1;
		
	str_utm = geo_calc.LatLong2UTMMerc(wgs_point_1.wgs_X_lon, wgs_point_1.wgs_Y_lat, pX, pY); 

	sprintf(info, "X1 =  %11.4f\n", X1);
	ern::Log::getInstance().Write(info);

	sprintf(info, "Y1 =  %11.4f\n", Y1);
	ern::Log::getInstance().Write(info);

	sprintf(info, "UTM1:  %s\n", str_utm.c_str() );
	ern::Log::getInstance().Write(info);
	
	////////-----------------------------------
	dX = X1 - X0;
	dY = Y1 - Y0;

	sprintf(info, "dX =  %11.4f\n", dX);
	ern::Log::getInstance().Write(info);

	sprintf(info, "dY =  %11.4f\n", dY);
	ern::Log::getInstance().Write(info);

	// результаты варианта 12
	// 01.06.2017 23:22:34 :  X0 =  345730.5719
	// 01.06.2017 23:22:34 :  Y0 =  6631788.9818
	// 01.06.2017 23:22:34 :  UTM0:  36 V 345731 6.63179e+006

	// 01.06.2017 23:22:34 :  X1 =  347135.3542
	// 01.06.2017 23:22:34 :  Y1 =  6632873.4137
	// 01.06.2017 23:22:34 :  UTM1:  36 V 347135 6.63287e+006

	// 01.06.2017 23:22:34 :  dX =    1404.7823
	// 01.06.2017 23:22:34 :  dY =    1084.4319

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
int main(int argc, char *argv[]) {
	
	////////////////////////////////////////////////////
	int err = 0;
	printf("Start service \"Analyzer conflict\"!\n");
	ern::Log::getInstance().Write("Start service \"Analyzer conflict\"!\n");

	#ifdef __linux__
        //linux code goes here
        const char* conf_file_name = "conf_app.xml";
        const char* conf_data_airodrome_name = "conf_airports.xml";
    #elif _WIN32
        // windows code goes here

    #else
        // #error "OS not supported!"
    #endif

	ern::ConfiguratorApp::getInstance().LoadXML(conf_file_name);
	ern::ConfiguratorAirports::getInstance().LoadXML(conf_data_airodrome_name);

	// testLoadAppSetXML();				// проверка правильности загрузки насроечных данных
	// testLoadAirportsSetXML();		// проверка правильности загрузки насроечных данных аэродромов
	// testDetermCorrectnCalcCoordPolygonvertices();		// определение правильности расчета координат вершин полигона
	// testCalcPrCoord();					   // тестирование точного алгоритма перевода из системы WGS-84 в прямоугольную 
	// testTransitionToAnotherCoordinateSystem();
	// testRefPointAndKoeffMasshtab();

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

	/*Получение дискриптора*/
	// HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	/*Цвет всего фона - белый. Цвет всего текста - черный */
	// system("color F0");
	// puts("Hello World!");

	/*Цвет символов - желтый. Цвет фона - темно-серый*/
	// SetConsoleTextAttribute(hConsole, (WORD) ((DARK_GRAY << 4) | YELLOW));
	// printf("Hello");

	/*Цвет символов - светло-зеленый. Цвет фона - желтый*/
	// SetConsoleTextAttribute(hConsole, (WORD) ((YELLOW << 4) | LightGREEN));
	// puts("World!");

	/*Цвет символов - белый. Цвет фона - красный*/
	// SetConsoleTextAttribute(hConsole, (WORD) ((RED << 4) | WHITE));
	// puts("                                                                     ");

	// SetConsoleTextAttribute(hConsole, (WORD) ((RED << 4) | WHITE));
	// puts("                                                                     ");
	// SetConsoleTextAttribute(hConsole, (WORD) ((DARK_GRAY << 4) | YELLOW));
	// for(int i=0; i<80; i++)
	//	printf(" ");
	// for(int i=0; i<75; i++)
	//	printf(" ");
	// SetConsoleTextAttribute(hConsole, (WORD) ((RED << 4) | WHITE));
	// puts("                                                                     ");
	// printf(" ");
	/*userInterface theUserInterface;
	theUserInterface.interact();*/
    // getch();

	return err;	
}

//------------------------------------------------


