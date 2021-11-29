#include "asterix_cat_10.hpp"
#include "asterix_template.hpp"
#include <stdlib.h>

#define BROKEN_CONTRACT(expr, frn, clean) do {                                                                          \
		if (!expr) {                                                                                            \
			fprintf(stderr, "Asterix lib contract broken at %s:%d %s\n", __FILE__, __LINE__, __FUNCTION__); \
			r._fspec &= ~(1 << frn);                                                                        \
			clean;                                                                                          \
			return;                                                                                         \
		}                                                                                                       \
} while (0)

namespace Cat10
{
template <> void Cat10::Uap::Serializer::skip<0>(Cat10&) {};

const Cat10::Uap Cat10::UAP[UAP_SIZE] =
{
	ASTERIX_UAP_FIELD(Cat10, 0,  0,   "Field Extension Indicator",                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat10, 1,  10,  "Data Source Identifier",                           2, dataSourceId),
	ASTERIX_UAP_FIELD(Cat10, 2,  0,   "Message Type",                                     1, messageType),
	ASTERIX_UAP_FIELD(Cat10, 3,  20,  "Target Report Descriptor",                         0, targetReportDescriptor),
	ASTERIX_UAP_FIELD(Cat10, 4,  140, "Time of Day",                                      3, timeOfDay),
	ASTERIX_UAP_FIELD(Cat10, 5,  41,  "Position in WGS-84 Co-ordinates",                  8, wgs84),
	ASTERIX_UAP_FIELD(Cat10, 6,  40,  "Measured Position in Polar Co-ordinates",          4, polarCoords),
	ASTERIX_UAP_FIELD(Cat10, 7,  42,  "Position in Cartesian Co-ordinates",               4, cartCoords),
	ASTERIX_UAP_FIELD(Cat10, 8,  200, "Calculated Track Velocity in Polar Co-ordinates ", 4, polarVelocity),
	ASTERIX_UAP_FIELD(Cat10, 9,  202, "Calculated Track Velocity in Cartesian Coord.",    4, velocity),
	ASTERIX_UAP_FIELD(Cat10, 10, 161, "Track Number",                                     2, trackNumber),
	ASTERIX_UAP_FIELD(Cat10, 11, 170, "Track Status",                                     0, trackStatus),
	ASTERIX_UAP_FIELD(Cat10, 12, 60,  "Mode-3/A Code in Octal Representation",            2, mode3A),
	ASTERIX_UAP_FIELD(Cat10, 13, 220, "Target Address",                                   3, targetAddress),
	ASTERIX_UAP_FIELD(Cat10, 14, 245, "Target Identification",                            7, targetId),
	ASTERIX_UAP_FIELD(Cat10, 15, 250, "Mode S MB Data",                                   0, modeS),
	ASTERIX_UAP_FIELD(Cat10, 16, 300, "Vehicle Fleet Identification",                     1, fleetId),
	ASTERIX_UAP_FIELD(Cat10, 17, 90,  "Flight Level in Binary Representation",            2, flightLevel),
	ASTERIX_UAP_FIELD(Cat10, 18, 91,  "Measured Height",                                  2, measuredHeight),
	ASTERIX_UAP_FIELD(Cat10, 19, 270, "Target Size & Orientation",                        0, size),
	ASTERIX_UAP_FIELD(Cat10, 20, 550, "System Status",                                    1, systemStatus),
	ASTERIX_UAP_FIELD(Cat10, 21, 310, "Pre-programmed Message",                           1, programmedMessage),
	ASTERIX_UAP_FIELD(Cat10, 22, 500, "Standard Deviation of Position",                   4, accuracy),
	ASTERIX_UAP_FIELD(Cat10, 23, 280, "Presence",                                         0, presence),
	ASTERIX_UAP_FIELD(Cat10, 24, 131, "Amplitude of Primary Plot",                        1, amplitude),
	ASTERIX_UAP_FIELD(Cat10, 25, 210, "Calculated Acceleration",                          2, acceleration),
	ASTERIX_UAP_FIELD(Cat10, 26, 0,   "Spare",                                            0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat10, 27, 0,   "Special Purpose Field",                            0, reserved),
	ASTERIX_UAP_FIELD(Cat10, 28, 0,   "Reserved Expansion Field",                         0, special),
};

struct Cat10::Reader :
	public Uap::Serializer
{
	TCommonObmenData* odata;
	TCatAsterix*      cat;
	TCat010*          c10;
	size_t            bytes_read;

public:

	Reader(const char* data, size_t size) :
		odata(NULL), cat(NULL), c10(NULL)
	{
		bytes_read = 0;
		odata      = initObmenData(size);
		setObmenData(odata, (void*) data, size);
		cat = decode010catRecord(odata, &bytes_read);
		c10 = get010category(cat);
	};

	~Reader()
	{
		freeCatAsterixData(cat);
		freeObmenData(odata);
	}

	void readRecord(Cat10& v)
	{
		size_t   size;
		uint8_t* f = get010recordFspec(c10, &size);
		v._fspec = AsterixReader::readFspec(f, size);
		Asterix::callRecord(v, this);
	}

	size_t bytesRead()
	{
		return bytes_read;
	}

private:

	void dataSourceId(Cat10& r)
	{
		uint32_t dsi = getDataSourceIdentificator(cat).val;
		r._dsi.sac = dsi >> 8 & 0xFF;
		r._dsi.sic = dsi & 0xFF;
	}

	void messageType(Cat10& r)
	{
		r._message_type = get010MessageType(c10).val;
	}

	void targetReportDescriptor(Cat10& r)
	{
		r._target_report_dscr = *get010TargetReportDescriptor(c10);
	}

	void timeOfDay(Cat10& r)
	{
		r._time = getTimeOfTrackInformation(cat).val;
	}

	void wgs84(Cat10& r)
	{
		r._position.wgs84_lat = getTrackPositionWgs84(cat).val1;
		r._position.wgs84_lon = getTrackPositionWgs84(cat).val2;
	}

	void polarCoords(Cat10& r)
	{
		r._polar_pos.thetha = getTrackPositionPolar(cat).val1;
		r._polar_pos.rho    = getTrackPositionPolar(cat).val2;
	}

	void cartCoords(Cat10& r)
	{
		r._cart_pos.x = getTrackPositionCartesian(cat).val1;
		r._cart_pos.y = getTrackPositionCartesian(cat).val2;
	}

	void polarVelocity(Cat10& r)
	{
		r._polar_vel.speed   = getTrackVelocityPolar(cat).val1;
		r._polar_vel.heading = getTrackVelocityPolar(cat).val2;
	}

	void velocity(Cat10& r)
	{
		r._velocity.vx = getTrackVelocityCartesian(cat).val1;
		r._velocity.vy = getTrackVelocityCartesian(cat).val2;
	}

	void trackNumber(Cat10& r)
	{
		r._track_number = getTrackNumber(cat).val;
	}

	void trackStatus(Cat10& r)
	{
		r._track_status = *get010TrackStatus(c10);
	}

	void mode3A(Cat10& r)
	{
		BoardNumCode code = getTrackMode3Acode(cat);

		r._mode_3a.flags.not_validated = !code.valid;
		r._mode_3a.flags.garbled       = code.garbled;
		r._mode_3a.flags.not_extracted = !code.change;
		r._mode_3a.code                = code.board_number_code;
	}

	void targetAddress(Cat10& r)
	{
		r._target_address = getTrackTargetAddress(cat).val;
	}

	void targetId(Cat10& r)
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

	void modeS(Cat10& r)
	{
		/*TCommonObmenData* modes = get010ModeSMBdata(c10);	// for Linux
		BROKEN_CONTRACT(modes, 15, );
		uint64_t* _data = static_cast<uint64_t*>(getObmenData(modes));
		BROKEN_CONTRACT(modes, 15, freeObmenData(modes));

		uint8_t count = getObmenDataLenght(modes) / sizeof(uint64_t);
		while (count--)
		{
			uint64_t data = be64toh(*_data++);
			r._mode_s.push_back(ModeS());
			struct ModeS& s = r._mode_s.back();
			s.data = data >> 8;
			s.bds  = data & 0xFF;
		}

		freeObmenData(modes);*/
	}

	void fleetId(Cat10& r)
	{
		r._fleet_type = FleetType(getTrackVehicleFleetIdentification(cat).val);
	}

	void flightLevel(Cat10& r)
	{
		r._flight_level = getTrackMeasuredFlightLevel(cat).val;
	}

	void measuredHeight(Cat10& r)
	{
		r._measured_altitude = getTrackCalcGeometricAltitude(cat).val;
	}

	void size(Cat10& r)
	{
		r._size.length      = getTrackTargetSizeLenght(cat).val;
		r._size.width       = getTrackTargetSizeWidth(cat).val;
		r._size.orientation = getTrackTargetSizeOrientation(cat).val;
	}

	void systemStatus(Cat10& r)
	{
		r._system_status = *get010systemStatus(c10);
	}

	void programmedMessage(Cat10& r)
	{

		r._proggramed_message.type    = get010PreprogrammedMessage(c10).val1;
		r._proggramed_message.trouble = get010PreprogrammedMessage(c10).val2;
	}

	void accuracy(Cat10& r)
	{
		TTrackAccuracy a = *getTrackEstimateAccuracies(cat);
		r._accuracy.sdp_x  = a.x_component;
		r._accuracy.sdp_y  = a.y_component;
		r._accuracy.sdp_xy = a.xy_cov_component;
	}

	void presence(Cat10& r)
	{
		PresenceData* pdata = get010PresenceData(c10);
		// Неверная кодограмма
		if (!pdata)
			return;
		r._presence.resize(pdata->presence_count);

		for (size_t i = 0; i < pdata->presence_count; ++i)
		{
			r._presence[i].thetha = pdata->presence_recs[i].azimuth;
			r._presence[i].rho    = pdata->presence_recs[i].distance;
		}
		freePresenceData(pdata);
	}

	void amplitude(Cat10& r)
	{
		r._amplitude = get010plotAmplitude(c10).val;
	}

	void acceleration(Cat10& r)
	{
		r._accel.ax = getTrackAccelerationCartesian(cat).val1;
		r._accel.ay = getTrackAccelerationCartesian(cat).val2;
	}

	void reserved(Cat10& r) {}
	void special(Cat10&) {}

};

struct Cat10::Writer :
	public Uap::Serializer
{
	TCommonObmenData* odata;
	TCatAsterix*      cat;
	TCat010*          c10;

	size_t& bytes_written;

public:

	Writer(size_t& size) :
		odata(NULL), cat(NULL), c10(NULL), bytes_written(size)
	{
		cat = createCatAsterixData(10);
		c10 = get010category(cat);
	}

	~Writer()
	{
		freeCatAsterixData(cat);
		free(odata);
	}

	char* writeRecord(const Cat10& v)
	{
		Asterix::callRecord(const_cast<Cat10&>(v), this);
		odata         = pack010catRecord(c10);
		bytes_written = getObmenDataLenght(odata);
		return (char*) getObmenData(odata);
	}

private:

	void dataSourceId(Cat10& r)
	{
		int dsi = r._dsi.sac << 8 | r._dsi.sic;
		setDataSourceIdentificator(cat, dsi);
	}

	void messageType(Cat10& r)
	{
		set010MessageType(c10, r._message_type);
	}

	void targetReportDescriptor(Cat10& r)
	{
		set010TargetReportDescriptor(c10, &r._target_report_dscr);
	}

	void timeOfDay(Cat10& r)
	{
		setTimeOfTrackInformation(cat, r._time);
	}

	void wgs84(Cat10& r)
	{
		setTrackPositionWgs84(cat, r._position.wgs84_lat, r._position.wgs84_lon);
	}

	void polarCoords(Cat10& r)
	{
		setTrackPositionPolar(cat, r._polar_pos.thetha, r._polar_pos.rho);
	}

	void cartCoords(Cat10& r)
	{
		setTrackPositionCartesian(cat, r._cart_pos.x, r._cart_pos.y);
	}

	void polarVelocity(Cat10& r)
	{
		setTrackVelocityPolar(cat, r._polar_vel.speed, r._polar_vel.heading);
	}

	void velocity(Cat10& r)
	{
		setTrackVelocityCartesian(cat, r._velocity.vx, r._velocity.vy);
	}

	void trackNumber(Cat10& r)
	{
		setTrackNumber(cat, r._track_number);
	}

	void trackStatus(Cat10& r)
	{
		set010TrackStatus(c10, &r._track_status);
	}

	void mode3A(Cat10& r)
	{
		BoardNumCode code = {};

		code.valid             = !r._mode_3a.flags.not_validated;
		code.garbled           = r._mode_3a.flags.garbled;
		code.change            = !r._mode_3a.flags.not_extracted;
		code.board_number_code = r._mode_3a.code;

		setTrackMode3Acode(cat, &code);
	}

	void targetAddress(Cat10& r)
	{
		setTrackTargetAddress(cat, r._target_address);
	}

	void targetId(Cat10& r)
	{
		setTrackTargetIdentification(cat, r._callsign.callsign, r._callsign.sti);
	}

	void modeS(Cat10& r)
	{
		/*TCommonObmenData* modes = initObmenData(r._mode_s.size() * sizeof(uint64_t));	// for Linux
		uint64_t*         data  = static_cast<uint64_t*>(getObmenData(modes));

		uint8_t count = r._mode_s.size();
		for (size_t i = 0; i < count; ++i)
		{
			struct ModeS& s = r._mode_s[i];
			*data++ = htobe64(s.data << 8 | s.bds);
		}
		set010ModeSMBdata(c10, modes);
		freeObmenData(modes);*/
	}

	void fleetId(Cat10& r)
	{
		setTrackVehicleFleetIdentification(cat, r._fleet_type);
	}

	void flightLevel(Cat10& r)
	{
		setTrackMeasuredFlightLevel(cat, r._flight_level);
	}

	void measuredHeight(Cat10& r)
	{
		setTrackCalcGeometricAltitude(cat, r._measured_altitude);
	}

	void size(Cat10& r)
	{
		setTrackTargetSize(cat, r._size.length, r._size.width, r._size.orientation);
	}

	void systemStatus(Cat10& r)
	{
		set010systemStatus(c10, &r._system_status);
	}

	void programmedMessage(Cat10& r)
	{
		set010PreprogrammedMessage(c10, r._proggramed_message.type, r._proggramed_message.trouble);
	}

	void accuracy(Cat10& r)
	{
		TTrackAccuracy a;
		a.x_component      = r._accuracy.sdp_x;
		a.y_component      = r._accuracy.sdp_y;
		a.xy_cov_component = r._accuracy.sdp_xy;
		setTrackEstimateAccuracies(cat, &a);
	}

	void presence(Cat10& r)
	{
		std::vector<PresencePositionPolar> _pdata(r._presence.size());
		PresenceData                       pdata;
		pdata.presence_count = _pdata.size();
		for (size_t i = 0; i < pdata.presence_count; ++i)
		{
			_pdata[i].azimuth  = r._presence[i].thetha;
			_pdata[i].distance = r._presence[i].rho;
		}
		pdata.presence_recs = _pdata.data();
		set010PresenceData(c10, &pdata);
	}

	void amplitude(Cat10& r)
	{
		set010plotAmplitude(c10, r._amplitude);
	}

	void acceleration(Cat10& r)
	{
		setTrackAccelerationCartesian(cat, r._accel.ax, r._accel.ay);
	}

	void reserved(Cat10& r) {}
	void special(Cat10& r) {}

};

AsterixRecord* Cat10::decode(const char* data, size_t size, AsterixError& err, size_t& read_size)
{
	if (size == 0)
	{
		err = A_EMPTY_BUFFER;
		return NULL;
	}
	Reader reader(data, size);

	Cat10* rec = new Cat10;
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

char* Cat10::code(const AsterixRecord* record, size_t& write_size)
{
	if (record->category != 10)
		return NULL;
	Writer writer(write_size);
	return writer.writeRecord(*(const Cat10*) record);
}

}                                                          // namespace Cat10
