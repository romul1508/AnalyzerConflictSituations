#ifndef ASTERIX_CAT_23_HPP
#define ASTERIX_CAT_23_HPP

#include "asterix.hpp"
#include "asterix_internal.hpp"

namespace Cat23
{
using Asterix::DataSourceId;

enum ReportType
{
	RT_GROUND_STATION_STATUS = 1,
	RT_SERVICE_STATUS        = 2,
	RT_SERVICE_STATISTICS    = 3
};

struct ServiceType
{
	enum
	{
		ST_ADSB_VDL4 = 1,
		ST_ADSB_ES   = 2,
		ST_ADSB_UAT  = 3,
		ST_TISB_VDL4 = 4,
		ST_TISB_ES   = 5,
		ST_TISB_UAT  = 6,
		ST_FISB_VDL4 = 7,
		ST_GRAS_VDL4 = 8,
		ST_MLT       = 9,
	};
	uint8_t sid;
	uint8_t styp;
};

struct GroundStationStatus
{
	bool   nogo : 1;
	bool   odp  : 1;
	bool   oxt  : 1;
	bool   msc  : 1;
	bool   tsv  : 1;
	bool   spo  : 1;
	bool   rn   : 1;
	double gssp;
};

struct ServiceConfig
{
	double  rp;
	uint8_t sc : 3;
	double  ssrp;
};

struct ServiceStatus
{
	enum
	{
		ST_UNKNOWN      = 0,
		ST_FAILED       = 1,
		ST_DISABLED     = 2,
		ST_DEGRADED     = 3,
		ST_NORMAL       = 4,
		ST_INITIALISING = 5,
	};
	uint8_t stat : 3;
};

struct ServiceStats
{
	enum Ref
	{
		REF_MIDNIGHT    = 0,
		REF_LAST_REPORT = 1,
	};

	enum Type
	{
		T_UNK                 = 0,                 // Number of unknown messages received
		T_TOO_OLD             = 1,                 // Number of ‘too old’ messages received
		T_FAILED_CONV         = 2,                 // Number of failed message conversions
		T_TOTAL_MSG_RECV      = 3,                 // Total Number of messages received
		T_TOTAL_MSG_SEND      = 4,                 // Total number of messages transmitted
		T_TISB_RECV           = 20,                // Number of TIS-B management messages received
		T_BASIC_RECV          = 21,                // Number of ‘Basic’ messages received
		T_HD_RECV             = 22,                // Number of ‘High Dynamic’ messages received
		T_FULL_POS_RECV       = 23,                // Number of ‘Full Position’ messages received
		T_BASIC_GRND_RECV     = 24,                // Number of ‘Basic Ground ‘ messages received
		T_TCP_RECV            = 25,                // Number of ‘TCP’ messages received
		T_UTC_RECV            = 26,                // Number of ‘UTC time ‘ messages received
		T_DATA_RECV           = 27,                // Number of “Data’ messages received
		T_HR_RECV             = 28,                // Number of ‘High Resolution’ messages received
		T_AIRBORNE_RECV       = 29,                // Number of ‘Aircraft Target Airborne’ messages received.
		T_GROUND_RECV         = 30,                // Number of ‘Aircraft Target ‘Ground’ messages received.
		T_GROUND_VEHICLE_RECV = 31,                // Number of ‘Ground Vehicle Target’ messages received.
		T_2TCP_RECV           = 32,                // Number of ‘2 slots TCP messages received
	};

	uint8_t  type;
	Ref      ref;
	uint32_t value;
};

/// Категория 23
struct Cat23 :
	public AsterixRecord
{
	struct Reader;
	struct Writer;

	/// Структура-описание полей asterix
	struct Uap
	{
		Asterix::UapInfo info;
		struct Serializer;
		void (Serializer::* serialize)(Cat23&);                  ///< Функция-обработчик
	};

	static const int RE_FRN   = 13;
	static const int RE_END   = 0;
	static const int LAST_FRN = 15;
	static const int UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId,        Cat23, 1, _dsi);            ///< Data Source Identifier
	ASTERIX_DECLARE(uint8_t,             Cat23, 2, _report_type);    ///< Report Type
	ASTERIX_DECLARE(ServiceType,         Cat23, 3, _service_type);   ///< Service Type and Identification
	ASTERIX_DECLARE(double,              Cat23, 4, _time);           ///< Time of Day
	ASTERIX_DECLARE(GroundStationStatus, Cat23, 5, _station_status); ///< Ground Station Status
	ASTERIX_DECLARE(ServiceConfig,       Cat23, 6, _service_conf);   ///< Service Configuration
	ASTERIX_DECLARE(double,              Cat23, 7, _range);          ///< Operational Range
	ASTERIX_DECLARE(ServiceStatus,       Cat23, 8, _status);         ///< Service Status

	ASTERIX_DECLARE_ARRAY(ServiceStats, Cat23, 9, _stats);           ///< Service Statistics

	ASTERIX_DECLARE(uint8_t, Cat23, 13, _re);                        ///< Reserved Expansion Field
	ASTERIX_DECLARE_ARRAY(uint8_t, Cat23, 14, _sp);                  ///< Special Purpose Field
	ASTERIX_DECLARE(uint8_t, Cat23, LAST_FRN + RE_END, _re_end);

public:

	Cat23() : AsterixRecord(23) {};
	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size);
	static char*          code(const AsterixRecord* record, size_t& write_size);
};

struct Cat23::Uap::Serializer
{
	virtual void dataSourceId(Cat23& r)  = 0;
	virtual void reportType(Cat23& r)    = 0;
	virtual void serviceType(Cat23& r)   = 0;
	virtual void timeOfDay(Cat23& r)          = 0;
	virtual void stationStatus(Cat23& r) = 0;
	virtual void serviceConfig(Cat23& r) = 0;
	virtual void range(Cat23& r)         = 0;
	virtual void status(Cat23& r)        = 0;
	virtual void stats(Cat23& r)         = 0;

	virtual void reserved(Cat23& r)     = 0;
	virtual void reserved_end(Cat23& r) = 0;
	virtual void special(Cat23& r)      = 0;

	template <size_t N> void skip(Cat23& r);

	virtual ~Serializer() {};
};

}                                                          // namespace Cat23

#endif                                                     /* ASTERIX_CAT_23_HPP */
