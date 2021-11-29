//#pragma warning( push )		// for windows
//#pragma warning( disable : 4066 )  
//#pragma warning( disable : 4554 )  
////#pragma warning( disable : 4707 )  
//// Some code  
//#pragma warning( pop )   

#include "asterix_cat_62.hpp"
#include "asterix_template.hpp"
#include <stdlib.h>
#include <string.h>

#define BROKEN_CONTRACT(expr, frn, clean) do {                                                                          \
		if (!expr) {                                                                                            \
			fprintf(stderr, "Asterix lib contract broken at %s:%d %s\n", __FILE__, __LINE__, __FUNCTION__); \
			r._fspec &= ~(1 << frn);                                                                        \
			clean;                                                                                          \
			return;                                                                                         \
		}                                                                                                       \
} while (0)
#define BROKEN_CONTRACT_MSG()                                                                           \
	fprintf(stderr, "Asterix lib contract broken at %s:%d %s\n", __FILE__, __LINE__, __FUNCTION__); \

namespace Cat62
{
template <> void Cat62::Uap::Serializer::skip<0>(Cat62&) {};

const Cat62::Uap Cat62::UAP[UAP_SIZE] =
{
	ASTERIX_UAP_FIELD(Cat62, 1,            10,  "Data Source Identifier",                                       2, dataSourceId),
	ASTERIX_UAP_FIELD(Cat62, 2,            0,   "Spare",                                                        0, skip<0>           ),
	ASTERIX_UAP_FIELD(Cat62, 3,            15,  "Service Identification",                                       1, service),
	ASTERIX_UAP_FIELD(Cat62, 4,            70,  "Time Of Track Information",                                    3, timeOfTrack),
	ASTERIX_UAP_FIELD(Cat62, 5,            105, "Calculated Track Position (WGS-84)",                           8, wgs84),
	ASTERIX_UAP_FIELD(Cat62, 6,            100, "Calculated Track Position (Cartesian)",                        6, cartCoords),
	ASTERIX_UAP_FIELD(Cat62, 7,            185, "Calculated Track Velocity (Cartesian)",                        4, velocity),
	ASTERIX_UAP_FIELD(Cat62, 8,            210, "Calculated Acceleration (Cartesian)",                          2, acceleration),
	ASTERIX_UAP_FIELD(Cat62, 9,            60,  "Track Mode 3/A Code",                                          2, mode3A),
	ASTERIX_UAP_FIELD(Cat62, 10,           245, "Target Identification",                                        7, targetId),
	ASTERIX_UAP_FIELD(Cat62, 11,           380, "Aircraft Derived Data",                                        0, aircraftData),
	ASTERIX_UAP_FIELD(Cat62, 12,           40,  "Track Number",                                                 2, trackNumber),
	ASTERIX_UAP_FIELD(Cat62, 13,           80,  "Track Status",                                                 0, trackStatus),
	ASTERIX_UAP_FIELD(Cat62, 14,           290, "System Track Update Ages",                                     0, systemAges),
	ASTERIX_UAP_FIELD(Cat62, 15,           200, "Mode of Movement",                                             1, movement),
	ASTERIX_UAP_FIELD(Cat62, 16,           295, "Track Data Ages",                                              0, dataAges),
	ASTERIX_UAP_FIELD(Cat62, 17,           136, "Measured Flight Level",                                        2, flightLevel),
	ASTERIX_UAP_FIELD(Cat62, 18,           130, "Calculated Track Geometric Altitude",                          2, geoAltitde),
	ASTERIX_UAP_FIELD(Cat62, 19,           135, "Calculated Track Barometric Altitude",                         2, baroAltitde),
	ASTERIX_UAP_FIELD(Cat62, 20,           220, "Calculated Rate Of Climb/Descent",                             2, vertRate),
	ASTERIX_UAP_FIELD(Cat62, 21,           390, "Flight Plan Related Data",                                     0, flightPlan),
	ASTERIX_UAP_FIELD(Cat62, 22,           270, "Target Size & Orientation",                                    0, size),
	ASTERIX_UAP_FIELD(Cat62, 23,           300, "Vehicle Fleet Identification",                                 1, fleetId),
	ASTERIX_UAP_FIELD(Cat62, 24,           110, "Mode 5 Data reports & Extended Mode 1 Code",                   0, mode5),
	ASTERIX_UAP_FIELD(Cat62, 25,           120, "Track Mode 2 Code",                                            2, mode2),
	ASTERIX_UAP_FIELD(Cat62, 26,           510, "Composed Track Number",                                        0, composed),
	ASTERIX_UAP_FIELD(Cat62, 27,           500, "Estimated Accuracies",                                         0, accuracy),
	ASTERIX_UAP_FIELD(Cat62, 28,           340, "Measured Information",                                         0, measured),
	ASTERIX_UAP_FIELD(Cat62, 29,           0,   "Spare",                                                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat62, 30,           0,   "Spare",                                                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat62, 31,           0,   "Spare",                                                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat62, 32,           0,   "Spare",                                                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat62, 33,           0,   "Spare",                                                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat62, 34,           0,   "Reserved Expansion Field",                                     0, reserved),
	ASTERIX_UAP_FIELD(Cat62, 35,           0,   "Reserved For Special Purpose Indicator",                       0, special),

	ASTERIX_UAP_FIELD(Cat62, LAST_FRN + 0, 0,   "Contributing Sensors With Local Tracknumbers",                 0, contributions),
	ASTERIX_UAP_FIELD(Cat62, LAST_FRN + 1, 0,   "Contributing Sensors No Local Tracknumbers",                   0, contributions_no_tn),
	ASTERIX_UAP_FIELD(Cat62, LAST_FRN + 2, 0,   "Calculated Track Velocity Relative to System Reference Point", 4, velocity_to_ref),
	ASTERIX_UAP_FIELD(Cat62, LAST_FRN + 3, 0,   "Supplementary Track Status",                                   0, track_status_sup),

};

struct Cat62::Reader :
	public Uap::Serializer
{
	TCommonObmenData* odata;
	TCatAsterix*      cat;
	TCat062*          c62;
	size_t            bytes_read;

public:

	Reader(const char* data, size_t size) :
		odata(NULL), cat(NULL), c62(NULL)
	{
		/*bytes_read = 0;		// for Linux
		odata      = initObmenData(size);
		setObmenData(odata, (void*) data, size);
		cat = decode062catRecord(odata, &bytes_read);
		c62 = get062category(cat);*/
	};

	~Reader()
	{
		freeCatAsterixData(cat);
		freeObmenData(odata);
	}

	void readRecord(Cat62& v)
	{
		size_t   size;
		uint8_t* f = get062recordFspec(c62, &size);
		v._fspec = AsterixReader::readFspec(f, size);
		Asterix::callRecord(v, this);
	}

	size_t bytesRead()
	{
		return bytes_read;
	}

private:

	void dataSourceId(Cat62& r)
	{
		uint32_t dsi = getDataSourceIdentificator(cat).val;
		r._dsi.sac = dsi >> 8 & 0xFF;
		r._dsi.sic = dsi & 0xFF;
	}

	void service(Cat62& r)
	{
		r._service = getServiceIdentificator(cat).val;
	}

	void timeOfTrack(Cat62& r)
	{
		r._time = getTimeOfTrackInformation(cat).val;
	}

	void wgs84(Cat62& r)
	{
		r._position.wgs84_lat = getTrackPositionWgs84(cat).val1;
		r._position.wgs84_lon = getTrackPositionWgs84(cat).val2;
	}

	void cartCoords(Cat62& r)
	{
		r._cart_pos.x = getTrackPositionCartesian(cat).val1;
		r._cart_pos.y = getTrackPositionCartesian(cat).val2;
	}

	void velocity(Cat62& r)
	{
		r._velocity.vx = getTrackVelocityCartesian(cat).val1;
		r._velocity.vy = getTrackVelocityCartesian(cat).val2;
	}

	void acceleration(Cat62& r)
	{
		r._accel.ax = getTrackAccelerationCartesian(cat).val1;
		r._accel.ay = getTrackAccelerationCartesian(cat).val2;
	}

	void mode3A(Cat62& r)
	{
		BoardNumCode code = getTrackMode3Acode(cat);

		r._mode_3a.flags.not_validated = !code.valid;
		r._mode_3a.flags.garbled       = code.garbled;
		r._mode_3a.flags.not_extracted = !code.change;
		r._mode_3a.code                = code.board_number_code;
	}

	void targetId(Cat62& r)
	{
		uint8_t           src = 0;
		TCommonObmenData* cs  = getTrackTargetIdentification(cat, &src);
		BROKEN_CONTRACT(cs, 14, );
		uint8_t* d = (uint8_t*) getObmenData(cs);
		BROKEN_CONTRACT(d,  14, );
		uint64_t id = ((((d[0] << 8 | d[1]) << 8 | d[2]) << 8 | d[3]) << 8 | d[4] << 8) | d[5];
		freeObmenData(cs);

		r._callsign.sti         = src < TIS_UNDEFINED ? TargetIdentificationSource(src) : TIS_UNDEFINED;
		r._callsign.callsign[0] = AsterixReader::getIa5(id >> 42);
		r._callsign.callsign[1] = AsterixReader::getIa5(id >> 36);
		r._callsign.callsign[2] = AsterixReader::getIa5(id >> 30);
		r._callsign.callsign[3] = AsterixReader::getIa5(id >> 24);
		r._callsign.callsign[4] = AsterixReader::getIa5(id >> 18);
		r._callsign.callsign[5] = AsterixReader::getIa5(id >> 12);
		r._callsign.callsign[6] = AsterixReader::getIa5(id >> 6);
		r._callsign.callsign[7] = AsterixReader::getIa5(id >> 0);
		r._callsign.callsign[8] = 0;
	}

	void aircraftData(Cat62& r)
	{
		AircraftData* ad = get062TrackAircraftDerivedData(c62);
		BROKEN_CONTRACT(ad, 21, memset(&r._aircraft_data, 0, sizeof(r._aircraft_data)));
		r._aircraft_data = *ad;
	}

	void trackNumber(Cat62& r)
	{
		r._track_number = getTrackNumber(cat).val;
	}

	void trackStatus(Cat62& r)
	{
		r._track_status = *getTrackStatus(cat);
	}

	void systemAges(Cat62& r)
	{
		r._system_ages.trk    = getTrackSystemTimeAges(cat,  STUTT_TRACK_AGE).val;
		r._system_ages.psr    = getTrackSystemTimeAges(cat,  STUTT_PSR_AGE).val;
		r._system_ages.ssr    = getTrackSystemTimeAges(cat,  STUTT_SSR_AGE).val;
		r._system_ages.mds    = getTrackSystemTimeAges(cat,  STUTT_MODE_S_AGE).val;
		r._system_ages.ads    = getTrackSystemTimeAges(cat,  STUTT_ADS_C_AGE).val;
		r._system_ages.ads_es = getTrackSystemTimeAges(cat,  STUTT_ADS_B_AGE).val;
		r._system_ages.vdl    = getTrackSystemTimeAges(cat,  STUTT_VDL_MODE4_AGE).val;
		r._system_ages.uat    = getTrackSystemTimeAges(cat,  STUTT_UAT_AGE).val;
		r._system_ages.loop   = getTrackSystemTimeAges(cat,  STUTT_LOOP_AGE).val;
		r._system_ages.mlt    = getTrackSystemTimeAges(cat,  STUTT_MLAT_AGE).val;
	}

	void dataAges(Cat62& r)
	{
		r._data_ages.mfl = getTrackDataAges(cat, TUAT_MFL_AGE).val;
		r._data_ages.md1 = getTrackDataAges(cat, TUAT_MD1_AGE).val;
		r._data_ages.md2 = getTrackDataAges(cat, TUAT_MD2_AGE).val;
		r._data_ages.mda = getTrackDataAges(cat, TUAT_MDA_AGE).val;
		r._data_ages.md4 = getTrackDataAges(cat, TUAT_MD4_AGE).val;
		r._data_ages.md5 = getTrackDataAges(cat, TUAT_MD5_AGE).val;
		r._data_ages.mhg = getTrackDataAges(cat, TUAT_MHG_AGE).val;
		r._data_ages.ias = getTrackDataAges(cat, TUAT_IAS_AGE).val;
		r._data_ages.tas = getTrackDataAges(cat, TUAT_TAS_AGE).val;
		r._data_ages.sal = getTrackDataAges(cat, TUAT_SAL_AGE).val;
		r._data_ages.fss = getTrackDataAges(cat, TUAT_FSS_AGE).val;
		r._data_ages.tid = getTrackDataAges(cat, TUAT_TID_AGE).val;
		r._data_ages.com = getTrackDataAges(cat, TUAT_COM_AGE).val;
		r._data_ages.sab = getTrackDataAges(cat, TUAT_SAB_AGE).val;
		r._data_ages.acs = getTrackDataAges(cat, TUAT_ACS_AGE).val;
		r._data_ages.bvr = getTrackDataAges(cat, TUAT_BVR_AGE).val;
		r._data_ages.gvr = getTrackDataAges(cat, TUAT_GVR_AGE).val;
		r._data_ages.ran = getTrackDataAges(cat, TUAT_RAN_AGE).val;
		r._data_ages.tar = getTrackDataAges(cat, TUAT_TAR_AGE).val;
		r._data_ages.tan = getTrackDataAges(cat, TUAT_TAN_AGE).val;
		r._data_ages.gsp = getTrackDataAges(cat, TUAT_GSP_AGE).val;
		r._data_ages.vun = getTrackDataAges(cat, TUAT_VUN_AGE).val;
		r._data_ages.met = getTrackDataAges(cat, TUAT_MET_AGE).val;
		r._data_ages.emc = getTrackDataAges(cat, TUAT_EMC_AGE).val;
		r._data_ages.pos = getTrackDataAges(cat, TUAT_POS_AGE).val;
		r._data_ages.gal = getTrackDataAges(cat, TUAT_GAL_AGE).val;
		r._data_ages.pun = getTrackDataAges(cat, TUAT_PUN_AGE).val;
		r._data_ages.mb  = getTrackDataAges(cat, TUAT_MB_AGE).val;
		r._data_ages.iar = getTrackDataAges(cat, TUAT_IAR_AGE).val;
		r._data_ages.mac = getTrackDataAges(cat, TUAT_MAC_AGE).val;
		r._data_ages.bps = getTrackDataAges(cat, TUAT_BPS_AGE).val;
	}

	void movement(Cat62& r)
	{
		r._movement.trans  = getTrackModeMovementHorizontal(cat);
		r._movement.longit = getTrackModeMovementBySpeed(cat);
		r._movement.vert   = getTrackModeMovementVertical(cat);
		r._movement.adf    = false;                ///< TODO Доступ не найден
	}

	void flightLevel(Cat62& r)
	{
		r._flight_level = getTrackMeasuredFlightLevel(cat).val;
	}

	void baroAltitde(Cat62& r)
	{
		r._baro_altitude.altitude = getTrackCalcBarometricAltitude(cat).val1;
		r._baro_altitude.qnh      = getTrackCalcBarometricAltitude(cat).val2;
	}

	void geoAltitde(Cat62& r)
	{
		r._geo_altitude = getTrackCalcGeometricAltitude(cat).val;
	}

	void vertRate(Cat62& r)
	{
		r._vert_rate = getTrackCalcVerticalRate(cat).val;
	}

	void size(Cat62& r)
	{
		r._size.length      = getTrackTargetSizeLenght(cat).val;
		r._size.width       = getTrackTargetSizeWidth(cat).val;
		r._size.orientation = getTrackTargetSizeOrientation(cat).val;
	}

	void flightPlan(Cat62& r)
	{
		FlightPlan* fp = getTrackFlightPlanData(cat);
		BROKEN_CONTRACT(fp, 21, memset(&r._flight_plan, 0, sizeof(r._flight_plan)));
		r._flight_plan = *fp;
		switch ((uint8_t) r._flight_plan.turbulence)
		{
		case 0xeb:
			r._flight_plan.turbulence_w = L'Л';
			break;
		case 0xec:
			r._flight_plan.turbulence_w = L'М';
			break;
		case 0xed:
			r._flight_plan.turbulence_w = L'Н';
			break;
		default:
			r._flight_plan.turbulence_w = (uint8_t) r._flight_plan.turbulence;
			break;
		}
		AsterixReader::cp866_to_wide(r._flight_plan.dep_airport,   r._flight_plan.dep_airport_w);
		AsterixReader::cp866_to_wide(r._flight_plan.dest_airport,  r._flight_plan.dest_airport_w);
		AsterixReader::cp866_to_wide(r._flight_plan.type_aircraft, r._flight_plan.type_aircraft_w);
		AsterixReader::cp866_to_wide(r._flight_plan.callsign,      r._flight_plan.callsign_w);
		AsterixReader::cp866_to_rw(r._flight_plan.dest_runway, r._flight_plan.dest_runway_w);
	}

	void fleetId(Cat62& r)
	{
		r._fleet_type = FleetType(getTrackVehicleFleetIdentification(cat).val);
	}

	void accuracy(Cat62& r)
	{
		r._accuracy = *getTrackEstimateAccuracies(cat);
	}

	void mode5(Cat62&) {}
	void mode2(Cat62& r)
	{
		BoardNumCode m2 = getTrackMode2code(cat);

		r._mode_2.flags.not_validated = m2.valid;
		r._mode_2.flags.garbled       = m2.garbled;
		r._mode_2.flags.not_extracted = m2.change;
		r._mode_2.code                = m2.board_number_code;
	}

	void measured(Cat62& r)
	{
		r._measured_data = *get062TrackMeasuredInformation(c62);
	}

	void composed(Cat62&) {};
	void contributions(Cat62&) {};
	void contributions_no_tn(Cat62&) {};
	void velocity_to_ref(Cat62&) {};
	void track_status_sup(Cat62&) {};

	void reserved(Cat62& r) {}
	void special(Cat62& r) {}
};

struct Cat62::Writer :
	public Uap::Serializer
{
	TCommonObmenData* odata;
	TCatAsterix*      cat;
	TCat062*          c62;

	size_t& bytes_written;

public:

	Writer(size_t& size) :
		odata(NULL), cat(NULL), c62(NULL), bytes_written(size)
	{
		cat = createCatAsterixData(11);
		c62 = get062category(cat);
	}

	~Writer()
	{
		freeCatAsterixData(cat);
		free(odata);
	}

	char* writeRecord(const Cat62& v)
	{
		Asterix::callRecord(const_cast<Cat62&>(v), this);
		odata         = pack062catRecord(c62);
		bytes_written = getObmenDataLenght(odata);
		char* data = (char*) getObmenData(odata);
		if (!data)
			BROKEN_CONTRACT_MSG();
		return data;
	}

private:

	void dataSourceId(Cat62& r)
	{
		int dsi = r._dsi.sac << 8 | r._dsi.sic;
		setDataSourceIdentificator(cat, dsi);
	}

	void service(Cat62& r)
	{
		setServiceIdentificator(cat, r._service);
	}

	void timeOfTrack(Cat62& r)
	{
		setTimeOfTrackInformation(cat, r._time);
	}

	void wgs84(Cat62& r)
	{
		setTrackPositionWgs84(cat, r._position.wgs84_lat, r._position.wgs84_lon);
	}

	void cartCoords(Cat62& r)
	{
		setTrackPositionCartesian(cat, r._cart_pos.x, r._cart_pos.y);
	}

	void velocity(Cat62& r)
	{
		setTrackVelocityCartesian(cat, r._velocity.vx, r._velocity.vy);
	}

	void acceleration(Cat62& r)
	{
		setTrackAccelerationCartesian(cat, r._accel.ax, r._accel.ay);
	}

	void mode3A(Cat62& r)
	{
		BoardNumCode code = {};

		code.valid             = !r._mode_3a.flags.not_validated;
		code.garbled           = r._mode_3a.flags.garbled;
		code.change            = !r._mode_3a.flags.not_extracted;
		code.board_number_code = r._mode_3a.code;

		setTrackMode3Acode(cat, &code);
	}

	void targetId(Cat62& r)
	{
		setTrackTargetIdentification(cat, r._callsign.callsign, r._callsign.sti);
	}

	void aircraftData(Cat62& r)
	{
		set062TrackAircraftDerivedData(c62, &r._aircraft_data);
	}

	void trackNumber(Cat62& r)
	{
		setTrackNumber(cat, r._track_number);
	}

	void trackStatus(Cat62& r)
	{
		setTrackStatus(cat, &r._track_status);
	}

	void systemAges(Cat62& r)
	{
		setTrackSystemTimeAges(cat, r._system_ages.trk,    STUTT_TRACK_AGE);
		setTrackSystemTimeAges(cat, r._system_ages.psr,    STUTT_PSR_AGE);
		setTrackSystemTimeAges(cat, r._system_ages.ssr,    STUTT_SSR_AGE);
		setTrackSystemTimeAges(cat, r._system_ages.mds,    STUTT_MODE_S_AGE);
		setTrackSystemTimeAges(cat, r._system_ages.ads,    STUTT_ADS_C_AGE);
		setTrackSystemTimeAges(cat, r._system_ages.ads_es, STUTT_ADS_B_AGE);
		setTrackSystemTimeAges(cat, r._system_ages.vdl,    STUTT_VDL_MODE4_AGE);
		setTrackSystemTimeAges(cat, r._system_ages.uat,    STUTT_UAT_AGE);
		setTrackSystemTimeAges(cat, r._system_ages.loop,   STUTT_LOOP_AGE);
		setTrackSystemTimeAges(cat, r._system_ages.mlt,    STUTT_MLAT_AGE);
	}

	void dataAges(Cat62& r)
	{
		setTrackDataAges(cat, r._data_ages.mfl, TUAT_MFL_AGE);
		setTrackDataAges(cat, r._data_ages.md1, TUAT_MD1_AGE);
		setTrackDataAges(cat, r._data_ages.md2, TUAT_MD2_AGE);
		setTrackDataAges(cat, r._data_ages.mda, TUAT_MDA_AGE);
		setTrackDataAges(cat, r._data_ages.md4, TUAT_MD4_AGE);
		setTrackDataAges(cat, r._data_ages.md5, TUAT_MD5_AGE);
		setTrackDataAges(cat, r._data_ages.mhg, TUAT_MHG_AGE);
		setTrackDataAges(cat, r._data_ages.ias, TUAT_IAS_AGE);
		setTrackDataAges(cat, r._data_ages.tas, TUAT_TAS_AGE);
		setTrackDataAges(cat, r._data_ages.sal, TUAT_SAL_AGE);
		setTrackDataAges(cat, r._data_ages.fss, TUAT_FSS_AGE);
		setTrackDataAges(cat, r._data_ages.tid, TUAT_TID_AGE);
		setTrackDataAges(cat, r._data_ages.com, TUAT_COM_AGE);
		setTrackDataAges(cat, r._data_ages.sab, TUAT_SAB_AGE);
		setTrackDataAges(cat, r._data_ages.acs, TUAT_ACS_AGE);
		setTrackDataAges(cat, r._data_ages.bvr, TUAT_BVR_AGE);
		setTrackDataAges(cat, r._data_ages.gvr, TUAT_GVR_AGE);
		setTrackDataAges(cat, r._data_ages.ran, TUAT_RAN_AGE);
		setTrackDataAges(cat, r._data_ages.tar, TUAT_TAR_AGE);
		setTrackDataAges(cat, r._data_ages.tan, TUAT_TAN_AGE);
		setTrackDataAges(cat, r._data_ages.gsp, TUAT_GSP_AGE);
		setTrackDataAges(cat, r._data_ages.vun, TUAT_VUN_AGE);
		setTrackDataAges(cat, r._data_ages.met, TUAT_MET_AGE);
		setTrackDataAges(cat, r._data_ages.emc, TUAT_EMC_AGE);
		setTrackDataAges(cat, r._data_ages.pos, TUAT_POS_AGE);
		setTrackDataAges(cat, r._data_ages.gal, TUAT_GAL_AGE);
		setTrackDataAges(cat, r._data_ages.pun, TUAT_PUN_AGE);
		setTrackDataAges(cat, r._data_ages.mb,  TUAT_MB_AGE);
		setTrackDataAges(cat, r._data_ages.iar, TUAT_IAR_AGE);
		setTrackDataAges(cat, r._data_ages.mac, TUAT_MAC_AGE);
		setTrackDataAges(cat, r._data_ages.bps, TUAT_BPS_AGE);
	}

	void movement(Cat62& r)
	{
		setTrackModeMovement(cat, r._movement.trans, r._movement.vert, r._movement.longit, r._movement.adf);
	}

	void flightLevel(Cat62& r)
	{
		setTrackMeasuredFlightLevel(cat, r._flight_level);
	}

	void baroAltitde(Cat62& r)
	{
		setTrackCalcBarometricAltitude(cat, r._baro_altitude.altitude, r._baro_altitude.qnh);
	}

	void geoAltitde(Cat62& r)
	{
		setTrackCalcGeometricAltitude(cat, r._geo_altitude);
	}

	void vertRate(Cat62& r)
	{
		setTrackCalcVerticalRate(cat, r._vert_rate);
	}

	void size(Cat62& r)
	{
		setTrackTargetSize(cat, r._size.length, r._size.width, r._size.orientation);
	}

	void flightPlan(Cat62& r)
	{
		setTrackFlightPlanData(cat, &r._flight_plan);
	}

	void fleetId(Cat62& r)
	{
		setTrackVehicleFleetIdentification(cat, r._fleet_type);
	}

	void accuracy(Cat62& r)
	{
		setTrackEstimateAccuracies(cat, &r._accuracy);
	}

	void mode5(Cat62&) {}
	void mode2(Cat62& r)
	{
		BoardNumCode m2 = {};

		m2.valid             = r._mode_2.flags.not_validated;
		m2.garbled           = r._mode_2.flags.garbled;
		m2.change            = r._mode_2.flags.not_extracted;
		m2.board_number_code = r._mode_2.code;

		setTrackMode2code(cat, m2);
	}

	void measured(Cat62& r)
	{
		set062TrackMeasuredInformation(c62, &r._measured_data);
	}

	void composed(Cat62&) {};
	void contributions(Cat62&) {};
	void contributions_no_tn(Cat62&) {};
	void velocity_to_ref(Cat62&) {};
	void track_status_sup(Cat62&) {};

	void reserved(Cat62& r) {}
	void special(Cat62& r) {}
};

AsterixRecord* Cat62::decode(const char* data, size_t size, AsterixError& err, size_t& read_size)
{
	if (size == 0)
	{
		err = A_EMPTY_BUFFER;
		return NULL;
	}
	Reader reader(data, size);

	Cat62* rec = new Cat62;
	rec->_fleet_type = (FleetType) 0;
	reader.readRecord(*rec);
	read_size = reader.bytesRead();

	if (read_size == 0)
	{
		err = A_DECODE_WRONG_SIZE;
		delete rec;
		rec = NULL;
	}
	else
		err = A_NO_ERROR;
	return rec;
}

char* Cat62::code(const AsterixRecord* record, size_t& write_size)
{
	if (record->category != 11)
		return NULL;
	Writer writer(write_size);
	return writer.writeRecord(*(const Cat62*) record);
}

}                                                          // namespace Cat62
