#ifndef ASTERIX_CAT_4_H
#define ASTERIX_CAT_4_H

#include "asterix.h"
#include "asterix_internal.h"

namespace Cat4
{
using Asterix::DataSourceId;
using Asterix::TWideCallsign;
using Asterix::Mode3A;
using Asterix::Position;
using Asterix::GAT;
using Asterix::IFR;
using Asterix::RVSM;
using Asterix::CDM;
using Asterix::Callsign;
using Asterix::TFlightPlanId;
using Asterix::TFlightLevel;
using Asterix::TControlPosition;
using Asterix::PositionCartesian;
using Asterix::Velocity;

// RAD Route Adherence Monitor
// MSA Minimum Safe Altitude
// CLAM Clearance Level Adherence Monitor
// STCA Short Term Conflict Alert
// AFDA Approach Funnel Deviation Alert
// ALM Arrival / Landing Monitor
// WRA Arrival / Departure Wrong Runway Alert
// OTA Arrival / Departure Opposite Traffic Alert
// RDM Departure Monitor
// RCM Runway / Taxiway Crossing Monitor
// TSM Taxiway Separation Monitor
// UTMM Unauthorized Taxiway Movement Monitor
// SBOA Stop Bar Overrun Alert
// EOC End Of Conflict
// ACASRA ACAS Resolution Advisory
// NTCA Near Term Conflict Alert

enum MsgType
{
	MT_ALIVE           = 1,
	MT_RAD_LONGIT_DEV  = 2,
	MT_RAD_HEADING_DEV = 3,
	MT_MSA_WARN        = 4,
	MT_AREA_PROX_WARN  = 5,
	MT_CLAM            = 6,
	MT_STCA            = 7,
	MT_AFDA            = 8,
	MT_RIMCAS_ALM      = 9,
	MT_RIMCAS_WRA      = 10,
	MT_RIMCAS_OTA      = 11,
	MT_RIMCAS_RDM      = 12,
	MT_RIMCAS_RCM      = 13,
	MT_RIMCAS_TSM      = 14,
	MT_RIMCAS_UTMM     = 15,
	MT_RIMCAS_SBOA     = 16,
	MT_EOC             = 17,
	MT_ACASRA          = 18,
	MT_NTCA            = 19,
};

struct AlertStatus
{
	uint8_t stat : 3;
};

struct SafetyNetStatus
{
	bool mrva  : 1;
	bool ramld : 1;
	bool ramhd : 1;
	bool msaw  : 1;
	bool apw   : 1;
	bool clam  : 1;
	bool stca  : 1;

	bool afda   : 1;
	bool rimca  : 1;
	bool acasra : 1;
	bool ntca   : 1;
	bool dg     : 1;
	bool of     : 1;
	bool ol     : 1;
};

struct ConfilictPosition
{
	double wgs84_lon;
	double wgs84_lat;
	double altitude;
};

struct ConflictPositionCartesian
{
	double x;
	double y;
	double altitude;
};


struct AircraftData :
	AsterixRecord
{
	struct Characteristics
	{
		GAT  gat;
		IFR  ifr;
		RVSM rvsm;
		bool hpr;

		CDM  cdm;
		bool pri;
		bool gv;
	};

	struct Uap { };

	static const int RE_FRN   = 0;
	static const int RE_END   = 0;
	static const int LAST_FRN = 10;
	static const int UAP_SIZE = LAST_FRN + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(TWideCallsign,              AircraftData,  1, _callsign);
	ASTERIX_DECLARE(Mode3A,                    AircraftData,  2, _mode_3a);
	ASTERIX_DECLARE(ConfilictPosition,         AircraftData,  3, _predicted_pos);
	ASTERIX_DECLARE(ConflictPositionCartesian, AircraftData,  4, _predicted_cart_pos);
	ASTERIX_DECLARE(double,                    AircraftData,  5, _time_to_thres);
	ASTERIX_DECLARE(double,                    AircraftData,  6, _dist_to_thres);
	ASTERIX_DECLARE(Characteristics,           AircraftData,  7, _char);
	ASTERIX_DECLARE(Callsign,                  AircraftData,  8, _mode_s_callsign);
	ASTERIX_DECLARE(TFlightPlanId,              AircraftData,  9, _flight_id);
	ASTERIX_DECLARE(TFlightLevel,               AircraftData, 10, _cleared_flight_level);

	AircraftData() : AsterixRecord(0)
		, _callsign             ()
		, _mode_3a              ()
		, _predicted_pos        ()
		, _predicted_cart_pos   ()
		, _time_to_thres        ()
		, _dist_to_thres        ()
		, _char                 ()
		, _mode_s_callsign      ()
		, _flight_id            ()
		, _cleared_flight_level ()
	{}
};

struct Conflict :
	AsterixRecord
{
	struct Nature
	{
		bool mas   : 1;
		bool cas   : 1;
		bool fld   : 1;
		bool type  : 1;
		bool cross : 1;
		bool div   : 1;

		bool rrc  : 1;
		bool rtc  : 1;
		bool mrva : 1;
	};

	enum
	{
		SEV_MAJ_NC_ND = 0,
		SEV_MIN_NC_ND = 1,
		SEV_MAJ_C_D   = 2,
		SEV_MIN_C_D   = 3,
	};

	enum
	{
		TBL_STCA_0 = 0,
		TBL_STCA_1 = 1,
		TBL_RIMCAS = 2,
	};

	struct Classification
	{
		uint8_t table_id : 4;
		union
		{
			uint8_t severety_class : 3;
			struct
			{
				bool lpf : 1;
				bool cpf : 1;
				bool mhf : 1;
			};
			struct
			{
				bool ras : 1;
			};
		};
		bool cs : 1;
	};

	struct Uap { };

	static const int RE_FRN   = 0;
	static const int RE_END   = 0;
	static const int LAST_FRN = 4;
	static const int UAP_SIZE = LAST_FRN + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(Nature,         Conflict, 1, _nature);         ///< Nature
	ASTERIX_DECLARE(Classification, Conflict, 2, _classification); ///< Conflict Classification
	ASTERIX_DECLARE(double,         Conflict, 3, _probability);    ///< Conflict Probabilty
	ASTERIX_DECLARE(double,         Conflict, 4, _duration);       ///< Conflict Duration

	Conflict() : AsterixRecord(0)
		, _nature         ()
		, _classification ()
		, _probability    ()
		, _duration       ()
	{}
};

struct ConflictTiming :
	AsterixRecord
{
	struct Uap { };

	static const int RE_FRN   = 0;
	static const int RE_END   = 0;
	static const int LAST_FRN = 6;
	static const int UAP_SIZE = LAST_FRN + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(double, ConflictTiming, 1, _time_to_conflict);         ///< Time to Conflict
	ASTERIX_DECLARE(double, ConflictTiming, 2, _time_to_closest_approach); ///< Time to Closest Approach
	ASTERIX_DECLARE(double, ConflictTiming, 3, _current_horiz_sep);        ///< Current Horizontal Separation
	ASTERIX_DECLARE(double, ConflictTiming, 4, _estimated_min_horiz_sep);  ///< Estimated Minimum Horizontal Separation
	ASTERIX_DECLARE(double, ConflictTiming, 5, _current_vert_sep);         ///< Current Vertical Separation
	ASTERIX_DECLARE(double, ConflictTiming, 6, _estimated_min_vert_sep);   ///< Estimated Minimum Vertical Separation

	ConflictTiming() : AsterixRecord(0)
		, _time_to_conflict         ()
		, _time_to_closest_approach ()
		, _current_horiz_sep        ()
		, _estimated_min_horiz_sep  ()
		, _current_vert_sep         ()
		, _estimated_min_vert_sep   ()
	{}
};

struct AreaDefinition :
	AsterixRecord
{

	struct Uap { };

	static const int RE_FRN   = 0;
	static const int RE_END   = 0;
	static const int LAST_FRN = 6;
	static const int UAP_SIZE = LAST_FRN + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(Callsign,     AreaDefinition, 1, _area_name);              ///< Area Name
	ASTERIX_DECLARE(TWideCallsign, AreaDefinition, 2, _crossing_area_name); ///< Crossing Area Name
	ASTERIX_DECLARE(TWideCallsign, AreaDefinition, 3, _rw_taxi_1);          ///< Runway/Taxiway Designator 1
	ASTERIX_DECLARE(TWideCallsign, AreaDefinition, 4, _rw_taxi_2);          ///< Runway/Taxiway Designator 2
	ASTERIX_DECLARE(TWideCallsign, AreaDefinition, 5, _stopbar);             ///< Stop Bar Designator
	ASTERIX_DECLARE(TWideCallsign, AreaDefinition, 6, _gate);                 ///< Gate Designator

	AreaDefinition() : AsterixRecord(0)
		, _area_name          ()
		, _crossing_area_name ()
		, _rw_taxi_1          ()
		, _rw_taxi_2          ()
		, _stopbar            ()
		, _gate               ()
	{}
};

struct TargetInfo :
	AsterixRecord
{
	struct Uap { };

	static const int RE_FRN   = 0;
	static const int RE_END   = 0;
	static const int LAST_FRN = 5;
	static const int UAP_SIZE = LAST_FRN + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(Position,          TargetInfo, 1, _position); ///< Current Position
	ASTERIX_DECLARE(PositionCartesian, TargetInfo, 2, _cart_pos); ///< Current Position Cartesian
	ASTERIX_DECLARE(TFlightLevel,       TargetInfo, 3, _mode_c);   ///< Current Mode C
	ASTERIX_DECLARE(Velocity,          TargetInfo, 4, _velocity); ///< Current Track Velocity
	TargetInfo() : AsterixRecord(0)
		, _position ()
		, _cart_pos ()
		, _mode_c   ()
		, _velocity ()
	{}
};

/// Категория 4
struct Cat4 :
	public AsterixRecord
{
	struct RW;

	/// Структура-описание полей asterix
	struct Uap
	{
		Asterix::UapInfo info;
		struct Serializer;
		void (Serializer::* serialize)(Cat4&);                      ///< Функция-обработчик
	};

	static const uint16_t VERSION  = 0x0108;
	static const int      RE_FRN   = 20;
	static const int      RE_END   = 2;
	static const int      LAST_FRN = 22;
	static const int      UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap      UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId, Cat4, 1, _dsi);                       ///< Data Source Identifier
	ASTERIX_DECLARE(uint8_t,      Cat4, 2, _message_type);              ///< Message Type
	ASTERIX_DECLARE_ARRAY(DataSourceId, Cat4, 3, _sdps);                ///< SDPS Identifier
	ASTERIX_DECLARE(double,          Cat4,  4, _time);                  ///< Time of Message
	ASTERIX_DECLARE(uint16_t,        Cat4,  5, _alert_id);              ///< Alert Identifier
	ASTERIX_DECLARE(AlertStatus,     Cat4,  6, _alert_status);          ///< Alert Status
	ASTERIX_DECLARE(SafetyNetStatus, Cat4,  7, _safety_status);         ///< Safety Net Function & System Status
	ASTERIX_DECLARE(uint16_t,        Cat4,  8, _track_number_1);        ///< Track Number 1
	ASTERIX_DECLARE(AircraftData,    Cat4,  9, _aircraft_1);            ///< Aircraft Identification & Characteristics 1
	ASTERIX_DECLARE(Conflict,        Cat4, 10, _conflict);              ///< Conflict Characteristics
	ASTERIX_DECLARE(ConflictTiming,  Cat4, 11, _conflict_timing);       ///< Conflict Timing and Separation
	ASTERIX_DECLARE(double,          Cat4, 12, _vertical_deviation);    ///< Vertical Deviation
	ASTERIX_DECLARE(double,          Cat4, 13, _longit_deviation);      ///< Longitudinal Deviation
	ASTERIX_DECLARE(double,          Cat4, 14, _transversal_deviation); ///< Transversal Distance Deviation
	ASTERIX_DECLARE(AreaDefinition,  Cat4, 15, _area);                  ///< Area Definition
	ASTERIX_DECLARE(uint16_t,        Cat4, 16, _track_number_2);        ///< Track Number 2
	ASTERIX_DECLARE(AircraftData,    Cat4, 17, _aircraft_2);            ///< Aircraft Identification & Characteristics 2
	ASTERIX_DECLARE_ARRAY(TControlPosition, Cat4, 18, _control);         ///< FDPS Sector Control Identifier

	ASTERIX_DECLARE(uint8_t, Cat4, 20, _re);                            ///< Reserved Expansion Field
	ASTERIX_DECLARE_ARRAY(uint8_t, Cat4, 21, _sp);                      ///< Special Purpose Field
	ASTERIX_DECLARE(TargetInfo, Cat4, LAST_FRN + 0,      _target_1);    ///< Information Target 1
	ASTERIX_DECLARE(TargetInfo, Cat4, LAST_FRN + 1,      _target_2);    ///< Information Target 2
	ASTERIX_DECLARE(uint8_t,    Cat4, LAST_FRN + RE_END, _re_end);

public:

	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size, TAst_explain*);
	static char*          code(const AsterixRecord* record, size_t& write_size);
	Cat4() : AsterixRecord(4)
		, _dsi                   ()
		, _message_type          ()
		, _sdps                  ()
		, _time                  ()
		, _alert_id              ()
		, _alert_status          ()
		, _safety_status         ()
		, _track_number_1        ()
		, _aircraft_1            ()
		, _conflict              ()
		, _conflict_timing       ()
		, _vertical_deviation    ()
		, _longit_deviation      ()
		, _transversal_deviation ()
		, _area                  ()
		, _track_number_2        ()
		, _aircraft_2            ()
		, _control               ()
		, _re                    ()
		, _sp                    ()
		, _target_1              ()
		, _target_2              ()
		, _re_end                ()
	{};
};

struct Cat4::Uap::Serializer
{
	virtual void dataSourceId(Cat4& r)   = 0;
	virtual void messageType(Cat4& r)    = 0;
	virtual void sdps(Cat4& r)           = 0;
	virtual void timeOfMessage(Cat4& r)  = 0;
	virtual void alertId(Cat4& r)        = 0;
	virtual void alertStatus(Cat4& r)    = 0;
	virtual void safetyStatus(Cat4& r)   = 0;
	virtual void trackNumber1(Cat4& r)   = 0;
	virtual void aircraft1(Cat4& r)      = 0;
	virtual void conflict(Cat4& r)       = 0;
	virtual void conflictTiming(Cat4& r) = 0;
	virtual void vertDev(Cat4& r)        = 0;
	virtual void longitDev(Cat4& r)      = 0;
	virtual void transvDev(Cat4& r)      = 0;
	virtual void area(Cat4& r)           = 0;
	virtual void trackNumber2(Cat4& r)   = 0;
	virtual void aircraft2(Cat4& r)      = 0;
	virtual void control(Cat4& r)        = 0;
	virtual void target1(Cat4& r)        = 0;
	virtual void target2(Cat4& r)        = 0;

	virtual void reserved(Cat4& r)     = 0;
	virtual void reserved_end(Cat4& r) = 0;
	virtual void special(Cat4& r)      = 0;

	template <size_t n>
	void skip(Cat4& r);
	virtual ~Serializer() {};
};

}  // namespace Cat23

#endif /* ASTERIX_CAT_4_H */
