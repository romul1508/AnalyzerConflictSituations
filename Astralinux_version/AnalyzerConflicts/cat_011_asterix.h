/*
 *
 *
 */
#ifndef __CAT_011_ASTERIX_INCLUDED__
#define __CAT_011_ASTERIX_INCLUDED__

#include <sys/types.h>
#include "common_lib_asterix.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief Тип сообщения для категории 011
 */
enum Cat11MessageType
{

	MT_UNKNOWN                       = 0,              //!< - не определен
	MT_TRACK_AND_FLIGHT_PLAN         = 1,              //!< - трасса (трасса + план)
	MT_MANUAL_ATTACH_FLIGHT_PLAN     = 2,              //!< - ручная прязка плановых данных к трассе
	MT_MANUAL_DETACHMENT_FLIGHT_PLAN = 3,              //!< - ручное удаление плановых данных из трассы
	MT_INSERT_FLIGHT_PLAN            = 4,              //!< - вставить плановые данные для трассы
	MT_SUPPRESSION_FLIGHT_PLAN       = 5,              //!< - задержка приостановка плановых данных
	MT_MODIFICATION_FLIGHT_PLAN      = 6,              //!< - изменить плановые данные
	MT_HOLDBAR_STATUS                = 7               //!< - статус стоп-линий
};


/**
 *  @brief Направление поворота по данным АЗН
 */
enum Cat11TurnDirectionByAds
{

	TDBA_STRAIGHT = 0,                                 //!< - прямо
	TDBA_RIGHT    = 1,                                 //!< - поворот на право
	TDBA_LEFT     = 2                                  //!< - поворот на лево
};

/**
 *  @brief Статус полета по данным АЗН
 */
enum Cat11FlightStatusByAds
{

	FSBA_ON_GROUND = 1,                                //!< - борт находится на земле
	FSBA_ALERT     = 2,                                //!< - борт передает сигнал ТРЕВОГА
	FSBA_SPI       = 4                                 //!< - борт передает сигнал SPI
};

/**
 *  @brief Состояние системы ACAS
 */
enum Cat11StatusACAS
{

	ACAS_NOT_ACTIVE = 1,
	ACAS_ACTIVE     = 2
};

/**
 *  @brief Статус использования мультинавигации (АЗН)
 */
enum Cat11MultiNavAidStatus
{

	MNA_NOT_PREZENT = 1,
	MNA_PREZENT     = 2
};

/**
 *  @brief Тип источника данных о высоте полета
 */
enum Cat11AltitudeSource
{

	AS_UNKNOWN  = 0,                                   //!< - не известен
	AS_GPS      = 1,                                   //!< - получена по информации ГНСС (GPS)
	AS_3D_RADAR = 2,                                   //!< - 3 координатная РЛС
	AS_TRIANG   = 3,                                   //!< - система триангуляции
	AS_COVERAGE = 4,                                   //!< - от единой информационной системы
	AS_TABLE    = 5,                                   //!< - по табличным данным
	AS_FICTION  = 6,                                   //!< - высота фиктивная
	AS_MLAT     = 7                                    //!< - по данным системы мультилатерации
};

/**
 *  @brief Признак выполнения этапа полета
 */
typedef enum Cat11PhaseOfFlight
{

	POF_UNKNOWN               = 0,                     //!< - не известно
	POF_ON_STAND              = 1,                     //!< - на стоянке
	POF_TAXIING_FOR_DEPARTURE = 2,                     //!< - руление на вылет
	POF_TAXIING_FOR_ARRIVAL   = 3,                     //!< - руление после посадки
	POF_RUNWAY_FOR_DEPARTURE  = 4,                     //!< - на ВПП вылет
	POF_RUNWAY_FOR_ARRIVAL    = 5,                     //!< - на ВПП посадка
	POF_HOLD_FOR_DEPARTURE    = 6,                     //!< - ожидание вылета
	POF_HOLD_FOR_ARRIVAL      = 7,                     //!< - ожидание прибытия
	POF_PUSH_BACK             = 8,                     //!< - задержка
	POF_ON_FINALS             = 9                      //!< - завершение полета
}Cat11PhaseOfFlight;

/**
 *  @brief Значение уровня тревоги для сообщений тревоги
 */
typedef enum Cat11AlertLevel
{

	AL_ZERO         = 0,                               //!< - нет тревоги, тревога снята
	AL_PREALARM     = 1,                               //!< - предпосылка к аварии
	AL_SEVERE_ALERT = 2                                //!< - аварийная ситуация

}Cat11AlertLevel;

/**
 * @brief Определение структуры тревоги по наземной ситуации категории 011 Астерикс
 */
typedef struct TCat011AlertMessage
{
	/**
	 * @brief Признак оповешения персонала о тревоге
	 *         0 - персонал не оповещен
	 *         1 - персонал оповещен
	 */
	u_int8_t alert_acknowledged;
	/**
	 * @brief Признак уровня тревоги - ALERT_LEVEL_ххххх
	 */
	u_int8_t alert_level;
	/**
	 * @brief Тип тревоги - устанавливается по перечню абонента
	 */
	u_int8_t alert_type;
	/**
	 * @brief Номер тревоги - устанавливается по перечню абонента
	 */
	u_int8_t alert_number;

}TCat011AlertMessage;

/**
 * @brief Определение структуры данных получаемых по каналу ModeS/Ads-B
 *        касательно сопровождаемого объекта
 */
typedef struct TCat011AdsBRelatedData
{

	/**
	 * @brief Значение 24-бит адреса ИКАО
	 */
	u_int32_t target_address;
	/**
	 * @brief Количество записей данных Mode S
	 */
	u_int8_t mode_s_data_count;
	/**
	 * @brief Указатель на сообщения Mode S
	 *        Каждое сообщение размером 64 бит, содержит:
	 *        64 - 9 бит - 56 бит кодограммы Mode S
	 *        8 - 5 бит - данные BDS 1
	 *        4 - 1 бит - данные BDS 2
	 */
	u_int64_t* mode_s_msg;
	/**
	 * @brief Тип воздушного судна - символы ASCII
	 */
	char aircraft_type[5];
	/**
	 * @brief Категория передатчика
	 */
	u_int8_t emitter_category;
	/**
	 * @brief Возможность коммуникации
	 */
	u_int8_t communication_capability;
	/**
	 * @brief Статус полета
	 */
	u_int8_t flight_status;
	/**
	 * @brief Признак спецобслуживания (приоритет обслуживания)
	 *          0 - обычный полет
	 *          1 - приоритетное обслуживание
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
	 * @brief Признак наличия оборудования VDL
	 *          0 - нет
	 *          1 - есть
	 */
	u_int8_t vdl_available;
	/**
	 * @brief Признак наличия оборудования Mode S
	 *          0 - нет
	 *          1 - есть
	 */
	u_int8_t mode_s_available;
	/**
	 * @brief Признак наличия оборудования UAT
	 *          0 - нет
	 *          1 - есть
	 */
	u_int8_t uat_available;

}TCat011AdsBRelatedData;

/**
 * @brief Определение структуры данных категории 011 Астерикс
 *
 */
typedef struct TCat011 TCat011;

/**
 * @brief Выдача указателя на структуру категории 011 из
 *        обобщенной структуры Астерикс
 * @param cat - указатель на структуру Астерикс
 * @return указатель на структуру категории 011 Астерикс,
 *         при отказе значение NULL
 */
TCat011* get011category(TCatAsterix* cat);
/**
 * @brief Распаковка записи категории 011(ed.1.12 May 2008) формата АСТЕРИКС УПФВ Интеграл
 * @param ptr - указатель на данные с кодограммой АСТЕРИКС
 * @return - указатель на структуру категории Астерикс
 */

TCatAsterix* decode011catCodogramm(TCommonObmenData* codogramm);

/**
 * @brief Запись типа сообщения в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param type_message - тип сообщения
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011MessageType(TCat011* cat, u_int8_t type_message);
/**
 * @brief Запрос типа сообщения из структуры категории 011
 * @param cat_lib - указатель на структуру категории 011
 * @return  - структура данных возврата:
 *         переменная val - содержит значение типа сообщения
 */
TReturnOneInt get011MessageType(TCat011* cat);
/**
 * @brief Запись типа выполняемого задания в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param type_mission  - тип выполняемого задания
 * @param trouble       - признак препятствия исполнению задания
 *                        0 - препятствий нет
 *                        1 - есть проблемы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011PreprogrammedMessage(TCat011* cat, u_int8_t type_mission, u_int8_t trouble);
/**
 * @brief Запрос типа выполняемого задания из структуры категории 011
 * @param cat_lib - указатель на структуру категории 011
 * @return  - структура данных возврата:
 *         переменная val1 - содержит значение типа выполняемого задания
 *         переменная val2 - содержит признак препятствия исполнению задания
 *                              0 - препятствий нет
 *                              1 - есть проблемы
 */
TReturnPairInt get011PreprogrammedMessage(TCat011* cat);
/**
 * @brief Запись этапа выполнения полета в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param flight_phase  - этап выполнения полета
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011PhaseOfFlight(TCat011* cat, Cat11PhaseOfFlight flight_phase);
/**
 * @brief Запрос этапа выполнения полета из структуры категории 011
 * @param cat_lib - указатель на структуру категории 011
 * @return  - структура данных возврата:
 *         переменная val - содержит значение этапа выполнения полета
 */
TReturnOneInt get011PhaseOfFlight(TCat011* cat);
/**
 * @brief Запись сообщения тревоги по наземной ситуации в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param alert   - указатель на структуру аварийной ситуации
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011AlertMessage(TCat011* cat, TCat011AlertMessage* alert);
/**
 * @brief Запрос тревоги по наземной ситуации из структуры категории 011
 * @param cat_lib - указатель на структуру категории 011
 * @return  - структура тревоги по наземной ситуации
 */
TCat011AlertMessage get011AlertMessage(TCat011* cat);
/**
 * @brief Упаковка сообщения категории 011 (ed. 1.2 May 2008) Астерикс
 * @param cat - указатель на структуру категории 011
 * @return - указатель на структуру с кодированным сообщением
 */
TCommonObmenData* pack011catRecord(TCat011* cat);
/**
 * @brief Распаковка записи категории 011(ed. 1.2 May 2008) формата АСТЕРИКС
 * @param codogramm - указатель на стурктуру обмена с кодограммой АСТЕРИКС
 * @param start_byte_pos - положение начала записи в теле кодограммы
 * @return - указатель на структуру категории Астерикс
 */
TCatAsterix* decode011catRecord(TCommonObmenData* codogramm, size_t* start_byte_pos);
/**
 * @brief Запись данных полученных от АЗН (Mode S) в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param fligth_phase  - этап выполнения полета
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011TrackAircraftRelatedData(TCat011* cat, TCat011AdsBRelatedData* ads_data);
/**
 * @brief Освобождение памяти после использования
 * @param ads_011 - указатель на структуру TCat011AdsBRelatedData
 */
void free011AdsBRelatedData(TCat011AdsBRelatedData* ads_011);
/**
 * @brief Выборка данных полученных от АЗН (Mode S) из контейнера категории
 * @param cat - указатель на структуру категории 011
 * @return указатель на структуру с данными АЗН - операция выполнена,
 *         NULL отказ исполнения
 */
TCat011AdsBRelatedData* get011TrackAircraftRelatedData(TCat011* cat);
/**
 * @brief Выборка "сырых" данных из записи категории 011 без распаковки
 * @param codogramm - указатель на контейнер с кодограммой
 * @param current_pos - начальное положение первого байта записи в теле кодограммы
 * @return - указатель на структуру контейнер Астерикс
 */
TCatAsterix* get011recordRawData(TCommonObmenData* codogramm, u_int64_t current_pos);
/**
 * @brief Запрос данных о содержимом FSPEC записи
 * @param cat - указатель структуру категории 011
 * @return - указатель на массив с байтами FSPEC записи
 */
u_int8_t* get011recordFspec(TCat011* cat, size_t* size);

/**
 * @brief Чтение признака текущей рулежки
 * @param cat - указатель на структуру категории 011
 * @return - структура обмена со статусом стоп-линий (uint16_t)
 */
TCommonObmenData* get011HoldbarStatus(TCat011* cat);

/**
 * @brief Запись признака текущей рулежки
 * @param cat - указатель на структуру категории 011
 * @param holdbar - структура обмена со статусом стоп-линий (uint16_t)
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011HoldbarStatus(TCat011* cat, TCommonObmenData* holdbar);

/**
 * @brief Чтение треков связанных с опасной ситуацией
 * @param cat - указатель на структуру категории 011
 * @return - структура обмена с массивом треков (uint16_t)
 */
TCommonObmenData* get011AlertTracks(TCat011* cat);

/**
 * @brief Запись треков связанных с опасной ситуацией
 * @param cat - указатель на структуру категории 011
 * @param alert_tr - структура обмена с массивом треков (uint16_t)
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011AlertTracks(TCat011* cat, TCommonObmenData* alert_tr);

#ifdef __cplusplus
}
#endif
#endif
