#ifndef	__CAT_ASTERIX_INCLUDED__
#define	__CAT_ASTERIX_INCLUDED__
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include "unistd.h"
#include <string.h>
#include <time.h>
#include <math.h>
// #include <sys/time.h>	// for Linux
#	include <time.h>			// for Windows

// #include "headers/common_lib_asterix.h"
#include "common_lib_asterix.h"


#ifndef DEG
    #define DEG 				57.29577951308232286
#endif
//!
#ifndef RAD
    #define RAD					0.01745329251994329509
#endif

#define FSPEC_ELEMENT_NAME_SIZE     50
// const double M_PI = 3.14;	// for Windows

/**
 * \def Общее количество доступных элементов для категории 062
 *      в соотвествии с UAP
 */
#define CAT062_INDEX_NUMBER            35

#define MAX_PLAN_TIMES              14
/**
 * \def Общее количество возможных повторов типовой информации в сообщении категории 062
 */
#define MAX_REPETITION_INDICATOR            255

#define CMR_ASTX_TIME		0.0078125
#define CMR_ASTX_H_100F		30.48
#define CMR_ASTX_H_10F		3.048
#define CMR_ASTX_H_25F		7.62
#define CMR_ASTX_H_6_25F	1.905

#define CMR_ASTX_VH			0.03175

//! 360/2^16
#define CMR_ASTX_AZ			0.005493164

//! 1852/256
#define CMR_ASTX_D			7.234375
#define CAT048_CMR_XY		14.46875

//! 1852m/2^14 - m/sec
#define CMR_ASTX_SPEED		0.113037109375
//! metr
#define CMR_ADS_TTR			18.52
//! 180/2^23
#define CMR_GEO_23			0.000021457672119140625
//! 180/2^25
#define CMR_GEO_25			0.00000536441802978515625
//! 180/2^31
#define CMR_GEO_31			0.00000008381903171539306640625
//!
#ifndef DEG
    #define DEG 				57.29577951308232286
#endif
//metr
#define CMR_UVD_HEIGTH      10
//!
#ifndef RAD
    #define RAD					0.01745329251994329509
#endif

//! m
#define CAT062_CMR_XY		0.5
//! m
#define CAT011_CMR_XY		1
//! m/sec
#define CMR_VXVY			0.25

//! sec
#define CMR_AGE_TIME		0.25

#ifndef ON
    #define ON		1
#endif

#ifndef OFF
    #define OFF		0
#endif

#define DEFAULT_CODE_SIZE   4096
#define TRACK_STATUS_EXTENT  5
#define DEFAULT_U_INT32_CODE    0xFFFFFFFF
#define DEFAULT_U_INT16_CODE    0xFFFF
#define DEFAULT_U_INT8_CODE     0xFF
#define DEFAULT_RADIAN_GEO_POS  M_PI
#define DEFAULT_RADIAN_AZIMUTH_POS  (2*M_PI)
//Лимит местоположения прямоугольных координат, в метрах
#define DEFAULT_CARTESIAN_POS   9000000
//Лимит скрости по оси координат, метр/секунда
#define DEFAULT_VELOCITY        9000.0
//Лимит ускорения по оси координат, метр/секунда^2
#define DEFAULT_ACCELERATION    100.0
//Лимит суточного времени в секундах
#define DEFAULT_TIME_OF_TRACK   86400.0
//Код номера борта по умолчанию
#define DEFAULT_RBS_UVD_NUM_CODE    0
//Лимит времени устаревания данных
#define DEFAULT_TIME_AGES       100.0
//Лимит высоты полета
#define DEFAULT_ALTITUDE        50000
//Идентификатор абонента по умолчанию
#define DEFAULT_SAC_SIC_CODE    0x5001

/**
 * @brief Размер профиля пользователя (UAP)
 *           для категорий АСТЕРИКС
 */
enum FspecSize
{
    FSPEC_SIZE_1_BYTE  = 1,
    FSPEC_SIZE_2_BYTE  = 2,
    FSPEC_SIZE_3_BYTE  = 3,
    FSPEC_SIZE_4_BYTE  = 4,
    FSPEC_SIZE_5_BYTE  = 5,
    FSPEC_SIZE_6_BYTE  = 6,
    FSPEC_SIZE_7_BYTE  = 7
};

/**
 * @brief Индексы полей элементов
 *        профиля пользователя (UAP)
 *        для категории 062
 */
enum FSPEC_062_CAT
{
    CAT062_EL010 = 1,
    CAT062_EL015 = 3,
    CAT062_EL070 = 4,
    CAT062_EL105 = 5,
    CAT062_EL100 = 6,
    CAT062_EL185 = 7,
    CAT062_EL210 = 8,
    CAT062_EL060 = 9,
    CAT062_EL245 = 10,
    CAT062_EL380 = 11,
    CAT062_EL040 = 12,
    CAT062_EL080 = 13,
    CAT062_EL290 = 14,
    CAT062_EL200 = 15,
    CAT062_EL295 = 16,
    CAT062_EL136 = 17,
    CAT062_EL130 = 18,
    CAT062_EL135 = 19,
    CAT062_EL220 = 20,
    CAT062_EL390 = 21,
    CAT062_EL270 = 22,
    CAT062_EL300 = 23,
    CAT062_EL110 = 24,
    CAT062_EL120 = 25,
    CAT062_EL510 = 26,
    CAT062_EL500 = 27,
    CAT062_EL340 = 28,
    CAT062_ELRS  = 34,
    CAT062_ELSP  = 35
};
typedef enum FSPEC_062_CAT Fspec062;

/**
 * @brief Индексы полей элементов
 *        профиля пользователя (UAP)
 *        для категории 011
 */
enum FSPEC_011_CAT
{
    CAT011_EL010 = 1,
    CAT011_EL000 = 2,
    CAT011_EL015 = 3,
    CAT011_EL140 = 4,
    CAT011_EL041 = 5,
    CAT011_EL042 = 6,
    CAT011_EL202 = 7,
    CAT011_EL210 = 8,
    CAT011_EL060 = 9,
    CAT011_EL245 = 10,
    CAT011_EL380 = 11,
    CAT011_EL161 = 12,
    CAT011_EL170 = 13,
    CAT011_EL290 = 14,
    CAT011_EL430 = 15,
    CAT011_EL090 = 16,
    CAT011_EL093 = 17,
    CAT011_EL092 = 18,
    CAT011_EL215 = 19,
    CAT011_EL270 = 20,
    CAT011_EL390 = 21,
    CAT011_EL300 = 22,
    CAT011_EL310 = 23,
    CAT011_EL500 = 24,
    CAT011_EL600 = 25,
    CAT011_EL605 = 26,
    CAT011_EL610 = 27,
    CAT011_ELSP  = 28,
    CAT011_ELRE  = 29
};
typedef enum FSPEC_011_CAT Fspec011;

/**
 * @brief Индексы полей элементов
 *        профиля пользователя (UAP)
 *        для категории 010
 */
enum FSPEC_010_CAT
{
    CAT010_EL010 = 1,
    CAT010_EL000 = 2,
    CAT010_EL020 = 3,
    CAT010_EL140 = 4,
    CAT010_EL041 = 5,
    CAT010_EL040 = 6,
    CAT010_EL042 = 7,
    CAT010_EL200 = 8,
    CAT010_EL202 = 9,
    CAT010_EL161 = 10,
    CAT010_EL170 = 11,
    CAT010_EL060 = 12,
    CAT010_EL220 = 13,
    CAT010_EL245 = 14,
    CAT010_EL250 = 15,
    CAT010_EL300 = 16,
    CAT010_EL090 = 17,
    CAT010_EL091 = 18,
    CAT010_EL270 = 19,
    CAT010_EL550 = 20,
    CAT010_EL310 = 21,
    CAT010_EL500 = 22,
    CAT010_EL280 = 23,
    CAT010_EL131 = 24,
    CAT010_EL210 = 25,
    CAT010_ELSP  = 27,
    CAT010_ELRE  = 28
};
typedef enum FSPEC_010_CAT Fspec010;

typedef struct ElementHeader
{
    u_int8_t fspec_index;
    u_int8_t code_size;
    u_int8_t *code;
    char name[FSPEC_ELEMENT_NAME_SIZE];
}ElementHeader;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - идентификатор источника данных
 *
*/
typedef struct DataSourceIdentifier
{
    ElementHeader head;
    u_int16_t sac_sic;
}DataSourceIdentifier;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - идентификатор сервиса
 *
*/
typedef struct ServiceIdentification
{
    ElementHeader head;
    u_int8_t sevice_id;
}ServiceIdentification;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - номер трека
 *
*/
typedef struct TrackNumber
{
    ElementHeader head;
    u_int16_t track_number;
}TrackNumber;


/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - номер борта Mode3/A
 *
*/
typedef struct TrackMode3Acode
{
    ElementHeader head;
    BoardNumCode  m3a;
}TrackMode3Acode;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - время обновления данных сообщения
 *
*/
typedef struct TimeOfTrackInformation
{
    ElementHeader head;
    double time_of_track;
}TimeOfTrackInformation;

/**
 * @brief Структура данных для категории Астерикс 062
 *        элемент данных 080 - статус трека
 *
*/
typedef struct Cat062TrackStatus
{
    ElementHeader head;
    TTrackStatus *track_status;
}Cat062TrackStatus;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - измеренное положение трека в полярных координатах
 *
*/
typedef struct PolarPosition
{
    double azimuth;
    double distance;
}PolarPosition;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - измеренное положение трека в полярных координатах
 *
*/
typedef struct MeasuredPositionInPolar
{
    ElementHeader head;
    PolarPosition polar;
}MeasuredPositionInPolar;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - расчитанное положение трека в прямоугольных координатах
 *
*/
typedef struct CalcPositionCartesian
{
    ElementHeader head;
    double x;
    double y;
}CalcPositionCartesian;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - расчитанное положение трека в WGS-84
 *
*/
typedef struct CalcPositionGeografic
{
    ElementHeader head;
    double latitude;
    double longitude;
}CalcPositionGeografic;

/**
 * @brief Структура данных для категории Астерикс 062
 *        элемент данных 110 - сообщение mode 5 и расширенное сообщение mode1
 *        (в основном для военных бортов)
 *
*/
typedef struct Mode5dataReportMode1extend
{
    ElementHeader head;
    u_int8_t m5_interrogate;
    u_int8_t m5_id_reply;
    u_int8_t m5_report;
    u_int8_t m5_mode1_present;
    u_int8_t m5_mode2_present;
    u_int8_t m5_mode3_present;
    u_int8_t m5_mode_c_present;
    u_int8_t m5_x_pulse_set;
    u_int16_t m5_pin_code;
    u_int8_t m5_national;
    u_int8_t m5_misson_code;
    double m5_latitude;
    double m5_longitude;
    float m5_gnss_altitude;
    u_int16_t m5_extend_mode1_code;
    float m5_valid_time_offset;
}Mode5dataReportMode1extend;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - номер борта mode 2
 *
*/
typedef struct TrackMode2code
{
    ElementHeader head;
    BoardNumCode  m2;
}TrackMode2code;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - расчитанная геометрическая высота
 *
*/
typedef struct CalcGeometricAltitude
{
    ElementHeader head;
    double calc_g_height;
}CalcGeometricAltitude;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - расчитанная барометрическая высота
 *
*/
typedef struct CalcBarometricAltitude
{
    ElementHeader head;
    u_int8_t  qnh_correction;
    double calc_b_height;
}CalcBarometricAltitude;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - измеренная барометрическая высота
 *                             получаемая по данным АЗН
 *
*/
typedef struct MeasuredFlightLevel
{
    ElementHeader head;
    double measured_fligth_level;
}MeasuredFlightLevel;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - расчитанная скорость и курс в полярных координатах
 *
*/
typedef struct CalcVelocityPolar
{
    ElementHeader head;
    double ground_speed;
    double track_angle;
}CalcVelocityPolar;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - расчитанная скорость в прямоугольных координатах
 *
*/
typedef struct CalcVelocityCartesian
{
    ElementHeader head;
    double vx;
    double vy;
}CalcVelocityCartesian;

/**
 * @brief Структура данных для категории Астерикс 062
 *        элемент данных 200 - расчитанный режим движения трека
 *
*/
typedef struct Cat062modeOfMovement
{
    ElementHeader head;
    u_int8_t transversal;
    u_int8_t long_acceleration;
    u_int8_t vertical_rate;
    u_int8_t altitude_discrepancy;
}Cat062modeOfMovement;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - расчитанное ускорение в прямоугольных координатах
 *
*/
typedef struct CalcAccelerationCartesian
{
    ElementHeader head;
    double ax;
    double ay;
}CalcAccelerationCartesian;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - расчитанная скорость снижения/подъема по высоте
 *
*/
typedef struct CalcRateClimbDescent
{
    ElementHeader head;
    double vh;
}CalcRateClimbDescent;


/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - 24 битный номер ИКАО
 *
*/
typedef struct TargetAddress
{
    ElementHeader head;
    u_int32_t target_address;
}TargetAddress;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - идентификатор цели
 *
*/
typedef struct TargetIdentification
{
    ElementHeader head;
    u_int8_t target_id_src;
    char target_id[10];
}TargetIdentification;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - параметры размера цели
 *
*/
typedef struct TargetSizeOrientation
{
    ElementHeader head;
    u_int8_t length;
    u_int8_t width;
    double orientation;
}TargetSizeOrientation;

/**
 * @brief Структура данных для категории Астерикс 062
 *        элемент данных 290 - системное время обновления данных трека
 *
*/
typedef struct Cat062systemTrackUpdateAges
{
    ElementHeader head;
    double trk_age;
    double psr_age;
    double ssr_age;
    double mds_age;
    double ads_age;
    double ads_es_age;
    double vdl_age;
    double uat_age;
    double loop_age;
    double mlt_age;
}Cat062systemTrackUpdateAges;

/**
 * @brief Структура данных для категории Астерикс 062
 *        элемент данных 295 - время устаревания данных трека
 *                       полученных по каналам ВРЛ и АЗН
 *
*/
typedef struct Cat062trackDataAges
{
    ElementHeader head;
    //1 octet
    double mfl_age;//Measured Flight Level
    double md1_age;//Mode 1
    double md2_age;//Mode 2
    double mda_age;//Mode 3/A
    double md4_age;//Mode 4
    double md5_age;//Mode 5
    double mhg_age;//Magnetic Heading
    //2 octet
    double ias_age;//Indicated Airspeed/Mach Nb
    double tas_age;//True Airspeed
    double sal_age;//Selected Altitude
    double fss_age;//Final State Selected Altitude
    double tid_age;//Trajectory Intent Data
    double com_age;//Communications / ACAS Capability and Flight Status
    double sab_age;//Status Reported by ADS-B
    //3 octet
    double acs_age;//ACAS Resolution Advisory Report
    double bvr_age;//Barometric Vertical Rate
    double gvr_age;//Geometric Vertical Rate
    double ran_age;//Roll Angle
    double tar_age;//Track Angle Rate
    double tan_age;//Track Angle
    double gsp_age;//Ground Speed
    //4 octet
    double vun_age;//Velocity Uncertainty
    double met_age;//Meteorological Data
    double emc_age;//Emitter Category
    double pos_age;//Position Data
    double gal_age;//Geometric Altitude Data
    double pun_age;//Position Uncertainty Data
    double mb_age; //Mode S MB Data
    //5 octet
    double iar_age;//Indicated Airspeed Data
    double mac_age;//Mach Number Data
    double bps_age;//Barometric Pressure Setting Data

}Cat062trackDataAges;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных - идентификатор транспортного средства
 *
*/
typedef struct VehicleFleetIdentification
{
    ElementHeader head;
    u_int8_t fleet_id;
}VehicleFleetIdentification;

/**
 * @brief Структура данных для категории Астерикс 062
 *        элемент данных 340 - данные последнего измерения
 *
*/
typedef struct Cat062measuredInformation
{
    ElementHeader head;
    void *measured_info;
}Cat062measuredInformation;

typedef struct TAdsRoutePoint
{
    u_int8_t point_num;
    u_int8_t compliance;
    int  point_altitude;
    double point_latitude;
    double point_longitude;
    u_int8_t point_type;
    int point_time;//time
    u_int8_t point_turn_sign;//1 - to right, 2 - to left
    float point_turn_radius;//turn raduis
}TAdsRoutePoint;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных 380 - данные полученные с борта по АЗН-В
 *
*/
typedef struct AircraftDerivedData
{
    ElementHeader head;

    u_int32_t target_address;
    char target_identification[9];
    float magnetic_heading;
    u_int8_t mach_indicator;
    float indicated_airspeed;
    float true_airspeed;
    float selected_altitude;
    u_int8_t selected_altitude_valid;
    u_int8_t selected_altitude_src;
    float final_selected_altitude;
    u_int8_t fsa_manage_vertical_mode;
    u_int8_t fsa_altitude_hold;
    u_int8_t fsa_approach_mode;
    u_int8_t trajectory_available;
    u_int8_t trajectory_valid;
    u_int8_t route_point_count;
    TAdsRoutePoint *route_point;
    u_int8_t communication_capability;
    u_int8_t flight_status;
    u_int8_t specific_service;
    u_int8_t altitude_report_capability;
    u_int8_t aircraft_ident_capability;
    u_int8_t bit_b1a;
    u_int8_t bits_b1b;
    u_int8_t acas_operational;
    u_int8_t multiple_navigational;
    u_int8_t differencial_correction;
    u_int8_t ground_bit;
    u_int8_t flight_status_emergency;
    u_int64_t acas_56_bit_msg;
    float vh_barometric;
    float vh_geometric;
    float roll_angle;
    u_int8_t turn_indicator;
    float turn_rate;
    float track_angle;
    float ground_speed;
    u_int8_t velocity_uncertainty;
    u_int8_t wind_speed_valid;
    float wind_speed;
    u_int8_t wind_direct_valid;
    float wind_direct;
    u_int8_t temperature_valid;
    short temperature;
    u_int8_t turbulence_valid;
    u_int8_t turbulence;
    u_int8_t emitter_category;
    double latitude_wgs84;
    double longitude_wgs84;
    float  geometric_altitude;
    u_int8_t position_uncertainty;
    u_int8_t mode_s_data_count;
    u_int64_t *mode_s_msg;
    float mach_number;
    float barometric_pressure_setting;
    char aircraft_type[5];
}AircraftDerivedData;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных 390 - данные плановой информации
 *
*/
typedef struct FlightPlanRelatedData
{
    ElementHeader head;
    TFlightPlanData fpdata;
}FlightPlanRelatedData;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных 500 - оценка точности данных
 *
*/
typedef struct EstimatedAccuracies
{
    ElementHeader head;
    TTrackAccuracy accuracy;
}EstimatedAccuracies;


/**
 * @brief Структура данных для номеров частных трасс
 *        для элемента данных 062/510 - составной номер трассы
 *        (строка таблицы соответствия)
*/
typedef struct TrackNumberChain
{
    u_int8_t system_unit_id;
    u_int16_t unit_track_number;
}TrackNumberChain;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных 062/510 - составной номер трассы
 *        (строка таблицы соответствия)
*/
typedef struct ComposedTrackNumber
{
    ElementHeader head;
    TrackNumberChain *num_chain;
}ComposedTrackNumber;

/**
 * @brief Структура данных для категории Астерикс 062
 *        элемент данных SP
 *
*/
typedef struct Cat062specialPuposeField
{
    ElementHeader head;
    TSpData *el_sp;
}Cat062specialPuposeField;


struct TCat062
{
    ElementHeader               item062_fspec;
    DataSourceIdentifier        item062_010;
    ServiceIdentification       item062_015;
    TrackNumber                 item062_040;
    TrackMode3Acode             item062_060;
    TimeOfTrackInformation      item062_070;
    Cat062TrackStatus           item062_080;
    CalcPositionCartesian       item062_100;
    CalcPositionGeografic       item062_105;
    Mode5dataReportMode1extend  item062_110;
    TrackMode2code              item062_120;
    CalcGeometricAltitude       item062_130;
    CalcBarometricAltitude      item062_135;
    MeasuredFlightLevel         item062_136;
    CalcVelocityCartesian       item062_185;
    Cat062modeOfMovement        item062_200;
    CalcAccelerationCartesian   item062_210;
    CalcRateClimbDescent        item062_220;
    TargetIdentification        item062_245;
    TargetSizeOrientation       item062_270;
    Cat062systemTrackUpdateAges item062_290;
    Cat062trackDataAges         item062_295;
    VehicleFleetIdentification  item062_300;
    Cat062measuredInformation   *item062_340;
    AircraftDerivedData         *item062_380;
    FlightPlanRelatedData       *item062_390;
    EstimatedAccuracies         item062_500;
    ComposedTrackNumber         item062_510;
    Cat062specialPuposeField    *item062_sp;
    struct TCat062 *next;
};

/**
 * @brief Структура данных для категории Астерикс 011
 *        элемент данных 290 - системное время обновления данных трека
 *
*/
typedef struct Cat011systemTrackUpdateAges
{
    ElementHeader head;
    double psr_age;
    double ssr_age;
    double mda_age;//Mode 3/A
    double mfl_age;//Measured Flight Level
    double mds_age;
    double ads_age;
    double ads_es_age;
    double md1_age;//Mode 1
    double md2_age;//Mode 2
    double loop_age;
    double trk_age;//
    double mlt_age;
}Cat011systemTrackUpdateAges;

/**
 * @brief Структура данных для категории Астерикс 011
 *        элемент данных 170 - статус трека
 *
*/
typedef struct Cat011trackStatus
{
    ElementHeader head;
    TTrackStatus  track_status;
}Cat011trackStatus;

/**
 * @brief Структура данных для категории Астерикс 011
 *        элемент данных 310 - действия по назначенной задаче
 *
*/
typedef struct Cat011preProgrammedMessage
{
    ElementHeader head;
    u_int8_t  in_trouble;
    u_int8_t  work_programm_code;
}Cat011preProgrammedMessage;

/**
 * @brief Структура данных для категории Астерикс
 *        элемент данных 000 - тип сообщения
 *
*/
typedef struct MessageTypeElem000
{
    ElementHeader head;
    u_int8_t  message_type;
}MessageTypeElem000;

/**
 * @brief Определение структуры данных эд 380 получаемых по каналу ModeS/Ads-B
 *        касательно сопровождаемого объекта
 */
typedef struct Cat011ed380AdsBRelatedData{

    ElementHeader head;

    u_int32_t target_address;
    u_int8_t mode_s_data_count;
    u_int64_t *mode_s_msg;
    char aircraft_type[5];
    u_int8_t emitter_category;
    u_int8_t communication_capability;
    u_int8_t flight_status;
    u_int8_t specific_service;
    u_int8_t altitude_report_capability;
    u_int8_t aircraft_ident_capability;
    u_int8_t bit_b1a;
    u_int8_t bits_b1b;
    u_int8_t acas_operational;
    u_int8_t multiple_navigational;
    u_int8_t differencial_correction;
    u_int8_t vdl_available;
    u_int8_t mode_s_available;
    u_int8_t uat_available;

}Cat011ed380AdsBRelatedData;


/**
 * @brief Структура данных для категории Астерикс 011
 *        элемент данных 430 - фаза полета
 *
*/
typedef struct Cat011phaseOfFligth
{
    ElementHeader head;
    u_int8_t  phase_fligth;
}Cat011phaseOfFligth;

/**
 * @brief Структура данных для категории Астерикс 011
 *        элемент данных 600 - сообщения о критических ситуациях
*/
typedef struct Cat011alertMessages
{
    ElementHeader head;
    u_int8_t  alert_acknowledged;
    u_int8_t  alert_level;
    u_int8_t  alert_type;
    u_int8_t  alert_number;
}Cat011alertMessages;

/**
 * @brief Структура данных для категории Астерикс 011
 *        элемент данных 605 - номера трасс находящихся в конфликтной ситуации
*/
typedef struct Cat011trackInAlert
{
    ElementHeader head;
    u_int8_t  all_track_in_alert;
    u_int16_t *fusion_track_num;
}Cat011trackInAlert;

/**
 * @brief Структура данных для категории Астерикс 011
 *        элемент данных 610 - статус полос удержания
*/
typedef struct Cat011holdbarStatus
{
    ElementHeader head;
    u_int8_t  all_holdbars;
    u_int16_t *holdbars;
}Cat011holdbarStatus;

/**
 * @brief Структура данных для категории Астерикс 011
 *        элемент данных SP
 *
*/
typedef struct Cat011specialPuposeField
{
    ElementHeader head;
    TSpData *el_sp;
}Cat011specialPuposeField;

/**
 * @brief Структура данных для категории Астерикс 011
 *        элемент данных RE - резервное поле расширения
 *
*/
typedef struct Cat011reservedExpansionField
{
    ElementHeader head;
    void *el_re;
}Cat011reservedExpansionField;

struct TCat011
{
    ElementHeader               item011_fspec;
    MessageTypeElem000          item011_000;
    DataSourceIdentifier        item011_010;
    ServiceIdentification       item011_015;
    CalcPositionGeografic       item011_041;
    CalcPositionCartesian       item011_042;
    TrackMode3Acode             item011_060;
    MeasuredFlightLevel         item011_090;
    CalcGeometricAltitude       item011_092;
    CalcBarometricAltitude      item011_093;
    TimeOfTrackInformation      item011_140;
    TrackNumber                 item011_161;
    Cat011trackStatus           item011_170;
    CalcVelocityCartesian       item011_202;
    CalcAccelerationCartesian   item011_210;
    CalcRateClimbDescent        item011_215;
    TargetIdentification        item011_245;
    TargetSizeOrientation       item011_270;
    Cat011systemTrackUpdateAges item011_290;
    VehicleFleetIdentification  item011_300;
    Cat011preProgrammedMessage  item011_310;
    Cat011ed380AdsBRelatedData  *item011_380;
    FlightPlanRelatedData       *item011_390;
    Cat011phaseOfFligth         item011_430;
    EstimatedAccuracies         item011_500;
    Cat011alertMessages         item011_600;
    Cat011trackInAlert          item011_605;
    Cat011holdbarStatus         item011_610;
    Cat011specialPuposeField    *item011_sp;
    Cat011reservedExpansionField *item011_re;
    struct TCat011 *next;
};

/**
 * @brief Структура данных для категории Астерикс 010
 *        элемент данных 020 - описание траектории
 *
*/
typedef struct Cat010targetDescriptor
{
    ElementHeader head;
    void  *track_descriptor;
}Cat010targetDescriptor;

/**
 * @brief Структура данных для категории Астерикс 010
 *        элемент данных 090 - высота полученная с борта ВО
 *        (Mode C, Mode S)
 *
*/
typedef struct Cat010flightLevelInBinary
{
    ElementHeader head;
    BoardAltitudeCode  fl_binary;
}Cat010flightLevelInBinary;

/**
 * @brief Структура данных для категории Астерикс 010
 *        элемент данных 170 - статус трека
 *
*/
typedef struct Cat010trackStatus
{
    ElementHeader head;
    void  *track_status;
}Cat010trackStatus;

/**
 * @brief Структура данных для категории Астерикс 010
 *        элемент данных 250 - данные Mode S
 *
*/
typedef struct Cat010modeSdata
{
    ElementHeader head;
    u_int8_t mode_s_data_count;
    u_int64_t *mode_s_msg;
}Cat010modeSdata;

/**
 * @brief Структура данных для категории Астерикс 010
 *        элемент данных 550 - состояние системы
 *
*/
typedef struct Cat010systemStatus
{
    ElementHeader head;
    void *system_status;

}Cat010systemStatus;

/**
 * @brief Структура данных для категории Астерикс 010
 *        элемент данных 280 - данные о положении эха относительно
 *        полученной координатной точки
 *
*/
typedef struct Cat010pResense
{
    ElementHeader head;
    u_int8_t presense_count;
    PolarPosition *presense;
}Cat010pResense;

/**
 * @brief Структура данных для категории Астерикс 010
 *        элемент данных 131 - данные об амплитуде первичной отметки
*/
typedef struct Cat010amplitudePrimaryPlot
{
    ElementHeader head;
    u_int8_t amplitude;
}Cat010amplitudePrimaryPlot;

/**
 * @brief Структура данных для категории Астерикс 010
 *        элемент данных SP
 *
*/
typedef struct Cat010specialPuposeField
{
    ElementHeader head;
    TSpData *el_sp;
}Cat010specialPuposeField;

/**
 * @brief Структура данных для категории Астерикс 010
 *        элемент данных RE - резервное поле расширения
 *
*/
typedef struct Cat010reservedExpansionField
{
    ElementHeader head;
    void *el_re;
}Cat010reservedExpansionField;

struct TCat010
{
    ElementHeader               item010_fspec;
    MessageTypeElem000          item010_000;
    DataSourceIdentifier        item010_010;
    Cat010targetDescriptor      item010_020;
    TimeOfTrackInformation      item010_140;
    CalcPositionGeografic       item010_041;
    MeasuredPositionInPolar     item010_040;
    CalcPositionCartesian       item010_042;
    CalcVelocityPolar           item010_200;
    CalcVelocityCartesian       item010_202;
    TrackNumber                 item010_161;
    Cat010trackStatus           item010_170;
    TrackMode3Acode             item010_060;
    TargetAddress               item010_220;
    TargetIdentification        item010_245;
    Cat010modeSdata             item010_250;
    VehicleFleetIdentification  item010_300;
    Cat010flightLevelInBinary   item010_090;
    CalcGeometricAltitude       item010_091;
    TargetSizeOrientation       item010_270;
    Cat010systemStatus          item010_550;
    Cat011preProgrammedMessage  item010_310;
    EstimatedAccuracies         item010_500;
    Cat010pResense              item010_280;
    Cat010amplitudePrimaryPlot  item010_131;
    CalcAccelerationCartesian   item010_210;
    Cat010specialPuposeField    *item010_sp;
    Cat010reservedExpansionField *item010_re;
    struct TCat010 *next;
};


/**
 * @brief Структура данных для категорий Астерикс
 *
*/
struct TCatAsterix
{
    int cat_num;
    u_int64_t index_number;
    u_int32_t records_count;
    u_int8_t  create_check_sum;
    u_int8_t  create_byte_staff;
    u_int8_t  create_flag_shell;
    u_int8_t  control_flag;
    union
    {
        struct TCat062 c062;
        struct TCat011 c011;
        struct TCat010 c010;
    }category;
};
/**
 * @brief Запись данных в заголовок элемента данных категории Астерикс
 * @param elhead - указатель структуру заголовка
 * @param index  - номер элемента в профиле пользователя
 * @param code_size - количество данных кодирующих элемент
 * @param code - указатель на кодированные данные
 * @param name - строчное название элемента данных
 * @return - результат записи
 *           -1 - отказ исполнения
 *            0 - установлен номер элемента
 *            1 - установлен номер, название элемента
 *            2 - установлен номер, кодированные данные
 *            3 - установлен номер, название, кодированные данные
 */
int setElementHeader( ElementHeader *elhead, u_int8_t index, u_int8_t code_size, u_int8_t *code, char *name);
/**
 * @brief Распаковка плановой информации в категорий категории 062, 011
 * @param code - указатель на массив кодограммы
 * @param cnt - номер начального байта плановой информации
 * @return - указатель на распакованную структуру элемента 390
 */
FlightPlanRelatedData *decodeElem390PlanData( unsigned char *code, u_int64_t *cnt, u_int64_t size_codogramm );
/**
 * @brief Выборка размера профиля пользователя сообщения категории АСТЕРИКС
 * @param codd - указатель на кодограмму формата АСТЕРИКС
 * @param cnt - начальная позиция первого байта профиля пользователя
 * @param size_codogramm - размер пакета данных переданных для анализа
 * @return - количество байт профиля пользователя
 */
int getCategoryFspecSize(unsigned char *codd, u_int64_t cnt , u_int64_t size_codogramm);
/**
 * @brief Упаковка плановой информации для сообщения категории Астерикс
 * @param plan - указатель на структуру с плановой информацией
 * @return - указатель на стуктуру с кодированными данными
 */
TCommonObmenData *packElem390PlanData( TFlightPlanData *plan, u_int32_t available_mask );
/**
 * @brief Кодирование текущего времени суток (UTC) для кодограмм Астерикс
 * @return - кодированное время
 */
int makeTimeForAsterixCodo( void );
/**
 * @brief Перевод из алфавита ASCII в алфавит IA5
 * @param dest - указатель на массив укладки результата преобразования
 * @param ist - указатель на массив с исходными данными
 * @param cnt - номер начального байта исходной записи
 * @return - 0 - успешное выполнение, -1 отказ выполнения операции
 */
int convertAsciiToIA5code(unsigned char *dest, unsigned char *ist, u_int64_t *cnt);
/**
 * @brief Перевод из алфавита IA5 в алфавит ASCII
 * @param dest - указатель на массив укладки результата преобразования
 * @param ist - указатель на массив с исходными данными
 * @param cnt - номер начального байта исходной записи
 * @return - 0 - успешное выполнение, -1 отказ выполнения операции
 */
int convertIA5codeToAscii(unsigned char *dest, unsigned char *ist, u_int64_t *cnt);
/**
 * @brief Кодирование длины, ширины и положения объекта
 * @param lenght - длина объекта
 * @param width  - ширина объекта
 * @param orientation - положение носовой части относительно направления на север
 * @return -
 */
TReturnOneInt packTargetSizeAndOrientation(u_int8_t length, u_int8_t width, double orientation);

/**
 * @brief Декодирование данных о запасе топлива на борту ВО
 * @param code - кодированное значение запаса топлива
 * @return - запас топлива на борту в %
 */
u_int8_t decodeUvdFuel( u_int8_t code );

/**
 * @brief Кодирование данных о запасе топлива на борту ВО
 * @param fuel - данные о запасе топлива на борту ВО в %
 * @return - кодированное значение запаса топлива
 */
u_int8_t packUvdFuel( u_int8_t fuel );
/**
 * @brief Проверка переполнения
 * @param current_pos - текущее положение отсчета данных
 * @param kd_size     - размер данных в байтах
 * @param ed_size     - размер элемента данных в байтах
 * @return 0 - нет переполнения, -1 - выход за предел с данными
 */
int checkOverloadFunc(  u_int64_t current_pos, u_int16_t ed_size, u_int64_t kd_size );
/**
 * @brief Определение данных относящихся к элементу данных 390
 * @param code - указатель массив кодограммы
 * @param cnt - позиция начала данных эд 390
 * @param code_size - общий размер кодограммы
 * @return - 0 - успешное выполнение, -1 - отказ выполнения
 */
int view390PlanData( unsigned char *code, u_int64_t *cnt, u_int64_t code_size );
/**
 * @brief Кодирование  времени устаревания параметра трассы
 * @param age_time - параметр времени в секундах (диапазон от 0.25 до 63.75 сек)
 * @return - кодированое значение времени диапазоне от 0 - 255
 */
u_int8_t packAgeTime( double age_time);
/**
 * @brief Укладка байтов профиля категории в массив с кодированными данными
 * @param dest - указатель на массив данных
 * @param src - указатель на массив с данными профиля пользователя
 * @param cat_max_fspec_size - максимальный размер профиля пользователя категории
 * @return - количество уложенных байт профиля пользователя
 */
u_int8_t packFspecToCodogramm( void *dest, void *src, u_int8_t cat_max_fspec_size);

double decodeAgeTime( u_int16_t code_time);


#endif
