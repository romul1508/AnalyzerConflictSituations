#include "asterix_cat_4.h"
#include "asterix_template.h"

static const Ratio<32, 1>        LONGIT_LSB;
static const Ratio<1, 4>         TRANSV_LSB;
static const Ratio<180, 1 << 25> WGS_LSB;
static const Ratio<1, 2>         CART_LSB;
static const Ratio<1, 2>         DIST_LSB;
static const Ratio<1, 200>       PROB_LSB;

namespace Cat4
{
template <>
void Cat4::Uap::Serializer::skip<0>(Cat4&) {};

const Cat4::Uap Cat4::UAP[UAP_SIZE] =
{
	ASTERIX_UAP_FIELD(Cat4,        0,          0, NULL,                                          0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat4,        1,         10, "Data Source Identifier",                      2, dataSourceId),
	ASTERIX_UAP_FIELD(Cat4,        2,          0, "Message Type",                                1, messageType),
	ASTERIX_UAP_FIELD(Cat4,        3,         15, "SDPS Identifier",                             0, sdps),
	ASTERIX_UAP_FIELD(Cat4,        4,         20, "Time of Message",                             3, timeOfMessage),
	ASTERIX_UAP_FIELD(Cat4,        5,         40, "Alert Identifier",                            2, alertId),
	ASTERIX_UAP_FIELD(Cat4,        6,         45, "Alert Status",                                1, alertStatus),
	ASTERIX_UAP_FIELD(Cat4,        7,         60, "Safety Net Function & System Status",         0, safetyStatus),
	ASTERIX_UAP_FIELD(Cat4,        8,         30, "Track Number 1",                              2, trackNumber1),
	ASTERIX_UAP_FIELD(Cat4,        9,        170, "Aircraft Identification & Characteristics 1", 0, aircraft1),
	ASTERIX_UAP_FIELD(Cat4,       10,        120, "Conflict Characteristics",                    0, conflict),
	ASTERIX_UAP_FIELD(Cat4,       11,         70, "Conflict Timing and Separation",              0, conflictTiming),
	ASTERIX_UAP_FIELD(Cat4,       12,         76, "Vertical Deviation",                          2, vertDev),
	ASTERIX_UAP_FIELD(Cat4,       13,         74, "Longitudinal Deviation",                      2, longitDev),
	ASTERIX_UAP_FIELD(Cat4,       14,         75, "Transversal Distance Deviation",              2, transvDev),
	ASTERIX_UAP_FIELD(Cat4,       15,        100, " Area Definition",                            0, area),
	ASTERIX_UAP_FIELD(Cat4,       16,         35, "Track Number 2",                              2, trackNumber2),
	ASTERIX_UAP_FIELD(Cat4,       17,        171, "Aircraft Identification & Characteristics 2", 0, aircraft2),
	ASTERIX_UAP_FIELD(Cat4,       18,        110, "FDPS Sector Control Identifier",              0, control),
	ASTERIX_UAP_FIELD(Cat4,       19,          0, NULL,                                          0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat4,       20,          0, "Reserved Expansion Field",                    0, reserved),
	ASTERIX_UAP_FIELD(Cat4,       21,          0, "Special Purpose Field",                       0, special),
	ASTERIX_UAP_FIELD(Cat4, LAST_FRN + 0,      0, "Information Target 1",                        0, target1),
	ASTERIX_UAP_FIELD(Cat4, LAST_FRN + 1,      0, "Information Target 2",                        0, target2),
	ASTERIX_UAP_FIELD(Cat4, LAST_FRN + RE_END, 0, NULL,                                          0, reserved_end),
};

struct Cat4::RW : Cat4::Uap::Serializer, AsterixRW
{
	RW(char*& data, size_t& size, Mode m) :
		AsterixRW(data, size, m) {};

	void dataSourceId(Cat4& r)
	{
		common(r._dsi);
	}

	void messageType(Cat4& r)
	{
		_enum(r._message_type);
	}

	void sdps(Cat4& r)
	{
		uint8_t count = r._sdps.size();
		_rep(count);
		if (mode == R)
			r._sdps.resize(count);
		for (int i = 0; i != count; ++i)
			common(r._sdps[i]);
	}

	void timeOfMessage(Cat4& r)
	{
		common_time(r._time);
	}

	void alertId(Cat4& r)
	{
		_uint16(r._alert_id);
	}

	void alertStatus(Cat4& r)
	{
		uint8_t flags = 0;
		if (mode == R)
			_uint8(flags);
		ast_rw_bifield(flags, r._alert_status.stat, 1, 3);
		if (mode == W)
			_uint8(flags);
	}

	void safetyStatus(Cat4& r)
	{
		uint64_t         flags = 0;
		SafetyNetStatus& s     = r._safety_status;
		_flags_start(flags);
		ast_rw_bifield(flags, s.mrva,   7,     1);
		ast_rw_bifield(flags, s.ramld,  6,     1);
		ast_rw_bifield(flags, s.ramhd,  5,     1);
		ast_rw_bifield(flags, s.msaw,   4,     1);
		ast_rw_bifield(flags, s.apw,    3,     1);
		ast_rw_bifield(flags, s.clam,   2,     1);
		ast_rw_bifield(flags, s.stca,   2,     1);

		ast_rw_bifield(flags, s.afda,   7 + 8, 1);
		ast_rw_bifield(flags, s.rimca,  6 + 8, 1);
		ast_rw_bifield(flags, s.acasra, 5 + 8, 1);
		ast_rw_bifield(flags, s.ntca,   4 + 8, 1);
		ast_rw_bifield(flags, s.dg,     3 + 8, 1);
		ast_rw_bifield(flags, s.of,     2 + 8, 1);
		ast_rw_bifield(flags, s.ol,     1 + 8, 1);
		_flags_end(flags);
	}

	void trackNumber1(Cat4& r)
	{
		common_track_number(r._track_number_1);
	}

	void commonAircraft(AircraftData& ad)
	{
		_fspec(ad._fspec);

		if (WideCallsign* callsign = const_cast<WideCallsign*>(ad.get_callsign()))
			common(*callsign);
		if (Mode3A* ma = const_cast<Mode3A*>(ad.get_mode_3a()))
			common(*ma, false);
		if (ConfilictPosition* pos = const_cast<ConfilictPosition*>(ad.get_predicted_pos()))
		{
			_double<32, signed>(WGS_LSB, pos->wgs84_lat);
			explain_last_as(EX_WGS);
			_double<32, signed>(WGS_LSB, pos->wgs84_lon);
			explain_last_as(EX_WGS);
			_double<16, signed>(FOOT_25, pos->altitude);
		}
		if (ConflictPositionCartesian* cart_pos = const_cast<ConflictPositionCartesian*>(ad.get_predicted_cart_pos()))
		{
			_double<24, signed>(CART_LSB, cart_pos->x);
			_double<24, signed>(CART_LSB, cart_pos->y);
			_double<16, signed>(FOOT_25,  cart_pos->altitude);
		}
		if (double* time = const_cast<double*>(ad.get_time_to_thres()))
			common_time(*time);
		if (double* dist = const_cast<double*>(ad.get_dist_to_thres()))
			_double<16, unsigned>(CART_LSB, *dist);
		if (AircraftData::Characteristics* chr = const_cast<AircraftData::Characteristics*>(ad.get_char()))
		{
			uint64_t flags = 0;
			_flags_start(flags);
			ast_rw_bifield(flags, chr->gat,  6,     2);
			ast_rw_bifield(flags, chr->ifr,  4,     2);
			ast_rw_bifield(flags, chr->rvsm, 2,     2);
			ast_rw_bifield(flags, chr->hpr,  1,     1);

			ast_rw_bifield(flags, chr->cdm,  6 + 8, 2);
			ast_rw_bifield(flags, chr->pri,  5 + 8, 1);
			ast_rw_bifield(flags, chr->gv,   4 + 8, 1);
			_flags_end(flags);
		}
		if (Callsign* cs = const_cast<Callsign*>(ad.get_mode_s_callsign()))
			common(*cs, false);
		if (FlightPlanId* fpid = const_cast<FlightPlanId*>(ad.get_flight_id()))
			common(*fpid, false);
		if (FlightLevel* cfl = const_cast<FlightLevel*>(ad.get_cleared_flight_level()))
			common(*cfl, false);
	}

	void aircraft1(Cat4& r)
	{
		commonAircraft(r._aircraft_1);
	}

	void conflict(Cat4& r)
	{
		Conflict& c = r._conflict;
		_fspec(c._fspec);

		if (Conflict::Nature* nature = const_cast<Conflict::Nature*>(c.get_nature()))
		{
			uint64_t flags = 0;
			_flags_start(flags);
			ast_rw_bifield(flags, nature->rrc,  7, 1);
			ast_rw_bifield(flags, nature->rtc,  6, 1);
			ast_rw_bifield(flags, nature->mrva, 5, 1);
			_flags_end(flags);
		}
		if (Conflict::Classification* classif = const_cast<Conflict::Classification*>(c.get_classification()))
		{
			uint8_t flags = 0;
			if (mode == R)
			{
				_flag_byte(flags);
				classif->table_id = flags >> 4;
				classif->cs       = flags & 1;
				switch (classif->table_id)
				{
				case Conflict::TBL_STCA_0:
					classif->severety_class = flags >> 1 & 0x7;
					break;
				case Conflict::TBL_STCA_1:
					classif->lpf = flags >> 3 & 1;
					classif->cpf = flags >> 2 & 1;
					classif->mhf = flags >> 1 & 1;
					break;
				case Conflict::TBL_RIMCAS:
					classif->ras = flags >> 3 & 1;
					break;
				}
			}
			else
			{
				flags |= (classif->table_id & 0xF) << 4;
				flags |= (classif->cs & 1);
				switch (classif->table_id)
				{
				case Conflict::TBL_STCA_0:
					flags |= (classif->severety_class & 0x7) << 1;
					break;
				case Conflict::TBL_STCA_1:
					flags |= (classif->lpf & 1) << 3;
					flags |= (classif->cpf & 1) << 2;
					flags |= (classif->mhf & 1) << 1;
					break;
				case Conflict::TBL_RIMCAS:
					flags |= (classif->ras & 1) << 3;
					break;
				}
				_flag_byte(flags);
			}
		}
		if (double* prob = const_cast<double*>(c.get_probability()))
			_double<8, unsigned>(PROB_LSB, *prob);
		if (double* duration = const_cast<double*>(c.get_duration()))
			common_time(*duration);
	}

	void conflictTiming(Cat4& r)
	{
		ConflictTiming& ct = r._conflict_timing;
		_fspec(ct._fspec);
		if (double* time = const_cast<double*>(ct.get_time_to_conflict()))
			common_time(*time);
		if (double* time = const_cast<double*>(ct.get_time_to_closest_approach()))
			common_time(*time);
		if (double* dist = const_cast<double*>(ct.get_current_horiz_sep()))
			_double<24, unsigned>(DIST_LSB, *dist);
		if (double* dist = const_cast<double*>(ct.get_estimated_min_horiz_sep()))
			_double<16, unsigned>(DIST_LSB, *dist);
		if (double* vdist = const_cast<double*>(ct.get_current_vert_sep()))
			_double<16, unsigned>(FOOT_25, *vdist);
		if (double* vdist = const_cast<double*>(ct.get_estimated_min_vert_sep()))
			_double<16, unsigned>(FOOT_25, *vdist);
	}

	void vertDev(Cat4& r)
	{
		_double<16, signed>(FOOT_25, r._vertical_deviation);
	}

	void longitDev(Cat4& r)
	{
		_double<16, unsigned>(LONGIT_LSB, r._longit_deviation);
	}

	void transvDev(Cat4& r)
	{
		_double<16, unsigned>(TRANSV_LSB, r._transversal_deviation);
	}

	void area(Cat4& r)
	{
		AreaDefinition& a = r._area;
		_fspec(a._fspec);
		if (Callsign* name = const_cast<Callsign*>(a.get_area_name()))
			common(*name, false);
		if (WideCallsign* name = const_cast<WideCallsign*>(a.get_crossing_area_name()))
			common(*name);
		if (WideCallsign* tw1 = const_cast<WideCallsign*>(a.get_rw_taxi_1()))
			common(*tw1);
		if (WideCallsign* tw2 = const_cast<WideCallsign*>(a.get_rw_taxi_2()))
			common(*tw2);
		if (WideCallsign* stop = const_cast<WideCallsign*>(a.get_stopbar()))
			common(*stop);
		if (WideCallsign* gate = const_cast<WideCallsign*>(a.get_gate()))
			common(*gate);
	}

	void trackNumber2(Cat4& r)
	{
		common_track_number(r._track_number_2);
	}

	void aircraft2(Cat4& r)
	{
		commonAircraft(r._aircraft_2);
	}

	void control(Cat4& r)
	{
		uint8_t count = r._control.size();
		_rep(count);
		if (mode == R)
			r._control.resize(count);
		for (int i = 0; i != count; ++i)
			common(r._control[i]);
	}

	void reserved(Cat4& r)
	{
		AsterixRW::reserved(r);
	}

	void reserved_end(Cat4& r)
	{
		AsterixRW::reserved_end(r);
	}

	void special(Cat4& r)
	{
		AsterixRW::special(r._sp);
	}

	void commonTargetInfo(TargetInfo& ti)
	{
		_fspec(ti._fspec);
		if (Position* pos = const_cast<Position*>(ti.get_position()))
			common_wgs_32bit(WGS_LSB, *pos);
		if (PositionCartesian* cart_pos = const_cast<PositionCartesian*>(ti.get_cart_pos()))
			common_cart_24bit(CART_LSB, *cart_pos);
		if (FlightLevel* fl = const_cast<FlightLevel*>(ti.get_mode_c()))
			common(*fl, true);
		if (Velocity* vel = const_cast<Velocity*>(ti.get_velocity()))
			common(*vel);
	}

	void target1(Cat4& r)
	{
		commonTargetInfo(r._target_1);
	}

	void target2(Cat4& r)
	{
		commonTargetInfo(r._target_2);
	}

};
AsterixRecord* Cat4::decode(const char* data, size_t size, AsterixError& err, size_t& read_size, ast_explain* expl)
{
	return ast_cat_decode<Cat4, 4>(data, size, err, read_size, expl);
}

char* Cat4::code(const AsterixRecord* record, size_t& write_size)
{
	return ast_cat_code<Cat4, 4>(record, write_size);
}

}                                                          // namespace Cat19
