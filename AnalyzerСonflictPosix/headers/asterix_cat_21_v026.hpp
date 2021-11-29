#ifndef ASTERIX_CAT_21_V026_HPP
#define ASTERIX_CAT_21_V026_HPP

#include "asterix.hpp"
#include "asterix_internal.hpp"

namespace Cat21
{
namespace Ver026
{

using Asterix::DataSourceId;
using Asterix::Position;
using Asterix::PolarVelocity;
using Asterix::Callsign;

struct AirSpeed
{
	bool   mach;
	double speed;
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
		void (Serializer::* serialize)(Cat21&);             ///< Функция-обработчик
	};

	static const int RE_FRN   = 34;
	static const int RE_END   = 0;
	static const int LAST_FRN = 36;
	static const int UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId,  Cat21,  1, _dsi);            ///< Data Source Identifier
	ASTERIX_DECLARE(double,        Cat21,  3, _time);           ///< Time of Day
	ASTERIX_DECLARE(Position,      Cat21,  4, _position);       ///< Position in WGS-84 Coordinates
	ASTERIX_DECLARE(uint32_t,      Cat21,  5, _target_address); ///< Target Address
	ASTERIX_DECLARE(double,        Cat21,  6, _altitude_ft);    ///< Geometric Height
	ASTERIX_DECLARE(double,        Cat21, 10, _flight_level);   ///< Flight Level
	ASTERIX_DECLARE(AirSpeed,      Cat21, 11, _air_speed);      ///< Air Speed
	ASTERIX_DECLARE(double,        Cat21, 12, _true_air_speed); ///< True Air Speed
	ASTERIX_DECLARE(double,        Cat21, 13, _heading);        ///< Magnetic Heading
	ASTERIX_DECLARE(PolarVelocity, Cat21, 16, _ground_vector);  ///< Ground Vector
	ASTERIX_DECLARE(Callsign,      Cat21, 18, _callsign);       ///< Target Identification

	ASTERIX_DECLARE(uint8_t,       Cat21, 34, _re);             ///< Reserved Expansion Field
	ASTERIX_DECLARE_ARRAY(uint8_t, Cat21, 35, _sp);             ///< Special Purpose Field
	ASTERIX_DECLARE(uint8_t, Cat21, LAST_FRN + RE_END, _re_end);

public:

	Cat21() : AsterixRecord(21) {};
	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size);
	static char*          code(const AsterixRecord* record, size_t& write_size);
};

struct Cat21::Uap::Serializer
{
	virtual void record(Cat21& r) = 0;

	virtual void dataSourceId(Cat21& r)  = 0;
	virtual void timeOfDay(Cat21& r)     = 0;
	virtual void wgs84(Cat21& r)         = 0;
	virtual void targetAddress(Cat21& r) = 0;
	virtual void altitude(Cat21& r)      = 0;
	virtual void flightLevel(Cat21& r)   = 0;
	virtual void airSpeed(Cat21& r)      = 0;
	virtual void trueAirSpeed(Cat21& r)  = 0;
	virtual void heading(Cat21& r)       = 0;
	virtual void groundVector(Cat21& r)  = 0;
	virtual void targetId(Cat21& r)      = 0;

	virtual void rateOfTurn(Cat21& r)       = 0;
	virtual void metInformation(Cat21& r)   = 0;
	virtual void trajectoryIntent(Cat21& r) = 0;
	virtual void reserved(Cat21& r)         = 0;
	virtual void reserved_end(Cat21& r)     = 0;
	virtual void special(Cat21& r)          = 0;

	virtual void             byteSkip(Cat21&) = 0;
	template <size_t n> void skip(Cat21& r);
	virtual ~Serializer() {};
};


}                                                          // namespace Ver026

}                                                          // namespace Cat21

#endif /* ASTERIX_CAT_21_V026_HPP */
