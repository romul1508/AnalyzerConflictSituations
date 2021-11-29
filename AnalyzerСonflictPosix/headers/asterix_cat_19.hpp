#ifndef ASTERIX_CAT_19_HPP
#define ASTERIX_CAT_19_HPP

#include "asterix.hpp"
#include "asterix_internal.hpp"

namespace Cat19
{
using Asterix::DataSourceId;
using Asterix::Position;

struct SystemStatus
{
	enum ReleaseStatus
	{
		RS_OPERATIONAL = 0,
		RS_DEGRADED    = 1,
		RS_NOGO        = 2,
		RS_UNDEFINED   = 3,
	};

	enum ReleaseStatus rel_status     : 2;
	bool               overload       : 1;
	bool               invalid_time   : 1;
	bool               target_failure : 1;
};

struct SensorStatus
{
	uint8_t rsid;
	bool    receiver_1090    : 1;
	bool    transmitter_1030 : 1;
	bool    transmitter_1090 : 1;
	bool    rs_good          : 1;
	bool    rs_online        : 1;
};

struct TrackingStatus
{
	bool tp1_exec : 1;
	bool tp1_good : 1;

	bool tp2_exec : 1;
	bool tp2_good : 1;

	bool tp3_exec : 1;
	bool tp3_good : 1;

	bool tp4_exec : 1;
	bool tp4_good : 1;
};

enum RefTransponderStatus
{
	RTS_GOOD    = 3,
	RTS_FAULTED = 2,
	RTS_WARNING = 1,
	RTS_NA      = 0
};

enum MsgType
{
	MT_START_UPDATE_CYCLE  = 1,
	MT_PERIODIC_STATUS_MSG = 2,
	MT_EVENT_STATUS_MSG    = 3
};

/// Категория 19
struct Cat19 :
	public AsterixRecord
{
	struct Reader;
	struct Writer;

	/// Структура-описание полей asterix
	struct Uap
	{
		Asterix::UapInfo info;
		struct Serializer;
		void (Serializer::* serialize)(Cat19&);                                 ///< Функция-обработчик
	};

	static const int RE_FRN   = 13;
	static const int RE_END   = 0;
	static const int LAST_FRN = 15;
	static const int UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId,   Cat19, 1, _dsi);                                ///< Data Source Identifier
	ASTERIX_DECLARE(uint8_t,        Cat19, 2, _message_type);                       ///< Message Type
	ASTERIX_DECLARE(double,         Cat19, 3, _time);                               ///< Time of Day
	ASTERIX_DECLARE(SystemStatus,   Cat19, 4, _system_status);                      ///< System Status
	ASTERIX_DECLARE(TrackingStatus, Cat19, 5, _tracking_status);                    ///< Tracking Processor Detailed Status

	ASTERIX_DECLARE_ARRAY(SensorStatus,         Cat19, 6, _sensor_status);          ///< Remote Sensor Detailed Status
	ASTERIX_DECLARE_ARRAY(RefTransponderStatus, Cat19, 7, _ref_transponder_status); ///< Reference Transponder Detailed Status

	ASTERIX_DECLARE(Position, Cat19,  8, _ref_point);                               ///< Position of the MLT System Reference point
	ASTERIX_DECLARE(double,   Cat19,  9, _ref_point_altitude);                      ///< Height of the MLT System Reference point
	ASTERIX_DECLARE(double,   Cat19, 10, _wgs84_undulation);                        ///< WGS-84 Undulation

	ASTERIX_DECLARE(uint8_t,  Cat19, 13, _re);                                      ///< Reserved Expansion Field
	ASTERIX_DECLARE_ARRAY(uint8_t, Cat19, 14, _sp);                                 ///< Special Purpose Field
	ASTERIX_DECLARE(uint8_t, Cat19, LAST_FRN + RE_END, _re_end);

public:

	Cat19() : AsterixRecord(19) {};
	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size);
	static char*          code(const AsterixRecord* record, size_t& write_size);
};

struct Cat19::Uap::Serializer
{
	virtual void dataSourceId(Cat19& r)      = 0;
	virtual void messageType(Cat19& r)       = 0;
	virtual void timeOfDay(Cat19& r)         = 0;
	virtual void sensorStatus(Cat19& r)      = 0;
	virtual void transponderStatus(Cat19& r) = 0;
	virtual void refPoint(Cat19& r)          = 0;
	virtual void refPointHeight(Cat19& r)    = 0;
	virtual void wgsUndulation(Cat19& r)     = 0;
	virtual void trackingStatus(Cat19& r)    = 0;
	virtual void systemStatus(Cat19& r)      = 0;

	virtual void reserved(Cat19& r)     = 0;
	virtual void reserved_end(Cat19& r) = 0;
	virtual void special(Cat19& r)      = 0;

	template <size_t N> void skip(Cat19& r);

	virtual ~Serializer() {};
};

}                                                          // namespace Cat19

#endif                                                     /* ASTERIX_CAT_19_HPP */
