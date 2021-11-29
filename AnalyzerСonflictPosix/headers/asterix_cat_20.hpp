#ifndef ASTERIX_CAT_20_HPP
#define ASTERIX_CAT_20_HPP

#include <math.h>
#include "asterix.hpp"
#include "asterix_internal.hpp"

namespace Cat20
{
	#define noop ((void)0)

using Asterix::DataSourceId;
using Asterix::Position;
using Asterix::ReportSource;
using Asterix::Callsign;
using Asterix::CodeFlags;
using Asterix::AcasRA;

struct TargetReportDescriptor
{
	bool ssr  : 1;
	bool ms   : 1;
	bool hf   : 1;
	bool vdl4 : 1;
	bool uat  : 1;
	bool mde  : 1;
	bool ot   : 1;

	ReportSource rab : 1;
	bool         spi : 1;
	bool         chn : 1;
	bool         gbs : 1;
	bool         crt : 1;
	bool         sim : 1;
	bool         tst : 1;
};

enum TrackStatusState
{
	TS_MAINTAINING = 0,
	TS_CLIMBING    = 1,
	TS_DECENDING   = 2,
	TS_INVALID     = 3
};

struct TrackStatus
{
	bool             init         : 1;
	bool             last         : 1;
	bool             extrapolated : 1;
	TrackStatusState state        : 2;
	bool             horiz_man    : 1;
	bool             smoothed     : 1;
	bool             ghost        : 1;
};

using Asterix::FleetType;

using Asterix::ProggramedMessage;

struct PositionAccuracy
{
	double dop_x, dop_y, dop_xy;
	double sdp_x, sdp_y, sdp_xy;
	double sdh;
	double sdw_lat, sdw_lon, sdw_latlon;

	static PositionAccuracy empty()
	{
		PositionAccuracy a;
		a.dop_x   = a.dop_y = a.dop_xy = __noop; // NAN, for Linux
		a.sdp_x   = a.sdp_y = a.sdp_xy = __noop; // NAN;
		a.sdh     = __noop; // NAN;
		a.sdw_lat = a.sdw_lon = a.sdw_latlon = __noop; // NAN;
		return a;
	}

};

using Asterix::ModeS;
using Asterix::ComCapability;

enum CommAcasFlightStatus
{
	CA_NO_ALERT_NO_SPI_AIR    = 0,
	CA_NO_ALERT_NO_SPI_GROUND = 1,
	CA_ALERT_NO_SPI_AIR       = 2,
	CA_ALERT_NO_SPI_GROUND    = 3,
	CA_ALERT_SPI              = 4,
	CA_NO_ALERT_SPI           = 5,
	CA_NOT_ASSIGNED           = 6,
	CA_NOT_EXTRACTED          = 7
};

enum CommAcasAltitudeCap
{
	CA_A100FT = 0,
	CA_A25FT  = 1,
};

struct CommAcas
{
	ComCapability        com                 : 3;
	CommAcasFlightStatus status              : 3;
	bool                 mode_s_specific_cap : 1;
	CommAcasAltitudeCap  arc                 : 1;
	bool                 aic                 : 1;

	bool    b1a : 1;
	uint8_t b1b : 4;
};

enum WarningError
{
	WE_NOT_DEFINED                                   = 0,
	WE_MULTIPATH_REPLY                               = 1,
	WE_SPLIT_PLOT                                    = 3,
	WE_PHANTOM_SSR                                   = 10,
	WE_NM_MODE_3A                                    = 11,
	WE_MODE_C_MODE_S_ALTITUDE_CODE_ABNORMAL          = 12,
	WE_TRANSPONDER_ANOMALY_DETECTED                  = 15,
	WE_DUPLICATED_OR_ILLEGAL_MODE_S_AIRCRAFT_ADDRESS = 16,
	WE_MODE_S_ERROR_CORRECTION_APPLIED               = 17,
	WE_UNDECODABLE_MODE_C_CODE_MODE_S_ALTITUDE_CODE  = 18,
};

using Asterix::PositionCartesian;
using Asterix::Velocity;
using Asterix::Acceleration;
using Asterix::Mode3A;
using Asterix::GroundVector;

struct FlightLevel
{
	CodeFlags flags;
	double    level_fl;
};

struct ModeC
{
	CodeFlags flags;
	uint16_t  code;
	uint16_t  confidence;
};

struct Mode1
{
	CodeFlags flags;
	uint8_t   code;
};

struct Mode2
{
	CodeFlags flags;
	uint16_t  code;
};

struct VelocityAccuracy
{
	double gssd;
	double tasd;
};

struct Ages
{
	double spi;
	double ti;
	double mb;
	double m3a;
	double fl;
	double fs;
	double gh;
	double ta;
	double mc;
	double mss;
	double arc;
	double aic;
	double m2;
	double m1;
	double ara;
	double vi;
	double msg;
};

/// Категория 20
struct Cat20 :
	public AsterixRecord
{
	struct Reader;
	struct Writer;

	/// Структура-описание полей asterix
	struct Uap
	{
		Asterix::UapInfo info;
		struct Serializer;
		void (Serializer::* serialize)(Cat20&);                                    ///< Функция-обработчик
	};

	static const int RE_FRN   = 27;
	static const int RE_END   = 5;
	static const int LAST_FRN = 29;
	static const int UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId,           Cat20,  1, _dsi);                          ///< Data Source Identifier
	ASTERIX_DECLARE(TargetReportDescriptor, Cat20,  2, _target_report_dscr);           ///< Target Report Descriptor
	ASTERIX_DECLARE(double,                 Cat20,  3, _time);                         ///< Time of Day
	ASTERIX_DECLARE(Position,               Cat20,  4, _position);                     ///< Position in WGS-84 Coordinates
	ASTERIX_DECLARE(PositionCartesian,      Cat20,  5, _cart_pos);                     ///< Position in Cartesian Coordinates
	ASTERIX_DECLARE(uint16_t,               Cat20,  6, _track_number);                 ///< Track Number
	ASTERIX_DECLARE(TrackStatus,            Cat20,  7, _track_status);                 ///< Track Status
	ASTERIX_DECLARE(Mode3A,                 Cat20,  8, _mode_3a);                      ///< Mode 3/A Code
	ASTERIX_DECLARE(Velocity,               Cat20,  9, _velocity);                     ///< Calculated Track Velocity in Cartesian Coord
	ASTERIX_DECLARE(FlightLevel,            Cat20, 10, _flight_level);                 ///< Flight Level
	ASTERIX_DECLARE(ModeC,                  Cat20, 11, _mode_c);                       ///< Mode-C Code
	ASTERIX_DECLARE(uint32_t,               Cat20, 12, _target_address);               ///< Target Address
	ASTERIX_DECLARE(Callsign,               Cat20, 13, _callsign);                     ///< Target Identification
	ASTERIX_DECLARE(double,                 Cat20, 14, _measured_altitude);            ///< Measured Height (Cartesian Coordinates)
	ASTERIX_DECLARE(double,                 Cat20, 15, _geometric_altitude);           ///< Geometric Height
	ASTERIX_DECLARE(Acceleration,           Cat20, 16, _accel);                        ///< Calculated Acceleration
	ASTERIX_DECLARE(uint8_t,                Cat20, 17, _fleet_type);                   ///< Vehicle Fleet Identification
	ASTERIX_DECLARE(ProggramedMessage,      Cat20, 18, _proggramed_message);           ///< Pre-programmed Message
	ASTERIX_DECLARE(PositionAccuracy,       Cat20, 19, _position_accuracy);            ///< Position Accuracy
	ASTERIX_DECLARE(uint64_t,               Cat20, 20, _contributing_devices);         ///< Contributing Devices

	ASTERIX_DECLARE_ARRAY(ModeS, Cat20, 21, _mode_s);                                  ///< Mode S MB Data

	ASTERIX_DECLARE(CommAcas, Cat20, 22, _comm_acas);                                  ///< Comms/ACAS Capability and Flight Status
	ASTERIX_DECLARE(AcasRA,   Cat20, 23, _acas);                                       ///< ACAS Resolution Advisory Report

	ASTERIX_DECLARE_ARRAY(uint8_t, Cat20, 24, _warning_errors);                        ///< Warning/Error Conditions

	ASTERIX_DECLARE(Mode1,   Cat20, 25, _mode_1);                                      ///< Mode-1 Code
	ASTERIX_DECLARE(Mode2,   Cat20, 26, _mode_2);                                      ///< Mode-2 Code
	ASTERIX_DECLARE(uint8_t, Cat20, 27, _re);                                          ///< Reserved Expansion Field
	ASTERIX_DECLARE_ARRAY(uint8_t, Cat20, 28, _sp);                                    ///< Special Purpose Field

	ASTERIX_DECLARE(PositionAccuracy, Cat20, LAST_FRN + 0,      _re_accuracy);         ///< Position Accuracy
	ASTERIX_DECLARE(GroundVector,     Cat20, LAST_FRN + 1,      _ground_vector);       ///< Ground Velocity Vector
	ASTERIX_DECLARE(VelocityAccuracy, Cat20, LAST_FRN + 2,      _ground_vel_accuracy); ///< Ground Velocity Accuracy
	ASTERIX_DECLARE(double,           Cat20, LAST_FRN + 3,      _report_time);         ///< Time of ASTERIX Report Transmission
	ASTERIX_DECLARE(Ages,             Cat20, LAST_FRN + 4,      _ages);                ///< Data-Ages
	ASTERIX_DECLARE(uint8_t,          Cat20, LAST_FRN + RE_END, _re_end);

public:

	Cat20() : AsterixRecord(20), _callsign(), _fleet_type(Asterix::FT_UNKNOWN), _ground_vector() {};
	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size);
	static char*          code(const AsterixRecord* record, size_t& write_size);

};

struct Cat20::Uap::Serializer
{
	virtual void dataSourceId(Cat20& r)           = 0;
	virtual void targetReportDescriptor(Cat20& r) = 0;
	virtual void timeOfDay(Cat20& r)              = 0;
	virtual void reserved(Cat20& r)               = 0;
	virtual void reserved_end(Cat20& r)           = 0;
	virtual void special(Cat20& r)                = 0;
	virtual void wgs84(Cat20& r)                  = 0;
	virtual void cartCoords(Cat20& r)             = 0;
	virtual void trackNumber(Cat20& r)            = 0;
	virtual void trackStatus(Cat20& r)            = 0;
	virtual void mode3A(Cat20& r)                 = 0;
	virtual void velocity(Cat20& r)               = 0;
	virtual void flightLevel(Cat20& r)            = 0;
	virtual void targetAddress(Cat20& r)          = 0;
	virtual void targetId(Cat20& r)               = 0;
	virtual void measuredHeight(Cat20& r)         = 0;
	virtual void geometricAltitude(Cat20& r)      = 0;
	virtual void acceleration(Cat20& r)           = 0;
	virtual void accuracy(Cat20& r)               = 0;
	virtual void contributions(Cat20& r)          = 0;
	virtual void modeS(Cat20& r)                  = 0;
	virtual void warning(Cat20& r)                = 0;
	virtual void fleetId(Cat20& r)                = 0;

	virtual void modeC(Cat20& r)             = 0;
	virtual void programmedMessage(Cat20& r) = 0;
	virtual void commsAcas(Cat20& r)         = 0;
	virtual void acas(Cat20& r)              = 0;
	virtual void mode1(Cat20& r)             = 0;
	virtual void mode2(Cat20& r)             = 0;

	virtual void re_accuracy(Cat20& r)          = 0;
	virtual void groundVector(Cat20& r)         = 0;
	virtual void groundVectorAccuracy(Cat20& r) = 0;
	virtual void timeOfReport(Cat20& r)         = 0;
	virtual void ages(Cat20& r)                 = 0;

	template <size_t N> void skip(Cat20& r);
	virtual ~Serializer() {};
};

}                                                          // namespace Cat20

#endif                                                     /* ASTERIX_CAT_20_HPP */
