#ifndef ANALYZER_CONFLICT_SERVICE_H_
#define ANALYZER_CONFLICT_SERVICE_H_

// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
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


#include "asterix_cat_11.hpp"
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
//#include <commctrl.h>
#include "Common.h"
#include "Log.h"
#include "Config_app.h"
#include "Config_airports.h"
#include "Pulkovo.h"

#ifdef __linux__ 
	//linux code goes here
	
#elif _WIN32
	// windows code goes here
	//#include "extcode.h"
	//#pragma comment(lib, "comctl32.lib")
	// #pragma comment(lib, "ws2_32.lib")
#else
// #error "OS not supported!"
#endif


// # define MAX_CONF_TEXT	500
# define MAX_SIZE_MSG_ASTERIX_011    20000
#define MAX_ERR_MESSAGE   240    /* Буфер сообщения об ошибке */
#define BUFF_MESSAGE_INFO 512	 // 1492   //8000 /* Максимальная длина поступающих данных */
#define	MAX_NUMBER_ENTRIES_CSV	30000	// maximum number of entries in the CSV file
#define STR_STATUS_TARGET 80

const int MAX = 2000;

// const double MyLAT = 59.800095;
// const double MyLON = 30.264818;
// using namespace std;
// using namespace ern;

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

 // Структура данных для передачи статуса трассы
typedef struct 
{
	 // Признак сопровождения трассы по данным (MON)
	 //          1 - одного источника
	 //          0 - мультирадарное сопровождение
	 uint8_t monosensor;
	
	 // Признак получения сигнала SPI от аппаратуры (SPI)
	 //          1 - получен сигнал
	 //          0 - нет получения сигнала
	 uint8_t spi_sign;
	
	 // Признак предпочтения высоты (более достоверной) (MRH)
	 //          1 - геометрическая высота
	 //          0 - барометрическая высота (Mode C)
	 uint8_t most_reliable_height;
	
	 // Признак источника геометрической высоты (SRC)
	 //          в соотвествии с определением ALTITUDE_SRC_хххххх
	 uint8_t geometric_altitude_source;
	
	 // Признак подтверждения (обновления) данных (CNF)
	 //          0 - данные подтверждаюся
	 //          1 - данные предположительные (экстраполяция отметки)
	 uint8_t confirm;
	
	 // Признак эталонной (имитированной) трассы (SIM)
	 //          0 - действительная трасса
	 //          1 - имитированная трасса
	 uint8_t simulate;
	
	 // Признак последнего сообщения о трассе (TSE)
	 //          0 - значение по умолчанию, обновление информации
	 //          1 - последнее сообщение, сброс трассы с сопровождения
	 uint8_t last_message;
	
	 // Признак первого сообщения о трассе	(TSB)
	 // 0 - значение по умолчанию, обновление информации
	 // 1 - первое сообщение, новая трасса на сопровождении
	 uint8_t first_message;
	
	 // Признак подтверждения с плановой информацией (FPC)
	 // 0 - трасса вне плана, нет подтверждения
	 // 1 - трасса подтверждена плановой информацией
	 uint8_t flight_in_plan;
	
	 // Признак наличия противоречия между данными наблюдения и данными полученными по каналу АЗН (AFF)
	 // 0 - значение по умолчанию, нет противоречия
	 // 1 - есть расхождение данных
	 uint8_t ads_inconsist;
	
	 // Признак трансляции трассы от подчиненных источников
	 //           0 - значение по умолчанию, выдается собственная трасса
	 //           1 - ретрансляция трассы подчиненного источника
	 uint8_t slave_track;
	
	 // Признак приоритетного обслуживания трассы (литерный борт)
	 //           0 - трасса на приоритетном обслуживании
	 //           1 - обычное обслуживание, стандартный траффик
	 uint8_t background_serv;
	
	 // Признак отождествленной трассы (результат третичной обработки нескольких трасс от разных источников)(AMA)
	 //           0 - значение по умолчанию, трасса не отождествлена
	 //           1 - отождествленная трасса
	 uint8_t amalgamation;
	
	 // Признак результата распознавания в режиме Mode 4 (FRI/FOE)
	 uint8_t mode4_interrogate;
	
	 // Признак поступления бедствия от военного борта	(ME)
	 //          0 - значение по умолчанию, нет бедствия
	 //          1 - принят сигнал бедствия
	 uint8_t military_emrg;
	
	 // Признак идентификации принадлежности борта к военному ведомству (MI)
	 //          0 - значение по умолчанию, нет бедствия
	 //          1 - принят сигнал бедствия
	 uint8_t military_id;
	
	 
	 // Признак результата распознавания в режиме Mode 5
	 uint8_t mode5_interrogate;
	
	 // Признак устаревания времени последнего полученного обновления (CST)
	 //           трека больше порога времени работного(инерции) системы
	 //           0 - значение по умолчанию, трасса обновляется
	 //           1 - трасса не обновлялась, экстраполяция трассы
	 uint8_t coast_track;
	
	 // Признак устаревания данных трека, полученных по каналу ПРЛ (PSR)
	 //           больше порога времени работного(инерции) системы
	 //           0 - значение по умолчанию, трасса обновляется
	 //           1 - трасса не обновлялась, нет данных ПРЛ
	 uint8_t psr_coast;
	
	 // Признак устаревания данных трека, полученных по каналу ВРЛ (SSR)
	 //           больше порога времени работного(инерции) системы
	 //           0 - значение по умолчанию, трасса обновляется
	 //           1 - трасса не обновлялась, нет данных ВРЛ
	 uint8_t ssr_coast;
	
	 // Признак устаревания данных трека, полученных по каналу Mode S (MDS)
	 //           больше порога времени работного(инерции) системы
	 //           0 - значение по умолчанию, трасса обновляется
	 //           1 - трасса не обновлялась, нет данных Mode S
	 uint8_t mode_s_coast;
	
	 // Признак устаревания данных трека, полученных по каналу ADS-B	(ADS)	
	 //           больше порога времени работного(инерции) системы
	 //           0 - значение по умолчанию, трасса обновляется
	 //           1 - трасса не обновлялась, нет данных ADS-B
	 uint8_t ads_coast;
	
	 // Признак использования специального кода режима Mode A (номер борта)	(SUC)
	 //           0 - значение по умолчанию, специальный номер борта не используется
	 //           1 - используется специальный номер борта
	 uint8_t suc_set;
	
	 // Признак наличия конфликта номеров в режиме Mode A (номер борта)	(AAC)
	 //            0 - значение по умолчанию, нет конфликта
	 //            1 - есть трасса с аналогичным кодом номера борта
	 uint8_t aac_conflict;                             // assigned mode A Code conflict
	
	 // Признак статуса данных наземного наблюдения
	 uint8_t sds_status;
	
	// Признак типа бедствия
	//           в соотвествии с определением EMERGENCY_STATUS_хххххх
	uint8_t emergency_status;
	
	 // Признак выставления нахождения бортового передатчика АЗН (Mode S) на земле (GBS)
	 //           0 - значение по умолчанию, не установлено
	 //           1 - установлен бит "на земле"
	 uint8_t ground_bit_set;

	
	 // Признак маневра в горизонтальной плоскости (по Х, У)
	 //           0 - нет маневра
	 //           1 - есть маневр
	 uint8_t horizontal_manoeuvre;

	
	 // Признак пересчета наклонных координат положения цели
	 //        на горизонтальную плоскость (для Х, У)
	 //           0 - нет пересчета
	 //           1 - есть пересчет	 
	uint8_t slant_correction;

	 // Признак преобразования координат положения цели (для Х, У)
	 //        0 - данные результатов измерения
	 //        1 - сглаженные данные
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

 // Признак источника идентификатора цели (позывного)
//typedef enum target_identification_source
//{
//	TIS_FROM_TRANSPONDER                  = 0,         //!< - идентификатор или позывной получен от бортового передатчика
//	TIS_CALLSIGN_NOT_FROM_TRANSPONDER     = 1,         //!< - позывной получен не от бортового передатчика
//	TIS_REGISTRATION_NOT_FROM_TRANSPONDER = 2,         //!< - идентификатор получен не от бортового передатчика
//	TIS_UNDEFINED                         = 3,	
//}target_identification_source;

typedef struct 
{
	static const int				CALLSIGN_SIZE = 9;
	// target_identification_source	sti;
	int sti;
	char							callsign[CALLSIGN_SIZE];
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


//	 Определение структуры данных получаемых по каналу ModeS/Ads-B
//        касательно сопровождаемого объекта

typedef struct 
{

	// Значение 24-бит адреса ИКАО для самолетов (субполе №2)
	u_int32_t target_address;
	
	// Количество записей данных Mode S (поле REP в субполе №1) 
	u_int8_t mode_s_data_count;
	
	// Указатель на сообщения Mode S (для повторяющихся субполей №1)
	//        Каждое сообщение размером 64 бит, содержит:
	//        64 - 9 бит - 56 бит кодограммы Mode S	(MB Data)
	//        8 - 5 бит - данные BDS 1
	//         4 - 1 бит - данные BDS 2
	u_int64_t* mode_s_msg;

	//	  Тип воздушного судна - символы ASCII (субполе №8)
	char aircraft_type[5];

	// Категория передатчика (параметр ECAT субполя №9 )
	u_int8_t emitter_category;
	
	// Возможность коммуникации (параметр COM субполя №4)
	u_int8_t communication_capability;
	
    // Статус полета	(параметр STAS субполя №4)
	u_int8_t flight_status;
	
	// Признак спецобслуживания (приоритет обслуживания,	параметр SSC субполя №4)
	//          0 - обычный полет
	//          1 - приоритетное обслуживание
	u_int8_t specific_service;

	 // Признак установленной точности определения высоты (параметр ARC субполя №4)
	 //        0 - 100 футов (30,48 м)
	 //        1 - 25 футов (7,62 м)
	 u_int8_t altitude_report_capability;

	 // Признак возможности идентификации (параметр AIC субполя №4)
	 //          0 - нет
	 //          1 - да
	 u_int8_t aircraft_ident_capability;

	// Значение В1А (параметр В1А субполя №4)
	u_int8_t bit_b1a;

	// Значение В1В (параметр В1В субполя №4)
	u_int8_t bits_b1b;

	// Признак работоспособности системы ACAS (параметр AC субполя №4)
	//          0 - нет
	//          1 - да
	u_int8_t acas_operational;

	// Признак наличия комплексного навигационного обеспечения (параметр MN субполя №4)
	//          0 - нет
	//          1 - есть
	u_int8_t multiple_navigational;
	

	//  Признак наличия дифференциальной коррекции (параметр DC субполя №4)
	//          0 - нет
	//          1 - есть
	u_int8_t differencial_correction;
	
	// Признак наличия оборудования VDL	(параметр VDL субполя №11)
	//          0 - нет
	//          1 - есть
	u_int8_t vdl_available;

	
	 //  Признак наличия оборудования Mode S	(параметр MDS субполя №11)
	 //          0 - нет
	 //          1 - есть
	 u_int8_t mode_s_available;

	 // Признак наличия оборудования UAT
	 //         0 - нет
	 //         1 - есть
	 u_int8_t uat_available;

}TAdsBRelatedData;

typedef struct 
{
	uint8_t   type_save_data;					// тип данных для текущего сохранения

	uint8_t msg_type;								// элемент данных I011/000
	data_source_id dsi;								// элемент данных I011/010
	uint8_t service_id;								// элемент данных I011/015
	position_wgs84 pos_wgs84;				// элемент данных I011/041
	position_cartesian pos_cart;					// элемент данных I011/042
	uint16_t mode_3A;								// элемент данных I011/060
	double flight_level;								// элемент данных I011/090
	double geo_altitude;							// элемент данных I011/092
	baro_altitude baro_alt;						// элемент данных I011/093
	double time_of_track;							// элемент данных I011/140
	uint16_t track_number;						// элемент данных I011/161
	track_status tr_status;							// элемент данных I011/170
	track_velocity tr_velocity;					// элемент данных I011/202
	calc_acceleration acceleration;				// элемент данных I011/210
	double calc_vert_rate;							// элемент данных I011/215
	call_sign _call_sign;							// элемент данных I011/245
	target_size_orient target_size;				// элемент данных I011/270
	ages _ages;										// элемент данных I011/290
	int fleet;												// элемент данных I011/300
	prog_msg _prog_msg;						// элемент данных I011/310
	TAdsBRelatedData adsBRelatedData;		//	элемент данных I011/380
}TRecCategory_11;
//----------------------------------------------------------------------------------
typedef struct
{
	uint8_t str_status[STR_STATUS_TARGET];
	uint8_t target_koord[STR_STATUS_TARGET];
}TTargetEnvironment;
//----------------------------------------------------------------------------------
//typedef struct 
//{
//    double kta_X_lon;		// точка в системе SK-42 (долгота в градусах)
//	double kta_Y_lat;		// точка в системе SK-42 (широта в градусах)	
//} TSK42Point;
//------------------------------------------------------
//typedef struct 
//{
//    double X;		// точка в прямоугольной системе с нулем в центре элипсоида в метрах
//	double Y;		
//	double Z;
//} TXYZSystemPoint;
//------------------------------------------------------
//typedef struct 
//{
//    double kta_X, kta_Y;	//точка в системе КTА в метрах (долгота и широта переведенные в лок. систему координат)
//} TKtaPoint;
//------------------------------------------------------
//typedef struct 
//{
//    double wgs_X_lon, wgs_Y_lat;	//точка в геодезической системе WGS-84 (долгота и широта в градусах)
//} TWgsPoint;
//------------------------------------------------------
double strToDouble(std::string token);						// получает строку, меняет ',' на '.', переводит строку в double
bool StartWinsock();												// инициализирует модуль Winsock (для операционных систем семейства Windows)
// int server();																				// переводит Сервис в режим приема сообщений и выполнения расчетных задач

//////////тестовые функции/////////
void testLoadAppSetXML();										// проверяет загрузку настроечных данных 
void testLoadAirportsSetXML();								// для проверки правильности загруженных настроечных данных аэродромов
void testDetermCorrectnCalcCoordPolygonvertices(); // определение правильности расчета координат вершин полигона
void testCalcPrCoord();											// тестирование точного алгоритма перевода из системы WGS-84 в прямоугольную
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Класс	 Point (Точка на поверхности аэродрома)////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Point
{
private:
	TKtaPoint 	kta_point;								// координаты точки в локальной системе КTА
	// TXYZSystemPoint xyz_point;				// координаты в прямоугольной системе координат относительно центра элипсоида
	// TSK42Point 	sk_42_point;					// координаты точки в системе SK-42					
	TWgsPoint	wgs_84_point;						// координаты точки в системе WGS-84
	TP3_90_02Point pz_90_02_point;			// координаты точки в системе П3-90.02
	TUtmPoint utm_point;							// координаты точки в системе UTM
	std::string str_utm;									// форматированная строка с координатами UTM
	double height;										// высота от поверхности элипсоида (в системе WGS-84)	

public:
	Point(double lat, double lon, double height, double Z, int type_coord);	
	
	virtual ~Point() { }

	double getWgs84_Lon() const  { return wgs_84_point.wgs_X_lon - 0.001852; }									// возвращает долготу в системе Wgs84
	int getWgs84_Degrees_Lon() const  { return wgs_84_point.degrees_lon; }							// возвращает градусы долготы в системе Wgs84
	unsigned char getWgs84_Minutes_Lon() const  { return wgs_84_point.minutes_x_lon; }		// возвращает минуты долготы в системе Wgs84
	double getWgs84_Seconds_Lon() const  { return wgs_84_point.seconds_x_lon; }					// возвращает минуты долготы в системе Wgs84

	double getWgs84_Lat() const  { return wgs_84_point.wgs_Y_lat - 0.00006575; }										// возвращает широту в системе Wgs84
	int getWgs84_Degrees_Lat() const  { return wgs_84_point.degrees_lat; }								// возвращает градусы широты в системе Wgs84
	unsigned char getWgs84_Minutes_Lat() const  { return wgs_84_point.minutes_y_lat; }			// возвращает минуты широты в системе Wgs84
	double getWgs84_Seconds_Lat() const  { return wgs_84_point.seconds_y_lat; }					// возвращает минуты широты в системе Wgs84

	// double getSK42_Lon() const	 { return sk_42_point.sk42_X_lon; }							// возвращает долготу в системе CK-42
	// double getSK42_Lat() const	 { return sk_42_point.sk42_Y_lat; }								// возвращает широту в системе CK-42

	double getP3_90_02_Height() const  { return pz_90_02_point.height; }										// возвращает высоту точки в системе П3-90.02
	double getP3_90_02_Lon() const  { return pz_90_02_point.p3_90_02_X_lon; }							// возвращает долготу в системе П3-90.02
	int getP3_90_02_Degrees_Lon() const  { return pz_90_02_point.degrees_lon; }							// возвращает градусы долготы в системе П3-90.02
	unsigned char getP3_90_02_Minutes_Lon() const  { return pz_90_02_point.minutes_x_lon; }		// возвращает минуты долготы в системе П3-90.02
	double getP3_90_02_Seconds_Lon() const  { return pz_90_02_point.seconds_x_lon; }					// возвращает минуты долготы в системе П3-90.02

	double getP3_90_02_Lat() const  { return pz_90_02_point.p3_90_02_Y_lat; }								// возвращает широту в системе П3-90.02
	int getP3_90_02_Degrees_Lat() const  { return pz_90_02_point.degrees_lat; }								// возвращает градусы широты в системе П3-90.02
	unsigned char getP3_90_02_Minutes_Lat() const  { return pz_90_02_point.minutes_y_lat; }			// возвращает минуты широты в системе П3-90.02
	double getP3_90_02_Seconds_Lat() const  { return pz_90_02_point.seconds_y_lat; }					// возвращает минуты широты в системе П3-90.02

	double getHeight() const     { return height; }														// возвращает высоту от поверхности эллипсоида в системе WGS-84

	double getUTM_easting()const { return utm_point.easting; }									// возвращает восточное смещение в системе UTM
	double getUTM_northing()const { return utm_point.northing; }								// возвращает северное смещение в системе UTM
	std::string getUTM_string()const { return str_utm; }												// возвращает форматированную UTM строку
	
	double getCPA_x() const	{	return this->kta_point.kta_X; }
	double getCPA_y() const	{	return this->kta_point.kta_Y; }

	void setCPA_x( double kta_X) 	{	this->kta_point.kta_X = kta_X; }
	void setCPA_y( double kta_Y) 	{	this->kta_point.kta_Y = kta_Y; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Класс Line (Линия [между точками полигона]) ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Line
{
private:
	Point* ptrPoint1;									// указатель на точку 1
	Point* ptrPoint2;									// указатель на точку 2		
	//-------------------------------------------
	// Параметры уравнения прямой у = кх+m (в локальной системе )
	double k;  
	double m;
	//------------------------------------------ 
	void calcParamLine();

public:
	Line(Point* ptrPoint1, Point* ptrPoint2); 
	virtual ~Line() { }
	double getIntersectionCPA_Y(double CPA_X) 	{ return k * CPA_X + m; }		// возвращает значение функции у = кх+m при x == SK42_X   в формате КТА	
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Класс  Runway (ВПП [Взлетно-посадочная полоса]) ///////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Runway
{
private:
	int 	id_vpp;												// идентификационный номер ВПП					
	std::string		caption_vpp;							// наименование ВПП
	std::string		cat_vpp;								// категория ВПП
	double	angle_vpp;										// угол ВПП
	int lenght_vpp;											// длина ВПП в метрах
	int width_vpp;											// ширина ВПП в метрах
	bool isRunwayEmploymentStatus_;				// статус занятости ВПП (true - занята, false - свободна)
	
	Point* ptrCentralPoint;								// центральная точка ВПП
	Point* ptrNorthPoint;									// северная точка ВПП
	Point* ptrSouthPoint;									// южная точка ВПП
	Point* ptrWesternPoint;								// западная точка ВПП
	Point* ptrEasternPoint;								// восточная точка ВПП

	Line* ptrWN;												// линия проходящая через западную и северную точку
	Line* ptrNE;												// линия проходящая через северную и восточную точку
	Line* ptrES;												// линия проходящая через восточную и южную точку
	Line* ptrSW;												// линия проходящая через южную и западную точку

	bool runway_employment_status(Point* ptrTargetPoint);		// возвращает статус занятости ВПП для местоположения конкретной цели
	void to_recalculate_coords();												// пересчитывает координаты точек ptrNorthPoint, ptrSouthPoint, ptrWesternPoint, ptrEasternPoint

public:
	Runway(int 	id_vpp, double height_cta); 
	
	virtual ~Runway() { 
		delete ptrCentralPoint; delete ptrNorthPoint; delete ptrSouthPoint; delete ptrWesternPoint; delete ptrEasternPoint; 
		delete ptrWN; delete ptrNE; delete ptrES, delete ptrSW;
	}

	bool isRunwayEmploymentStatus(Point* ptrTargetPoint)			// возвращает статус занятости ВПП для местоположения конкретной цели
	{
		if(runway_employment_status(ptrTargetPoint))
			return true;
		else
			return false; 		
	}

	// устанавливает статус занятости ВПП
	void setRunwayEmploymentStatus(bool isRunwayEmploymentStatus)  { isRunwayEmploymentStatus_ = isRunwayEmploymentStatus; }
	bool getRunwayEmploymentStatus() const { return isRunwayEmploymentStatus_; }
	Point* getPtrNorthPoint() const	{ return ptrNorthPoint; }				// возвращает указатель на самую северную точку ВПП
	Point* getPtrSouthPoint() const	{ return ptrSouthPoint; }				// возвращает указатель на самую южную точку ВПП
	Point* getPtrWestPoint() const	{ return ptrWesternPoint; }				// возвращает указатель на самую западную точку ВПП
	Point* getPtrEastPoint() const	{ return ptrEasternPoint; }				// возвращает указатель на самую западную точку ВПП
	// void setNameInfoArraj(string name_file);		// устанавливает наименование информационного массива	
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Класс 	Airdrome (Аэродром) ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Airdrome
{
private:
	uint8_t	count_vpp;								// количество ВПП					
	std::string  name_airdrome;					// наименование аэродрома
	Point* ptrAirRefPoint;								// указатель на референсную точку аэродрома
	// unsigned int CalcHash(char *str);			// вычисляет хэш-код строки
	Runway* ptrRunway_1;							// впп 1
	Runway* ptrRunway_2;							// впп 2
public:
	Airdrome(); 
	
	// Airdrome(unsigned int num_info_arraj, string name_file) : id_info_arraj(num_info_arraj), name_info_arraj(name_file) { }
	// infoArjRecord(string name_file) : name_info_arraj(name_file) { id_info_arraj = CalcHash((char*)name_info_arraj.c_str()); }
	virtual ~Airdrome() { delete ptrRunway_1; delete ptrRunway_2; delete ptrAirRefPoint; }

	Runway* getPtrRunway(uint8_t	number_vpp) const		// возвращает указатель на ВПП
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
	Point* getPtrAirRefPoint() const { return ptrAirRefPoint; }    // возвращает указатель на контрольную точку аэродрома 
	void setIdInfoArraj(uint16_t num_info_arraj);   	// устанавливает идентификационный (порядковый) номер информационного массива
	std::string getNameAirdrome() const	{ return name_airdrome; }				// возвращает наименование аэродрома
	void setNameAirdrome(std::string name_airdrome)	{	this->name_airdrome = name_airdrome;	}		// устанавливает наименование аэродрома	
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////  класс RecCategory_11 - запись категории 11 Астерикс	/////////////////////////////
/////////////////  идентификатор(индекс) - наименование информационного массива  ////////////

class RecCategory_11
{
private:
	// unsigned int	id_info_arraj;					// идентификатор информационного массива (МЭО или МКЭО)
	// string  name_info_arraj;						// наименование информационного массива (МЭО или МКЭО, таким же образом будет названо и хранилище - бинарный файл)
	// unsigned int CalcHash(char *str);				// вычисляет хэш-код строки

public:
	uint8_t   type_save_data;					// тип данных для текущего сохранения

	uint8_t msg_type;								// элемент данных I011/000
	data_source_id dsi;								// элемент данных I011/010
	uint8_t service_id;								// элемент данных I011/015
	position_wgs84 pos_wgs84;				// элемент данных I011/041
	position_cartesian pos_cart;					// элемент данных I011/042
	uint16_t mode_3A;								// элемент данных I011/060
	double flight_level;								// элемент данных I011/090
	double geo_altitude;							// элемент данных I011/092
	baro_altitude baro_alt;						// элемент данных I011/093
	double time_of_track;							// элемент данных I011/140
	uint16_t track_number;						// элемент данных I011/161
	track_status tr_status;							// элемент данных I011/170
	track_velocity tr_velocity;					// элемент данных I011/202
	calc_acceleration acceleration;				// элемент данных I011/210
	double calc_vert_rate;							// элемент данных I011/215
	call_sign _call_sign;							// элемент данных I011/245
	target_size_orient target_size;				// элемент данных I011/270
	ages _ages;										// элемент данных I011/290
	int fleet;												// элемент данных I011/300
	prog_msg _prog_msg;						// элемент данных I011/310
	TAdsBRelatedData adsBRelatedData;		//	элемент данных I011/380

	RecCategory_11() { }
	// RecCategory_11(unsigned int num_info_arraj, string name_file) : id_info_arraj(num_info_arraj), name_info_arraj(name_file) { }
	// RecCategory_11(string name_file) : name_info_arraj(name_file) { id_info_arraj = CalcHash((char*)name_info_arraj.c_str()); }
	~RecCategory_11() 
	{ 		
			delete adsBRelatedData.mode_s_msg;
	}

	// может пригодится для выполнения соответствующих алгоритмов (при обработке списков)
	/*friend bool operator < (const RecCategory_11&, const RecCategory_11&);
	friend bool operator == (const RecCategory_11&, const RecCategory_11&);
	friend bool operator != (const RecCategory_11&, const RecCategory_11&);
	friend bool operator > (const RecCategory_11&, const RecCategory_11&);*/
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// класс Cat11Records - список записей категории 11 Астерикс ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Cat11Records
{
private:
	std::vector<RecCategory_11> vectCat11Records;
	// vector<RecCategory_11>::iterator iter;

public:
	void addCat11Record(RecCategory_11);					// добавление новой записи
	
	uint16_t getCountElemList() const							// возвращает количество элементов в списке
	{
		return vectCat11Records.size();
	}
	// list<RecCategory_11>::iterator begin_iter();			// устанавливает итератор в начало списка RecordCat11List
	// list<RecCategory_11>::iterator next_iter();			// перемещает итератор в прямом направлении на один индекс в списке
	void clearAll() { vectCat11Records.clear(); }				// очищает список хранящихся записей

	~Cat11Records() { vectCat11Records.clear(); }

	RecCategory_11 getCat11Record(uint16_t numCat11Rec);		// возвращает копию экземпляра класса, находящуюся под номером numInfoArrRec в векторе
};
//////////////////////////////////////////////////////////////////////
///////////////класс Track (трасса)//////////////////////////////
///////////////////////////////////////////////////////////////
class Track
{
private:
	uint16_t  track_number;									// номер трассы
	Cat11Records* ptrCat11Records;					   	// список записей по трассам
	bool isLocatedRunway1;									// true - находится на ВПП1, false - не находится на ВПП1
	bool isLocatedRunway2;									// true - находится на ВПП2, false - не находится на ВПП2
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
	~Track() { ptrCat11Records->clearAll();	delete ptrCat11Records; }		// деструктор, предварительно удаляет все записи из списка
	void addCat11Asterix(RecCategory_11);												// добавление записи в список (новой записи в конец списка!!!)

	void setTrackNumber(uint16_t  track_number) { this->track_number = track_number; }
	uint16_t getTrackNumber() const { return track_number; }						// вернуть номер трассы
	
	Cat11Records* getCat11Record() { return ptrCat11Records; }            		// возвращает указатель на список записей, связанный с трассой

	void setLocatedRunway1(bool isLocatedRunway1)	{ this->isLocatedRunway1 = isLocatedRunway1; }
	bool getLocatedRunway1() const  { return isLocatedRunway1; }

	void setLocatedRunway2(bool isLocatedRunway2)	{ this->isLocatedRunway2 = isLocatedRunway2; }
	bool getLocatedRunway2() const  { return isLocatedRunway2; }

	// Нужно для использования во множестве
	friend bool operator < (const Track&, const Track&);
	friend bool operator == (const Track&, const Track&);	
};
///////////////////////////////////////////////////////////////
//////////////////класс compareTrack///////////////////////////////////////////////
class compareTrack									//функциональный объект для сравнения трасс (по их номерам)														
{
public:
	bool operator() (Track*, Track*) const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////класс TrackList (список [множество] трасс)/////////////////////////////////////////////////
//////////////// обеспечивает скоростной доступ к хранящимся инф. массивам //////////////////////
class TrackList
{
private:
	// Установить указатели на трассы
	std::set<Track*, compareTrack> setPtrsTrack;
	std::set<Track*, compareTrack>::iterator iter;

public:
	~TrackList();																	// удаление всех трасс из списка (освобождение ресурсов)
		
	int addTrack(Track* track);												// добавление трассы в список (новой трассы в конец списка!!!)	
	int deleteTrack(uint16_t);												// удаление трассы из списка в соответствии с ее номером
	Track* getTrack(uint16_t);												// возвращает указатель на трассу по ее номеру в списке
	Track* getBegTrack();														// возвращает трассу на которую указывает начальный итератор
	Track* getNextTrack();													// возвращает следующую трассу согласно последовательности итераторов
	uint16_t getNumberTraces() const;									// возвращает общее количество трасс

	// int saveTracesFile(const char* name_file);						// записывает дaнные о трассах в бинарный файл
	bool isTrackWithNumber(uint16_t num_track);					// проверяет - существует ли трасса в наборе с указанным номером
	void checkActualizationTraces(double actual_time);			// проверяет актуализацию всех трасс, если трасса не обновлялась более 15 сек, то она подлежит удалению,
																						// где атрибут actual_time - время в секундах относительно начала суток
	bool isRunwayEmploymentStatus(uint8_t number_vpp);		//  проверяет состояние занятости ВПП путем обхода всех актуальных (действующих) трасс
	void clearAllTracks();														// очищает хранилище от трасс
};

// unsigned int CalcHash(char *str);

///////////////класс AsterixWorker  предназначен для работы c АСТЕРИКС 11-й категории//////////////////////
class AsterixWorker
{
private:
	int record_count;																					// счетчик записей в csv-файл [инкрементируется до тех пор пока их количество не превысит 30000 (MAX_NUMBER_ENTRIES_CSV), после этого обнуляется]
	int table_count;																						// счетчик создаваемых csv-файлов
	int recording_mode_to_csv;																		// режим расчетов и записи в csv с учетом заданного периода времени (записывать все записи [all], записывать записи с интервалом 100 мс [standard], 
																												// записывать записи с интервалом 500 мс[extended_period] и т.д)
	int size_buff_Asterix_MsgCat011_;															// размер поступившей АСТЕРИКС-дейтаграммы категории 011
	char buffAsterixMsgCat011[BUFF_MESSAGE_INFO];									//Буфер для поступающих дейтаграмм с информацией в формате АСТЕРИКС категории 011
	int count_msg_asterix_cat011;																	// количество поступивших трасс за односекундный преиод наблюдения для анализа конфликтных ситуаций
	// std::ofstream out_csv_file;
	std::string name_csv_file;																		// наименование текущего csv-файла, в котором идет работа
	// double the_last_message;																	// время поступления последнего сообщения в секундах
	double the_first_message_arrives;															// время начала наблюдения (время получения первой посылки [дайтаграммы]) после завершения последнего расчета или при первом периоде, 
																												// начало временного отсчета (нужно для определения старта рассчетной части)
	bool isBegObservPeriodSpecified;																// показывает задано ли начало временного отсчета от начала наблюдения (старта сервиса или после окончания последнего расчета),
																												// если false, не начало наблюдения еще не зафиксировано
	TrackList* ptrTrackList;         																	// список трасс 
	Airdrome* ptrAirdrome;																			// указатель на аэродром(ы)
		
	void init_sourse_data_cat011();																// выделяет необходимое количество памяти для АСТЕРИКС-данных категории 11
	void handleCat11(Cat11::Cat11* cat11);													// разбирает новую запись категории 11 Астерикс, 
																												// данные добавляются в соответствующие поля класса, а строка дозаписывается в csv-файл 
	void handleCat11(TRecCategory_11 *ptrCat11);
	// void parseAsterixMsgCat11();																// разбирает полученную строку АСТЕРИКС категории 011 [buffAsterixMsgInfo]
	bool isStartCalculation(RecCategory_11 &cat_11);										// складывает полученные элементы данных категории 11 в оперативное хранилище
	int StartCalculation() { isBegObservPeriodSpecified = false;	return 1; }			// здесь начинается рассчетная часть						
	void addStrToCSV(RecCategory_11 &cat_11);
public:
	AsterixWorker() {
		char name_file[20];
		isBegObservPeriodSpecified = false;
		// record_count = 1;
		record_count = ern::ConfiguratorApp::getInstance().getRecordCount();
		// table_count = 1;
		table_count = ern::ConfiguratorApp::getInstance().getTableCount();
		// recording_mode_to_csv = ALL_RECORDS;		// по умолчанию пишем все записи
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
			std::string csv_string = "msg_type;SAC;SIC;ServiceID;wgs84_lat;wgs84_lon;X;Y;Mode3A;flight_level;geo_altitude;QNH;baro_altitude;time_of_track;track_number;";
			csv_string += "MON;GBS;MRH;SRC;CNF;SIM;TSE;TSB;FRI/FOE;ME;MI;AMA;SPI;CST;FPC;AFF;PSR;SSR;MDS;ADS;SUC;AAC;VX;VY;AX;AY;RATE;STI;CALLSIGN;LENGTH;WIDTH;ORIENTATION;";
			csv_string += "PSR_Ages;SSR_Ages;MDA;MFL;MDS_Ages;ADS_Ages;ADB;MD1;MD2;LOP;TRK;MUL;VFI;TRB;MSG;REP;MB_DATA;ADDRESS;COM;STAS;SSC;ARC;AIC;B1A;B1B;AC;VVP1;VVP2\n";
			out_csv_file << csv_string;
			record_count++;
			ern::ConfiguratorApp::getInstance().setRecordCount(record_count);
		}

		ptrAirdrome = new Airdrome();
		ptrTrackList = new TrackList();
	}
	virtual ~AsterixWorker() { this->clearAllDataCat011(); delete ptrAirdrome; delete ptrTrackList; }
	void setAsterixMSG(const char* ptrBuffAsterixMsg, int size_buff_Asterix_Msg);								// обновляет массив buffAsterixMsgInfo новыми данными  
	void clearAllDataCat011();																										// очищает оперативное хранилище данных категории 011 для трасс

	void setRecordingMode(int recording_mode_to_csv) {this->recording_mode_to_csv = recording_mode_to_csv;}
	int getRecordingMode() const { return recording_mode_to_csv;}
	void parseAsterixMsgCat11();																									// разбирает полученную строку АСТЕРИКС категории 011 [buffAsterixMsgInfo]
	void parseAsterixMsgCat11(TRecCategory_11 *ptrCat11);
	int getRecordCount() const { return record_count; }
	int getTableCount() const { return this->table_count; }
	void drawTargetSituation();
};

int server(AsterixWorker* ptrAsterixWorker);
int testLocalMeh(AsterixWorker* ptrAsterixWorker);																		// позволяет тестировать механизм при работе с локальными данными														

#endif /* ANALYZER_CONFLICT_SERVICE_H_ */
