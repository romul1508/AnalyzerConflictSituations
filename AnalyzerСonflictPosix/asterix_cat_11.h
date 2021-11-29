#ifndef ASTERIX_CAT_11_H
#define ASTERIX_CAT_11_H

#include "asterix.h"
#include "asterix_internal.h"

namespace Cat11
{
#include "asterix_011_category.h"
using Asterix::DataSourceId;
using Asterix::Position;
using Asterix::PositionCartesian;
using Asterix::PolarVelocity;
using Asterix::Velocity;
using Asterix::Callsign;
using Asterix::FleetType;
using Asterix::ProggramedMessage;
using Asterix::Acceleration;
using Asterix::Size;
using Asterix::Mode3A;

typedef Cat11MessageType       MessageType;
typedef TTrackStatus           TrackStatus;
typedef TCat011AdsBRelatedData ModeS;
typedef Cat11PhaseOfFlight     PhaseOfFlight;
typedef TFlightPlanData        FlightPlan;
typedef TTrackAccuracy         Accuracy;
typedef TCat011AlertMessage    Alert;

struct Holdbar
{
	uint8_t  bank;
	uint16_t indicator;
};

struct Ages
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
};

struct BaroAltitude
{
	bool   qnh;
	double altitude;
};

enum CommAcasFlightStatus
{
	CA_NO_ALERT_NO_SPI_AIR    = 0,
	CA_NO_ALERT_NO_SPI_GROUND = 1,
	CA_ALERT_NO_SPI_AIR       = 2,
	CA_ALERT_NO_SPI_GROUND    = 3,
	CA_ALERT_SPI              = 4,
	CA_NO_ALERT_SPI           = 5,
	CA_GENERAL_EMERGENCY      = 6,
	CA_MEDICAL_EMERGENCY      = 7,
	CA_MINIMUM_FUEL           = 8,
	CA_NO_COMMUNICATIONS      = 9,
	CA_UNLAWFUL_INFERENCE     = 10,
};

enum CommAcasAltitudeCap
{
	CA_A100FT = 0,
	CA_A25FT  = 1,
};

/// Категория 11
struct Cat11 :
	public AsterixRecord
{
	struct Reader;
	struct Writer;

	/// Структура-описание полей asterix
	struct Uap
	{
		Asterix::UapInfo info;
		struct Serializer;

		// Функция-обработчик
		void (Serializer::* serialize)(Cat11&);                     
	};

	static const int RE_FRN   = 29;
	static const int RE_END   = 0;
	static const int LAST_FRN = 36;
	static const int UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap UAP[UAP_SIZE];

	// Data Source Identifier
	ASTERIX_DECLARE(DataSourceId,      Cat11,  1, _dsi);                
		
	
	ASTERIX_DECLARE(uint8_t,           Cat11,  2, _message_type);       ///< Message Type
	ASTERIX_DECLARE(uint8_t,           Cat11,  3, _service);            ///< Service Identification
	ASTERIX_DECLARE(double,            Cat11,  4, _time);               ///< Time of Track
	ASTERIX_DECLARE(Position,          Cat11,  5, _position);           ///< Position in WGS-84 Coordinates
	ASTERIX_DECLARE(PositionCartesian, Cat11,  6, _cart_pos);           ///< Position in Cartesian Co-ordinates
	ASTERIX_DECLARE(Velocity,          Cat11,  7, _velocity);           ///< Calculated Track Velocity in Cartesian Coord.
	ASTERIX_DECLARE(Acceleration,      Cat11,  8, _accel);              ///< Calculated Acceleration
	ASTERIX_DECLARE(Mode3A,            Cat11,  9, _mode_3a);            ///< Mode 3/A Code
	ASTERIX_DECLARE(Callsign,          Cat11, 10, _callsign);           ///< Target Identification
	ASTERIX_DECLARE(ModeS,             Cat11, 11, _mode_s);             ///< Mode-S / ADS-B Related Data
	ASTERIX_DECLARE(uint16_t,          Cat11, 12, _track_number);       ///< Track Number
	ASTERIX_DECLARE(TrackStatus,       Cat11, 13, _track_status);       ///< Track Status
	ASTERIX_DECLARE(Ages,              Cat11, 14, _ages);               ///< System Track Update Ages
	ASTERIX_DECLARE(PhaseOfFlight,     Cat11, 15, _phase);              ///< Phase of Flight
	ASTERIX_DECLARE(double,            Cat11, 16, _flight_level);       ///< Flight Level
	ASTERIX_DECLARE(BaroAltitude,      Cat11, 17, _baro_altitude);      ///< Calculated Track Barometric Altitude
	ASTERIX_DECLARE(double,            Cat11, 18, _geo_altitude);       ///< Calculated Track Geometric Altitude
	ASTERIX_DECLARE(double,            Cat11, 19, _vert_rate);          ///< Calculated Track Geometric Altitude
	ASTERIX_DECLARE(Size,              Cat11, 20, _size);               ///< Target Size & Orientation
	ASTERIX_DECLARE(FlightPlan,        Cat11, 21, _flight_plan);        ///< Flight Plan Related Data
	ASTERIX_DECLARE(FleetType,         Cat11, 22, _fleet_type);         ///< Vehicle Fleet Identification
	ASTERIX_DECLARE(ProggramedMessage, Cat11, 23, _proggramed_message); ///< Pre-programmed Message
	ASTERIX_DECLARE(Accuracy,          Cat11, 24, _accuracy);           ///< Estimated Accuracies
	ASTERIX_DECLARE(Alert,             Cat11, 25, _alert);              ///< Alert Message

	ASTERIX_DECLARE_ARRAY(uint16_t, Cat11, 26, _alert_tracks);          ///< Tracks in Alert
	ASTERIX_DECLARE_ARRAY(Holdbar,  Cat11, 27, _holdbar_status);        ///< Holdbar Status
	ASTERIX_DECLARE_ARRAY(uint8_t,  Cat11, 28, _sp);                    ///< Special Purpose Field
	ASTERIX_DECLARE(uint8_t, Cat11, 29, _re);                           ///< Reserved Expansion Field

public:

	Cat11() : AsterixRecord(11), _phase() {};
	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size);
	static char*          code(const AsterixRecord* record, size_t& write_size);
};

struct Cat11::Uap::Serializer
{
	virtual void dataSourceId(Cat11& r)      = 0;
	virtual void messageType(Cat11& r)       = 0;
	virtual void service(Cat11& r)           = 0;
	virtual void timeOfTrack(Cat11& r)       = 0;
	virtual void wgs84(Cat11& r)             = 0;
	virtual void cartCoords(Cat11& r)        = 0;
	virtual void velocity(Cat11& r)          = 0;
	virtual void acceleration(Cat11& r)      = 0;
	virtual void mode3A(Cat11& r)            = 0;
	virtual void targetId(Cat11& r)          = 0;
	virtual void modeS(Cat11& r)             = 0;
	virtual void trackNumber(Cat11& r)       = 0;
	virtual void trackStatus(Cat11& r)       = 0;
	virtual void dataAges(Cat11& r)          = 0;
	virtual void flightPhase(Cat11& r)       = 0;
	virtual void flightLevel(Cat11& r)       = 0;
	virtual void baroAltitde(Cat11& r)       = 0;
	virtual void geoAltitde(Cat11& r)        = 0;
	virtual void vertRate(Cat11& r)          = 0;
	virtual void size(Cat11& r)              = 0;
	virtual void flightPlan(Cat11& r)        = 0;
	virtual void fleetId(Cat11& r)           = 0;
	virtual void programmedMessage(Cat11& r) = 0;
	virtual void accuracy(Cat11& r)          = 0;
	virtual void alert(Cat11& r)             = 0;
	virtual void alertTracks(Cat11& r)       = 0;
	virtual void holdbar(Cat11& r)           = 0;
	virtual void reserved(Cat11& r)          = 0;
	virtual void special(Cat11& r)           = 0;

	template <size_t N> void skip(Cat11& r);

	virtual ~Serializer() {};
};
};

#endif   /* ASTERIX_CAT_11_H */
