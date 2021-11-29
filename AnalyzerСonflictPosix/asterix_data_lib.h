#ifndef __ASTERIX_DATA_LIB__
#define __ASTERIX_DATA_LIB__

#include <sys/types.h>
#include <wchar.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char u_int8_t; 
typedef unsigned short u_int16_t;
typedef unsigned int u_int32_t;

#ifdef __linux__
    //linux code goes here
    typedef uint64_t u_int64_t;
#elif _WIN32
    // windows code goes here
    typedef unsigned long long u_int64_t;
#else
// #error "OS not supported!"
#endif


/**
 *  Type of maneuvering of the tracked object - acceleration
 */
typedef enum TargModeMov
{
	// Type of maneuvering of the tracked object - uniform movement
	TMM_MOVE_STAB              = 0,                    
	
	// Type of maneuvering of the tracked object - turning to the left
	TMM_TURN_LEFT              = 1,                    
	
	// Type of maneuvering of the tracked object - turning to the right
	TMM_TURN_RIGHT             = 2,                    
	
	// Type of maneuvering of the tracked object - acceleration
	TMM_INCREASING_GROUNDSPEED = 3,                    
	
	// Type of maneuvering of the tracked object - speed reduction
	TMM_DECREASING_GROUNDSPEED = 4,                    
	
	// Type of maneuvering of the tracked object - speed vertical climb
	TMM_VERTICAL_RATE_CLIMB    = 5,                    
	
	// Type of maneuvering of the tracked object - decrease in height
	TMM_VERTICAL_RATE_DESCENT  = 6                     

}TargModeMov;

/**
 *  Distress message designation
 */
typedef enum EmergStat
{
	EM_STAT_UNKN				= 0,              //  not installed
	EM_STAT_GEN					= 1,              //  general disaster
	EM_STAT_MED					= 2,              //  threat to human health
	EM_STAT_MIN_FUEL			= 3,              //  fuel accident
	EM_STAT_NO_COMMUNIC			= 4,              //  loss of connection
	EM_STAT_UNLAWFUL_INTERFER	= 5,              //  seizure of the side (attack)
	EM_STAT_DOWNED_AIRCRAFT     = 6,              //  emergency landing (lowering in height)
	EM_STAT_RESERV              = 7               
}EmergStat;


/**
 *  Sign of the data source for measuring the path of an airborne object
 */
typedef enum MeasDataSrc
{
	// Data source attribute - not defined
	MDS_NO_DETECTED          = 0,                      
	
	// Data source attribute - Primary radar 
	MDS_PSR                  = 1,                      
	
	// Data source attribute - Secondary radar 
	MDS_SSR                  = 2,                      
	
	// Data source attribute - Primary and Secondary radars
	MDS_PSR_SSR              = 3,                      
	
	// Data source attribute - receiver Mode S general poll
	MDS_MODE_S_ALL_CALL      = 4,                      
	
	// Data source attribute - receiver Mode S "roll call"
	MDS_MODE_S_ROLL_CALL     = 5,                      
	
	// Data source attribute - receiver Mode S general poll 
	// and Primary radar
	MDS_MODE_S_ALL_CALL_PSR  = 6,                      
	
	// Data source attribute - receiver Mode S "roll call" and Primary radar
	MDS_MODE_S_ROLL_CALL_PSR = 7                       
}MeasDataSrc;


/**
 *  Sampling in height from the aircraft avionics
 */
typedef enum AircraftSelectedAlt
{
	ASA_UNKNOWN       = 0,            // indefined
	ASA_FROM_AIRCRAFT = 1,            // altitude measured by aircraft avionics
	ASA_FROM_FCU_MCP  = 2,            // altitude according to the aircraft flight control module
	ASA_FROM_FMS      = 3             // altitude from aircraft flight control system
}AircraftSelectedAlt;


/**
 *  Nationality sign when working with the NRZ channel
 */
typedef enum IdentifFrRez
{
	IFR_ALIEN          = 1,
	IFR_OWN            = 2,
	IFR_OWN_GUARANTEED = 3,
	IFR_OWN_INDIVIDUAL = 4,
	IFR_NOT_TESTED     = 5
}IdentifFrRez;

/**
 *  Nationality sign when working with the NRZ channel
 */
enum InterrogSystemType
{
	IST_UNKNOWN = 0,
	IST_UVD     = 1,
	IST_NRZ     = 2,
	IST_MARK    = 3
};
typedef enum InterrogSystemType InterrSystemType;


/**
 *  Plan time binding flag
 */
enum PlanDayAttach
{
	PDA_TODAY     = 0,               // Sign of the action of the plan time - the current day
	PDA_YESTERDAY = 1,               // The sign of the plan time is yesterday
	PDA_TOMORROW  = 2                // Sign of the action of the time of the plan - tomorrow
};
typedef enum PlanDayAttach PlanDayAttach;

/**
 *  Sign of admission to RVSM
 */
enum RvsmAvailable
{
	RA_UNKNOWN      = 0,                      // RVSM Approval Sign - Unknown
	RA_APPROVED     = 1,                      // RVSM Approval Sign - Approved
	RA_EXTEMPT      = 2,                      // RVSM approval mark - free admission
	RA_NOT_APPROVED = 3                       // RVSM Approval Sign - Not Approved
};
typedef enum RvsmAvailable RvsmAvailable;

/**
 *  Flight rules field 4 (bit 4,5) ed 390
 */
enum FligthRulesType
{
	// Flight rules - instrument flight
	FRT_INSTRUMENTAL     = 0,                  
	
	// Flight rules - flight with visual control
	FRT_VISUAL           = 1,                  
	
	// Flight rules - flight with visual control and dispatch support
	FRT_CONTROLED_VISUAL = 3                   
	                                           
};
typedef enum FligthRulesType FligthRulesType;

/**
 *  Plan time type for field 12 ed 380
 */
enum TimeOfDepartArriv
{
	// Time type - the planned time of leaving the parking blocks
	TODA_SCHEDULED_OFF_BLOCK   = 0,                    
	
	// Time type - estimated time of leaving the parking blocks
	TODA_ESTIMATED_OFF_BLOCK   = 1,                    
	
	// Time type - estimated take-off time
	TODA_ESTIMATED_TAKE_OFF    = 2,                    
	
	// Time type - actual (valid) time of leaving the parking blocks
	TODA_ACTUAL_OFF_BLOCK      = 3,                    
	
	// Time type - estimated runway time
	TODA_PREDICTED_HOLD_RUNWAY = 4,                    
	
	// Time type - actual (actual) runway occupation time
	TODA_ACTUAL_HOLD_RUNWAY    = 5,                    
	
	// Тип времени - актуальное (действительное) время занятия исходной позиции на ВПП
	TODA_ACTUAL_LINE_UP        = 6,                    
	
	// Тип времени - актуальное (действительное) время взлета
	TODA_ACTUAL_TAKE_OFF       = 7,                    
	
	// Тип времени - расчетное время прибытия
	TODA_ESTIMATED_ARRIVAL     = 8,                    
	
	// Тип времени -  предполагаемое время посадки
	TODA_PREDICTED_LANDING     = 9,                    
	
	// Тип времени - актуальное (действительное) время посадки
	TODA_ACTUAL_LANDING        = 10,                   
	
	// Тип времени - актуальное (действительное) время схода с ВПП
	TODA_ACTUAL_OFF_RUNWAY     = 11,                   
	
	// Тип времени - предполагаемое время подачи к высадке пассажиров
	TODA_PREDICTED_TO_GATE     = 12,                   
	
	// Тип времени - актуальное (действительное) время постановки на стоянку
	TODA_ACTUAL_ON_BLOCK       = 13,                   
	
	//!< - общее количество типов времен
	TODA_MAX_COUNT             = 14                    
};
typedef enum TimeOfDepartArriv TimeOfDepartArriv;


/**
 * Идентификатор типа категории передатчика
 */
enum EmitterCategoryType
{
	ECAT_UNDEFINED                 = 0,                //!< Не определен
	ECAT_LIGHT_AIRCRAFT            = 1,                //!< Легкий ЛА
	ECAT_SMALL_AIRCRAFT            = 2,                //!< Малый ЛА
	ECAT_MEDIUM_AIRCRAFT           = 3,                //!< Средний ЛА
	ECAT_HIGH_VORTEX               = 4,                //!< Воздушное судно с сильным вихревым следом
	ECAT_HEAVY_AIRCRAFT            = 5,                //!< Тяжелый ЛА
	ECAT_SPEED_AND_MANOEUVRABLE    = 6,                //!< Скоростной и маневренный ЛА
	ECAT_RESERVED1                 = 7,                //!< Зарезервировано
	ECAT_RESERVED2                 = 8,                //!< Зарезервировано
	ECAT_RESERVED3                 = 9,                //!< Зарезервировано
	ECAT_ROTORCRAFT                = 10,               //!< Вертолет/автожир
	ECAT_GLIDER                    = 11,               //!< Планер
	ECAT_AIR_BALLOON               = 12,               //!< Воздушный шар
	ECAT_UAV                       = 13,               //!< Беспилотный ЛА
	ECAT_SPACE_SHUTTLE             = 14,               //!< Трансатмосферный/космический ЛА
	ECAT_PARA_GLIDER               = 15,               //!< Дельтаплан
	ECAT_PARACHUTIST               = 16,               //!< Парашютист
	ECAT_RESERVED4                 = 17,               //!< Зарезервировано
	ECAT_RESERVED5                 = 18,               //!< Зарезервировано
	ECAT_RESERVED6                 = 19,               //!< Зарезервировано
	ECAT_SURFACE_EMERGENCY_VEHICLE = 20,               //!< Наземное ТС спасательное
	ECAT_SURFACE_SERVICE_VEHICLE   = 21,               //!< Наземное ТС служебное
	ECAT_FIXED_GROUND              = 22,               //!< Наземный стационарный объект
	ECAT_CLUSTER_OBSTACLE          = 23,               ///< Групповое препятствие
	ECAT_LINE_OBSTACLE             = 24,               ///< Линейное препятствие
};
typedef enum EmitterCategoryType EmitterCategoryType;

/**
 *  @brief Признак источника идентификатора цели (позывного)
 */
typedef enum TargetIdentificationSource
{

	TIS_FROM_TRANSPONDER                  = 0,         //!< - идентификатор или позывной получен от бортового передатчика
	TIS_CALLSIGN_NOT_FROM_TRANSPONDER     = 1,         //!< - позывной получен не от бортового передатчика
	TIS_REGISTRATION_NOT_FROM_TRANSPONDER = 2,         //!< - идентификатор получен не от бортового передатчика
	TIS_UNDEFINED                         = 3,

	// Для категории 20
	TIS_NOT_FROM_TRANSPONDER          = 0,             //!< - идентификатор или позывной получен не от бортового передатчика
	TIS_REGISTRATION_FROM_TRANSPONDER = 1,             //!< - идентификатор получен от бортового передатчика
	TIS_CALLSIGN_FROM_TRANSPONDER     = 2,             //!< - позывной получен от бортового передатчика
}TargetIdentificationSource;

/**
 * @brief Идентификаторы типа данных системного времени
 *        устаревания данных трассы
 */
enum SystemTrackUpdateTimeType
{
	STUTT_UNDEFINED_AGE = 0,                           //!< Тип времени не определен
	STUTT_TRACK_AGE     = 1,                           //!< Тип времени - время устаревания трассы
	STUTT_PSR_AGE       = 2,                           //!< Тип времени - время устаревания данных ПРЛ
	STUTT_SSR_AGE       = 3,                           //!< Тип времени - время устаревания данных ВРЛ
	STUTT_MODE_S_AGE    = 4,                           //!< Тип времени - время устаревания данных Mode S
	STUTT_ADS_C_AGE     = 5,                           //!< Тип времени - время устаревания данных ADS-C
	STUTT_ADS_B_AGE     = 6,                           //!< Тип времени - время устаревания данных ADS-B
	STUTT_VDL_MODE4_AGE = 7,                           //!< Тип времени - время устаревания данных VDL Mode 4
	STUTT_UAT_AGE       = 8,                           //!< Тип времени - время устаревания данных UAT
	STUTT_LOOP_AGE      = 9,                           //!< Тип времени - время устаревания данных Loop system(пеленгатора)
	STUTT_MLAT_AGE      = 10,                          //!< Тип времени - время устаревания данных MLAT
	STUTT_MFL_AGE       = 11,                          //!< Тип времени - время устаревания данных об измеренном эшелоне полета
	STUTT_MD1_AGE       = 12,                          //!< Тип времени - время устаревания данных о номере борта в режиме Mode1
	STUTT_MD2_AGE       = 13,                          //!< Тип времени - время устаревания данных о номере борта в режиме Mode2
	STUTT_MDA_AGE       = 14                           //!< Тип времени - время устаревания данных о номере борта в режиме Mode 3/A

};
typedef enum SystemTrackUpdateTimeType SystemTrackUpdateTimeType;

/**
 *  @brief Признак выполнения задачи на летном поле
 */
enum WorkProgrammType
{

	WORK_PROGRAMM_TOWING_AIRCRAFT     = 1,             //!< - буксирование самолета
	WORK_PROGRAMM_FOLLOW_ME_OPERATION = 2,             //!< - сопровождение самолета
	WORK_PROGRAMM_RUNWAY_CHECK        = 3,             //!< - контроль ВПП
	WORK_PROGRAMM_EMERGENCY_OPERATION = 4,             //!< - спасательные действия
	WORK_PROGRAMM_WORK_MAINTENANCE    = 5              //!< - работа по задаче
};

/**
 *  @brief Признак принадлежности по наземного ТС по назначению
 */
enum VehicleFleetIdType
{

	VFID_UNKNOWN           = 0,                        //!< - принадлежность не известна
	VFID_ATC_EQUIP_MAINTEN = 1,                        //!< - обслуживание оборудования УВД
	VFID_AIRPORT_MAINTEN   = 2,                        //!< - аэропортовая служба
	VFID_FIRE              = 3,                        //!< - противопожарная безопасность
	VFID_BIRD_SCARER       = 4,                        //!< - орнитологическая служба (отпугивание птиц)
	VFID_SNOW_PLOUG        = 5,                        //!< - снегоуборщик
	VFID_RUNWAY_SWEEPER    = 6,                        //!< - уборка ВПП
	VFID_EMERGENCY         = 7,                        //!< - служба спасения
	VFID_POLICE            = 8,                        //!< - служба безопасности(полиция)
	VFID_BUS               = 9,                        //!< - автобус
	VFID_TUG               = 10,                       //!< - тягач
	VFID_GRASS_CUTTER      = 11,                       //!< - газонокосильщик
	VFID_FUEL              = 12,                       //!< - топливозаправщик
	VFID_BAGGAGE           = 13,                       //!< - перевозка багажа
	VFID_CATERING          = 14,                       //!< - бортовое питание
	VFID_AIRCRAFT_MAINTEN  = 15,                       //!< - обслуживание самолетов
	VFID_FOLLOW_ME         = 16                        //!< - машина диспетчерской службы(follow me)
};

/**
 * @brief Идентификаторы типа данных времени
 *        устаревания данных полученных с борта цели
 */
enum TrackDataAgesType
{
	TUAT_UNDEFINED_AGE = 0,                            //!< Тип времени не определен
	TUAT_MFL_AGE       = 1,                            //!< время старения данных о измеренном эшедоне полета
	TUAT_MD1_AGE       = 2,                            //!< время старения данных о номере борта в режиме Mode1
	TUAT_MD2_AGE       = 3,                            //!< время старения данных о номере борта в режиме Mode2
	TUAT_MDA_AGE       = 4,                            //!< время старения данных о номере борта в режиме Mode 3/A
	TUAT_MD4_AGE       = 5,                            //!< время старения данных о номере борта в режиме Mode4
	TUAT_MD5_AGE       = 6,                            //!< время старения данных о номере борта в режиме Mode5
	TUAT_MHG_AGE       = 7,                            //!< время старения данных о магнитном курсе борта
	TUAT_IAS_AGE       = 8,                            //!< время старения данных о приборной воздушной скорости
	TUAT_TAS_AGE       = 9,                            //!< время старения данных о реальной воздушной скорости
	TUAT_SAL_AGE       = 10,                           //!< время старения данных o выборке по высоте
	TUAT_FSS_AGE       = 11,                           //!< время старения данных о финальной выборки высоты
	TUAT_TID_AGE       = 12,                           //!< время старения данных о точках траектории полета борта
	TUAT_COM_AGE       = 13,                           //!< время старения данных о коммуникации и аппаратуре ACAS
	TUAT_SAB_AGE       = 14,                           //!< время старения данных о статусе аппаратуры ADS-B
	TUAT_ACS_AGE       = 15,                           //!< время старения данных о работоспособности аппаратуры ACAS
	TUAT_BVR_AGE       = 16,                           //!< время старения данных о барометрической высоте
	TUAT_GVR_AGE       = 17,                           //!< время старения данных о геометрической высоте
	TUAT_RAN_AGE       = 18,                           //!< время старения данных о текущем угле поворота
	TUAT_TAR_AGE       = 19,                           //!< время старения данных о скорости поворота
	TUAT_TAN_AGE       = 20,                           //!< время старения данных о заданном угле поворота
	TUAT_GSP_AGE       = 21,                           //!< время старения данных о наземной скорости
	TUAT_VUN_AGE       = 22,                           //!< время старения данных о неопределенности воздушной скорости
	TUAT_MET_AGE       = 23,                           //!< время старения данных o выборке метеоданных
	TUAT_EMC_AGE       = 24,                           //!< время старения данных о категории передатчика
	TUAT_POS_AGE       = 25,                           //!< время старения данных о местоположении
	TUAT_GAL_AGE       = 26,                           //!< время старения данных o геометрической высоте
	TUAT_PUN_AGE       = 27,                           //!< время старения данных о значении неопределенности местоположения
	TUAT_MB_AGE        = 28,                           //!< время старения данных сообщений Mode S
	TUAT_IAR_AGE       = 29,                           //!< время старения данных о приборной воздушной скорости
	TUAT_MAC_AGE       = 30,                           //!< время старения данных o скорости в махах
	TUAT_BPS_AGE       = 31                            //!< время старения данных o заданном барометрическом давлении
};
typedef enum TrackDataAgesType TrackDataAgesType;

/**
 * @brief Идентификаторы типа данных для TCommonObmenData
 */
enum CommonObmenDataType
{
	CODT_UNDEFINED = 0,                                //!< Тип данных не определен
	CODT_CAT_010   = 10,                               //!< Тип данных - категория 010 Астерикс
	CODT_CAT_011   = 11,                               //!< Тип данных - категория 011 Астерикс
	CODT_CAT_062   = 62                                //!< Тип данных - категория 062 Астерикс
};
typedef enum CommonObmenDataType CommonObmenDataType;

/**
 * @brief Структура данных для хранения кода
 *          бортового номера
 */
typedef struct BoardNumCode
{
	/**
	 * @brief Получен на текущем обзоре
	 *        0 - обновления номера не было
	 *        1 - получен на текущем обзоре
	 */
	u_int8_t change;
	/**
	 * @brief Признак искажения кода номера
	 *        0 - не искажён
	 *        1 - искажён
	 */
	u_int8_t garbled;
	/**
	 * @brief Признак достоверности кода номера
	 *        0 - код достоверен
	 *        1 - код не достоверен
	 */
	u_int8_t valid;
	/**
	 * @brief Код бортового номера
	 */
	u_int32_t board_number_code;

}BoardNumCode;
/**
 * @brief Структура данных для хранения кода
 *          барометрической высоты полученной с борта ВО
 *          (Mode C, УВД, НРЗ)
 */
typedef struct BoardAltitudeCode
{
	/**
	 * @brief Получен на текущем обзоре
	 *        0 - обновления высоты не было
	 *        1 - высота получена на текущем обзоре
	 */
	u_int8_t change;
	/**
	 * @brief Признак искажения кода высоты
	 *        0 - высота не искажён
	 *        1 - высота искажёна
	 */
	u_int8_t garbled;
	/**
	 * @brief Признак достоверности кода высоты
	 *        0 - код высоты достоверен
	 *        1 - код высоты не достоверен
	 */
	u_int8_t valid;
	/**
	 * @brief Высота
	 */
	double altitude;

}BoardAltitudeCode;

/**
 * @brief Структура данных для передачи статуса трассы
 */
typedef struct TTrackStatus
{
	/**
	 * @brief Признак сопровождения трассы по данным
	 *          1 - одного источника
	 *          0 - мультирадарное сопровождение
	 */
	u_int8_t monosensor;
	/**
	 * @brief Признак получения сигнала SPI от аппаратуры
	 *          1 - получен сигнал
	 *          0 - нет получения сигнала
	 */
	u_int8_t spi_sign;
	/**
	 * @brief Признак предпочтения высоты (более достоверной)
	 *          1 - геометрическая высота
	 *          0 - барометрическая высота (Mode C)
	 */
	u_int8_t most_reliable_height;
	/**
	 * @brief Признак источника геометрической высоты
	 *          в соотвествии с определением ALTITUDE_SRC_хххххх
	 */
	u_int8_t geometric_altitude_source;
	/**
	 * @brief Признак подтверждения (обновления) данных
	 *          0 - данные подтверждаюся
	 *          1 - данные предположительные (экстраполяция отметки)
	 */
	u_int8_t confirm;
	/**
	 * @brief Признак эталонной (имитированной) трассы
	 *          0 - действительная трасса
	 *          1 - имитированная трасса
	 */
	u_int8_t simulate;
	/**
	 * @brief Признак последнего сообщения о трассе
	 *          0 - значение по умолчанию, обновление информации
	 *          1 - последнее сообщение, сброс трассы с сопровождения
	 */
	u_int8_t last_message;
	/**
	 * @brief Признак первого сообщения о трассе
	 * 0 - значение по умолчанию, обновление информации
	 * 1 - первое сообщение, новая трасса на сопровождении
	 */
	u_int8_t first_message;
	/**
	 * @brief Признак подтверждения с плановой информацией
	 * 0 - трасса вне плана, нет подтверждения
	 * 1 - трасса подтверждена плановой информацией
	 */
	u_int8_t flight_in_plan;
	/**
	 * @brief Признак наличия противоречия между данными наблюдения и данными полученными по каналу АЗН
	 * 0 - значение по умолчанию, нет противоречия
	 * 1 - есть расхождение данных
	 */
	u_int8_t ads_inconsist;
	/**
	 * @brief Признак трансляции трассы от подчиненных источников
	 *           0 - значение по умолчанию, выдается собственная трасса
	 *           1 - ретрансляция трассы подчиненного источника
	 */
	u_int8_t slave_track;
	/**
	 * @brief Признак приоритетного обслуживания трассы (литерный борт)
	 *           0 - трасса на приоритетном обслуживании
	 *           1 - обычное обслуживание, стандартный траффик
	 */
	u_int8_t background_serv;
	/**
	 * @brief Признак отождествленной трассы (результат третичной обработки нескольких трасс от разных источников)
	 *           0 - значение по умолчанию, трасса не отождествлена
	 *           1 - отождествленная трасса
	 */
	u_int8_t amalgamation;
	/**
	 * @brief Признак результата распознавания в режиме Mode 4
	 */
	u_int8_t mode4_interrogate;
	/**
	 * @brief Признак поступления бедствия от военного борта
	 *           0 - значение по умолчанию, нет бедствия
	 *           1 - принят сигнал бедствия
	 */
	u_int8_t military_emrg;
	/**
	 * @brief Признак идентификации принадлежности борта к военному ведомству
	 *           0 - значение по умолчанию, нет бедствия
	 *           1 - принят сигнал бедствия
	 */
	u_int8_t military_id;
	/**
	 * @brief Признак результата распознавания в режиме Mode 4
	 */
	u_int8_t mode5_interrogate;
	/**
	 * @brief Признак устаревания времени последнего полученного обновления
	 *           трека больше порога времени работного(инерции) системы
	 *           0 - значение по умолчанию, трасса обновляется
	 *           1 - трасса не обновлялась, экстраполяция трассы
	 */
	u_int8_t coast_track;
	/**
	 * @brief Признак устаревания данных трека, полученных по каналу ПРЛ
	 *           больше порога времени работного(инерции) системы
	 *           0 - значение по умолчанию, трасса обновляется
	 *           1 - трасса не обновлялась, нет данных ПРЛ
	 */
	u_int8_t psr_coast;
	/**
	 * @brief Признак устаревания данных трека, полученных по каналу ВРЛ
	 *           больше порога времени работного(инерции) системы
	 *           0 - значение по умолчанию, трасса обновляется
	 *           1 - трасса не обновлялась, нет данных ВРЛ
	 */
	u_int8_t ssr_coast;
	/**
	 * @brief Признак устаревания данных трека, полученных по каналу Mode S
	 *           больше порога времени работного(инерции) системы
	 *           0 - значение по умолчанию, трасса обновляется
	 *           1 - трасса не обновлялась, нет данных Mode S
	 */
	u_int8_t mode_s_coast;
	/**
	 * @brief Признак устаревания данных трека, полученных по каналу ADS-B
	 *           больше порога времени работного(инерции) системы
	 *           0 - значение по умолчанию, трасса обновляется
	 *           1 - трасса не обновлялась, нет данных ADS-B
	 */
	u_int8_t ads_coast;
	/**
	 * @brief Признак использования специального кода режима Mode A (номер борта)
	 *           0 - значение по умолчанию, специальный номер борта не используется
	 *           1 - используется специальный номер борта
	 */
	u_int8_t suc_set;
	/**
	 * @brief Признак наличия конфликта номеров в режиме Mode A (номер борта)
	 *           0 - значение по умолчанию, нет конфликта
	 *           1 - есть трасса с аналогичным кодом номера борта
	 */
	u_int8_t aac_conflict;                             // assigned mode A Code conflict
	/**
	 * @brief Признак статуса данных наземного наблюдения
	 */
	u_int8_t sds_status;
	/**
	 * @brief Признак типа бедствия
	 *           в соотвествии с определением EMERGENCY_STATUS_хххххх
	 */
	u_int8_t emergency_status;
	/**
	 * @brief Признак выставления нахождения бортового передатчика АЗН (Mode S) на земле
	 *           0 - значение по умолчанию, не установлено
	 *           1 - установлен бит "на земле"
	 */
	u_int8_t ground_bit_set;

	/**
	 * @brief Признак маневра в горизонтальной плоскости (по Х, У)
	 *           0 - нет маневра
	 *           1 - есть маневр
	 */
	u_int8_t horizontal_manoeuvre;

	/**
	 * @brief Признак пересчета наклонных координат положения цели
	 *        на горизонтальную плоскость (для Х, У)
	 *           0 - нет пересчета
	 *           1 - есть пересчет
	 */
	u_int8_t slant_correction;

	/**
	 * @brief Признак преобразования координат положения цели
	 *        (для Х, У)
	 *        0 - данные результатов измерения
	 *        1 - сглаженные данные
	 */
	u_int8_t smooth_position;

}TTrackStatus;

/**
 * @brief время плановых данных UTC
 * номер ячейки массива соответствует типу времени плана
 */
typedef struct TTimeDepArrival
{
	// признак актуальности времени 1 - актуально, 0 - не актуально
	u_int8_t active;
	/**
	 *   @brief Признак принадлежности к времени суток
	 *           0 - текущие сутки
	 *           1 - вчера (до текущих суток),
	 *           2 - завтра (после текущих суток)
	 */
	u_int8_t curr_day;
	u_int8_t hour;
	u_int8_t min;
	u_int8_t sec;
} TTimeDepArrival;

/**
 * @brief Структура данных для плановой информации Астерикс
 *
 */
typedef struct TFlightPlanData
{
	/**
	 * @brief идентификатор источника плановой информации
	 */
	u_int16_t plan_sac_sic;
	/**
	 * @brief позывной текущий
	 */
	char callsign[8];
	wchar_t callsign_w[8];
	/**
	 * @brief идентификатор плана - номер плана
	 */
	u_int64_t plan_identificator;
	/**
	 * @brief признак общего воздушного движения
	 *        0 - нет признака
	 *        1 - признак установлен
	 */
	u_int8_t general_air_traffic;
	/**
	 * @brief признак действующего воздушного движения
	 *        0 - нет признака
	 *        1 - признак установлен
	 */
	u_int8_t operational_air_traffic;
	/**
	 * @brief признак правила полета
	 */
	u_int8_t flight_rules;
	/**
	 * @brief признак допуска к использованию RVSM
	 */
	u_int8_t rvsm_approve;
	/**
	 * @brief Признак высшего приоритета (литерный, особый диспетчесркий контроль)
	 *        0 - нет признака
	 *        1 - признак установлен
	 */
	u_int8_t priority;
	/**
	 * @brief тип воздушного судна - 4 символа
	 */
	char type_aircraft[5];
	wchar_t type_aircraft_w[5];
	/**
	 * @brief тип турбулентности - символ - L-легкий, M-средний, H-тяжелый, J-супертяж.
	 */
	char turbulence;
	wchar_t turbulence_w;
	/**
	 * @brief аэропорт вылета - 4 символа
	 */
	char dep_airport[5];
	wchar_t dep_airport_w[5];
	/**
	 * @brief аэропорт назначения(прибытия) - 4 символа
	 */
	char dest_airport[5];
	wchar_t dest_airport_w[5];
	/**
	 * @brief назначенная ВПП - 3 символа
	 */
	char dest_runway[4];
	wchar_t dest_runway_w[4];
	/**
	 * @brief заданный эшелон
	 */
	u_int16_t current_fl;
	/**
	 * @brief код цетра управления Current Control Position - 1 байт(ст.) код центра, 2 байт - код позиции
	 */
	u_int16_t current_ctrl_pos;
	/**
	 * @brief количество данных о времени
	 */
	u_int8_t fptime_count;
	/**
	 * @brief время в UTC
	 * номер ячейки массива соответствует типу времени плана
	 */
	TTimeDepArrival fptime[TODA_MAX_COUNT];
	/**
	 * @brief назначенная стоянка - 6 символов
	 */
	char aircraft_stand[7];
	/**
	 * @brief признак использования стоянки
	 */
	u_int8_t stand_status;
	/**
	 * @brief признак доступности стоянки
	 */
	u_int8_t stand_available;
	/**
	 * @brief стандартный маршрут вылета - 7 символов
	 */
	char sid[8];
	/**
	 * @brief стандартный маршрут посадки - 7 символов
	 */
	char star[8];
	/**
	 * @brief номер борта Mode3А до бедствия
	 */
	u_int16_t pre_emerg_m3a_code;
	/**
	 * @brief позывной до бедствия
	 */
	char pre_emerg_callsign[8];

}TFlightPlanData;

/**
 * @brief Структура данных для категории Астерикс
 *        точностные данные трассы
 *
 */
typedef struct TTrackAccuracy
{
	/**
	 * @brief расчетная точность положения по оси Х (СКО)
	 */
	double x_component;
	/**
	 * @brief расчетная точность положения по оси У (СКО)
	 */
	double y_component;
	/**
	 * @brief расчетная ковариация положения по осям Х и У (СКО)
	 *  Формула расчета ковариации
	 *  xy_cov_component = sign{Cov(X,Y)} * sqrt{abs[Cov(X,Y)]}
	 *  максимальное значение = 16.383 км
	 */
	double xy_cov_component;
	/**
	 * @brief расчетная точность положения широте (СКО)
	 */
	double lat_component;
	/**
	 * @brief расчетная точность положения по долготе (СКО)
	 */
	double longit_component;
	/**
	 * @brief расчетная точность положения по геометрической высоте (СКО)
	 */
	double h_geo_component;
	/**
	 * @brief расчетная точность положения по барометрической высоте (СКО)
	 */
	double h_bar_component;
	/**
	 * @brief расчетная точность определения скорости по оси Х (СКО)
	 */
	double vx_component;
	/**
	 * @brief расчетная точность определения скорости по оси У (СКО)
	 */
	double vy_component;
	/**
	 * @brief расчетная точность определения скорости по оси Н(высоте) (СКО)
	 */
	double vh_component;
	/**
	 * @brief расчетная точность определения ускорения по оси Х (СКО)
	 */
	double ax_component;
	/**
	 * @brief расчетная точность определения ускорения по оси У (СКО)
	 */
	double ay_component;
}TTrackAccuracy;

/**
 * @brief Структура данных для категории Астерикс
 *        данные для передачи информации УВД и
 *        иной дополнительной информации в поле SP
 */
typedef struct TSpData
{
	/**
	 * @brief Признак искажения данных кода
	 *        бортового номера (УВД/НРЗ) ВО
	 *        0 - код не искажен
	 *        1 - код искажен
	 */
	u_int8_t board_num_garbled;
	/**
	 * @brief Признак достоверности данных кода
	 *        бортового номера (УВД/НРЗ) ВО
	 *        0 - код достоверен
	 *        1 - код не достоверен
	 */
	u_int8_t board_num_valid;
	/**
	 * @brief Признак обновления данных кода
	 *        бортового номера (УВД/НРЗ) ВО
	 *        0 - код получен на текущем обзоре
	 *        1 - код не обновлялся
	 */
	u_int8_t board_num_update;
	/**
	 * @brief Код бортового номера (УВД/НРЗ) ВО
	 */
	u_int32_t board_num;
	/**
	 * @brief Остаток топлива на борту (УВД/НРЗ) ВО
	 *        в процентах от общего объема запаса топлива
	 */
	u_int8_t fuel;
	/**
	 * @brief Признак относительно измеренной барометрической
	 *        высоты (УВД/НРЗ) ВО
	 *        0 - высота измерена относительно уровня моря
	 *        1 - высота измерена относительно аэродрома
	 */
	u_int8_t barometric_altitude_absolute;
	/**
	 * @brief Признак получения сигнала Бедствие по каналу (УВД/НРЗ)
	 *        0 - нет Бедствия
	 *        1 - получен сигнал Бедствие
	 */
	u_int8_t sos;
	/**
	 * @brief Признак обновления данных государственного опознавания ВО
	 *        по каналу (УВД/НРЗ)
	 *        0 - данные опознавания не обновлялись
	 *        1 - данные опознавания обновлены
	 */
	u_int8_t ogp_update;
	/**
	 * @brief Признак государственного опознавания(УВД/НРЗ) ВО
	 */
	u_int8_t ogp;
	/**
	 * @brief Признак системы получения данных опознавания (УВД/НРЗ) ВО
	 *        0 - система не опрелена
	 *        1 - система НРЗ
	 *        2 - система ИО (индивидуального опознавания)
	 */
	u_int8_t interrogate_system;
	/**
	 * @brief Данные курсового параметра (УВД/НРЗ) ВО
	 */
	float uvd_course;
	/**
	 * @brief Данные курсовой скорости (УВД/НРЗ) ВО
	 */
	float uvd_speed;
	/**
	 * @brief Данные барометрической высоты (УВД/НРЗ) ВО
	 */
	float altitude_barometric;
	/**
	 * @brief Признак искажения данных кода
	 *        барометрической высоты (УВД/НРЗ) ВО
	 *        0 - код не искажен
	 *        1 - код искажен
	 */
	u_int8_t altitude_garbled;
	/**
	 * @brief Признак достоверности данных кода
	 *        барометрической высоты (УВД/НРЗ) ВО
	 *        0 - код достоверен
	 *        1 - код не достоверен
	 */
	u_int8_t altitude_valid;

}TSpData;

/**
 * @brief Определение структуры возвращаемых данных
 *        категории Астерикс тип 1
 */
typedef struct TReturnOneInt
{
	/**
	 * @brief Признак обновления данных в val
	 *        0 - данные не обновлялись
	 *        1 - данные обновлены
	 */
	u_int8_t update;
	int      val;
}TReturnOneInt;

/**
 * @brief Определение структуры возвращаемых данных
 *        категории Астерикс тип 2
 */
typedef struct TReturnOneDouble
{
	/**
	 * @brief Признак обновления данных в val
	 *        0 - данные не обновлялись
	 *        1 - данные обновлены
	 */
	u_int8_t update;
	double   val;
}TReturnOneDouble;
/**
 * @brief Определение структуры возвращаемых данных
 *        категории Астерикс тип 3
 */
typedef struct TReturnPairInt
{
	/**
	 * @brief Признак обновления данных в val1 и val2
	 *        0 - данные не обновлялись
	 *        1 - данные обновлены
	 */
	u_int8_t update;
	int      val1;
	int      val2;
}TReturnPairInt;

/**
 * @brief Определение структуры возвращаемых данных
 *        категории Астерикс тип 4
 */
typedef struct TReturnPairDouble
{
	/**
	 * @brief Признак обновления данных в val1 и val2
	 *        0 - данные не обновлялись
	 *        1 - данные обновлены
	 */
	u_int8_t update;
	double   val1;
	double   val2;
}TReturnPairDouble;

/**
 * @brief Определение структуры контейнера категорий Астерикс
 */
typedef struct TCatAsterix TCatAsterix;

/**
 * @brief Определение структуры данных обмена
 */
typedef struct TCommonObmenData TCommonObmenData;

/**
 * @brief Освобождение памяти занятой структурой категории Астерикс
 * @param cat - указатель на структуру категории Астерикс
 */
void freeCatAsterixData(TCatAsterix* cat);
/**
 * @brief Инициализация структуры Астерикс в памяти
 * @param cat_num - номер категории Астерикс
 * @return - указатель на созданную структуру, NULL - отказ
 */
TCatAsterix* createCatAsterixData(unsigned int cat_num);
/**
 * @brief Функция выдачи номера категории Астерикс
 * @param cat_lib - указатель на структуру категории
 * @return - номер категории, 0 - в случае отказа
 */
// CommonObmenDataType getCatergoryNum(TCatAsterix* cat_lib);
int getCatergoryNum(TCatAsterix* cat_lib);
/**
 * @brief Функция выдачи максимального индекса категории Астерикс
 *        (для определения количества элементов в профиле пользователя категории)
 * @param cat_lib - указатель на структуру категории
 * @return - номер категории, 0 - в случае отказа
 */
int getCatIndexSize(TCatAsterix* cat_lib);

/**
 * @brief Определение структуры данных обмена
 */
typedef struct TCommonObmenData TCommonObmenData;

/**
 * @brief Инициализация структуры массива данных обмена библиотеки
 * @param data_len - размер данных в байтах
 * @return указатель на созданный массив данных
 */
TCommonObmenData* initObmenData(size_t data_len);
/**
 * @brief Освобождение памяти структуры обмена данными бибилиотеки
 * @param data - указатель на массив данных
 */
void freeObmenData(TCommonObmenData* data);
/**
 * @brief Запись данных для обмена в структуру массива данных библиотеки
 * @param data      - указатель на структуру массива данных библиотеки
 * @param info      - указатель на данные
 * @param info_len  - длина данных в баййтах
 * @return  - результат записи
 */
int setObmenData(TCommonObmenData* data, void* info, size_t info_len);
/**
 * @brief Получение указателя на данные в массиве обмена бибилиотеки
 * @param data  - указатель на структуру обмена данными библиотеки
 * @return - указатель на массив данных
 */
void* getObmenData(TCommonObmenData* data);
/**
 * @brief Получение длины данных в массиве обмена библиотеки
 * @param data - указатель на структуру данных обмена бибилиотеки
 * @return - длина данных массива обмена в байтах
 */
size_t getObmenDataLenght(TCommonObmenData* data);

/**
 * @brief Запись идентификатора источника в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param cat_num - номер категории
 * @param service_id - код сервиса
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setServiceIdentificator(TCatAsterix* cat_lib, u_int8_t service_id);
/**
 * @brief Запись идентификатора источника в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param cat_num - номер категории
 * @param sac_sic_code - код источника данных
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setDataSourceIdentificator(TCatAsterix* cat_lib, u_int16_t sac_sic_code);
/**
 * @brief Запись идентификатора источника в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param time_of_track - суточное время (UTC) трека, в секундах от 00:00
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTimeOfTrackInformation(TCatAsterix* cat_lib, double time_of_track);
/**
 * @brief Запись идентификатора источника в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param latitude - широта трассы, в радианах
 * @param longitude - долгота трека, в радианах
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackPositionWgs84(TCatAsterix* cat_lib, double latitude, double longitude);
/**
 * @brief Запись положения трассы в прямоугольных координатах в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param x - координата Х положения трассы, в метрах
 * @param y - координата У положения трассы, в метрах
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackPositionCartesian(TCatAsterix* cat_lib, int x, int y);

/**
 * @brief Запись положения трассы в полярных координатах в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param azimuth - азимут положения трассы, в радианах
 * @param distance - дальность положения трассы, в метрах
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackPositionPolar(TCatAsterix* cat_lib, double azimuth, int distance);

/**
 * @brief Запись значения скорости трассы в прямоугольных координатах в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param vx - вектор скорости трассы по координате Х, в метрах/секунду
 * @param vy - вектор скорости трассы по координате У, в метрах/секунду
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackVelocityCartesian(TCatAsterix* cat_lib, double vx, double vy);

/**
 * @brief Запись значения скорости и курса трассы в полярных координатах в структуру категории
 * @param cat_lib   - указатель на структуру категории
 * @param velocity  - скорость трассы, в метрах/секунду
 * @param course    - курс трассы, в радианах
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackVelocityPolar(TCatAsterix* cat_lib, double velocity, double course);

/**
 * @brief Запись значения ускорения по скорости трассы в прямоугольных координатах в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param ax - вектор ускорения трассы по координате Х, в метрах/секунду2
 * @param ay - вектор ускорения трассы по координате У, в метрах/секунду2
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackAccelerationCartesian(TCatAsterix* cat_lib, double ax, double ay);
/**
 * @brief Запись значения номера RBS Mode 3/A в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param mode3a_code - код номера в режиме 3/А
 * @param change_code - признак обновления данных номера в режиме 3/А
 *                      1 - код обновлен, 0 - код не обновлялся
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackMode3Acode(TCatAsterix* cat_lib, BoardNumCode* num3a_code);

/**
 * @brief Запись значения идентификатора цели (позывной или регистрационный номер) в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param target_id - указатель на массив с идентификатором цели
 * @param tid_src -  признак источника идентификатора
 *                      0 - идентификатор (позывной или регистрационный номер)
 *                          получен непосредственно от объекта
 *                      1 - идентификатор (позывной) получен из другого источника
 *                      2 - идентификатор (регистрационный номер) получен из другого источника
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackTargetIdentification(TCatAsterix* cat_lib, char* target_id, u_int8_t tid_src);
/**
 * @brief Запись значения номера трассы в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param track_number - машинный номер трассы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackNumber(TCatAsterix* cat_lib, u_int32_t track_number);

/**
 * @brief Запись значения адреса трасссы (24 бит. номер ИКАО) в структуру категории
 * @param cat_lib           - указатель на структуру категории
 * @param target_address    - значения адреса трасссы (24 бит. номер ИКАО)
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackTargetAddress(TCatAsterix* cat_lib, u_int32_t target_address);

/**
 * @brief Запись значения измеренного эшелона полета трека в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param measured_fligth_level - измеренный эшелон полета
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackMeasuredFlightLevel(TCatAsterix* cat_lib, double measured_fligth_level);

/**
 * @brief Запись значения высоты полета полученного от борта ВО в структуру категории
 * @param cat_lib   - указатель на структуру категории
 * @param altitude  - значение высоты полета
 * @param mode_c    - структура данных высоты Mode C
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackModeCdata(TCatAsterix* cat_lib, BoardAltitudeCode mode_c);

/**
 * @brief Запись значения статуса трассы в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param status  - указатель на структуру статуса трассы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackStatus(TCatAsterix* cat_lib, TTrackStatus* status);
/**
 * @brief Запись значения времени устаревания системных данных о трассе в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param systime_age  - время устаревания системного времени
 *                       диапазон изменения 0.25 - 63.75 секунды
 *                       отсчитывается от последнего времени обновления соответствующих данных
 * @param time_type    - тип системного времени
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackSystemTimeAges(TCatAsterix* cat_lib, double systime_age, SystemTrackUpdateTimeType time_type);

/**
 * @brief Запись значения времени устаревания данных о трассе в структуру категории
 *        полученных с борта воздушного средства
 * @param cat_lib - указатель на структуру категории
 * @param track_data_age  - время устаревания системного времени
 *                          диапазон изменения 0.25 - 63.75 секунды
 *                          отсчитывается от последнего времени обновления соответствующих данных
 * @param time_type    - тип времени трека
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackDataAges(TCatAsterix* cat_lib, double track_data_age, TrackDataAgesType age_time_type);
/**
 * @brief Запись измеренной геометрической высоты полета трека в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param geometric_altitude - измеренная наземными средствами, источник указывается
 *                             в статусе трассы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackCalcGeometricAltitude(TCatAsterix* cat_lib, double geometric_altitude);
/**
 * @brief Запись измеренной барометрической высоты полета трека в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param barometric_altitude - измеренная борту воздушного судна
 * @param qnh_correction - признак относительной барометрической высоты по QNH
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackCalcBarometricAltitude(TCatAsterix* cat_lib, double barometric_altitude, u_int8_t qnh_correction);
/**
 * @brief Запись скорости изменения высоты полета трека в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param vertical_rate - измеренная борту воздушного судна
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackCalcVerticalRate(TCatAsterix* cat_lib, double vertical_rate);
/**
 * @brief Запись данных плановой информации полета в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param fp_data - указатель на структуру плановой данных
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackFlightPlanData(TCatAsterix* cat_lib, TFlightPlanData* fp_data);
/**
 * @brief Запись данных о размерах и положении в прострастве в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param length - длина транспортного средства
 * @param width - ширина транспортного средства
 * @param orientation - положение транспортного средства в прострастве
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackTargetSize(TCatAsterix* cat_lib, u_int32_t length, u_int32_t width, float orientation);
/**
 * @brief Запись значения номера RBS Mode 2 в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param mode2_code - код номера в режиме 2
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackMode2code(TCatAsterix* cat_lib, BoardNumCode m2_code);
/**
 * @brief Запись точностных данных местоположения трассы в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param accuracy - указатель на точностные данные
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackEstimateAccuracies(TCatAsterix* cat_lib, TTrackAccuracy* accuracy);
/**
 * @brief Запись дополнительных данных траектории трассы ВО в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param sp_data - указатель на структуру дополнительных данных (УВД/НРЗ)
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackDefaultSpData(TCatAsterix* cat_lib, TSpData* sp_data);
/**
 * @brief Запрос идентификатора источника из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 1:
 *         переменная val содержит идентификатор источника
 */
TReturnOneInt getDataSourceIdentificator(TCatAsterix* cat_lib);
/**
 * @brief Запрос типа сервиса из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 1:
 *         переменная val содержит тип сервиса
 */
TReturnOneInt getServiceIdentificator(TCatAsterix* cat_lib);
/**
 * @brief Запрос суточного времени сообщения UTC в секундах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 2:
 *         переменная val содержит суточное время сообщения UTC в секундах
 */
TReturnOneDouble getTimeOfTrackInformation(TCatAsterix* cat_lib);
/**
 * @brief Запрос данных положения ВО в географических координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение широты в радианах
 *         переменная val2 - содержит значение долготы в радианах
 */
TReturnPairDouble getTrackPositionWgs84(TCatAsterix* cat_lib);
/**
 * @brief Запрос данных положения ВО в прямоугольных координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 3:
 *         переменная val1 - содержит значение координаты Х, в метрах
 *         переменная val2 - содержит значение координаты У, в метрах
 */
TReturnPairInt getTrackPositionCartesian(TCatAsterix* cat_lib);
/**
 * @brief Запрос данных положения ВО в полярных координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение азимута, в радианах
 *         переменная val2 - содержит значение дальности, в метрах
 */
TReturnPairDouble getTrackPositionPolar(TCatAsterix* cat_lib);
/**
 * @brief Запрос скорости движения ВО в прямоугольных координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение скорости по оси Х, метр/секунда
 *         переменная val2 - содержит значение скорости по оси У, метр/секунда
 */
TReturnPairDouble getTrackVelocityCartesian(TCatAsterix* cat_lib);
/**
 * @brief Запрос скорости движения ВО в полярной системе координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение скорости, метр/секунда
 *         переменная val2 - содержит значение курса, радианы
 */
TReturnPairDouble getTrackVelocityPolar(TCatAsterix* cat_lib);
/**
 * @brief Запрос ускорения движения ВО в прямоугольных координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение ускорения по оси Х, метр/секунда^2
 *         переменная val2 - содержит значение ускорения по оси У, метр/секунда^2
 */
TReturnPairDouble getTrackAccelerationCartesian(TCatAsterix* cat_lib);
/**
 * @brief Запрос данных бортового номера ВО в режиме Mode 3/A из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных бортового номера, если номер борта равен 0
 *         данный элемент в сообщении не присутствовал
 */
BoardNumCode getTrackMode3Acode(TCatAsterix* cat_lib);
/**
 * @brief Запрос бортовой высоты полета полученного от борта ВО из категории
 * @param cat_lib   - указатель на структуру категории
 * @return структура бортовой высоты, при отсутствии данных все значения структуры
 *         обнулены
 */
BoardAltitudeCode getTrackModeCdata(TCatAsterix* cat_lib);
/**
 * @brief Запрос данных идентификатора ВО
 *        (позывной полученный вне данных
 *          плановой информации) категории
 * @param cat_lib - указатель на структуру категории
 * @param[out] tid_src - указатель на признак источника идентификатора
 * @return указатель на структура данных возврата
 */
TCommonObmenData* getTrackTargetIdentification(TCatAsterix* cat_lib, u_int8_t* tid_src);
/**
 * @brief Запрос номера трассы ВО в структуре категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *         переменная val содержит номер трассы ВО
 */
TReturnOneInt getTrackNumber(TCatAsterix* cat_lib);
/**
 * @brief Запрос значения адреса трасссы (24 бит. номер ИКАО) из категории
 * @param cat_lib           - указатель на структуру категории
 * @return структура данных возврата:
 *         переменная val содержит значение адреса трасссы (24 бит. номер ИКАО)
 */
TReturnOneInt getTrackTargetAddress(TCatAsterix* cat_lib);
/**
 * @brief Запрос времени устаревания данных о трассе в структуре категории
 *        полученных с борта воздушного средства
 * @param cat_lib - указатель на структуру категории
 * @param time_type    - тип времени трека
 * @return структура вовращения данных:
 *          переменная val содержит время устаревания данных
 *                          диапазон изменения 0.25 - 63.75 секунды
 *                          отсчитывается от последнего времени обновления соответствующих данных
 */
TReturnOneDouble getTrackDataAges(TCatAsterix* cat_lib, TrackDataAgesType age_time_type);
/**
 * @brief Запись значения времени устаревания системных данных о трассе в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param time_type    - тип системного времени
 * @return возвращаемая структура:
 *         переменная val соедержит время устаревания системных данных
 *                       диапазон изменения 0.25 - 63.75 секунды
 *                       отсчитывается от последнего времени обновления соответствующих данных
 */
TReturnOneDouble getTrackSystemTimeAges(TCatAsterix* cat_lib, SystemTrackUpdateTimeType time_type);
/**
 * @brief Запрос измеренного эшелона полета ВО из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение эшелона полета ВО, в метрах
 */
TReturnOneDouble getTrackMeasuredFlightLevel(TCatAsterix* cat_lib);
/**
 * @brief Запрос расчитанной геометрической высоты полета ВО из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение геометрической высоты полета ВО, в метрах
 */
TReturnOneDouble getTrackCalcGeometricAltitude(TCatAsterix* cat_lib);
/**
 * @brief Запрос расчитанной барометрической высоты полета ВО из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val1 - содержит значение барометрической высоты полета ВО, в метрах
 *          переменная val2 - содержит признак QNH коррекции барометрической высоты полета ВО:
 *                            0 - передается абсолютная высота 760 мм. отн. моря
 *                            1 - передается относительная (скорректированная) высота
 */
TReturnPairInt getTrackCalcBarometricAltitude(TCatAsterix* cat_lib);
/**
 * @brief Запрос расчитанной вертикальной (по высоте) скорости полета ВО из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение вертикальной скорости полета ВО, метр/секуда
 */
TReturnOneDouble getTrackCalcVerticalRate(TCatAsterix* cat_lib);
/**
 * @brief Запрос данных плановой информации полета из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - указатель на структуру плановых данных
 */
TFlightPlanData* getTrackFlightPlanData(TCatAsterix* cat_lib);
/**
 * @brief Запрос размера объекта (длины) из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение длины транспортного средства (ВО), в метрах
 */
TReturnOneInt getTrackTargetSizeLenght(TCatAsterix* cat_lib);
/**
 * @brief Запрос размера объекта (ширина) из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение ширины транспортного средства (ВО), в метрах
 */
TReturnOneInt getTrackTargetSizeWidth(TCatAsterix* cat_lib);
/**
 * @brief Запрос размера объекта (ширина) из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение ширины транспортного средства (ВО), в метрах
 */
TReturnOneDouble getTrackTargetSizeOrientation(TCatAsterix* cat_lib);
/**
 * @brief Запрос данных бортового номера ВО в режиме Mode 2 из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *         переменная val - содержит значение бортового номера Mode 2
 */
BoardNumCode getTrackMode2code(TCatAsterix* cat_lib);
/**
 * @brief Запрос точностных данных трассовой информации из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - указатель на структуру точностных данных
 */
TTrackAccuracy* getTrackEstimateAccuracies(TCatAsterix* cat_lib);
/**
 * @brief Запрос результатов распаковки поля SP из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - указатель на структуру данных результатов распаковки поля SP
 *            (распаковка данных по умолчанию)
 */
TSpData* getTrackDefaultSpData(TCatAsterix* cat_lib);
/**
 * @brief Запрос содержимого поля SP из из структуры категории
 *        (нераспакованные данные поля SP, полученные в сообщении категории)
 * @param cat_lib - указатель на структуру категории
 * @return указатель на структуру данных возврата
 */
TCommonObmenData* getTrackSpRawData(TCatAsterix* cat_lib);
/**
 * @brief Запись дополнительных данных траектории трассы ВО в структуру категории
 *        (данные закодированы пользователем и переданы в виде массива байт)
 * @param cat_lib - указатель на структуру категории
 * @param sp_data - указатель на массив байт с кодированными данными
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackSpRawData(TCatAsterix* cat_lib, TCommonObmenData* sp_raw);
/**
 * @brief Запрос статуса трассы ВО в структуре категории
 * @param cat_lib - указатель на структуру категории
 * @return указатель на структуру данных статуса трассы ВО
 *         переменная val содержит номер трассы ВО
 */
TTrackStatus* getTrackStatus(TCatAsterix* cat_lib);
/**
 * @brief Запись идентификатора транспортного средства в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param vehicle_fleet_id - идентификатор транспортного стрества
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackVehicleFleetIdentification(TCatAsterix* cat_lib, u_int8_t vehicle_fleet_id);
/**
 * @brief Запрос идентификатора транспортного средства из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - структура данных возврата:
 *         переменная val - содержит значение идентификатора транспортного средства
 */
TReturnOneInt getTrackVehicleFleetIdentification(TCatAsterix* cat_lib);
/**
 * @brief Запрос названия элемента из контейнера категории
 * @param cat - указатель на структуру категории Астерикс
 * @param ed_index - индекс элемента категории в соответствии с профилем пользователя
 * @return - указатель на структуру с названием элемента
 */
TCommonObmenData* getCatElementName(TCatAsterix* cat, u_int16_t ed_index);
/**
 * @brief Запрос кодированного содержимого элемента из контейнера категории
 * @param cat - указатель на структуру категории Астерикс
 * @param ed_index - индекс элемента категории в соответствии с профилем пользователя
 * @return - указатель на структуру с кодированными данными элемента
 */
TCommonObmenData* getCatElementRawData(TCatAsterix* cat,  u_int16_t ed_index);
/**
 * @brief Упаковка данных о маневрировании объекта в структуру категории Астерикс
 * @param cat_lib - указатель на контейнер Астерикс
 * @param horizontal - вид маневра в горизонтальной плоскости
 * @param vertical   - вид маневра в вертикальной плоскости
 * @param speed      - вид маневра по скорости
 * @param alt_disc   - признак наличия расхождения данных по высоте измеренной и полученной с борта ВО
 *                     0 - нет расхождения
 *                     1 - есть расхождение
 * @return
 */
int setTrackModeMovement(TCatAsterix* cat_lib, TargModeMov horizontal, TargModeMov vertical, TargModeMov speed, u_int8_t alt_disc);

/**
 * Выборка данных о маневрировании объекта
 * в горизонтальной плоскости, из контейнера категории
 * Вид маневрирования в горизонтальной плоскости
 */
int getTrackModeMovementHorizontal(TCatAsterix* cat_lib);

/**
 * @brief Выборка данных о маневрировании объекта
 *          в вертикальной плоскости, из контейнера категории
 * @return Вид маневрирования в вертикальной плоскости
 */

int getTrackModeMovementVertical(TCatAsterix* cat_lib);
/**
 * Выборка данных о маневрировании объекта
 * по скорости, из контейнера категории
 * return Вид маневрирования по скорости
 */
int getTrackModeMovementBySpeed(TCatAsterix* cat_lib);

#ifdef __cplusplus
}
#endif
#endif
