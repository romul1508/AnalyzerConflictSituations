#ifndef ASTERIX_STRUCT_HPP
#define ASTERIX_STRUCT_HPP

namespace Asterix
{

// Макрос для поддержки перевода полей
#define UAP_NAME(x) x

struct TWideCallsign
{
	static const int CALLSIGN_SIZE = 8;
	wchar_t          callsign[CALLSIGN_SIZE];
};

struct TAirport
{
	static const int AIRPORT_SIZE = 5;
	wchar_t          airport[AIRPORT_SIZE];
};

struct TAirSpeed
{
	bool   mach;
	double speed;
};

struct TAircraftType
{
	static const int TYPE_SIZE = 5;
	wchar_t          type[TYPE_SIZE];
};

struct FlightPlanId
{
	uint8_t  typ;
	uint32_t nbr;
};

enum GAT
{
	GT_UNK            = 0,
	GT_GENERAL        = 1,
	GT_OPERATIONAL    = 2,
	GT_NOT_APPLICABLE = 3,
};

enum CommAcasFlightStatus
{
	CA_NO_ALERT_NO_SPI_AIR    = 0,
	CA_NO_ALERT_NO_SPI_GROUND = 1,
	CA_ALERT_NO_SPI_AIR       = 2,
	CA_ALERT_NO_SPI_GROUND    = 3,
	CA_ALERT_SPI              = 4,
	CA_NO_ALERT_SPI           = 5,

	CA_C20_NOT_ASSIGNED  = 6,
	CA_C20_NOT_EXTRACTED = 7,

	CA_C11_GENERAL_EMERGENCY  = 6,
	CA_C11_MEDICAL_EMERGENCY  = 7,
	CA_C11_MINIMUM_FUEL       = 8,
	CA_C11_NO_COMMUNICATIONS  = 9,
	CA_C11_UNLAWFUL_INFERENCE = 10,
};

struct CommAcas
{
	ComCapability        com;
	CommAcasFlightStatus status;
	bool                 mssc;
	bool                 arc;
	bool                 aic;

	bool    b1a : 1;
	uint8_t b1b : 4;
};

enum IFR
{
	FR_IFR  = 0,
	FR_VFR  = 1,
	FR_NA   = 2,
	FR_CVFR = 3,
};

enum RVSM
{
	RVSM_UNK          = 0,
	RVSM_APPROVED     = 1,
	RVSM_EXEMPT       = 2,
	RVSM_NOT_APPROVED = 3,
};

enum CDM
{
	CDM_MAINTAINING = 0,
	CDM_CLIMBING    = 1,
	CDM_DECENDING   = 2,
	CDM_INVALID     = 3
};

struct TFlightPlanId
{
	uint8_t  typ;
	uint32_t nbr;
};

struct TFlightLevel
{
	CodeFlags flags;
	double    level_fl;
};

struct TControlPosition
{
	uint8_t centre;
	uint8_t position;
};

struct TModeSArray
{
	std::vector<ModeS> data;
};

struct HighPrecitionTime
{
	enum FullSecondIndication
	{
		WS          = 0,                           ///< TOMRv whole seconds = Whole seconds
		WS_P1       = 1,                           ///< TOMRv whole seconds = Whole seconds + 1
		WS_M1       = 2,                           ///< TOMRv whole seconds = Whole seconds – 1
		WS_RESERVED = 3,
	};
	FullSecondIndication fsi;
	double               time_fract;
};

struct TMode1
{
	CodeFlags flags;
	uint16_t  code;
};

struct TMode2
{
	CodeFlags flags;
	uint16_t  code;
};

struct TrajectoryIntent
{
	bool      nc;
	uint8_t   tcp;
	double    altitude;
	Position  pos;
	int type;		// PointType
	int  turn;		// TurnType
	double    tov;
	double    ttr;
};

struct Trajectory
{
	std::vector<TrajectoryIntent> data;
};

enum PointType
{
	PT_UNK         = 0,
	PT_FLY_BY      = 1,
	PT_FlY_OVER    = 2,
	PT_HOLD        = 3,
	PT_PROC_HOLD   = 4,
	PT_PROC_TURN   = 5,
	PT_RF_LEG      = 6,
	PT_TOP_CLIMB   = 7,
	PT_TOP_DESCENT = 8,
	PT_LEVEL       = 9,
	PT_Cross_alt   = 10,
	PT_Trans_alt   = 11,
};

enum TurnType
{
	TT_NA      = 0,
	TT_RIGHT   = 1,
	TT_LEFT    = 2,
	TT_NO_TURN = 3,
};

enum SelAltSource
{
	ASA_UNKNOWN       = 0,                             ///< Не определен
	ASA_FROM_AIRCRAFT = 1,                         ///< Высота измерена БРЭО самолета
	ASA_FROM_FCU_MCP  = 2,                         ///< Высота по данным модуля контроля полета самолета
	ASA_FROM_FMS      = 3                              ///< Высота от системы управления полетом самолета
};

struct TSelAltitude
{
	SelAltSource source : 2;
	double       altitude;
};

struct TFinalSelAltitude
{
	bool   mv : 1;
	bool   ah : 1;
	bool   am : 1;
	double altitude;
};

enum STI
{
	STI_FROM_TRANSPONDER                  = 0,         ///< Идентификатор или позывной получен от бортового передатчика
	STI_CALLSIGN_NOT_FROM_TRANSPONDER     = 1,         ///< Позывной получен не от бортового передатчика
	STI_REGISTRATION_NOT_FROM_TRANSPONDER = 2,         ///< Идентификатор получен не от бортового передатчика
	STI_UNDEFINED                         = 3,

	// Для категории 20
	STI_NOT_FROM_TRANSPONDER          = 0,             ///< Идентификатор или позывной получен не от бортового передатчика
	STI_REGISTRATION_FROM_TRANSPONDER = 1,             ///< Идентификатор получен от бортового передатчика
	STI_CALLSIGN_FROM_TRANSPONDER     = 2,             ///< Позывной получен от бортового передатчика
};

struct TTurnRate
{
	TurnType ti;
	double   rate;
};

struct TMetInfo
{
	double wind;
	double wind_dir;
	double temperature;
	double turbulence;
};

}

#endif /* ASTERIX_STRUCT_HPP */
