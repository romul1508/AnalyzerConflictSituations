#ifndef ASTERIX_FP_HPP
#define ASTERIX_FP_HPP

#include "asterix.hpp"
#include "asterix_internal.hpp"

namespace Asterix
{
enum
{
	FPT_SCHEDULED_OFF_BLOCK   = 0,                     // !< Планируемое время схода со стояночных колодок
	FPT_ESTIMATED_OFF_BLOCK   = 1,                     // !< Расчетное время схода со стояночных колодок
	FPT_ESTIMATED_TAKE_OFF    = 2,                     // !< Расчетное время взлета
	FPT_ACTUAL_OFF_BLOCK      = 3,                     // !< Актуальное (действительное) время схода со стояночных колодок
	FPT_PREDICTED_HOLD_RUNWAY = 4,                     // !< Предполагаемое время занятия ВПП
	FPT_ACTUAL_HOLD_RUNWAY    = 5,                     // !< Актуальное (действительное ) время занятия ВПП
	FPT_ACTUAL_LINE_UP        = 6,                     // !< Актуальное (действительное) время занятия исходной позиции на ВПП
	FPT_ACTUAL_TAKE_OFF       = 7,                     // !< Актуальное (действительное) время взлета
	FPT_ESTIMATED_ARRIVAL     = 8,                     // !< Расчетное время прибытия
	FPT_PREDICTED_LANDING     = 9,                     // !< Предполагаемое время посадки
	FPT_ACTUAL_LANDING        = 10,                    // !< Актуальное (действительное) время посадки
	FPT_ACTUAL_OFF_RUNWAY     = 11,                    // !< Актуальное (действительное) время схода с ВПП
	FPT_PREDICTED_TO_GATE     = 12,                    // !< Предполагаемое время подачи к высадке пассажиров
	FPT_ACTUAL_ON_BLOCK       = 13,                    // !< Актуальное (действительное) время постановки на стоянку
	FPT_LAST                  = 14                     // !<
};

struct TFlightPlan :
	AsterixRecord
{
	struct TFlightCat
	{
		GAT  gat;
		IFR  ifr;
		RVSM rvsm;
		bool hpr;
	};
	struct RW
	{
		wchar_t rw[4];
	};
	enum Day
	{
		TODAY        = 0,
		YESTERDAY    = 1,
		TOMORROW     = 2,
		NOT_AVALIBLE = 3,                          // Данное время не известно
	};
	struct Time
	{
		Day     day;
		uint8_t hour, min, sec;
	};
	struct TimeArray
	{
		Time time[FPT_LAST];
	};
	struct Stand
	{
		static const int STAND_SIZE = 7;
		wchar_t          stand[STAND_SIZE];
	};
	enum AVAL
	{
		AVALIABLE, NON_AVALIABLE, UNK
	};
	struct StandStatus
	{
		AVAL emp;
		AVAL avl;
	};
	struct SidStar
	{
		static const int SID_STAR_SIZE = 7;
		wchar_t          name[SID_STAR_SIZE];
	};

	struct Uap { };

	static const int RE_FRN   = 0;
	static const int RE_END   = 0;
	static const int LAST_FRN = 18;
	static const int UAP_SIZE = LAST_FRN + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId,     TFlightPlan,  1, _fpps_id);
	ASTERIX_DECLARE(TWideCallsign,     TFlightPlan,  2, _callsign);
	ASTERIX_DECLARE(TFlightPlanId,     TFlightPlan,  3, _flight_id);
	ASTERIX_DECLARE(TFlightCat,        TFlightPlan,  4, _flight_cat);
	ASTERIX_DECLARE(TAircraftType,     TFlightPlan,  5, _aircraft_type);
	ASTERIX_DECLARE(wchar_t,          TFlightPlan,  6, _turbulence);
	ASTERIX_DECLARE(Asterix::TAirport, TFlightPlan,  7, _dep_aip);

	ASTERIX_DECLARE(Asterix::TAirport, TFlightPlan,  8, _dst_aip);
	ASTERIX_DECLARE(RW,               TFlightPlan,  9, _runway);
	ASTERIX_DECLARE(TFlightLevel,      TFlightPlan, 10, _cleared_flight_level);
	ASTERIX_DECLARE(TControlPosition,  TFlightPlan, 11, _control);
	ASTERIX_DECLARE(TimeArray,        TFlightPlan, 12, _time);
	ASTERIX_DECLARE(Stand,            TFlightPlan, 13, _stand);
	ASTERIX_DECLARE(StandStatus,      TFlightPlan, 14, _stand_status);

	ASTERIX_DECLARE(SidStar,          TFlightPlan, 15, _sid);
	ASTERIX_DECLARE(SidStar,          TFlightPlan, 16, _star);
	ASTERIX_DECLARE(Mode3A,           TFlightPlan, 17, _pre_emergeny_m3a);
	ASTERIX_DECLARE(TWideCallsign,     TFlightPlan, 18, _pre_emergeny_callsign);


	TFlightPlan() : AsterixRecord(0),
		_fpps_id       (),
		_callsign      (),
		_flight_id     (),
		_flight_cat    (),
		_aircraft_type (),
		_turbulence    (),
		_dep_aip       (),

		_dst_aip              (),
		_runway               (),
		_cleared_flight_level (),
		_control              (),
		_time                 (),
		_stand                (),
		_stand_status         (),

		_sid                   (),
		_star                  (),
		_pre_emergeny_m3a      (),
		_pre_emergeny_callsign ()
	{}
};
}                                                          // namespace Asterix
#endif                                                     /* ASTERIX_FP_HPP */
