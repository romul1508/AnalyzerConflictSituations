/*
 *
 *
 */
#ifndef __CAT_062_ASTERIX_INCLUDED__
#define __CAT_062_ASTERIX_INCLUDED__

#include <sys/types.h>
#include "common_lib_asterix.h"



#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief Способоность бортового ответчика к обмену данными
 */
enum Cat062CommCapability
{

	CC_NO_AVAILABLE             = 0,                   //!< - связь с бортом не доступна
	CC_COMM_A_B                 = 1,                   //!< - связь организована по А и В
	CC_COMM_A_B_UpELM           = 2,                   //!< - связь организована по А и В, данные передаются по ELM
	CC_COMM_A_B_UP_ELM_DOWN_ELM = 3,                   //!< - связь организована по А и В, данные передаются и принимаются по ELM
	CC_COMM_LEVEL_5             = 4                    //!< - установлен передатчик 5 уровня
};


/**
 *  @brief Статус полета по данным АЗН
 *         битовое поле
 */
enum Cat062FlightStatus
{

	FS_ON_GROUND = 1,                                  //!< - борт находится на земле
	FS_ALERT     = 2,                                  //!< - борт передает сигнал ТРЕВОГА
	FS_SPI       = 4                                   //!< - борт передает сигнал SPI
};

typedef enum AdsPointTypeMission
{

	APTM_UNKNOWN      = 0,                             //!< Тип задачи на точке - не известно
	APTM_FLY_BY       = 1,                             //!< Тип задачи на точке - полет к точке ( Fly by waypoint (LT)
	APTM_FLY_OVER     = 2,                             //!< Тип задачи на точке - пролет точки  ( Fly over waypoint (LT)
	APTM_HOLD_PATTERN = 3,                             //!< Тип задачи на точке - движение по маршруту ( Hold pattern (LT)
	APTM_HOLD_PROC    = 4,                             //!< Тип задачи на точке - процедура удержания ( Procedure hold (LT)
	APTM_TURN_PROC    = 5,                             //!< Тип задачи на точке - процедура разворота ( Procedure turn (LT)
	APTM_RF_LEG       = 6,                             //!< Тип задачи на точке - движение по огибающей( RF leg (LT)
	APTM_TOP_CLIMB    = 7,                             //!< Тип задачи на точке - высшая точка (горка) ( Top of climb (VT)
	APTM_TOP_DESCENT  = 8,                             //!< Тип задачи на точке - низшая точка (снижение)( Top of descent (VT)
	APTM_START_LVL    = 9,                             //!< Тип задачи на точке - начало перехода по высоте( Start of level (VT)
	APTM_ALT_CROSS    = 10,                            //!< Тип задачи на точке - пересечение высоты ( Cross-over altitude (VT)
	APTM_ALT_TRANS    = 11                             //!< Тип задачи на точке - высота перехода (Transition altitude (VT)

}AdsPointTypeMission;

/**
 *  @brief Тип источника данных о высоте полета
 */
typedef enum AltitudeSourceType
{
	AST_UNKNOWN       = 0,                             //!< Источник данных о высоте полета - не известен
	AST_GPS           = 1,                             //!< Источник данных о высоте полета - получена по информации ГНСС (GPS)
	AST_3D_RADAR      = 2,                             //!< Источник данных о высоте полета - 3 координатная РЛС
	AST_TRIANGULATION = 3,                             //!< Источник данных о высоте полета - система триангуляции
	AST_COVERAGE      = 4,                             //!< Источник данных о высоте полета - от единой информационной системы
	AST_TABLE         = 5,                             //!< Источник данных о высоте полета - по табличным данным
	AST_FICTION       = 6,                             //!< Источник данных о высоте полета - высота фиктивная
	AST_MLAT          = 7                              //!< Источник данных о высоте полета - по данным системы мультилатерации
}AltitudeSourceType;


/**
 * @brief Структура для передачи данных измерения от наземных
 *        радиолокационных источников (необработанные данные измерения)
 *        параметров воздушного объекта (ВО)
 */
typedef struct TMeasuredData
{
	/**
	 * @brief Идентификатор источника данных
	 */
	u_int16_t sensor_sac_sic;
	/**
	 * @brief Дистанция до ВО измеренная на источнике
	 */
	double distanse;
	/**
	 * @brief Азимут на ВО измеренный на источнике
	 */
	double azimuth;
	/**
	 * @brief Высота полета ВО измеренная на источнике
	 */
	double height;
	/**
	 * @brief Данные бортовой высоты полета ВО полученные на источнике
	 */
	BoardAltitudeCode mode_c;
	/**
	 * @brief Данные бортового номера  ВО полученные на источнике
	 */
	BoardNumCode mode3a;
	/**
	 * @brief Тип источника данных о ВО
	 *        в соотвествиии с MEASURED_DATA_SRC_TYPE_xxxxx
	 */
	u_int8_t src_type;
	/**
	 * @brief Признак эталонной (тестовой) трассы
	 *        0 - реальный ВО
	 *        1 - эталонный (тестовый ) ВО
	 */
	u_int8_t simulate;
	/**
	 * @brief Признак стационарного ответчика
	 *        0 - данные получены от подвижного ответчика
	 *        1 - данные от стационарного ответчика
	 */
	u_int8_t rab;
	/**
	 * @brief Признак тренажного ВО
	 *        0 - реальный ВО
	 *        1 - тренажный ВО
	 */
	u_int8_t test_target;
}TMeasuredData;


/**
 * @brief Структура точки маршрута по данным,
 *        полученным с борта ВО
 */
typedef struct TrajectoryIntentData
{
	/**
	 * @brief Номер точки маршрута
	 */
	u_int8_t rp_num;
	/**
	 * @brief Признак соответсвия точки утвержденному маршруту
	 *        0 - соответствует
	 *        1 - не соответствует
	 */
	u_int8_t rp_compliance;
	/**
	 * @brief Высота точки маршрута, в метрах
	 */
	int rp_altitude;
	/**
	 * @brief Широта точки маршрута, в радианах
	 */
	double rp_latitude;
	/**
	 * @brief Долгота точки маршрута, в радианах
	 */
	double rp_longitude;
	/**
	 * @brief Тип исполняемой задачи в точке маршрута
	 */
	u_int8_t rp_type;
	/**
	 * @brief Время достижения точки, в сек от полуночи
	 *        текущих суток UTC
	 */
	int rp_time;
	/**
	 * @brief Знак исполнения разворота в точке маршрута
	 *        0 - недопустимое значение
	 *        1 - поворот вправо
	 *        2 - поворот влево
	 *        3 - движение прямо
	 */
	u_int8_t rp_turn_sign;
	/**
	 * @brief Величина радиуса разворота, в метрах
	 */
	float rp_turn_radius;

}TrajectoryIntentData;

/**
 * @brief Структура данных для обмена
 *        данными полученными с борта ВО по АЗН-В
 */
typedef struct Cat062AircraftDerivedData
{
	/**
	 * @brief 24 битный идентификатор ИКАО
	 */
	u_int32_t target_address;
	/**
	 * @brief Символьный идентификатор (позывной)
	 */
	char target_identification[9];
	/**
	 * @brief Магнитный курс полета
	 */
	float magnetic_heading;
	/**
	 * @brief Индикатор представления приборной воздушной скорости полета
	 *        0 - скорость представлена в м/сек
	 *        1 - скорость представлена в единицах Маха
	 */
	u_int8_t mach_indicator;
	/**
	 * @brief Приборная воздушная скорость полета
	 */
	float indicated_airspeed;
	/**
	 * @brief Реальная воздушная скорость, м/сек
	 */
	float true_airspeed;
	/**
	 * @brief Выбираемая высота полета, м
	 */
	float selected_altitude;
	/**
	 * @brief Признак достоверности данных о выбираемой высоте полета
	 *        0 - не достоверные данные
	 *        1 - подтвержденные данные
	 */
	u_int8_t selected_altitude_valid;
	/**
	 * @brief Источник информации о для выбора высоты полета
	 */
	u_int8_t selected_altitude_src;
	/**
	 * @brief Финальное значение выбранной высоты полета
	 */
	float final_selected_altitude;
	/**
	 * @brief Признак изменения выбранной высоты полета
	 *        0 - изменения нет
	 *        1 - маневрирование по высоте
	 */
	u_int8_t fsa_manage_vertical_mode;
	/**
	 * @brief Признак удержания выбранной высоты полета
	 *        0 - удержания нет
	 *        1 - удержание по высоте
	 */
	u_int8_t fsa_altitude_hold;
	/**
	 * @brief Признак включения режима подход выбранной высоты полета
	 *        0 - нет режима подход
	 *        1 - есть режим подход
	 */
	u_int8_t fsa_approach_mode;
	/**
	 * @brief Признак наличия данных о траектории(точках маршрута) полета
	 *        0 - есть данные о точках маршрута
	 *        1 - нет данных о точках маршрута
	 */
	u_int8_t trajectory_available;
	/**
	 * @brief Признак достоверности данных о траектории(точках маршрута) полета
	 *        0 - данные достоверны
	 *        1 - данные не достоверны
	 */
	u_int8_t trajectory_valid;
	/**
	 * @brief Счетчик точек маршрута полета
	 */
	u_int8_t route_point_count;
	/**
	 * @brief Данные о точках маршрута
	 */
	TrajectoryIntentData* route_point;
	/**
	 * @brief Данные типе связи
	 */
	u_int8_t communication_capability;
	/**
	 * @brief Статус полета enum
	 */
	u_int8_t flight_status;
	/**
	 * @brief Признак специального обслуживания (литер)
	 *        0 - нет обслуживания
	 *        1 - есть спец. обслуживание
	 */
	u_int8_t specific_service;
	/**
	 * @brief Признак установленной точности определения высоты
	 *        0 - 100 футов (30,48 м)
	 *        1 - 25 футов (7,62 м)
	 */
	u_int8_t altitude_report_capability;
	/**
	 * @brief Признак возможности идентификации
	 *          0 - нет
	 *          1 - да
	 */
	u_int8_t aircraft_ident_capability;
	/**
	 * @brief Значение В1А
	 */
	u_int8_t bit_b1a;
	/**
	 * @brief Значение В1В
	 */
	u_int8_t bits_b1b;
	/**
	 * @brief Признак работоспособности системы ACAS
	 *          0 - нет
	 *          1 - да
	 */
	u_int8_t acas_operational;
	/**
	 * @brief Признак наличия комплексного навигационного обеспечения
	 *          0 - нет
	 *          1 - есть
	 */
	u_int8_t multiple_navigational;
	/**
	 * @brief Признак наличия дифференциальной коррекции
	 *          0 - нет
	 *          1 - есть
	 */
	u_int8_t differencial_correction;
	/**
	 * @brief Выставление в передатчике признака нахождения на земле
	 *        0 - не установлен
	 *        1 - установлен
	 */
	u_int8_t ground_bit;
	/**
	 * @brief Вид бедствия
	 */
	u_int8_t flight_status_emergency;
	/**
	 * @brief Сообщение Mode S
	 */
	u_int64_t acas_56_bit_msg;
	/**
	 * @brief Скорость изменения высоты, барометрическая, м/сек
	 */
	float vh_barometric;
	/**
	 * @brief Скорость изменения высоты, геометрическая, м/сек
	 */
	float vh_geometric;
	/**
	 * @brief Угол выполняемого поворота, радиан
	 */
	float roll_angle;
	/**
	 * @brief Признак порворота относительно курса полета
	 *        0 - не известно
	 *        1 - враво
	 *        2 - влево
	 *        3 - прямо
	 */
	u_int8_t turn_indicator;
	/**
	 * @brief Скорость поворота, град/сек
	 */
	float turn_rate;
	/**
	 * @brief Текущий курс полета, радиан
	 */
	float track_angle;
	/**
	 * @brief Скорость относительно земной поверхности, м/сек
	 */
	float ground_speed;
	/**
	 * @brief СКО измерения скоростиы, барометрическая, м/сек
	 */
	u_int8_t velocity_uncertainty;
	/**
	 * @brief Признак достоверности скорости ветра
	 *          0 - не достоверна
	 *          1 - достоверна
	 */
	u_int8_t wind_speed_valid;
	/**
	 * @brief Скорость ветра, м/сек
	 */
	float wind_speed;
	/**
	 * @brief Признак достоверности направления ветра
	 *          0 - не достоверен
	 *          1 - достоверен
	 */
	u_int8_t wind_direct_valid;
	/**
	 * @brief Направление ветра, радиан
	 */
	float wind_direct;
	/**
	 * @brief Признак достоверности температуры
	 *          0 - не достоверна
	 *          1 - достоверна
	 */
	u_int8_t temperature_valid;
	/**
	 * @brief Температура, град
	 */
	short temperature;
	/**
	 * @brief Признак достоверности турбулентности
	 *          0 - не достоверна
	 *          1 - достоверна
	 */
	u_int8_t turbulence_valid;
	/**
	 * @brief Турбулентность, в у.е. диапазон 0 ... 15
	 */
	u_int8_t turbulence;
	/**
	 * @brief Категория передатчика
	 */
	u_int8_t emitter_category;
	/**
	 * @brief Широта положения борта, в радианах
	 */
	double latitude_wgs84;
	/**
	 * @brief Долгота положения борта, в радианах
	 */
	double longitude_wgs84;
	/**
	 * @brief Геометрическая высота, в м
	 */
	float geometric_altitude;
	/**
	 * @brief Неопределенность измерения положения, в у.е. диапазон 0...15
	 */
	u_int8_t position_uncertainty;
	/**
	 * @brief Количество данных Mode S
	 */
	u_int8_t mode_s_data_count;
	/**
	 * @brief Данные Mode S
	 */
	u_int64_t* mode_s_msg;
	/**
	 * @brief Число Маха
	 */
	float mach_number;
	/**
	 * @brief Барометрическое давление в самолете, мБар
	 */
	float barometric_pressure_setting;

}Cat062AircraftDerivedData;


/**
 * @brief Определение структуры данных категории 062 Астерикс
 */
typedef struct TCat062 TCat062;

/**
 * @brief Выдача указателя на структуру категории 062 из
 *        обобщенной структуры Астерикс
 * @param cat - указатель на структуру Астерикс
 * @return указатель на структуру категории 062 Астерикс,
 *         при отказе значение NULL
 */
TCat062* get062category(TCatAsterix* cat);
/**
 * @brief Упаковка сообщения категории 062 (ed.1.14 July 2011) АСТЕРИКС
 * @param cat - указатель на структуру категории 062 Астерикс
 * @return - указатель на структуру с кодированным сообщением,
 *              при отказе значение NULL
 */
TCommonObmenData* pack062catRecord(TCat062* cat);

/**
 * @brief Распаковка записи категории 062(ed.1.14 July 2011) формата АСТЕРИКС
 * @param ptr - указатель на данные с кодограммой АСТЕРИКС
 * @param start_byte_pos - начальный
 * @return - указатель на структуру категории Астерикс
 */
TCatAsterix* decode062catRecord(TCommonObmenData* codogramm, u_int64_t* start_byte_pos);
/**
 * @brief Запись данных измерения параметров траектории трассы ВО в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param msd_data - указатель на структуру данных последнего измерения
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set062TrackMeasuredInformation(TCat062* cat, TMeasuredData* msd_data);
/**
 * @brief Запрос данных последнего измерения параметров трассы из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - указатель на структуру данных последнего измерения
 */
TMeasuredData* get062TrackMeasuredInformation(TCat062* cat);
/**
 * @brief Запрос данных о содержимом FSPEC записи
 * @param cat - указатель структуру категории 062
 * @return - указатель на массив с байтами FSPEC записи
 */
u_int8_t* get062recordFspec(TCat062* cat, size_t* size);
/**
 * @brief Запись данных измерения параметров траектории трассы ВО в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param add_data - указатель на структуру данных полученных с борта ВО по каналу АЗН
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set062TrackAircraftDerivedData(TCat062* cat, Cat062AircraftDerivedData* add_data);
/**
 * @brief Выборка "сырых" данных из записи категории 062 без распаковки
 * @param codogramm - указатель на контейнер с кодограммой
 * @param current_pos - начальное положение первого байта записи в теле кодограммы
 * @return - указатель на структуру контейнер Астерикс
 */
TCatAsterix* get062recordRawData(TCommonObmenData* codogramm, u_int64_t current_pos);
/**
 * @brief Выборка бортовых данных трассы ВО, полученных
 * @param cat_lib - указатель на структуру категории 062
 * @return - указатель на структуру данных полученных с борта ВО по каналу АЗН
 *           NULL - данные не получены
 */
Cat062AircraftDerivedData* get062TrackAircraftDerivedData(TCat062* cat);
/**
 * @brief Освобождение памяти от структуры с бортовыми данными трассы ВО, полученными
 *        по каналу АЗН
 * @param add - указатель на структуру с бортовыми данными трассы ВО
 * @return - 0 - успешное выполнение, -1 - отказ
 */
int free062TrackAircraftDerivedData(Cat062AircraftDerivedData* add);

#ifdef __cplusplus
}
#endif
#endif
