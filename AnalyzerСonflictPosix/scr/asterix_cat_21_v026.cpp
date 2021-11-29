#include "asterix_cat_21_v026.hpp"
#include "asterix_template.hpp"
#include <math.h>

static const Ratio<1, 128>             TIME_LSB;
static const Ratio<25* 3048, 4* 10000> ALTITUDE_LSB;
static const Ratio<360, 1 << 16>       DEG_360_16;
static const Ratio<1852, 1 << 14>      NM_14;
static const Ratio<1852, 3600>         KNOT;
static const Ratio<1, 4>               FLIGHT_LEVEL_LSB;

namespace Cat21
{
namespace Ver026
{
const Ratio<180, 1 << 25> WGS84_LSB;

template <> void Cat21::Uap::Serializer::skip<0>(Cat21&) {};
template <size_t N> void Cat21::Uap::Serializer::skip(Cat21& r)
{
	byteSkip(r);
	skip<N - 1>(r);
}

const Cat21::Uap Cat21::UAP[Cat21::UAP_SIZE] =
{
	ASTERIX_UAP_FIELD(Cat21,        0,          0, "Field Extension Indicator",             0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat21,        1,         10, "Data Source Identification",            2, dataSourceId),
	ASTERIX_UAP_FIELD(Cat21,        2,         40, "Target Report Descriptor",              2, skip<2> ),
	ASTERIX_UAP_FIELD(Cat21,        3,         30, "Time of Day",                           3, timeOfDay),
	ASTERIX_UAP_FIELD(Cat21,        4,        130, "Position in WGS-84 coordinates",        8, wgs84),
	ASTERIX_UAP_FIELD(Cat21,        5,         80, "Target Address",                        3, targetAddress),
	ASTERIX_UAP_FIELD(Cat21,        6,        140, "Geometric Altitude",                    2, altitude),
	ASTERIX_UAP_FIELD(Cat21,        7,         90, "Figure of Merit",                       2, skip<2> ),
	ASTERIX_UAP_FIELD(Cat21,        8,        210, "Link technology",                       1, skip<1> ),
	ASTERIX_UAP_FIELD(Cat21,        9,        230, "Roll Angle",                            2, skip<2> ),
	ASTERIX_UAP_FIELD(Cat21,       10,        145, "Flight Level",                          2, flightLevel),
	ASTERIX_UAP_FIELD(Cat21,       11,        150, "Air Speed",                             2, airSpeed),
	ASTERIX_UAP_FIELD(Cat21,       12,        151, "True Air Speed",                        2, trueAirSpeed),
	ASTERIX_UAP_FIELD(Cat21,       13,        152, "Magnetic Heading",                      2, heading),
	ASTERIX_UAP_FIELD(Cat21,       14,        155, "Barometric Vertical Rate",              2, skip<2> ),
	ASTERIX_UAP_FIELD(Cat21,       15,        157, "Geometric Vertical Rate",               2, skip<2> ),
	ASTERIX_UAP_FIELD(Cat21,       16,        160, "Ground Vector",                         4, groundVector),
	ASTERIX_UAP_FIELD(Cat21,       17,        165, "Rate of Turn",                          0, rateOfTurn),
	ASTERIX_UAP_FIELD(Cat21,       18,        170, "Target Identification",                 6, targetId),
	ASTERIX_UAP_FIELD(Cat21,       19,         95, "Velocity Accuracy",                     1, skip<1> ),
	ASTERIX_UAP_FIELD(Cat21,       20,         32, "Time of day accuracy",                  1, skip<1> ),
	ASTERIX_UAP_FIELD(Cat21,       21,        200, "Target Status",                         1, skip<1> ),
	ASTERIX_UAP_FIELD(Cat21,       22,         20, "Emitter Category",                      1, skip<1> ),
	ASTERIX_UAP_FIELD(Cat21,       23,        220, "Met report",                            0, metInformation),
	ASTERIX_UAP_FIELD(Cat21,       24,        146, "Intermediate State Selected Altitude",  2, skip<2> ),
	ASTERIX_UAP_FIELD(Cat21,       25,        148, "Final State Selected Altitude",         2, skip<2> ),
	ASTERIX_UAP_FIELD(Cat21,       26,        110, "Trajectory Intent",                     0, trajectoryIntent),
	ASTERIX_UAP_FIELD(Cat21,       27,         70, "Mode 3/A Code in Octal Representation", 2, skip<2> ),
	ASTERIX_UAP_FIELD(Cat21,       28,        131, "Signal Amplitude",                      1, skip<1> ),
	ASTERIX_UAP_FIELD(Cat21,       29,          0, "Spare bits set to zero",                0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat21,       30,          0, "Spare bits set to zero",                0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat21,       31,          0, "Spare bits set to zero",                0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat21,       32,          0, "Spare bits set to zero",                0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat21,       33,          0, "Spare bits set to zero",                0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat21,       34,          0, "Reserved Expansion Field",              0, reserved),
	ASTERIX_UAP_FIELD(Cat21,       35,          0, "Special Purpose Field",                 0, special),
	ASTERIX_UAP_FIELD(Cat21, LAST_FRN + RE_END, 0, "Reserved Expansion Field",              0, reserved_end),
};

struct Cat21::Reader :
	public Cat21::Uap::Serializer, public AsterixReaderT
{
public:

	Reader(const char* data, size_t size) :
		AsterixReaderT(data, size) {};

	void readRecord(Cat21& v)
	{
		v._fspec = readFspec();
		record(v);
	}

private:

	void record(Cat21& r)
	{
		const Uap* p = Cat21::UAP;
		for (int i = 0; i < Cat21::UAP_SIZE; ++i, ++p)
		{
			if ((r._fspec & p->info.mask) && p->serialize)
				(this->*p->serialize)(r);
		}
	}

	void dataSourceId(Cat21& r)
	{
		r._dsi.sac = readInt<8, unsigned>();
		r._dsi.sic = readInt<8, unsigned>();
	}

	void timeOfDay(Cat21& r)
	{
		r._time = readDouble<24, unsigned>(TIME_LSB);
	}

	void wgs84(Cat21& r)
	{
		r._position.wgs84_lat = readDouble<32>(WGS84_LSB);
		r._position.wgs84_lon = readDouble<32>(WGS84_LSB);
	}

	void targetAddress(Cat21& r)
	{
		r._target_address = readInt<24, unsigned>();
	}

	void altitude(Cat21& r)
	{
		r._altitude_ft = readDouble<16>(ALTITUDE_LSB);
	}

	void flightLevel(Cat21& r)
	{
		r._flight_level = readDouble<16>(FLIGHT_LEVEL_LSB);
	}

	void groundVector(Cat21& r)
	{
		r._ground_vector.speed   = readDouble<16>(NM_14);
		r._ground_vector.heading = readDouble<16, unsigned>(DEG_360_16);
	}

	void airSpeed(Cat21& r)
	{
		uint16_t speed_i = readInt<16, unsigned>();
		r._air_speed.mach = speed_i & 1 << 15;
		speed_i          &= ~(1 << 15);
		if (r._air_speed.mach)
			r._air_speed.speed = speed_i * 0.001;
		else
			r._air_speed.speed = speed_i * NM_14;
	}

	void trueAirSpeed(Cat21& r)
	{
		r._true_air_speed = readDouble<16, unsigned>(KNOT);
	}

	void heading(Cat21& r)
	{
		r._heading = readDouble<16, unsigned>(DEG_360_16);
	}

	void targetId(Cat21& r)
	{
		uint64_t id = readInt<48, unsigned>();
		r._callsign.sti         = TIS_UNDEFINED;
		r._callsign.callsign[0] = getIa5(id >> 42);
		r._callsign.callsign[1] = getIa5(id >> 36);
		r._callsign.callsign[2] = getIa5(id >> 30);
		r._callsign.callsign[3] = getIa5(id >> 24);
		r._callsign.callsign[4] = getIa5(id >> 18);
		r._callsign.callsign[5] = getIa5(id >> 12);
		r._callsign.callsign[6] = getIa5(id >> 6);
		r._callsign.callsign[7] = getIa5(id >> 0);
		r._callsign.callsign[8] = 0;
	}

	void reserved(Cat21& r)
	{
		AsterixReaderT::reserved(&r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat21&)
	{
		AsterixReaderT::reserved_end();
	}

	void special(Cat21& r)
	{
		AsterixReaderT::special(r._sp);
	}

	void rateOfTurn(Cat21&)
	{
		readFxFlags();
	}

	void metInformation(Cat21&)
	{
		uint8_t byte = readFxFlags();
		if (byte & (1 << 7))
		{
			readUint16();
		}
		if (byte & (1 << 6))
		{
			readUint16();
		}
		if (byte & (1 << 5))
		{
			readUint16();
		}
		if (byte & (1 << 4))
		{
			readUint8();
		}
	}

	void trajectoryIntent(Cat21&)
	{
		uint8_t mask = readFxFlags();
		if (mask & (1 << 7))
		{
			readFxFlags();
		}
		if (mask & (1 << 6))
		{
			uint8_t count = readUint8();
			readUint8();
			for (int intent = 0; intent < count; ++intent)
			{
				readInt16();
				readInt32();
				readInt32();
				readInt8();
				readUint8();
				readUint8();
				readUint8();
				readUint16();
			}
		}
	}

	void byteSkip(Cat21&)
	{
		readUint8();
	}

};


AsterixRecord* Cat21::decode(const char* data, size_t size, AsterixError& err, size_t& read_size)
{
	if (size == 0)
	{
		err = A_EMPTY_BUFFER;
		return NULL;
	}
	Reader reader(data, size);

	Cat21* rec = new Cat21;
	reader.readRecord(*rec);
	if (reader.have_error())
	{
		err       = A_DECODE_WRONG_SIZE;
		read_size = size;
		return 0;
	}
	else
	{
		err       = A_NO_ERROR;
		read_size = reader.bytesRead();
		return rec;
	}
}

}                                                          // namespace Ver026

}                                                          // namespace Cat21
