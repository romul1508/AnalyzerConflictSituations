#include "asterix_cat_21.hpp"
#include "asterix_template.hpp"
#include <math.h>

static const Ratio<1, 1>                ID_LSB;
static const Ratio<1, 128>              TIME_LSB;
static const Ratio<1, 1 << 30>          TIME_HIGH_LSB;
static const Ratio<25* 3048, 4* 10000>  ALTITUDE_LSB;
static const Ratio<360, 1 << 16>        DEG_360_16;
static const Ratio<1852, 1 << 14>       NM_14;
static const Ratio<1852, 3600>          KNOT;
static const Ratio<1, 4>                FLIGHT_LEVEL_LSB;
static const Ratio<3048*25, 4*60*10000> FOOT_MIN_LSB;
static const Ratio<1, 100>              DEG_100;
static const Ratio<1, 32>               DEG_32;
static const Ratio<1, 4>                TEMP_LSB;
static const Ratio<10, 1>               FOOT_10;
static const Ratio<762, 100>            FOOT_25_M;
static const Ratio<1, 2>                SERVICE_TIME;
static const Ratio<1852, 100>           TURN_LSB;
static const Ratio<1, 10>               AGE_LSB;
static const Ratio<1, 10>               BPS_LSB;
static const Ratio<360, 512>            SEL_HEADING_LSB;
static const Ratio<1852, 3600* 8>       SURF_SPEED_LSB;
static const Ratio<360, 128>            SURF_HEADING_LSB;
static const Ratio<180, 1 << 23>        WGS84_LSB;
static const Ratio<180, 1 << 30>        WGS84_LSB_HIGH;

namespace Cat21
{
template <> void Cat21::Uap::Serializer::skip<0>(Cat21&) {};

const Cat21::Uap Cat21::UAP[Cat21::UAP_SIZE] =
{
	ASTERIX_UAP_FIELD( Cat21,        0,          0, NULL,                                                              0, skip<0> ),
	ASTERIX_UAP_FIELD( Cat21,        1,         10, UAP_NAME("Data Source Identification"),                            2, dataSourceId),
	ASTERIX_UAP_FIELD( Cat21,        2,         40, UAP_NAME("Target Report Descriptor"),                              0, targetReportDescriptor),
	ASTERIX_UAP_FIELD( Cat21,        3,        161, UAP_NAME("Track Number"),                                          2, trackNumber),
	ASTERIX_UAP_FIELD( Cat21,        4,         15, UAP_NAME("Service Identification"),                                1, service),
	ASTERIX_UAP_FIELD( Cat21,        5,         71, UAP_NAME("Time of Applicability for Position"),                    3, timeApplicPos),
	ASTERIX_UAP_FIELD( Cat21,        6,        130, UAP_NAME("Position in WGS-84 Coordinates"),                        6, wgs84),
	ASTERIX_UAP_FIELD( Cat21,        7,        131, UAP_NAME("Position in WGS-84 Coordinates, High resolution"),       8, wgs84high),
	ASTERIX_UAP_FIELD( Cat21,        8,         72, UAP_NAME("Time of Applicability for Velocity"),                    3, timeApplicVel),
	ASTERIX_UAP_FIELD( Cat21,        9,        150, UAP_NAME("Air Speed"),                                             2, airSpeed),
	ASTERIX_UAP_FIELD( Cat21,       10,        151, UAP_NAME("True Air Speed"),                                        2, trueAirSpeed),
	ASTERIX_UAP_FIELD( Cat21,       11,         80, UAP_NAME("Target Address"),                                        3, targetAddress),
	ASTERIX_UAP_FIELD( Cat21,       12,         73, UAP_NAME("Time of Message Reception of Position"),                 3, timeOfPos),
	ASTERIX_UAP_FIELD( Cat21,       13,         74, UAP_NAME("Time of Message Reception of Position, High Precision"), 4, timeOfPosHigh),
	ASTERIX_UAP_FIELD( Cat21,       14,         75, UAP_NAME("Time of Message Reception of Velocity"),                 3, timeOfVel),
	ASTERIX_UAP_FIELD( Cat21,       15,         76, UAP_NAME("Time of Message Reception of Velocity, High Precision"), 4, timeOfVelHigh),
	ASTERIX_UAP_FIELD( Cat21,       16,        140, UAP_NAME("Geometric Height"),                                      2, geometricAltitude),
	ASTERIX_UAP_FIELD( Cat21,       17,         90, UAP_NAME("Quality Indicators"),                                    0, quality),
	ASTERIX_UAP_FIELD( Cat21,       18,        210, UAP_NAME("MOPS Version"),                                          1, mops),
	ASTERIX_UAP_FIELD( Cat21,       19,         70, UAP_NAME("Mode-3/A Code"),                                         2, mode3A),
	ASTERIX_UAP_FIELD( Cat21,       20,        230, UAP_NAME("Roll Angle"),                                            2, rollAngle),
	ASTERIX_UAP_FIELD( Cat21,       21,        145, UAP_NAME("Flight Level"),                                          2, flightLevel),
	ASTERIX_UAP_FIELD( Cat21,       22,        152, UAP_NAME("Magnetic Heading"),                                      2, magHeading),
	ASTERIX_UAP_FIELD( Cat21,       23,        200, UAP_NAME("Target Status"),                                         1, targetStatus),
	ASTERIX_UAP_FIELD( Cat21,       24,        155, UAP_NAME("Barometric Vertical Rate"),                              2, baroVertSpeed),
	ASTERIX_UAP_FIELD( Cat21,       25,        157, UAP_NAME("Geometric Vertical Rate"),                               2, geoVertSpeed),
	ASTERIX_UAP_FIELD( Cat21,       26,        160, UAP_NAME("Airborne Ground Vector"),                                4, groundVector),
	ASTERIX_UAP_FIELD( Cat21,       27,        165, UAP_NAME("Track Angle Rate"),                                      2, turnRate),
	ASTERIX_UAP_FIELD( Cat21,       28,         77, UAP_NAME("Time of Report Transmission"),                           3, timeOfReport),
	ASTERIX_UAP_FIELD( Cat21,       29,        170, UAP_NAME("Target Identification"),                                 0, targetId),
	ASTERIX_UAP_FIELD( Cat21,       30,         20, UAP_NAME("Emitter Category"),                                      1, emitterCategory),
	ASTERIX_UAP_FIELD( Cat21,       31,        220, UAP_NAME("Meteorological Information"),                            0, metInformation),
	ASTERIX_UAP_FIELD( Cat21,       32,        146, UAP_NAME("Selected Altitude"),                                     2, selectedAlt),
	ASTERIX_UAP_FIELD( Cat21,       33,        148, UAP_NAME("Final State Selected Altitude"),                         2, finalSelectedAlt),
	ASTERIX_UAP_FIELD( Cat21,       34,        110, UAP_NAME("Trajectory Intent"),                                     0, trajectory),
	ASTERIX_UAP_FIELD( Cat21,       35,         16, UAP_NAME("Service Management"),                                    1, servicePeriod),
	ASTERIX_UAP_FIELD( Cat21,       36,          8, UAP_NAME("Aircraft Operational Status"),                           1, operationalStatus),
	ASTERIX_UAP_FIELD( Cat21,       37,        271, UAP_NAME("Surface Capabilities and Characteristics"),              0, surfCap),
	ASTERIX_UAP_FIELD( Cat21,       38,        132, UAP_NAME("Message Amplitude"),                                     1, amplitude),
	ASTERIX_UAP_FIELD( Cat21,       39,        250, UAP_NAME("Mode S MB Data"),                                        0, modeS),
	ASTERIX_UAP_FIELD( Cat21,       40,        260, UAP_NAME("ACAS Resolution Advisory Report"),                       7, acas),
	ASTERIX_UAP_FIELD( Cat21,       41,        400, UAP_NAME("Receiver ID"),                                           1, recvId),
	ASTERIX_UAP_FIELD( Cat21,       42,        295, UAP_NAME("Data Ages"),                                             0, ages),
	ASTERIX_UAP_FIELD( Cat21,       43,          0, NULL,                                                              0, skip<0> ),
	ASTERIX_UAP_FIELD( Cat21,       44,          0, NULL,                                                              0, skip<0> ),
	ASTERIX_UAP_FIELD( Cat21,       45,          0, NULL,                                                              0, skip<0> ),
	ASTERIX_UAP_FIELD( Cat21,       46,          0, NULL,                                                              0, skip<0> ),
	ASTERIX_UAP_FIELD( Cat21,       47,          0, NULL,                                                              0, skip<0> ),
	ASTERIX_UAP_FIELD( Cat21,       48,          0, UAP_NAME("Reserved Expansion Field"),                              0, reserved),
	ASTERIX_UAP_FIELD( Cat21,       49,          0, UAP_NAME("Special Purpose Field"),                                 0, special),

	ASTERIX_UAP_FIELD (Cat21, LAST_FRN + 0,      0, UAP_NAME("Barometric Pressure Setting"),                           2, bps),
	ASTERIX_UAP_FIELD (Cat21, LAST_FRN + 1,      0, UAP_NAME("Selected Heading"),                                      2, selHeading),
	ASTERIX_UAP_FIELD (Cat21, LAST_FRN + 2,      0, UAP_NAME("Navigation Mode"),                                       1, nav),
	ASTERIX_UAP_FIELD (Cat21, LAST_FRN + 3,      0, UAP_NAME("GPS Antenna Offset"),                                    1, gpsOffset),
	ASTERIX_UAP_FIELD (Cat21, LAST_FRN + 4,      0, UAP_NAME("Surface Ground Vector"),                                 0, surfVector),
	ASTERIX_UAP_FIELD (Cat21, LAST_FRN + 5,      0, UAP_NAME("Aircraft Status"),                                       0, aircraftStatus),
	ASTERIX_UAP_FIELD (Cat21, LAST_FRN + 6,      0, UAP_NAME("True North Heading"),                                    2, trueHeading),
	ASTERIX_UAP_FIELD (Cat21, LAST_FRN + 7,      0, UAP_NAME("Military Extended Squitter"),                            0, millitary),
	ASTERIX_UAP_FIELD( Cat21, LAST_FRN + RE_END, 0, NULL,                                                              0, reserved_end),
};

struct Cat21::Reader :
	public Cat21::Uap::Serializer, public AsterixReaderT
{
public:

	Reader(const char* data, size_t size) :
		AsterixReaderT(data, size) {};
	void readRecord(Cat21& v)
	{
		Asterix::readRecord(v, this);
	}

private:

	void dataSourceId(Cat21& r)
	{
		r._dsi.sac = readInt<8, unsigned>();
		r._dsi.sic = readInt<8, unsigned>();
	}

	void trackNumber(Cat21& r)
	{
		r._track_number  = readInt<16, unsigned>();
		r._track_number &= 0x0FFF;
	}

	void service(Cat21& r)
	{
		r._service = readInt<8, unsigned>();
	}

	void timeOfPos(Cat21& r)
	{
		r._pos_time = readDouble<24, unsigned>(TIME_LSB);
	}

	void timeOfVel(Cat21& r)
	{
		r._vel_time = readDouble<24, unsigned>(TIME_LSB);
	}

	void timeOfReport(Cat21& r)
	{
		r._report_time = readDouble<24, unsigned>(TIME_LSB);
	}

	void wgs84(Cat21& r)
	{
		r._position.wgs84_lat = readDouble<24>(WGS84_LSB);
		r._position.wgs84_lon = readDouble<24>(WGS84_LSB);
	}

	void wgs84high(Cat21& r)
	{
		r._position_high.wgs84_lat = readDouble<32>(WGS84_LSB_HIGH);
		r._position_high.wgs84_lon = readDouble<32>(WGS84_LSB_HIGH);
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

	void geometricAltitude(Cat21& r)
	{
		r._geometric_altitude = readDouble<16>(ALTITUDE_LSB);
	}

	void targetAddress(Cat21& r)
	{
		r._target_address = readInt<24, unsigned>();
	}

	void quality(Cat21& r)
	{
		uint64_t           flags = readFxFlags();
		QualityIndicators& q     = r._quality;

		q.nic   = flags >> 1  & 0xF;
		q.nac_v = flags >> 5  & 0xF;

		q.nac_p    = flags >> 9  & 0xF;
		q.sil      = flags >> 13 & 0x3;
		q.nic_baro = flags >> 15 & 1;

		q.sils = flags >> 21 & 1;
		q.sda  = flags >> 19 & 0x3;
		q.gva  = flags >> 17 & 0x3;

		q.pic = flags >> 28 & 0xF;
	}

	void mops(Cat21& r)
	{
		uint8_t data = readInt<8, unsigned>();
		r._mops.unsupported = !!(data & 0x3F);
		r._mops.version     = (data >> 3) & 0x07;
		r._mops.link        = Mops::Technology(data & 0x07);
	}

	void targetId(Cat21& r)
	{
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

	void magHeading(Cat21& r)
	{
		r._mag_heading = readDouble<16, unsigned>(DEG_360_16);
	}

	void targetStatus(Cat21& r)
	{
		uint8_t data = readInt<8, unsigned>();
		r._target_status.icf       = !!(data & 0x80);
		r._target_status.lnav      = !!(data & 0x40);
		r._target_status.me        = !!(data & 0x20);
		r._target_status.emergency = EmergencyStatus((data >> 2) & 0x07);
		r._target_status.status    = TargetStatus::Status(data & 0x03);
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
		uint16_t      speed_i = readInt<16, unsigned>();
		TrueAirSpeed& b       = r._true_air_speed;

		b.range_exceeded = speed_i & 1 << 15;
		speed_i         &= ~(1 << 15);
		b.speed          = speed_i * KNOT;
	}

	void groundVector(Cat21& r)
	{
		GroundVector& v       = r._ground_vector;
		uint16_t      speed_i = readInt<16, unsigned>();
		v.range_exceeded = speed_i & 1 << 15;
		speed_i         &= ~(1 << 15);
		v.speed          = speed_i * NM_14;
		v.heading        = readDouble<16, unsigned>(DEG_360_16);
	}

	void turnRate(Cat21& r)
	{
		uint16_t data = readInt<16, unsigned>();
		r._turn_rate = sign_extend<10>(data) * DEG_32;
	}

	void flightLevel(Cat21& r)
	{
		r._flight_level = readDouble<16>(FLIGHT_LEVEL_LSB);
	}

	void baroVertSpeed(Cat21& r)
	{
		int16_t       speed_i = readInt<16>();
		BaroVertRate& b       = r._baro_vert_speed;

		b.range_exceeded = speed_i & 1 << 15;
		speed_i          = sign_extend<15>(speed_i);
		b.speed          = speed_i * FOOT_MIN_LSB;
	}

	void geoVertSpeed(Cat21& r)
	{
		int16_t      speed_i = readInt<16>();
		GeoVertRate& b       = r._geo_vert_speed;

		b.range_exceeded = speed_i & 1 << 15;
		speed_i          = sign_extend<15>(speed_i);
		b.speed          = speed_i * FOOT_MIN_LSB;
	}

	void mode3A(Cat21& r)
	{
		uint16_t mode = readInt<16, unsigned>();
		r._mode_3a.code = mode & 0xFFF;

		r._mode_3a.flags.not_validated = mode & (1 << 15);
		r._mode_3a.flags.garbled       = mode & (1 << 14);
		r._mode_3a.flags.not_extracted = mode & (1 << 13);
	}

	void rollAngle(Cat21& r)
	{
		r._roll_angle = readDouble<16>(DEG_100);
	}

	void timeOfPosHigh(Cat21& r)
	{
		uint32_t           itime = readInt<32, unsigned>();
		HighPrecitionTime& time  = r._pos_time_high;
		time.fsi        = HighPrecitionTime::FullSecondIndication(itime >> 30);
		time.time_fract = (itime & 0x3FFFFFFF) * TIME_HIGH_LSB;
	}

	void timeOfVelHigh(Cat21& r)
	{
		uint32_t           itime = readInt<32, unsigned>();
		HighPrecitionTime& time  = r._vel_time_high;
		time.fsi        = HighPrecitionTime::FullSecondIndication(itime >> 30);
		time.time_fract = (itime & 0x3FFFFFFF) * TIME_HIGH_LSB;
	}

	void timeApplicPos(Cat21& r)
	{
		r._time_applicability_pos = readDouble<32>(TIME_LSB);
	}

	void targetReportDescriptor(Cat21& r)
	{
		uint64_t                flags = readFxFlags();
		TargetReportDescriptor& trd   = r._target_report_dscr;
		trd.atp = AddressType((flags >> 5) & 0x7);
		trd.arc = AltReportCap((flags >> 3) & 0x3);
		trd.rc  = flags & (1 << 2);
		trd.rab = ReportSource(!!(flags & (1 << 1)));

		trd.cl  = (flags >> 9) & 0x3;
		trd.saa = flags & (1 << 11);
		trd.tst = flags & (1 << 12);
		trd.sim = flags & (1 << 13);
		trd.gbs = flags & (1 << 14);
		trd.dcr = flags & (1 << 15);

		trd.rc_fail  = flags & (1 << 17);
		trd.ldpj     = flags & (1 << 18);
		trd.cpr_fail = flags & (1 << 19);
		trd.nogo     = flags & (1 << 20);
		trd.ipc_fail = flags & (1 << 21);
		trd.llc_fail = flags & (1 << 22);
	}

	void timeApplicVel(Cat21& r)
	{
		r._time_applicability_vel = readDouble<32>(TIME_LSB);
	}

	void emitterCategory(Cat21& r)
	{
		r._emitter_category = readInt<8, unsigned>();
	}

	void metInformation(Cat21& r)
	{
		uint64_t flags = readFxFlags();
		r._met_info.wind        = flags & 0x80 ? readDouble<16, unsigned>(KNOT) : __noop;	// NAN;		// for Linux
		r._met_info.wind_dir    = flags & 0x40 ? readDouble<16, unsigned>(1) : __noop;		//NAN;		// for Linux
		r._met_info.temperature = flags & 0x20 ? readDouble<16, signed>(TEMP_LSB) : __noop;	// NAN;		// for Linux
		r._met_info.turbulence  = flags & 0x20 ? readInt<8, unsigned>() : 0;
	}

	void selectedAlt(Cat21& r)
	{
		uint16_t data = readInt<16, unsigned>();
		r._sel_altitude.have_source = !!(data & 0x08000);
		r._sel_altitude.source      = AircraftSelectedAltitude((data >> 13) & 0x03);
		r._sel_altitude.altitude    = sign_extend<13>(data) * FOOT_25_M;
	}

	void finalSelectedAlt(Cat21& r)
	{
		uint16_t data = readInt<16, unsigned>();
		r._final_sel_altitude.mv       = !!(data & 0x8000);
		r._final_sel_altitude.ah       = !!(data & 0x4000);
		r._final_sel_altitude.am       = !!(data & 0x2000);
		r._final_sel_altitude.altitude = sign_extend<13>(data) * FOOT_25_M;
	}

	void trajectory(Cat21& r)
	{
		uint64_t flags   = readFxFlags();
		bool     has_sf1 = flags & 0x80;
		bool     has_sf2 = flags & 0x40;
		if (has_sf1)
		{
			r._trajectory.available = (flags >> 8) & 0x80;
			r._trajectory.valid     = (flags >> 8) & 0x40;
		}
		else
		{
			r._trajectory.available = false;
			r._trajectory.valid     = false;
		}
		if (!has_sf2)
			r._trajectory.tr.clear();
		else
		{
			uint8_t count = readInt<8, unsigned>();
			r._trajectory.tr.resize(count);
			for (uint8_t i = 0; i < count; ++i)
			{
				TrajectoryIntentData& t     = r._trajectory.tr[i];
				uint8_t               data1 = readInt<8, unsigned>();
				if (data1 & 0x80)
					t.rp_num = data1 & 0x3F;
				else
					t.rp_num = 0;
				t.rp_compliance = !!(data1 & 0x40);
				t.rp_altitude   = readInt<16, signed>() * FOOT_10;
				t.rp_latitude   = readDouble<24, signed>(WGS84_LSB);
				t.rp_longitude  = readDouble<24, signed>(WGS84_LSB);

				uint8_t data2 = readInt<8, unsigned>();
				t.rp_type        = (data2 >> 4) &  0x0F;
				t.rp_turn_sign   = (data2 >> 2) & 0x03;
				t.rp_time        = readInt<24>();
				t.rp_turn_radius = readDouble<16, unsigned>(TURN_LSB);

				if (!(data2 & 0x01))
					t.rp_time = 0;
				if (!(data2 & 0x02))
					t.rp_turn_radius = __noop;		// NAN;	//for Linux
			}
		}
	}

	void servicePeriod(Cat21& r)
	{
		r._service_period = readDouble<8, unsigned>(SERVICE_TIME);
	}

	void operationalStatus(Cat21& r)
	{
		uint8_t data = readInt<8, unsigned>();
		r._operational_status.ra       = !!(data & 0x80);
		r._operational_status.tc       = (data >> 6) & 0x03;
		r._operational_status.ts       = !!(data & 0x10);
		r._operational_status.arv      = !!(data & 0x08);
		r._operational_status.cdtia    = !!(data & 0x04);
		r._operational_status.not_tcas = !!(data & 0x02);
		r._operational_status.sa       = !!(data & 0x01);
	}

	void surfCap(Cat21& r)
	{
		uint64_t flags = readFxFlags();

		r._surf_cap.poa   =  flags & 0x20;
		r._surf_cap.cdtis =  flags & 0x10;
		r._surf_cap.b2low =  flags & 0x08;
		r._surf_cap.ras   =  flags & 0x04;
		r._surf_cap.ident =  flags & 0x02;
		r._surf_cap.lw    =  (flags >> 12) & 0x0F;
	}

	void amplitude(Cat21& r)
	{
		r._amplitude = readInt<8, unsigned>();
	}

	void modeS(Cat21& r)
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

	void acas(Cat21& r)
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

	void recvId(Cat21& r)
	{
		r._recv_id = readInt<8, unsigned>();
	}

	void ages(Cat21& r)
	{
		uint64_t data = readFxFlags();
#define read_age(i, v) \
	r._ages.v = (data & (1 << (i - 1))) ? readDouble<8, unsigned>(AGE_LSB) : __noop;	//NAN;	// for Linux

		read_age(32, aos);
		read_age(31, trd);
		read_age(30, m3a);
		read_age(29,  qi);
		read_age(28,  ti);
		read_age(27, mam);
		read_age(26,  gh);

		read_age(24,  fl);
		read_age(23, isa);
		read_age(22, fsa);
		read_age(21,  as);
		read_age(20, tas);
		read_age(19,  mh);
		read_age(18, bvr);

		read_age(16, gvr);
		read_age(15,  gv);
		read_age(14, tar);
		read_age(13, tid);
		read_age(12,  ts);
		read_age(11, met);
		read_age(10, roa);

		read_age(8,  ara);
		read_age(7,  scc);
#undef read_age
	}

	void bps(Cat21& r)
	{
		r._bps = readDouble<16, unsigned>(BPS_LSB);
	}

	void selHeading(Cat21& r)
	{
		uint16_t data = readInt<16, unsigned>();
		r._sel_heading.magnetic = data & (1 << 11);
		if (data & (1 << 10))
			r._sel_heading.heading = (data & 0x3FF) * SEL_HEADING_LSB;
		else
			r._sel_heading.heading = __noop;		// NAN;	//for Linux
	}

	void nav(Cat21& r)
	{
		uint8_t flags = readInt<8, unsigned>();
		r._nav.autopilot = flags & 1 << 7;
		r._nav.vnav      = flags & 1 << 6;
		r._nav.alt_hold  = flags & 1 << 5;
		r._nav.approach  = flags & 1 << 4;
	}

	void gpsOffset(Cat21& r)
	{
		r._gps_offset = readInt<8, unsigned>();
	}

	void surfVector(Cat21& r)
	{
		uint16_t          flags = readInt<16, unsigned>();
		bool              stp   = flags & 1 << 15;
		bool              hts   = flags & 1 << 14;
		SurfGroundVector& v     = r._surf_ground_vector;
		v.ground_track = flags & 1 << 13;
		v.magnetic     = flags & 1 << 12;
		v.ground_speed = __noop;		// NAN;	// for Linux
		v.heading      = __noop;			// NAN; // for Linux
		if (stp)
			v.ground_speed = 0;
		else if (hts)
			v.ground_speed = ((flags >> 1) & 0x7FF) * SURF_SPEED_LSB;
		if (flags & 1)
		{
			uint8_t flags = readInt<8, unsigned>();
			v.heading = (flags >> 1) * SURF_HEADING_LSB;
		}
	}

	void aircraftStatus(Cat21& r)
	{
		uint8_t flags = readFxFlags();
		r._sta.es  = flags & 1 << 7;
		r._sta.uat = flags & 1 << 6;
	}

	void trueHeading(Cat21& r)
	{
		r._true_heading = readDouble<16, unsigned>(DEG_360_16);
	}

	void millitary(Cat21&) {}

	void byteSkip(Cat21&) {}

};

struct Cat21::Writer :
	public Cat21::Uap::Serializer, public AsterixWriterT
{
public:

	Writer(char*& data, size_t& size) :
		AsterixWriterT(data, size) {};

	void writeRecord(const Cat21& v)
	{
		Asterix::writeRecord(v, this);
	}

private:

	void dataSourceId(Cat21& r)
	{
		writeInt<8, unsigned>(r._dsi.sac);
		writeInt<8, unsigned>(r._dsi.sic);
	}

	void trackNumber(Cat21& r)
	{
		writeInt<16, unsigned>(r._track_number & 0x0FFF);
	}

	void service(Cat21& r)
	{
		writeInt<8, unsigned>(r._service);
	}

	void timeOfPos(Cat21& r)
	{
		writeDouble<24, unsigned>(TIME_LSB, r._pos_time);
	}

	void timeOfVel(Cat21& r)
	{
		writeDouble<24, unsigned>(TIME_LSB, r._vel_time);
	}

	void timeOfReport(Cat21& r)
	{
		writeDouble<24, unsigned>(TIME_LSB, r._report_time);
	}

	void wgs84(Cat21& r)
	{
		writeDouble<24>(WGS84_LSB, r._position.wgs84_lat);
		writeDouble<24>(WGS84_LSB, r._position.wgs84_lon);
	}

	void wgs84high(Cat21& r)
	{
		writeDouble<32>(WGS84_LSB_HIGH, r._position_high.wgs84_lat);
		writeDouble<32>(WGS84_LSB_HIGH, r._position_high.wgs84_lon);
	}

	void reserved(Cat21& r)
	{
		AsterixWriterT::reserved(r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat21&)
	{
		AsterixWriterT::reserved_end();
	}

	void special(Cat21& r)
	{
		AsterixWriterT::special(r._sp);
	}

	void geometricAltitude(Cat21& r)
	{
		writeDouble<16>(ALTITUDE_LSB, r._geometric_altitude);
	}

	void targetAddress(Cat21& r)
	{
		writeInt<24, unsigned>(r._target_address);
	}

	void quality(Cat21& r)
	{
		uint64_t          flags = 0;
		QualityIndicators q     = r._quality;

		flags |= q.nic << 1;
		flags |= q.nac_v << 5;
		flags |= q.nac_p << 9;
		flags |= q.sil << 13;
		flags |= q.nic_baro << 15;

		flags |= q.sils << 21;
		flags |= q.sda << 19;
		flags |= q.gva << 17;

		flags |= q.pic << 28;

		writeFxFlags(flags);
	}

	void mops(Cat21& r)
	{
		uint8_t data = 0;
		data |= r._mops.unsupported << 6;
		data |= r._mops.version << 3;
		data |= r._mops.link;
		writeInt<8, unsigned>(data);
	}

	void targetId(Cat21& r)
	{
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

	void magHeading(Cat21& r)
	{
		writeDouble<16, unsigned>(DEG_360_16, r._mag_heading);
	}

	void targetStatus(Cat21& r)
	{
		uint8_t data = 0;
		data |= r._target_status.icf << 7;
		data |= r._target_status.lnav << 6;
		data |= r._target_status.me << 5;
		data |= r._target_status.emergency << 2;
		data |= r._target_status.status;
		writeInt<8, unsigned>(data);
	}

	void airSpeed(Cat21& r)
	{
		uint16_t speed_i = 0;
		if (r._air_speed.mach)
			speed_i = r._air_speed.speed / 0.001;
		else
			speed_i = r._air_speed.speed / NM_14;
		speed_i &= 0x7FFF;
		speed_i |= r._air_speed.mach << 15;
		writeInt<16, unsigned>(speed_i);
	}

	void trueAirSpeed(Cat21& r)
	{
		TrueAirSpeed& b       = r._true_air_speed;
		uint16_t      speed_i = 0;
		speed_i  = b.speed / KNOT;
		speed_i &= 0x7FFF;
		speed_i |= b.range_exceeded << 15;
		writeInt<16, unsigned>(speed_i);
	}

	void groundVector(Cat21& r)
	{
		GroundVector& v       = r._ground_vector;
		uint16_t      speed_i = 0;
		speed_i  = v.speed / NM_14;
		speed_i &= ~(1 << 15);
		speed_i |= v.range_exceeded << 15;
		writeInt<16, unsigned>(speed_i);
		writeDouble<16, unsigned>(DEG_360_16, v.heading);
	}

	void turnRate(Cat21& r)
	{
		uint16_t data = r._turn_rate / DEG_32;
		writeInt<16, unsigned>(data & 0x3FF);
	}

	void flightLevel(Cat21& r)
	{
		writeDouble<16>(FLIGHT_LEVEL_LSB, r._flight_level);
	}

	void baroVertSpeed(Cat21& r)
	{
		BaroVertRate& b       = r._baro_vert_speed;
		int16_t       speed_i = 0;
		speed_i  = b.speed / FOOT_MIN_LSB;
		speed_i &= 0x7FFF;
		speed_i |= b.range_exceeded << 15;
		writeInt<16>(speed_i);
	}

	void geoVertSpeed(Cat21& r)
	{
		GeoVertRate& b       = r._geo_vert_speed;
		int16_t      speed_i = 0;
		speed_i  = b.speed / FOOT_MIN_LSB;
		speed_i &= 0x7FFF;
		speed_i |= b.range_exceeded << 15;
		writeInt<16>(speed_i);
	}

	void mode3A(Cat21& r)
	{
		uint16_t mode = r._mode_3a.code;

		mode |= r._mode_3a.flags.not_validated << 15;
		mode |= r._mode_3a.flags.garbled << 14;
		mode |= r._mode_3a.flags.not_extracted << 13;

		writeInt<16, unsigned>(mode);
	}

	void rollAngle(Cat21& r)
	{
		writeDouble<16>(DEG_100, r._roll_angle);
	}

	void timeOfPosHigh(Cat21& r)
	{
		HighPrecitionTime& time  = r._pos_time_high;
		uint32_t           itime = time.fsi << 30;
		itime |= uint32_t(time.time_fract / TIME_HIGH_LSB) & 0x3FFFFFFF;
		writeInt<32, unsigned>(itime);
	}

	void timeOfVelHigh(Cat21& r)
	{
		HighPrecitionTime& time  = r._vel_time_high;
		uint32_t           itime = time.fsi << 30;
		itime |= uint32_t(time.time_fract / TIME_HIGH_LSB) & 0x3FFFFFFF;
		writeInt<32, unsigned>(itime);
	}

	void timeApplicPos(Cat21& r)
	{
		writeDouble<32>(TIME_LSB, r._time_applicability_pos);
	}

	void timeApplicVel(Cat21& r)
	{
		writeDouble<32>(TIME_LSB, r._time_applicability_vel);
	}

	void targetReportDescriptor(Cat21& r)
	{
		uint64_t                flags = 0;
		TargetReportDescriptor& trd   = r._target_report_dscr;
		flags |= (trd.atp & 0x7) << 5;
		flags |= (trd.arc & 0x3) << 3;
		flags |= trd.rc << 2;
		flags |= trd.rab << 1;

		flags |= (trd.cl & 0x3) << 9;
		flags |= trd.saa << 11;
		flags |= trd.tst << 12;
		flags |= trd.sim << 13;
		flags |= trd.gbs << 14;
		flags |= trd.dcr << 15;

		flags |= trd.rc_fail << 17;
		flags |= trd.ldpj << 18;
		flags |= trd.cpr_fail << 19;
		flags |= trd.nogo << 20;
		flags |= trd.ipc_fail << 21;
		flags |= trd.llc_fail << 22;

		writeFxFlags(flags);
	}

	void emitterCategory(Cat21& r)
	{
		writeInt<8, unsigned>(r._emitter_category);
	}

	void metInformation(Cat21& r)
	{
		/*uint64_t flags = 0;		// for Linux
		flags |= !!finite(r._met_info.wind) << 7;
		flags |= !!finite(r._met_info.wind_dir) << 6;
		flags |= !!finite(r._met_info.temperature) << 5;
		flags |= (r._met_info.turbulence != 0) << 4;
		writeFxFlags(flags);
		if (finite(r._met_info.wind))
			writeDouble<16>(KNOT, r._met_info.wind);
		if (finite(r._met_info.wind_dir))
			writeDouble<16>(ID_LSB, r._met_info.wind_dir);
		if (finite(r._met_info.temperature))
			writeDouble<16>(TEMP_LSB, r._met_info.temperature);
		if (r._met_info.turbulence != 0)
			writeInt<8, unsigned>(r._met_info.turbulence);*/
	}

	void selectedAlt(Cat21& r)
	{
		uint16_t data = 0;
		data |= r._sel_altitude.have_source << 15;
		data |= r._sel_altitude.source << 13;
		uint16_t alt = r._sel_altitude.altitude / FOOT_25_M;
		data |= alt & 0x1FFF;
		writeInt<16, unsigned>(data);
	}

	void finalSelectedAlt(Cat21& r)
	{
		uint16_t data = 0;
		data |= r._final_sel_altitude.mv << 15;
		data |= r._final_sel_altitude.ah << 14;
		data |= r._final_sel_altitude.am << 13;
		uint16_t alt = r._final_sel_altitude.altitude / FOOT_25_M;
		data |= alt & 0x1FFF;
		writeInt<16, unsigned>(data);
	}

	void trajectory(Cat21& r)
	{
		uint64_t flags = 0;
		flags |= 0x80;
		bool has_sf2 = !r._trajectory.tr.empty();
		if (has_sf2)
			flags |= 0x40;
		writeFxFlags(flags);
		writeInt<8, unsigned>((r._trajectory.available << 7) | (r._trajectory.valid << 6));
		if (!has_sf2)
			return;
		uint8_t count = r._trajectory.tr.size();
		writeInt<8, unsigned>(count);

		for (uint8_t i = 0; i < count; ++i)
		{
			TrajectoryIntentData& t     = r._trajectory.tr[i];
			uint8_t               data1 = t.rp_compliance << 6 | (t.rp_num & 0x3F);
			writeInt<8, unsigned>(data1);

			writeInt<16, signed>( t.rp_altitude / FOOT_10);
			writeDouble<24, signed>(WGS84_LSB, t.rp_latitude);
			writeDouble<24, signed>(WGS84_LSB, t.rp_longitude);

			uint8_t data2 =
				(t.rp_type & 0x0F) << 4 |
				(t.rp_turn_sign & 0x03) << 2 |
				/*(!!finite(t.rp_turn_radius) << 1) |*/	// for Linux
				(!!t.rp_time);

			writeInt<8, unsigned>( data2);
			writeInt<24, unsigned>(t.rp_time);
			// writeDouble<16, unsigned>(TURN_LSB, finite(t.rp_turn_radius) ? t.rp_turn_radius : 0);	// for Linux
		}
	}

	void servicePeriod(Cat21& r)
	{
		writeDouble<8, unsigned>(SERVICE_TIME, r._service_period);
	}

	void operationalStatus(Cat21& r)
	{
		uint8_t data = 0;
		data |= r._operational_status.ra << 7;
		data |= r._operational_status.tc << 5;
		data |= r._operational_status.ts << 4;
		data |= r._operational_status.arv << 3;
		data |= r._operational_status.cdtia << 2;
		data |= r._operational_status.not_tcas << 1;
		data |= r._operational_status.sa << 0;
		writeInt<8, unsigned>(data);
	}

	void surfCap(Cat21& r)
	{
		uint64_t flags = 0;

		flags |= r._surf_cap.poa << 5;
		flags |= r._surf_cap.cdtis << 4;
		flags |= r._surf_cap.b2low << 3;
		flags |= r._surf_cap.ras << 2;
		flags |= r._surf_cap.ident << 1;
		flags |= r._surf_cap.lw << 12;

		writeFxFlags(flags);
	}

	void amplitude(Cat21& r)
	{
		writeInt<8, unsigned>(r._amplitude);
	}

	void modeS(Cat21& r)
	{
		uint8_t count = r._mode_s.size();
		writeInt<8, unsigned>(count);
		for (size_t i = 0; i != count; ++i)
		{
			struct ModeS& s = r._mode_s[i];
			writeInt<64, unsigned>(s.data << 8 | s.bds);
		}
	}

	void acas(Cat21& r)
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

	void recvId(Cat21& r)
	{
		writeInt<8, unsigned>(r._recv_id);
	}

	void ages(Cat21& r)
	{
		/*uint64_t data = 0;	// for Linux
#define make_flags(i, v) \
	data |= !!finite(r._ages.v) << (i - 1);

		make_flags(32, aos);
		make_flags(31, trd);
		make_flags(30, m3a);
		make_flags(29,  qi);
		make_flags(28,  ti);
		make_flags(27, mam);
		make_flags(26,  gh);

		make_flags(24,  fl);
		make_flags(23, isa);
		make_flags(22, fsa);
		make_flags(21,  as);
		make_flags(20, tas);
		make_flags(19,  mh);
		make_flags(18, bvr);

		make_flags(16, gvr);
		make_flags(15,  gv);
		make_flags(14, tar);
		make_flags(13, tid);
		make_flags(12,  ts);
		make_flags(11, met);
		make_flags(10, roa);

		make_flags(8,  ara);
		make_flags(7,  scc);
#undef make_flags
		writeFxFlags(data);
#define write_ages(i, v) \
	if (data & (1 << (i - 1))) writeDouble<8, unsigned>(AGE_LSB, r._ages.v);

		write_ages(32, aos);
		write_ages(31, trd);
		write_ages(30, m3a);
		write_ages(29,  qi);
		write_ages(28,  ti);
		write_ages(27, mam);
		write_ages(26,  gh);

		write_ages(24,  fl);
		write_ages(23, isa);
		write_ages(22, fsa);
		write_ages(21,  as);
		write_ages(20, tas);
		write_ages(19,  mh);
		write_ages(18, bvr);

		write_ages(16, gvr);
		write_ages(15,  gv);
		write_ages(14, tar);
		write_ages(13, tid);
		write_ages(12,  ts);
		write_ages(11, met);
		write_ages(10, roa);

		write_ages(8,  ara);
		write_ages(7,  scc);
#undef write_ages
		*/
	}

	void bps(Cat21& r)
	{
		writeDouble<16, unsigned>(BPS_LSB, r._bps);
	}

	void selHeading(Cat21& r)
	{
		/*uint16_t data = 0;	// for Linux
		data |= r._sel_heading.magnetic << 11;
		data |= !!finite(r._sel_heading.heading) << 10;
		uint16_t selh = r._sel_heading.heading / SEL_HEADING_LSB;
		data |= selh;
		writeInt<16, unsigned>(data);*/
	}

	void nav(Cat21& r)
	{
		uint8_t flags = 0;
		flags |= r._nav.autopilot << 7;
		flags |= r._nav.vnav << 6;
		flags |= r._nav.alt_hold << 5;
		flags |= r._nav.approach << 4;
		writeInt<8, unsigned>(flags);
	}

	void gpsOffset(Cat21& r)
	{
		writeInt<8, unsigned>(r._gps_offset);
	}

	void surfVector(Cat21& r)
	{
		/*uint16_t          flags = 0;		// for Linux
		SurfGroundVector& v     = r._surf_ground_vector;
		flags |= (v.ground_speed == 0) << 15;
		flags |= isfinite(v.ground_speed) << 14;
		flags |= v.ground_track << 13;
		flags |= v.magnetic << 12;
		uint16_t gss = isfinite(v.ground_speed) ? v.ground_speed / SURF_SPEED_LSB : 0;
		flags |= gss << 1;
		flags |= isfinite(v.heading);

		writeInt<16, unsigned>(flags);
		if (isfinite(v.heading))
		{
			uint8_t flags = 0;
			uint8_t hgt   = v.heading / SURF_HEADING_LSB;
			flags |= hgt << 1;
			writeInt<8, unsigned>(flags);
		}*/
	}

	void aircraftStatus(Cat21& r)
	{
		uint8_t flags = 0;
		flags |= r._sta.es << 7;
		flags |= r._sta.uat << 6;
		writeFxFlags(flags);
	}

	void trueHeading(Cat21& r)
	{
		writeDouble<16, unsigned>(DEG_360_16, r._true_heading);
	}

	void millitary(Cat21&) {}

	void byteSkip(Cat21&) {}

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
	return rec;
}

char* Cat21::code(const AsterixRecord* record, size_t& write_size)
{
	if (record->category != 21)
		return NULL;
	char* data;
	{
		Writer writer(data, write_size);
		writer.writeRecord(*(const Cat21*) record);
	}
	return data;
}

}                                                          // namespace Cat21
