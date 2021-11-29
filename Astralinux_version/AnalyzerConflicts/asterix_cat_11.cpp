#include "asterix_cat_11.hpp"
#include "asterix_template.hpp"
#include <stdlib.h>
#include <string.h>

// #pragma comment(lib, "ws2_32.lib");	

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

namespace Cat11
{
template <> void Cat11::Uap::Serializer::skip<0>(Cat11&) {};

const Cat11::Uap Cat11::UAP[UAP_SIZE] =
{

	ASTERIX_UAP_FIELD(Cat11, 0,  0,   "Field Extension Indicator",                     0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat11, 1,  10,  "Data Source Identifier",                        2, dataSourceId),
	ASTERIX_UAP_FIELD(Cat11, 2,  0,   "Message Type",                                  1, messageType),
	ASTERIX_UAP_FIELD(Cat11, 3,  15,  "Service Identification",                        1, service),
	ASTERIX_UAP_FIELD(Cat11, 4,  140, "Time of Track Information",                     3, timeOfTrack),
	ASTERIX_UAP_FIELD(Cat11, 5,  41,  "Position in WGS-84 Co-ordinates",               8, wgs84),
	ASTERIX_UAP_FIELD(Cat11, 6,  42,  "Calculated Position in Cartesian Co-ordinates", 4, cartCoords),
	ASTERIX_UAP_FIELD(Cat11, 7,  202, "Calculated Track Velocity in Cartesian coord.", 4, velocity),
	ASTERIX_UAP_FIELD(Cat11, 8,  210, "Calculated Acceleration",                       2, acceleration),
	ASTERIX_UAP_FIELD(Cat11, 9,  60,  "Mode-3/A Code in octal representation",         2, mode3A),
	ASTERIX_UAP_FIELD(Cat11, 10, 245, "Target Identification",                         7, targetId),
	ASTERIX_UAP_FIELD(Cat11, 11, 380, "Mode-S / ADS-B Related Data",                   0, modeS),
	ASTERIX_UAP_FIELD(Cat11, 12, 161, "Track Number",                                  2, trackNumber),
	ASTERIX_UAP_FIELD(Cat11, 13, 170, "Track Status",                                  0, trackStatus),
	ASTERIX_UAP_FIELD(Cat11, 14, 290, "System Track Update Ages",                      0, dataAges),
	ASTERIX_UAP_FIELD(Cat11, 15, 430, "Phase of Flight",                               1, flightPhase),
	ASTERIX_UAP_FIELD(Cat11, 16, 90,  "Measured Flight Level",                         2, flightLevel),
	ASTERIX_UAP_FIELD(Cat11, 17, 93,  "Calculated Track Barometric Altitude",          2, baroAltitde),
	ASTERIX_UAP_FIELD(Cat11, 18, 92,  "Calculated Track Geometric Altitude",           2, geoAltitde),
	ASTERIX_UAP_FIELD(Cat11, 19, 215, "Calculated Rate of Climb/Descent",              2, vertRate),
	ASTERIX_UAP_FIELD(Cat11, 20, 270, "Target Size & Orientation",                     0, size),
	ASTERIX_UAP_FIELD(Cat11, 21, 390, "Flight Plan Related Data",                      0, flightPlan),
	ASTERIX_UAP_FIELD(Cat11, 22, 300, "Vehicle Fleet Identification",                  1, fleetId),
	ASTERIX_UAP_FIELD(Cat11, 23, 310, "Pre-programmed Message",                        1, programmedMessage),
	ASTERIX_UAP_FIELD(Cat11, 24, 500, "Estimated Accuracies",                          0, accuracy),
	ASTERIX_UAP_FIELD(Cat11, 25, 600, "Alert Messages",                                3, alert),
	ASTERIX_UAP_FIELD(Cat11, 26, 605, "Tracks in Alert",                               0, alertTracks),
	ASTERIX_UAP_FIELD(Cat11, 27, 610, "Holdbar Status",                                0, holdbar),
	ASTERIX_UAP_FIELD(Cat11, 28, 0,   "Special Purpose Field",                         0, special),
	ASTERIX_UAP_FIELD(Cat11, 29, 0,   "Reserved Expansion Field",                      0, reserved),
	ASTERIX_UAP_FIELD(Cat11, 30, 0,   "Spare",                                         0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat11, 31, 0,   "Spare",                                         0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat11, 32, 0,   "Spare",                                         0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat11, 33, 0,   "Spare",                                         0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat11, 34, 0,   "Spare",                                         0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat11, 35, 0,   "Spare",                                         0, skip<0> ),
};

struct Cat11::Reader :
	public Uap::Serializer
{
	TCommonObmenData* odata;
	TCatAsterix*      cat;
	TCat011*          c11;
	size_t            bytes_read;

public:

	Reader(const char* data, size_t size) :
		odata(NULL), cat(NULL), c11(NULL)
	{
		bytes_read = 0;
		odata      = initObmenData(size);
		setObmenData(odata, (void*) data, size);
		cat = decode011catRecord(odata, &bytes_read);
		c11 = get011category(cat);
	};

	~Reader()
	{
		freeCatAsterixData(cat);
		freeObmenData(odata);
	}

	void readRecord(Cat11& v)
	{
		size_t   size;
		uint8_t* f = get011recordFspec(c11, &size);
		v._fspec = AsterixReader::readFspec(f, size);
		Asterix::callRecord(v, this);
	}

	size_t bytesRead()
	{
		return bytes_read;
	}

private:

	void dataSourceId(Cat11& r)
	{
		uint32_t dsi = getDataSourceIdentificator(cat).val;
		r._dsi.sac = dsi >> 8 & 0xFF;
		r._dsi.sic = dsi & 0xFF;
	}

	void messageType(Cat11& r)
	{
		r._message_type = get011MessageType(c11).val;
	}

	void service(Cat11& r)
	{
		r._service = getServiceIdentificator(cat).val;
	}

	void timeOfTrack(Cat11& r)
	{
		r._time = getTimeOfTrackInformation(cat).val;
	}

	void wgs84(Cat11& r)
	{
		r._position.wgs84_lat = getTrackPositionWgs84(cat).val1;
		r._position.wgs84_lon = getTrackPositionWgs84(cat).val2;
	}

	void cartCoords(Cat11& r)
	{
		r._cart_pos.x = getTrackPositionCartesian(cat).val1;
		r._cart_pos.y = getTrackPositionCartesian(cat).val2;
	}

	void velocity(Cat11& r)
	{
		r._velocity.vx = getTrackVelocityCartesian(cat).val1;
		r._velocity.vy = getTrackVelocityCartesian(cat).val2;
	}

	void acceleration(Cat11& r)
	{
		r._accel.ax = getTrackAccelerationCartesian(cat).val1;
		r._accel.ay = getTrackAccelerationCartesian(cat).val2;
	}

	void mode3A(Cat11& r)
	{
		BoardNumCode code = getTrackMode3Acode(cat);

		r._mode_3a.flags.not_validated = !code.valid;
		r._mode_3a.flags.garbled       = code.garbled;
		r._mode_3a.flags.not_extracted = !code.change;
		r._mode_3a.code                = code.board_number_code;
	}

	void targetId(Cat11& r)
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

	void modeS(Cat11& r)
	{
		ModeS* ms = get011TrackAircraftRelatedData(c11);
		BROKEN_CONTRACT(ms, 21, memset(&r._mode_s, 0, sizeof(r._mode_s)));
		r._mode_s = *ms;
	}

	void trackNumber(Cat11& r)
	{
		r._track_number = getTrackNumber(cat).val;
	}

	void trackStatus(Cat11& r)
	{
		r._track_status = *getTrackStatus(cat);
	}

	void dataAges(Cat11& r)
	{
		r._ages.trk_age    = getTrackSystemTimeAges(cat,  STUTT_TRACK_AGE).val;
		r._ages.psr_age    = getTrackSystemTimeAges(cat,  STUTT_PSR_AGE).val;
		r._ages.ssr_age    = getTrackSystemTimeAges(cat,  STUTT_SSR_AGE).val;
		r._ages.mds_age    = getTrackSystemTimeAges(cat,  STUTT_MODE_S_AGE).val;
		r._ages.ads_age    = getTrackSystemTimeAges(cat,  STUTT_ADS_C_AGE).val;
		r._ages.ads_es_age = getTrackSystemTimeAges(cat,  STUTT_ADS_B_AGE).val;
		r._ages.mda_age    = getTrackSystemTimeAges(cat,  STUTT_MDA_AGE).val;
		r._ages.md1_age    = getTrackSystemTimeAges(cat,  STUTT_MD1_AGE).val;
		r._ages.md2_age    = getTrackSystemTimeAges(cat,  STUTT_MD2_AGE).val;
		r._ages.mfl_age    = getTrackSystemTimeAges(cat,  STUTT_MFL_AGE).val;
		r._ages.loop_age   = getTrackSystemTimeAges(cat,  STUTT_LOOP_AGE).val;
		r._ages.mlt_age    = getTrackSystemTimeAges(cat,  STUTT_MLAT_AGE).val;
	}

	void flightPhase(Cat11& r)
	{
		r._phase = (PhaseOfFlight) get011PhaseOfFlight(c11).val;
	}

	void flightLevel(Cat11& r)
	{
		r._flight_level = getTrackMeasuredFlightLevel(cat).val;
	}

	void baroAltitde(Cat11& r)
	{
		r._baro_altitude.altitude = getTrackCalcBarometricAltitude(cat).val1;
		r._baro_altitude.qnh      = getTrackCalcBarometricAltitude(cat).val2;
	}

	void geoAltitde(Cat11& r)
	{
		r._geo_altitude = getTrackCalcGeometricAltitude(cat).val;
	}

	void vertRate(Cat11& r)
	{
		r._vert_rate = getTrackCalcVerticalRate(cat).val;
	}

	void size(Cat11& r)
	{
		r._size.length      = getTrackTargetSizeLenght(cat).val;
		r._size.width       = getTrackTargetSizeWidth(cat).val;
		r._size.orientation = getTrackTargetSizeOrientation(cat).val;
	}

	void flightPlan(Cat11& r)
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

	void fleetId(Cat11& r)
	{
		r._fleet_type = FleetType(getTrackVehicleFleetIdentification(cat).val);
	}

	void programmedMessage(Cat11& r)
	{

		r._proggramed_message.type    = get011PreprogrammedMessage(c11).val1;
		r._proggramed_message.trouble = get011PreprogrammedMessage(c11).val2;
	}

	void accuracy(Cat11& r)
	{
		r._accuracy = *getTrackEstimateAccuracies(cat);
	}

	void alert(Cat11& r)
	{
		r._alert = get011AlertMessage(c11);
	}

	void alertTracks(Cat11& r)
	{
		TCommonObmenData* atr = get011AlertTracks(c11);
		BROKEN_CONTRACT(atr, 26, );
		uint16_t* d = (uint16_t*) getObmenData(atr);
		BROKEN_CONTRACT(d,   26, );
		int count = getObmenDataLenght(atr) / 2;
		r._alert_tracks.resize(count);
		for (int i = 0; i < count; ++i)
			r._alert_tracks[i] = ntohs(d[i]);
		freeObmenData(atr);
	}

	void holdbar(Cat11& r)
	{
		TCommonObmenData* hb = get011HoldbarStatus(c11);
		BROKEN_CONTRACT(hb, 27, );
		uint16_t* d = (uint16_t*) getObmenData(hb);
		BROKEN_CONTRACT(d,  27, );
		int count = getObmenDataLenght(hb) / 2;
		r._holdbar_status.resize(count);
		for (int i = 0; i < count; ++i)
		{
			uint16_t t = ntohs(d[i]);
			r._holdbar_status[i].bank      = t >> 12;
			r._holdbar_status[i].indicator = t & 0xFFF;
		}
		freeObmenData(hb);
	}

	void reserved(Cat11& r) {}
	void special(Cat11& r) {}
};

struct Cat11::Writer :
	public Uap::Serializer
{
	TCommonObmenData* odata;
	TCatAsterix*      cat;
	TCat011*          c11;

	size_t& bytes_written;

public:

	Writer(size_t& size) :
		odata(NULL), cat(NULL), c11(NULL), bytes_written(size)
	{
		cat = createCatAsterixData(11);
		c11 = get011category(cat);
	}

	~Writer()
	{
		freeCatAsterixData(cat);
		free(odata);
	}

	char* writeRecord(const Cat11& v)
	{
		Asterix::callRecord(const_cast<Cat11&>(v), this);
		odata         = pack011catRecord(c11);
		bytes_written = getObmenDataLenght(odata);
		char* data = (char*) getObmenData(odata);
		if (!data)
			BROKEN_CONTRACT_MSG();
		return data;
	}

private:

	void dataSourceId(Cat11& r)
	{
		int dsi = r._dsi.sac << 8 | r._dsi.sic;
		setDataSourceIdentificator(cat, dsi);
	}

	void messageType(Cat11& r)
	{
		set011MessageType(c11, r._message_type);
	}

	void service(Cat11& r)
	{
		setServiceIdentificator(cat, r._service);
	}

	void timeOfTrack(Cat11& r)
	{
		setTimeOfTrackInformation(cat, r._time);
	}

	void wgs84(Cat11& r)
	{
		setTrackPositionWgs84(cat, r._position.wgs84_lat, r._position.wgs84_lon);
	}

	void cartCoords(Cat11& r)
	{
		setTrackPositionCartesian(cat, r._cart_pos.x, r._cart_pos.y);
	}

	void velocity(Cat11& r)
	{
		setTrackVelocityCartesian(cat, r._velocity.vx, r._velocity.vy);
	}

	void acceleration(Cat11& r)
	{
		setTrackAccelerationCartesian(cat, r._accel.ax, r._accel.ay);
	}

	void mode3A(Cat11& r)
	{
		BoardNumCode code = {};

		code.valid             = !r._mode_3a.flags.not_validated;
		code.garbled           = r._mode_3a.flags.garbled;
		code.change            = !r._mode_3a.flags.not_extracted;
		code.board_number_code = r._mode_3a.code;

		setTrackMode3Acode(cat, &code);
	}

	void targetId(Cat11& r)
	{
		setTrackTargetIdentification(cat, r._callsign.callsign, r._callsign.sti);
	}

	void modeS(Cat11& r)
	{
		set011TrackAircraftRelatedData(c11, &r._mode_s);
	}

	void trackNumber(Cat11& r)
	{
		setTrackNumber(cat, r._track_number);
	}

	void trackStatus(Cat11& r)
	{
		setTrackStatus(cat, &r._track_status);
	}

	void dataAges(Cat11& r)
	{
		setTrackSystemTimeAges(cat, r._ages.trk_age,    STUTT_TRACK_AGE);
		setTrackSystemTimeAges(cat, r._ages.psr_age,    STUTT_PSR_AGE);
		setTrackSystemTimeAges(cat, r._ages.ssr_age,    STUTT_SSR_AGE);
		setTrackSystemTimeAges(cat, r._ages.mds_age,    STUTT_MODE_S_AGE);
		setTrackSystemTimeAges(cat, r._ages.ads_age,    STUTT_ADS_C_AGE);
		setTrackSystemTimeAges(cat, r._ages.ads_es_age, STUTT_ADS_B_AGE);
		setTrackSystemTimeAges(cat, r._ages.mda_age,    STUTT_MDA_AGE);
		setTrackSystemTimeAges(cat, r._ages.md1_age,    STUTT_MD1_AGE);
		setTrackSystemTimeAges(cat, r._ages.md2_age,    STUTT_MD2_AGE);
		setTrackSystemTimeAges(cat, r._ages.mfl_age,    STUTT_MFL_AGE);
		setTrackSystemTimeAges(cat, r._ages.loop_age,   STUTT_LOOP_AGE);
		setTrackSystemTimeAges(cat, r._ages.mlt_age,    STUTT_MLAT_AGE);
	}

	void flightPhase(Cat11& r)
	{
		set011PhaseOfFlight(c11, r._phase);
	}

	void flightLevel(Cat11& r)
	{
		setTrackMeasuredFlightLevel(cat, r._flight_level);
	}

	void baroAltitde(Cat11& r)
	{
		setTrackCalcBarometricAltitude(cat, r._baro_altitude.altitude, r._baro_altitude.qnh);
	}

	void geoAltitde(Cat11& r)
	{
		setTrackCalcGeometricAltitude(cat, r._geo_altitude);
	}

	void vertRate(Cat11& r)
	{
		setTrackCalcVerticalRate(cat, r._vert_rate);
	}

	void size(Cat11& r)
	{
		setTrackTargetSize(cat, r._size.length, r._size.width, r._size.orientation);
	}

	void flightPlan(Cat11& r)
	{
		setTrackFlightPlanData(cat, &r._flight_plan);
	}

	void fleetId(Cat11& r)
	{
		setTrackVehicleFleetIdentification(cat, r._fleet_type);
	}

	void programmedMessage(Cat11& r)
	{
		set011PreprogrammedMessage(c11, r._proggramed_message.type, r._proggramed_message.trouble);
	}

	void accuracy(Cat11& r)
	{
		setTrackEstimateAccuracies(cat, &r._accuracy);
	}

	void alert(Cat11& r)
	{
		set011AlertMessage(c11, &r._alert);
	}

	void alertTracks(Cat11& r)
	{
		TCommonObmenData* atr = initObmenData(r._alert_tracks.size() * sizeof(uint16_t));
		uint16_t*         d   = static_cast<uint16_t*>(getObmenData(atr));

		uint8_t count = r._alert_tracks.size();
		for (size_t i = 0; i < count; ++i)
			*d++ = htons(r._alert_tracks[i]);
		set011AlertTracks(c11, atr);
		freeObmenData(atr);
	}

	void holdbar(Cat11& r)
	{
		TCommonObmenData* hb = initObmenData(r._holdbar_status.size() * sizeof(uint16_t));
		uint16_t*         d  = static_cast<uint16_t*>(getObmenData(hb));

		uint8_t count = r._holdbar_status.size();
		for (size_t i = 0; i < count; ++i)
		{
			const Holdbar& h = r._holdbar_status[i];
			*d = htons((h.bank << 12) | (h.indicator & 0xFFF));
			d++;
		}
		set011HoldbarStatus(c11, hb);
		freeObmenData(hb);
	}

	void reserved(Cat11& r) {}
	void special(Cat11& r) {}
};

AsterixRecord* Cat11::decode(const char* data, size_t size, AsterixError& err, size_t& read_size)
{
	if (size == 0)
	{
		err = A_EMPTY_BUFFER;
		return NULL;
	}
	Reader reader(data, size);

	Cat11* rec = new Cat11;
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

char* Cat11::code(const AsterixRecord* record, size_t& write_size)
{
	if (record->category != 11)
		return NULL;
	Writer writer(write_size);
	return writer.writeRecord(*(const Cat11*) record);
}

}                                                          // namespace Cat11
