#include "asterix_cat_23.hpp"
#include "asterix_template.hpp"

const Ratio<1, 128>  TIME_LSB;
const Ratio<1, 1>    REPORT_TIME_LSB;
const Ratio<1852, 1> RANGE_LSB;

namespace Cat23
{
template <> void Cat23::Uap::Serializer::skip<0>(Cat23&) {};

const Cat23::Uap Cat23::UAP[UAP_SIZE] =
{
	ASTERIX_UAP_FIELD(Cat23,        0,          0, NULL,                                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat23,        1,         10, UAP_NAME("Data Source Identifier"),          2, dataSourceId),
	ASTERIX_UAP_FIELD(Cat23,        2,          0, UAP_NAME("Report Type"),                     1, reportType),
	ASTERIX_UAP_FIELD(Cat23,        3,         15, UAP_NAME("Service Type and Identification"), 1, serviceType),
	ASTERIX_UAP_FIELD(Cat23,        4,         70, UAP_NAME("Time of Day"),                     3, timeOfDay),
	ASTERIX_UAP_FIELD(Cat23,        5,        100, UAP_NAME("Ground Station Status"),           0, stationStatus),
	ASTERIX_UAP_FIELD(Cat23,        6,        101, UAP_NAME("Service Configuration"),           0, serviceConfig),
	ASTERIX_UAP_FIELD(Cat23,        7,        200, UAP_NAME("Operational Range"),               1, range),
	ASTERIX_UAP_FIELD(Cat23,        8,        110, UAP_NAME("Service Status"),                  0, status),
	ASTERIX_UAP_FIELD(Cat23,        9,        120, UAP_NAME("Service Statistics"),              0, stats),
	ASTERIX_UAP_FIELD(Cat23,       10,          0, NULL,                                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat23,       11,          0, NULL,                                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat23,       12,          0, NULL,                                        0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat23,       13,          0, UAP_NAME("Reserved Expansion Field"),        0, reserved),
	ASTERIX_UAP_FIELD(Cat23,       14,          0, UAP_NAME("Special Purpose Field"),           0, special),
	ASTERIX_UAP_FIELD(Cat23, LAST_FRN + RE_END, 0, NULL,                                        0, reserved_end),
};

struct Cat23::Reader :
	public Uap::Serializer, public AsterixReaderT
{
public:

	Reader(const char* data, size_t size) :
		AsterixReaderT(data, size) {};

	void readRecord(Cat23& v)
	{
		Asterix::readRecord(v, this);
	}

private:

	void dataSourceId(Cat23& r)
	{
		r._dsi.sac = readInt<8, unsigned>();
		r._dsi.sic = readInt<8, unsigned>();
	}

	void reportType(Cat23& r)
	{
		r._report_type = readInt<8, unsigned>();
	}

	void timeOfDay(Cat23& r)
	{
		r._time = readDouble<24, unsigned>(TIME_LSB);
	}

	void serviceType(Cat23& r)
	{
		uint8_t type = readInt<8, unsigned>();
		r._service_type.sid  = type >> 4;
		r._service_type.styp = type & 0xF;
	}

	void stationStatus(Cat23& r)
	{
		uint64_t             flags = readFxFlags();
		GroundStationStatus& s     = r._station_status;
		s.nogo = flags >> 7 & 1;
		s.odp  = flags >> 6 & 1;
		s.oxt  = flags >> 5 & 1;
		s.msc  = flags >> 4 & 1;
		s.tsv  = flags >> 3 & 1;
		s.spo  = flags >> 2 & 1;
		s.rn   = flags >> 1 & 1;

		uint8_t gssp = flags >> 9 & 0x7F;
		s.gssp = gssp ? gssp * REPORT_TIME_LSB : __noop; // NAN;	// for Linux
	}

	void serviceConfig(Cat23& r)
	{
		r._service_conf.rp = readInt<8, unsigned>();
		uint64_t flags = readFxFlags();
		r._service_conf.sc = flags >> 5 & 0x7;
		uint8_t ssrp = flags >> 9 & 0x7F;
		r._service_conf.ssrp = ssrp ? ssrp * REPORT_TIME_LSB : __noop;	// NAN;	for Linux
	}

	void range(Cat23& r)
	{
		r._range = readDouble<8, unsigned>(RANGE_LSB);
	}

	void status(Cat23& r)
	{
		uint64_t flags = readFxFlags();
		r._status.stat = flags  >> 1 & 0x7;
	}

	void stats(Cat23& r)
	{
		uint8_t count = readInt<8, unsigned>();
		r._stats.resize(count);
		for (uint8_t i = 0; i < count; ++i)
		{
			uint16_t flags = readInt<16, unsigned>();
			r._stats[i].type  = flags >> 8;
			r._stats[i].ref   = ServiceStats::Ref(flags >> 7 & 0x1);
			r._stats[i].value = readInt<32, unsigned>();
		}
	}

	void reserved(Cat23& r)
	{
		AsterixReaderT::reserved(&r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat23&)
	{
		AsterixReaderT::reserved_end();
	}

	void special(Cat23& r)
	{
		AsterixReaderT::special(r._sp);
	}

};

struct Cat23::Writer :
	public Uap::Serializer, public AsterixWriterT
{
public:

	Writer(char*& data, size_t& size) :
		AsterixWriterT(data, size) {};

	void writeRecord(const Cat23& v)
	{
		Asterix::writeRecord(v, this);
	}

private:

	void dataSourceId(Cat23& r)
	{
		writeInt<8, unsigned>(r._dsi.sac);
		writeInt<8, unsigned>(r._dsi.sic);
	}

	void reportType(Cat23& r)
	{
		writeInt<8, unsigned>(r._report_type);
	}

	void timeOfDay(Cat23& r)
	{
		writeDouble<24, unsigned>(TIME_LSB, r._time);
	}

	void serviceType(Cat23& r)
	{
		writeInt<8, unsigned>(r._service_type.sid << 4 | r._service_type.styp);
	}

	void stationStatus(Cat23& r)
	{
		uint64_t             flags = 0;
		GroundStationStatus& s     = r._station_status;
		flags |= s.nogo << 7;
		flags |= s.odp << 6;
		flags |= s.oxt << 5;
		flags |= s.msc << 4;
		flags |= s.tsv << 3;
		flags |= s.spo << 2;
		flags |= s.rn << 1;

		/*if (isfinite(s.gssp))	// for Linux
		{
			uint8_t gssp = s.gssp / REPORT_TIME_LSB;
			flags |= gssp << 9;
		}
		writeFxFlags(flags);*/
	}

	void serviceConfig(Cat23& r)
	{
		writeInt<8, unsigned>(r._service_conf.rp);
		uint64_t flags = 0;
		flags |= r._service_conf.sc << 5;
		/*if (isfinite(r._service_conf.ssrp))	// for Linux
		{
			uint8_t ssrp = r._service_conf.ssrp / REPORT_TIME_LSB;
			flags |= ssrp << 9;
		}
		writeFxFlags(flags);*/
	}

	void range(Cat23& r)
	{
		writeDouble<8, unsigned>(RANGE_LSB, r._range);
	}

	void status(Cat23& r)
	{
		uint64_t flags = 0;
		flags |= r._status.stat <<  1;
		writeFxFlags(flags);
	}

	void stats(Cat23& r)
	{
		uint8_t count = r._stats.size();
		writeInt<8, unsigned>(count);

		for (uint8_t i = 0; i < count; ++i)
		{
			uint16_t flags = 0;
			flags |= r._stats[i].type << 8;
			flags |= r._stats[i].ref << 7;
			writeInt<16, unsigned>(flags);
			writeInt<32, unsigned>(r._stats[i].value);
		}
	}

	void reserved(Cat23& r)
	{
		AsterixWriterT::reserved(r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat23&)
	{
		AsterixWriterT::reserved_end();
	}

	void special(Cat23& r)
	{
		AsterixWriterT::special(r._sp);
	}

};

AsterixRecord* Cat23::decode(const char* data, size_t size, AsterixError& err, size_t& read_size)
{
	if (size == 0)
	{
		err = A_EMPTY_BUFFER;
		return NULL;
	}
	Reader reader(data, size);

	Cat23* rec = new Cat23;
	reader.readRecord(*rec);
	read_size = reader.bytesRead();

	err = A_NO_ERROR;
	return rec;
}

char* Cat23::code(const AsterixRecord* record, size_t& write_size)
{
	if (record->category != 23)
		return NULL;
	char* data;
	{
		Writer writer(data, write_size);
		writer.writeRecord(*(const Cat23*) record);
	}
	return data;
}

}                                                          // namespace Cat19
