#include "asterix_cat_20.hpp"
#include "asterix_template.hpp"
#include "math.h"

const Ratio<1, 1> ID_LSB;

const Ratio<180, 1 << 25> WGS84_LSB;
const Ratio<180, 1 << 25> SDW_LSB;

const Ratio<1, 128>            TIME_LSB;
const Ratio<1, 2>              CART_LSB;
const Ratio<1, 4>              VEL_LSB;
const Ratio<1, 4>              ACCEL_LSB;
const Ratio<1, 4>              FLIGHT_LEVEL_LSB;
const Ratio<25* 3048, 4*10000> ALTITUDE_LSB;

const Ratio<1, 4>        DOP_LSB;
const Ratio<1, 4>        SDP_LSB;
const Ratio<1, 2>        SDH_LSB_M;
const Ratio<3048, 10000> SDH_LSB_FT;

const Ratio<1852, 3600>    KNOT;
const Ratio<1852, 1>       NM;
const Ratio<1852, 1 << 14> NM_14;
const Ratio<360, 1 << 16>  DEG_360_16;
const Ratio<360, 1 << 12>  DEG_360_12;
const Ratio<1, 10>         AGE_LSB;

namespace Cat20
{
template <> void Cat20::Uap::Serializer::skip<0>(Cat20&) {};

const Cat20::Uap Cat20::UAP[Cat20::UAP_SIZE] =
{
	ASTERIX_UAP_FIELD(Cat20,        0,          0, NULL,                                                0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat20,        1,         10, UAP_NAME("Data Source Identifier"),                  2, dataSourceId),
	ASTERIX_UAP_FIELD(Cat20,        2,         20, UAP_NAME("Target Report Descriptor"),                0, targetReportDescriptor),
	ASTERIX_UAP_FIELD(Cat20,        3,        140, UAP_NAME("Time of Day"),                             3, timeOfDay),
	ASTERIX_UAP_FIELD(Cat20,        4,         41, UAP_NAME("Position in WGS-84 Coordinates"),          8, wgs84),
	ASTERIX_UAP_FIELD(Cat20,        5,         42, UAP_NAME("Position in Cartesian Coordinates"),       6, cartCoords),
	ASTERIX_UAP_FIELD(Cat20,        6,        161, UAP_NAME("Track Number"),                            2, trackNumber),
	ASTERIX_UAP_FIELD(Cat20,        7,        170, UAP_NAME("Track Status"),                            0, trackStatus),
	ASTERIX_UAP_FIELD(Cat20,        8,         70, UAP_NAME("Mode-3/A Code"),                           2, mode3A),
	ASTERIX_UAP_FIELD(Cat20,        9,        202, UAP_NAME("Velocity in Cartesian Coord"),             4, velocity),
	ASTERIX_UAP_FIELD(Cat20,       10,         90, UAP_NAME("Flight Level"),                            2, flightLevel),
	ASTERIX_UAP_FIELD(Cat20,       11,        100, UAP_NAME("Mode-C Code"),                             4, modeC),
	ASTERIX_UAP_FIELD(Cat20,       12,        220, UAP_NAME("Target Address"),                          3, targetAddress),
	ASTERIX_UAP_FIELD(Cat20,       13,        245, UAP_NAME("Target Identification"),                   7, targetId),
	ASTERIX_UAP_FIELD(Cat20,       14,        110, UAP_NAME("Measured Height"),                         2, measuredHeight),
	ASTERIX_UAP_FIELD(Cat20,       15,        105, UAP_NAME("Geometric Height"),                        2, geometricAltitude),
	ASTERIX_UAP_FIELD(Cat20,       16,        210, UAP_NAME("Acceleration"),                            2, acceleration),
	ASTERIX_UAP_FIELD(Cat20,       17,        300, UAP_NAME("Vehicle Fleet Identification"),            1, fleetId),
	ASTERIX_UAP_FIELD(Cat20,       18,        310, UAP_NAME("Pre-programmed Message"),                  1, programmedMessage),
	ASTERIX_UAP_FIELD(Cat20,       19,        500, UAP_NAME("Position Accuracy"),                       0, accuracy),
	ASTERIX_UAP_FIELD(Cat20,       20,        400, UAP_NAME("Contributing Devices"),                    0, contributions),
	ASTERIX_UAP_FIELD(Cat20,       21,        250, UAP_NAME("Mode S MB Data"),                          0, modeS),
	ASTERIX_UAP_FIELD(Cat20,       22,        230, UAP_NAME("Comms/ACAS Capability and Flight Status"), 2, commsAcas),
	ASTERIX_UAP_FIELD(Cat20,       23,        260, UAP_NAME("ACAS Resolution Advisory Report"),         7, acas),
	ASTERIX_UAP_FIELD(Cat20,       24,         30, UAP_NAME("Warning/Error Conditions"),                0, warning),
	ASTERIX_UAP_FIELD(Cat20,       25,         55, UAP_NAME("Mode-1 Code"),                             1, mode1),
	ASTERIX_UAP_FIELD(Cat20,       26,         50, UAP_NAME("Mode-2 Code"),                             2, mode2),
	ASTERIX_UAP_FIELD(Cat20,       27,          0, UAP_NAME("Reserved Expansion Field"),                0, reserved),
	ASTERIX_UAP_FIELD(Cat20,       28,          0, UAP_NAME("Special Purpose Field"),                   0, special),

	ASTERIX_UAP_FIELD(Cat20, LAST_FRN + 0,      0, UAP_NAME("Position Accuracy (RE)"),                  0, re_accuracy),
	ASTERIX_UAP_FIELD(Cat20, LAST_FRN + 1,      0, UAP_NAME("Ground Velocity Vector"),                  4, groundVector),
	ASTERIX_UAP_FIELD(Cat20, LAST_FRN + 2,      0, UAP_NAME("Ground Velocity Accuracy"),                2, groundVectorAccuracy),
	ASTERIX_UAP_FIELD(Cat20, LAST_FRN + 3,      0, UAP_NAME("Time of Report Transmission"),             3, timeOfReport),
	ASTERIX_UAP_FIELD(Cat20, LAST_FRN + 4,      0, UAP_NAME("Data Ages"),                               0, ages),
	ASTERIX_UAP_FIELD(Cat20, LAST_FRN + RE_END, 0, NULL,                                                0, reserved_end),
};


struct Cat20::Reader :
	public Uap::Serializer, public AsterixReaderT
{
public:

	Reader(const char* data, size_t size) :
		AsterixReaderT(data, size) {};
	void readRecord(Cat20& v)
	{
		Asterix::readRecord(v, this);
	}

private:

	void dataSourceId(Cat20& r)
	{
		r._dsi.sac = readInt<8, unsigned>();
		r._dsi.sic = readInt<8, unsigned>();
	}

	void targetReportDescriptor(Cat20& r)
	{
		uint64_t                flags = readFxFlags();
		TargetReportDescriptor& t     = r._target_report_dscr;

		t.ssr  = flags & (1 << 7);
		t.ms   = flags & (1 << 6);
		t.hf   = flags & (1 << 5);
		t.vdl4 = flags & (1 << 4);
		t.uat  = flags & (1 << 3);
		t.mde  = flags & (1 << 2);
		t.ot   = flags & (1 << 1);

		t.rab = ReportSource(!!(flags & (1 << 15)));
		t.spi = flags & (1 << 14);
		t.chn = flags & (1 << 13);
		t.gbs = flags & (1 << 12);
		t.crt = flags & (1 << 11);
		t.sim = flags & (1 << 10);
		t.tst = flags & (1 << 9);
	}

	void timeOfDay(Cat20& r)
	{
		r._time = readDouble<24, unsigned>(TIME_LSB);
	}

	void wgs84(Cat20& r)
	{
		r._position.wgs84_lat = readDouble<32>(WGS84_LSB);
		r._position.wgs84_lon = readDouble<32>(WGS84_LSB);
	}

	void cartCoords(Cat20& r)
	{
		r._cart_pos.x = readDouble<24>(CART_LSB);
		r._cart_pos.y = readDouble<24>(CART_LSB);
	}

	void trackNumber(Cat20& r)
	{
		r._track_number  = readInt<16, unsigned>();
		r._track_number &= 0x0FFF;
	}

	void trackStatus(Cat20& r)
	{
		uint64_t flags = readFxFlags();
		r._track_status.init         = flags & (1 << 7);
		r._track_status.last         = flags & (1 << 6);
		r._track_status.extrapolated = flags & (1 << 5);
		r._track_status.state        = TrackStatusState((flags >> 3) & 0x3);
		r._track_status.horiz_man    = flags & (1 << 2);
		r._track_status.smoothed     = flags & (1 << 1);
		r._track_status.ghost        = flags & (1 << 15);
	}

	void accuracy_common(PositionAccuracy& a)
	{
		uint8_t flags = readInt<8, unsigned>();

		bool have_dop = flags & (1 << 7);
		bool have_sdp = flags & (1 << 6);
		bool have_sdh = flags & (1 << 5);
		bool have_sdw = flags & (1 << 4);

		if (have_dop)
		{
			a.dop_x  = readDouble<16, unsigned>(DOP_LSB);
			a.dop_y  = readDouble<16, unsigned>(DOP_LSB);
			a.dop_xy = readDouble<16, signed>(DOP_LSB);
		}
		else
			a.dop_x = a.dop_y = a.dop_xy = __noop;	//	NAN;

		if (have_sdp)
		{
			a.sdp_x  = readDouble<16, unsigned>(SDP_LSB);
			a.sdp_y  = readDouble<16, unsigned>(SDP_LSB);
			a.sdp_xy = readDouble<16, signed>(SDP_LSB);
		}
		else
			a.sdp_x = a.sdp_y = a.sdp_xy = __noop;		// NAN;

		if (have_sdh)
			a.sdh = readDouble<16, unsigned>(1);
		else
			a.sdh = __noop;	// NAN;

		if (have_sdw)
		{
			a.sdw_lat    = readDouble<16, unsigned>(SDW_LSB);
			a.sdw_lon    = readDouble<16, unsigned>(SDW_LSB);
			a.sdw_latlon = readDouble<16, signed>(SDW_LSB);
		}
		else
			a.sdw_lat = a.sdw_lon = a.sdw_latlon = __noop;	// NAN;
	}

	void accuracy(Cat20& r)
	{
		PositionAccuracy& a = r._position_accuracy;
		accuracy_common(a);
		a.sdh *= SDH_LSB_M;
	}

	void re_accuracy(Cat20& r)
	{
		PositionAccuracy& a = r._re_accuracy;
		accuracy_common(a);
		a.sdh *= SDH_LSB_FT;
	}

	void contributions(Cat20& r)
	{
		uint8_t count = readInt<8, unsigned>();
		r._contributing_devices = 0;
		for (int i = 0; i < count; ++i)
		{
			uint8_t mask = readInt<8, unsigned>();
			r._contributing_devices <<= 8;
			r._contributing_devices  |= uint64_t(mask);
		}
	}

	void modeS(Cat20& r)
	{
		uint8_t count = readInt<8, unsigned>();
		while (count--)
		{
			uint64_t data = readInt<64, unsigned>();
			r._mode_s.push_back(ModeS());
			struct ModeS& s = r._mode_s.back();
			s.data = data >> 8;
			s.bds  = data & 0xFF;
		}
	}

	void warning(Cat20& r)
	{
		uint8_t warn;
		do
		{
			warn = readInt<8, unsigned>();
			r._warning_errors.push_back(warn >> 1);
		}
		while (warn & 0x1);
	}

	void special(Cat20& r)
	{
		AsterixReaderT::special(r._sp);
	}

	void geometricAltitude(Cat20& r)
	{
		r._geometric_altitude = readDouble<16>(ALTITUDE_LSB);
	}

	void targetAddress(Cat20& r)
	{
		r._target_address = readInt<24, unsigned>();
	}

	void mode3A(Cat20& r)
	{
		uint16_t mode = readInt<16, unsigned>();
		r._mode_3a.code = mode & 0xFFF;

		r._mode_3a.flags.not_validated = mode & (1 << 15);
		r._mode_3a.flags.garbled       = mode & (1 << 14);
		r._mode_3a.flags.not_extracted = mode & (1 << 13);
	}

	void flightLevel(Cat20& r)
	{
		uint16_t level   = readInt<16, unsigned>();
		int16_t  level_i = int16_t(level << 2) >> 2;
		r._flight_level.flags.not_validated = level & (1 << 15);
		r._flight_level.flags.garbled       = level & (1 << 14);
		r._flight_level.level_fl            = level_i * double(FLIGHT_LEVEL_LSB);
	}

	void measuredHeight(Cat20& r)
	{
		r._measured_altitude = readDouble<16>(ALTITUDE_LSB);
	}

	void velocity(Cat20& r)
	{
		r._velocity.vx = readDouble<16>(VEL_LSB);
		r._velocity.vy = readDouble<16>(VEL_LSB);
	}

	void acceleration(Cat20& r)
	{
		r._accel.ax = readDouble<8>(ACCEL_LSB);
		r._accel.ay = readDouble<8>(ACCEL_LSB);
	}

	void targetId(Cat20& r)
	{
		uint8_t sti = readInt<8, unsigned>();
		r._callsign.sti = TargetIdentificationSource(sti >> 6);

		uint64_t id = readInt<48, unsigned>();
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

	void fleetId(Cat20& r)
	{
		r._fleet_type = readEnum<8, FleetType>();
	}

	void modeC(Cat20& r)
	{
		r._mode_c.code                = readInt<16, unsigned>();
		r._mode_c.confidence          = readInt<16, unsigned>();
		r._mode_c.flags.not_validated = r._mode_c.code & (1 << 15);
		r._mode_c.flags.garbled       = r._mode_c.code & (1 << 14);
		r._mode_c.code               &= 0xFFF;
		r._mode_c.confidence         &= 0xFFF;
	}

	void programmedMessage(Cat20& r)
	{
		uint8_t flags = readInt<8, unsigned>();
		r._proggramed_message.trouble = flags & (1 << 7);
		r._proggramed_message.type    = flags & 0x7F;
	}

	void commsAcas(Cat20& r)
	{
		uint16_t  flags = readInt<16, unsigned>();
		CommAcas& a     = r._comm_acas;
		a.com                 = ComCapability((flags >> 13) & 0x7);
		a.status              = CommAcasFlightStatus((flags >> 10) & 0x7);
		a.mode_s_specific_cap = (flags >> 8) & 0x1;
		a.arc                 = CommAcasAltitudeCap((flags >> 7) & 0x1);
		a.aic                 = (flags >> 6) & 0x1;
		a.b1a                 = (flags >> 4) & 0x1;
		a.b1b                 = (flags >> 0) & 0xF;
	}

	void acas(Cat20& r)
	{
		uint64_t data = readInt<56, unsigned>();
		AcasRA&  a    = r._acas;
		a.tid  = data & 0x3FFFFFFU;
		data >>= 26;
		a.tti  = data & 0x03;
		a.mte  = data & 0x04;
		a.rat  = data & 0x08;
		a.rac  = (data >> 4) & 0x0F;
		data >>= 6;
		a.ara  = data & 0x3FFF;
		data >>= 14;
		a.styp = data & 0x07;
		a.typ  = (data >> 3) & 0x1F;
	}

	void mode1(Cat20& r)
	{
		uint8_t flags = readInt<8, unsigned>();
		r._mode_1.flags.not_validated = flags & (1 << 7);
		r._mode_1.flags.garbled       = flags & (1 << 6);
		r._mode_1.flags.not_extracted = flags & (1 << 5);
		r._mode_1.code                = (((flags >> 2) & 0x7) << 3) + (flags & 0x3);
	}

	void mode2(Cat20& r)
	{
		uint16_t mode = readInt<16, unsigned>();
		r._mode_2.code = mode & 0x0FFF;

		r._mode_2.flags.not_validated = mode & (1 << 15);
		r._mode_2.flags.garbled       = mode & (1 << 14);
		r._mode_2.flags.not_extracted = mode & (1 << 13);
	}

	void groundVector(Cat20& r)
	{
		GroundVector& v       = r._ground_vector;
		uint16_t      speed_i = readInt<16, unsigned>();
		v.range_exceeded = speed_i & 1 << 15;
		speed_i         &= ~(1 << 15);
		v.speed          = speed_i * NM_14;
		v.heading        = readDouble<16, unsigned>(DEG_360_16);
	}

	void groundVectorAccuracy(Cat20& r)
	{
		r._ground_vel_accuracy.gssd = readDouble<8, unsigned>(NM_14);
		r._ground_vel_accuracy.tasd = readDouble<8, unsigned>(DEG_360_12);
	}

	void timeOfReport(Cat20& r)
	{
		r._report_time = readDouble<24, unsigned>(TIME_LSB);
	}

	void ages(Cat20& r)
	{
		uint64_t data = readFxFlags();
#define read_age(i, v) \
	r._ages.v = (data & (1 << (i - 1))) ? readDouble<8, unsigned>(AGE_LSB) : __noop;	// NAN; for Linux

		read_age(24, spi);
		read_age(23,  ti);
		read_age(22,  mb);
		read_age(21, m3a);
		read_age(20,  fl);
		read_age(19,  fs);
		read_age(18,  gh);

		read_age(16,  ta);
		read_age(15,  mc);
		read_age(14, mss);
		read_age(13, arc);
		read_age(12, aic);
		read_age(11,  m2);
		read_age(10,  m1);

		read_age(8,  ara);
		read_age(7,   vi);
		read_age(6,  msg);
#undef read_age
	}

	void reserved(Cat20& r)
	{
		AsterixReaderT::reserved(&r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat20&)
	{
		AsterixReaderT::reserved_end();
	}

};

struct Cat20::Writer :
	public Uap::Serializer, public AsterixWriterT
{
public:

	Writer(char*& data, size_t& size) :
		AsterixWriterT(data, size) {};

	void writeRecord(const Cat20& v)
	{
		Asterix::writeRecord(v, this);
	}

private:

	void dataSourceId(Cat20& r)
	{
		writeInt<8, unsigned>(r._dsi.sac);
		writeInt<8, unsigned>(r._dsi.sic);
	}

	void targetReportDescriptor(Cat20& r)
	{
		uint64_t                flags = 0;
		TargetReportDescriptor& t     = r._target_report_dscr;

		flags |= t.ssr << 7;
		flags |= t.ms << 6;
		flags |= t.hf << 5;
		flags |= t.vdl4 << 4;
		flags |= t.uat << 3;
		flags |= t.mde << 2;
		flags |= t.ot << 1;

		flags |= t.rab << 15;
		flags |= t.spi << 14;
		flags |= t.chn << 13;
		flags |= t.gbs << 12;
		flags |= t.crt << 11;
		flags |= t.sim << 10;
		flags |= t.tst << 9;

		writeFxFlags(flags);
	}

	void timeOfDay(Cat20& r)
	{
		writeDouble<24, unsigned>(TIME_LSB, r._time);
	}

	void wgs84(Cat20& r)
	{
		writeDouble<32>(WGS84_LSB, r._position.wgs84_lat);
		writeDouble<32>(WGS84_LSB, r._position.wgs84_lon);
	}

	void cartCoords(Cat20& r)
	{
		writeDouble<24>(CART_LSB, r._cart_pos.x);
		writeDouble<24>(CART_LSB, r._cart_pos.y);
	}

	void trackNumber(Cat20& r)
	{
		writeInt<16, unsigned>(r._track_number & 0x0FFF);
	}

	void trackStatus(Cat20& r)
	{
		uint64_t flags = 0;
		flags |= r._track_status.init << 7;
		flags |= r._track_status.last << 6;
		flags |= r._track_status.extrapolated << 5;
		flags |= r._track_status.state << 3;
		flags |= r._track_status.horiz_man << 2;
		flags |= r._track_status.smoothed << 1;
		flags |= r._track_status.ghost << 15;
		writeFxFlags(flags);
	}

	void accuracy_common(PositionAccuracy& a)
	{
		/*bool have_dop = isfinite(a.dop_x) && isfinite(a.dop_y) && isfinite(a.dop_xy);	// for Linux
		bool have_sdp = isfinite(a.sdp_x) && isfinite(a.sdp_y) && isfinite(a.sdp_xy);
		bool have_sdh = isfinite(a.sdh);
		bool have_sdw = isfinite(a.sdw_lat) && isfinite(a.sdw_lon) && isfinite(a.sdw_latlon);

		uint8_t flags = 0;
		flags |= have_dop << 7;
		flags |= have_sdp << 6;
		flags |= have_sdh << 5;
		flags |= have_sdw << 4;
		writeInt<8, unsigned>(flags);

		if (have_dop)
		{
			writeDouble<16, unsigned>(DOP_LSB, a.dop_x);
			writeDouble<16, unsigned>(DOP_LSB, a.dop_y);
			writeDouble<16, signed>(  DOP_LSB, a.dop_xy);
		}
		if (have_sdp)
		{
			writeDouble<16, unsigned>(SDP_LSB, a.sdp_x);
			writeDouble<16, unsigned>(SDP_LSB, a.sdp_y);
			writeDouble<16, signed>(  SDP_LSB, a.sdp_xy);
		}
		if (have_sdh)
		{
			writeDouble<16, unsigned>(ID_LSB, a.sdh);
		}
		if (have_sdw)
		{
			writeDouble<16, unsigned>(SDW_LSB, a.sdw_lat);
			writeDouble<16, unsigned>(SDW_LSB, a.sdw_lon);
			writeDouble<16, signed>(  SDW_LSB, a.sdw_latlon);
		}*/
	}

	void accuracy(Cat20& r)
	{
		PositionAccuracy a = r._position_accuracy;
		a.sdw_lat = a.sdw_lon = a.sdw_latlon = __noop;		// NAN;
		a.sdh    /= SDH_LSB_M;
		accuracy_common(a);
	}

	void re_accuracy(Cat20& r)
	{
		PositionAccuracy a = r._re_accuracy;
		a.sdh /= SDH_LSB_FT;
		accuracy_common(a);
	}

	void contributions(Cat20& r)
	{
		//// Поиск старшего байта	// for Linux
		//uint8_t count = sizeof(r._contributing_devices) - __builtin_clzll(r._contributing_devices) / 8;
		//writeInt<8, unsigned>(count);
		//for (int i = 0; i < count; ++i)
		//	writeInt<8, unsigned>(r._contributing_devices >> (8 * (count - i - 1)));
	}

	void modeS(Cat20& r)
	{
		uint8_t count = r._mode_s.size();
		writeInt<8, unsigned>(count);
		for (size_t i = 0; i != count; ++i)
		{
			struct ModeS& s = r._mode_s[i];
			writeInt<64, unsigned>(s.data << 8 | s.bds);
		}
	}

	void warning(Cat20& r)
	{
		if (r._warning_errors.empty())
			writeInt<8, unsigned>(0);
		for (size_t i = 0, end = r._warning_errors.size(); i != end; ++i)
			writeInt<8, unsigned>(r._warning_errors[i] << 1 | (i != end - 1));
	}

	void special(Cat20& r)
	{
		AsterixWriterT::special(r._sp);
	}

	void geometricAltitude(Cat20& r)
	{
		writeDouble<16>(ALTITUDE_LSB, r._geometric_altitude);
	}

	void targetAddress(Cat20& r)
	{
		writeInt<24, unsigned>(r._target_address);
	}

	void mode3A(Cat20& r)
	{
		uint16_t mode = r._mode_3a.code;

		mode |= r._mode_3a.flags.not_validated << 15;
		mode |= r._mode_3a.flags.garbled << 14;
		mode |= r._mode_3a.flags.not_extracted << 13;

		writeInt<16, unsigned>(mode);
	}

	void flightLevel(Cat20& r)
	{
		uint16_t level_i = int16_t(r._flight_level.level_fl / double(FLIGHT_LEVEL_LSB));
		level_i &= 0x3FFF;
		level_i |= r._flight_level.flags.not_validated << 15;
		level_i |= r._flight_level.flags.garbled << 14;
		writeInt<16, unsigned>(level_i);
	}

	void measuredHeight(Cat20& r)
	{
		writeDouble<16>(ALTITUDE_LSB, r._measured_altitude);
	}

	void velocity(Cat20& r)
	{
		writeDouble<16>(VEL_LSB, r._velocity.vx);
		writeDouble<16>(VEL_LSB, r._velocity.vy);
	}

	void acceleration(Cat20& r)
	{
		writeDouble<8>(ACCEL_LSB, r._accel.ax);
		writeDouble<8>(ACCEL_LSB, r._accel.ay);
	}

	void targetId(Cat20& r)
	{
		writeInt<8, unsigned>(r._callsign.sti << 6);

		uint64_t id = 0;
		id |= uint64_t(getIa5(r._callsign.callsign[0])) << 42;
		id |= uint64_t(getIa5(r._callsign.callsign[1])) << 36;
		id |= uint64_t(getIa5(r._callsign.callsign[2])) << 30;
		id |= uint64_t(getIa5(r._callsign.callsign[3])) << 24;
		id |= uint64_t(getIa5(r._callsign.callsign[4])) << 18;
		id |= uint64_t(getIa5(r._callsign.callsign[5])) << 12;
		id |= uint64_t(getIa5(r._callsign.callsign[6])) << 6;
		id |= uint64_t(getIa5(r._callsign.callsign[7])) << 0;
		writeInt<48, unsigned>(id);
	}

	void fleetId(Cat20& r)
	{
		writeInt<8, unsigned>(r._fleet_type);
	}

	void modeC(Cat20& r)
	{
		uint16_t code = r._mode_c.code & 0xFFF;
		code |= r._mode_c.flags.not_validated << 15;
		code |= r._mode_c.flags.garbled << 14;
		writeInt<16, unsigned>(code);
		writeInt<16, unsigned>(r._mode_c.confidence & 0xFFF);
	}

	void programmedMessage(Cat20& r)
	{
		uint8_t flags = r._proggramed_message.type & 0x7F;
		flags |= r._proggramed_message.trouble << 7;
		writeInt<8, unsigned>(flags);
	}

	void commsAcas(Cat20& r)
	{
		uint16_t  flags = 0;
		CommAcas& a     = r._comm_acas;
		flags |= (a.com                 & 0x7) << 13;
		flags |= (a.status              & 0x7) << 10;
		flags |= (a.mode_s_specific_cap & 0x1) << 8;
		flags |= (a.arc                 & 0x1) << 7;
		flags |= (a.aic                 & 0x1) << 6;
		flags |= (a.b1a                 & 0x1) << 4;
		flags |= (a.b1b                 & 0xF) << 0;
		writeInt<16, unsigned>(flags);
	}

	void acas(Cat20& r)
	{
		uint64_t data = 0;
		AcasRA&  a    = r._acas;

		data  |= a.styp;
		data  |= a.typ << 3;
		data <<= 14;
		data  |= a.ara;
		data <<= 6;

		data |= a.tti;
		data |= a.mte << 2;
		data |= a.rat << 3;
		data |= a.rac << 4;

		data <<= 26;
		data  |= a.tid;

		writeInt<56, unsigned>(data);
	}

	void mode1(Cat20& r)
	{
		uint8_t flags = 0;
		flags |= r._mode_1.flags.not_validated << 7;
		flags |= r._mode_1.flags.garbled << 6;
		flags |= r._mode_1.flags.not_extracted << 5;
		flags |= ((r._mode_1.code >> 3) & 0x7) << 2;
		flags |= r._mode_1.code & 0x3;
		writeInt<8, unsigned>(flags);
	}

	void mode2(Cat20& r)
	{
		uint16_t mode = r._mode_2.code & 0x0FFF;

		mode |= r._mode_2.flags.not_validated << 15;
		mode |= r._mode_2.flags.garbled << 14;
		mode |= r._mode_2.flags.not_extracted << 13;

		writeInt<16, unsigned>(mode);
	}

	void groundVector(Cat20& r)
	{
		GroundVector& v       = r._ground_vector;
		uint16_t      speed_i = 0;
		if (v.speed < 2 * NM && !v.range_exceeded)
		{
			speed_i  = v.speed / NM_14;
			speed_i &= ~(1 << 15);
		}
		else
		{
			speed_i  = 2 * NM / NM_14 - 1;
			speed_i |= 1 << 15;
		}
		writeInt<16, unsigned>(speed_i);
		writeDouble<16, unsigned>(DEG_360_16, v.heading);
	}

	void groundVectorAccuracy(Cat20& r)
	{
		writeDouble<8, unsigned>(NM_14,      r._ground_vel_accuracy.gssd);
		writeDouble<8, unsigned>(DEG_360_12, r._ground_vel_accuracy.tasd);
	}

	void timeOfReport(Cat20& r)
	{
		writeDouble<24, unsigned>(TIME_LSB, r._report_time);
	}

	void ages(Cat20& r)
	{
		/*uint64_t data = 0;	// for Linux
#define make_flags(i, v) \
	data |= !!finite(r._ages.v) << (i - 1);
		make_flags(24, spi);
		make_flags(23,  ti);
		make_flags(22,  mb);
		make_flags(21, m3a);
		make_flags(20,  fl);
		make_flags(19,  fs);
		make_flags(18,  gh);

		make_flags(16,  ta);
		make_flags(15,  mc);
		make_flags(14, mss);
		make_flags(13, arc);
		make_flags(12, aic);
		make_flags(11,  m2);
		make_flags(10,  m1);

		make_flags(8,  ara);
		make_flags(7,   vi);
		make_flags(6,  msg);

#undef make_flags
		writeFxFlags(data);
#define write_ages(i, v) \
	if (data & (1 << (i - 1))) writeDouble<8, unsigned>(AGE_LSB, r._ages.v);
		write_ages(24, spi);
		write_ages(23,  ti);
		write_ages(22,  mb);
		write_ages(21, m3a);
		write_ages(20,  fl);
		write_ages(19,  fs);
		write_ages(18,  gh);

		write_ages(16,  ta);
		write_ages(15,  mc);
		write_ages(14, mss);
		write_ages(13, arc);
		write_ages(12, aic);
		write_ages(11,  m2);
		write_ages(10,  m1);

		write_ages(8,  ara);
		write_ages(7,   vi);
		write_ages(6,  msg);
#undef write_ages*/
	}

	void reserved(Cat20& r)
	{
		// AsterixWriterT::reserved(r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat20&)
	{
		AsterixWriterT::reserved_end();
	}

};

AsterixRecord* Cat20::decode(const char* data, size_t size, AsterixError& err, size_t& read_size)
{
	if (size == 0)
	{
		err = A_EMPTY_BUFFER;
		return NULL;
	}
	Reader reader(data, size);

	Cat20* rec = new Cat20;
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

char* Cat20::code(const AsterixRecord* record, size_t& write_size)
{
	if (record->category != 20)
		return NULL;
	char* data;
	{
		Writer writer(data, write_size);
		writer.writeRecord(*(const Cat20*) record);
	}
	return data;
}

}                                                          // namespace Cat20
