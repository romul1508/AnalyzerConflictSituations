// TestsAnalyzСonfApp implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 17.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

#include "GeoCalc.h"
#include <cstring>

double UTM2LatLon::strToDouble(std::string token)
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
//---------------------------------
std::string UTM2LatLon::getHemisphere(std::string latZone)
{
      std::string hemisphere = "N";
      // if (southernHemisphere.indexOf(latZone) > -1)
	  if (southernHemisphere.find_first_of(latZone) > -1)
      		hemisphere = "S";
      
      return hemisphere;
}
//-----------------------------------
void UTM2LatLon::setVariables()
{
    arc = northing / k0;
    mu = arc  / (a * (1 - pow(e, 2) / 4.0 - 3 * pow(e, 4) / 64.0 - 5 * pow(e, 6) / 256.0));

    ei = (1 - pow((1 - e * e), (1 / 2.0))) / (1 + pow((1 - e * e), (1 / 2.0)));

    ca = 3 * ei / 2 - 27 * pow(ei, 3) / 32.0;

    cb = 21 * pow(ei, 2) / 16.0 - 55 * pow(ei, 4) / 32.0;
    cc = 151 * pow(ei, 3) / 96.0;
    cd = 1097 * pow(ei, 4) / 512.0;
    phi1 = mu + ca * sin(2 * mu) + cb * sin(4 * mu) + cc * sin(6 * mu) + cd
        * sin(8 * mu);

    n0 = a / pow((1 - pow((e * sin(phi1)), 2)), (1 / 2.0));

    r0 = a * (1 - e * e) / pow((1 - pow((e * sin(phi1)), 2)), (3 / 2.0));
    fact1 = n0 * tan(phi1) / r0;

    _a1 = 500000 - easting;
    dd0 = _a1 / (n0 * k0);
    fact2 = dd0 * dd0 / 2;

    t0 = pow(tan(phi1), 2);
    Q0 = e1sq * pow(cos(phi1), 2);
    fact3 = (5 + 3 * t0 + 10 * Q0 - 4 * Q0 * Q0 - 9 * e1sq) * pow(dd0, 4) / 24.0;

    fact4 = (61 + 90 * t0 + 298 * Q0 + 45 * t0 * t0 - 252 * e1sq - 3 * Q0 * Q0) * pow(dd0, 6) / 720.0;

    lof1 = _a1 / (n0 * k0);
    lof2 = (1 + 2 * t0 + Q0) * pow(dd0, 3) / 6.0;
    lof3 = (5 - 2 * Q0 + 28 * t0 - 3 * pow(Q0, 2) + 8 * e1sq + 24 * pow(t0, 2))
        * pow(dd0, 5) / 120;
    _a2 = (lof1 - lof2 + lof3) / cos(phi1);
    _a3 = _a2 * 180 / Pi;
}   
//---------------------------------------------------
// конвертирует координаты из формата UTM в географические координаты WGS-84
void UTM2LatLon::convertUTMToLatLong(std::string UTM, double *pLat, double *pLon)
{
      int n_simb;
	  char strTemp[40];
	  std::string str_temp;
	  std::string min_str_temp = UTM;
	  std::vector<std::string> utm_n;

	  memset(strTemp, 0, 40);
	  min_str_temp.copy(strTemp, min_str_temp.length(), 0);
	  
      // String[] utm = UTM.split(" ");
	  while(min_str_temp.size() > 0)
	  {
			std::string token;
			n_simb = min_str_temp.find_first_of(" \r\n");

			if(n_simb == -1)
				break;

			min_str_temp.copy(strTemp, n_simb, 0);
			strTemp[n_simb] = 0;

			token = strTemp;
			// cout << "step 3: token = " << token << endl;

			utm_n.push_back(token);
			min_str_temp.erase(0, n_simb+1);
	 }
      
	  str_temp = utm_n[0];
	  zone = fromString<int>(str_temp);	// Integer.parseInt(utm[0]);

	  std::string latZone = utm_n[1];
      
	  // easting = Double.parseDouble(utm[2]);
	  easting = strToDouble(utm_n[2]);

      // northing = Double.parseDouble(utm[3]);
	  northing = strToDouble(utm_n[3]);

      std::string hemisphere = getHemisphere(latZone);
      double latitude = 0.0;
      double longitude = 0.0;

      // if (hemisphere.equals("S"))
	  if (hemisphere == "S")
		  northing = 10000000 - northing;
      
      setVariables();
      latitude = 180.0 * (phi1 - fact1 * (fact2 + fact3 + fact4)) / Pi;

      if (zone > 0)
      		zoneCM = 6.0 * zone - 183.0;
      else
            zoneCM = 3.0;      

      longitude = zoneCM - _a3;

      if (hemisphere =="S")      
			latitude = -latitude;      

	  *pLat = latitude;
	  *pLon = longitude;      
}
//--------------------------------------
// конвертирует координаты из формата UTM в географические координаты WGS-84
void UTM2LatLon::convertUTMToLatLong(std::string longZone, std::string latZone, double easting, double northing, double *pLat, double *pLon)
{
	zone = fromString<int>(longZone);

	this->easting = easting;      
	 this->northing = northing; 
	
	  std::string hemisphere = getHemisphere(latZone);
      double latitude = 0.0;
      double longitude = 0.0;

      if (hemisphere == "S")
		  northing = 10000000 - northing;
      
      setVariables();
      latitude = 180.0 * (phi1 - fact1 * (fact2 + fact3 + fact4)) / Pi;

      if (zone > 0)
      		zoneCM = 6.0 * zone - 183.0;
      else
            zoneCM = 3.0;      

      longitude = zoneCM - _a3;

      if (hemisphere == "S")      
			latitude = -latitude;      

	  *pLat = latitude;
	  *pLon = longitude;
}
//--------------------------------------
int LatZones::getLatZoneDegree(std::string letter)
{
	char ltr = letter[0];		//.at(0);	//.charAt(0);
	for (int i = 0; i < arrayLength; i++)
    {
        if (letters[i] == ltr)
        	return degrees[i];
        
    }
    return -100;
}
//----------------------------------------
std::string LatZones::getLatZone(double latitude)
{
      int latIndex = -2;
      int lat = (int) latitude;
	  std::string val;

      if (lat >= 0)
      {
		  // int len = posLetters.length;
		  for (int i = 0; i < posLettersLength; i++)
		  {
				if (lat == posDegrees[i])
				{
					latIndex = i;
					break;
				}

				if (lat > posDegrees[i])
					continue;
				else
				{
					latIndex = i - 1;
					break;
				 }
			}
      }
      else
      {
			// int len = negLetters.length;
			for (int i = 0; i < negLettersLength; i++)
			{
				 if (lat == negDegrees[i])
				 {
						latIndex = i;
						break;
				}

				 if (lat < negDegrees[i])
				{
					  latIndex = i - 1;
					  break;
				}
				else
					 continue;				
			}
      }

      if (latIndex == -1)
      		latIndex = 0;
      
      if (lat >= 0)
      {
			if (latIndex == -2)
				latIndex = posLettersLength - 1;
			
			val.append(1, posLetters[latIndex]);
			 return val;
      }
      else
      {
			if (latIndex == -2)
				 latIndex = negLettersLength - 1;
			
			val.append(1, negLetters[latIndex]);
			return val;
      }
  }
//----------------------------------------
// проверяет на корректность широту и долготу
bool LatLon2UTM::validate(double latitude, double longitude)
{
	if (latitude < -90.0 || latitude > 90.0 || longitude < -180.0 || longitude >= 180.0)
    {
     	ern::Log::getInstance().Write("Legal ranges: latitude [-90,90], longitude [-180,180).");
		return false;	// исходные данные не корректны
    }
	else
		return true;
}
//----------------------------------------
void LatLon2UTM::setVariables(double latitude, double longitude)
{
	  double sinLat, cosLat, tanLat;
	  latitude = degreeToRadian(latitude);
	  sinLat = sin(latitude);
	  cosLat = cos(latitude);
	  tanLat = tan(latitude);

      rho = equatorialRadius * (1 - e * e) / pow(1 - pow(e * sinLat, 2), 3 / 2.0);

      nu = equatorialRadius / pow(1 - pow(e * sinLat, 2), 0.5 );

      double var1;
      if (longitude < 0.0)      
        var1 = ((int) ((180 + longitude) / 6.0)) + 1;      
      else      
        var1 = ((int) (longitude / 6)) + 31;
      
      double var2 = (6 * var1) - 183;
      double var3 = longitude - var2;
      p = var3 * 3600.0 / 10000.0;

      S = A0 * latitude - B0 * sin(2 * latitude) + C0 * sin(4 * latitude) - D0
          * sin(6 * latitude) + E0 * sin(8 * latitude);

      K1 = S * k0;
      K2 = nu * sinLat * cosLat * pow(sin1, 2) * k0 * (100000000) / 2;
      K3 = ((pow(sin1, 4) * nu * sinLat * pow(cosLat, 3)) / 24)
          * (5 - pow(tanLat, 2) + 9 * e1sq * pow(cosLat, 2) + 4
              * pow(e1sq, 2) * pow(cosLat, 4)) * k0  * (10000000000000000L);

      K4 = nu * cosLat * sin1 * k0 * 10000.0;

      K5 = pow(sin1 * cosLat, 3) * (nu / 6)
          * (1 - pow(tanLat, 2) + e1sq * pow(cosLat, 2)) * k0
          * 1000000000000L;

      A6 = (pow(p * sin1, 6) * nu * sinLat * pow(cosLat, 5) / 720)
          * (61 - 58 * pow(tanLat, 2) + pow(tanLat, 4) + 270
              * e1sq * pow(cosLat, 2) - 330 * e1sq
              * pow(sinLat, 2)) * k0 * (1E+24);
}
//----------------------------------------
std::string LatLon2UTM::getLongZone(double longitude)
{
      double longZone = 0;
      if (longitude < 0.0)
      {
			longZone = ((180.0 + longitude) / 6.0) + 1.0;
      }
      else
      {
			longZone = (longitude / 6.0) + 31;
      }

	  std::string val;
	  val = toString((int) longZone);
	  
      if (val.length() == 1)
      {
        val = "0" + val;
      }
      return val;
}
//----------------------------------------
double LatLon2UTM::getNorthing(double latitude)
{
      double northing = K1 + K2 * p * p + K3 * pow(p, 4);
      if (latitude < 0.0)
			northing = 10000000 + northing;
      
      return northing;
}
//----------------------------------------
// переводит широту и долготу в UTM
std::string LatLon2UTM::convertLatLonToUTM(double latitude, double longitude, double *pX, double *pY)
{
	std::string info("1");
	std::string UTM = "";
	LatZones latZones;
	
	if(!validate(latitude, longitude) )
	{
		info = toString(0);
		return info;
	}

	setVariables(latitude, longitude);
	
	std::string longZone = getLongZone(longitude);
	std::string latZone = latZones.getLatZone(latitude);
	double _easting = getEasting();
	*pX = _easting;
	double _northing = getNorthing(latitude);
	*pY = _northing;

	UTM = longZone + " " + latZone + " ";		// + ((int) _easting) + " "+ ((int) _northing);
	info = toString((float) _easting);
	UTM += info;
	UTM += " ";
	info = toString((float) _northing);
	UTM += info;

	return UTM;
}
//----------------------------------------
// проверяет на корректность широту и долготу
bool CoordinateUTMConversion::validate(double latitude, double longitude)
{
	if (latitude < -90.0 || latitude > 90.0 || longitude < -180.0 || longitude >= 180.0)
    {
     	ern::Log::getInstance().Write("Legal ranges: latitude [-90,90], longitude [-180,180).");
		return false;	// исходные данные не корректны
    }
	else
		return true;
}
//----------------------------------------
// переводит широту и долготу в UTM
std::string CoordinateUTMConversion::latLon2UTM(double lat, double lon, double* pX, double* pY)
{
	LatLon2UTM c;	// = new LatLon2UTM();
    return c.convertLatLonToUTM(lat, lon, pX, pY);
}
//----------------------------------------
void CoordinateUTMConversion::utm2LatLon(std::string longZone, std::string latZone, double easting, double northing, double *pLat, double *pLon)
{
    UTM2LatLon c;	// = new UTM2LatLon();
    c.convertUTMToLatLong(longZone, latZone, easting, northing, pLat, pLon);
 }
//----------------------------------------
void CoordinateUTMConversion::utm2LatLon(std::string UTM, double *pLat, double *pLon)
{
    UTM2LatLon c;	// = new UTM2LatLon();
    c.convertUTMToLatLong(UTM, pLat, pLon);
 }
//----------------------------------------
// находит нужный корень долготы для различных систем координат в основной системе уравнений 
int GeoCalc::Find_Root(double A, double B, double cosLon, double z, double e2, double a, double x, double* m)
{
	int Count = 0;
	double Eps = 0.0000001;

	if (Diff(A, cosLon, z, e2, a, x) * Diff(B, cosLon, z, e2, a, x) > 0) 
		return 0;

	do
	{
		*m = (A + B) / 2;
		if (Diff(*m, cosLon, z, e2, a, x) * Diff(A, cosLon, z, e2, a, x) > 0) 
		{
			A = *m;
		}
		else
		{
			B = *m;
		}
		Count ++;
	} while (fabs(B - A) >= Eps);

	return 1;
}

void GeoCalc::WgsToPrGaus(double dLat, double dLon, double *pX, double *pY)
{
	// Перевод географических координат (широты и долготы) в плоские прямоугольные
	// координаты проекции Гаусса-Крюгера 
	
	// Географические координаты точки (в градусах)
	// double dLon = 37.618;
	// double dLat = 55.752;

	// Номер зоны Гаусса-Крюгера (если точка рассматривается в системе координат
	// соседней зоны, то номер зоны следует присвоить вручную)
	int zone = (int) (dLon/6.0 + 1);

	// Параметры эллипсоида Красовского
	double a = 6378245.0;		// Большая (экваториальная) полуось
	double b = 6356863.019;		// Малая (полярная) полуось

	double PI = 3.14159265358979;	// число Пи

	double e2 = ( pow(a, 2) - pow(b, 2) ) / pow(a, 2);	// Эксцентриситет
	double n = (a - b) / (a + b);						//	Приплюснотость

	// Параметры зоны Гаусса-Крюгера
	double F = 1.0;				// Масштабный коэффициент
	double Lat0 = 0.0;			// Начальная параллель в радианах

	double Lon0 = (zone*6-3)* PI / 180;		// Центральный меридиан (в радианах)

	double N0 = 0.0;					// Условное северное смещение для начальной параллели
	double E0 = zone * 1E6 + 500000.0;	// Условное восточное смещение для центрального меридиана
	
	// Перевод широты и долготы в радианы
	double Lat = dLat * PI / 180.0;
	double Lon = dLon * PI / 180.0;

	// Вычисление переменных для преобразования
	double sinLat = sin(Lat);
	double cosLat = cos(Lat);
	double tanLat = tan(Lat);

	double v = a * F * pow(1 - e2 * pow(sinLat, 2), -0.5);
	double p = a * F * (1-e2) * pow(1 - e2 * pow(sinLat, 2), -1.5);

	double n2 = v/p - 1;
	double M1 = (1 + n + 5.0/4.0 * pow(n,2) + 5.0/4.0 * pow(n, 3)) * (Lat-Lat0);
	double M2 = (3 * n + 3 * pow(n, 2) + 21.0/8.0 * pow(n, 3)) * sin(Lat - Lat0) * cos(Lat + Lat0);
	double M3 = (15.0/8.0 * pow(n, 2) + 15.0/8.0 * pow(n, 3)) * sin(2 * (Lat - Lat0) ) * cos(2 * (Lat + Lat0) );
	double M4 = 35.0/24.0 * pow(n, 3) * sin(3 * (Lat - Lat0)) * cos(3 * (Lat + Lat0));
	double M = b * F * (M1 - M2 + M3 - M4);

	double I = M + N0;
	double II = v/2 * sinLat * cosLat;
	double III = v/24 * sinLat * pow(cosLat, 3) * (5 - pow(tanLat, 2) + 9 * n2);
	double IIIA = v/720 * sinLat * pow(cosLat, 5) * (61 - 58 * pow(tanLat, 2) + pow(tanLat, 4));
	double IV = v * cosLat;
	double V = v/6 * pow(cosLat, 3) * (v/p - pow(tanLat, 2));
	double VI = v/120 * pow(cosLat, 5) * (5 - 18 * pow(tanLat, 2) + pow(tanLat, 4) + 14 * n2 - 58 * pow(tanLat, 2) * n2);

	// Вычисление северного и восточного смещения (в метрах)
	double N = I + II * pow(Lon - Lon0, 2) + III * pow(Lon - Lon0, 4) + IIIA * pow(Lon - Lon0, 6);
	double E = E0 + IV * (Lon - Lon0) + V * pow(Lon - Lon0, 3) + VI * pow(Lon - Lon0, 5);

	*pY = N;
	*pX = E;

	// Если Широта: 55.752;
	//		Долгота: 37.618;
	// то Северное смещение составляет:
	//		6181924.245933299
	// а Восточное смещение:
	// 7413223.481449484
}
//---------------------------------------------------
// пересчет координат из системы CK-42 в плоские прямоугольные (проекция Гаусса-Крюгера)
void GeoCalc::SK42ToPrGaus(double Lat, double Lon, double *pX, double *pY)
{
	double B, L, l, l2, sin2Lat, sin4Lat, sin6Lat, cosLat;
	int n;

	B = Lat * DEG_RAD;				// широта в радианах
	L = Lon * DEG_RAD;				// долгота в радианах

	n = (Lon + 6.0)/6;												// номер шестиградусной зоны в проекции Гаусса - Крюгера 
	l = (Lon - (3 + 6 * (n - 1) ) ) / 57.29577951;		// расстояние от заданной точки до осевого меридиана зоны
	l2 = pow(l, 2);

	sin2Lat = pow(sin(B), 2);
	sin4Lat = pow(sin2Lat, 2);
	sin6Lat = pow(sin(B), 6);
	cosLat = cos(B);

	/**pX = 6367558.4968 * B - sin(B * 2.0) * (16002.89 + 66.9607 * sin2Lat + 0.3515 * sin4Lat - l2 * (1594561.25 + 5336.535 * sin2Lat +
		26.79 * sin4Lat + 0.149 * sin6Lat + l2 * (672483.4 - 811219.9 * sin2Lat + 5420.0 * sin4Lat - 10.6 * sin6Lat + l2 * (278194.0 - 830174.0 * sin2Lat + 
		572434.0 * sin4Lat - 16010.0 * sin6Lat + l2 * (109500.0 - 574700.0 * sin2Lat + 863700.0 * sin4Lat - 398600.0 * sin6Lat ))))); 

	*pY = (5.0 + 10.0 * n) * 10000.0 + l * cosLat * (6378245.0 + 21346.1415 * sin2Lat + 107.159 * sin4Lat + 0.5977 * sin6Lat + l2 * (1070204.16 -
		2136826.66 * sin2Lat + 17.98 * sin4Lat - 11.99 * sin6Lat + l2 * ( 270806.0 - 1523417.0 * sin2Lat + 1327645.0 * sin4Lat -
		21701.0 * sin6Lat + l2 * (79690.0 - 866190.0 * sin2Lat + 1730360.0 * sin4Lat - 945460.0 * sin6Lat))));*/

	*pY = 6367558.4968 * B - sin(B * 2.0) * (16002.89 + 66.9607 * sin2Lat + 0.3515 * sin4Lat - l2 * (1594561.25 + 5336.535 * sin2Lat +
		26.79 * sin4Lat + 0.149 * sin6Lat + l2 * (672483.4 - 811219.9 * sin2Lat + 5420.0 * sin4Lat - 10.6 * sin6Lat + l2 * (278194.0 - 830174.0 * sin2Lat + 
		572434.0 * sin4Lat - 16010.0 * sin6Lat + l2 * (109500.0 - 574700.0 * sin2Lat + 863700.0 * sin4Lat - 398600.0 * sin6Lat ))))); 

	*pX = (5.0 + 10.0 * n) * 10000.0 + l * cosLat * (6378245.0 + 21346.1415 * sin2Lat + 107.159 * sin4Lat + 0.5977 * sin6Lat + l2 * (1070204.16 -
		2136826.66 * sin2Lat + 17.98 * sin4Lat - 11.99 * sin6Lat + l2 * ( 270806.0 - 1523417.0 * sin2Lat + 1327645.0 * sin4Lat -
		21701.0 * sin6Lat + l2 * (79690.0 - 866190.0 * sin2Lat + 1730360.0 * sin4Lat - 945460.0 * sin6Lat))));
}
//---------------------------------------------------
// преобразование плоских прямоугольных координат в проекции Гаусса - Крюгера
// в геодезические координаты CK-42 на эллипсоиде Красовского
void GeoCalc::PrGausToSK42(double X, double Y, double *pLat, double *pLon)
{
	double Beta, Z0, Z02, B, B0, sin2Beta, sin4Beta, dB, sin2B0, sin4B0, sin6B0, l, L;
	int n;

	n = X / 100000;					// номер шестиградусной зоны в проекции Гаусса- Крюгера
	Beta = Y / 6367558.4968;		// вспомогательная величина
	/*double tempY = Y * 0.000001;
	Beta = tempY / 6.36756;*/			// вспомогательная величина

	sin2Beta = pow(sin(Beta), 2);
	sin4Beta = pow(sin2Beta, 2);

	B0 = Beta + sin(2.0 * Beta) * (0.00252588685 - 0.0000149186 * sin2Beta + 0.00000011904 * sin4Beta);
	sin2B0 = pow(sin(B0), 2);
	sin4B0 =  pow(sin2B0, 2);
	sin6B0 = pow(sin(B0), 6);

	Z0 = ( Y - (10.0 * n + 5) * 10000.0)/(6378245.0 * cos(B0));
	Z02 = pow(Z0, 2);
	dB = - Z02 * sin( 2.0 * B0) * (0.251684631 - 0.003369263 * sin2B0 + 0.000011276 * sin4B0 -
		Z02 * ( 0.10500614 - 0.04559916 * sin2B0 + 0.00228901 * sin4B0 - 0.00002987 * sin6B0 -
		Z02 * ( 0.042858 - 0.025318 * sin2B0 + 0.014346 * sin4B0 - 0.001264 * sin6B0 - 
		Z02 * (0.01672 - 0.0063 * sin2B0 + 0.01188 * sin4B0 - 0.00328 * sin6B0))));

	B = B0 + dB;
	*pLat = B * RAD_DEG;

	l = Z0 * (1 - 0.0033467108 * sin2B0 - 0.0000056002 * sin4B0 - 0.0000000187 * sin6B0 - 
		Z02 * (0.16778975 + 0.16273586 * sin2B0 - 0.0005249 * sin4B0 - 0.00000846 * sin6B0 -
		Z02 * (0.0420025 + 0.1487407 * sin2B0 + 0.005942 * sin4B0 - 0.000015 * sin6B0 -
		Z02 * (0.01225 + 0.09477 * sin2B0 + 0.03282 * sin4B0 - 0.00034 * sin6B0 -
		Z02 * (0.0038 + 0.0524 * sin2B0 + 0.0482 * sin4B0 + 0.0032 * sin6B0)))));

	L = 6 * (n - 0.5)/57.29577951 + l;
	*pLon = L * RAD_DEG;
}
//------------------------------------------------------
// Пересчет координат из широты/долготы в поперечную проекцию Меркатора/WGS84
void GeoCalc::LatLongUTMMerc(double lon, double lat, double* x, double* y)
{

}
//-------------------------------------------------------
//double dB(double Bd, double Ld, double H, double aP, double aW, double dx, double dy, double dz, double wx, double wy, double wz, double ms)
//{
//	double B, L, M, N, dB;
//	// double aP, double aW;
//	// const double Pi = 3.14159265358979;		// число Пи
//	const double ro = 206264.8062;				// Число угловых секунд в радиане
//
//	//-------------------------------------------
//	//// Линейные элементы трансформирования, в метрах
//	////double dx;  
//	//double dy;
//	//double dz; 
//	//------------------------------------------ 
//	//// Угловые элементы трансформирования, в секундах
//	//double wx;
//	//double wy;
//	//double wz;
//	//-----------------------------------------
//	// Дифференциальное различие масштабов
//	// double ms;
//
//	B = Bd * Pi / 180;
//	L = Ld * Pi / 180;
//	M = a(aP, aW) * ( 1 - e2() ) / pow( (1 - e2() * sin(B) * sin(B) ), 1.5);
//	N = a(aP, aW) * pow( (1 - e2() * sin(B) * sin(B) ), -0.5);
//
//	double expr1 = ro / (M + H);
//	double expr2 = N / a(aP, aW) * e2() * sin(B) * cos(B) * da(aP, aW);
//	double expr3 = (N * N) / (a(aP, aW) * a(aP, aW) ) + 1;
//
//	// dB = ro / (M + H)  *  (N / a() * e2() * sin(B) * cos(B) * da() + (N * N / a() * a() + 1) * N * sin(B) * cos(B) * de2() / 2 - (dx * cos(L) + dy * sin(L)) * sin(B) + dz * cos(B)) - wx * sin(L) * (1 + e2() * cos(2 * B)) + wy * cos(L) * (1 + e2() * cos(2 * B)) - ro * ms * e2() * sin(B) * cos(B);
//	dB = expr1 * (expr2 + expr3 * N * sin(B) * cos(B) * de2() / 2 - (dx * cos(L) + dy * sin(L)) * sin(B) + dz * cos(B)) - wx * sin(L) * (1 + e2() * cos(2 * B)) + wy * cos(L) * (1 + e2() * cos(2 * B)) - ro * ms * e2() * sin(B) * cos(B);
//	return dB;
//}
////--------------------------------------------------------------
//double dL(double Bd, double Ld, double H, double aP, double aW, double dx, double dy, double wx, double wy, double wz)
//{
//	double B, L, N, dL;
//	// const double Pi = 3.14159265358979;		// число Пи
//	const double ro = 206264.8062;				// Число угловых секунд в радиане
//		
//	B = Bd * Pi / 180;
//	L = Ld * Pi / 180;
//	N = a(aP, aW) * pow((1 - e2() * sin(B) * sin(B)), -0.5);
//	dL = ro / ((N + H) * cos(B)) * (-dx * sin(L) + dy * cos(L))  + tan(B) * (1 - e2() ) * (wx * cos(L) + wy * sin(L)) - wz;
//	return dL;
//}
//-----------------------------------------------------------------
//// вычисляет широту в системе Sk42
//double Wgs84_SK42_Lat(double Bd, double Ld, double H){		
//	double aP = 6378245;
//	double aW = 6378137;
//	double dx  = 23.92;
//	double dy = -141.27;
//	double dz = -80.9;
//	double ms = 0;
//	double wx = 0;
//	double wy = 0;
//	double wz = 0;
//
//	double WGS84_SK42_Lat = Bd - dB(Bd, Ld, H, aP, aW, dx, dy, dz, wx, wy, wz, ms) / 3600;
//	return WGS84_SK42_Lat;
//}
//----------------------------------------------------------------
//// вычисляет широту в системе WGS84
//double SK42_WGS84_Lat(double	Bd, double Ld, double H) {	
//	double aP = 6378245;
//	double aW = 6378137;
//	double dx  = 23.92;
//	double dy = -141.27;
//	double dz = -80.9;
//	double ms = 0;
//	double wx = 0;
//	double wy = 0;
//	double wz = 0;	
//	
//	double SK42_WGS84_Lat = Bd + dB(Bd, Ld, H, aP, aW, dx, dy, dz, wx, wy, wz, ms) / 3600;
//	return SK42_WGS84_Lat;
//}
//---------------------------------------------------------------
//// вычисляет долготу в системе Sk42
//double WGS84_SK42_Long(double Bd, double Ld, double H){		
//	double aP = 6378245;
//	double aW = 6378137;
//	double dx = 23.92;
//	double dy = -141.27;
//	double wx = 0;
//	double wy = 0;
//	double wz = 0;
//
//	// double dL(double Bd, double Ld, double H, double aP, double aW, double dx, double dy, double wx, double wy, double wz)
//	double WGS84_SK42_Long = Ld - dL(Bd, Ld, H, aP, aW, dx, dy, wx, wy, wz) / 3600;
//	return WGS84_SK42_Long;
//}
//---------------------------------------------------------------
//// вычисляет долготу в системе WGS84
//double SK42_WGS84_Long(double Bd, double Ld, double H){		
//	double aP = 6378245;
//	double aW = 6378137;
//	double dx = 23.92;
//	double dy = -141.27;
//	double wx = 0;
//	double wy = 0;
//	double wz = 0;
//	
//	double SK42_WGS84_Long = Ld + dL(Bd, Ld, H, aP, aW, dx, dy, wx, wy, wz) / 3600;
//	return SK42_WGS84_Long;
//}
//---------------------------------------------------------------------------
SimplifiedGeoCalc::SimplifiedGeoCalc(double lat, double lon, double height, double Z, int system_coord ) 
{ 
	double B, L, N, X, Y, X_ref, Y_ref;

	TWgsPoint	wgs_84_point_cpa;
	TSK42Point sk_42_point_cpa;
	TXYZSystemPoint xyz_point_cpa;

	const double Pi = 3.14159265358979;		// число Пи
	const double e  = 0.0066934216;
	aP = 6378245;
	aW = 6378137;
	dx  = 23.92;
	dy = -141.27;
	dz = -80.9;
	ms = 0;
	wx = 0;
	wy = 0;
	wz = 0;

	X = 0;
	Y = 0;
	X_ref = 0;
	Y_ref = 0;

	this->height = height;
		
	wgs_84_point_cpa.wgs_Y_lat = MyLAT;			// задается широта референсной точки приложения
	wgs_84_point_cpa.wgs_X_lon = MyLON;			// задается долгота референсной точки приложения
	sk_42_point_cpa.sk42_Y_lat = Wgs84_SK42_Lat(wgs_84_point_cpa.wgs_Y_lat, wgs_84_point_cpa.wgs_X_lon, height);
	sk_42_point_cpa.sk42_X_lon = WGS84_SK42_Long(wgs_84_point_cpa.wgs_Y_lat, wgs_84_point_cpa.wgs_X_lon, height);

	B = sk_42_point_cpa.sk42_Y_lat * Pi / 180;
	L = sk_42_point_cpa.sk42_X_lon * Pi / 180;
	N = aP * pow( (1 - e * sin(B) * sin(B) ), -0.5);
	
	xyz_point_cpa.X = (N + height)* cos(B) * cos(L);
	xyz_point_cpa.Y = (N + height)* cos(B) * sin(L);
	xyz_point_cpa.Z = (N * (1 - e) + height) * sin(B);
	
	GeoCalc geoCalc;
	geoCalc.WgsToPrGaus(wgs_84_point_cpa.wgs_Y_lat, wgs_84_point_cpa.wgs_X_lon, &X_ref, &Y_ref);
	//xyz_point_cpa.X = X_ref;
	//xyz_point_cpa.Y = Y_ref;

	if(system_coord == WGS_84)
	{
		wgs_84_point.wgs_X_lon = lon;
		wgs_84_point.wgs_Y_lat = lat;
		
		sk_42_point.sk42_Y_lat = Wgs84_SK42_Lat(lat, lon, height);
		sk_42_point.sk42_X_lon = WGS84_SK42_Long(lat, lon, height);
		//------------------------
		// найдем пространственные координаты
		B = sk_42_point.sk42_Y_lat * Pi / 180;
		L = sk_42_point.sk42_X_lon * Pi / 180;
		N = aP * pow( (1 - e * sin(B) * sin(B) ), -0.5);
	
		xyz_point.X = (N + height)* cos(B) * cos(L);
		xyz_point.Y = (N + height)* cos(B) * sin(L);
		xyz_point.Z = (N * (1 - e) + height) * sin(B);
		//---------------------------
		// рассчитаем плоские координаты
		X = 0;																// восточное смещение
		Y = 0;																// северное смещение
		geoCalc.WgsToPrGaus(lat, lon, &X, &Y);
				
		//---------------------------
		// определим точку в локальной системе координат
		// для теста используем приближенный алгоритм
		/*FuzzyWgsToLocPlkv(lat, lon, &X, &Y);
		kta_point.kta_X = X;
		kta_point.kta_Y = Y;*/

		// а теперь точный, и сравним результаты ...
		kta_point.kta_Y = Y - Y_ref;
		kta_point.kta_X = X - X_ref; 
	}
	else if(system_coord == SK_42)
	{
		sk_42_point.sk42_Y_lat = lat;
		sk_42_point.sk42_X_lon = lon;
		wgs_84_point.wgs_X_lon = SK42_WGS84_Long(lat, lon, height); 
		wgs_84_point.wgs_Y_lat = SK42_WGS84_Lat(lat, lon, height);
		//------------------------
		// определим пространственные координаты
		B = sk_42_point.sk42_Y_lat * Pi / 180;
		L = sk_42_point.sk42_X_lon * Pi / 180;
		N = aP * pow( (1 - e * sin(B) * sin(B) ), -0.5);
	
		xyz_point.X = (N + height)* cos(B) * cos(L);
		xyz_point.Y = (N + height)* cos(B) * sin(L);
		xyz_point.Z = (N * (1 - e) + height) * sin(B);
		//-----------------------------
		// вычислим плоские координаты
		X = 0;																	// восточное смещение
		Y = 0;																	// северное смещение
		geoCalc.WgsToPrGaus(wgs_84_point.wgs_Y_lat, wgs_84_point.wgs_X_lon, &X, &Y);
		
		//-------------------------
		// определим точку в локальной системе координат
		kta_point.kta_Y = Y - Y_ref;
		kta_point.kta_X = X - X_ref;
	}
	else if(system_coord == main_XYZ)
	{
		xyz_point.Y = lat;					// координата Y прямоугольной системе координат с нулем в центре элипсоида
		xyz_point.X = lon;					// координата X
		xyz_point.Z = Z;						// координата Z
		//-----------------------------------------
		if(XYZ_SK42(&xyz_point, &sk_42_point))
		{
			// сохраним и в системе WGS-84
			wgs_84_point.wgs_X_lon = SK42_WGS84_Long(sk_42_point.sk42_Y_lat, sk_42_point.sk42_X_lon, height); 
			wgs_84_point.wgs_Y_lat = SK42_WGS84_Lat(sk_42_point.sk42_Y_lat, sk_42_point.sk42_X_lon, height);
			// вычислим плоские координаты
			X = 0;								// восточное смещение
			Y = 0;								// северное смещение
			geoCalc.WgsToPrGaus(wgs_84_point.wgs_Y_lat, wgs_84_point.wgs_X_lon, &X, &Y);
		
			//-------------------------
			// определим точку в локальной системе координат
			kta_point.kta_Y = Y - Y_ref;
			kta_point.kta_X = X - X_ref;
		}		
	}
	else if(system_coord == CPA_GAUSE)
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
		/*xyz_point.Y = kta_point.kta_Y + xyz_point_cpa.Y;
		xyz_point.X = kta_point.kta_X + xyz_point_cpa.X;
		xyz_point.Z = xyz_point_cpa.Z;*/
		//----------------------
		////// определим точку в системе СК-42
		geoCalc.PrGausToSK42(lon, lat, pLat, pLon);
		// FuzzyPlkvLocToWgs(kta_point.kta_X, kta_point.kta_Y, pLat, pLon);
		sk_42_point.sk42_Y_lat = dLat;
		sk_42_point.sk42_X_lon = dLon;

		wgs_84_point.wgs_X_lon = SK42_WGS84_Long(dLat, dLon, height); 
		wgs_84_point.wgs_Y_lat = SK42_WGS84_Lat(dLat, dLon, height);
		//------------------------------------
		// определим пространственные координаты
		B = sk_42_point.sk42_Y_lat * Pi / 180;
		L = sk_42_point.sk42_X_lon * Pi / 180;
		N = aP * pow( (1 - e * sin(B) * sin(B) ), -0.5);
	
		xyz_point.X = (N + height)* cos(B) * cos(L);
		xyz_point.Y = (N + height)* cos(B) * sin(L);
		xyz_point.Z = (N * (1 - e) + height) * sin(B);
	}
	else
	{
		ern::Log::getInstance().Write("Unknown coordinate system");
		return;
	}
}
//--------------------------------------------------------------------------
// находит нужный корень долготы в системе координат CK-42 в системе уравнений 
int SimplifiedGeoCalc::Find_Root(double A, double B, double h, double z, double* m)
{
	int Count = 0;
	double Eps = 0.0000001;

	if (Diff(A, h, z) * Diff(B, h, z) > 0) 
		return 0;

	do
	{
		*m = (A + B) / 2;
		if (Diff(*m, h, z) * Diff(A, h, z) > 0) 
		{
			A = *m;
		}
		else
		{
			B = *m;
		}
		Count ++;
	} while (fabs(B - A) >= Eps);

	return 1;
}
//--------------------------------------------------------------------------
double SimplifiedGeoCalc::dB(double Bd, double Ld, double H)
{
	double B, L, M, N, dB;
	const double Pi = 3.14159265358979;		// число Пи
	const double ro = 206264.8062;				// Число угловых секунд в радиане
		
	B = Bd * Pi / 180;
	L = Ld * Pi / 180;
	M = a() * ( 1 - e2() ) / pow( (1 - e2() * sin(B) * sin(B) ), 1.5);
	N = a() * pow( (1 - e2() * sin(B) * sin(B) ), -0.5);

	double expr1 = ro / (M + H);
	double expr2 = N / a() * e2() * sin(B) * cos(B) * da();
	double expr3 = (N * N) / (a() * a() ) + 1;

	// dB = ro / (M + H)  *  (N / a() * e2() * sin(B) * cos(B) * da() + (N * N / a() * a() + 1) * N * sin(B) * cos(B) * de2() / 2 - (dx * cos(L) + dy * sin(L)) * sin(B) + dz * cos(B)) - wx * sin(L) * (1 + e2() * cos(2 * B)) + wy * cos(L) * (1 + e2() * cos(2 * B)) - ro * ms * e2() * sin(B) * cos(B);
	dB = expr1 * (expr2 + expr3 * N * sin(B) * cos(B) * de2() / 2 - (dx * cos(L) + dy * sin(L)) * sin(B) + dz * cos(B)) - wx * sin(L) * (1 + e2() * cos(2 * B)) + wy * cos(L) * (1 + e2() * cos(2 * B)) - ro * ms * e2() * sin(B) * cos(B);
	return dB;
}
//-------------------------------------------------------------------------
double SimplifiedGeoCalc::dL(double Bd, double Ld, double H)
{
	double B, L, N, dL;
	const double Pi = 3.14159265358979;		// число Пи
	const double ro = 206264.8062;				// Число угловых секунд в радиане
		
	B = Bd * Pi / 180;
	L = Ld * Pi / 180;
	N = a() * pow((1 - e2() * sin(B) * sin(B)), -0.5);
	dL = ro / ((N + H) * cos(B)) * (-dx * sin(L) + dy * cos(L))  + tan(B) * (1 - e2() ) * (wx * cos(L) + wy * sin(L)) - wz;
	return dL;
}
//---------------------------------------------------------------------------
//// пересчитывает координаты из прямоугольной (глобальной) XYZ системы в координаты в системе СК42
int SimplifiedGeoCalc::XYZ_SK42(TXYZSystemPoint *ptrXYZSystemPoint, TSK42Point *ptrSK42Point)
{
	double lat1, lat2, m, k, B, N, L, e, aP;
	int res;

	const double Pi = 3.14159265358979;		// число Пи

	lat1 = 59.0;
	lat2 = 61.0;
	aP = 6378245;
	e = 0.0066934216;
	m = 0.0;
	res = 1;		// пока все Ok!

	// xyz_point.Y = lat;			// координата Y прямоугольной системе координат с нулем в центре элипсоида
	// xyz_point.X = lon;			// координата X
	// xyz_point.Z = Z;				// координата Z

	if(Find_Root(lat1, lat2, ptrSK42Point->height, ptrXYZSystemPoint->Z, &m) )
	{
		B = m * Pi / 180;
		ptrSK42Point->sk42_Y_lat = m;
		k = sin(B);
		N = aP / sqrt(1 - e * k * k );  
		L = acos(ptrXYZSystemPoint->X / ( (N + ptrSK42Point->height) * cos(B) ) );
		ptrSK42Point->sk42_X_lon = L * 57.29578049;		// (перевод в градусы)
	}
	else
		res = 0;

	return res;
}
//---------------------------------------------------------------------------
//// пересчитывает координаты из системы СК42 в прямоугольную (глобальную) XYZ
void SimplifiedGeoCalc::SK42_XYZ(TSK42Point *ptrSK42Point, TXYZSystemPoint *ptrXYZSystemPoint)
{
	double B, L, N, aP;
	const double Pi = 3.14159265358979;		// число Пи
	const double e  = 0.0066934216;
	aP = 6378245;

	B = ptrSK42Point->sk42_Y_lat * Pi / 180;
	L = ptrSK42Point->sk42_X_lon * Pi / 180;
	N = aP * pow( (1 - e * sin(B) * sin(B) ), -0.5);
	
	ptrXYZSystemPoint->X = (N + ptrSK42Point->height) * cos(B) * cos(L);
	ptrXYZSystemPoint->Y = (N + ptrSK42Point->height) * cos(B) * sin(L);
	ptrXYZSystemPoint->Z = (N * (1 - e) + ptrSK42Point->height) * sin(B);
	//-------------------------
	// определим точку в локальной системе координат
	// kta_point.kta_Y = xyz_point.Y - xyz_point_cpa.Y;
	// kta_point.kta_X = xyz_point.X - xyz_point_cpa.X;
}
//---------------------------------------------------------------------------
double GeoCalc::WgsToPrMercEllipse_Y(double Lat)
{
	if(Lat > MAX_LAT)
		Lat = MAX_LAT;

	if(Lat < -MAX_LAT)
		Lat = -MAX_LAT;

	double phi = Lat * DEG_RAD;
	double con = ECCENT * sin(phi);
	con = pow( (1.0 - con) / (1 + con), ECCNTH * 0.5) ;

	return -R_MAJOR * log(tan(0.5 * (PI_2 - phi) ) / con );
}
//---------------------------------------------------------------------------
double GeoCalc::PrMercEllipseYToWgs_Lat(double Y)
{
	double ts = exp(-Y / R_MAJOR);
	double phi = PI_2 - 2.0 * atan(ts);

	int i = 0;
	double dPhi = 1;

	while( ( dPhi >= 0 ? dPhi : -dPhi) > 0.00000001 && i++ < 15 ){
		double con  = ECCENT * sin(phi);
		dPhi = PI_2 - 2.0 * atan(ts * pow( (1.0 - con) / (1.0 + con), ECCNTH) ) - phi;
		phi +=dPhi;
	}

	return phi * RAD_DEG;
}
//--------------------------------------------------------------------------
// Пересчет координат из широты/долготы в проекцию Меркатора/WGS84
std::string GeoCalc::LatLong2UTMMerc(double lon, double lat, double* pX, double* pY) {
	CoordinateUTMConversion c;
	return c.latLon2UTM(lat, lon, pX, pY);
}
//--------------------------------------------------------------------------
// переводит UTM в географические координаты WGS-84 (широту и долготу) 
void GeoCalc::UTM2LatLongMerc(std::string longZone, std::string latZone, double easting, double northing, double *pLat, double *pLon)
{
	CoordinateUTMConversion c;
	c.utm2LatLon(longZone, latZone, easting, northing, pLat, pLon);
}
//---------------------------------------------------------------------------
// переводит UTM в географические координаты WGS-84 (широту и долготу) 
void GeoCalc::UTM2LatLongMerc(std::string UTM, double *pLat, double *pLon)
{
	CoordinateUTMConversion c;
	c.utm2LatLon(UTM, pLat, pLon);
}
//--------------------------------------------------------------------------
double GeoCalc::WgsToPrMercDomain_Y(double Lat)
{
	double a = 6370997.0;

	if(Lat > MAX_LAT)
		Lat = MAX_LAT;

	if(Lat < -MAX_LAT)
		Lat = -MAX_LAT;

	double phi = Lat * DEG_RAD;
	double F6 = Pi/4;
	double F7 = phi/2;

	return a * log(tan(F6+F7) );
}
//---------------------------------------------------------------------------
double GeoCalc::PrMercDomainYToWgs_Lat(double Y)
{
	double ts = exp(-Y / R_MAJOR);
	double phi = PI_2 - 2.0 * atan(ts);

	int i = 0;
	double dPhi = 1;

	while( ( dPhi >= 0 ? dPhi : -dPhi) > 0.00000001 && i++ < 15 ){
		double con  = ECCENT * sin(phi);
		dPhi = PI_2 - 2.0 * atan(ts * pow( (1.0 - con) / (1.0 + con), ECCNTH) ) - phi;
		phi +=dPhi;
	}

	return phi * RAD_DEG;
}
//----------------------------------------------------------------------------
int GeoCalc::P3_90_02_SK42(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
{
	int res;
	// double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
	double B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, N_p3_90_02, N_sk_42, X_p3_90_02, Y_p3_90_02, Z_p3_90_02, X_sk_42, Y_sk_42, Z_sk_42, h_wgs;
	const double a_sk_42 = 6378245.0;							// размер большой полуоси в системе СК-42 в метрах
	const double a_p3_90_02 = 6378136.0;					// размер большой полуоси в системе П3-90.02 в метрах
	// const double a = (a_sk_42 + a_p3_90_02) * 0.5;		// средняя большая полуось
	// const double dA = a_sk_42 - a_p3_90_02;
	//-----------------------------
	const double alfa_sk_42 = 1/298.3;							// сжатие эллипсоида Красовского в системе СК-42
	const double alfa_p3_90_02 = 1/298.25784;			// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);							// квадрат эксцентриситета эллипсоида в системе СК-42
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	// const double e2 = (e_2_sk_42 + e_2_p3_90_02)/2;											// квадрат эксцентриситета
	// const double dE2 = e_2_sk_42 - e_2_p3_90_02;												// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX = -23.93;
	const double dY = 141.03;
	const double dZ = 79.98;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx = 0;
	const double Wy = 0.0000035;
	const double Wz = 0.0000079;
	//-------------------------------
	const double m = 0.0000022;
											
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	//double M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	//double N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
	N_p3_90_02 = a_p3_90_02 * pow((1 - e_2_p3_90_02 * sin2Lat), -0.5);

	res = 1;		// пока все Ок!

	//------------------------------------------------
	// реализация через переход к пространственным координатам X, Y, Z
	X_p3_90_02 = (N_p3_90_02 + H) * cosLat * cosLon;
	Y_p3_90_02 = (N_p3_90_02 + H) * cosLat * sinLon;
	Z_p3_90_02 = (N_p3_90_02 * (1 - e_2_p3_90_02) + H) * sinLat;

	X_sk_42 = (1 + m) * (X_p3_90_02  + Wz * Y_p3_90_02 + Wy * Z_p3_90_02) + dX;
	Y_sk_42 = (1 + m) * (-X_p3_90_02 * Wz + Y_p3_90_02 + Wx * Z_p3_90_02)  + dY;
	Z_sk_42 = (1 + m) * (Wy * X_p3_90_02 - Wx * Y_p3_90_02 + Z_p3_90_02) + dZ;

	//-----------------------------------------------------
	double lat1 = 59.0;
	double lat2 = 61.0;
	
	L = 0;
	
	if(abs(X_sk_42) > 0.000001 && (abs(Y_sk_42/X_sk_42) < Pi/2 - 0.000001 || abs(Y_sk_42/X_sk_42) > Pi/2 + 0.000001) )
		L = atan(Y_sk_42/X_sk_42);
	else
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	sinLon = sin(L);
	cosLon = cos(L);

	if(abs(sinLon) < 0.000001 || abs(cosLon) < 0.000001)
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	if(Find_Root(lat1, lat2, cosLon, Z_sk_42, e_2_sk_42, a_sk_42, X_sk_42, &B) )
	{
		*pLatOut = B;
		B = B * Pi / 180;
		sinLat = sin(B);
		cosLat = cos(B);

		if( abs(cosLat) < 0.000001)
		{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
		}

		N_sk_42 = a_sk_42 / sqrt(1 - e_2_sk_42 * pow(sinLat, 2) ); 
		h_wgs = X_sk_42/(cosLat * cosLon) - N_sk_42;
		
		*pLonOut = L * RAD_DEG;// 57.29578049;		// (перевод в градусах)
		*ph_wgs = h_wgs;
	}
	else
	{
		res = 0;
		*pLonOut = -100000.0;
		*pLatOut = -100000.0;
		*ph_wgs = -100000.0;
	}	

	return res;
}
//---------------------------------------------------------------
int GeoCalc::SK42_P3_90_02(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
{
	int res;
	// double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
	double B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, N_p3_90_02, N_sk_42, X_p3_90_02, Y_p3_90_02, Z_p3_90_02, X_sk_42, Y_sk_42, Z_sk_42, h_wgs;
	const double a_sk_42 = 6378245.0;							// размер большой полуоси в системе СК-42 в метрах
	const double a_p3_90_02 = 6378136.0;					// размер большой полуоси в системе П3-90.02 в метрах
	// const double a = (a_sk_42 + a_p3_90_02) * 0.5;		// средняя большая полуось
	// const double dA = a_sk_42 - a_p3_90_02;
	//-----------------------------
	const double alfa_sk_42 = 1/298.3;							// сжатие эллипсоида Красовского в системе СК-42
	const double alfa_p3_90_02 = 1/298.25784;			// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);							// квадрат эксцентриситета эллипсоида в системе СК-42
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	// const double e2 = (e_2_sk_42 + e_2_p3_90_02)/2;											// квадрат эксцентриситета
	// const double dE2 = e_2_sk_42 - e_2_p3_90_02;												// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX = 23.93;
	const double dY = -141.03;
	const double dZ = -79.98;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx = 0;
	const double Wy = -0.0000035;
	const double Wz = -0.0000079;
	//-------------------------------
	const double m = -0.0000022;
											
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	//double M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	//double N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
	N_p3_90_02 = a_p3_90_02 * pow((1 - e_2_p3_90_02 * sin2Lat), -0.5);

	res = 1;		// пока все Ок!

	//------------------------------------------------
	// реализация через переход к пространственным координатам X, Y, Z
	X_p3_90_02 = (N_p3_90_02 + H) * cosLat * cosLon;
	Y_p3_90_02 = (N_p3_90_02 + H) * cosLat * sinLon;
	Z_p3_90_02 = (N_p3_90_02 * (1 - e_2_p3_90_02) + H) * sinLat;

	X_sk_42 = (1 + m) * (X_p3_90_02  + Wz * Y_p3_90_02 + Wy * Z_p3_90_02) + dX;
	Y_sk_42 = (1 + m) * (-X_p3_90_02 * Wz + Y_p3_90_02 + Wx * Z_p3_90_02)  + dY;
	Z_sk_42 = (1 + m) * (Wy * X_p3_90_02 - Wx * Y_p3_90_02 + Z_p3_90_02) + dZ;

	//-----------------------------------------------------
	double lat1 = 59.0;
	double lat2 = 61.0;
	
	L = 0;
	
	if(abs(X_sk_42) > 0.000001 && (abs(Y_sk_42/X_sk_42) < Pi/2 - 0.000001 || abs(Y_sk_42/X_sk_42) > Pi/2 + 0.000001) )
		L = atan(Y_sk_42/X_sk_42);
	else
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	sinLon = sin(L);
	cosLon = cos(L);

	if(abs(sinLon) < 0.000001 || abs(cosLon) < 0.000001)
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	if(Find_Root(lat1, lat2, cosLon, Z_sk_42, e_2_sk_42, a_sk_42, X_sk_42, &B) )
	{
		*pLatOut = B;
		B = B * Pi / 180;
		sinLat = sin(B);
		cosLat = cos(B);

		if( abs(cosLat) < 0.000001)
		{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
		}

		N_sk_42 = a_sk_42 / sqrt(1 - e_2_sk_42 * pow(sinLat, 2) ); 
		h_wgs = X_sk_42/(cosLat * cosLon) - N_sk_42;
		
		*pLonOut = L * RAD_DEG;// 57.29578049;		// (перевод в градусах)
		*ph_wgs = h_wgs;
	}
	else
	{
		res = 0;
		*pLonOut = -100000.0;
		*pLatOut = -100000.0;
		*ph_wgs = -100000.0;
	}	

	return res;
}
//---------------------------------------------------------------
// Преобразование геодезических координат из системы П3-90.02 в систему СК-42
// пересчет широты
//double GeoCalc::P3_90_02_SK42_Lat(double Lat, double Lon, double H)
//{
//	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
//	const double a_sk_42 = 6378245.0;							// размер большой полуоси в системе СК-42 в метрах
//	const double a_p3_90_02 = 6378136.0;					// размер большой полуоси в системе П3-90.02 в метрах
//	const double a = (a_sk_42 + a_p3_90_02) * 0.5;		// средняя большая полуось
//	const double dA = a_sk_42 - a_p3_90_02;
//	//-----------------------------
//	const double alfa_sk_42 = 1/298.3;							// сжатие эллипсоида Красовского в системе СК-42
//	const double alfa_p3_90_02 = 1/298.25784;			// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
//	//-----------------------------
//	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);							// квадрат эксцентриситета эллипсоида в системе СК-42
//	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
//	const double e2 = (e_2_sk_42 + e_2_p3_90_02)/2;											// квадрат эксцентриситета
//	const double dE2 = e_2_sk_42 - e_2_p3_90_02;												// разность квадратов эксцентриситета
//	//-------------------------------
//	// линейные элементы трансформирования систем координат
//	const double dX = -23.93;
//	const double dY = 141.03;
//	const double dZ = 79.98;
//	//-------------------------------
//	// угловые элементы трансформирования систем координат
//	const double Wx = 0;
//	const double Wy = 0.35;
//	const double Wz = 0.79;
//	//-------------------------------
//	const double m = 0.0000022;
//	//-------------------------------
//	const double ro = 206264.8062;								// число угловых секунд в радиане
//	B = Lat * DEG_RAD;												// широта в радианах
//	L = Lon * DEG_RAD;												// долгота в радианах
//	sinLat = sin(B);
//	cosLat = cos(B);
//	cosLon = cos(L);
//	sinLon = sin(L);
//
//	sin2Lat = pow(sinLat, 2);
//
//	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	/*dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2.0) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
//		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;
//
//	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;*/
//
//	// далее идет расчет промежуточных вспомогательных переменных
//	
//	double var_temp1 = ro/(M + H);
//	double var_temp2 =	(N/a) * e2 * sinLat * cosLat * dA;
//	double var_temp3 = pow(N, 2)/pow(a, 2) + 1;
//	double var_temp4 = var_temp3 * N * sinLat * cosLat * dE2/2.0;
//	double var_temp5 = dX * cosLon + dY * sinLon;
//	double var_temp6 = 1 + e2 * cos(2.0 * B);
//	double var_temp7 = Wx * sinLon * var_temp6;
//	double var_temp8 =	Wy * cosLon * var_temp6;
//	double var_temp9 =	ro * m * e2 * sinLat * cosLat;
//
//
//	/*dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2.0 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
//		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;*/
//
//	dB = var_temp1 * ( var_temp2 + var_temp4 - var_temp5 * sinLat + dZ * cosLat) -	var_temp7 + var_temp8 - var_temp9;
//
//	// dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	// dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;
//
//	B = B + dB;
//	// L = L + dL;
//	// H = H + dH;
//
//	/*B = B + dB;
//	L = L + dL;
//	H = H + dH;*/
//
//	// для уменьшения погрешности делаем вторую итерацию
//	//sinLat = sin(B);
//	//cosLat = cos(B);
//	//cosLon = cos(L);
//	//sinLon = sin(L);
//
//	//sin2Lat = pow(sinLat, 2);
//	//M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	//N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	//dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
//	//	Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;
//
//	//B = (B + (B + dB))/2;
//
//	return B * RAD_DEG;
//}
//----------------------------------------------------------------------------
// Преобразование геодезических координат из системы П3-90.02 в систему СК-42
// пересчет долготы
//double GeoCalc::P3_90_02_SK42_Lon(double Lat, double Lon, double H)
//{
//	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
//	const double a_sk_42 = 6378245.0;							// размер большой полуоси в системе СК-42 в метрах
//	const double a_p3_90_02 = 6378136.0;					// размер большой полуоси в системе П3-90.02 в метрах
//	const double a = (a_sk_42 + a_p3_90_02) * 0.5;		// средняя большая полуось
//	const double dA = a_sk_42 - a_p3_90_02;
//	//-----------------------------
//	const double alfa_sk_42 = 1/298.3;							// сжатие эллипсоида Красовского в системе СК-42
//	const double alfa_p3_90_02 = 1/298.25784;			// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
//	//-----------------------------
//	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);							// квадрат эксцентриситета эллипсоида в системе СК-42
//	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
//	const double e2 = (e_2_sk_42 + e_2_p3_90_02)/2;											// квадрат эксцентриситета
//	const double dE2 = e_2_sk_42 - e_2_p3_90_02;												// разность квадратов эксцентриситета
//	//-------------------------------
//	// линейные элементы трансформирования систем координат
//	const double dX = -23.93;
//	const double dY = 141.03;
//	const double dZ = 79.98;
//	//-------------------------------
//	// угловые элементы трансформирования систем координат
//	const double Wx = 0;
//	const double Wy = 0.35;
//	const double Wz = 0.79;
//	//-------------------------------
//	const double m = 0.0000022;
//	//-------------------------------
//	const double ro = 206264.8062;								// число угловых секунд в радиане
//	B = Lat * DEG_RAD;												// широта в радианах
//	L = Lon * DEG_RAD;												// долгота в радианах
//	sinLat = sin(B);
//	cosLat = cos(B);
//	cosLon = cos(L);
//	sinLon = sin(L);
//
//	sin2Lat = pow(sinLat, 2);
//	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	/*dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
//		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;
//
//	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;*/
//
//	// далее идет расчет промежуточных вспомогательных переменных
//	double var_temp1 = ro/((N + H) * cosLat);
//	double var_temp2 = dY * cosLon - dX * sinLon;
//	double var_temp3 = tan(B);
//	double var_temp4 = Wx * cosLon + Wy * sinLon;
//	
//	dL = var_temp1 * var_temp2 + var_temp3 * (1 - e2) * var_temp4 - Wz;
//
//	
//	// B = B + dB;
//	L = L + dL;
//	// H = H + dH;
//
//	// для уменьшения погрешности делаем вторую итерацию
//	//sinLat = sin(B);
//	//cosLat = cos(B);
//	//cosLon = cos(L);
//	//sinLon = sin(L);
//
//	//sin2Lat = pow(sinLat, 2);
//	//M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	//N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	//dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	//L= (L + (L + dL))/2;
//
//	return L * RAD_DEG;
//}
//---------------------------------------------------------
// Преобразование геодезических координат из системы СК-42 в систему П3-90.02
// пересчет широты
double GeoCalc::SK42_P3_90_02_Lat(double Lat, double Lon, double H)
{
	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
	const double a_sk_42 = 6378245.0;							// размер большой полуоси в системе СК-42 в метрах
	const double a_p3_90_02 = 6378136.0;					// размер большой полуоси в системе П3-90.02 в метрах
	const double a = (a_sk_42 + a_p3_90_02) * 0.5;		// средняя большая полуось
	const double dA = a_sk_42 - a_p3_90_02;
	//-----------------------------
	const double alfa_sk_42 = 1/298.3;							// сжатие эллипсоида Красовского в системе СК-42
	const double alfa_p3_90_02 = 1/298.25784;			// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);							// квадрат эксцентриситета эллипсоида в системе СК-42
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	const double e2 = (e_2_sk_42 + e_2_p3_90_02)/2;											// квадрат эксцентриситета
	const double dE2 = e_2_sk_42 - e_2_p3_90_02;												// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX = 23.93;
	const double dY = -141.03;
	const double dZ = -79.98;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx = 0;
	const double Wy = -0.35;
	const double Wz = -0.79;
	//-------------------------------
	const double m = -22000.0;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;

	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;

	dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;

	B = B + dB;
	L = L + dL;
	H = H + dH;

	// для уменьшения погрешности делаем вторую итерацию
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;

	B = (B + (B + dB))/2;

	return B * RAD_DEG;
}
//----------------------------------------------------------------
// Преобразование геодезических координат из системы СК-42 в систему П3-90.02
// пересчет долготы
double GeoCalc::SK42_P3_90_02_Lon(double Lat, double Lon, double H)
{
	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
	const double a_sk_42 = 6378245.0;							// размер большой полуоси в системе СК-42 в метрах
	const double a_p3_90_02 = 6378136.0;					// размер большой полуоси в системе П3-90.02 в метрах
	const double a = (a_sk_42 + a_p3_90_02) * 0.5;		// средняя большая полуось
	const double dA = a_sk_42 - a_p3_90_02;
	//-----------------------------
	const double alfa_sk_42 = 1/298.3;							// сжатие эллипсоида Красовского в системе СК-42
	const double alfa_p3_90_02 = 1/298.25784;			// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);							// квадрат эксцентриситета эллипсоида в системе СК-42
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	const double e2 = (e_2_sk_42 + e_2_p3_90_02)/2;											// квадрат эксцентриситета
	const double dE2 = e_2_sk_42 - e_2_p3_90_02;												// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX = 23.93;
	const double dY = -141.03;
	const double dZ = -79.98;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx = 0;
	const double Wy = -0.35;
	const double Wz = -0.79;
	//-------------------------------
	const double m = -22000.0;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;

	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;

	dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;

	B = B + dB;
	L = L + dL;
	H = H + dH;

	// для уменьшения погрешности делаем вторую итерацию
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;

	L= (L + (L + dL))/2;

	return L * RAD_DEG;
}
//-----------------------------------------------------
// Преобразование геодезических координат из системы П3-90.02 в систему WGS-84
// пересчет широты
int GeoCalc::P3_90_02_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
{
	int res;
	double B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, N_p3_90_02, N_wgs_84, X_p3_90_02, Y_p3_90_02, Z_p3_90_02, X_wgs_84, Y_wgs_84, Z_wgs_84, h_wgs;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_p3_90_02 = 6378136.0;						// размер большой полуоси в системе П3-90.02 в метрах
	//const double a = (a_wgs_84 + a_p3_90_02) * 0.5;		// средняя большая полуось
	//const double dA = a_wgs_84 - a_p3_90_02;
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_p3_90_02 = 1/298.25784;				// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);					// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	//const double e2 = (e_2_wgs_84 + e_2_p3_90_02)/2;									// квадрат эксцентриситета
	//const double dE2 = e_2_wgs_84 - e_2_p3_90_02;										// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX =	-0.03;
	const double dY =	-0.27;
	const double dZ =	-0.92;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx =	 0;
	const double Wy =  0;
	const double Wz =	-0.0000007;
	//-------------------------------
	const double m = 0.000001;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	//double M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	//double N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
	N_p3_90_02 = a_p3_90_02 * pow((1 - e_2_p3_90_02 * sin2Lat), -0.5);

	res = 1;		// пока все Ок!

	//------------------------------------------------
	// реализация через переход к пространственным координатам X, Y, Z
	X_p3_90_02 = (N_p3_90_02 + H) * cosLat * cosLon;
	Y_p3_90_02 = (N_p3_90_02 + H) * cosLat * sinLon;
	Z_p3_90_02 = (N_p3_90_02 * (1 - e_2_p3_90_02) + H) * sinLat;

	X_wgs_84 = (1 + m) * (X_p3_90_02  + Wz * Y_p3_90_02 + Wy * Z_p3_90_02) + dX;
	Y_wgs_84 = (1 + m) * (-X_p3_90_02 * Wz + Y_p3_90_02 + Wx * Z_p3_90_02)  + dY;
	Z_wgs_84 = (1 + m) * (Wy * X_p3_90_02 - Wx * Y_p3_90_02 + Z_p3_90_02) + dZ;

	//-----------------------------------------------------
	double lat1 = 59.0;
	double lat2 = 61.0;
	
	L = 0;
	
	if(abs(X_wgs_84) > 0.000001 && (abs(Y_wgs_84/X_wgs_84) < Pi/2 - 0.000001 || abs(Y_wgs_84/X_wgs_84) > Pi/2 + 0.000001) )
		L = atan(Y_wgs_84/X_wgs_84);
	else
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	sinLon = sin(L);
	cosLon = cos(L);

	if(abs(sinLon) < 0.000001 || abs(cosLon) < 0.000001)
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	if(Find_Root(lat1, lat2, cosLon, Z_wgs_84, e_2_wgs_84, a_wgs_84, X_wgs_84, &B) )
	{
		*pLatOut = B;
		B = B * Pi / 180;
		sinLat = sin(B);
		cosLat = cos(B);

		if( abs(cosLat) < 0.000001)
		{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
		}

		N_wgs_84 = a_wgs_84 / sqrt(1 - e_2_wgs_84 * pow(sinLat, 2) ); 
		h_wgs = X_wgs_84/(cosLat * cosLon) - N_wgs_84;
		
		*pLonOut = L * RAD_DEG;// 57.29578049;		// (перевод в градусах)
		*ph_wgs = h_wgs;
	}
	else
	{
		res = 0;
		*pLonOut = -100000.0;
		*pLatOut = -100000.0;
		*ph_wgs = -100000.0;
	}	

	return res;
}
//-----------------------------------------------------
// реализация предыдущего варианта для определения оптимальных параметров по МНК
int GeoCalc::P3_90_02_WGS_84(double Lat, double Lon, double H, double dX, double dY, double dZ, double Wx, double Wy, double Wz, double m, double *ph_wgs, double *pLatOut, double *pLonOut)
{
	int res;
	double B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, N_p3_90_02, N_wgs_84, X_p3_90_02, Y_p3_90_02, Z_p3_90_02, X_wgs_84, Y_wgs_84, Z_wgs_84, h_wgs;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_p3_90_02 = 6378136.0;						// размер большой полуоси в системе П3-90.02 в метрах
	//const double a = (a_wgs_84 + a_p3_90_02) * 0.5;		// средняя большая полуось
	//const double dA = a_wgs_84 - a_p3_90_02;
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_p3_90_02 = 1/298.25784;				// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);					// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	//const double e2 = (e_2_wgs_84 + e_2_p3_90_02)/2;									// квадрат эксцентриситета
	//const double dE2 = e_2_wgs_84 - e_2_p3_90_02;										// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	// const double dX = -0.03;
	// const double dY = -0.27;
	// const double dZ = -0.92;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	// const double Wx = 0;
	// const double Wy = 0;
	// const double Wz = -0.0000007;
	//-------------------------------
	// const double m = 0.000001;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	//double M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	//double N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
	N_p3_90_02 = a_p3_90_02 * pow((1 - e_2_p3_90_02 * sin2Lat), -0.5);

	res = 1;		// пока все Ок!

	//------------------------------------------------
	// реализация через переход к пространственным координатам X, Y, Z
	X_p3_90_02 = (N_p3_90_02 + H) * cosLat * cosLon;
	Y_p3_90_02 = (N_p3_90_02 + H) * cosLat * sinLon;
	Z_p3_90_02 = (N_p3_90_02 * (1 - e_2_p3_90_02) + H) * sinLat;

	X_wgs_84 = (1 + m) * (X_p3_90_02  + Wz * Y_p3_90_02 + Wy * Z_p3_90_02) + dX;
	Y_wgs_84 = (1 + m) * (-X_p3_90_02 * Wz + Y_p3_90_02 + Wx * Z_p3_90_02)  + dY;
	Z_wgs_84 = (1 + m) * (Wy * X_p3_90_02 - Wx * Y_p3_90_02 + Z_p3_90_02) + dZ;

	//-----------------------------------------------------
	double lat1 = 59.0;
	double lat2 = 61.0;
	
	L = 0;
	
	if(abs(X_wgs_84) > 0.000001 && (abs(Y_wgs_84/X_wgs_84) < Pi/2 - 0.000001 || abs(Y_wgs_84/X_wgs_84) > Pi/2 + 0.000001) )
		L = atan(Y_wgs_84/X_wgs_84);
	else
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	sinLon = sin(L);
	cosLon = cos(L);

	if(abs(sinLon) < 0.000001 || abs(cosLon) < 0.000001)
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	if(Find_Root(lat1, lat2, cosLon, Z_wgs_84, e_2_wgs_84, a_wgs_84, X_wgs_84, &B) )
	{
		*pLatOut = B;
		B = B * Pi / 180;
		sinLat = sin(B);
		cosLat = cos(B);

		if( abs(cosLat) < 0.000001)
		{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
		}

		N_wgs_84 = a_wgs_84 / sqrt(1 - e_2_wgs_84 * pow(sinLat, 2) ); 
		h_wgs = X_wgs_84/(cosLat * cosLon) - N_wgs_84;
		
		*pLonOut = L * RAD_DEG;// 57.29578049;		// (перевод в градусах)
		*ph_wgs = h_wgs;
	}
	else
	{
		res = 0;
		*pLonOut = -100000.0;
		*pLatOut = -100000.0;
		*ph_wgs = -100000.0;
	}	

	return res;
}
//-----------------------------------------------------
// Преобразование геодезических координат из системы П3-90.02 в систему WGS-84
// через систему П3-90.11
// пересчет широты, долготы и высоты
int GeoCalc::P3_90_02_P3_90_11_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
{
	int res;
	double B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, N_p3_90_02, N_wgs_84;
	double X_p3_90_02, Y_p3_90_02, Z_p3_90_02, X_p3_90_11, Y_p3_90_11, Z_p3_90_11, X_wgs_84, Y_wgs_84, Z_wgs_84, h_wgs;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_p3_90_02 = 6378136.0;						// размер большой полуоси в системе П3-90.02 в метрах
	//const double a = (a_wgs_84 + a_p3_90_02) * 0.5;		// средняя большая полуось
	//const double dA = a_wgs_84 - a_p3_90_02;
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_p3_90_02 = 1/298.25784;				// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);					// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	//const double e2 = (e_2_wgs_84 + e_2_p3_90_02)/2;									// квадрат эксцентриситета
	//const double dE2 = e_2_wgs_84 - e_2_p3_90_02;										// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX1 = -0.373;
	const double dY1 = 0.186;
	const double dZ1 = 0.202;
	//-------------------------------
	const double dX2 = -0.013;
	const double dY2 = 0.106;
	const double dZ2 = 0.022;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx = -0.0000001115;
	const double Wy = 0.0000001716;
	const double Wz = -0.0000002041;
	//-------------------------------
	const double m = -0.00000008;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;											// широта в радианах
	L = Lon * DEG_RAD;											// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	//double M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	//double N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
	N_p3_90_02 = a_p3_90_02 * pow((1 - e_2_p3_90_02 * sin2Lat), -0.5);

	res = 1;		// пока все Ок!

	//------------------------------------------------
	// реализация через переход к пространственным координатам X, Y, Z
	X_p3_90_02 = (N_p3_90_02 + H) * cosLat * cosLon;
	Y_p3_90_02 = (N_p3_90_02 + H) * cosLat * sinLon;
	Z_p3_90_02 = (N_p3_90_02 * (1 - e_2_p3_90_02) + H) * sinLat;

	X_p3_90_11 = (1 + m) * (X_p3_90_02  + Wz * Y_p3_90_02 - Wy * Z_p3_90_02) + dX1;
	Y_p3_90_11 = (1 + m) * (Y_p3_90_02 - X_p3_90_02 * Wz + Wx * Z_p3_90_02)  + dY1;
	Z_p3_90_11 = (1 + m) * (Wy * X_p3_90_02 - Wx * Y_p3_90_02 + Z_p3_90_02) + dZ1;
	//////////////////////////////////////////
	//------------------------------------------------
	X_wgs_84 = (1 - m) * (X_p3_90_11  - Wz * Y_p3_90_11 + Wy * Z_p3_90_11) - dX2;
	Y_wgs_84 = (1 - m) * (X_p3_90_11 * Wz + Y_p3_90_11 - Wx * Z_p3_90_11)  - dY2;
	Z_wgs_84 = (1 - m) * (Wx * Y_p3_90_11 - Wy * X_p3_90_11 + Z_p3_90_11) - dZ2;

	//-----------------------------------------------------
	double lat1 = 59.0;
	double lat2 = 61.0;
	
	L = 0;
	
	if(abs(X_wgs_84) > 0.000001 && (abs(Y_wgs_84/X_wgs_84) < Pi/2 - 0.000001 || abs(Y_wgs_84/X_wgs_84) > Pi/2 + 0.000001) )
		L = atan(Y_wgs_84/X_wgs_84);
	else
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	sinLon = sin(L);
	cosLon = cos(L);

	if(abs(sinLon) < 0.000001 || abs(cosLon) < 0.000001)
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	if(Find_Root(lat1, lat2, cosLon, Z_wgs_84, e_2_wgs_84, a_wgs_84, X_wgs_84, &B) )
	{
		*pLatOut = B;
		B = B * Pi / 180;
		sinLat = sin(B);
		cosLat = cos(B);

		if( abs(cosLat) < 0.000001)
		{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
		}

		N_wgs_84 = a_wgs_84 / sqrt(1 - e_2_wgs_84 * pow(sinLat, 2) ); 
		h_wgs = X_wgs_84/(cosLat * cosLon) - N_wgs_84;
		
		*pLonOut = L * RAD_DEG;// 57.29578049;		// (перевод в градусах)
		*ph_wgs = h_wgs;
	}
	else
	{
		res = 0;
		*pLonOut = -100000.0;
		*pLatOut = -100000.0;
		*ph_wgs = -100000.0;
	}	

	return res;
}
//-----------------------------------------------------
//// Преобразование геодезических координат из системы П3-90.02 в систему WGS-84
//// пересчет широты
//double GeoCalc::P3_90_02_WGS_84_Lat(double Lat, double Lon, double H)
//{
//	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
//	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
//	const double a_p3_90_02 = 6378136.0;						// размер большой полуоси в системе П3-90.02 в метрах
//	const double a = (a_wgs_84 + a_p3_90_02) * 0.5;		// средняя большая полуось
//	const double dA = a_wgs_84 - a_p3_90_02;
//	//-----------------------------
//	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
//	const double alfa_p3_90_02 = 1/298.25784;				// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
//	//-----------------------------
//	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);					// квадрат эксцентриситета эллипсоида в системе WGS-84
//	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
//	const double e2 = (e_2_wgs_84 + e_2_p3_90_02)/2;										// квадрат эксцентриситета
//	const double dE2 = e_2_wgs_84 - e_2_p3_90_02;											// разность квадратов эксцентриситета
//	//-------------------------------
//	// линейные элементы трансформирования систем координат
//	const double dX = -0.03;
//	const double dY = -0.27;
//	const double dZ = -0.92;
//	//-------------------------------
//	// угловые элементы трансформирования систем координат
//	const double Wx = 0;
//	const double Wy = 0;
//	const double Wz = -0.0000007;
//	//-------------------------------
//	const double m = 0.000001;
//	//-------------------------------
//	const double ro = 206264.8062;								// число угловых секунд в радиане
//	B = Lat * DEG_RAD;												// широта в радианах
//	L = Lon * DEG_RAD;												// долгота в радианах
//	sinLat = sin(B);
//	cosLat = cos(B);
//	cosLon = cos(L);
//	sinLon = sin(L);
//
//	sin2Lat = pow(sinLat, 2);
//	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	// далее идет расчет промежуточных вспомогательных переменных
//	double var_temp1 = ro/(M + H);
//	double var_temp2 =	(N/a) * e2 * sinLat * cosLat * dA;
//	double var_temp3 = pow(N, 2)/pow(a, 2) + 1;
//	double var_temp4 = var_temp3 * N * sinLat * cosLat * dE2/2.0;
//	double var_temp5 = dX * cosLon + dY * sinLon;
//	double var_temp6 = 1 + e2 * cos(2.0 * B);
//	double var_temp7 = Wx * sinLon * var_temp6;
//	double var_temp8 =	Wy * cosLon * var_temp6;
//	double var_temp9 =	ro * m * e2 * sinLat * cosLat;
//
//	/*dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
//		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;*/
//
//	// dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	dB = var_temp1 * ( var_temp2 + var_temp4 - var_temp5 * sinLat + dZ * cosLat) -	var_temp7 + var_temp8 - var_temp9;
//	// dL = var_temp1 * var_temp2 + var_temp3 * (1 - e2) * var_temp4 - Wz;
//
//	/*dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;*/
//
//	B = B + dB;
//	// L = L + dL;
//	// H = H + dH;
//
//	// для уменьшения погрешности делаем вторую итерацию
//	//sinLat = sin(B);
//	//cosLat = cos(B);
//	//cosLon = cos(L);
//	//sinLon = sin(L);
//
//	//sin2Lat = pow(sinLat, 2);
//	//M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	//N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	//dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	//L = (L + (L + dL))/2;
//
//	return B * RAD_DEG;
//}
//-----------------------------------------------------
////// Преобразование геодезических координат из системы П3-90.02 в систему WGS-84
////// пересчет долготы
//double GeoCalc::P3_90_02_WGS_84_Lon(double Lat, double Lon, double H)
//{
//	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
//	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
//	const double a_p3_90_02 = 6378136.0;						// размер большой полуоси в системе П3-90.02 в метрах
//	const double a = (a_wgs_84 + a_p3_90_02) * 0.5;		// средняя большая полуось
//	const double dA = a_wgs_84 - a_p3_90_02;
//	//-----------------------------
//	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
//	const double alfa_p3_90_02 = 1/298.25784;				// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
//	//-----------------------------
//	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);					// квадрат эксцентриситета эллипсоида в системе WGS-84
//	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
//	const double e2 = (e_2_wgs_84 + e_2_p3_90_02)/2;										// квадрат эксцентриситета
//	const double dE2 = e_2_wgs_84 - e_2_p3_90_02;											// разность квадратов эксцентриситета
//	//-------------------------------
//	// линейные элементы трансформирования систем координат
//	const double dX = -0.03;
//	const double dY = -0.27;
//	const double dZ = -0.92;
//	//-------------------------------
//	// угловые элементы трансформирования систем координат
//	const double Wx = 0;
//	const double Wy = 0;
//	const double Wz = -0.0000007;
//	//-------------------------------
//	const double m = 0.000001;
//	//-------------------------------
//	const double ro = 206264.8062;								// число угловых секунд в радиане
//	B = Lat * DEG_RAD;												// широта в радианах
//	L = Lon * DEG_RAD;												// долгота в радианах
//	sinLat = sin(B);
//	cosLat = cos(B);
//	cosLon = cos(L);
//	sinLon = sin(L);
//
//	sin2Lat = pow(sinLat, 2);
//	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	// далее идет расчет промежуточных вспомогательных переменных
//	double var_temp1 = ro/((N + H) * cosLat);
//	double var_temp2 = dY * cosLon - dX * sinLon;
//	double var_temp3 = tan(B);
//	double var_temp4 = Wx * cosLon + Wy * sinLon;
//
//	/*dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
//		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;*/
//
//	// dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	
//	dL = var_temp1 * var_temp2 + var_temp3 * (1 - e2) * var_temp4 - Wz;
//
//	/*dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;*/
//
//	// B = B + dB;
//	L = L + dL;
//	// H = H + dH;
//
//	// для уменьшения погрешности делаем вторую итерацию
//	//sinLat = sin(B);
//	//cosLat = cos(B);
//	//cosLon = cos(L);
//	//sinLon = sin(L);
//
//	//sin2Lat = pow(sinLat, 2);
//	//M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	//N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	//dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	//L = (L + (L + dL))/2;
//
//	return L * RAD_DEG;
//}
//-----------------------------------------------------
// Преобразование геодезических координат из системы WGS-84 в систему П3-90.02
// пересчет широты, долготы и высоты
int GeoCalc::WGS_84_P3_90_02(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
{
	int res;
	double B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, N_p3_90_02, N_wgs_84, X_p3_90_02, Y_p3_90_02, Z_p3_90_02, X_wgs_84, Y_wgs_84, Z_wgs_84, h_wgs;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_p3_90_02 = 6378136.0;						// размер большой полуоси в системе П3-90.02 в метрах
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_p3_90_02 = 1/298.25784;				// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);					// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX =	0.03;
	const double dY =	0.27;
	const double dZ =	0.92;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx =	 0;
	const double Wy =  0;
	const double Wz =	0.0000007;
	//-------------------------------
	const double m = -0.000001;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	//double M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	//double N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
	N_p3_90_02 = a_p3_90_02 * pow((1 - e_2_p3_90_02 * sin2Lat), -0.5);

	res = 1;		// пока все Ок!

	//------------------------------------------------
	// реализация через переход к пространственным координатам X, Y, Z
	X_p3_90_02 = (N_p3_90_02 + H) * cosLat * cosLon;
	Y_p3_90_02 = (N_p3_90_02 + H) * cosLat * sinLon;
	Z_p3_90_02 = (N_p3_90_02 * (1 - e_2_p3_90_02) + H) * sinLat;

	X_wgs_84 = (1 + m) * (X_p3_90_02  + Wz * Y_p3_90_02 + Wy * Z_p3_90_02) + dX;
	Y_wgs_84 = (1 + m) * (-X_p3_90_02 * Wz + Y_p3_90_02 + Wx * Z_p3_90_02)  + dY;
	Z_wgs_84 = (1 + m) * (Wy * X_p3_90_02 - Wx * Y_p3_90_02 + Z_p3_90_02) + dZ;

	//-----------------------------------------------------
	double lat1 = 59.0;
	double lat2 = 61.0;
	
	L = 0;
	
	if(abs(X_wgs_84) > 0.000001 && (abs(Y_wgs_84/X_wgs_84) < Pi/2 - 0.000001 || abs(Y_wgs_84/X_wgs_84) > Pi/2 + 0.000001) )
		L = atan(Y_wgs_84/X_wgs_84);
	else
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	sinLon = sin(L);
	cosLon = cos(L);

	if(abs(sinLon) < 0.000001 || abs(cosLon) < 0.000001)
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	if(Find_Root(lat1, lat2, cosLon, Z_wgs_84, e_2_wgs_84, a_wgs_84, X_wgs_84, &B) )
	{
		*pLatOut = B;
		B = B * Pi / 180;
		sinLat = sin(B);
		cosLat = cos(B);

		if( abs(cosLat) < 0.000001)
		{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
		}

		N_wgs_84 = a_wgs_84 / sqrt(1 - e_2_wgs_84 * pow(sinLat, 2) ); 
		h_wgs = X_wgs_84/(cosLat * cosLon) - N_wgs_84;
		
		*pLonOut = L * RAD_DEG;		// 57.29578049;		// (перевод в градусах)
		*ph_wgs = h_wgs;
	}
	else
	{
		res = 0;
		*pLonOut = -100000.0;
		*pLatOut = -100000.0;
		*ph_wgs = -100000.0;
	}	

	return res;
}
//-----------------------------------------------------
// Преобразование геодезических координат из системы WGS-84 в систему П3-90.02
// пересчет долготы
//double GeoCalc::WGS_84_P3_90_02_Lon(double Lat, double Lon, double H)
//{
//	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
//	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
//	const double a_p3_90_02 = 6378136.0;						// размер большой полуоси в системе П3-90.02 в метрах
//	const double a = (a_wgs_84 + a_p3_90_02) * 0.5;		// средняя большая полуось
//	const double dA = a_wgs_84 - a_p3_90_02;
//	//-----------------------------
//	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
//	const double alfa_p3_90_02 = 1/298.25784;				// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
//	//-----------------------------
//	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);					// квадрат эксцентриситета эллипсоида в системе WGS-84
//	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
//	const double e2 = (e_2_wgs_84 + e_2_p3_90_02)/2;										// квадрат эксцентриситета
//	const double dE2 = e_2_wgs_84 - e_2_p3_90_02;											// разность квадратов эксцентриситета
//	//-------------------------------
//	// линейные элементы трансформирования систем координат
//	const double dX = 0.03;
//	const double dY = 0.27;
//	const double dZ = 0.92;
//	//-------------------------------
//	// угловые элементы трансформирования систем координат
//	const double Wx = 0;
//	const double Wy = 0;
//	const double Wz = 0.07;
//	//-------------------------------
//	const double m = -10000.0;
//	//-------------------------------
//	const double ro = 206264.8062;								// число угловых секунд в радиане
//	B = Lat * DEG_RAD;												// широта в радианах
//	L = Lon * DEG_RAD;												// долгота в радианах
//	sinLat = sin(B);
//	cosLat = cos(B);
//	cosLon = cos(L);
//	sinLon = sin(L);
//
//	sin2Lat = pow(sinLat, 2);
//	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
//		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;
//
//	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;
//
//	B = B + dB;
//	L = L + dL;
//	H = H + dH;
//
//	// для уменьшения погрешности делаем вторую итерацию
//	sinLat = sin(B);
//	cosLat = cos(B);
//	cosLon = cos(L);
//	sinLon = sin(L);
//
//	sin2Lat = pow(sinLat, 2);
//	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
//	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
//
//	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;
//
//	L = (L + (L + dL))/2;
//
//	return L * RAD_DEG;
//}
//-----------------------------------------------------
// Более точное преобразование геодезических координат из системы CK-42 в систему WGS-84
// пересчет широты
double GeoCalc::SK_42_Sup_WGS_84_Lat(double Lat, double Lon, double H)
{
	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_sk_42 = 6378245.0;								// размер большой полуоси в системе CK-42 в метрах
	const double a = (a_wgs_84 + a_sk_42) * 0.5;				// средняя большая полуось
	const double dA = a_wgs_84 - a_sk_42;						
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_sk_42 = 1/298.3;								// сжатие эллипсоида Красовского в системе СК-42
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);			// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);					// квадрат эксцентриситета эллипсоида в системе П3-90.02
	const double e2 = (e_2_wgs_84 + e_2_sk_42)/2;										// квадрат эксцентриситета
	const double dE2 = e_2_wgs_84 - e_2_sk_42;											// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX = 23.9;
	const double dY = -141.3;
	const double dZ = -80.9;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx = 0;
	const double Wy = -0.35;
	const double Wz = -0.86;
	//-------------------------------
	const double m = -12000.0;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;

	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;

	dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;

	B = B + dB;
	L = L + dL;
	H = H + dH;

	// для уменьшения погрешности делаем вторую итерацию
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;

	B = (B + (B + dB))/2;

	return B * RAD_DEG;
}
//-----------------------------------------------------
// Более точное преобразование геодезических координат из системы CK-42 в систему WGS-84
// пересчет долготы
double GeoCalc::SK_42_Sup_WGS_84_Lon(double Lat, double Lon, double H)
{
	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_sk_42 = 6378245.0;								// размер большой полуоси в системе CK-42 в метрах
	const double a = (a_wgs_84 + a_sk_42) * 0.5;				// средняя большая полуось
	const double dA = a_wgs_84 - a_sk_42;						
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_sk_42 = 1/298.3;								// сжатие эллипсоида Красовского в системе СК-42
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);			// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);					// квадрат эксцентриситета эллипсоида в системе П3-90.02
	const double e2 = (e_2_wgs_84 + e_2_sk_42)/2;										// квадрат эксцентриситета
	const double dE2 = e_2_wgs_84 - e_2_sk_42;											// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX = 23.9;
	const double dY = -141.3;
	const double dZ = -80.9;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx = 0;
	const double Wy = -0.35;
	const double Wz = -0.86;
	//-------------------------------
	const double m = -12000.0;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;

	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;

	dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;

	B = B + dB;
	L = L + dL;
	H = H + dH;

	// для уменьшения погрешности делаем вторую итерацию
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;

	L = (L + (L + dL))/2;

	return L * RAD_DEG;
}
//-----------------------------------------------------
// Более точное преобразование геодезических координат из системы WGS-84 в систему CK-42
// пересчет широты
double GeoCalc::WGS_84_Sup_Sk_42_Lat(double Lat, double Lon, double H)
{
	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_sk_42 = 6378245.0;								// размер большой полуоси в системе CK-42 в метрах
	const double a = (a_wgs_84 + a_sk_42) * 0.5;				// средняя большая полуось
	const double dA = a_wgs_84 - a_sk_42;						
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_sk_42 = 1/298.3;								// сжатие эллипсоида Красовского в системе СК-42
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);			// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);					// квадрат эксцентриситета эллипсоида в системе П3-90.02
	const double e2 = (e_2_wgs_84 + e_2_sk_42)/2;										// квадрат эксцентриситета
	const double dE2 = e_2_wgs_84 - e_2_sk_42;											// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX = -23.9;
	const double dY = 141.3;
	const double dZ = 80.9;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx = 0;
	const double Wy = 0.35;
	const double Wz = 0.86;
	//-------------------------------
	const double m = 12000.0;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;

	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;

	dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;

	B = B + dB;
	L = L + dL;
	H = H + dH;

	// для уменьшения погрешности делаем вторую итерацию
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;

	B = (B + (B + dB))/2;

	return B * RAD_DEG;
}
//-----------------------------------------------------
// Более точное преобразование геодезических координат из системы WGS-84 в систему CK-42
// пересчет долготы
double GeoCalc::WGS_84_Sup_Sk_42_Lon(double Lat, double Lon, double H)
{
	double dB, dL, dH, B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, M, N;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_sk_42 = 6378245.0;								// размер большой полуоси в системе CK-42 в метрах
	const double a = (a_wgs_84 + a_sk_42) * 0.5;				// средняя большая полуось
	const double dA = a_wgs_84 - a_sk_42;						
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_sk_42 = 1/298.3;								// сжатие эллипсоида Красовского в системе СК-42
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);			// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_sk_42 = 2 * alfa_sk_42 - pow(alfa_sk_42, 2);					// квадрат эксцентриситета эллипсоида в системе П3-90.02
	const double e2 = (e_2_wgs_84 + e_2_sk_42)/2;										// квадрат эксцентриситета
	const double dE2 = e_2_wgs_84 - e_2_sk_42;											// разность квадратов эксцентриситета
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX = -23.9;
	const double dY = 141.3;
	const double dZ = 80.9;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx = 0;
	const double Wy = 0.35;
	const double Wz = 0.86;
	//-------------------------------
	const double m = 12000.0;
	//-------------------------------
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dB = (ro/(M + H)) * ( (N/a) * e2 * sinLat * cosLat * dA + (pow(N, 2)/pow(a, 2) + 1 ) * N * sinLat * cosLat * dE2/2 - (dX * cosLon + dY * sinLon) * sinLat + dZ * cosLat) -
		Wx * sinLon * (1 + e2 * cos(2.0 * B)) + Wy * cosLon * (1 + e2 * cos(2.0 * B) ) - ro * m * e2 * sinLat * cosLat;

	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;

	dH = (-a/N) * dA + N * sin2Lat * (dE2/2) + (dX * cosLon + dY * sinLon) * cosLat + dZ * sinLat - N * e2 * sinLat * cosLat * ( (Wx/ro) * sinLon -  (Wy/ro) * cosLon ) + (pow(a, 2)/N + H ) * m;

	B = B + dB;
	L = L + dL;
	H = H + dH;

	// для уменьшения погрешности делаем вторую итерацию
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала

	dL = (ro/((N + H) * cosLat ) ) * (-dX * sinLon + dY * cosLon) + tan(B) * (1 - e2) * (Wx * cosLon + Wy * sinLon) - Wz;

	L = (L + (L + dL))/2;

	return L * RAD_DEG;
}
//---------------------------------------------------------
//	возвращает местоположение цели в минутах и секундах
void GeoCalc::GetTargetLocationInMinutes(TTargetLocationInMinutes* pTargetLocationInMinutes, const double lon_lat_dec)
{
	double x, y;
	
	pTargetLocationInMinutes->degrees = lon_lat_dec * 1;
	y = lon_lat_dec - pTargetLocationInMinutes->degrees;

	x = y * 3600.00;
	pTargetLocationInMinutes->minutes = x/60;
	pTargetLocationInMinutes->seconds = x - pTargetLocationInMinutes->minutes * 60.00;
}
//---------------------------------------------------------
//	возвращает местоположение цели в градусах в десятичных дробях 
double GeoCalc::GetDecTargetLocation(TTargetLocationInMinutes* pTargetLocationInMinutes)
{
	double lon_lat_dec, temp;

	temp = pTargetLocationInMinutes->minutes * 60.00 + pTargetLocationInMinutes->seconds;
	lon_lat_dec = pTargetLocationInMinutes->degrees + temp/3600.00;

	return lon_lat_dec;
}
//----------------------------------------------------------