/*
 *
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include "unistd.h"
#include <string.h>
#include <time.h>

#include "cat_asterix.h"
#include "cat_062_asterix.h"

/**
 * @brief Установка значений по умолчанию для параметров структуры
 *        категории 062 Астерикс
 * @param cat - указатель на структуру категории
 * @return 0 - успешное выполнение, -1 отказ исполнения
 */
int set062catDefaultData(TCat062* cat)
{
	if (cat == NULL)
		return -1;

	cat->item062_010.sac_sic               = DEFAULT_U_INT16_CODE;
	cat->item062_015.sevice_id             = DEFAULT_U_INT8_CODE;
	cat->item062_040.track_number          = DEFAULT_U_INT16_CODE;
	cat->item062_060.m3a.board_number_code = DEFAULT_RBS_UVD_NUM_CODE;
	cat->item062_060.m3a.change            = DEFAULT_U_INT8_CODE;
	cat->item062_070.time_of_track         = DEFAULT_TIME_OF_TRACK;
	cat->item062_100.x                     = DEFAULT_CARTESIAN_POS;
	cat->item062_100.y                     = DEFAULT_CARTESIAN_POS;
	cat->item062_105.latitude              = DEFAULT_RADIAN_GEO_POS;
	cat->item062_105.longitude             = DEFAULT_RADIAN_GEO_POS;
	cat->item062_120.m2.board_number_code  = DEFAULT_RBS_UVD_NUM_CODE;
	cat->item062_120.m2.change             = DEFAULT_U_INT8_CODE;
	cat->item062_130.calc_g_height         = DEFAULT_ALTITUDE;
	cat->item062_135.calc_b_height         = DEFAULT_ALTITUDE;
	cat->item062_136.measured_fligth_level = DEFAULT_ALTITUDE;
	cat->item062_185.vx                    = DEFAULT_VELOCITY;
	cat->item062_185.vy                    = DEFAULT_VELOCITY;
	cat->item062_210.ax                    = DEFAULT_ACCELERATION;
	cat->item062_210.ay                    = DEFAULT_ACCELERATION;
	cat->item062_220.vh                    = DEFAULT_VELOCITY;
	cat->item062_270.length                = DEFAULT_U_INT8_CODE;
	cat->item062_270.width                 = DEFAULT_U_INT8_CODE;
	cat->item062_270.orientation           = DEFAULT_RADIAN_AZIMUTH_POS;

	return 0;
}

/**
 * @brief Распаковка данных канала АЗН-В (I062/380) передающихся в составе категории 062
 * @param code - указатель массив кодограммы
 * @param cnt - номер начального байта данных АЗН-В
 * @return - указатель на структуру с распакованными данными
 */
AircraftDerivedData* decode062ed380(unsigned char* code, u_int64_t* cnt, u_int64_t codo_size)
{
	u_int64_t k                           = *cnt, fspec_size;
	u_int8_t  ads_spec[FSPEC_SIZE_4_BYTE] =
	{
		0
	};
	AircraftDerivedData* ads_b = NULL;

	if ((code == NULL) || (*cnt >= codo_size))
		return ads_b;

	if (checkOverloadFunc(k, 1, codo_size) < 0)
	{
		return NULL;
	}

	fspec_size = getCategoryFspecSize(code, k, codo_size);
	if ((fspec_size > 0) && (fspec_size < FSPEC_SIZE_5_BYTE))
	{
		memcpy(ads_spec, code + k, fspec_size);
		k += fspec_size;
	}
	else
	{
		fprintf(stderr, "proc[pid-%d] decode062ed380 elment Primary Subfield size ERROR\n", getpid());
		return ads_b;
	}

	ads_b = malloc(sizeof(AircraftDerivedData));

	if (ads_b == NULL)
	{
		fprintf(stderr, "proc[pid-%d] decode062ed380 elment malloc ERROR\n", getpid());
		return ads_b;
	}

	bzero(ads_b, sizeof(AircraftDerivedData));

	// Adress IKAO VO
	if (ads_spec[ 0 ] & 0x80)
	{
		if (checkOverloadFunc(k, 3, codo_size) < 0)
		{
			free(ads_b);
			return NULL;
		}
		ads_b->target_address = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
		k                    += 3;
	}
	// Identifik VO
	if (ads_spec[ 0 ] & 0x40)
	{
		u_int64_t cnt_pos = k;
		if (checkOverloadFunc(k, 6, codo_size) < 0)
		{
			free(ads_b);
			return NULL;
		}
		convertIA5codeToAscii((unsigned char*) ads_b->target_identification, code, &cnt_pos);
		k = cnt_pos;
	}
	// Course VO
	if (ads_spec[ 0 ] & 0x20)
	{
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			free(ads_b);
			return NULL;
		}
		ads_b->magnetic_heading = ((code[k] << 8) | code[k + 1]) * CMR_ASTX_AZ;
		k                      += 2;
	}
	// Speed barometric/Mach
	if (ads_spec[ 0 ] & 0x10)
	{
		float ff;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			free(ads_b);
			return NULL;
		}
		if (code[k] & 0x80)                                             // Mach speed
		{
			ff                    = 0.001;                          // cmr mach
			ads_b->mach_indicator = 1;
		}
		else
		{
			ff = CMR_ASTX_SPEED;                                    // cmr m/sec
		}

		ads_b->indicated_airspeed = ff * (((code[k] & 0x7f) << 8) | code[k + 1]);
		k                        += 2;
	}
	// Speed real measured
	if (ads_spec[ 0 ] & 0x08)
	{
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			free(ads_b);
			return NULL;
		}
		ads_b->true_airspeed = 0.5144 * ((code[k] << 8) | code[k + 1]); // m/sec
		k                   += 2;
	}
	// Selected Alt VO SAL
	if (ads_spec[ 0 ] & 0x04)
	{
		u_int16_t bb;
		int       zn = 1;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			free(ads_b);
			return NULL;
		}

		ads_b->selected_altitude_valid = !!(code[k] & 0x80);

		ads_b->selected_altitude_src = ASA_UNKNOWN;

		switch ((code[k] >> 5) & 0x03)
		{
		case 1:
			ads_b->selected_altitude_src = ASA_FROM_AIRCRAFT;
			break;
		case 2:
			ads_b->selected_altitude_src = ASA_FROM_FCU_MCP;
			break;
		case 3:
			ads_b->selected_altitude_src = ASA_FROM_FMS;
			break;
		}

		bb = ((code[k] & 0x1f) << 8) | code[k + 1];

		if (code[k] & 0x10)
		{
			bb |= 0xE000;
			bb  = ~(bb - 1);
			zn  = -1;
		}

		ads_b->selected_altitude = zn * bb * CMR_ASTX_H_25F;
		k                       += 2;
	}
	// Final Alt VO FSS
	if (ads_spec[ 0 ] & 0x02)
	{
		u_int16_t bb;
		int       zn = 1;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			free(ads_b);
			return NULL;
		}

		ads_b->fsa_manage_vertical_mode = !!(code[k] & 0x80);
		ads_b->fsa_altitude_hold        = !!(code[ k ] & 0x40);
		ads_b->fsa_approach_mode        = !!(code[ k ] & 0x01);

		bb = ((code[k] & 0x1f) << 8) | code[k + 1];
		if (code[k] & 0x10)
		{
			bb |= 0xE000;
			bb  = ~(bb - 1);
			zn  = -1;
		}

		ads_b->final_selected_altitude = zn * bb * CMR_ASTX_H_25F;

		k += 2;
	}
	// Status trajectory VO
	if (ads_spec[ 1 ] & 0x80)
	{
		if (checkOverloadFunc(k, 1, codo_size) < 0)
		{
			free(ads_b);
			return NULL;
		}
		ads_b->trajectory_available = !!(code[k] & 0x80);
		ads_b->trajectory_valid     = !!(code[k] & 0x40);
		k                          += 1;
	}
	// Data trajectory VO
	if (ads_spec[ 1 ] & 0x40)
	{
		u_int8_t  b;
		u_int16_t bb;
		int       i, zn = 1, ccc;
		// REP factor
		if (checkOverloadFunc(k, 1, codo_size) < 0)
		{
			free(ads_b);
			return NULL;
		}
		ads_b->route_point_count = code[k];
		k                       += 1;

		ads_b->route_point = malloc(sizeof(TAdsRoutePoint) * ads_b->route_point_count);

		if (ads_b->route_point)
		{
			for (i = 0; i < ads_b->route_point_count; i++)
			{
				if (checkOverloadFunc(k, 15, codo_size) < 0)
				{
					free(ads_b->route_point);
					free(ads_b);
					return NULL;
				}

				if (!(code[k] & 0x40))
					ads_b->route_point[i].compliance = 1;
				k                              += 1;
				ads_b->route_point[i].point_num = b & 0x3f;
				bb                              = (code[k] << 8) | code[k + 1];
				if (code[k] & 0x80)
				{
					bb = ~(bb - 1);
					zn = -1;
				}
				else
				{
					zn = 1;
				}
				ads_b->route_point[i].point_altitude  = zn * bb * CMR_ASTX_H_10F;
				k                                    += 2;
				ccc                                   = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
				k                                    += 3;
				ads_b->route_point[i].point_latitude  = ccc * CMR_GEO_23;
				ccc                                   = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
				k                                    += 3;
				ads_b->route_point[i].point_longitude = ccc * CMR_GEO_23;
				b                                     = code[k];
				k                                    += 1;
				ads_b->route_point[i].point_type      = b >> 4;
				ads_b->route_point[i].point_turn_sign = TMM_MOVE_STABLE;
				switch ((b & 0x0C) >> 2)
				{
				case 1:
					ads_b->route_point[i].point_turn_sign = TMM_TURN_RIGHT;
					break;
				case 2:
					ads_b->route_point[i].point_turn_sign = TMM_TURN_LEFT;
					break;
				}
				// sec from 00:00 current day
				ads_b->route_point[i].point_time        = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
				k                                      += 3;
				ccc                                     = (code[k] << 8) | code[k + 1];
				k                                      += 2;
				ads_b->route_point[i].point_turn_radius = ccc * CMR_ADS_TTR; // turn radius in metr
			}
		}
		else
		{
			for (i = 0; i < ads_b->route_point_count; i++)
				k += 15;
		}
	}
	// TCAS Data VO
	if (ads_spec[ 1 ] & 0x20)
	{
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}

		ads_b->communication_capability   = code[k] >> 5;
		ads_b->flight_status              = (code[k] & 0x1C) >> 2;
		k                                += 1;
		ads_b->specific_service           = !!(code[k] & 0x80);
		ads_b->altitude_report_capability = !!(code[k] & 0x40);
		ads_b->aircraft_ident_capability  = !!(code[k] & 0x20);
		ads_b->bit_b1a                    = !!(code[k] & 0x10);
		ads_b->bits_b1b                   = code[ k ] & 0x0f;
		k                                += 1;
	}
	// Status ADS VO
	if (ads_spec[ 1 ] & 0x10)
	{
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}

		switch (code[k] >> 6)
		{
		case 1: ads_b->acas_operational = 0;
			break;
		case 2: ads_b->acas_operational = 1;
			break;
		}

		switch ((code[k] & 0x30) >> 4)
		{
		case 1: ads_b->multiple_navigational = 0;
			break;
		case 2: ads_b->multiple_navigational = 1;
			break;
		}

		switch ((code[k] & 0x0C) >> 2)
		{
		case 1: ads_b->differencial_correction = 1;
			break;
		case 2: ads_b->differencial_correction = 0;
			break;
		}

		ads_b->ground_bit = !!(code[k] & 0x02);
		k                += 1;

		ads_b->flight_status_emergency = code[k] & 0x07;
		k                             += 1;
	}
	// Status ACAS VO
	if (ads_spec[ 1 ] & 0x08)
	{
		int i;
		if (checkOverloadFunc(k, 7, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		// no processing
		ads_b->acas_56_bit_msg = 0;
		for (i = 0; i < 7; i++)
		{
			ads_b->acas_56_bit_msg |= code[ k ] << (8 * (6 - i));
			k                      += 1;
		}
	}
	// Bar vertic rate VO
	if (ads_spec[ 1 ] & 0x04)
	{
		u_int16_t bb;
		int       zn = 1;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		bb = (code[k] << 8) | code[k + 1];
		if (code[k] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		ads_b->vh_barometric = (float) (CMR_ASTX_H_6_25F * zn * bb);
		k                   += 2;
	}
	// Geometric vertic rate VO
	if (ads_spec[ 1] & 0x02)
	{
		u_int16_t bb;
		int       zn = 1;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		bb = (code[k] << 8) | code[k + 1];
		if (code[k] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		ads_b->vh_geometric = (float) (CMR_ASTX_H_6_25F * zn * bb);
		k                  += 2;
	}
	// Turn angle VO
	if (ads_spec[ 2 ] & 0x80)
	{
		u_int16_t bb;
		int       zn = 1;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		bb = (code[k] << 8) | code[k + 1];
		if (code[k] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		ads_b->roll_angle = (float) (0.01 * zn * bb); // grad
		k                += 2;
	}
	// Turn rate VO
	if (ads_spec[ 2 ] & 0x40)
	{
		u_int16_t b;
		int       zn = 1;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}

		ads_b->turn_indicator = TMM_MOVE_STABLE;
		switch (code[k])
		{
		case 1: ads_b->turn_indicator = TMM_TURN_LEFT;
			break;
		case 2: ads_b->turn_indicator = TMM_TURN_RIGHT;
			break;
		}
		k += 1;
		b  = code[k] >> 1;
		if (b & 0x40)
		{
			b |= 0x80;
			b  = ~(b - 1);
			zn = -1;
		}
		ads_b->turn_rate = (float) (0.25 * zn * b);      // grad/sec
		k               += 1;
	}
	// track angle
	if (ads_spec[ 2 ] & 0x20)
	{
		u_int16_t bb;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		bb                 = (code[k] << 8) | code[k + 1];
		ads_b->track_angle = (float) (CMR_ASTX_AZ * bb); // grad
		k                 += 2;
	}
	// Ground speed VO
	if (ads_spec[ 2 ] & 0x10)
	{
		u_int16_t bb;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		bb                  = (code[k] << 8) | code[k + 1];
		k                  += 2;
		ads_b->ground_speed = (float) (CMR_ASTX_SPEED * bb); // m/sec
		k                  += 2;
	}
	// SCO speed VO
	if (ads_spec[ 2 ] & 0x08)
	{
		if (checkOverloadFunc(k, 1, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		ads_b->velocity_uncertainty = code[k];
		k                          += 1;
	}
	// Meteodata VO
	if (ads_spec[ 2 ] & 0x04)
	{
		u_int16_t bb;
		int       zn = 1;
		if (checkOverloadFunc(k, 8, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}

		ads_b->wind_speed_valid  = !!(code[k] & 0x80);
		ads_b->wind_direct_valid = !!(code[k] & 0x40);
		ads_b->temperature_valid = !!(code[k] & 0x20);
		ads_b->turbulence_valid  = !!(code[k] & 0x10);
		k                       += 1;
		bb                       = (code[k] << 8) | code[k + 1];
		ads_b->wind_speed        = (float) (0.5144 * bb); // m/sec
		k                       += 2;
		bb                       = (code[k] << 8) | code[k + 1];
		ads_b->wind_direct       = (float) (bb);          // grad
		k                       += 2;
		bb                       = (code[k] << 8) | code[k + 1];
		if (code[k] & 0x8000)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		ads_b->temperature = (short) (zn * bb * 0.25);    // grad
		k                 += 2;
		ads_b->turbulence  = code[k];
		k                 += 1;
	}
	// Transmitter cat VO
	if (ads_spec[ 2 ] & 0x02)
	{
		if (checkOverloadFunc(k, 1, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		ads_b->emitter_category = code[k];
		k                      += 1;
	}
	// Position data VO
	if (ads_spec[ 3 ] & 0x80)
	{
		int ccc;
		if (checkOverloadFunc(k, 6, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		ccc                    = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
		k                     += 3;
		ads_b->latitude_wgs84  = (double) (ccc * CMR_GEO_23);
		ccc                    = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
		k                     += 3;
		ads_b->longitude_wgs84 = (double) (ccc * CMR_GEO_23);
	}
	// Geo Alt VO
	if (ads_spec[ 3 ] & 0x40)
	{
		u_int16_t bb;
		int       zn = 1;

		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}

		bb = (code[k] << 8) | code[k + 1];
		if (code[k] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		ads_b->geometric_altitude = (float) (zn * bb * CMR_ASTX_H_6_25F); // meter
		k                        += 2;
	}
	// SCO Position data VO
	if (ads_spec[ 3 ] & 0x20)
	{
		if (checkOverloadFunc(k, 1, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}
		ads_b->position_uncertainty = code[k] & 0x0f;
		k                          += 1;
	}
	// Mode S Data
	if (ads_spec[ 3 ] & 0x10)
	{
		int i, j;
		// rep factor
		if (checkOverloadFunc(k, 1, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			free(ads_b);
			return NULL;
		}

		ads_b->mode_s_data_count = code[k];
		k                       += 1;

		ads_b->mode_s_msg = malloc(ads_b->mode_s_data_count * sizeof(u_int64_t));

		if (ads_b->mode_s_msg)
			bzero(ads_b->mode_s_msg, sizeof(u_int64_t));

		for (i = 0; i < ads_b->mode_s_data_count; i++)
		{
			if (checkOverloadFunc(k, 8, codo_size) < 0)
			{
				if (ads_b->route_point)
					free(ads_b->route_point);
				if (ads_b->mode_s_msg)
					free(ads_b->mode_s_msg);
				free(ads_b);
				return NULL;
			}
			if (ads_b->mode_s_msg)
			{
				for (j = 0; j < 8; j++)
				{
					ads_b->mode_s_msg[i] |= (code[k] << (8 * (7 - j)));
					k                    += 1;
				}
			}
			else
				k += 8;
		}
	}
	// Barom speed data VO
	if (ads_spec[ 3 ] & 0x08)
	{
		u_int16_t bb;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			if (ads_b->mode_s_msg)
				free(ads_b->mode_s_msg);
			free(ads_b);
			return NULL;
		}
		bb                        = (code[k] << 8) | code[k + 1];
		ads_b->indicated_airspeed = (float) (bb * 1.609); // km/hour
		k                        += 2;
	}
	// Mach speed data VO
	if (ads_spec[ 3 ] & 0x04)
	{
		u_int16_t bb;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			if (ads_b->mode_s_msg)
				free(ads_b->mode_s_msg);
			free(ads_b);
			return NULL;
		}
		bb                 = (code[k] << 8) | code[k + 1];
		ads_b->mach_number = (float) (bb * 0.008); // Mach
		k                 += 2;
	}
	// Barometr pressure VO
	if (ads_spec[ 3 ] & 0x02)
	{
		u_int16_t bb;
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			if (ads_b->route_point)
				free(ads_b->route_point);
			if (ads_b->mode_s_msg)
				free(ads_b->mode_s_msg);
			free(ads_b);
			return NULL;
		}
		bb                                 = ((code[k] & 0x0f) << 8) | code[k + 1];
		ads_b->barometric_pressure_setting = (float) (bb * 0.1); // mB
		k                                 += 2;
	}

	*cnt = k;

	return ads_b;
}

/**
 * @brief Распаковка поля SP категории 062 (по умолчанию)
 * @param code - указатель на массив с кодограммой
 * @param cnt - номер начального байта кодированных данных в кодограмме
 * @return - указатель на структуру с распакованными данными
 */
TSpData* decode062defaultSPdata(unsigned char* code, u_int64_t* cnt, u_int64_t codo_size)
{
	u_int64_t      k  = *cnt, fspec_pos;
	unsigned char  b  = 0;
	unsigned short bb = 0;
	TSpData*       sp = NULL;

	if ((code == NULL) || (*cnt < 0))
		return sp;

	sp = malloc(sizeof(TSpData));

	if (sp)
		bzero(sp, sizeof(TSpData));
	else
		return sp;

	if (checkOverloadFunc(k, 1, codo_size) < 0)
	{
		free(sp);
		return NULL;
	}

	k += 1;                                            // size

	if (checkOverloadFunc(k, 1, codo_size) < 0)
	{
		free(sp);
		return NULL;
	}
	fspec_pos = k;
	k        += 1;                                     // fspec1

//    if(fspec[1]&0x01)
//    {
//        fspec[0] = code[k];	k+=1;//fspec2
//    }
	if (code[ fspec_pos ] & 0x01)
	{
		fprintf(stdout, "proc[pid-%d] decode062defaultSPdata: Illegal fspec SP!!! %08X\n", getpid(), code[ fspec_pos ]);
		free(sp);
		return NULL;
	}

	if (code[ fspec_pos] & 0x80)                       // OGP NRZ SP01
	{
		if (checkOverloadFunc(k, 1, codo_size) < 0)
		{
			free(sp);
			return NULL;
		}
		b  = code[k];
		k += 1;

		if (b & 0x01)
			sp->ogp = IFFR_OWN;                //
		if (b & 0x02)
			sp->ogp = IFFR_OWN_GUARANTEED;     //
		if (b & 0x04)                              // Regime 3 ?????????????
			sp->ogp = IFFR_OWN_INDIVIDUAL;
		if (b & 0x08)
			sp->ogp = IFFR_ALIEN;
		if (b & 0x10)                              // Nalichie 6110
		{
			;
		}
		if (b & 0x20)
			sp->sos = 1;

		if (b & 0x80)
			sp->ogp_update = 1;
		if (sp->ogp == 0)
			sp->ogp = IFFR_NOT_TESTED;
		sp->interrogate_system = IST_NRZ;
	}
	if (code[ fspec_pos] & 0x40)                       // bort number NRZ SP02 [3 byte]
	{
		if (checkOverloadFunc(k, 3, codo_size) < 0)
		{
			free(sp);
			return NULL;
		}
		b = code[k];
		if (!(b & 0x80))
			sp->board_num_valid = 1;
		if (b & 0x40)
			sp->board_num_garbled = 1;
		if (!(b & 0x20))
			sp->board_num_update = 1;
		sp->board_num          = ((code[k] & 0x0f) << 16) | (code[k + 1] << 8) | code[k + 2];
		k                     += 3;
		sp->interrogate_system = IST_NRZ;
	}
	if (code[ fspec_pos] & 0x20)                       // H_bar + Toplivo SP03
	{
		double zn = 1.0;
		if (checkOverloadFunc(k, 3, codo_size) < 0)
		{
			free(sp);
			return NULL;
		}
		b        = code[k];
		k       += 1;
		sp->fuel = decodeUvdFuel(b & 0x0f);

		if (!(b & 0x20))
			sp->barometric_altitude_absolute = 1;
		if (b & 0x10)
			sp->sos = 1;

		bb = ((code[k] & 0x3F) << 8) | code[k + 1];
		if (code[k] & 0x20)
		{
			bb |= 0xC000;
			bb  = ~(bb - 1);
			zn  = -1.0;
		}
		else
			zn = 1.0;
		sp->altitude_barometric = bb * CMR_UVD_HEIGTH * zn;
		k                      += 2;
		sp->interrogate_system  = IST_NRZ;
	}

	if (code[ fspec_pos] & 0x08)                       // reserv in flag
	{
		if (checkOverloadFunc(k, 1, codo_size) < 0)
		{
			free(sp);
			return NULL;
		}
		b  = code[k];
		k += 1;
		if (b & 0x02)
			sp->interrogate_system = IST_UVD;  // UVD prezent
	}

	if (code[ fspec_pos ] & 0x04)                      // bort num from UVD SP04 UVD_Num[3 byte]
	{
		if (checkOverloadFunc(k, 3, codo_size) < 0)
		{
			free(sp);
			return NULL;
		}
		b = code[k];
		if (!(b & 0x80))
			sp->board_num_valid = 1;
		if (b & 0x40)
			sp->board_num_garbled = 1;
		if (!(b & 0x20))
			sp->board_num_update = 1;
		sp->board_num          = ((code[k] & 0x0f) << 16) | (code[k + 1] << 8) | code[k + 2];
		k                     += 3;
		sp->interrogate_system = IST_UVD;
	}
	if (code[ fspec_pos ] & 0x02)                      // Hbar + Toplivo from UVD SP05
	{
		double zn = 1.0;
		if (checkOverloadFunc(k, 3, codo_size) < 0)
		{
			free(sp);
			return NULL;
		}
		b        = code[k];
		k       += 1;
		sp->fuel = decodeUvdFuel(b & 0x0f);

		if (!(b & 0x20))
			sp->barometric_altitude_absolute = 1;
		if (b & 0x10)
			sp->sos = 1;

		bb = ((code[ k ] & 0x3F) << 8) | code[ k + 1 ];
		if (code[ k ] & 0x20)
		{
			bb |= 0xC000;
			bb  = ~(bb - 1);
			zn  = -1.0;
		}
		else
			zn = 1.0;
		sp->altitude_barometric = bb * CMR_UVD_HEIGTH * zn;
		k                      += 2;
		sp->interrogate_system  = IST_UVD;
	}

	if (sp->altitude_barometric == 0.)
		sp->barometric_altitude_absolute = 1;

	*cnt = k;

	return sp;
}

/**
 * @brief Упаковка поля SP в категории 062 (по умолчанию)
 * @param sp_elem - указатель на поле данных SP
 * @return - кодированная последовательность в байтах для укладки в кодограмму
 */

TCommonObmenData* pack062defaultSPdata(TSpData* sp)
{
	int           ccc      = 0, cnt = 0;
	unsigned char fspec[2] =
	{
		0
	}, b                      = 0, code[20];
	TCommonObmenData* sp_data = NULL;


	if (sp == NULL)
		return sp_data;

	cnt       = 0;
	code[cnt] = 0;
	cnt      += 1;                                     // size
	code[cnt] = 0;
	cnt      += 1;                                     // fspec1
	// по умолчанию всегда выдается признак ОГП
	{
		b = 0;
		switch (sp->ogp)
		{
		case IFFR_OWN: b |= 0x01;
			break;
		case IFFR_OWN_GUARANTEED:  b |= 0x02;
			break;
		case IFFR_OWN_INDIVIDUAL:  b |= 0x04;
			break;
		case IFFR_ALIEN: b |= 0x08;
			break;
		}
		// if( b & 0x10 )// Nalichie 6110
		// {
		// }
		if (sp->sos)
			b |= 0x20;
		if (sp->ogp_update)
			b |= 0x80;
		code[ cnt ] = b;
		cnt        += 1;
		fspec[1]   |= 0x80;
	}
	// Номер борта  по данным НРЗ SP02 [3 byte]
	if ((sp->interrogate_system == IST_NRZ) &&
	    (sp->board_num > 0))
	{
		b = 0;
		if (sp->board_num_valid == 0)
			b |= 0x80;
		if (sp->board_num_garbled)
			b |= 0x40;
		if (sp->board_num_update == 0)
			b |= 0x20;

		ccc           = sp->board_num;
		code[cnt]     = (ccc >> 16) | b;
		code[cnt + 1] = ccc >> 8;
		code[cnt + 2] = ccc;
		cnt          += 3;
		fspec[1]     |= 0x40;
	}
	// Высота и запас топлива по данным НРЗ (SP03)
	if ((sp->interrogate_system == IST_NRZ) &&
	    (sp->altitude_barometric != 0))
	{
		b = packUvdFuel(sp->fuel);
		if (!(sp->barometric_altitude_absolute))
			b |= 0x20;
		if (sp->sos)
			b |= 0x10;
		code[cnt]     = b;
		cnt          += 1;
		ccc           = sp->altitude_barometric / CMR_UVD_HEIGTH;
		code[cnt]     = ccc >> 8;
		code[cnt + 1] = ccc;
		cnt          += 2;
		fspec[1]     |= 0x20;
	}
	if (sp->interrogate_system == IST_UVD)             // reserv in flag
	{
		code[cnt] = 0x02;
		cnt      += 1;
		fspec[1] |= 0x08;
	}

	// Номер борта по данным УВД SP04 [3 byte]
	if ((sp->interrogate_system == IST_UVD) &&
	    (sp->board_num > 0))
	{
		b = 0;
		if (sp->board_num_valid == 0)
			b |= 0x80;
		if (sp->board_num_garbled)
			b |= 0x40;
		if (sp->board_num_update == 0)
			b |= 0x20;

		ccc           = sp->board_num;
		code[cnt]     = (ccc >> 16) | b;
		code[cnt + 1] = ccc >> 8;
		code[cnt + 2] = ccc;
		cnt          += 3;
		fspec[1]     |= 0x04;
	}
	// Высота и запас топлива по данным УВД SP05
	if ((sp->interrogate_system == IST_UVD) &&
	    (sp->altitude_barometric != 0))
	{
		b = packUvdFuel(sp->fuel);
		if (!(sp->barometric_altitude_absolute))
			b |= 0x20;
		if (sp->sos)
			b |= 0x10;
		code[cnt]     = b;
		cnt          += 1;
		ccc           = sp->altitude_barometric / CMR_UVD_HEIGTH;
		code[cnt]     = ccc >> 8;
		code[cnt + 1] = ccc;
		cnt          += 2;
		fspec[1]     |= 0x02;
	}

	code[ 1 ] = fspec[ 1 ];
	code[ 0 ] = cnt;

	sp_data = initObmenData(cnt);

	if (sp_data != NULL)
	{
		setObmenData(sp_data, code, cnt);
	}
	return sp_data;
}

/**
 * @brief Распаковка элемента данных последнего измерения трассы ВО
 * @param code - указатель на упакованные данные
 * @param cnt - позиционный номер начального байта данных
 * @return - указатель на структуру элемента данных 340 категории 062
 */
Cat062measuredInformation* decode062ed340(unsigned char* code, u_int64_t* cnt, u_int64_t codo_size)
{
	u_int64_t                  k        = *cnt;
	u_int8_t                   octet    = 0;
	Cat062measuredInformation* msd_info = NULL;
	TMeasuredData*             md_data  = NULL;

	if (code == NULL)
		return msd_info;

	if (checkOverloadFunc(k, 1, codo_size) < 0)
	{
		return NULL;
	}

	if (code[k] == 0)
		return msd_info;

	octet = code[k];
	k    += 1;

	msd_info = (Cat062measuredInformation*) malloc(sizeof(Cat062measuredInformation));

	if (msd_info == NULL)
	{
		fprintf(stderr, "proc[pid-%d] In decode062measuredInfo ERROR malloc for element 340 memory \n", getpid());
		return NULL;
	}
	else
	{
		bzero(msd_info, sizeof(Cat062measuredInformation));
		msd_info->measured_info = (TMeasuredData*) malloc(sizeof(TMeasuredData));
		if (msd_info->measured_info == NULL)
		{
			free(msd_info);
			fprintf(stderr, "proc[pid-%d] In decode062measuredInfo ERROR malloc for element 340 memory \n", getpid());
			return NULL;
		}
		else
		{
			bzero(msd_info->measured_info, sizeof(TMeasuredData));
			md_data = msd_info->measured_info;
		}
	}
	// SID sic_sac code src
	if (octet & 0x80)
	{
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			free(md_data);
			free(msd_info);
			return NULL;
		}

		if (md_data)
			md_data->sensor_sac_sic = (code[k] << 8) | code[k + 1];
		k += 2;
	}
	// Az & D last scan
	if (octet & 0x40)
	{
		if (checkOverloadFunc(k, 4, codo_size) < 0)
		{
			free(md_data);
			free(msd_info);
			return NULL;
		}
		if (md_data)
			md_data->distanse = ((code[k] << 8) | code[k + 1]) * CMR_ASTX_D;
		k += 2;
		if (md_data)
			md_data->azimuth = (((code[k] << 8) | code[k + 1]) * CMR_ASTX_AZ) / DEG;
		k += 2;
	}
	// H from 3D RLS  src
	if (octet & 0x20)
	{
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			free(md_data);
			free(msd_info);
			return NULL;
		}
		if (md_data)
			md_data->height = ((code[k] << 8) | code[k + 1]) * CMR_ASTX_H_25F;
		k += 2;
	}
	// Hbar from Mode C src
	if (octet & 0x10)
	{
		int      zn = 1;
		u_int8_t bb;

		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			free(md_data);
			free(msd_info);
			return NULL;
		}

		bb = ((code[k] & 0x3f) << 8) | code[k + 1];
		if (code[k] & 0x20)
		{
			bb |= (0xC0 << 8);
			bb  = ~(bb - 1);
			zn  = -1;
		}

		if (md_data)
		{
			md_data->mode_c.altitude = (int) (zn * bb * CMR_ASTX_H_25F);
			md_data->mode_c.valid    = !!(code[k] & 0x80);
			md_data->mode_c.garbled  = !!(code[k] & 0x40);
		}

		k += 2;
	}
	// number from Mode 3A src
	if (octet & 0x08)
	{
		if (checkOverloadFunc(k, 2, codo_size) < 0)
		{
			free(md_data);
			free(msd_info);
			return NULL;
		}

		if (md_data)
		{
			md_data->mode3a.board_number_code = ((code[k] & 0x0f) << 8) | code[k + 1];
			md_data->mode3a.valid             = !!(code[k] & 0x80);
			md_data->mode3a.garbled           = !!(code[k] & 0x40);
			if (!(code[k] & 0x20))
				md_data->mode3a.change = 1;
		}
		k += 2;
	}
	// tii from src
	if (octet & 0x04)
	{
		if (checkOverloadFunc(k, 1, codo_size) < 0)
		{
			free(md_data);
			free(msd_info);
			return NULL;
		}
		if (md_data)
		{
			md_data->src_type    = code[k] >> 5;
			md_data->simulate    = !!(code[k] & 0x10);
			md_data->rab         = !!(code[k] & 0x80);
			md_data->test_target = !!(code[k] & 0x40);
		}
		k += 1;
	}

	*cnt = k;

	return msd_info;
}

/**
 * @brief Распаковка записи категории 062(ed.1.14 July 2011) формата АСТЕРИКС
 * @param ptr - указатель на данные с кодограммой АСТЕРИКС
 * @param start_byte_pos - начальный
 * @return - указатель на структуру категории Астерикс
 */
TCatAsterix* decode062catRecord(TCommonObmenData* codogramm, u_int64_t* start_byte_pos)
{
	unsigned char* code    = NULL;
	TCatAsterix*   asterix = NULL;
	TCat062*       cat     = NULL;
	u_int64_t      cdgm_size, k = 0;
	int            fspec_size = 0;


	if (codogramm == NULL)
	{
		fprintf(stderr, "proc[pid-%d] decode062catRecord data pointer ERROR\n", getpid());
		return asterix;
	}
	cdgm_size = getObmenDataLenght(codogramm);
	if (cdgm_size == 0)
	{
		fprintf(stderr, "proc[pid-%d] decode062catRecord null size data ERROR\n", getpid());
		return asterix;
	}

	code = (unsigned char*) getObmenData(codogramm);

	if (code == NULL)
	{
		fprintf(stderr, "proc[pid-%d] decode062catRecord data array ERROR\n", getpid());
		return asterix;
	}


	asterix = createCatAsterixData(CODT_CAT_062);

	if (asterix)
	{
		int i;

		cat = get062category(asterix);

		set062catDefaultData(cat);

		for (i = STUTT_TRACK_AGE; i <= STUTT_MLAT_AGE; i++)
			setTrackSystemTimeAges(asterix, DEFAULT_TIME_AGES, i);
		for (i = TUAT_MFL_AGE; i <= TUAT_BPS_AGE; i++)
			setTrackDataAges(asterix, DEFAULT_TIME_AGES, i);
	}
	else
	{
		fprintf(stderr, "proc[pid-%d] decodeCodogramm create category ERROR\n", getpid());
		return asterix;
	}

	if (checkOverloadFunc(k, FSPEC_SIZE_5_BYTE, cdgm_size) < 0)
	{
		freeCatAsterixData(asterix);
		return NULL;
	}

	fspec_size = getCategoryFspecSize(code, k, cdgm_size);

	if ((fspec_size > 0) && (fspec_size <= FSPEC_SIZE_5_BYTE) && (k + fspec_size < cdgm_size))
	{
		if (cat->item062_fspec.code)
		{
			memset(cat->item062_fspec.code, 0, FSPEC_SIZE_5_BYTE);
			memcpy(cat->item062_fspec.code, code + k, fspec_size);
			cat->item062_fspec.code_size = fspec_size;
		}
		k += fspec_size;
	}
	else
	{
		freeCatAsterixData(asterix);
		fprintf(stderr, "proc[pid-%d] decode062catRecord FSPEC ERROR\n", getpid());
		return NULL;
	}

	// I062/010 sic_sac ident code
	if (cat->item062_fspec.code[ 0 ] & 0x80)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_010.sac_sic = (code[k] << 8) | code[k + 1];
		k                       += 2;
	}
	// I062/015  service code
	if (cat->item062_fspec.code[ 0 ] & 0x20)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_015.sevice_id = code[k];
		k                         += 1;
	}
	// I062/070  time message
	if (cat->item062_fspec.code[ 0 ] & 0x10)
	{
		int ccc;
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		ccc                            = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
		cat->item062_070.time_of_track = (double) (ccc * CMR_ASTX_TIME);
		k                             += 3;
	}
	// I062/105  WGS-84 coord
	if (cat->item062_fspec.code[ 0 ] & 0x08)
	{
		double ddd = 0.0, zn = 1.0;
		int    i, ccc;

		if (checkOverloadFunc(k, 8, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		for (i = 0; i < 2; i += 1)
		{
			ccc = (code[k] << 24) | (code[k + 1] << 16) | (code[k + 2] << 8) | code[k + 3];
			if (code[k] & 0x02)
			{
				zn  = -1.0;
				ccc = ~(ccc - 1);
			}
			else
				zn = 1.0;

			ddd = (zn * ccc * CMR_GEO_25) / DEG;

			if (i == 0)
				cat->item062_105.latitude = ddd;
			else
				cat->item062_105.longitude = ddd;

			k += 4;
		}
	}
	// I062/100  XY coord
	if (cat->item062_fspec.code[ 0 ] & 0x04)
	{
		double ddd = 0.0, zn = 1.0;
		int    i, ccc;

		if (checkOverloadFunc(k, 6, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		for (i = 0; i < 2; i += 1)
		{
			ccc = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
			if (code[k] & 0x80)
			{
				ccc |= 0xff000000;
				zn   = -1.0;
				ccc  = ~(ccc - 1);
			}
			else
				zn = 1.0;
			ddd = zn * ccc * CAT062_CMR_XY;

			if (i == 0)
				cat->item062_100.x = ddd;  // m
			else
				cat->item062_100.y = ddd;
			k += 3;
		}
	}
	// I062/185  speed vx vy
	if (cat->item062_fspec.code[ 0 ] & 0x02)
	{
		double    ddd = 0.0, zn = 1.0;
		u_int16_t bb;
		int       i;
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		for (i = 0; i < 2; i += 1)
		{
			bb = (code[k] << 8) | code[k + 1];
			if (code[k] & 0x80)
			{
				zn = -1.0;
				bb = ~(bb - 1);
			}
			else
				zn = 1.0;
			ddd = zn * bb * CMR_VXVY;          // m/sec

			if (i == 0)
				cat->item062_185.vx = ddd; // m/sec
			else
				cat->item062_185.vy = ddd; // m/sec

			k += 2;
		}
	}
	// I062/210  calculated speed acceleration (ax, ay)
	if (cat->item062_fspec.code[ 1 ] & 0x80)
	{
		double   ddd = 0.0, zn = 1.0;
		int      i;
		u_int8_t b;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		for (i = 0; i < 2; i += 1)
		{
			b = code[k];
			if (b & 0x80)
			{
				zn = -1.0;
				b  = ~(b - 1);
			}
			else
				zn = 1.0;
			ddd = zn * b * CMR_VXVY;

			if (i == 0)
				cat->item062_210.ax = ddd; // m/sec^2
			else
				cat->item062_210.ay = ddd;

			k += 1;
		}
	}
	// I062/060  Mode 3A
	if (cat->item062_fspec.code[ 1 ] & 0x40)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		cat->item062_060.m3a.change            = !!(code[k] & 0x20);
		cat->item062_060.m3a.board_number_code = ((code[k] & 0x0F) << 8) | code[k + 1];
		k                                     += 2;
	}
	// I062/245  Ident VO LA-5
	if (cat->item062_fspec.code[ 1 ] & 0x20)
	{
		u_int64_t cnt_pos;
		if (checkOverloadFunc(k, 7, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_245.target_id_src = code[k] >> 6;
		k                             += 1;
		cnt_pos                        = k;
		convertIA5codeToAscii((unsigned char*) cat->item062_245.target_id, code, &cnt_pos);
		k = cnt_pos;
	}
	// I062/380  ADS-B data
	if (cat->item062_fspec.code[ 1 ] & 0x10)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_380 = decode062ed380(code, &k, cdgm_size);
	}
	// I062/040  trs num
	if (cat->item062_fspec.code[ 1 ] & 0x08)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_040.track_number = (code[k] << 8) | code[k + 1];
		k                            += 2;
	}
	// I062/080  status trs
	if (cat->item062_fspec.code[ 1 ] & 0x04)
	{
		u_int8_t b;
		// 1 octet
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		b  = code[k];
		k += 1;

		cat->item062_080.track_status = malloc(sizeof(TTrackStatus));

		if (cat->item062_080.track_status)
		{
			bzero(cat->item062_080.track_status, sizeof(TTrackStatus));
			cat->item062_080.track_status->monosensor           = !!(b & 0x80);
			cat->item062_080.track_status->spi_sign             = !!(b & 0x40);
			cat->item062_080.track_status->most_reliable_height = !!(b & 0x20);
			// H tii
			switch ((b & 0x1C) >> 2)
			{
			case 0: cat->item062_080.track_status->geometric_altitude_source = AST_UNKNOWN;
				break;
			case 1: cat->item062_080.track_status->geometric_altitude_source = AST_GPS;
				break;
			case 2: cat->item062_080.track_status->geometric_altitude_source = AST_3D_RADAR;
				break;
			case 3: cat->item062_080.track_status->geometric_altitude_source = AST_TRIANGULATION;
				break;
			case 4: cat->item062_080.track_status->geometric_altitude_source = AST_COVERAGE;
				break;
			case 5: cat->item062_080.track_status->geometric_altitude_source = AST_TABLE;
				break;
			case 6: cat->item062_080.track_status->geometric_altitude_source = AST_FICTION;
				break;
			case 7: cat->item062_080.track_status->geometric_altitude_source = AST_MLAT;
				break;
			}
			cat->item062_080.track_status->confirm = !!(b & 0x02);
		}
		// 2 octet
		if (b & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			b  = code[k];
			k += 1;
			if (cat->item062_080.track_status)
			{
				cat->item062_080.track_status->simulate        = !!(b & 0x80);
				cat->item062_080.track_status->last_message    = !!(b & 0x40);
				cat->item062_080.track_status->first_message   = !!(b & 0x20);
				cat->item062_080.track_status->flight_in_plan  = !!(b & 0x10);
				cat->item062_080.track_status->ads_inconsist   = !!(b & 0x08);
				cat->item062_080.track_status->slave_track     = !!(b & 0x04);
				cat->item062_080.track_status->background_serv = !!(b & 0x02);
			}
			// 3 octet
			if (b & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				b  = code[k];
				k += 1;
				if (cat->item062_080.track_status)
				{
					cat->item062_080.track_status->amalgamation      = !!(b & 0x80);
					cat->item062_080.track_status->mode4_interrogate = (b & 0x60) >> 5;
					cat->item062_080.track_status->military_emrg     = !!(b & 0x10);
					cat->item062_080.track_status->military_id       = !!(b & 0x08);
					cat->item062_080.track_status->mode5_interrogate = (b & 0x06) >> 1;
				}
				// 4 octet
				if (b & 0x01)
				{
					if (checkOverloadFunc(k, 1, cdgm_size) < 0)
					{
						freeCatAsterixData(asterix);
						return NULL;
					}
					b  = code[k];
					k += 1;
					if (cat->item062_080.track_status)
					{
						cat->item062_080.track_status->coast_track  = !!(b & 0x80);
						cat->item062_080.track_status->psr_coast    = !!(b & 0x40);
						cat->item062_080.track_status->ssr_coast    = !!(b & 0x20);
						cat->item062_080.track_status->mode_s_coast = !!(b & 0x10);
						cat->item062_080.track_status->ads_coast    = !!(b & 0x08);
						cat->item062_080.track_status->suc_set      = !!(b & 0x04);
						cat->item062_080.track_status->aac_conflict = !!(b & 0x02);
					}
					// 5 octet
					if (b & 0x01)
					{
						if (checkOverloadFunc(k, 1, cdgm_size) < 0)
						{
							freeCatAsterixData(asterix);
							return NULL;
						}
						b  = code[k];
						k += 1;
						if (cat->item062_080.track_status)
						{
							cat->item062_080.track_status->sds_status       = b >> 6;
							cat->item062_080.track_status->emergency_status = (b & 0x38) >> 3;
						}
					}                  // 5octet
				}                          // 4 octet
			}                                  // 3 octet
		}                                          // 2 octet
	}
	// I062/290  news times
	if (cat->item062_fspec.code[ 1 ] & 0x02)
	{
		u_int8_t octet1 = 0, octet2 = 0;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		// 2 octet
		octet1 = code[k];
		k     += 1;
		// 2 octet
		if (octet1 & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			octet2 = code[k];
			k     += 1;
		}

		if (octet1 & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_290.trk_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_290.psr_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_290.ssr_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x10)                         // Mode S time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_290.mds_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x08)                         // ADS_C time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_290.ads_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x04)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_290.ads_es_age = decodeAgeTime(code[k]);
			k                          += 1;
		}
		if (octet1 & 0x02)                         // VDL time
		{
			cat->item062_290.vdl_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		// 2 octet
		if (octet2 & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_290.uat_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet2 & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_290.loop_age = decodeAgeTime(code[k]);
			k                        += 1;
		}
		if (octet2 & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_290.mlt_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
	}
	// I062/200  moving param
	if (cat->item062_fspec.code[ 2 ] & 0x80)
	{
		u_int8_t b;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		if ((code[ k ] >> 6) == 1)
			cat->item062_200.transversal = TMM_TURN_RIGHT;
		if ((code[ k ] >> 6) == 2)
			cat->item062_200.transversal = TMM_TURN_LEFT;
		b = (code[ k ] & 0x30) >> 4;
		if (b == 1)
			cat->item062_200.long_acceleration = TMM_INCREASING_GROUNDSPEED;
		if (b == 2)
			cat->item062_200.long_acceleration = TMM_DECREASING_GROUNDSPEED;
		b = (code[ k ] & 0x0C) >> 2;
		if (b == 1)
			cat->item062_200.vertical_rate = TMM_VERTICAL_RATE_CLIMB;
		if (b == 2)
			cat->item062_200.vertical_rate = TMM_VERTICAL_RATE_DESCENT;
		cat->item062_200.altitude_discrepancy = !!(code[ k ] & 0x02);
		k                                    += 1;

	}
	// I062/295  adv news times
	if (cat->item062_fspec.code[ 2 ] & 0x40)
	{
		u_int8_t octets[FSPEC_SIZE_5_BYTE] =
		{
			0
		}, b, i;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		b         = 0;
		octets[b] = code[k];
		k        += 1;

		for (i = 1; i < FSPEC_SIZE_5_BYTE; i++)
		{
			if (octets[ b ] & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				b++;
				octets[ b ] = code[k];
				k          += 1;
//                if(b==0) break;
			}
			else
				break;
		}
		// 1 octet
		if (octets[0] & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.mfl_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[0] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.md1_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[0] & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.md2_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[0] & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}

			cat->item062_295.mda_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[0] & 0x08)                      // Mode 4
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}

			cat->item062_295.md4_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[0] & 0x04)                      // Mode 5
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.md5_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[0] & 0x02)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.mhg_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		// 2 octet
		if (octets[1] & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.ias_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[1] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.tas_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[1] & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.sal_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[1] & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.fss_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[1] & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.tid_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[1] & 0x04)                      // Comm/ACAS time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.com_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[1] & 0x02)                      // ADS time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.sab_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		// 3 octet
		if (octets[2] & 0x80)                      // ACAS report
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.acs_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[2] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.bvr_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[2] & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.gvr_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[2] & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.ran_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[2] & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.tar_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[2] & 0x04)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.tan_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[2] & 0x02)                      // Ground speed
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.gsp_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		// 4 octet
		if (octets[3] & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.vun_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[3] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.met_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[3] & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.emc_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[3] & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.pos_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[3] & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.gal_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[3] & 0x04)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.pun_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[3] & 0x02)                      // Mode S data
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.mb_age = decodeAgeTime(code[k]);
			k                      += 1;
		}
		// 5 octet
		if (octets[4] & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.iar_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[4] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.mac_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octets[4] & 0x20)                      // board pressure time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_295.bps_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
	}
	// I062/136  measured FL
	if (cat->item062_fspec.code[ 2 ] & 0x20)
	{
		int zn = 1, ccc;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		ccc = (code[k] << 8) | code[k + 1];
		if (code[k] & 0x80)
		{
			zn  = -1;
			ccc = ~(ccc - 1);
		}
		cat->item062_136.measured_fligth_level = (double) (zn * ccc * CMR_ASTX_H_25F); // m
		k                                     += 2;
	}
	// I062/130  calc H geo
	if (cat->item062_fspec.code[ 2 ] & 0x10)
	{
		int ccc;
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		ccc                            = (code[k] << 8) | code[k + 1];
		cat->item062_130.calc_g_height = (double) (ccc * CMR_ASTX_H_6_25F);            // m
		k                             += 2;
	}
	// I062/135  calc H bar
	if (cat->item062_fspec.code[ 2 ] & 0x08)
	{
		int       zn = 1;
		u_int16_t bb;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		bb = ((code[k] & 0x7F) << 8) | code[k + 1];

		cat->item062_135.qnh_correction = !!(code[k] & 0x80);

		if (code[k] & 0x40)
		{
			bb |= 0x8000;
			zn  = -1;
			bb  = ~(bb - 1);
		}
		cat->item062_135.calc_b_height = (double) (zn * bb * CMR_ASTX_H_25F); // m
		k                             += 2;
	}
	// I062/220  speed vh
	if (cat->item062_fspec.code[ 2 ] & 0x04)
	{
		int       zn = 1;
		u_int16_t bb;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		bb = (code[k] << 8) | code[k + 1];
		if (code[k] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		cat->item062_220.vh = (double) (zn * bb * CMR_ASTX_VH); // m/Sec
		k                  += 2;
	}
	// I062/390  plan info
	if (cat->item062_fspec.code[ 2 ] & 0x02)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_390 = decodeElem390PlanData(code, &k, cdgm_size);
	}
	// I062/270  VO size param
	if (cat->item062_fspec.code[ 3 ] & 0x80)
	{
		u_int8_t b;
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		b                       = code[k];
		k                      += 1;
		cat->item062_270.length = b >> 1;                               // metr
		if (b & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			b                            = code[k];
			k                           += 1;
			cat->item062_270.orientation = ((b >> 1) * 2.81) / DEG; // grad
			if (b & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				b                      = code[k];
				k                     += 1;
				cat->item062_270.width = b >> 1;                // metr
			}
		}
	}
	// I062/300  Vehicle Fleet Identification
	if (cat->item062_fspec.code[ 3 ] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_300.fleet_id = code[k];
		k                        += 1;
	}
	// I062/110  Mode 5 Data reports & Extended Mode 1 Code
	if (cat->item062_fspec.code[ 3 ] & 0x20)
	{
		u_int8_t octet = 0, b;
		int      ccc;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		octet = code[k];
		k    += 1;

		if (octet & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			b                                  = code[k];
			k                                 += 1;
			cat->item062_110.m5_interrogate    = !!(b & 0x80);
			cat->item062_110.m5_id_reply       = !!(b & 0x40);
			cat->item062_110.m5_report         = !!(b & 0x20);
			cat->item062_110.m5_mode1_present  = !!(b & 0x10);
			cat->item062_110.m5_mode2_present  = !!(b & 0x08);
			cat->item062_110.m5_mode3_present  = !!(b & 0x04);
			cat->item062_110.m5_mode_c_present = !!(b & 0x02);
//            if( b & 0x01)
			//              cat->item062_110.m5_x_pulse_set = 1;
		}
		if (octet & 0x40)
		{
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_110.m5_pin_code    = (code[k] << 8) | code[k + 1];
			k                              += 2;
			cat->item062_110.m5_national    = code[k];
			k                              += 1;
			cat->item062_110.m5_misson_code = code[k];
			k                              += 1;
		}
		if (octet & 0x20)
		{
			if (checkOverloadFunc(k, 6, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			ccc                           = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
			k                            += 3;
			cat->item062_110.m5_latitude  = ccc * CMR_GEO_23;
			ccc                           = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
			k                            += 3;
			cat->item062_110.m5_longitude = ccc * CMR_GEO_23;
		}
		if (octet & 0x10)
		{
			float resolution = CMR_ASTX_H_100F;
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			if (code[k] & 0x40)
				resolution = CMR_ASTX_H_25F;
			ccc                               = ((code[k] & 0x3f) << 8) | code[k + 1];
			k                                += 2;
			cat->item062_110.m5_gnss_altitude = ccc * resolution;
		}
		if (octet & 0x08)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_110.m5_extend_mode1_code = ((code[k] & 0x0f) << 8) | code[k + 1];
			k                                    += 2;
		}
		if (octet & 0x04)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_110.m5_valid_time_offset = code[k] * CMR_ASTX_TIME;
			k                                    += 1;
		}
		if (octet & 0x02)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_110.m5_x_pulse_set = code[k] & 0x1f;
			k                              += 1;
		}
	}
	// I062/120  Track Mode 2 Code
	if (cat->item062_fspec.code[ 3 ] & 0x10)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_120.m2.board_number_code = ((code[k] & 0x0f) << 8) | code[k + 1];
		k                                    += 2;
	}
	// I062/510  composed track number( number chaine - связка номеров)
	if (cat->item062_fspec.code[ 3 ] & 0x08)
	{
//        u_int8_t b, b1;
//        u_int16_t bb;
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
//        b = code[k];
		k += 1;                                    // source id code
//        bb= (code[k]<<8)|code[k+1];//OT number
//        bb>>=1;
//        b1=code[k+1];
		k += 2;
		// if(getVerboseMode(ctx))
		// fprintf(stdout,"%02X %02X %02X number chain src code=%d number=%d", code[k-3], code[k-2], code[k-1], b, bb);
		while (code[k - 1] & 0x01)
		{
			if (checkOverloadFunc(k, 3, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			//                b = code[k];
			k += 1;                            // source id code
			//                bb= (code[k]<<8)|code[k+1];//CHT number
			//                bb>>=1;
			//                b1=code[k+1];
			k += 2;
			// if(getVerboseMode(ctx))
			// fprintf(stdout,"%02X %02X %02X number chain src code=%d number=%d", code[k-3], code[k-2], code[k-1], b, bb);
		}
	}
	// I062/500  accuracy
	if (cat->item062_fspec.code[ 3 ] & 0x04)
	{
		u_int8_t octet1 = 0, octet2 = 0;
		int      ccc;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		octet1 = code[k];
		k     += 1;
		if (octet1 & 0x01)
			octet2 = code[k];
		k += 1;
		// CKO on XY
		if (octet1 & 0x80)                                                      // Estimated Accuracy Of Track Position (Cartesian)
		{
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			ccc                                   = (code[k] << 8) | code[k + 1];
			k                                    += 2;
			cat->item062_500.accuracy.x_component = ccc * CAT062_CMR_XY;
			ccc                                   = (code[k] << 8) | code[k + 1];
			k                                    += 2;
			cat->item062_500.accuracy.y_component = ccc * CAT062_CMR_XY;
		}
		// CKO cov XY
		if (octet1 & 0x40)                                                      // XY covariance component
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			ccc                                        = (code[k] << 8) | code[k + 1];
			k                                         += 2;
			cat->item062_500.accuracy.xy_cov_component = ccc * CAT062_CMR_XY;
		}
		// CKO geo
		if (octet1 & 0x20)                                                      // Estimated Accuracy Of Track Position (WGS-84)
		{
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			ccc                                        = (code[k] << 8) | code[k + 1];
			k                                         += 2;
			cat->item062_500.accuracy.lat_component    = (ccc * CMR_GEO_25) / DEG;
			ccc                                        = (code[k] << 8) | code[k + 1];
			k                                         += 2;
			cat->item062_500.accuracy.longit_component = (ccc * CMR_GEO_25) / DEG;
		}
		// CKO on H geometric
		if (octet1 & 0x10)                                                      // Estimated Accuracy Of Calculated Track Geometric Altitude
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_500.accuracy.h_geo_component = code[k] * CMR_ASTX_H_6_25F;
			k                                        += 1;
		}
		// CKO on H barometric
		if (octet1 & 0x08)                                                      // Estimated Accuracy Of Calculated Track Barometric Altitude
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_500.accuracy.h_bar_component = code[k] * CMR_ASTX_H_25F;
			k                                        += 1;
		}
		// CKO VXVY
		if (octet1 & 0x04)                                                      // Estimated Accuracy Of Track Velocity (Cartesian)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_500.accuracy.vx_component = code[k] * CMR_VXVY;    // m/sec
			k                                     += 1;
			cat->item062_500.accuracy.vy_component = code[k] * CMR_VXVY;    // m/sec
			k                                     += 1;
		}
		// CKO on AXAY
		if (octet1 & 0x02)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_500.accuracy.ax_component = code[k] * CMR_VXVY;    // m/sec^2
			k                                     += 1;
			cat->item062_500.accuracy.ay_component = code[k] * CMR_VXVY;    // m/sec^2
			k                                     += 1;
		}
		// CKO on VH
		if (octet2 & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item062_500.accuracy.vh_component = code[k] * CMR_ASTX_VH; // m/sec
			k                                     += 1;
		}
	}
	// I062/340  last measured data
	if (cat->item062_fspec.code[ 3 ] & 0x02)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		if (code[k] > 0)
			cat->item062_340 = decode062ed340(code, &k, cdgm_size);
	}
	// I062/Reserv Expansion Field (RF) -
	if (cat->item062_fspec.code[ 4 ] & 0x04)
	{
		u_int8_t octet = 0;
		int      ind_t = 0, j = 0, i;              // , zn = 1;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		// b = code[k];
		k    += 1;                                 // size RF element
		octet = code[k];
		k    += 1;                                 // RF fspec

		// bzero( &rf_info, sizeof(T_Trs_RF_Append));

		// list of src trace
		if (octet & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			ind_t = code[k];
			k    += 1;                         // rep factor
			j     = ind_t;
			for (i = 0; i < ind_t; i++)
			{
				// if(i < MAX_SRC_RF62)
				{
					if (checkOverloadFunc(k, 5, cdgm_size) < 0)
					{
						freeCatAsterixData(asterix);
						return NULL;
					}
					// bb=(code[k]<<8)|code[k+1];
					// rf_info.src_data[i].sic_sac=bb;
					k += 2;
					// b2=code[k]&0x0f;
					k += 1;
//                    switch(b2)
//                    {
//                        case 0: rf_info.src_data[i].tii |= fTII_EXTRA; break;
//                        case 1: rf_info.src_data[i].tii |= fTII_PRL; break;
//                        case 2: rf_info.src_data[i].tii |= fTII_VRL; break;
//                        case 3: rf_info.src_data[i].tii |= (fTII_PRL|fTII_VRL); break;
//                        case 4:
//                        case 5:
//                            rf_info.src_data[i].tii |= fTII_MODE_S;
//                            break;
//                        case 6:
//                        case 7:
//                            rf_info.src_data[i].tii |= (fTII_MODE_S|fTII_PRL);
//                            break;
//                        case 8: rf_info.src_data[i].tii |= fTII_ADS_B; break;
//                        case 9: rf_info.src_data[i].tii |= fTII_WAM; break;
//                    }
					// bb=(code[k]<<8)|code[k+1];
//                    rf_info.src_data[i].cht_trs_num=bb;
					k += 2;
				}
				// else
				// k+=5;
			}
		}
		// list of src koordinate
		if (octet & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			ind_t = code[k];
			k    += 1;                         // rep factor

			for (i = j; i < ind_t + j; i++)
			{
//                if(i < MAX_SRC_RF62)
				{
					if (checkOverloadFunc(k, 3, cdgm_size) < 0)
					{
						freeCatAsterixData(asterix);
						return NULL;
					}
					// bb=(code[k]<<8)|code[k+1];
//                    rf_info.src_data[i].sic_sac=bb;
					k += 2;
					// b2=code[k]&0x0f;
					k += 1;
//                    switch(b2)
//                    {
//                        case 0: rf_info.src_data[i].tii |= fTII_EXTRA; break;
//                        case 1: rf_info.src_data[i].tii |= fTII_PRL; break;
//                        case 2: rf_info.src_data[i].tii |= fTII_VRL; break;
//                        case 3: rf_info.src_data[i].tii |= (fTII_PRL|fTII_VRL); break;
//                        case 4:
//                        case 5:
//                            rf_info.src_data[i].tii |= fTII_MODE_S;
//                            break;
//                        case 6:
//                        case 7:
//                            rf_info.src_data[i].tii |= (fTII_MODE_S|fTII_PRL);
//                            break;
//                        case 8: rf_info.src_data[i].tii |= fTII_ADS_B; break;
//                        case 9: rf_info.src_data[i].tii |= fTII_WAM; break;
//                    }
				}
//                else
//                    k+=3;
			}
		}
		// speed of src trace
		if (octet & 0x20)
		{
			// float ff;
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			for (i = 0; i < 2; i += 1)
			{
				// bb=(code[k]<<8)|code[k+1];
//                if(code[k]&0x80)
//                {
//                    //bb = ~(bb-1);
//                    //zn = -1;
//                }
//                else
//                {
//                    zn = 1;
//                }
				// ff = zn*bb*CMR_VXVY;
//                if(i==0)
//                    rf_info.VX_gi = ff;
//                else
//                    rf_info.VY_gi = ff;
				k += 2;
			}
		}
		// arm flag
		if (octet & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			// b2=code[k];
			k += 1;
//            if(!(b2&0x80))
//                rf_info.arm|=fARM_FLAG_IN_PLAN;
		}

	}
	// I062/SP
	if (cat->item062_fspec.code[ 4 ] & 0x02)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		if (code[k] > 0)                           // Есть данные поля SP
		{
			cat->item062_sp = malloc(sizeof(Cat062specialPuposeField));

			if (cat->item062_sp)
			{
				bzero(cat->item062_sp, sizeof(Cat062specialPuposeField));
				setElementHeader(&cat->item062_sp->head, CAT062_ELSP, code[k], code + k, "Special Purpose Field");
				cat->item062_sp->el_sp = decode062defaultSPdata(code, &k, cdgm_size);
			}
		}
	}

	*start_byte_pos = k;
	return asterix;
}

/**
 * @brief Упаковка данных наблюдения АЗН-В для сообщения категори 62
 * @param ads_b - указатель на структуру данных АЗН-В
 * @return - указатель на структуру с кодированными данными
 */
TCommonObmenData* pack062element380AdsData(AircraftDerivedData* ads_b)
{
	u_int8_t ads_spec[FSPEC_SIZE_4_BYTE] =
	{
		0
	}, code[DEFAULT_CODE_SIZE] =
	{
		0
	};
	int               cnt         = 0;
	TCommonObmenData* packed_data = NULL;

	if (ads_b == NULL)
		return packed_data;

	code[cnt] = 0;
	cnt      += 1;                                     // ads_fspec
	code[cnt] = 0;
	cnt      += 1;                                     // ads_fspec
	code[cnt] = 0;
	cnt      += 1;                                     // ads_fspec
	code[cnt] = 0;
	cnt      += 1;                                     // ads_fspec

	// Adres VO
	if (ads_b->target_address > 0)                     // ICAO24
	{
		code[cnt]      = ads_b->target_address >> 16;
		cnt           += 1;
		code[cnt]      = ads_b->target_address >> 8;
		cnt           += 1;
		code[cnt]      = ads_b->target_address;
		cnt           += 1;
		ads_spec[ 0 ] |= 0x80;
	}
	// Identifik VO
	if (strlen(ads_b->target_identification) > 0)
	{
		u_int64_t cnt_pos;
		code[cnt] = 0;
		cnt      += 1;
		// 0 - from VO
		// 0x40 - not from VO
		// 0x80 - regist no downlinked
		cnt_pos = cnt;
		convertAsciiToIA5code(code, (unsigned char*) ads_b->target_identification, &cnt_pos);
		cnt            = cnt_pos;
		ads_spec[ 0 ] |= 0x40;
	}
	// Course VO
	if (ads_b->magnetic_heading)
	{
		int ccc = (int) (ads_b->magnetic_heading / CMR_ASTX_AZ);
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 0 ] |= 0x20;
	}
	// Speed barometric/Mach
	if (ads_b->indicated_airspeed > 0)
	{
		int   ccc;
		float ff;
		if (ads_b->mach_indicator == 1)                  // Mach speed
		{
			ff          = 0.001;                     // cmr mach
			code[ cnt ] = 0x80;
		}
		else
			ff = CMR_ASTX_SPEED;                     // cmr m/sec
		ccc            = (int) (ads_b->indicated_airspeed / ff);
		ccc           &= 0x7fff;
		code[cnt]     |= ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 0 ] |= 0x10;
	}
	// Speed real measured
	if (ads_b->true_airspeed > 0)
	{
		int ccc = (int) (ads_b->true_airspeed / 0.5144); // m/sec
		code[cnt]      = ccc >> 8;
		cnt           +=  1;
		code[cnt]      = ccc;
		cnt           +=  1;
		ads_spec[ 0 ] |= 0x08;
	}
	// Selected Alt VO SAL
	if (ads_b->selected_altitude != 0)
	{
		int ccc;
		if (ads_b->selected_altitude_valid)
			code[cnt] |= 0x80;
		if (ads_b->selected_altitude_src == ASA_FROM_AIRCRAFT)
			code[cnt] |= 0x20;
		else if (ads_b->selected_altitude_src == ASA_FROM_FCU_MCP)
			code[cnt] |= 0x40;
		else if (ads_b->selected_altitude_src == ASA_FROM_FMS)
			code[cnt] |= 0x60;

		ccc            = (int) (ads_b->selected_altitude / CMR_ASTX_H_25F);
		ccc           &= 0x1fff;
		code[cnt]     |= (ccc >> 8);
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 0 ] |= 0x04;
	}
	// Final Alt VO FSS
	if (ads_b->final_selected_altitude != 0)
	{
		int ccc;
		if (ads_b->fsa_manage_vertical_mode == 1)
			code[cnt] |= 0x80;
		if (ads_b->fsa_altitude_hold == 1)
			code[cnt] |= 0x40;
		if (ads_b->fsa_approach_mode == 1)
			code[cnt] |= 0x20;

		ccc            = (int) (ads_b->final_selected_altitude / CMR_ASTX_H_25F);
		ccc           &= 0x1fff;
		code[cnt]     |= (ccc >> 8);
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 0 ] |= 0x02;
	}
	// Status trajectory VO
	if (ads_b->trajectory_available == 1)
	{
		if (ads_b->trajectory_available == 1)
			code[cnt] |= 0x80;
		if (ads_b->trajectory_valid == 1)
			code[cnt] |= 0x40;
		cnt           += 1;
		ads_spec[ 1 ] |= 0x80;
	}
	// Data trajectory VO
	if ((ads_b->route_point_count > 0) && (ads_b->route_point))
	{
		int      ccc, i;
		u_int8_t b = 0;

		// REP factor
		code[ cnt ] = ads_b->route_point_count;
		cnt        += 1;
		for (i = 0; i < ads_b->route_point_count; i++)
		{
			if (ads_b->route_point[i].point_num == 0)
				code[cnt] |= 0x80;
			if (!(ads_b->route_point[i].compliance))
				code[cnt] |= 0x40;
			code[cnt] |= ads_b->route_point[i].point_num & 0x3f;
			cnt       += 1;
			ccc        = (int) (ads_b->route_point[i].point_altitude / CMR_ASTX_H_10F);
			code[cnt]  = ccc >> 8;
			cnt       += 1;
			code[cnt]  = ccc;
			cnt       += 1;
			ccc        = (int) ((ads_b->route_point[i].point_latitude * DEG) / CMR_GEO_23);
			code[cnt]  = ccc >> 16;
			cnt       += 1;
			code[cnt]  = ccc >> 8;
			cnt       += 1;
			code[cnt]  = ccc;
			cnt       += 1;
			ccc        = (int) ((ads_b->route_point[i].point_longitude * DEG) / CMR_GEO_23);
			code[cnt]  = ccc >> 16;
			cnt       += 1;
			code[cnt]  = ccc >> 8;
			cnt       += 1;
			code[cnt]  = ccc;
			cnt       += 1;
			code[cnt]  = ads_b->route_point[i].point_type << 4;
			b          = 0x0C;
			if (ads_b->route_point[i].point_turn_sign == TMM_TURN_RIGHT)
				b = 0x04;
			if (ads_b->route_point[i].point_turn_sign == TMM_TURN_LEFT)
				b = 0x08;
			if (ads_b->route_point[i].point_time)
				code[cnt] |= 0x01;                                                  // TOA
			if (ads_b->route_point[i].point_turn_radius)
				code[cnt] |= 0x02;                                                  // TRA
			code[cnt] |= b;
			cnt       += 1;
			ccc        = ads_b->route_point[i].point_time;                              // sec from 00:00
			code[cnt]  = ccc >> 16;
			cnt       += 1;
			code[cnt]  = ccc >> 8;
			cnt       += 1;
			code[cnt]  = ccc;
			cnt       += 1;
			ccc        = (int) (ads_b->route_point[i].point_turn_radius / CMR_ADS_TTR); // turn radius in metr
			code[cnt]  = ccc >> 8;
			cnt       += 1;
			code[cnt]  = ccc;
			cnt       += 1;
		}
		ads_spec[ 1 ] |= 0x40;
	}
	//! TCAS Data VO
	if ((ads_b->communication_capability) ||
	    (ads_b->flight_status))
	{
		code[cnt]  = (ads_b->communication_capability) << 5;
		code[cnt] |= ((ads_b->flight_status & 0x07) << 2);
		cnt       += 1;
		if (ads_b->specific_service == 1)
			code[cnt] |= 0x80;
		if (ads_b->altitude_report_capability == 1)
			code[cnt] |= 0x40;
		if (ads_b->aircraft_ident_capability == 1)
			code[cnt] |= 0x20;
		if (ads_b->bit_b1a == 1)
			code[cnt] |= 0x10;
		code[cnt]     |= (ads_b->bits_b1b & 0x0f);
		cnt           += 1;
		ads_spec[ 1 ] |= 0x20;
	}
	//! Status ADS VO
	if ((ads_b->flight_status_emergency) ||
	    (ads_b->acas_operational) ||
	    (ads_b->multiple_navigational) ||
	    (ads_b->differencial_correction) ||
	    (ads_b->ground_bit))
	{
		code[cnt]  = (ads_b->acas_operational & 0x03) << 6;
		code[cnt] |= (ads_b->multiple_navigational & 0x03) << 4;
		code[cnt] |= (ads_b->differencial_correction & 0x03) << 2;
		if (ads_b->ground_bit == 1)
			code[cnt] |= 0x02;
		cnt           += 1;
		code[cnt]      = ads_b->flight_status_emergency;
		cnt           += 1;
		ads_spec[ 1 ] |= 0x10;
	}
	//! Status ACAS VO
	if (ads_b->acas_56_bit_msg)
	{
		int i;
		for (i = 0; i < 7; i++)
		{
			code[cnt] = (ads_b->acas_56_bit_msg >> (8 * (6 - i)));
			cnt      += 1;
		}
		ads_spec[ 1 ] |= 0x08;
	}
	//! Bar vertic rate VO
	if (ads_b->vh_barometric != 0)
	{
		int ccc = (int) (ads_b->vh_barometric / CMR_ASTX_H_6_25F);
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 1 ] |= 0x04;
	}
	//! Geometric vertic rate VO
	if (ads_b->vh_geometric != 0)
	{
		int ccc = (int) (ads_b->vh_geometric / CMR_ASTX_H_6_25F);
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 1 ] |= 0x02;
	}
	//! Turn angle VO
	if (ads_b->roll_angle != 0)
	{
		int ccc = (int) ((ads_b->roll_angle * DEG) / 0.01); // grad
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 2 ] |= 0x80;
	}
	//! Turn rate VO
	if (ads_b->turn_rate != 0)
	{
		int ccc;
		if (ads_b->turn_indicator == TMM_TURN_LEFT)
			code[cnt] = 1;
		else if (ads_b->turn_indicator == TMM_TURN_RIGHT)
			code[cnt] = 2;
		else
			code[cnt] = 3;
		ccc            = (int) (ads_b->turn_rate / CMR_VXVY);                  // grad/sec
		cnt           += 1;
		code[cnt]      = ccc << 1;
		cnt           += 1;
		ads_spec[ 2 ] |= 0x40;
	}
	//! Course VO
	if (ads_b->track_angle)
	{
		int ccc = (unsigned short) ((ads_b->track_angle * DEG) / CMR_ASTX_AZ); // grad
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 2 ] |= 0x20;
	}
	//! Ground speed VO
	if (ads_b->ground_speed != 0)
	{
		int ccc = (unsigned short) (ads_b->ground_speed / CMR_ASTX_SPEED);     // grad
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 2 ] |= 0x10;
	}
	//! SCO speed VO
	if (ads_b->velocity_uncertainty)
	{
		code[cnt]      = (unsigned char) (ads_b->velocity_uncertainty);
		cnt           += 1;
		ads_spec[ 2 ] |= 0x08;
	}
	//! Meteodata VO
	if ((ads_b->wind_speed > 0) ||
	    (ads_b->wind_direct > 0) ||
	    (ads_b->temperature != 0) ||
	    (ads_b->turbulence > 0))
	{
		int ccc;
		code[cnt] = 0;
		if (ads_b->wind_speed_valid)
			code[cnt] |= 0x80;
		if (ads_b->wind_direct_valid)
			code[cnt] |= 0x40;
		if (ads_b->temperature_valid)
			code[cnt] |= 0x20;
		if (ads_b->turbulence_valid)
			code[cnt] |= 0x10;
		cnt += 1;
		if (ads_b->wind_speed > 0)                                   // wind speed
			ccc = (unsigned short) (ads_b->wind_speed / 0.5144); // m/sec
		else
			ccc = 0;
		code[cnt] = ccc >> 8;
		cnt      += 1;
		code[cnt] = ccc;
		cnt      += 1;

		if (ads_b->wind_direct > 0)                                  // wind direct
			ccc = (unsigned short) (ads_b->wind_direct / 1.0);
		else
			ccc = 0;
		code[cnt] = ccc >> 8;
		cnt      += 1;
		code[cnt] = ccc;
		cnt      += 1;

		if (ads_b->temperature != 0)                                 // temperature
			ccc = (int) (ads_b->temperature / 0.25);             // grad
		else
			ccc = 0;
		code[cnt] = ccc >> 8;
		cnt      += 1;
		code[cnt] = ccc;
		cnt      += 1;
		if (ads_b->turbulence != 0)                                  // turbulence
			code[cnt] = (unsigned char) ads_b->turbulence;
		else
			code[cnt] = 0;
		cnt += 1;

		ads_spec[ 2 ] |= 0x04;
	}
	//! Transmitter cat VO
	if (ads_b->emitter_category)
	{
		code[cnt]      = ads_b->emitter_category;
		cnt           += 1;
		ads_spec[ 2 ] |= 0x02;
	}
	//! Position data VO
	if ((ads_b->latitude_wgs84 != 0) ||
	    (ads_b->longitude_wgs84 != 0))
	{
		int ccc = (int) ((ads_b->latitude_wgs84 * DEG) / CMR_GEO_23);
		code[cnt]      = ccc >> 16;
		cnt           += 1;
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ccc            = (int) ((ads_b->longitude_wgs84 * DEG) / CMR_GEO_23);
		code[cnt]      = ccc >> 16;
		cnt           += 1;
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 3 ] |= 0x80;
	}
	//! Geo Alt VO
	if (ads_b->geometric_altitude > 0)
	{
		int ccc = (int) (ads_b->geometric_altitude / CMR_ASTX_H_6_25F); // meter
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 3 ] |= 0x40;
	}
	//! SCO Position data VO
	if (ads_b->position_uncertainty > 0)
	{
		// y.e. 0...15
		code[cnt]      = ads_b->position_uncertainty;
		cnt           += 1;
		ads_spec[ 3 ] |= 0x20;
	}
	//! Mode S Data
	if ((ads_b->mode_s_msg) && (ads_b->mode_s_data_count > 0))
	{
		int i, j;
		// rep factor
		code[cnt] = ads_b->mode_s_data_count;
		cnt      += 1;
		for (i = 0; i < ads_b->mode_s_data_count; i++)
			for (j = 0; j < 8; j++)
			{
				code[ cnt ] = (ads_b->mode_s_msg[i] >> (8 * (7 - j)));
				cnt        += 1;
			}



		ads_spec[ 3 ] |= 0x10;
	}
	//! Barom speed data VO
	if (ads_b->indicated_airspeed > 0)
	{
		int ccc = (unsigned short) (ads_b->indicated_airspeed / 1.609); // km/hour
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 3 ] |= 0x08;
	}
	//! Mach speed data VO
	if (ads_b->mach_number > 0)
	{
		int ccc = (unsigned short) (ads_b->mach_number / 0.008);        // Mach
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 3 ] |= 0x04;
	}
	//! Barometr pressure VO
	if (ads_b->barometric_pressure_setting != 0)
	{
		int ccc = (int) (ads_b->barometric_pressure_setting / 0.1);     // mB
		code[cnt]      = ccc >> 8;
		cnt           += 1;
		code[cnt]      = ccc;
		cnt           += 1;
		ads_spec[ 3 ] |= 0x02;
	}

	if (cnt > FSPEC_SIZE_4_BYTE)
	{
		u_int8_t spec_pos;
		spec_pos = packFspecToCodogramm(code, ads_spec, FSPEC_SIZE_4_BYTE);
		cnt     -= (FSPEC_SIZE_4_BYTE - spec_pos);
		memmove(code + spec_pos, code + FSPEC_SIZE_4_BYTE, cnt);

		packed_data = initObmenData(cnt);
		if (packed_data != NULL)
		{
			setObmenData(packed_data, code, cnt);
		}
	}

	return packed_data;
}

/**
 * @brief Упаковка записи категории 062 (ed.1.14 July 2011) Астерикс
 * @param cat - указатель контейнер категории 062
 * @return - указатель на структуру с кодированными данными
 */
TCommonObmenData* pack062catRecord(TCat062* cat)
{
	u_int8_t code[DEFAULT_CODE_SIZE] =
	{
		0
	};
	u_int8_t fspec_pos = 0;
	// T_Trs_RF_Append  rf_field_app;
	size_t            cnt = 0;
	TCommonObmenData* msg = NULL;


	if (cat == NULL)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 062 ERROR - nothing function argument\n", getpid());
		return msg;
	}

	if (cat->item062_fspec.code == NULL)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 062 ERROR - fspec code not set\n", getpid());
		return msg;
	}

	if (cat->item062_010.sac_sic == 0)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 062 ERROR - sic sac data NULL\n", getpid());
		return msg;
	}

	if ((cat->item062_070.time_of_track < 0) &&
	    (cat->item062_070.time_of_track >= DEFAULT_TIME_OF_TRACK))
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 062 ERROR - time of Day not present\n", getpid());
		return msg;
	}

	if (cat->item062_040.track_number == 0)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 062 ERROR - track number NULL\n", getpid());
		return msg;
	}

	if (cat->item062_080.track_status == NULL)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 062 ERROR - track number NULL\n", getpid());
		return msg;
	}

	cnt += FSPEC_SIZE_5_BYTE;

	// I062/010 sic_sac ident code
	if (cat->item062_fspec.code[fspec_pos] & 0x80)
	{
		code[cnt]     = cat->item062_010.sac_sic >> 8;
		code[cnt + 1] = cat->item062_010.sac_sic;
		cnt          += 2;
	}

	// I062/015  service code
	if (cat->item062_fspec.code[fspec_pos] & 0x20)     // service kode
	{
		code[cnt] = cat->item062_015.sevice_id;
		cnt      += 1;
	}
	// I062/070  time message
	// time on server
	// in sec from 00:00
	if (cat->item062_fspec.code[fspec_pos] & 0x10)
	{
		int ccc = (int) (cat->item062_070.time_of_track / CMR_ASTX_TIME);
		code[cnt] = ccc >> 16;
		cnt      += 1;
		code[cnt] = ccc >> 8;
		cnt      += 1;
		code[cnt] = ccc;
		cnt      += 1;
	}

	// I062/105  WGS-84 coord
	if (cat->item062_fspec.code[fspec_pos] & 0x08)
	{
		int diff = cnt, i, ccc;
		if ((cat->item062_105.latitude != DEFAULT_RADIAN_GEO_POS) ||
		    (cat->item062_105.longitude != DEFAULT_RADIAN_GEO_POS))
		{
			for (i = 0; i < 2; i++)
			{
				if (i == 0)
					ccc = (int) (cat->item062_105.latitude / (CMR_GEO_25 * M_PI / 180.));
				if (i == 1)
					ccc = (int) (cat->item062_105.longitude / (CMR_GEO_25 * M_PI / 180.));
				code[cnt] = ccc >> 24;
				cnt      += 1;
				code[cnt] = ccc >> 16;
				cnt      += 1;
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
			}
		}

		if ((cnt - diff) == 0)
			cat->item062_fspec.code[fspec_pos] &= ~0x08;
	}
	// I062/100  XY coord
	if (cat->item062_fspec.code[fspec_pos] & 0x04)
	{
		int diff = cnt, i, ccc;
		if ((cat->item062_100.x != DEFAULT_CARTESIAN_POS) ||
		    (cat->item062_100.y != DEFAULT_CARTESIAN_POS))
		{
			for (i = 0; i < 2; i++)
			{
				if (i == 0)
					ccc = (int) (cat->item062_100.x / CAT062_CMR_XY);
				if (i == 1)
					ccc = (int) (cat->item062_100.y / CAT062_CMR_XY);

				//            if(i==0)
				//                ccc1=trs->pos.y;
				//            if(i==1)
				//                ccc1=trs->pos.x;
				code[cnt] = ccc >> 16;
				cnt      += 1;
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
			}
		}
		if ((cnt - diff) == 0)
			cat->item062_fspec.code[fspec_pos] &= ~0x04;
	}
	// I062/185  speed vx vy
	if (cat->item062_fspec.code[fspec_pos] & 0x02)
	{
		int diff = cnt, i, ccc;

		if ((cat->item062_185.vx != DEFAULT_VELOCITY) ||
		    (cat->item062_185.vy != DEFAULT_VELOCITY))
		{
			for (i = 0; i < 2; i++)
			{
				if (i == 0)
					ccc = (int) (cat->item062_185.vx / CMR_VXVY);
				if (i == 1)
					ccc = (int) (cat->item062_185.vy / CMR_VXVY);
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
			}
		}
		if ((cnt - diff) == 0)
			cat->item062_fspec.code[fspec_pos] &= ~0x02;
	}

	fspec_pos = 1;
	// I062/210  speed ax ay
	if (cat->item062_fspec.code[fspec_pos] & 0x80)
	{
		int diff = cnt, i, ccc;

		if ((cat->item062_210.ax != DEFAULT_ACCELERATION) ||
		    (cat->item062_210.ay != DEFAULT_ACCELERATION))
		{
			for (i = 0; i < 2; i++)
			{
				if (i == 0)
					ccc = (int) (cat->item062_210.ax / CMR_VXVY);
				if (i == 1)
					ccc = (int) (cat->item062_210.ay / CMR_VXVY);
				code[cnt] = ccc;
				cnt      += 1;
			}
		}
		if ((cnt - diff) == 0)
			cat->item062_fspec.code[fspec_pos] &= ~0x80;
	}
	// I062/060  Mode 3A
	if (cat->item062_fspec.code[fspec_pos] & 0x40)
	{
		if (cat->item062_060.m3a.board_number_code > DEFAULT_RBS_UVD_NUM_CODE)
		{
			code[cnt] = cat->item062_060.m3a.board_number_code >> 8;
			if (cat->item062_060.m3a.change == 1)
				code[cnt] |= 0x40;
			cnt      += 1;
			code[cnt] = cat->item062_060.m3a.board_number_code;
			cnt      += 1;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x40;
	}
	// I062/245  Ident VO IA-5
	if (cat->item062_fspec.code[fspec_pos] & 0x20)
	{
		if ((strlen(cat->item062_245.target_id) > 0) &&
		    (cat->item062_390 == NULL))
		{
			u_int64_t cnt_pos;
			u_int8_t  b = 0;
			switch (cat->item062_245.target_id_src)
			{
			case TIS_CALLSIGN_NOT_FROM_TRANSPONDER:
				b = 1;
				break;
			case TIS_REGISTRATION_NOT_FROM_TRANSPONDER:
				b = 2;
				break;
			case TIS_UNDEFINED:
				b = 3;
				break;
			}
			code[cnt] = b;
			cnt      += 1;
			cnt_pos   = cnt;
			convertAsciiToIA5code(code, (unsigned char*) cat->item062_245.target_id, &cnt_pos);
			cnt = cnt_pos;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x20;

	}
	// I062/380  ADS-B data
	if (cat->item062_fspec.code[fspec_pos] & 0x10)
	{
		if (cat->item062_380)
		{
			TCommonObmenData* ads = NULL;
			ads = pack062element380AdsData(cat->item062_380);
			if (ads)
			{
				int ccc = getObmenDataLenght(ads);
				if (ccc > 0)
				{
					unsigned char* p = NULL;
					p = (unsigned char*) getObmenData(ads);
					if (p != NULL)
					{
						memcpy(code + cnt, p, ccc);
						cnt += ccc;
					}
				}
				freeObmenData(ads);
			}
			else
				cat->item062_fspec.code[fspec_pos] &= ~0x10;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x10;
	}
	// I062/040  trs num
	if (cat->item062_fspec.code[fspec_pos] & 0x08)
	{
		code[cnt] = cat->item062_040.track_number >> 8;
		cnt      += 1;
		code[cnt] = cat->item062_040.track_number;
		cnt      += 1;
	}
	// I062/080  status trs
	// 1 octet
	if (cat->item062_fspec.code[fspec_pos] & 0x04)
	{
		u_int8_t octet[ FSPEC_SIZE_5_BYTE ] =
		{
			0
		}, b = 0, i;

		if (cat->item062_080.track_status)
		{
			if (cat->item062_080.track_status->monosensor)
				octet[ 0 ] |= 0x80;
			if (cat->item062_080.track_status->spi_sign)
				octet[ 0 ] |= 0x40;
			if (cat->item062_080.track_status->most_reliable_height)
				octet[ 0 ] |= 0x20;
			// by default b1=0 -> asterix->track_status.geometric_altitude_source ALTITUDE_SRC_UNKNOWN;
			switch (cat->item062_080.track_status->geometric_altitude_source)
			{
			case AST_UNKNOWN:  b = 0;
				break;
			case AST_GPS:      b = 1;
				break;
			case AST_3D_RADAR: b = 2;
				break;
			case AST_TRIANGULATION:   b = 3;
				break;
			case AST_COVERAGE: b = 4;
				break;
			case AST_TABLE:    b = 5;
				break;
			case AST_FICTION:  b = 6;
				break;
			case AST_MLAT:     b = 7;
				break;
			default:
				b = 0;
				break;
			}
			octet[ 0 ] |= (b << 2);
			if (cat->item062_080.track_status->confirm)
				octet[ 0 ] |= 0x02;

			// 2 octet

			if (cat->item062_080.track_status->simulate)
				octet[ 1 ] |= 0x80;
			if (cat->item062_080.track_status->last_message)
				octet[ 1 ] |= 0x40;
			if (cat->item062_080.track_status->first_message)
				octet[ 1 ] |= 0x20;
			if (cat->item062_080.track_status->flight_in_plan)
				octet[ 1 ] |= 0x10;
			if (cat->item062_080.track_status->ads_inconsist)
				octet[ 1 ] |= 0x08;
			if (cat->item062_080.track_status->slave_track)
				octet[ 1 ] |= 0x04;
			if (cat->item062_080.track_status->background_serv)
				octet[ 1 ] |= 0x02;
			if (octet[ 1 ] > 0)
			{
				octet[ 0 ] |= 0x01;
			}

			// 3 octet
			if (cat->item062_080.track_status->amalgamation)
				octet[ 2 ] |= 0x80;
			switch (cat->item062_080.track_status->mode4_interrogate)
			{
			case IFFR_OWN:          octet[ 2 ] |= 0x20;
				break;
			case IFFR_NOT_TESTED:   octet[ 2 ] |= 0x40;
				break;
			case IFFR_ALIEN:        octet[ 2 ] |= 0x60;
				break;
			}
			if (cat->item062_080.track_status->military_emrg)
				octet[ 2 ] |= 0x10;
			if (cat->item062_080.track_status->military_id)
				octet[ 2 ] |= 0x08;
			switch (cat->item062_080.track_status->mode5_interrogate)
			{
			case IFFR_OWN:         octet[ 2 ] |= 0x02;
				break;
			case IFFR_NOT_TESTED:  octet[ 2 ] |= 0x04;
				break;
			case IFFR_ALIEN:       octet[ 2 ] |= 0x06;
				break;
			}
			if (octet[ 2 ] > 0)
			{
				octet[ 0 ] |= 0x01;
				octet[ 1 ] |= 0x01;
			}

			// 4 octet
			if (cat->item062_080.track_status->coast_track)
				octet[ 3 ] |= 0x80;
			if (cat->item062_080.track_status->psr_coast)
				octet[ 3 ] |= 0x40;
			if (cat->item062_080.track_status->ssr_coast)
				octet[ 3 ] |= 0x20;
			if (cat->item062_080.track_status->mode_s_coast)
				octet[ 3 ] |= 0x10;
			if (cat->item062_080.track_status->ads_coast)
				octet[ 3 ] |= 0x08;
			if (cat->item062_080.track_status->suc_set)
				octet[ 3 ] |= 0x04;
			if (cat->item062_080.track_status->aac_conflict)
				octet[ 3 ] |= 0x02;

			if (octet[ 3 ] > 0)
			{
				octet[ 0 ] |= 0x01;
				octet[ 1 ] |= 0x01;
				octet[ 2 ] |= 0x01;
			}
			// 5 octet
			b           = cat->item062_080.track_status->sds_status & 0x03;
			octet[ 4 ] |= (b << 6);
			b           = cat->item062_080.track_status->emergency_status & 0x07;
			octet[ 4 ] |= (b << 3);
			if (octet[ 4 ] > 0)
			{
				octet[ 0 ] |= 0x01;
				octet[ 1 ] |= 0x01;
				octet[ 2 ] |= 0x01;
				octet[ 3 ] |= 0x01;
			}

			code[ cnt ] = octet[ 0 ];
			cnt        += 1;
			i           = 1;
			while (code[ cnt - 1 ] & 0x01)
			{
				code[ cnt ] = octet[ i ];
				cnt        += 1;
				i          += 1;
				if (i == FSPEC_SIZE_5_BYTE)
					break;
			}
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x04;
	}

	// I062/290  news times
	if (cat->item062_fspec.code[fspec_pos] & 0x02)
	{
		u_int8_t b    = 0;
		int      diff = cnt;

		if ((cat->item062_290.trk_age >= CMR_AGE_TIME) ||
		    (cat->item062_290.psr_age >= CMR_AGE_TIME) ||
		    (cat->item062_290.ssr_age >= CMR_AGE_TIME) ||
		    (cat->item062_290.mds_age >= CMR_AGE_TIME) ||
		    (cat->item062_290.ads_age >= CMR_AGE_TIME) ||
		    (cat->item062_290.ads_es_age >= CMR_AGE_TIME) ||
		    (cat->item062_290.vdl_age >= CMR_AGE_TIME) ||
		    (cat->item062_290.uat_age >= CMR_AGE_TIME) ||
		    (cat->item062_290.loop_age >= CMR_AGE_TIME) ||
		    (cat->item062_290.mlt_age >= CMR_AGE_TIME))
		{
			// 1 octet
			cnt            += 2;
			code[ diff ]    = 0;
			code[ diff + 1] = 0;

			if (cat->item062_290.trk_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.trk_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x80;
				}
			}
			if (cat->item062_290.psr_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.psr_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x40;
				}
			}
			if (cat->item062_290.ssr_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.ssr_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x20;
				}
			}
			if (cat->item062_290.mds_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.mds_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x10;
				}
			}

			if (cat->item062_290.ads_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.ads_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x08;
				}
			}

			if (cat->item062_290.ads_es_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.ads_es_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x04;
				}
			}

			if (cat->item062_290.vdl_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.vdl_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x02;
				}
			}

			// 2 octet
			if (cat->item062_290.uat_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.uat_age);
				if (b > 0)
				{
					code[ cnt ]      = b;
					cnt             += 1;
					code[ diff + 1] |= 0x80;
				}
			}
			if (cat->item062_290.loop_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.loop_age);
				if (b > 0)
				{
					code[ cnt ]      = b;
					cnt             += 1;
					code[ diff + 1] |= 0x40;
				}
			}
			if (cat->item062_290.mlt_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_290.mlt_age);
				if (b > 0)
				{
					code[ cnt ]      = b;
					cnt             += 1;
					code[ diff + 1] |= 0x20;
				}
			}

			if (cnt > (diff + 2))
			{
				if (code[ diff + 1] > 0)
				{
					code[diff] |= 0x01;
				}
				else
				{
					memmove(code + diff + 1, code + diff + 2, cnt - diff - 2);
					cnt -= 1;
				}
			}
			else
			{
				cnt = diff;
			}
		}

		if ((cnt - diff) == 0)
			cat->item062_fspec.code[fspec_pos] &= ~0x02;

	}

	fspec_pos = 2;
	// I062/200  Mode of Movement
	if (cat->item062_fspec.code[fspec_pos] & 0x80)
	{
		u_int8_t b = 0;
		if ((cat->item062_200.transversal) ||
		    (cat->item062_200.long_acceleration) ||
		    (cat->item062_200.vertical_rate) ||
		    (cat->item062_200.altitude_discrepancy))
		{
			if (cat->item062_200.transversal == TMM_TURN_RIGHT)
				b |= 0x40;
			if (cat->item062_200.transversal == TMM_TURN_LEFT)
				b |= 0x80;
			if (cat->item062_200.long_acceleration == TMM_INCREASING_GROUNDSPEED)
				b |= 0x10;
			if (cat->item062_200.long_acceleration == TMM_DECREASING_GROUNDSPEED)
				b |= 0x20;
			if (cat->item062_200.vertical_rate == TMM_VERTICAL_RATE_CLIMB)
				b |= 0x04;
			if (cat->item062_200.vertical_rate == TMM_VERTICAL_RATE_DESCENT)
				b |= 0x08;
			if (cat->item062_200.altitude_discrepancy)
				b |= 0x02;
		}

		if (b > 0)
		{
			code[ cnt ] = b;
			cnt        += 1;
		}
		else
		{
			cat->item062_fspec.code[fspec_pos] &= ~0x80;
		}
	}
	// I062/295  adv news times
	if (cat->item062_fspec.code[fspec_pos] & 0x40)
	{
		u_int8_t b    = 0;
		int      diff = cnt;

		if ((cat->item062_295.mfl_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.md1_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.md2_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.mda_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.md4_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.md5_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.mhg_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.ias_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.tas_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.sal_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.fss_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.tid_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.com_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.sab_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.acs_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.bvr_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.gvr_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.tar_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.tan_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.gsp_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.vun_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.met_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.emc_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.pos_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.gal_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.pun_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.mb_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.iar_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.mac_age >= CMR_AGE_TIME) ||
		    (cat->item062_295.bps_age >= CMR_AGE_TIME))
		{
			cnt           += 5;
			code[diff]     = 0;
			code[diff + 1] = 0;
			code[diff + 2] = 0;
			code[diff + 3] = 0;
			code[diff + 4] = 0;
			// 1 octet
			if (cat->item062_295.mfl_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.mfl_age);
				if (b > 0)
				{
					code[ cnt ] = b;
					cnt        += 1;
					code[diff] |= 0x80;
				}
			}
			if (cat->item062_295.md1_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.md1_age);
				if (b > 0)
				{
					code[ cnt ] = b;
					cnt        += 1;
					code[diff] |= 0x40;
				}
			}
			if (cat->item062_295.md2_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.md2_age);
				if (b > 0)
				{
					code[ cnt ] = b;
					cnt        += 1;
					code[diff] |= 0x20;
				}
			}
			if (cat->item062_295.mda_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.mda_age);
				if (b > 0)
				{
					code[ cnt ] = b;
					cnt        += 1;
					code[diff] |= 0x10;
				}
			}
			if (cat->item062_295.md4_age >= CMR_AGE_TIME) // Mode 4
			{
				b = packAgeTime(cat->item062_295.md4_age);
				if (b > 0)
				{
					code[ cnt ] = b;
					cnt        += 1;
					code[diff] |= 0x08;
				}
			}
			if (cat->item062_295.md5_age >= CMR_AGE_TIME) // Mode 5
			{
				b = packAgeTime(cat->item062_295.md5_age);
				if (b > 0)
				{
					code[ cnt ] = b;
					cnt        += 1;
					code[diff] |= 0x04;
				}
			}
			if (cat->item062_295.mhg_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.mhg_age);
				if (b > 0)
				{
					code[ cnt ] = b;
					cnt        += 1;
					code[diff] |= 0x02;
				}
			}

			// 2 octet
			if (cat->item062_295.ias_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.ias_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 1] |= 0x80;
				}
			}
			if (cat->item062_295.tas_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.tas_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 1] |= 0x40;
				}
			}
			if (cat->item062_295.sal_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.sal_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 1] |= 0x20;
				}
			}
			if (cat->item062_295.fss_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.fss_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 1] |= 0x10;
				}
			}
			if (cat->item062_295.tid_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.tid_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 1] |= 0x08;
				}
			}
			if (cat->item062_295.com_age >= CMR_AGE_TIME) // Comm/ACAS time
			{
				b = packAgeTime(cat->item062_295.com_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 1] |= 0x04;
				}
			}
			if (cat->item062_295.sab_age >= CMR_AGE_TIME) // ADS time
			{
				b = packAgeTime(cat->item062_295.sab_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 1] |= 0x02;
				}
			}
			if (code[diff + 1] > 0)
			{
				code[diff] |= 0x01;
			}

			// 3 octet
			if (cat->item062_295.acs_age >= CMR_AGE_TIME) // ACAS report
			{
				b = packAgeTime(cat->item062_295.acs_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 2] |= 0x80;
				}
			}
			if (cat->item062_295.bvr_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.bvr_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 2] |= 0x40;
				}
			}
			if (cat->item062_295.gvr_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.gvr_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 2] |= 0x20;
				}
			}
			if (cat->item062_295.ran_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.ran_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 2] |= 0x10;
				}
			}
			if (cat->item062_295.tar_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.tar_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 2] |= 0x08;
				}
			}
			if (cat->item062_295.tan_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.tan_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 2] |= 0x04;
				}
			}
			if (cat->item062_295.gsp_age >= CMR_AGE_TIME) // Ground speed
			{
				b = packAgeTime(cat->item062_295.gsp_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 2] |= 0x02;
				}
			}
			if (code[diff + 2] > 0)
			{
				code[ diff ]     |= 0x01;
				code[ diff + 1 ] |= 0x01;
			}

			// 4 octet
			if (cat->item062_295.vun_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.vun_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 3] |= 0x80;
				}
			}
			if (cat->item062_295.met_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.met_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 3] |= 0x40;
				}
			}
			if (cat->item062_295.emc_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.emc_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 3] |= 0x20;
				}
			}
			if (cat->item062_295.pos_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.pos_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 3] |= 0x10;
				}
			}
			if (cat->item062_295.gal_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.gal_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 3] |= 0x08;
				}
			}
			if (cat->item062_295.pun_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.pun_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 3] |= 0x04;
				}
			}
			if (cat->item062_295.mb_age >= CMR_AGE_TIME) // Mode S data
			{
				b = packAgeTime(cat->item062_295.mb_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 3] |= 0x02;
				}
			}
			if (code[diff + 3] > 0)
			{
				code[ diff ]   |= 0x01;
				code[diff + 1] |= 0x01;
				code[diff + 2] |= 0x01;
			}
			// 5 octet
			if (cat->item062_295.iar_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.iar_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 4] |= 0x80;
				}
			}
			if (cat->item062_295.mac_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item062_295.mac_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 4] |= 0x40;
				}
			}
			if (cat->item062_295.bps_age >= CMR_AGE_TIME) // board pressure time
			{
				b = packAgeTime(cat->item062_295.bps_age);
				if (b > 0)
				{
					code[ cnt ]     = b;
					cnt            += 1;
					code[diff + 4] |= 0x20;
				}
			}
			if (code[diff + 4] > 0)
			{
				code[diff]     |= 0x01;
				code[diff + 1] |= 0x01;
				code[diff + 2] |= 0x01;
				code[diff + 3] |= 0x01;
			}

			if ((cnt - diff) > 5)
			{
				int i, b1 = 1;

				for (i = diff; i < diff + 5; i++)
				{
					if (code[i] & 0x01)
						b1 += 1;
					else
						break;
				}

				memmove(code + diff + b1, code + diff + 5, cnt - diff - 5);

				cnt -= (5 - b1);
			}
			else
			{
				cnt = diff;
			}
		}

		if ((cnt - diff) == 0)
			cat->item062_fspec.code[fspec_pos] &= ~0x40;
	}
	// I062/136  measured FL
	if (cat->item062_fspec.code[fspec_pos] & 0x20)
	{
		if (cat->item062_136.measured_fligth_level != DEFAULT_ALTITUDE)
		{
			int ccc = (int) (cat->item062_136.measured_fligth_level / CMR_ASTX_H_25F); // m
			code[cnt] = ccc >> 8;
			cnt      += 1;
			code[cnt] = ccc;
			cnt      += 1;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x20;
	}
	// I062/130  calc H geo
	if (cat->item062_fspec.code[fspec_pos] & 0x10)
	{
		if (cat->item062_130.calc_g_height != DEFAULT_ALTITUDE)
		{
			int ccc = (int) (cat->item062_130.calc_g_height / CMR_ASTX_H_6_25F);       // m
			code[cnt] = ccc >> 8;
			cnt      += 1;
			code[cnt] = ccc;
			cnt      += 1;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x10;
	}
	// I062/135  calc H bar
	if (cat->item062_fspec.code[fspec_pos] & 0x08)
	{
		if (cat->item062_135.calc_b_height != DEFAULT_ALTITUDE)
		{
			int ccc = (int) (cat->item062_135.calc_b_height / CMR_ASTX_H_25F);         // m
			code[cnt] = ccc >> 8;
			if (cat->item062_135.qnh_correction)
				code[cnt] |= 0x80;
			cnt      += 1;
			code[cnt] = ccc;
			cnt      += 1;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x08;
	}
	// I062/220  speed vh
	if (cat->item062_fspec.code[fspec_pos] & 0x04)
	{
		if (cat->item062_220.vh != DEFAULT_VELOCITY)
		{
			int ccc = (int) (cat->item062_220.vh / CMR_ASTX_VH); // m/Sec
			code[cnt] = ccc >> 8;
			cnt      += 1;
			code[cnt] = ccc;
			cnt      += 1;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x04;
	}
	// I062/390  plan info
	if (cat->item062_fspec.code[fspec_pos] & 0x02)
	{
		if (cat->item062_390)
		{
			TCommonObmenData* pl = NULL;

			pl = packElem390PlanData(&cat->item062_390->fpdata, 0x00ffffff);

			if (pl)
			{
				int ccc = getObmenDataLenght(pl);
				if (ccc > 0)
				{
					unsigned char* p = NULL;
					p = (unsigned char*) getObmenData(pl);
					if (p != NULL)
					{
						memcpy(code + cnt, p, ccc);
						cnt += ccc;
					}
				}
				freeObmenData(pl);
			}
			else
				cat->item062_fspec.code[fspec_pos] &= ~0x02;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x02;
	}

	fspec_pos = 3;
	// I062/270  VO size param
	if (cat->item062_fspec.code[fspec_pos] & 0x80)
	{
		if ((cat->item062_270.length != DEFAULT_U_INT8_CODE) ||
		    (cat->item062_270.width != DEFAULT_U_INT8_CODE) ||
		    (cat->item062_270.orientation != DEFAULT_RADIAN_GEO_POS))
		{
			TReturnOneInt ret = packTargetSizeAndOrientation(cat->item062_270.length,
			                                                 cat->item062_270.width,
			                                                 cat->item062_270.orientation);
			if (ret.update)
			{
				code[ cnt ] = ret.val >> 16;
				cnt        += 1;
				if (code[ cnt - 1] & 0x01)
				{
					code[ cnt ] = ret.val >> 8;
					cnt        += 1;

					if (code[ cnt - 1] & 0x01)
					{
						code[ cnt ] = ret.val;
						cnt        += 1;
					}
				}
			}
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x80;
	}
	// I062/300 Vehicle Fleet Id
	if (cat->item062_fspec.code[fspec_pos] & 0x40)
	{
		if ((cat->item062_300.fleet_id > 0) &&
		    (cat->item062_300.fleet_id != DEFAULT_U_INT8_CODE))
		{
			code[ cnt ] = cat->item062_300.fleet_id;
			cnt        += 1;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x40;
	}
	// I062/120 Mode 2 Code
	if (cat->item062_fspec.code[fspec_pos] & 0x10)
	{
		if (cat->item062_120.m2.board_number_code > 0)
		{
			code[ cnt ] = cat->item062_120.m2.board_number_code >> 8;
			cnt        += 1;
			code[ cnt ] = cat->item062_120.m2.board_number_code;
			cnt        += 1;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x10;
	}
	// I062/510  number chaine
	// if( cat->item062_fspec.code[fspec_pos] & 0x08)
	// {
	// b = code[cnt]; cnt+=1;//source id code
	// bb= (code[cnt]<<8)|code[cnt+1];//OT number
	// bb>>=1;
	// b1=code[cnt+1]; cnt+=2;
	// if(b1&0x01)
	// {
	// while(b1&0x01)
	// {
	// b = code[cnt]; cnt+=1;//source id code
	// bb= (code[cnt]<<8)|code[cnt+1];//CHT number
	// bb>>=1;
	// b1=code[cnt+1]; cnt+=2;
	// }
	// }
	// }

	// I062/500  accuracy
	if (cat->item062_fspec.code[fspec_pos] & 0x04)
	{
		int diff = cnt, ccc;

		if ((cat->item062_500.accuracy.x_component) ||
		    (cat->item062_500.accuracy.y_component) ||
		    (cat->item062_500.accuracy.xy_cov_component) ||
		    (cat->item062_500.accuracy.lat_component) ||
		    (cat->item062_500.accuracy.longit_component) ||
		    (cat->item062_500.accuracy.h_geo_component) ||
		    (cat->item062_500.accuracy.h_bar_component) ||
		    (cat->item062_500.accuracy.vx_component) ||
		    (cat->item062_500.accuracy.vy_component) ||
		    (cat->item062_500.accuracy.vh_component) ||
		    (cat->item062_500.accuracy.ax_component) ||
		    (cat->item062_500.accuracy.ay_component))
		{
			cnt       += 2;
			code[diff] = 0;
			code[diff] = 0;
			// CKO on XY
			if ((cat->item062_500.accuracy.x_component != 0) ||
			    (cat->item062_500.accuracy.y_component != 0))
			{
				ccc = (unsigned short) (cat->item062_500.accuracy.x_component / 0.5);
				if (ccc > DEFAULT_U_INT16_CODE)
					ccc = DEFAULT_U_INT16_CODE;
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
				ccc       = (unsigned short) (cat->item062_500.accuracy.y_component / 0.5);
				if (ccc > DEFAULT_U_INT16_CODE)
					ccc = DEFAULT_U_INT16_CODE;
				code[cnt]   = ccc >> 8;
				cnt        += 1;
				code[cnt]   = ccc;
				cnt        += 1;
				code[diff] |= 0x80;
			}
			// CKO cov XY
			if (cat->item062_500.accuracy.xy_cov_component != 0)
			{
				ccc = (unsigned short) (cat->item062_500.accuracy.xy_cov_component / 0.5);
				if (ccc > DEFAULT_U_INT16_CODE)
					ccc = DEFAULT_U_INT16_CODE;
				code[cnt]   = ccc >> 8;
				cnt        += 1;
				code[cnt]   = ccc;
				cnt        += 1;
				code[diff] |= 0x40;
			}
			// CKO geo
			if ((cat->item062_500.accuracy.lat_component != 0) ||
			    (cat->item062_500.accuracy.longit_component != 0))
			{
				ccc = (unsigned short) (cat->item062_500.accuracy.lat_component * DEG / CMR_GEO_25);
				if (ccc > DEFAULT_U_INT16_CODE)
					ccc = DEFAULT_U_INT16_CODE;
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
				ccc       = (unsigned short) (cat->item062_500.accuracy.longit_component * DEG / CMR_GEO_25);
				if (ccc > DEFAULT_U_INT16_CODE)
					ccc = DEFAULT_U_INT16_CODE;
				code[cnt]   = ccc >> 8;
				cnt        += 1;
				code[cnt]   = ccc;
				cnt        += 1;
				code[diff] |= 0x20;
			}
			// CKO on H geo
			if (cat->item062_500.accuracy.h_geo_component != 0)
			{
				ccc = (unsigned char) (cat->item062_500.accuracy.h_geo_component / CMR_ASTX_H_6_25F);
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt]   = ccc;
				cnt        += 1;
				code[diff] |= 0x10;
			}
			// CKO on H bar
			if (cat->item062_500.accuracy.h_bar_component != 0)
			{
				ccc = (unsigned char) (cat->item062_500.accuracy.h_bar_component / CMR_ASTX_H_25F + 0.5);
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt]   = ccc;
				cnt        += 1;
				code[diff] |= 0x08;
			}
			// CKO VXVY
			if ((cat->item062_500.accuracy.vx_component != 0) ||
			    (cat->item062_500.accuracy.vy_component != 0))
			{
				ccc = (unsigned char) (cat->item062_500.accuracy.vx_component / 0.25);
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt] = ccc;
				cnt      += 1;
				ccc       = (unsigned char) (cat->item062_500.accuracy.vy_component / 0.25);
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt]   = ccc;
				cnt        += 1;
				code[diff] |= 0x04;
			}
			// CKO on AXAY
			if ((cat->item062_500.accuracy.ax_component != 0) ||
			    (cat->item062_500.accuracy.ay_component != 0))
			{
				ccc = (unsigned char) (cat->item062_500.accuracy.ax_component / 0.25);       // m/sec^2
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt] = ccc;
				cnt      += 1;
				ccc       = (unsigned char) (cat->item062_500.accuracy.ay_component / 0.25); // m/sec^2
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt]    = ccc;
				cnt         += 1;
				code[diff ] |= 0x02;
			}
			// CKO on VH
			if (cat->item062_500.accuracy.vh_component != 0)
			{
				ccc = (int) (cat->item062_500.accuracy.vh_component / CMR_ASTX_VH);          // m/sec
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt]      = ccc;
				cnt           += 1;
				code[diff ]   |= 0x01;
				code[diff + 1] = 0x80;
			}
			else
			{
				memmove(code + diff + 1, code + diff + 2, cnt - diff - 2);                   // 1 byte
				cnt -= 1;
			}
		}

		if ((cnt - diff) == 0)
			cat->item062_fspec.code[fspec_pos] &= ~0x04;
	}
	// I062/340  last measured data
	if (cat->item062_fspec.code[fspec_pos] & 0x02)
	{
		if (cat->item062_340)
		{
			if (cat->item062_340->measured_info)
			{
				TMeasuredData* mdi  = cat->item062_340->measured_info;
				int            diff = cnt, ccc;
				code[diff] = 0;
				cnt       += 1;
				// SID sic_sac code src
				if (mdi->sensor_sac_sic)
				{
					code[cnt]   = mdi->sensor_sac_sic >> 8;
					cnt        += 1;
					code[cnt]   = mdi->sensor_sac_sic;
					cnt        += 1;
					code[diff] |= 0x80;
				}
				// Az & D last scan
				if ((mdi->azimuth != 0) ||
				    (mdi->distanse != 0))
				{
					ccc         = (unsigned short) (mdi->azimuth / CMR_ASTX_AZ);
					code[cnt]   = ccc >> 8;
					cnt        += 1;
					code[cnt]   = ccc;
					cnt        += 1;
					ccc         = (unsigned short) (mdi->distanse / CMR_ASTX_D);
					code[cnt]   = ccc >> 8;
					cnt        += 1;
					code[cnt]   = ccc;
					cnt        += 1;
					code[diff] |= 0x40;
				}
				// H from 3D RLS  src
				if (mdi->height != 0)
				{
					ccc         = (unsigned short) (mdi->height / CMR_ASTX_H_25F);
					code[cnt]   = ccc >> 8;
					cnt        += 1;
					code[cnt]   = ccc;
					cnt        += 1;
					code[diff] |= 0x20;
				}
				// Hbar from Mode C src
				if (mdi->mode_c.altitude != 0)
				{
					ccc       = (unsigned short) (mdi->mode_c.altitude / CMR_ASTX_H_25F);
					code[cnt] = (ccc >> 8) & 0x3f;
					if (mdi->mode_c.valid)
						code[cnt] |= 0x80;
					if (mdi->mode_c.garbled)
						code[cnt] |= 0x40;
					cnt        += 1;
					code[cnt]   = ccc;
					cnt        += 1;
					code[diff] |= 0x10;
				}
				// number from Mode 3A src
				if (mdi->mode3a.board_number_code)
				{
					ccc       = mdi->mode3a.board_number_code;
					code[cnt] = (ccc >> 8) & 0x0f;
					if (mdi->mode3a.valid)
						code[cnt] |= 0x80;
					if (mdi->mode3a.garbled)
						code[cnt] |= 0x40;
					if (!(mdi->mode3a.change))
						code[cnt] |= 0x20;
					cnt        += 1;
					code[cnt]   = ccc;
					cnt        += 1;
					code[diff] |= 0x08;
				}
				// tii from src
				if ((mdi->src_type) ||
				    (mdi->simulate) ||
				    (mdi->rab) ||
				    (mdi->test_target))
				{
					u_int8_t b = 0;
					switch (mdi->src_type)
					{
					case MDS_PSR: b = 1;
						break;
					case MDS_SSR: b = 2;
						break;
					case MDS_PSR_SSR: b = 3;
						break;
					case MDS_MODE_S_ALL_CALL: b = 4;
						break;
					case MDS_MODE_S_ROLL_CALL: b = 5;
						break;
					case MDS_MODE_S_ALL_CALL_PSR: b = 6;
						break;
					case MDS_MODE_S_ROLL_CALL_PSR: b = 7;
						break;
					}
					code[cnt] = b << 5;
					if (mdi->simulate)
						code[cnt] |= 0x10;
					if (mdi->rab)
						code[cnt] |= 0x08;
					if (mdi->test_target)
						code[cnt] |= 0x04;
					cnt        += 1;
					code[diff] |= 0x04;
				}

				if ((cnt - diff) == 1)
				{
					cnt                                 = diff;
					cat->item062_fspec.code[fspec_pos] &= ~0x02;
				}
			}
			else
				cat->item062_fspec.code[fspec_pos] &= ~0x02;
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x02;
	}
	fspec_pos = 4;
	// I062/RF - TC TOI
/*
        if( FSPEC[2]&0x04 )
        {
            ccc = cnt; cnt+=2;//size RF element
            b=0; //RF fspec
            //bzero( &RF_app, sizeof(T_Trs_RF_Append));
            //! list of src trace
            if(b1&0x80)
            {
                ind_t=code[cnt]; cnt+=1;//rep factor
                j = ind_t;
                for(i=0; i<ind_t; i++)
                {
                    if(i<MAX_SRC_RF62)
                    {
                        bb=(code[cnt]<<8)|code[cnt+1];
                        RF_app.src_data[i].sic_sac=bb;
                        cnt+=2;
                        b2=code[cnt]&0x0f; cnt+=1;
                        switch(b2)
                        {
                            case 0: RF_app.src_data[i].tii |= fTII_EXTRA; break;
                            case 1: RF_app.src_data[i].tii |= fTII_PRL; break;
                            case 2: RF_app.src_data[i].tii |= fTII_VRL; break;
                            case 3: RF_app.src_data[i].tii |= (fTII_PRL|fTII_VRL); break;
                            case 4:
                            case 5:
                                RF_app.src_data[i].tii |= fTII_MODE_S;
                                break;
                            case 6:
                            case 7:
                                RF_app.src_data[i].tii |= (fTII_MODE_S|fTII_PRL);
                                break;
                            case 8: RF_app.src_data[i].tii |= fTII_ADS_B; break;
                            case 9: RF_app.src_data[i].tii |= fTII_WAM; break;
                        }
                        bb=(code[cnt]<<8)|code[cnt+1];
                        RF_app.src_data[i].cht_trs_num=bb;
                        cnt+=2;
                    }
                    else
                        cnt+=5;
                }
            }
            //! list of src koordinate
            if(b1&0x40)
            {
                ind_t=code[cnt]; cnt+=1;//rep factor

                for(i=j; i<ind_t+j; i++)
                {
                    if(i<MAX_SRC_RF62)
                    {
                        bb=(code[cnt]<<8)|code[cnt+1];
                        RF_app.src_data[i].sic_sac=bb;
                        cnt+=2;
                        b2=code[cnt]&0x0f; cnt+=1;
                        switch(b2)
                        {
                            case 0: RF_app.src_data[i].tii |= fTII_EXTRA; break;
                            case 1: RF_app.src_data[i].tii |= fTII_PRL; break;
                            case 2: RF_app.src_data[i].tii |= fTII_VRL; break;
                            case 3: RF_app.src_data[i].tii |= (fTII_PRL|fTII_VRL); break;
                            case 4:
                            case 5:
                                RF_app.src_data[i].tii |= fTII_MODE_S;
                                break;
                            case 6:
                            case 7:
                                RF_app.src_data[i].tii |= (fTII_MODE_S|fTII_PRL);
                                break;
                            case 8: RF_app.src_data[i].tii |= fTII_ADS_B; break;
                            case 9: RF_app.src_data[i].tii |= fTII_WAM; break;
                        }
                    }
                    else
                        cnt+=3;
                }
            }
                    //! speed of src trace
            if(b1&0x20)
            {
                bb=(code[cnt]<<8)|code[cnt+1];
                if(code[cnt]&0x80)
                {
                    bb=~(bb-1);
                    Zn=-1.0;
                }
                else
                    Zn=1.0;
                RF_app.VX_gi=Zn*bb*CMR_VXVY;
                cnt+=2;

                bb=(code[cnt]<<8)|code[cnt+1];
                if(code[cnt]&0x80)
                {
                    bb=~(bb-1);
                    Zn=-1.0;
                }
                else
                    Zn=1.0;
                RF_app.VY_gi=Zn*bb*CMR_VXVY;
                cnt+=2;
            }
                    //! arm flag
            if(b1&0x10)
            {
                b2=code[cnt]; cnt+=1;
                if(!(b2&0x80))
                    RF_app.arm|=fARM_FLAG_IN_PLAN;
            }

        }
 */
	// I062/SP
	if (cat->item062_fspec.code[fspec_pos] & 0x02)
	{
		if (cat->item062_sp)
		{
			TCommonObmenData* sp_elem = NULL;
			unsigned char*    p       = NULL;
			int               ccc;

			// Если нет упакованных данных от клиента, упаковываются
			// данные по умолчанию
			if (cat->item062_sp->head.code_size == 0)
			{
				sp_elem = pack062defaultSPdata(cat->item062_sp->el_sp);
				if (sp_elem)
				{
					ccc = getObmenDataLenght(sp_elem);
					if (ccc > 0)
						p = (unsigned char*) getObmenData(sp_elem);
				}
			}
			else
			{
				ccc = cat->item062_sp->head.code_size;
				if (cat->item062_sp->head.code)
					p = (unsigned char*) cat->item062_sp->head.code;
			}

			if (p != NULL)
			{
				memcpy(code + cnt, p, ccc);
				cnt += ccc;
			}
			else
				cat->item062_fspec.code[fspec_pos] &= ~0x02;

			if (sp_elem)
				freeObmenData(sp_elem);
		}
		else
			cat->item062_fspec.code[fspec_pos] &= ~0x02;
	}

	if (cnt > FSPEC_SIZE_5_BYTE)
	{
		fspec_pos = packFspecToCodogramm(code, cat->item062_fspec.code, FSPEC_SIZE_5_BYTE);
		cnt      -= (FSPEC_SIZE_5_BYTE - fspec_pos);
		memmove(code + fspec_pos, code + FSPEC_SIZE_5_BYTE, cnt);

		msg = initObmenData(cnt);

		if (msg != NULL)
		{
			setObmenData(msg, code, cnt);
		}
	}

	return msg;

}

/**
 * @brief Запрос данных последнего измерения параметров трассы из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - указатель на структуру данных последнего измерения
 */
TMeasuredData* get062TrackMeasuredInformation(TCat062* cat)
{
	TMeasuredData* ret = NULL;


	if ((cat == NULL) || (cat->item062_340 == NULL))
		return ret;

	if (cat->item062_340)
	{
		ret = (TMeasuredData*) cat->item062_340->measured_info;
	}
	return ret;
}

/**
 * @brief Запись данных измерения параметров траектории трассы ВО в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param msd_data - указатель на структуру данных последнего измерения
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set062TrackMeasuredInformation(TCat062* cat, TMeasuredData* msd_data)
{
	int res = -1;


	if ((cat == NULL) || (msd_data == NULL))
		return res;

	if ((cat) && (msd_data))
	{
		if (cat->item062_340 == NULL)
		{
			cat->item062_340 = malloc(sizeof(Cat062measuredInformation));
			if (cat->item062_340)
				bzero(cat->item062_340, sizeof(Cat062measuredInformation));
		}
		if (cat->item062_340)
		{
			res = 0;

			if (cat->item062_340 == NULL)
				cat->item062_340->measured_info = malloc(sizeof(TMeasuredData));
			if (cat->item062_340->measured_info)
			{
				bzero(cat->item062_340->measured_info, sizeof(TMeasuredData));
				memcpy(cat->item062_340->measured_info, msd_data, sizeof(TMeasuredData));
				if (cat->item062_fspec.code)
					cat->item062_fspec.code[3] |= 0x02;

			}
		}
	}
	return res;
}

/**
 * @brief Выдача указателя на структуру категории 062 из
 *        обобщенной структуры Астерикс
 * @param cat - указатель на структуру Астерикс
 * @return указатель на структуру категории 062 Астерикс,
 *         при отказе значение NULL
 */
TCat062* get062category(TCatAsterix* cat)
{
	TCat062* ret = NULL;


	if (cat == NULL)
		return ret;

	if (cat)
	{
		if (cat->cat_num == CODT_CAT_062)
			ret = &cat->category.c062;
	}

	return ret;
}

/**
 * @brief Запрос данных о содержимом FSPEC записи
 * @param cat - указатель структуру категории 062
 * @return - указатель на массив с байтами FSPEC записи
 */
u_int8_t* get062recordFspec(TCat062* cat, size_t* size)
{
	u_int8_t* ret = NULL;
	size_t    res = 0;

	if (cat == NULL)
	{
		*size = res;
		return ret;
	}

	if (cat->item062_fspec.code)
	{
		ret = cat->item062_fspec.code;
		res = cat->item062_fspec.code_size;
	}

	*size = res;
	return ret;
}

/**
 * @brief Выборка "сырых" данных из записи категории 062 без распаковки
 * @param codogramm - указатель на контейнер с кодограммой
 * @param current_pos - начальное положение первого байта записи в теле кодограммы
 * @return - указатель на структуру контейнер Астерикс
 */
TCatAsterix* get062recordRawData(TCommonObmenData* codogramm, u_int64_t current_pos)
{
	unsigned char  b       = 0, b1 = 0;
	unsigned char* code    = NULL;
	unsigned short bb      = 0;
	int            ccc     = 0, i;
	TCatAsterix*   asterix = NULL;
	TCat062*       cat     = NULL;
	u_int64_t      cdgm_size, k = 0;


	if (codogramm == NULL)
	{
		fprintf(stderr, "proc[pid-%d] get062recordRawData data pointer ERROR\n", getpid());
		return asterix;
	}
	cdgm_size = getObmenDataLenght(codogramm);
	if (cdgm_size == 0)
	{
		fprintf(stderr, "proc[pid-%d] get062recordRawData null size data ERROR\n", getpid());
		return asterix;
	}

	code = (unsigned char*) getObmenData(codogramm);

	if (code == NULL)
	{
		fprintf(stderr, "proc[pid-%d] get062recordRawData data array ERROR\n", getpid());
		return asterix;
	}

	asterix = createCatAsterixData(CODT_CAT_062);

	if (asterix)
	{
		cat = get062category(asterix);
	}
	else
	{
		fprintf(stderr, "proc[pid-%d] get062recordRawData create category ERROR\n", getpid());
		return asterix;
	}

	if (checkOverloadFunc(k, FSPEC_SIZE_5_BYTE, cdgm_size) < 0)
	{
		freeCatAsterixData(asterix);
		return NULL;
	}

	ccc = getCategoryFspecSize(code, k, cdgm_size);

	if ((ccc > 0) && (ccc <= FSPEC_SIZE_5_BYTE) && (k + ccc < cdgm_size))
	{
		if (cat->item062_fspec.code)
		{
			memcpy(cat->item062_fspec.code, code + k, ccc);
			cat->item062_fspec.code_size = ccc;
		}
		k += ccc;
	}
	else
	{
		freeCatAsterixData(asterix);
		fprintf(stderr, "proc[pid-%d] get062recordRawData FSPEC ERROR\n", getpid());
		return NULL;
	}

	// I062/010 sic_sac ident code
	if (cat->item062_fspec.code[ 0 ] & 0x80)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_010.head, CAT062_EL010, 2, code + k, "Data Source Identifier");
		k += 2;
	}
	// I062/015  service code
	if (cat->item062_fspec.code[ 0 ] & 0x20)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_015.head, CAT062_EL015, 1, code + k, "Service Identification");
		k += 1;
	}
	// I062/070  time message
	if (cat->item062_fspec.code[ 0 ] & 0x10)
	{
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_070.head, CAT062_EL070, 3, code + k, "Time Of Track Information");
		k += 3;
	}
	// I062/105  WGS-84 coord
	if (cat->item062_fspec.code[ 0 ] & 0x08)
	{
		if (checkOverloadFunc(k, 8, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item062_105.head, CAT062_EL105, 8, code + k, "Calculated Track Position (WGS-84)");
		k += 8;
	}
	// I062/100  XY coord
	if (cat->item062_fspec.code[ 0 ] & 0x04)
	{
		if (checkOverloadFunc(k, 6, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item062_100.head, CAT062_EL100, 6, code + k, "Calculated Track Position (Cartesian)");
		k += 6;
	}
	// I062/185  speed vx vy
	if (cat->item062_fspec.code[ 0 ] & 0x02)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item062_185.head, CAT062_EL185, 4, code + k, "Calculated Track Velocity (Cartesian)");
		k += 4;
	}
	// I062/210  calculated speed acceleration (ax, ay)
	if (cat->item062_fspec.code[ 1 ] & 0x80)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item062_210.head, CAT062_EL210, 2, code + k, "Calculated Acceleration (Cartesian)");
		k += 2;
	}
	// I062/060  Mode 3A
	if (cat->item062_fspec.code[ 1 ] & 0x40)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_060.head, CAT062_EL060, 2, code + k, "Track Mode 3/A Code");
		k += 2;
	}
	// I062/245  Ident VO LA-5
	if (cat->item062_fspec.code[ 1 ] & 0x20)
	{
		if (checkOverloadFunc(k, 7, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_245.head, CAT062_EL245, 7, code + k, "Target Identification");
		k += 7;
	}
	// I062/380  ADS-B data
	if (cat->item062_fspec.code[ 1 ] & 0x10)
	{
		u_int64_t diff = k;
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_380 = decode062ed380(code, &k, cdgm_size);
		if (cat->item062_380)
		{
			setElementHeader(&cat->item062_380->head, CAT062_EL380, k - diff, code + diff, "Aircraft Derived Data");
		}
	}
	// I062/040  trs num
	if (cat->item062_fspec.code[ 1 ] & 0x08)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_040.head, CAT062_EL040, 2, code + k, "Track Number");
		k += 2;
	}
	// I062/080  status trs
	if (cat->item062_fspec.code[ 1 ] & 0x04)
	{
		int differ = k;
		// 1 octet
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		k += 1;

		if (code[ k - 1] & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
			// 3 octet
			if (code[ k - 1] & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				k += 1;
				// 4 octet
				if (code[ k - 1] & 0x01)
				{
					if (checkOverloadFunc(k, 1, cdgm_size) < 0)
					{
						freeCatAsterixData(asterix);
						return NULL;
					}
					k += 1;
					// 5 octet
					if (code[ k - 1]  & 0x01)
					{
						if (checkOverloadFunc(k, 1, cdgm_size) < 0)
						{
							freeCatAsterixData(asterix);
							return NULL;
						}
						k += 1;
					}                  // 5octet
				}                          // 4 octet
			}                                  // 3 octet
		}                                          // 2 octet
		setElementHeader(&cat->item062_080.head, CAT062_EL080, k - differ, code + differ, "Track Status");
	}
	// I062/290  news times
	if (cat->item062_fspec.code[ 1 ] & 0x02)
	{
		u_int8_t  octet1 = 0, octet2 = 0;
		u_int64_t differ = k;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		// 2 octet
		octet1 = code[k];
		k     += 1;
		// 2 octet
		if (octet1 & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			octet2 = code[k];
			k     += 1;
		}

		if (octet1 & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet1 & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet1 & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet1 & 0x10)                         // Mode S time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet1 & 0x08)                         // ADS_C time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet1 & 0x04)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet1 & 0x02)                         // VDL time
		{
			k += 1;
		}
		// 2 octet
		if (octet2 & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet2 & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet2 & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		setElementHeader(&cat->item062_290.head, CAT062_EL290, k - differ, code + differ, "System Track Update Ages");
	}
	// I062/200  moving param
	if (cat->item062_fspec.code[ 2 ] & 0x80)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_200.head, CAT062_EL200, 1, code + k, "Mode of Movement");
		k += 1;
	}
	// I062/295  adv news times
	if (cat->item062_fspec.code[ 2 ] & 0x40)
	{
		u_int8_t octets[FSPEC_SIZE_5_BYTE] =
		{
			0
		};
		u_int64_t differ = k;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		b           = 0;
		octets[ b ] = code[k];
		k          += 1;

		for (i = 1; i < FSPEC_SIZE_5_BYTE; i++)
		{
			if (octets[ b ] & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				b          += 1;
				octets[ b ] = code[k];
				k          += 1;
			}
			else
				break;
		}
		// 1 octet
		if (octets[ 0 ] & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 0 ] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 0 ] & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 0 ] & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 0 ] & 0x08)                    // Mode 4
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 0 ] & 0x04)                    // Mode 5
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 0 ] & 0x02)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		// 2 octet
		if (octets[ 1 ] & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 1 ] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 1 ] & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 1 ] & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 1 ] & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 1 ] & 0x04)                    // Comm/ACAS time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 1 ] & 0x02)                    // ADS time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		// 3 octet
		if (octets[ 2 ] & 0x80)                    // ACAS report
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 2 ] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 2 ] & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 2 ] & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 2 ] & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 2 ] & 0x04)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 2 ] & 0x02)                    // Ground speed
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		// 4 octet
		if (octets[ 3 ] & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 3 ] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 3 ] & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 3 ] & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 3 ] & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 3 ] & 0x04)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 3 ] & 0x02)                    // Mode S data
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		// 5 octet
		if (octets[ 4 ] & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 4 ] & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octets[ 4 ] & 0x20)                    // board pressure time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		setElementHeader(&cat->item062_295.head, CAT062_EL295, k - differ, code + differ, "Track Data Ages");
	}
	// I062/136  measured FL
	if (cat->item062_fspec.code[ 2 ] & 0x20)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_136.head, CAT062_EL136, 2, code + k, "Measured Flight Level");
		k += 2;
	}
	// I062/130  calc H geo
	if (cat->item062_fspec.code[ 2 ] & 0x10)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_130.head, CAT062_EL130, 2, code + k, "Calculated Track Geometric Altitude");
		k += 2;
	}
	// I062/135  calc H bar
	if (cat->item062_fspec.code[ 2 ] & 0x08)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_135.head, CAT062_EL135, 2, code + k, "Calculated Track Barometric Altitude");
		k += 2;
	}
	// I062/220  speed vh
	if (cat->item062_fspec.code[ 2 ] & 0x04)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_220.head, CAT062_EL220, 2, code + k, "Calculated Rate Of Climb or Descent");
		k += 2;
	}
	// I062/390  plan info
	if (cat->item062_fspec.code[ 2 ] & 0x02)
	{
		u_int64_t diff = k;
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item062_390 = decodeElem390PlanData(code, &k, cdgm_size);
		if (cat->item062_390)
			setElementHeader(&cat->item062_390->head, CAT062_EL390, k - diff, code + diff, "Flight Plan Related Data");
	}
	// I062/270  VO size param
	if (cat->item062_fspec.code[ 3 ] & 0x80)
	{
		u_int64_t diff = k;
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		k += 1;
		if (code[ k - 1 ] & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
			if (code[ k - 1 ] & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				k += 1;
			}
		}
		setElementHeader(&cat->item062_270.head, CAT062_EL270, k - diff, code + diff, "Target Size and Orientation");
	}
	// I062/300  Vehicle Fleet Identification
	if (cat->item062_fspec.code[ 3 ] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_300.head, CAT062_EL300, 1, code + k, "Vehicle Fleet Identification");
		k += 1;
	}
	// I062/110  Mode 5 Data reports & Extended Mode 1 Code
	if (cat->item062_fspec.code[ 3 ] & 0x20)
	{
		u_int64_t diff  = k;
		u_int8_t  octet = 0;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		octet = code[k];
		k    += 1;

		if (octet & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet & 0x40)
		{
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 4;
		}
		if (octet & 0x20)
		{
			if (checkOverloadFunc(k, 6, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 6;
		}
		if (octet & 0x10)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		if (octet & 0x08)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		if (octet & 0x04)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet & 0x02)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		setElementHeader(&cat->item062_110.head, CAT062_EL110, k - diff, code + diff, "Mode 5 Data reports and Extended Mode 1 Code");
	}
	// I062/120  Track Mode 2 Code
	if (cat->item062_fspec.code[ 3 ] & 0x10)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item062_120.head, CAT062_EL120, 2, code + k, "Track Mode 2 Code");
		k += 2;
	}
	// I062/510  composed track number( number chaine - связка номеров)
	if (cat->item062_fspec.code[ 3 ] & 0x08)
	{
		u_int64_t differ = k;
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		b    = code[k];
		k   += 1;                                  // source id code
		bb   = (code[k] << 8) | code[k + 1];       // OT number
		bb >>= 1;
		b1   = code[k + 1];
		k   += 2;
		// if(getVerboseMode(ctx))
		// fprintf(stdout,"%02X %02X %02X number chain src code=%d number=%d", code[k-3], code[k-2], code[k-1], b, bb);
		if (b1 & 0x01)
		{
			while (b1 & 0x01)
			{
				if (checkOverloadFunc(k, 3, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				b    = code[k];
				k   += 1;                            // source id code
				bb   = (code[k] << 8) | code[k + 1]; // CHT number
				bb >>= 1;
				b1   = code[k + 1];
				k   += 2;
				// if(getVerboseMode(ctx))
				// fprintf(stdout,"%02X %02X %02X number chain src code=%d number=%d", code[k-3], code[k-2], code[k-1], b, bb);
			}
		}
		setElementHeader(&cat->item062_120.head, CAT062_EL120, k - differ, code + differ, "Composed Track Number");
	}
	// I062/500  accuracy
	if (cat->item062_fspec.code[ 3 ] & 0x04)
	{
		u_int64_t diff   = k;
		u_int8_t  octet1 = 0, octet2 = 0;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		octet1 = code[k];
		k     += 1;
		if (octet1 & 0x01)
			octet2 = code[k];
		k += 1;
		// CKO on XY
		if (octet1 & 0x80)                         // Estimated Accuracy Of Track Position (Cartesian)
		{
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 4;
		}
		// CKO cov XY
		if (octet1 & 0x40)                         // XY covariance component
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		// CKO geo
		if (octet1 & 0x20)                         // Estimated Accuracy Of Track Position (WGS-84)
		{
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k                                         += 4;
			cat->item062_500.accuracy.longit_component = (bb * CMR_GEO_25) / DEG;
		}
		// CKO on H geometric
		if (octet1 & 0x10)                         // Estimated Accuracy Of Calculated Track Geometric Altitude
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		// CKO on H barometric
		if (octet1 & 0x08)                         // Estimated Accuracy Of Calculated Track Barometric Altitude
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		// CKO VXVY
		if (octet1 & 0x04)                         // Estimated Accuracy Of Track Velocity (Cartesian)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		// CKO on AXAY
		if (octet1 & 0x02)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		// CKO on VH
		if (octet2 & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		setElementHeader(&cat->item062_500.head, CAT062_EL500, k - diff, code + diff, "Estimated Accuracies");
	}
	// I062/340  last measured data
	if (cat->item062_fspec.code[ 3 ] & 0x02)
	{
		u_int64_t diff = k;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		if (code[k] > 0)
			cat->item062_340 = decode062ed340(code, &k, cdgm_size);

		if (cat->item062_340)
			setElementHeader(&cat->item062_340->head, CAT062_EL340, k - diff, code + diff, "Measured Information");
	}
	// I062/Reserv Expansion Field (RF) -
	if (cat->item062_fspec.code[ 4 ] & 0x04)
	{
		// u_int64_t diff = k;
		u_int8_t octet = 0;
		int      ind_t = 0, j = 0;                 // , zn = 1;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		b     = code[k];
		k    += 1;                                 // size RF element
		octet = code[k];
		k    += 1;                                 // RF fspec

		// bzero( &rf_info, sizeof(T_Trs_RF_Append));

		// list of src trace
		if (octet & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			ind_t = code[k];
			k    += 1;                         // rep factor
			j     = ind_t;
			for (i = 0; i < ind_t; i++)
			{
				// if(i < MAX_SRC_RF62)
				{
					if (checkOverloadFunc(k, 5, cdgm_size) < 0)
					{
						freeCatAsterixData(asterix);
						return NULL;
					}
					bb = (code[k] << 8) | code[k + 1];
					// rf_info.src_data[i].sic_sac=bb;
					k += 2;
					// b2=code[k]&0x0f;
					k += 1;
//                    switch(b2)
//                    {
//                        case 0: rf_info.src_data[i].tii |= fTII_EXTRA; break;
//                        case 1: rf_info.src_data[i].tii |= fTII_PRL; break;
//                        case 2: rf_info.src_data[i].tii |= fTII_VRL; break;
//                        case 3: rf_info.src_data[i].tii |= (fTII_PRL|fTII_VRL); break;
//                        case 4:
//                        case 5:
//                            rf_info.src_data[i].tii |= fTII_MODE_S;
//                            break;
//                        case 6:
//                        case 7:
//                            rf_info.src_data[i].tii |= (fTII_MODE_S|fTII_PRL);
//                            break;
//                        case 8: rf_info.src_data[i].tii |= fTII_ADS_B; break;
//                        case 9: rf_info.src_data[i].tii |= fTII_WAM; break;
//                    }
					bb = (code[k] << 8) | code[k + 1];
//                    rf_info.src_data[i].cht_trs_num=bb;
					k += 2;
				}
				// else
				// k+=5;
			}
		}
		// list of src koordinate
		if (octet & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			ind_t = code[k];
			k    += 1;                         // rep factor

			for (i = j; i < ind_t + j; i++)
			{
//                if(i < MAX_SRC_RF62)
				{
					if (checkOverloadFunc(k, 3, cdgm_size) < 0)
					{
						freeCatAsterixData(asterix);
						return NULL;
					}
					bb = (code[k] << 8) | code[k + 1];
//                    rf_info.src_data[i].sic_sac=bb;
					k += 2;
					// b2=code[k]&0x0f;
					k += 1;
//                    switch(b2)
//                    {
//                        case 0: rf_info.src_data[i].tii |= fTII_EXTRA; break;
//                        case 1: rf_info.src_data[i].tii |= fTII_PRL; break;
//                        case 2: rf_info.src_data[i].tii |= fTII_VRL; break;
//                        case 3: rf_info.src_data[i].tii |= (fTII_PRL|fTII_VRL); break;
//                        case 4:
//                        case 5:
//                            rf_info.src_data[i].tii |= fTII_MODE_S;
//                            break;
//                        case 6:
//                        case 7:
//                            rf_info.src_data[i].tii |= (fTII_MODE_S|fTII_PRL);
//                            break;
//                        case 8: rf_info.src_data[i].tii |= fTII_ADS_B; break;
//                        case 9: rf_info.src_data[i].tii |= fTII_WAM; break;
//                    }
				}
//                else
//                    k+=3;
			}
		}
		// speed of src trace
		if (octet & 0x20)
		{
			// float ff;
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			for (i = 0; i < 2; i += 1)
			{
				bb = (code[k] << 8) | code[k + 1];
				if (code[k] & 0x80)
				{
					bb = ~(bb - 1);
//                    zn = -1;
				}
//                else
//                {
//                    zn = 1;
//                }
				// ff = zn*bb*CMR_VXVY;
//                if(i==0)
//                    rf_info.VX_gi = ff;
//                else
//                    rf_info.VY_gi = ff;
				k += 2;
			}
		}
		// arm flag
		if (octet & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			// b2=code[k];
			k += 1;
//            if(!(b2&0x80))
//                rf_info.arm|=fARM_FLAG_IN_PLAN;
		}
		// setElementHeader(&cat->item062_rf->head, CAT062_rf, k - diff, code + diff, "Reserved Expansion Field");
	}
	// I062/SP
	if (cat->item062_fspec.code[ 4 ] & 0x02)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		if (code[k] > 0)                           // Есть данные поля SP
		{
			cat->item062_sp = malloc(sizeof(Cat062specialPuposeField));

			if (cat->item062_sp)
			{
				bzero(cat->item062_sp, sizeof(Cat062specialPuposeField));
				setElementHeader(&cat->item062_sp->head, CAT062_ELSP, code[k], code + k, "I062 Special Purpose Field");
			}
		}
	}

	return asterix;
}

/**
 * @brief Запись данных измерения параметров траектории трассы ВО в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param add_data - указатель на структуру данных полученных с борта ВО по каналу АЗН
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set062TrackAircraftDerivedData(TCat062* cat, Cat062AircraftDerivedData* add_data)
{
	int res = -1, size = 0;


	if ((cat == NULL) || (add_data == NULL))
		return res;

	if ((cat) && (add_data))
	{
		if (cat->item062_380 == NULL)
		{
			cat->item062_380 = malloc(sizeof(AircraftDerivedData));
			if (cat->item062_380)
				bzero(cat->item062_380, sizeof(AircraftDerivedData));
		}
		if (cat->item062_380)
		{
			res                                           = 0;
			cat->item062_380->acas_56_bit_msg             = add_data->acas_56_bit_msg;
			cat->item062_380->acas_operational            = add_data->acas_operational;
			cat->item062_380->aircraft_ident_capability   = add_data->aircraft_ident_capability;
			cat->item062_380->altitude_report_capability  = add_data->altitude_report_capability;
			cat->item062_380->barometric_pressure_setting = add_data->barometric_pressure_setting;
			cat->item062_380->bits_b1b                    = add_data->bits_b1b;
			cat->item062_380->bit_b1a                     = add_data->bit_b1a;
			cat->item062_380->communication_capability    = add_data->communication_capability;
			cat->item062_380->differencial_correction     = add_data->differencial_correction;
			cat->item062_380->emitter_category            = add_data->emitter_category;
			cat->item062_380->final_selected_altitude     = add_data->final_selected_altitude;
			cat->item062_380->flight_status               = add_data->flight_status;
			cat->item062_380->flight_status_emergency     = add_data->flight_status_emergency;
			cat->item062_380->fsa_altitude_hold           = add_data->fsa_altitude_hold;
			cat->item062_380->fsa_approach_mode           = add_data->fsa_approach_mode;
			cat->item062_380->fsa_manage_vertical_mode    = add_data->fsa_manage_vertical_mode;
			cat->item062_380->geometric_altitude          = add_data->geometric_altitude;
			cat->item062_380->ground_bit                  = add_data->ground_bit;
			cat->item062_380->ground_speed                = add_data->ground_speed;
			cat->item062_380->indicated_airspeed          = add_data->indicated_airspeed;
			cat->item062_380->latitude_wgs84              = add_data->latitude_wgs84;
			cat->item062_380->longitude_wgs84             = add_data->longitude_wgs84;
			cat->item062_380->mach_indicator              = add_data->mach_indicator;
			cat->item062_380->mach_number                 = add_data->mach_number;
			cat->item062_380->magnetic_heading            = add_data->magnetic_heading;
			// очистка старых данных
			if (cat->item062_380->mode_s_msg)
			{
				free(cat->item062_380->mode_s_msg);
				cat->item062_380->mode_s_data_count = 0;
				cat->item062_380->mode_s_msg        = NULL;
			}
			// запись новых
			if (add_data->mode_s_msg)
			{
				if ((add_data->mode_s_msg) && (add_data->mode_s_data_count > 0))
				{
					size                         = add_data->mode_s_data_count * sizeof(u_int64_t);
					cat->item062_380->mode_s_msg = malloc(size);
					if (cat->item062_380->mode_s_msg)
					{
						bzero(cat->item062_380->mode_s_msg, size);
						cat->item062_380->mode_s_data_count = add_data->mode_s_data_count;
						memcpy(cat->item062_380->mode_s_msg, add_data->mode_s_msg, size);
					}
				}
			}
			cat->item062_380->multiple_navigational = add_data->multiple_navigational;
			cat->item062_380->position_uncertainty  = add_data->position_uncertainty;
			cat->item062_380->roll_angle            = add_data->roll_angle;
			cat->item062_380->route_point_count     = add_data->route_point_count;
			// очистка старых данных
			if (cat->item062_380->route_point)
			{
				free(cat->item062_380->route_point);
				cat->item062_380->route_point_count = 0;
				cat->item062_380->route_point       = NULL;
			}
			if (add_data->route_point)
			{
				if ((add_data->route_point) && (add_data->route_point_count > 0))
				{
					size                          = add_data->route_point_count * sizeof(TAdsRoutePoint);
					cat->item062_380->route_point = malloc(size);
					if (cat->item062_380->route_point)
					{
						int i;
						bzero(cat->item062_380->route_point, size);
						cat->item062_380->route_point_count = add_data->route_point_count;
						for (i = 0; i < cat->item062_380->route_point_count; i++)
						{
							cat->item062_380->route_point[i].compliance        = add_data->route_point[i].rp_compliance;
							cat->item062_380->route_point[i].point_altitude    = add_data->route_point[i].rp_altitude;
							cat->item062_380->route_point[i].point_latitude    = add_data->route_point[i].rp_latitude;
							cat->item062_380->route_point[i].point_longitude   = add_data->route_point[i].rp_longitude;
							cat->item062_380->route_point[i].point_num         = add_data->route_point[i].rp_num;
							cat->item062_380->route_point[i].point_time        = add_data->route_point[i].rp_time;
							cat->item062_380->route_point[i].point_turn_radius = add_data->route_point[i].rp_turn_radius;
							cat->item062_380->route_point[i].point_turn_sign   = add_data->route_point[i].rp_turn_sign;
							cat->item062_380->route_point[i].point_type        = add_data->route_point[i].rp_type;
						}
					}
				}
			}
			cat->item062_380->selected_altitude       = add_data->selected_altitude;
			cat->item062_380->selected_altitude_src   = add_data->selected_altitude_src;
			cat->item062_380->selected_altitude_valid = add_data->selected_altitude_valid;
			cat->item062_380->specific_service        = add_data->specific_service;
			cat->item062_380->target_address          = add_data->target_address;
			strcpy(cat->item062_380->target_identification, add_data->target_identification);
			cat->item062_380->temperature          = add_data->temperature;
			cat->item062_380->temperature_valid    = add_data->temperature_valid;
			cat->item062_380->track_angle          = add_data->track_angle;
			cat->item062_380->trajectory_available = add_data->trajectory_available;
			cat->item062_380->trajectory_valid     = add_data->trajectory_valid;
			cat->item062_380->true_airspeed        = add_data->true_airspeed;
			cat->item062_380->turbulence           = add_data->turbulence;
			cat->item062_380->turbulence_valid     = add_data->turbulence_valid;
			cat->item062_380->turn_indicator       = add_data->turn_indicator;
			cat->item062_380->turn_rate            = add_data->turn_rate;
			cat->item062_380->velocity_uncertainty = add_data->velocity_uncertainty;
			cat->item062_380->vh_barometric        = add_data->vh_barometric;
			cat->item062_380->vh_geometric         = add_data->vh_geometric;
			cat->item062_380->wind_direct          = add_data->wind_direct;
			cat->item062_380->wind_direct_valid    = add_data->wind_direct_valid;
			cat->item062_380->wind_speed           = add_data->wind_speed;
			cat->item062_380->wind_speed_valid     = add_data->wind_speed_valid;

			if (cat->item062_fspec.code)
				cat->item062_fspec.code[1] |= 0x10;

		}
	}
	return res;
}

/**
 * @brief Выборка бортовых данных трассы ВО, полученных
 * @param cat_lib - указатель на структуру категории 062
 * @return - указатель на структуру данных полученных с борта ВО по каналу АЗН
 *           NULL - данные не получены
 */
Cat062AircraftDerivedData* get062TrackAircraftDerivedData(TCat062* cat)
{
	Cat062AircraftDerivedData* res  = NULL;
	int                        size = 0;


	if (cat == NULL)
		return res;

	if (cat->item062_380 == NULL)
		return res;

	res = (Cat062AircraftDerivedData*) malloc(sizeof(Cat062AircraftDerivedData));
	if (res)
	{
		bzero(res, sizeof(Cat062AircraftDerivedData));
		res->acas_56_bit_msg             = cat->item062_380->acas_56_bit_msg;
		res->acas_operational            = cat->item062_380->acas_operational;
		res->aircraft_ident_capability   = cat->item062_380->aircraft_ident_capability;
		res->altitude_report_capability  = cat->item062_380->altitude_report_capability;
		res->barometric_pressure_setting = cat->item062_380->barometric_pressure_setting;
		res->bits_b1b                    = cat->item062_380->bits_b1b;
		res->bit_b1a                     = cat->item062_380->bit_b1a;
		res->communication_capability    = cat->item062_380->communication_capability;
		res->differencial_correction     = cat->item062_380->differencial_correction;
		res->emitter_category            = cat->item062_380->emitter_category;
		res->final_selected_altitude     = cat->item062_380->final_selected_altitude;
		res->flight_status               = cat->item062_380->flight_status;
		res->flight_status_emergency     = cat->item062_380->flight_status_emergency;
		res->fsa_altitude_hold           = cat->item062_380->fsa_altitude_hold;
		res->fsa_approach_mode           = cat->item062_380->fsa_approach_mode;
		res->fsa_manage_vertical_mode    = cat->item062_380->fsa_manage_vertical_mode;
		res->geometric_altitude          = cat->item062_380->geometric_altitude;
		res->ground_bit                  = cat->item062_380->ground_bit;
		res->ground_speed                = cat->item062_380->ground_speed;
		res->indicated_airspeed          = cat->item062_380->indicated_airspeed;
		res->latitude_wgs84              = cat->item062_380->latitude_wgs84;
		res->longitude_wgs84             = cat->item062_380->longitude_wgs84;
		res->mach_indicator              = cat->item062_380->mach_indicator;
		res->mach_number                 = cat->item062_380->mach_number;
		res->magnetic_heading            = cat->item062_380->magnetic_heading;

		if ((cat->item062_380->mode_s_msg) && (cat->item062_380->mode_s_data_count > 0))
		{
			size            = cat->item062_380->mode_s_data_count * sizeof(u_int64_t);
			res->mode_s_msg = malloc(size);
			if (res->mode_s_msg)
			{
				bzero(res->mode_s_msg, size);
				res->mode_s_data_count = cat->item062_380->mode_s_data_count;
				memcpy(res->mode_s_msg, cat->item062_380->mode_s_msg, size);
			}
		}

		res->multiple_navigational = cat->item062_380->multiple_navigational;
		res->position_uncertainty  = cat->item062_380->position_uncertainty;
		res->roll_angle            = cat->item062_380->roll_angle;
		res->route_point_count     = cat->item062_380->route_point_count;

		if ((cat->item062_380->route_point) && (cat->item062_380->route_point_count > 0))
		{
			size             = cat->item062_380->route_point_count * sizeof(TAdsRoutePoint);
			res->route_point = malloc(size);
			if (res->route_point)
			{
				int i;
				bzero(res->route_point, size);
				res->route_point_count = cat->item062_380->route_point_count;
				for (i = 0; i < res->route_point_count; i++)
				{
					res->route_point[i].rp_compliance  = cat->item062_380->route_point[i].compliance;
					res->route_point[i].rp_altitude    = cat->item062_380->route_point[i].point_altitude;
					res->route_point[i].rp_latitude    = cat->item062_380->route_point[i].point_latitude;
					res->route_point[i].rp_longitude   = cat->item062_380->route_point[i].point_longitude;
					res->route_point[i].rp_num         = cat->item062_380->route_point[i].point_num;
					res->route_point[i].rp_time        = cat->item062_380->route_point[i].point_time;
					res->route_point[i].rp_turn_radius = cat->item062_380->route_point[i].point_turn_radius;
					res->route_point[i].rp_turn_sign   = cat->item062_380->route_point[i].point_turn_sign;
					res->route_point[i].rp_type        = cat->item062_380->route_point[i].point_type;
				}
			}
		}
		res->selected_altitude       = cat->item062_380->selected_altitude;
		res->selected_altitude_src   = cat->item062_380->selected_altitude_src;
		res->selected_altitude_valid = cat->item062_380->selected_altitude_valid;
		res->specific_service        = cat->item062_380->specific_service;
		res->target_address          = cat->item062_380->target_address;
		strcpy(res->target_identification, cat->item062_380->target_identification);
		res->temperature          = cat->item062_380->temperature;
		res->temperature_valid    = cat->item062_380->temperature_valid;
		res->track_angle          = cat->item062_380->track_angle;
		res->trajectory_available = cat->item062_380->trajectory_available;
		res->trajectory_valid     = cat->item062_380->trajectory_valid;
		res->true_airspeed        = cat->item062_380->true_airspeed;
		res->turbulence           = cat->item062_380->turbulence;
		res->turbulence_valid     = cat->item062_380->turbulence_valid;
		res->turn_indicator       = cat->item062_380->turn_indicator;
		res->turn_rate            = cat->item062_380->turn_rate;
		res->velocity_uncertainty = cat->item062_380->velocity_uncertainty;
		res->vh_barometric        = cat->item062_380->vh_barometric;
		res->vh_geometric         = cat->item062_380->vh_geometric;
		res->wind_direct          = cat->item062_380->wind_direct;
		res->wind_direct_valid    = cat->item062_380->wind_direct_valid;
		res->wind_speed           = cat->item062_380->wind_speed;
		res->wind_speed_valid     = cat->item062_380->wind_speed_valid;
	}
	return res;
}

/**
 * @brief Освобождение памяти от структуры с бортовыми данными трассы ВО, полученными
 *        по каналу АЗН
 * @param add - указатель на структуру с бортовыми данными трассы ВО
 * @return - 0 - успешное выполнение, -1 - отказ
 */
int free062TrackAircraftDerivedData(Cat062AircraftDerivedData* add)
{
	int ret = -1;

	if (add == NULL)
		return ret;

	if (add)
	{
		if (add->mode_s_msg)
			free(add->mode_s_msg);
		if (add->route_point)
			free(add->route_point);
		free(add);
		ret = 0;
	}
	return ret;
}
