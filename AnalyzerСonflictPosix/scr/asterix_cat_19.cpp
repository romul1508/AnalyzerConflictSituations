#include "asterix_cat_19.hpp"
#include "asterix_template.hpp"

const Ratio<180, 1 << 30> WGS84_LSB_CAT19;
const Ratio<1, 128>       TIME_LSB;
const Ratio<1, 4>         REF_ALT_LSB;
const Ratio<1, 1>         WGS_UNDUL;

namespace Cat19
{
template <> void Cat19::Uap::Serializer::skip<0>(Cat19&) {};

const Cat19::Uap Cat19::UAP[UAP_SIZE] =
{
	ASTERIX_UAP_FIELD(Cat19,        0,          0, NULL,                                                   0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat19,        1,         10, UAP_NAME("Data Source Identifier"),                     2, dataSourceId),
	ASTERIX_UAP_FIELD(Cat19,        2,          0, UAP_NAME("Message Type"),                               1, messageType),
	ASTERIX_UAP_FIELD(Cat19,        3,        140, UAP_NAME("Time of Day"),                                3, timeOfDay),
	ASTERIX_UAP_FIELD(Cat19,        4,        550, UAP_NAME("System Status"),                              1, systemStatus),
	ASTERIX_UAP_FIELD(Cat19,        5,        551, UAP_NAME("Tracking Processor Detailed Status"),         1, trackingStatus),
	ASTERIX_UAP_FIELD(Cat19,        6,        552, UAP_NAME("Remote Sensor Detailed Status"),              0, sensorStatus),
	ASTERIX_UAP_FIELD(Cat19,        7,        553, UAP_NAME("Reference Transponder Detailed Status"),      0, transponderStatus),
	ASTERIX_UAP_FIELD(Cat19,        8,        600, UAP_NAME("Position of the MLT System Reference point"), 8, refPoint),
	ASTERIX_UAP_FIELD(Cat19,        9,        610, UAP_NAME("Height of the MLT System Reference point"),   2, refPointHeight),
	ASTERIX_UAP_FIELD(Cat19,       10,        620, UAP_NAME("WGS-84 Undulation"),                          1, wgsUndulation),
	ASTERIX_UAP_FIELD(Cat19,       11,          0, NULL,                                                   0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat19,       12,          0, NULL,                                                   0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat19,       13,          0, UAP_NAME("Reserved Expansion Field"),                   0, reserved),
	ASTERIX_UAP_FIELD(Cat19,       14,          0, UAP_NAME("Special Purpose Field"),                      0, special),
	ASTERIX_UAP_FIELD(Cat19, LAST_FRN + RE_END, 0, NULL,                                                   0, reserved_end),
};

struct Cat19::Reader :
	public Uap::Serializer, public AsterixReaderT
{
public:

	Reader(const char* data, size_t size) :
		AsterixReaderT(data, size) {};

	void readRecord(Cat19& v)
	{
		Asterix::readRecord(v, this);
	}

private:

	void dataSourceId(Cat19& r)
	{
		r._dsi.sac = readInt<8, unsigned>();
		r._dsi.sic = readInt<8, unsigned>();
	}

	void messageType(Cat19& r)
	{
		r._message_type = readInt<8, unsigned>();
	}

	void timeOfDay(Cat19& r)
	{
		r._time = readDouble<24, unsigned>(TIME_LSB);
	}

	void reserved(Cat19& r)
	{
		AsterixReaderT::reserved(&r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat19&)
	{
		AsterixReaderT::reserved_end();
	}

	void special(Cat19& r)
	{
		AsterixReaderT::special(r._sp);
	}

	void sensorStatus(Cat19& r)
	{
		uint8_t count = readInt<8, unsigned>();
		r._sensor_status.resize(count);
		for (int sensor = 0; sensor < count; ++sensor)
		{
			r._sensor_status[sensor].rsid = readInt<8, unsigned>();
			uint8_t status = readInt<8, unsigned>();

			r._sensor_status[sensor].receiver_1090    = status & (1 << 6);
			r._sensor_status[sensor].transmitter_1030 = status & (1 << 5);
			r._sensor_status[sensor].transmitter_1090 = status & (1 << 4);
			r._sensor_status[sensor].rs_good          = status & (1 << 3);
			r._sensor_status[sensor].rs_online        = status & (1 << 2);
		}
	}

	void transponderStatus(Cat19& r)
	{
		uint8_t flags;
		do
		{
			flags = readInt<8, unsigned>();
			r._ref_transponder_status.push_back(RefTransponderStatus((flags >> 6) & 0x3));
			r._ref_transponder_status.push_back(RefTransponderStatus((flags >> 2) & 0x3));
		}
		while (flags & 0x1);
	}

	void refPoint(Cat19& r)
	{
		r._ref_point.wgs84_lat = readDouble<32>(WGS84_LSB_CAT19);
		r._ref_point.wgs84_lon = readDouble<32>(WGS84_LSB_CAT19);
	}

	void refPointHeight(Cat19& r)
	{
		r._ref_point_altitude = readDouble<16>(REF_ALT_LSB);
	}

	void wgsUndulation(Cat19& r)
	{
		r._wgs84_undulation = readDouble<8, signed>(WGS_UNDUL);
	}

	void trackingStatus(Cat19& r)
	{
		uint8_t flags = readInt<8, unsigned>();
		r._tracking_status.tp1_exec = flags & (1 << 7);
		r._tracking_status.tp1_good = flags & (1 << 6);
		r._tracking_status.tp2_exec = flags & (1 << 5);
		r._tracking_status.tp2_good = flags & (1 << 4);
		r._tracking_status.tp3_exec = flags & (1 << 3);
		r._tracking_status.tp3_good = flags & (1 << 2);
		r._tracking_status.tp4_exec = flags & (1 << 1);
		r._tracking_status.tp4_good = flags & (1 << 0);
	}

	void systemStatus(Cat19& r)
	{
		uint8_t flags = readInt<8, unsigned>();
		r._system_status.rel_status     = SystemStatus::ReleaseStatus(flags >> 6);
		r._system_status.overload       = flags & (1 << 5);
		r._system_status.invalid_time   = flags & (1 << 4);
		r._system_status.target_failure = flags & (1 << 3);
	}

};

struct Cat19::Writer :
	public Uap::Serializer, public AsterixWriterT
{
public:

	Writer(char*& data, size_t& size) :
		AsterixWriterT(data, size) {};

	void writeRecord(const Cat19& v)
	{
		Asterix::writeRecord(v, this);
	}

private:

	void dataSourceId(Cat19& r)
	{
		writeInt<8, unsigned>(r._dsi.sac);
		writeInt<8, unsigned>(r._dsi.sic);
	}

	void messageType(Cat19& r)
	{
		writeInt<8, unsigned>(r._message_type);
	}

	void timeOfDay(Cat19& r)
	{
		writeDouble<24, unsigned>(TIME_LSB, r._time);
	}

	void reserved(Cat19& r)
	{
		AsterixWriterT::reserved(r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat19&)
	{
		AsterixWriterT::reserved_end();
	}

	void special(Cat19& r)
	{
		AsterixWriterT::special(r._sp);
	}

	void sensorStatus(Cat19& r)
	{
		uint8_t count = r._sensor_status.size();
		writeInt<8, unsigned>(count);

		for (int sensor = 0; sensor < count; ++sensor)
		{
			writeInt<8, unsigned>(r._sensor_status[sensor].rsid);

			uint8_t status = 0;
			status |= r._sensor_status[sensor].receiver_1090 << 6;
			status |= r._sensor_status[sensor].transmitter_1030 << 5;
			status |= r._sensor_status[sensor].transmitter_1090 << 4;
			status |= r._sensor_status[sensor].rs_good << 3;
			status |= r._sensor_status[sensor].rs_online << 2;

			writeInt<8, unsigned>(status);
		}
	}

	void transponderStatus(Cat19& r)
	{
		typedef std::vector<RefTransponderStatus>::iterator It;
		for (It it = r._ref_transponder_status.begin(), end = r._ref_transponder_status.end(); it != end;)
		{
			uint8_t flags1 = 0, flags2 = 0;
			bool    next = false;
			flags1 = *it << 2;
			++it;
			if (it != end)
			{
				flags2 = *it << 2;
				next   = (it + 1) != end;
				++it;
			}
			uint8_t flags = flags1 << 4 | flags2 | next;
			writeInt<8, unsigned>(flags);
		}
	}

	void refPoint(Cat19& r)
	{
		writeDouble<32>(WGS84_LSB_CAT19, r._ref_point.wgs84_lat);
		writeDouble<32>(WGS84_LSB_CAT19, r._ref_point.wgs84_lon);
	}

	void refPointHeight(Cat19& r)
	{
		writeDouble<16>(REF_ALT_LSB, r._ref_point_altitude);
	}

	void wgsUndulation(Cat19& r)
	{
		writeDouble<8, signed>(WGS_UNDUL, r._wgs84_undulation);
	}

	void trackingStatus(Cat19& r)
	{
		uint8_t flags = 0;
		flags |= r._tracking_status.tp1_exec << 7;
		flags |= r._tracking_status.tp1_good << 6;
		flags |= r._tracking_status.tp2_exec << 5;
		flags |= r._tracking_status.tp2_good << 4;
		flags |= r._tracking_status.tp3_exec << 3;
		flags |= r._tracking_status.tp3_good << 2;
		flags |= r._tracking_status.tp4_exec << 1;
		flags |= r._tracking_status.tp4_good << 0;
		writeInt<8, unsigned>(flags);
	}

	void systemStatus(Cat19& r)
	{
		uint8_t flags = 0;
		flags |= r._system_status.rel_status << 6;
		flags |= r._system_status.overload << 5;
		flags |= r._system_status.invalid_time << 4;
		flags |= r._system_status.target_failure << 3;
		writeInt<8, unsigned>(flags);
	}

};

AsterixRecord* Cat19::decode(const char* data, size_t size, AsterixError& err, size_t& read_size)
{
	if (size == 0)
	{
		err = A_EMPTY_BUFFER;
		return NULL;
	}
	Reader reader(data, size);

	Cat19* rec = new Cat19;
	reader.readRecord(*rec);
	read_size = reader.bytesRead();

	err = A_NO_ERROR;
	return rec;
}

char* Cat19::code(const AsterixRecord* record, size_t& write_size)
{
	if (record->category != 19)
		return NULL;
	char* data;
	{
		Writer writer(data, write_size);
		writer.writeRecord(*(const Cat19*) record);
	}
	return data;
}

}                                                          // namespace Cat19
