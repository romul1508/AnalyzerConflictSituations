#ifndef ASTERIX_CAT_62_HPP
#define ASTERIX_CAT_62_HPP

#include "asterix.hpp"
#include "asterix_internal.hpp"
#include "cat_062_asterix.h"

namespace Cat62
{
using Asterix::DataSourceId;
using Asterix::Position;
using Asterix::PositionCartesian;
using Asterix::Velocity;
using Asterix::Callsign;
using Asterix::FleetType;
using Asterix::Acceleration;
using Asterix::Size;
using Asterix::Mode3A;
using Asterix::CodeFlags;

typedef TTrackStatus              TrackStatus;
typedef Cat062AircraftDerivedData AircraftData;
typedef TFlightPlanData           FlightPlan;
struct Mode5 {};                                           ///< TODO не экспортировано библиотекой
struct ComposedTrack
{
	uint8_t  sid;
	uint16_t tn;
};
typedef TTrackAccuracy Accuracy;
typedef TMeasuredData  MeasuredData;

struct SystemAges
{
	double trk;
	double psr;
	double ssr;
	double mds;
	double ads;
	double ads_es;
	double vdl;
	double uat;
	double loop;
	double mlt;
};

struct DataAges
{
	double mfl;
	double md1;
	double md2;
	double mda;
	double md4;
	double md5;
	double mhg;

	double ias;
	double tas;
	double sal;
	double fss;
	double tid;
	double com;
	double sab;

	double acs;
	double bvr;
	double gvr;
	double ran;
	double tar;
	double tan;
	double gsp;

	double vun;
	double met;
	double emc;
	double pos;
	double gal;
	double pun;
	double mb;

	double iar;
	double mac;
	double bps;
};

struct BaroAltitude
{
	bool   qnh;
	double altitude;
};

struct Mode2
{
	CodeFlags flags;
	uint16_t  code;
};

struct Movement
{
	TargetModeMovement trans;
	TargetModeMovement longit;
	TargetModeMovement vert;
	bool               adf;
};

struct Contributions
{
	DataSourceId dsi;
	uint8_t      typ;
	uint16_t     track;
};
struct ContributionsNoTN
{
	DataSourceId dsi;
	uint8_t      typ;
};

struct TrackStatusSup
{
	bool fdr;
};

/// Категория 62
struct Cat62 :
	public AsterixRecord
{
	struct Reader;
	struct Writer;

	/// Структура-описание полей asterix
	struct Uap
	{
		Asterix::UapInfo info;
		struct Serializer;
		void (Serializer::* serialize)(Cat62&);                                     ///< Функция-обработчик
	};

	static const int RE_FRN   = 34;
	static const int RE_END   = 4;
	static const int LAST_FRN = 36;
	static const int UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId,      Cat62,  1, _dsi);                                ///< Data Source Identifier

	ASTERIX_DECLARE(uint8_t,           Cat62,  3, _service);                            ///< Service Identification
	ASTERIX_DECLARE(double,            Cat62,  4, _time);                               ///< Time of Track
	ASTERIX_DECLARE(Position,          Cat62,  5, _position);                           ///< Position in WGS-84 Coordinates
	ASTERIX_DECLARE(PositionCartesian, Cat62,  6, _cart_pos);                           ///< Position in Cartesian Co-ordinates
	ASTERIX_DECLARE(Velocity,          Cat62,  7, _velocity);                           ///< Calculated Track Velocity in Cartesian Coord.
	ASTERIX_DECLARE(Acceleration,      Cat62,  8, _accel);                              ///< Calculated Acceleration
	ASTERIX_DECLARE(Mode3A,            Cat62,  9, _mode_3a);                            ///< Mode 3/A Code
	ASTERIX_DECLARE(Callsign,          Cat62, 10, _callsign);                           ///< Target Identification
	ASTERIX_DECLARE(AircraftData,      Cat62, 11, _aircraft_data);                      ///< Aircraft Derived Data
	ASTERIX_DECLARE(uint16_t,          Cat62, 12, _track_number);                       ///< Track Number
	ASTERIX_DECLARE(TrackStatus,       Cat62, 13, _track_status);                       ///< Track Status
	ASTERIX_DECLARE(SystemAges,        Cat62, 14, _system_ages);                        ///< System Track Update Ages
	ASTERIX_DECLARE(Movement,          Cat62, 15, _movement);                           ///< Mode of Movement
	ASTERIX_DECLARE(DataAges,          Cat62, 16, _data_ages);                          ///< Track Data Ages
	ASTERIX_DECLARE(double,            Cat62, 17, _flight_level);                       ///< Flight Level
	ASTERIX_DECLARE(double,            Cat62, 18, _geo_altitude);                       ///< Calculated Track Geometric Altitude
	ASTERIX_DECLARE(BaroAltitude,      Cat62, 19, _baro_altitude);                      ///< Calculated Track Barometric Altitude
	ASTERIX_DECLARE(double,            Cat62, 20, _vert_rate);                          ///< Calculated Track Geometric Altitude
	ASTERIX_DECLARE(FlightPlan,        Cat62, 21, _flight_plan);                        ///< Flight Plan Related Data
	ASTERIX_DECLARE(Size,              Cat62, 22, _size);                               ///< Target Size & Orientation
	ASTERIX_DECLARE(FleetType,         Cat62, 23, _fleet_type);                         ///< Vehicle Fleet Identification
	ASTERIX_DECLARE(Mode5,             Cat62, 24, _mode5);                              ///< Mode 5 Data reports & Extended Mode 1 Code
	ASTERIX_DECLARE(Mode2,             Cat62, 25, _mode_2);                             ///< Mode-2 Code
	ASTERIX_DECLARE_ARRAY(ComposedTrack, Cat62, 26, _composed_track);                   ///< Composed Track Number
	ASTERIX_DECLARE(Accuracy,     Cat62, 27, _accuracy);                                ///< Estimated Accuracies
	ASTERIX_DECLARE(MeasuredData, Cat62, 28, _measured_data);                           ///< Measured Information

	ASTERIX_DECLARE(uint8_t,      Cat62, 34, _re);                                      ///< Reserved Expansion Field
	ASTERIX_DECLARE_ARRAY(uint8_t, Cat62, 35, _sp);                                     ///< Special Purpose Field

	ASTERIX_DECLARE(Contributions,     Cat62, LAST_FRN + 0,      _contributions);       ///< Contributing Sensors With Local Tracknumbers
	ASTERIX_DECLARE(ContributionsNoTN, Cat62, LAST_FRN + 1,      _contributions_no_tn); ///< Contributing Sensors No Local Tracknumbers
	ASTERIX_DECLARE(Velocity,          Cat62, LAST_FRN + 2,      _velocity_to_ref);     ///< Calculated Track Velocity Relative to System Reference Point
	ASTERIX_DECLARE(TrackStatusSup,    Cat62, LAST_FRN + 3,      _track_status_sup);    ///< Supplementary Track Status
	ASTERIX_DECLARE(uint8_t,           Cat62, LAST_FRN + RE_END, _re_end);

public:

	Cat62() : AsterixRecord(62) {};
	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size);
	static char*          code(const AsterixRecord* record, size_t& write_size);
};

struct Cat62::Uap::Serializer
{
	virtual void dataSourceId(Cat62& r) = 0;
	virtual void service(Cat62& r)      = 0;
	virtual void timeOfTrack(Cat62& r)  = 0;
	virtual void wgs84(Cat62& r)        = 0;
	virtual void cartCoords(Cat62& r)   = 0;
	virtual void velocity(Cat62& r)     = 0;
	virtual void acceleration(Cat62& r) = 0;
	virtual void mode3A(Cat62& r)       = 0;
	virtual void targetId(Cat62& r)     = 0;
	virtual void aircraftData(Cat62& r) = 0;
	virtual void trackNumber(Cat62& r)  = 0;
	virtual void trackStatus(Cat62& r)  = 0;
	virtual void systemAges(Cat62& r)   = 0;
	virtual void movement(Cat62& r)     = 0;
	virtual void dataAges(Cat62& r)     = 0;
	virtual void flightLevel(Cat62& r)  = 0;
	virtual void geoAltitde(Cat62& r)   = 0;
	virtual void baroAltitde(Cat62& r)  = 0;
	virtual void vertRate(Cat62& r)     = 0;
	virtual void flightPlan(Cat62& r)   = 0;
	virtual void size(Cat62& r)         = 0;
	virtual void fleetId(Cat62& r)      = 0;
	virtual void mode5(Cat62& r)        = 0;
	virtual void mode2(Cat62& r)        = 0;
	virtual void composed(Cat62& r)     = 0;
	virtual void accuracy(Cat62& r)     = 0;
	virtual void measured(Cat62& r)     = 0;
	virtual void reserved(Cat62& r)     = 0;
	virtual void special(Cat62& r)      = 0;

	virtual void contributions(Cat62& r)       = 0;
	virtual void contributions_no_tn(Cat62& r) = 0;
	virtual void velocity_to_ref(Cat62& r)     = 0;
	virtual void track_status_sup(Cat62& r)    = 0;

	template <size_t N> void skip(Cat62& r);

	virtual ~Serializer() {};
};
};

#endif                                                     /* ASTERIX_CAT_62_HPP */
