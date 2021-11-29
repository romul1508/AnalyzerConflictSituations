#ifndef ASTERIX_CAT_21_HPP
#define ASTERIX_CAT_21_HPP

#include "asterix.hpp"
#include "asterix_internal.hpp"
#include "common_lib_asterix.h"
#include "cat_062_asterix.h"

namespace Cat21
{
using Asterix::Position;
using Asterix::DataSourceId;
using Asterix::ReportSource;
using Asterix::Callsign;
using Asterix::Mode3A;

using Asterix::ModeS;
using Asterix::GroundVector;
using Asterix::AcasRA;
typedef EmitterCategoryType EmitterCategory;

struct AirSpeed
{
	bool   mach;
	double speed;
};

struct QualityIndicators
{
	uint8_t nac_v : 3;
	uint8_t nic   : 4;

	bool    nic_baro : 1;
	bool    sil      : 2;
	uint8_t nac_p    : 4;

	bool    sils : 1;
	uint8_t sda  : 2;
	uint8_t gva  : 2;

	uint8_t pic : 4;
};

struct ExceedableSpeed
{
	bool   range_exceeded;
	double speed;
};

typedef ExceedableSpeed TrueAirSpeed;
typedef ExceedableSpeed BaroVertRate;
typedef ExceedableSpeed GeoVertRate;


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

enum AddressType
{
	AT_ICAO            = 0,                            ///< 24-Bit ICAO address
	AT_DUPLICATE       = 1,                            ///< Duplicate address
	AT_SURFACE_VEHICLE = 2,                            ///< Surface vehicle address
	AT_ANON            = 3,                            ///< Anonymous address
	AT_RESERVED_1      = 4,
	AT_RESERVED_2      = 5,
	AT_RESERVED_3      = 6,
	AT_RESERVED_4      = 7,
};

enum AltReportCap
{
	ARC_25_FT   = 0,
	ARC_100_FT  = 1,
	ARC_UNKNOWN = 2,
	ARC_INVALID = 3,
};

struct TargetReportDescriptor
{
	enum Confidence
	{
		VALID    = 0,
		SUSPECT  = 1,
		NO_INFO  = 2,
		RESERVED = 3,
	};

	AddressType  atp : 3;
	AltReportCap arc : 2;
	bool         rc  : 1;
	ReportSource rab : 1;

	bool    dcr : 1;
	bool    gbs : 1;
	bool    sim : 1;
	bool    tst : 1;
	bool    saa : 1;
	uint8_t cl  : 2;

	bool llc_fail : 1;
	bool ipc_fail : 1;
	bool nogo     : 1;
	bool cpr_fail : 1;
	bool ldpj     : 1;
	bool rc_fail  : 1;

};

struct Mops
{
	bool    unsupported;
	uint8_t version : 3;
	enum Technology
	{
		MOPS_OTHER     = 0,
		MOPS_UAT       = 1,
		MOPS_1090ES    = 2,
		MOPS_VDL4      = 3,
		MOPS_RESERVED1 = 4,
		MOPS_RESERVED2 = 5,
		MOPS_RESERVED3 = 6,
		MOPS_RESERVED4 = 7,
	};
	Technology link : 3;
};

struct TargetStatus
{
	bool            icf       : 1;
	bool            lnav      : 1;
	bool            me        : 1;
	EmergencyStatus emergency : 3;
	enum Status
	{
		TS_NORMAL    = 0,
		TS_ALERT     = 1,
		TS_TMP_ALERT = 2,
		TS_SPI       = 3,
	};
	Status status : 2;
};

struct MetInfo
{
	double  wind;
	double  wind_dir;
	double  temperature;
	uint8_t turbulence;
};

struct SelAltitude
{
	bool                     have_source;
	AircraftSelectedAltitude source : 2;
	double                   altitude;
};

struct FinalSelAltitude
{
	bool   mv : 1;
	bool   ah : 1;
	bool   am : 1;
	double altitude;
};

struct AircraftOperationalStatus
{
	bool    ra       : 1;
	uint8_t tc       : 2;
	bool    ts       : 1;
	bool    arv      : 1;
	bool    cdtia    : 1;
	bool    not_tcas : 1;
	bool    sa       : 1;
};

struct SurfaceCap
{
	bool    poa   : 1;
	bool    cdtis : 1;
	bool    b2low : 1;
	bool    ras   : 1;
	bool    ident : 1;
	uint8_t lw    : 4;
};

struct TrajectoryIntent
{
	bool                              available;
	bool                              valid;
	std::vector<TrajectoryIntentData> tr;
};

struct Ages
{
	double aos;
	double trd;
	double m3a;
	double qi;
	double ti;
	double mam;
	double gh;
	double fl;
	double isa;
	double fsa;
	double as;
	double tas;
	double mh;
	double bvr;
	double gvr;
	double gv;
	double tar;
	double tid;
	double ts;
	double met;
	double roa;
	double ara;
	double scc;
};

struct SelHeading
{
	bool   magnetic : 1;
	double heading;
};

struct NavMode
{
	bool autopilot : 1;
	bool vnav      : 1;
	bool alt_hold  : 1;
	bool approach  : 1;
};

struct SurfGroundVector
{
	bool   ground_track : 1;
	bool   magnetic     : 1;
	double ground_speed;
	double heading;
};

struct AircraftStatus
{
	bool es  : 1;
	bool uat : 1;
};

struct Military
{
	// Не реализовано
};

/// Категория 21
struct Cat21 :
	public AsterixRecord
{
	struct Reader;
	struct Writer;

	/// Структура-описание полей asterix
	struct Uap
	{
		Asterix::UapInfo info;
		struct Serializer;
		void (Serializer::* serialize)(Cat21&);                                   ///< Функция-обработчик
	};

	static const int RE_FRN   = 48;
	static const int RE_END   = 8;
	static const int LAST_FRN = 50;
	static const int UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId,              Cat21,  1, _dsi);                      ///< Data Source Identification
	ASTERIX_DECLARE(TargetReportDescriptor,    Cat21,  2, _target_report_dscr);       ///< Target Report Descriptor
	ASTERIX_DECLARE(uint16_t,                  Cat21,  3, _track_number);             ///< Track Number
	ASTERIX_DECLARE(uint8_t,                   Cat21,  4, _service);                  ///< Service Identification
	ASTERIX_DECLARE(double,                    Cat21,  5, _time_applicability_pos);   ///< Time of Applicability for Position
	ASTERIX_DECLARE(Position,                  Cat21,  6, _position);                 ///< Position in WGS-84 Coordinates
	ASTERIX_DECLARE(Position,                  Cat21,  7, _position_high);            ///< Position in WGS-84 Coordinates High Precision
	ASTERIX_DECLARE(double,                    Cat21,  8, _time_applicability_vel);   ///< Time of Applicability for Velocity
	ASTERIX_DECLARE(AirSpeed,                  Cat21,  9, _air_speed);                ///< Air Speed
	ASTERIX_DECLARE(TrueAirSpeed,              Cat21, 10, _true_air_speed);           ///< True Air Speed
	ASTERIX_DECLARE(uint32_t,                  Cat21, 11, _target_address);           ///< Target Address
	ASTERIX_DECLARE(double,                    Cat21, 12, _pos_time);                 ///< Time of Message Reception of Position
	ASTERIX_DECLARE(HighPrecitionTime,         Cat21, 13, _pos_time_high);            ///< Time of Message Reception of Position-High Precision
	ASTERIX_DECLARE(double,                    Cat21, 14, _vel_time);                 ///< Time of Message Reception of Velocity
	ASTERIX_DECLARE(HighPrecitionTime,         Cat21, 15, _vel_time_high);            ///< Time of Message Reception of Velocity-High Precision
	ASTERIX_DECLARE(double,                    Cat21, 16, _geometric_altitude);       ///< Geometric Height
	ASTERIX_DECLARE(QualityIndicators,         Cat21, 17, _quality);                  ///< Quality Indicators
	ASTERIX_DECLARE(Mops,                      Cat21, 18, _mops);                     ///< MOPS Version
	ASTERIX_DECLARE(Mode3A,                    Cat21, 19, _mode_3a);                  ///< Mode 3/A Code
	ASTERIX_DECLARE(double,                    Cat21, 20, _roll_angle);               ///< Roll Angle
	ASTERIX_DECLARE(double,                    Cat21, 21, _flight_level);             ///< Flight Level
	ASTERIX_DECLARE(double,                    Cat21, 22, _mag_heading);              ///< Magnetic Heading
	ASTERIX_DECLARE(TargetStatus,              Cat21, 23, _target_status);            ///< Target Status
	ASTERIX_DECLARE(BaroVertRate,              Cat21, 24, _baro_vert_speed);          ///< Barometric Vertical Rate
	ASTERIX_DECLARE(GeoVertRate,               Cat21, 25, _geo_vert_speed);           ///< Geometric Vertical Rate
	ASTERIX_DECLARE(GroundVector,              Cat21, 26, _ground_vector);            ///< Airborne Ground Vector
	ASTERIX_DECLARE(double,                    Cat21, 27, _turn_rate);                ///< Track Angle Rate
	ASTERIX_DECLARE(double,                    Cat21, 28, _report_time);              ///< Time of Report Transmission
	ASTERIX_DECLARE(Callsign,                  Cat21, 29, _callsign);                 ///< Target Identification
	ASTERIX_DECLARE(uint8_t,                   Cat21, 30, _emitter_category);         ///< Emitter Category
	ASTERIX_DECLARE(MetInfo,                   Cat21, 31, _met_info);                 ///< Met Information
	ASTERIX_DECLARE(SelAltitude,               Cat21, 32, _sel_altitude);             ///< Selected Altitude
	ASTERIX_DECLARE(FinalSelAltitude,          Cat21, 33, _final_sel_altitude);       ///< Final State Selected Altitude
	ASTERIX_DECLARE(TrajectoryIntent,          Cat21, 34, _trajectory);               ///< Trajectory Intent
	ASTERIX_DECLARE(double,                    Cat21, 35, _service_period);           ///< Service Management
	ASTERIX_DECLARE(AircraftOperationalStatus, Cat21, 36, _operational_status);       ///< Aircraft Operational Status
	ASTERIX_DECLARE(SurfaceCap,                Cat21, 37, _surf_cap);                 ///< Surface Capabilities and Characteristics
	ASTERIX_DECLARE(uint8_t,                   Cat21, 38, _amplitude);                ///< Message Amplitude

	ASTERIX_DECLARE_ARRAY(ModeS, Cat21, 39, _mode_s);                                 ///< Mode S MB Data

	ASTERIX_DECLARE(AcasRA,  Cat21, 40, _acas);                                       ///< ACAS Resolution Advisory Report
	ASTERIX_DECLARE(uint8_t, Cat21, 41, _recv_id);                                    ///< Receiver ID
	ASTERIX_DECLARE(Ages,    Cat21, 42, _ages);                                       ///< Data Ages

	ASTERIX_DECLARE(uint8_t, Cat21, 48, _re);                                         ///< Reserved Expansion Field
	ASTERIX_DECLARE_ARRAY(uint8_t, Cat21, 49, _sp);                                   ///< Special Purpose Field

	ASTERIX_DECLARE(double,           Cat21, LAST_FRN + 0,      _bps);                ///< Barometric Pressure Setting
	ASTERIX_DECLARE(SelHeading,       Cat21, LAST_FRN + 1,      _sel_heading);        ///< Selected Heading
	ASTERIX_DECLARE(NavMode,          Cat21, LAST_FRN + 2,      _nav);                ///< Navigation Mode
	ASTERIX_DECLARE(uint8_t,          Cat21, LAST_FRN + 3,      _gps_offset);         ///< GPS Antenna Offset
	ASTERIX_DECLARE(SurfGroundVector, Cat21, LAST_FRN + 4,      _surf_ground_vector); ///< Surface Ground Vector
	ASTERIX_DECLARE(AircraftStatus,   Cat21, LAST_FRN + 5,      _sta);                ///< Aircraft Status
	ASTERIX_DECLARE(double,           Cat21, LAST_FRN + 6,      _true_heading);       ///< True North Heading
	ASTERIX_DECLARE(Military,         Cat21, LAST_FRN + 7,      _military);           ///< Military Extended Squitter
	ASTERIX_DECLARE(uint8_t,          Cat21, LAST_FRN + RE_END, _re_end);

public:

	Cat21() : AsterixRecord(21), _air_speed(), _pos_time_high(), _vel_time_high(), _mops(), _baro_vert_speed(), _geo_vert_speed(), _ground_vector(), _sel_altitude(), _trajectory() {};
	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size);
	static char*          code(const AsterixRecord* record, size_t& write_size);
};

struct Cat21::Uap::Serializer
{
	virtual void targetReportDescriptor(Cat21& r) = 0;

	virtual void dataSourceId(Cat21& r)      = 0;
	virtual void trackNumber(Cat21& r)       = 0;
	virtual void service(Cat21& r)           = 0;
	virtual void wgs84(Cat21& r)             = 0;
	virtual void wgs84high(Cat21& r)         = 0;
	virtual void targetAddress(Cat21& r)     = 0;
	virtual void timeApplicPos(Cat21& r)     = 0;
	virtual void timeApplicVel(Cat21& r)     = 0;
	virtual void timeOfPos(Cat21& r)         = 0;
	virtual void timeOfPosHigh(Cat21& r)     = 0;
	virtual void timeOfVel(Cat21& r)         = 0;
	virtual void timeOfVelHigh(Cat21& r)     = 0;
	virtual void timeOfReport(Cat21& r)      = 0;
	virtual void geometricAltitude(Cat21& r) = 0;
	virtual void quality(Cat21& r)           = 0;
	virtual void mops(Cat21& r)              = 0;
	virtual void targetId(Cat21& r)          = 0;
	virtual void magHeading(Cat21& r)        = 0;
	virtual void targetStatus(Cat21& r)      = 0;
	virtual void airSpeed(Cat21& r)          = 0;
	virtual void trueAirSpeed(Cat21& r)      = 0;
	virtual void groundVector(Cat21& r)      = 0;
	virtual void flightLevel(Cat21& r)       = 0;
	virtual void baroVertSpeed(Cat21& r)     = 0;
	virtual void geoVertSpeed(Cat21& r)      = 0;
	virtual void mode3A(Cat21& r)            = 0;
	virtual void rollAngle(Cat21& r)         = 0;
	virtual void turnRate(Cat21& r)          = 0;
	virtual void emitterCategory(Cat21& r)   = 0;
	virtual void metInformation(Cat21& r)    = 0;
	virtual void selectedAlt(Cat21& r)       = 0;
	virtual void finalSelectedAlt(Cat21& r)  = 0;
	virtual void trajectory(Cat21& r)        = 0;
	virtual void servicePeriod(Cat21& r)     = 0;
	virtual void operationalStatus(Cat21& r) = 0;
	virtual void surfCap(Cat21& r)           = 0;
	virtual void amplitude(Cat21& r)         = 0;
	virtual void modeS(Cat21& r)             = 0;
	virtual void acas(Cat21& r)              = 0;
	virtual void recvId(Cat21& r)            = 0;
	virtual void ages(Cat21& r)              = 0;
	virtual void reserved(Cat21& r)          = 0;
	virtual void reserved_end(Cat21& r)      = 0;
	virtual void special(Cat21& r)           = 0;

	virtual void bps(Cat21& r)            = 0;
	virtual void selHeading(Cat21& r)     = 0;
	virtual void nav(Cat21& r)            = 0;
	virtual void gpsOffset(Cat21& r)      = 0;
	virtual void surfVector(Cat21& r)     = 0;
	virtual void aircraftStatus(Cat21& r) = 0;
	virtual void trueHeading(Cat21& r)    = 0;
	virtual void millitary(Cat21& r)      = 0;

	template <size_t n> void skip(Cat21& r);
	virtual ~Serializer() {};
};

}                                                          // namespace Cat21
#endif /* ASTERIX_CAT_21_HPP */
