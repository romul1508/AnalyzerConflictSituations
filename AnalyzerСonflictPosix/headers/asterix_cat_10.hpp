#ifndef ASTERIX_CAT_10_HPP
#define ASTERIX_CAT_10_HPP

#include "asterix.hpp"
#include "asterix_internal.hpp"

namespace Cat10
{
#include "cat_010_asterix.h"
using Asterix::DataSourceId;
using Asterix::Position;
using Asterix::PositionCartesian;
using Asterix::PolarVelocity;
using Asterix::Velocity;
using Asterix::Callsign;
using Asterix::ModeS;
using Asterix::FleetType;
using Asterix::ProggramedMessage;
using Asterix::Acceleration;
using Asterix::Size;
using Asterix::Mode3A;

typedef Cat10MessageType MessageType;
typedef TrackStatus010   TrackStatus;

struct PolarPosition
{
	double rho;
	double thetha;
};

struct Accuracy
{
	double sdp_x, sdp_y, sdp_xy;
};

/// Категория 10
struct Cat10 :
	public AsterixRecord
{
	struct Reader;
	struct Writer;

	/// Структура-описание полей asterix
	struct Uap
	{
		Asterix::UapInfo info;
		struct Serializer;
		void (Serializer::* serialize)(Cat10&);                          ///< Функция-обработчик
	};

	static const int RE_FRN   = 28;
	static const int RE_END   = 0;
	static const int LAST_FRN = 29;
	static const int UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId,           Cat10,  1, _dsi);                ///< Data Source Identifier
	ASTERIX_DECLARE(uint8_t,                Cat10,  2, _message_type);       ///< Message Type
	ASTERIX_DECLARE(TargetReportDescriptor, Cat10,  3, _target_report_dscr); ///< Target Report Descriptor
	ASTERIX_DECLARE(double,                 Cat10,  4, _time);               ///< Time of Day
	ASTERIX_DECLARE(Position,               Cat10,  5, _position);           ///< Position in WGS-84 Coordinates
	ASTERIX_DECLARE(PolarPosition,          Cat10,  6, _polar_pos);          ///< Measured Position in Polar Co-ordinates
	ASTERIX_DECLARE(PositionCartesian,      Cat10,  7, _cart_pos);           ///< Position in Cartesian Co-ordinates
	ASTERIX_DECLARE(PolarVelocity,          Cat10,  8, _polar_vel);          ///< Calculated Track Velocity in Polar Co-ordinates
	ASTERIX_DECLARE(Velocity,               Cat10,  9, _velocity);           ///< Calculated Track Velocity in Cartesian Coord.
	ASTERIX_DECLARE(uint16_t,               Cat10, 10, _track_number);       ///< Track Number
	ASTERIX_DECLARE(TrackStatus,            Cat10, 11, _track_status);       ///< Track Status
	ASTERIX_DECLARE(Mode3A,                 Cat10, 12, _mode_3a);            ///< Mode 3/A Code
	ASTERIX_DECLARE(uint32_t,               Cat10, 13, _target_address);     ///< Target Address
	ASTERIX_DECLARE(Callsign,               Cat10, 14, _callsign);           ///< Target Identification

	ASTERIX_DECLARE_ARRAY(ModeS, Cat10, 15, _mode_s);                        ///< Mode S MB Data

	ASTERIX_DECLARE(FleetType,         Cat10, 16, _fleet_type);              ///< Vehicle Fleet Identification
	ASTERIX_DECLARE(double,            Cat10, 17, _flight_level);            ///< Flight Level
	ASTERIX_DECLARE(double,            Cat10, 18, _measured_altitude);       ///< Measured Height
	ASTERIX_DECLARE(Size,              Cat10, 19, _size);                    ///< Target Size & Orientation
	ASTERIX_DECLARE(SystemStatus,      Cat10, 20, _system_status);           ///< System Status
	ASTERIX_DECLARE(ProggramedMessage, Cat10, 21, _proggramed_message);      ///< Pre-programmed Message
	ASTERIX_DECLARE(Accuracy,          Cat10, 22, _accuracy);                ///< Standard Deviation of Position

	ASTERIX_DECLARE_ARRAY(PolarPosition, Cat10, 23, _presence);              ///< Presence

	ASTERIX_DECLARE(uint8_t,      Cat10, 24, _amplitude);                    ///< Amplitude of Primary Plot
	ASTERIX_DECLARE(Acceleration, Cat10, 25, _accel);                        ///< Calculated Acceleration

	ASTERIX_DECLARE_ARRAY(uint8_t, Cat10, 27, _sp);                          ///< Special Purpose Field
	ASTERIX_DECLARE(uint8_t, Cat10, 28, _re);                                ///< Reserved Expansion Field

public:

	Cat10() : AsterixRecord(10) {};
	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size);
	static char*          code(const AsterixRecord* record, size_t& write_size);
};

struct Cat10::Uap::Serializer
{
	virtual void dataSourceId(Cat10& r)           = 0;
	virtual void messageType(Cat10& r)            = 0;
	virtual void targetReportDescriptor(Cat10& r) = 0;
	virtual void timeOfDay(Cat10& r)              = 0;
	virtual void wgs84(Cat10& r)                  = 0;
	virtual void polarCoords(Cat10& r)            = 0;
	virtual void cartCoords(Cat10& r)             = 0;
	virtual void polarVelocity(Cat10& r)          = 0;
	virtual void velocity(Cat10& r)               = 0;
	virtual void trackNumber(Cat10& r)            = 0;
	virtual void trackStatus(Cat10& r)            = 0;
	virtual void mode3A(Cat10& r)                 = 0;
	virtual void targetAddress(Cat10& r)          = 0;
	virtual void targetId(Cat10& r)               = 0;
	virtual void modeS(Cat10& r)                  = 0;
	virtual void fleetId(Cat10& r)                = 0;
	virtual void flightLevel(Cat10& r)            = 0;
	virtual void measuredHeight(Cat10& r)         = 0;
	virtual void size(Cat10& r)                   = 0;
	virtual void systemStatus(Cat10& r)           = 0;
	virtual void programmedMessage(Cat10& r)      = 0;
	virtual void accuracy(Cat10& r)               = 0;
	virtual void presence(Cat10& r)               = 0;
	virtual void amplitude(Cat10& r)              = 0;
	virtual void acceleration(Cat10& r)           = 0;
	virtual void reserved(Cat10& r)               = 0;
	virtual void special(Cat10& r)                = 0;

	template <size_t N> void skip(Cat10& r);

	virtual ~Serializer() {};
};
};

#endif                                                     /* ASTERIX_CAT_10_HPP */
