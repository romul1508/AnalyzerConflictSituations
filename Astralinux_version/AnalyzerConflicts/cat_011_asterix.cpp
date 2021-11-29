#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
    //linux code goes here
    #include <unistd.h>
#elif _WIN32
    // windows code goes here
    #include "unistd.h"
#else
// #error "OS not supported!"
#endif

#include <string.h>
#include <time.h>

#include "cat_asterix.h"
#include "cat_011_asterix.h"

//extern const double M_PI;

/**
 * @brief Установка значений по умолчанию для параметров структуры
 *        категории 011 Астерикс
 * @param cat - указатель на структуру категории
 * @return 0 - успешное выполнение, -1 отказ исполнения
 */
int set011catDefaultData(TCat011* cat)
{
	if (cat == NULL)
		return -1;

	cat->item011_010.sac_sic               = DEFAULT_U_INT16_CODE;
	cat->item011_015.sevice_id             = DEFAULT_U_INT8_CODE;
	cat->item011_161.track_number          = DEFAULT_U_INT16_CODE;
	cat->item011_060.m3a.board_number_code = DEFAULT_RBS_UVD_NUM_CODE;
	cat->item011_060.m3a.change            = DEFAULT_U_INT8_CODE;
	cat->item011_140.time_of_track         = DEFAULT_TIME_OF_TRACK;
	cat->item011_042.x                     = DEFAULT_CARTESIAN_POS;
	cat->item011_042.y                     = DEFAULT_CARTESIAN_POS;
	cat->item011_041.latitude              = DEFAULT_RADIAN_GEO_POS;
	cat->item011_041.longitude             = DEFAULT_RADIAN_GEO_POS;
	cat->item011_092.calc_g_height         = DEFAULT_ALTITUDE;
	cat->item011_093.calc_b_height         = DEFAULT_ALTITUDE;
	cat->item011_090.measured_fligth_level = DEFAULT_ALTITUDE;
	cat->item011_202.vx                    = DEFAULT_VELOCITY;
	cat->item011_202.vy                    = DEFAULT_VELOCITY;
	cat->item011_210.ax                    = DEFAULT_ACCELERATION;
	cat->item011_210.ay                    = DEFAULT_ACCELERATION;
	cat->item011_215.vh                    = DEFAULT_VELOCITY;
	cat->item011_270.length                = DEFAULT_U_INT8_CODE;
	cat->item011_270.width                 = DEFAULT_U_INT8_CODE;
	cat->item011_270.orientation           = DEFAULT_RADIAN_AZIMUTH_POS;
	cat->item011_430.phase_fligth          = DEFAULT_U_INT8_CODE;

	return 0;

}

/**
 * @brief Выдача указателя на структуру категории 011 из
 *        обобщенной структуры Астерикс
 * @param cat - указатель на структуру Астерикс
 * @return указатель на структуру категории 011 Астерикс,
 *         при отказе значение NULL
 */
TCat011* get011category(TCatAsterix* cat)
{
	TCat011* ret = NULL;

	if (cat == NULL)
		return ret;

	if (cat)
	{
		if (cat->cat_num == CODT_CAT_011)
			ret = &cat->category.c011;
	}

	return ret;
}

void freeCat11adsData(TCat011AdsBRelatedData* cat11_ads)
{
	if (cat11_ads == NULL)
		return;
	if (cat11_ads)
	{
		if (cat11_ads->mode_s_msg)
			free(cat11_ads->mode_s_msg);
		free(cat11_ads);
	}

	return;
}

/**
 * @brief Распаковка данных канала АЗН-В передающихся в составе категории 011
 * @param code - указатель массив кодограммы
 * @param cnt - номер начального байта данных АЗН-В
 * @param code_size - размер массива кодограммы
 * @return - указатель на структуру с распакованными данными
 */
Cat011ed380AdsBRelatedData* decode011adsData(unsigned char* code, u_int64_t* cnt, u_int64_t code_size)
{
	u_int8_t ads_spec[FSPEC_SIZE_5_BYTE] =
	{
		0
	}, fspec_size;
	u_int64_t                   k     = *cnt;
	Cat011ed380AdsBRelatedData* ads_b = NULL;

	if (code == NULL)
		return ads_b;

	ads_b = (Cat011ed380AdsBRelatedData*)malloc(sizeof(Cat011ed380AdsBRelatedData));

	if (ads_b == NULL)
		return ads_b;

	// bzero(ads_b, sizeof(Cat011ed380AdsBRelatedData));
	memset(ads_b, 0, sizeof(Cat011ed380AdsBRelatedData));

	if (checkOverloadFunc(k, 2, code_size) < 0)
	{
		free(ads_b);
		return NULL;
	}

	fspec_size = getCategoryFspecSize(code, k, code_size);

	if ((fspec_size > 0) && (fspec_size <= FSPEC_SIZE_2_BYTE))
	{
		memcpy(ads_spec, code + k, fspec_size);
		k += fspec_size;
	}
	else
	{
		fprintf(stderr, "proc[pid-%d] decodeCodogramm cat 011 in decode 011/380 spec ERROR\n", getpid());
		free(ads_b);
		return NULL;
	}
	// Mode S Data
	if (ads_spec[ 0 ] & 0x80)
	{
		int i, j;
		// rep factor
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			free(ads_b);
			return NULL;
		}

		ads_b->mode_s_data_count = code[k];
		k                       += 1;

		if (checkOverloadFunc(k, ads_b->mode_s_data_count * sizeof(u_int64_t), code_size) < 0)
		{
			free(ads_b);
			return NULL;
		}

		if (ads_b->mode_s_data_count > 0)
			ads_b->mode_s_msg = (u_int64_t*)malloc(ads_b->mode_s_data_count * sizeof(u_int64_t));
		if (ads_b->mode_s_msg)
		{
			// bzero(ads_b->mode_s_msg, ads_b->mode_s_data_count * sizeof(u_int64_t));
			memset(ads_b->mode_s_msg, 0, ads_b->mode_s_data_count * sizeof(u_int64_t));

			for (i = 0; i < ads_b->mode_s_data_count; i++)
			{
				for (j = 0; j < 8; j++)
				{
					ads_b->mode_s_msg[i] |= (code[ k ] << (8 * (7 - j)));
					k                    += 1;
				}
			}
		}
		else
		{
			k                       += (ads_b->mode_s_data_count * 8);
			ads_b->mode_s_data_count = 0;
		}
	}
	// Adress IKAO VO
	if (ads_spec[ 0 ] & 0x40)
	{
		if (checkOverloadFunc(k, 3, code_size) < 0)
		{
			if (ads_b->mode_s_msg)
				free(ads_b->mode_s_msg);
			free(ads_b);
			return NULL;
		}
		ads_b->target_address = (code[k] << 16) | (code[k + 1] << 8) | code[k + 2];
		k                    += 3;
	}
	// Communication/ACAS
	if (ads_spec[ 0 ] & 0x10)
	{
		if (checkOverloadFunc(k, 3, code_size) < 0)
		{
			if (ads_b->mode_s_msg)
				free(ads_b->mode_s_msg);
			free(ads_b);
			return NULL;
		}

		ads_b->communication_capability   = code[k] >> 5;
		ads_b->flight_status              = (code[k] & 0x1C) >> 2;
		k                                += 1;
		ads_b->specific_service           = !!(code[ k ] & 0x80);
		ads_b->altitude_report_capability = !!(code[ k ] & 0x40);
		ads_b->aircraft_ident_capability  = !!(code[ k ] & 0x20);
		ads_b->bit_b1a                    = !!(code[ k ] & 0x10);
		ads_b->bits_b1b                   = code[ k ] & 0x0f;
		k                                += 1;
		ads_b->acas_operational           = !!(code[ k ] & 0x80);
		ads_b->multiple_navigational      = !!(code[ k ] & 0x40);
		ads_b->differencial_correction    = !!(code[ k ] & 0x20);
		k                                += 1;

	}

	// Aircraft Type
	if (ads_spec[ 1 ] & 0x80)
	{
		if (checkOverloadFunc(k, 4, code_size) < 0)
		{
			if (ads_b->mode_s_msg)
				free(ads_b->mode_s_msg);
			free(ads_b);
			return NULL;
		}
		memcpy(ads_b->aircraft_type, code + k, 4);
		k += 4;
	}
	// Emitter Category
	if (ads_spec[ 1 ] & 0x40)
	{
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			if (ads_b->mode_s_msg)
				free(ads_b->mode_s_msg);
			free(ads_b);
			return NULL;
		}
		ads_b->emitter_category = code[k];
		k                      += 1;
	}
	// Available Technologies
	if (ads_spec[ 1 ] & 0x10)
	{
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			if (ads_b->mode_s_msg)
				free(ads_b->mode_s_msg);
			free(ads_b);
			return NULL;
		}

		// VDL Mode
		ads_b->vdl_available = !!(code[ k ] & 0x80);
		// Mode S
		ads_b->mode_s_available = !!(code[ k ] & 0x40);
		// UAT Mode
		ads_b->uat_available = !!(code[ k ] & 0x20);
		k                   += 1;
	}

	*cnt = k;

	return ads_b;
}

/**
 * @brief Распаковка поля SP категории 011 (по умолчанию)
 * @param code - указатель на массив с кодограммой
 * @param cnt - номер начального байта кодированных данных в кодограмме
 * @return - указатель на структуру с распакованными данными
 */
TSpData* decode011defaultSPdata(unsigned char* code, u_int64_t* cnt, u_int64_t code_size)
{
	int           zn       = 1;
	u_int64_t     k        = *cnt;
	unsigned char b        = 0;
	unsigned short bb = 0;
	TSpData*       sp = NULL;

	if (code == NULL)
		return sp;

	sp = (TSpData*)malloc(sizeof(TSpData));

	if (sp)
		// bzero(sp, sizeof(TSpData));
		memset(sp, 0, sizeof(TSpData));
	else
		return sp;

	if (checkOverloadFunc(k, 1, code_size) < 0)
	{
		free(sp);
		return NULL;
	}

	b = code[k];

	if (checkOverloadFunc(k, b, code_size) < 0)
	{
		free(sp);
		return NULL;
	}
	*cnt = k + b;
	return sp;
}

/**
 * @brief Распаковка записи категории 011(ed.1.12 May 2008) формата АСТЕРИКС
 * @param ptr - указатель на данные с кодограммой АСТЕРИКС
 * @return - указатель на структуру категории Астерикс
 */
TCatAsterix* decode011catRecord(TCommonObmenData* codogramm, size_t* start_byte_pos)
{
	u_int8_t       fspec_size, fspec_pos = 0;
	unsigned char* code    = NULL;
	u_int64_t      k       = *start_byte_pos, cdgm_size = 0;
	TCatAsterix*   asterix = NULL;
	TCat011*       cat     = NULL;



	if (codogramm == NULL)
	{
		fprintf(stderr, "proc[pid-%d] decodeCodogramm cat 011 data pointer ERROR\n", getpid());
		return asterix;
	}

	cdgm_size = getObmenDataLenght(codogramm);

	if (cdgm_size == 0)
	{
		fprintf(stderr, "proc[pid-%d] decodeCodogramm cat 011 null size data ERROR\n", getpid());
		return asterix;
	}

	if (checkOverloadFunc(k, cdgm_size, cdgm_size) < 0)
	{
		return asterix;
	}

	code = (unsigned char*) getObmenData(codogramm);
	if (code == NULL)
	{
		fprintf(stderr, "proc[pid-%d] decodeCodogramm cat 011 data array ERROR\n", getpid());
		return asterix;
	}

	asterix = createCatAsterixData(CODT_CAT_011);


	if (asterix)
	{
		cat = get011category(asterix);

		set011catDefaultData(cat);

	}
	else
	{
		fprintf(stderr, "proc[pid-%d] decodeCodogramm cat 011 create category ERROR\n", getpid());
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
		if (cat->item011_fspec.code)
		{
			memset(cat->item011_fspec.code, 0, FSPEC_SIZE_5_BYTE);
			memcpy(cat->item011_fspec.code, code + k, fspec_size);
			cat->item011_fspec.code_size = fspec_size;
		}
		k += fspec_size;
	}
	else
	{
		freeCatAsterixData(asterix);
		fprintf(stderr, "proc[pid-%d] decode011catRecord FSPEC ERROR\n", getpid());
		return NULL;
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x80)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item011_010.sac_sic = (code[ k ] << 8) | code[ k + 1 ];
		k                       += 2;
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		switch (code[ k ])
		{
		case 1: cat->item011_000.message_type = MT_TRACK_AND_FLIGHT_PLAN;
			break;
		case 2: cat->item011_000.message_type = MT_MANUAL_ATTACH_FLIGHT_PLAN;
			break;
		case 3: cat->item011_000.message_type = MT_MANUAL_DETACHMENT_FLIGHT_PLAN;
			break;
		case 4: cat->item011_000.message_type = MT_INSERT_FLIGHT_PLAN;
			break;
		case 5: cat->item011_000.message_type = MT_SUPPRESSION_FLIGHT_PLAN;
			break;
		case 6: cat->item011_000.message_type = MT_MODIFICATION_FLIGHT_PLAN;
			break;
		case 7: cat->item011_000.message_type = MT_HOLDBAR_STATUS;
			break;
		}
		k++;
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x20)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item011_015.sevice_id = code[ k ];
		k++;
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x10)
	{
		int ccc;
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		ccc                            = (code[ k ] << 16) | (code[ k + 1 ] << 8) | code[ k + 2 ];
		cat->item011_140.time_of_track = CMR_ASTX_TIME * ccc; // seconds
		k                             += 3;
	}
	if (cat->item011_fspec.code[ fspec_pos ] & 0x08)
	{
		double ddd = 0.0;
		int    zn  = 1, i, ccc;

		if (checkOverloadFunc(k, 8, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		for (i = 0; i < 2; i += 1)
		{
			ccc = (code[k] << 24) | (code[k + 1] << 16) | (code[k + 2] << 8) | code[k + 3];
			if (code[k] & 0x80)
			{
				zn  = -1;
				ccc = ~(ccc - 1);
			}
			else
				zn = 1;

			ddd = (zn * ccc * CMR_GEO_31) / DEG;

			if (i == 0)
				cat->item011_041.latitude = ddd;
			else
				cat->item011_041.longitude = ddd;

			k += 4;
		}
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x04)
	{
		u_int16_t bb;
		int       zn = 1;

		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		bb = (code[ k ] << 8) | code[ k + 1 ];
		if (code[ k ] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		else
			zn = 1;

		cat->item011_042.x = zn * bb;              // meters

		bb = (code[ k + 2 ] << 8) | code[ k + 3 ];
		if (code[ k + 2 ] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		else
			zn = 1;

		cat->item011_042.y = zn * bb;              // meters
		k                 += 4;

	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x02)
	{
		u_int16_t bb;
		int       zn = 1;

		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		bb = (code[ k ] << 8) | code[ k + 1 ];
		if (code[ k ] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		else
			zn = 1;

		cat->item011_202.vx = zn * CMR_VXVY * bb;  // m/sec

		bb = (code[ k + 2 ] << 8) | code[ k + 3 ];
		if (code[ k + 2 ] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		else
			zn = 1;

		cat->item011_202.vy = zn * CMR_VXVY * bb;  // m/sec

		k += 4;
	}

	// 2 octet
	fspec_pos = 1;
	if (cat->item011_fspec.code[ fspec_pos ] & 0x80)
	{
		u_int8_t b;
		int      zn = 1;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		b = code[ k ];
		if (code[ k ] & 0x80)
		{
			b  = ~(b - 1);
			zn = -1;
		}
		else
			zn = 1;

		cat->item011_210.ax = zn * CMR_VXVY * b;   // m/sec^2

		b = code[ k + 1 ];
		if (code[ k + 1 ] & 0x80)
		{
			b  = ~(b - 1);
			zn = -1;
		}
		else
			zn = 1;

		cat->item011_210.ay = zn * CMR_VXVY * b;   // m/sec^2

		k += 2;
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x40)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item011_060.m3a.board_number_code = (code[ k ] << 8) | code[ k + 1 ];
		k                                     += 2;
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x20)
	{
		u_int64_t cnt_pos;

		if (checkOverloadFunc(k, 7, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		cat->item011_245.target_id_src = code[k] >> 6;
		k                             += 1;
		cnt_pos                        = k;
		convertIA5codeToAscii((unsigned char*) cat->item011_245.target_id, code, &cnt_pos);
		k = cnt_pos;
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x10)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item011_380 = decode011adsData(code, &k, cdgm_size);
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x08)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item011_161.track_number = ((code[ k ] & 0x7f) << 8) | code[ k + 1 ];
		k                            += 2;
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x04)
	{
		// 1 octet
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item011_170.track_status.monosensor           = !!(code[k] & 0x80);
		cat->item011_170.track_status.ground_bit_set       = !!(code[k] & 0x40);
		cat->item011_170.track_status.most_reliable_height = !!(code[k] & 0x20);
		// H tii
		switch ((code[k] & 0x1C) >> 2)
		{
		case 0: cat->item011_170.track_status.geometric_altitude_source = AS_UNKNOWN;
			break;
		case 1: cat->item011_170.track_status.geometric_altitude_source = AS_GPS;
			break;
		case 2: cat->item011_170.track_status.geometric_altitude_source = AS_3D_RADAR;
			break;
		case 3: cat->item011_170.track_status.geometric_altitude_source = AS_TRIANG;
			break;
		case 4: cat->item011_170.track_status.geometric_altitude_source = AS_COVERAGE;
			break;
		case 5: cat->item011_170.track_status.geometric_altitude_source = AS_TABLE;
			break;
		case 6: cat->item011_170.track_status.geometric_altitude_source = AS_FICTION;
			break;
		case 7: cat->item011_170.track_status.geometric_altitude_source = AS_MLAT;
			break;
		}
		cat->item011_170.track_status.confirm = !!(code[k] & 0x02);
		k                                    += 1;
		// 2 octet
		if (code[ k - 1 ] & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_170.track_status.simulate          = !!(code[k] & 0x80);
			cat->item011_170.track_status.last_message      = !!(code[k] & 0x40);
			cat->item011_170.track_status.first_message     = !!(code[k] & 0x20);
			cat->item011_170.track_status.mode4_interrogate = (code[k] & 0x18) >> 3;
			cat->item011_170.track_status.military_emrg     = !!(code[k] & 0x04);
			cat->item011_170.track_status.military_id       = !!(code[k] & 0x02);
			k                                              += 1;
			// 3 octet
			if (code[ k - 1 ] & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				cat->item011_170.track_status.amalgamation   = !!(code[k] & 0x80);
				cat->item011_170.track_status.spi_sign       = !!(code[k] & 0x40);
				cat->item011_170.track_status.coast_track    = !!(code[k] & 0x20);
				cat->item011_170.track_status.flight_in_plan = !!(code[k] & 0x10);
				cat->item011_170.track_status.ads_inconsist  = !!(code[k] & 0x08);
				k                                           += 1;
				// 4 octet
				if (code[ k - 1 ] & 0x01)
				{
					if (checkOverloadFunc(k, 1, cdgm_size) < 0)
					{
						freeCatAsterixData(asterix);
						return NULL;
					}
					cat->item011_170.track_status.psr_coast    = !!(code[k] & 0x40);
					cat->item011_170.track_status.ssr_coast    = !!(code[k] & 0x20);
					cat->item011_170.track_status.mode_s_coast = !!(code[k] & 0x10);
					cat->item011_170.track_status.ads_coast    = !!(code[k] & 0x08);
					cat->item011_170.track_status.suc_set      = !!(code[k] & 0x04);
					cat->item011_170.track_status.aac_conflict = !!(code[k] & 0x02);
					k                                         += 1;
				}                          // 4 octet
			}                                  // 3 octet
		}                                          // 2 octet
	}

	// I062/290  news times
	if (cat->item011_fspec.code[ fspec_pos ] & 0x02)
	{
		u_int8_t octet1 = 0, octet2 = 0;

		// 1 octet
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
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
			cat->item011_290.psr_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.ssr_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.mda_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.mfl_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.mds_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet1 & 0x04)                         // ADS_C time
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.ads_age = decodeAgeTime((code[ k ] << 8) | code[ k + 1]);
			k                       += 2;
		}
		if (octet1 & 0x02)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.ads_es_age = decodeAgeTime(code[k]);
			k                          += 1;
		}
		// 2 octet
		if (octet2 & 0x80)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.md1_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet2 & 0x40)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.md2_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet2 & 0x20)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.loop_age = decodeAgeTime(code[k]);
			k                        += 1;
		}
		if (octet1 & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.trk_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
		if (octet2 & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_290.mlt_age = decodeAgeTime(code[k]);
			k                       += 1;
		}
	}

	// 3 octet
	fspec_pos = 2;
	if (cat->item011_fspec.code[ fspec_pos ] & 0x80)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item011_430.phase_fligth = code[k];
		k                            += 1;
	}
	// I011/090  measured FL
	if (cat->item011_fspec.code[ fspec_pos ] & 0x40)
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
			zn = -1;
			bb = ~(bb - 1);
		}
		cat->item011_090.measured_fligth_level = (double) (zn * bb * CMR_ASTX_H_25F); // m
		k                                     += 2;
	}
	// I011/093  calc H bar
	if (cat->item011_fspec.code[ fspec_pos ] & 0x20)
	{
		int       zn = 1;
		u_int16_t bb;
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		bb = ((code[k] & 0x7F) << 8) | code[k + 1];
		if (code[k] & 0x80)
			cat->item011_093.qnh_correction = 1;

		if (code[k] & 0x40)
		{
			bb |= 0x8000;
			zn  = -1;
			bb  = ~(bb - 1);
		}
		cat->item011_093.calc_b_height = (double) (zn * bb * CMR_ASTX_H_25F); // m
		k                             += 2;
	}
	// I011/092  calc H geo
	if (cat->item011_fspec.code[ fspec_pos ] & 0x10)
	{
		u_int16_t bb;
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		bb                             = (code[k] << 8) | code[k + 1];
		cat->item011_092.calc_g_height = (double) (bb * CMR_ASTX_H_6_25F);    // m
		k                             += 2;
	}
	// I011/215  speed vh
	if (cat->item011_fspec.code[ fspec_pos ] & 0x08)
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
		cat->item011_215.vh = (double) (zn * bb * CMR_ASTX_VH);               // m/Sec
		k                  += 2;
	}
	// I011/270  VO size param
	if (cat->item011_fspec.code[ fspec_pos ] & 0x04)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item011_270.length = code[k] >> 1;                               // metr
		k                      += 1;
		if (code[ k - 1] & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_270.orientation = ((code[k] >> 1) * 2.81) / DEG; // grad
			k                           += 1;
			if (code[ k - 1 ] & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				cat->item011_270.width = code[k] >> 1;                // metr
				k                     += 1;
			}
		}
	}
	// I011/390  plan info
	if (cat->item011_fspec.code[ fspec_pos ] & 0x02)
	{
		cat->item011_390 = decodeElem390PlanData(code, &k, cdgm_size);
	}


	// 4 octet
	fspec_pos = 3;
	// I011/300  Vehicle Fleet Identification
	if (cat->item011_fspec.code[ fspec_pos ] & 0x80)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item011_300.fleet_id = code[k];
		k                        += 1;
	}
	// I011/310  Pre-programmed Message
	if (cat->item011_fspec.code[ fspec_pos ] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		if (code[ k ] & 0x80)
			cat->item011_310.in_trouble = 1;
		cat->item011_310.work_programm_code = code[k] & 0x7F;
		k                                  += 1;
	}
	// I011/500  accuracy
	if (cat->item011_fspec.code[ fspec_pos ] & 0x20)
	{
		u_int8_t  octet = 0;
		u_int16_t bb;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		octet = code[k];
		k    += 1;

		// CKO on XY
		if (octet & 0x80)                          // Estimated Accuracy Of Track Position (Cartesian)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_500.accuracy.x_component = code[k] * 0.25;
			k                                    += 1;
			cat->item011_500.accuracy.y_component = code[k] * 0.25;
			k                                    += 1;
		}
		// CKO geo
		if (octet & 0x40)                          // Estimated Accuracy Of Track Position (WGS-84)
		{
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			bb                                          = (code[k] << 8) | code[k + 1];
			k                                          += 2;
			cat->item011_500.accuracy.lat_component     = bb * CMR_GEO_31;
			cat->item011_500.accuracy.lat_component    /= DEG;
			bb                                          = (code[k] << 8) | code[k + 1];
			k                                          += 2;
			cat->item011_500.accuracy.longit_component  = bb * CMR_GEO_31;
			cat->item011_500.accuracy.longit_component /= DEG;
		}
		// CKO on H geometric
		if (octet & 0x20)                          // Estimated Accuracy Of Calculated Track Geometric Altitude
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			bb                                        = (code[k] << 8) | code[k + 1];
			cat->item011_500.accuracy.h_geo_component = bb * 0.5;
			k                                        += 2;
		}
		// CKO VXVY
		if (octet & 0x10)                          // Estimated Accuracy Of Track Velocity (Cartesian)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_500.accuracy.vx_component = code[k] * 0.1;
			k                                     += 1;
			cat->item011_500.accuracy.vy_component = code[k] * 0.1;
			k                                     += 1;
		}
		// CKO on VH
		if (octet & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_500.accuracy.vh_component = code[k] * 0.1;
			k                                     += 1;
		}
		// CKO on AXAY
		if (octet & 0x04)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_500.accuracy.ax_component = code[k] * 0.01;
			k                                     += 1;

			cat->item011_500.accuracy.ay_component = code[k] * 0.01;
			k                                     += 1;
		}
	}

	// I011/600  Alert Messages
	if (cat->item011_fspec.code[ fspec_pos ] & 0x10)
	{
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		if (code[ k ] & 0x80)
			cat->item011_600.alert_acknowledged = 1;
		cat->item011_600.alert_level  = (code[ k ] & 0x60) >> 5;
		cat->item011_600.alert_type   = code[ k + 1];
		cat->item011_600.alert_number = code[ k + 2];
		k                            += 3;
	}

	// I011/605  Tracks in Alert
	if (cat->item011_fspec.code[ fspec_pos ] & 0x08)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		cat->item011_605.all_track_in_alert = code[ k ];
		k                                  += 1;

		if (cat->item011_605.all_track_in_alert > 0)
		{
			if (checkOverloadFunc(k, sizeof(u_int16_t) * cat->item011_605.all_track_in_alert, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}

			cat->item011_605.fusion_track_num = (u_int16_t*)malloc(sizeof(u_int16_t) * cat->item011_605.all_track_in_alert);
			if (cat->item011_605.fusion_track_num)
				memcpy(cat->item011_605.fusion_track_num, code + k, sizeof(u_int16_t) * cat->item011_605.all_track_in_alert);
			k += (sizeof(u_int16_t) * cat->item011_605.all_track_in_alert);
		}
	}

	// I011/605  Holdbar Status
	if (cat->item011_fspec.code[ fspec_pos ] & 0x04)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		cat->item011_610.all_holdbars = code[ k ];
		k                            += 1;

		if (cat->item011_610.all_holdbars > 0)
		{
			if (checkOverloadFunc(k, sizeof(u_int16_t) * cat->item011_610.all_holdbars, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}

			cat->item011_610.holdbars = (u_int16_t*)malloc(sizeof(u_int16_t) * cat->item011_610.all_holdbars);

			if (cat->item011_610.holdbars)
				memcpy(cat->item011_610.holdbars, code + k, sizeof(u_int16_t) * cat->item011_610.all_holdbars);

			k += (sizeof(u_int16_t) * cat->item011_610.all_holdbars);
		}
	}

	if (cat->item011_fspec.code[ fspec_pos ] & 0x02)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		if (code[ k ] > 0)                         // Есть данные поля SP
		{
			if (checkOverloadFunc(k, code[ k ], cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			cat->item011_sp = (Cat011specialPuposeField*)malloc(sizeof(Cat011specialPuposeField));

			if (cat->item011_sp)
			{
				// bzero(cat->item011_sp, sizeof(Cat011specialPuposeField));
				memset(cat->item011_sp, 0, sizeof(Cat011specialPuposeField));
				cat->item011_sp->el_sp = decode011defaultSPdata(code, &k, cdgm_size);
			}
		}
	}

	*start_byte_pos = k;
	return asterix;
}

/**
 * @brief Упаковка данных наблюдения АЗН-В для сообщения категори 11
 * @param ads_b - указатель на структуру данных АЗН-В
 * @return - указатель на структуру с кодированными данными
 */
TCommonObmenData* pack011AdsData(Cat011ed380AdsBRelatedData* ads_b)
{
	unsigned char code[DEFAULT_CODE_SIZE] =
	{
		0
	};
	int               cnt = 0;
	TCommonObmenData* ads = NULL;

	if (ads_b == NULL)
		return ads;

	code[cnt] = 0;
	cnt      += 1;
	code[cnt] = 0;
	cnt      += 1;
	// Mode S MB data
	if ((ads_b->mode_s_msg) &&
	    (ads_b->mode_s_data_count > 0) &&
	    (ads_b->mode_s_data_count <= 255))
	{
		int i, j;
		code[cnt] = ads_b->mode_s_data_count;
		cnt      += 1;
		for (i = 0; i < ads_b->mode_s_data_count; i++)
		{
			for (j = 0; j < 8; j++)
			{
				code[ cnt ] = ads_b->mode_s_msg[i] >> (8 * (7 - j));
				cnt        += 1;
			}
		}
		code[ 0 ] |= 0x80;
	}

	// Adres VO
	if (ads_b->target_address > 0)                     // ICAO24
	{
		code[cnt]  = ads_b->target_address >> 16;
		cnt       += 1;
		code[cnt]  = ads_b->target_address >> 8;
		cnt       += 1;
		code[cnt]  = ads_b->target_address;
		cnt       += 1;
		code[ 0 ] |= 0x40;
	}

	// Communications/ACAS Capability and Flight Status
	//! TCAS Data VO
	if ((ads_b->communication_capability) ||
	    (ads_b->flight_status) ||
	    (ads_b->specific_service) ||
	    (ads_b->altitude_report_capability) ||
	    (ads_b->aircraft_ident_capability) ||
	    (ads_b->bit_b1a) ||
	    (ads_b->bits_b1b) ||
	    (ads_b->acas_operational) ||
	    (ads_b->multiple_navigational) ||
	    (ads_b->differencial_correction))
	{
		code[cnt]  = (ads_b->communication_capability) << 5;
		code[cnt] |= ((ads_b->flight_status & 0x0F) << 1);
		cnt       += 1;
		if (ads_b->specific_service)
			code[cnt] |= 0x80;
		if (ads_b->altitude_report_capability)
			code[cnt] |= 0x40;
		if (ads_b->aircraft_ident_capability)
			code[cnt] |= 0x20;
		if (ads_b->bit_b1a)
			code[cnt] |= 0x10;
		code[cnt] |= (ads_b->bits_b1b & 0x0f);
		cnt       += 1;
		if (ads_b->acas_operational)
			code[cnt] |= 0x80;
		if (ads_b->multiple_navigational)
			code[cnt] |= 0x40;
		if (ads_b->differencial_correction)
			code[cnt] |= 0x20;
		cnt       += 1;
		code[ 0 ] |= 0x10;
	}
	// Type of AirCraft
	if (strlen(ads_b->aircraft_type) > 0)
	{
		code[ 0 ] |= 0x01;
		code[ 1 ] |= 0x80;
		memcpy(code + cnt, ads_b->aircraft_type, 4);
		cnt += 4;
	}
	// Emitter Category
	if (ads_b->emitter_category > 0)
	{
		u_int8_t b;
		switch (ads_b->emitter_category)
		{
		case ECAT_AIR_BALLOON:              b = ECAT_AIR_BALLOON;
			break;
		case ECAT_FIXED_GROUND:             b = ECAT_FIXED_GROUND;
			break;
		case ECAT_GLIDER:                   b = ECAT_GLIDER;
			break;
		case ECAT_HEAVY_AIRCRAFT:           b = ECAT_HEAVY_AIRCRAFT;
			break;
		case ECAT_LIGHT_AIRCRAFT:           b = ECAT_LIGHT_AIRCRAFT;
			break;
		case ECAT_MEDIUM_AIRCRAFT:          b = ECAT_MEDIUM_AIRCRAFT;
			break;
		case ECAT_PARACHUTIST:              b = ECAT_PARACHUTIST;
			break;
		case ECAT_PARA_GLIDER:              b = ECAT_PARA_GLIDER;
			break;
		case ECAT_ROTORCRAFT:               b = ECAT_ROTORCRAFT;
			break;
		case ECAT_SPACE_SHUTTLE:            b = ECAT_SPACE_SHUTTLE;
			break;
		case ECAT_SPEED_AND_MANOEUVRABLE:   b = ECAT_SPEED_AND_MANOEUVRABLE;
			break;
		case ECAT_SURFACE_EMERGENCY_VEHICLE: b = ECAT_SURFACE_EMERGENCY_VEHICLE;
			break;
		case ECAT_SURFACE_SERVICE_VEHICLE:  b = ECAT_SURFACE_SERVICE_VEHICLE;
			break;
		case ECAT_UAV:                      b = ECAT_UAV;
			break;
		default:
			b = 0;
			break;

		}

		if (b > 0)
		{
			code[cnt]  = b;
			cnt       += 1;
			code[ 0 ] |= 0x01;
			code[ 1 ] |= 0x40;
		}
	}

	// Available Technologies
	if ((ads_b->uat_available) ||
	    (ads_b->mode_s_available) ||
	    (ads_b->vdl_available))
	{
		if (ads_b->vdl_available)
			code[cnt] |= 0x80;
		if (ads_b->mode_s_available)
			code[cnt] |= 0x40;
		if (ads_b->uat_available)
			code[cnt] |= 0x20;
		cnt       += 1;
		code[ 0 ] |= 0x01;
		code[ 1 ] |= 0x10;
	}

	if (code[1] == 0)
	{
		memmove(code + 1, code + 2, cnt - 2);
		cnt -= 1;
	}

	ads = initObmenData(cnt);
	if (ads != NULL)
	{
		setObmenData(ads, code, cnt);
	}

	return ads;
}

/**
 * @brief Упаковка поля SP в категории 011 (по умолчанию)
 * @param sp_elem - указатель на поле данных SP
 * @return - кодированная последовательность в байтах для укладки в кодограмму
 */

TCommonObmenData* pack011defaultSPdata(TSpData* sp)
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
 * @brief Упаковка сообщения категории 011 (ed.1.2 May 2008) Астерикс
 * @param code - указатель на массив для кодограммы
 * @return - указатель на структуру с кодированным сообщением
 */
TCommonObmenData* pack011catRecord(TCat011* cat)
{
	u_int8_t code[DEFAULT_CODE_SIZE] =
	{
		0
	};
	u_int8_t          fspec_pos = 0;
	size_t            cnt       = 0;
	TCommonObmenData* msg       = NULL;


	if (cat == NULL)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 011 ERROR - nothing function argument\n", getpid());
		return msg;
	}

	if (cat->item011_fspec.code == NULL)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 011 ERROR - fspec code not set\n", getpid());
		return msg;
	}

	if (cat->item011_010.sac_sic == 0)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 011 ERROR - sic sac data NULL\n", getpid());
		return msg;
	}

	if (cat->item011_000.message_type == 0)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 011 ERROR - message type unknown\n", getpid());
		return msg;
	}

	if ((cat->item011_140.time_of_track < 0) &&
	    (cat->item011_140.time_of_track >= DEFAULT_TIME_OF_TRACK))
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 011 ERROR - time of Day not present\n", getpid());
		return msg;
	}

	cnt += FSPEC_SIZE_5_BYTE;

	// I011/010 sic_sac ident code
	// SAC = 0 fixed value
	if (cat->item011_fspec.code[fspec_pos] & 0x80)
	{
		code[cnt] = 0;
		cnt      += 1;
		code[cnt] = cat->item011_010.sac_sic;
		cnt      += 1;
	}
	// I011/000 Message type
	if (cat->item011_fspec.code[fspec_pos] & 0x40)
	{
		code[cnt] = cat->item011_000.message_type;
		cnt      += 1;
	}

	// I011/015  service code
	if (cat->item011_fspec.code[fspec_pos] & 0x20)
	{
		code[cnt] = cat->item011_015.sevice_id;
		cnt      += 1;
	}
	// I011/140  time message
	// time on server
	// in sec from 00:00
	if (cat->item011_fspec.code[fspec_pos] & 0x10)
	{
		int ccc = (int) (cat->item011_140.time_of_track / CMR_ASTX_TIME);
		code[cnt] = ccc >> 16;
		cnt      += 1;
		code[cnt] = ccc >> 8;
		cnt      += 1;
		code[cnt] = ccc;
		cnt      += 1;
	}

	// I011/041  WGS-84 coord
	if (cat->item011_fspec.code[fspec_pos] & 0x08)
	{
		int diff = cnt, ccc, i;
		if ((cat->item011_041.latitude != DEFAULT_RADIAN_GEO_POS) ||
		    (cat->item011_041.longitude != DEFAULT_RADIAN_GEO_POS))
		{
			for (i = 0; i < 2; i++)
			{
				if (i == 0)
					ccc = (int) (cat->item011_041.latitude / (CMR_GEO_31 * M_PI / 180.));
				if (i == 1)
					ccc = (int) (cat->item011_041.longitude / (CMR_GEO_31 * M_PI / 180.));
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
			cat->item011_fspec.code[fspec_pos] &= ~0x08;
	}
	// I011/100  XY coord
	if (cat->item011_fspec.code[fspec_pos] & 0x04)
	{
		int diff = cnt, i, ccc = 0;
		if ((cat->item011_042.x != DEFAULT_CARTESIAN_POS) ||
		    (cat->item011_042.y != DEFAULT_CARTESIAN_POS))
		{
			for (i = 0; i < 2; i++)
			{
				if (i == 0)
					ccc = (int) (cat->item011_042.x);
				if (i == 1)
					ccc = (int) (cat->item011_042.y);

				//            if(i==0)
				//                ccc1=trs->pos.y;
				//            if(i==1)
				//                ccc1=trs->pos.x;
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
			}
		}
		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x04;
	}
	// I011/202  speed vx vy
	if (cat->item011_fspec.code[fspec_pos] & 0x02)
	{
		int diff = cnt, i, ccc;
		if ((cat->item011_202.vx != DEFAULT_VELOCITY) ||
		    (cat->item011_202.vy != DEFAULT_VELOCITY))
		{
			for (i = 0; i < 2; i++)
			{
				if (i == 0)
					ccc = (int) (cat->item011_202.vx / CMR_VXVY);
				if (i == 1)
					ccc = (int) (cat->item011_202.vy / CMR_VXVY);
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
			}
		}
		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x02;
	}
	fspec_pos = 1;
	// I011/210  speed ax ay
	if (cat->item011_fspec.code[fspec_pos] & 0x80)
	{
		int diff = cnt, i, ccc;
		if ((cat->item011_210.ax != DEFAULT_ACCELERATION) ||
		    (cat->item011_210.ay != DEFAULT_ACCELERATION))
		{
			for (i = 0; i < 2; i++)
			{
				if (i == 0)
					ccc = (int) (cat->item011_210.ax / CMR_VXVY);
				if (i == 1)
					ccc = (int) (cat->item011_210.ay / CMR_VXVY);
				code[cnt] = ccc;
				cnt      += 1;
			}
		}
		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x80;
	}

	// I011/060  Mode 3A
	if (cat->item011_fspec.code[fspec_pos] & 0x40)
	{
		if (cat->item011_060.m3a.board_number_code > DEFAULT_RBS_UVD_NUM_CODE)
		{
			code[cnt] = cat->item011_060.m3a.board_number_code >> 8;
			cnt      += 1;
			code[cnt] = cat->item011_060.m3a.board_number_code;
			cnt      += 1;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x40;
	}
	// I011/245  Ident VO IA-5
	if (cat->item011_fspec.code[fspec_pos] & 0x20)
	{
		int diff = cnt;

		if ((strlen(cat->item011_245.target_id) > 0) &&
		    (cat->item011_390 == NULL))
		{
			u_int64_t cnt_pos;
			code[cnt] = 0;
			switch (cat->item011_245.target_id_src)
			{
			case TIS_CALLSIGN_NOT_FROM_TRANSPONDER:
				code[cnt] = 1;
				break;
			case TIS_REGISTRATION_NOT_FROM_TRANSPONDER:
				code[cnt] = 2;
				break;
			case TIS_UNDEFINED:
				code[cnt] = 3;
				break;
			}
			cnt    += 1;
			cnt_pos = cnt;
			convertAsciiToIA5code(code, (unsigned char*) cat->item011_245.target_id, &cnt_pos);
			cnt = cnt_pos;
		}

		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x20;
	}
	// I011/380  ADS-B data
	if (cat->item011_fspec.code[fspec_pos] & 0x10)
	{
		int diff = cnt;
		if (cat->item011_380)
		{
			TCommonObmenData* ads = pack011AdsData(cat->item011_380);
			if (ads)
			{
				int ccc = getObmenDataLenght(ads);
				if ((ccc > 0) && ((ccc + cnt) < DEFAULT_CODE_SIZE))
				{
					u_int8_t* p = NULL;
					p = (u_int8_t*) getObmenData(ads);
					if (p != NULL)
					{
						memcpy(code + cnt, p, ccc);
						cnt += ccc;
					}
				}
				freeObmenData(ads);
			}
		}

		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x10;
	}
	// I011/161  Track Number
	if (cat->item011_fspec.code[fspec_pos] & 0x08)
	{
		if (cat->item011_161.track_number != DEFAULT_U_INT16_CODE)
		{
			code[cnt] = cat->item011_161.track_number >> 8;
			cnt      += 1;
			code[cnt] = cat->item011_161.track_number;
			cnt      += 1;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x08;
	}
	// I011/170  Track Status only + I011/161  Track Number
	if ((cat->item011_fspec.code[fspec_pos] & 0x08) &&
	    (cat->item011_fspec.code[fspec_pos] & 0x04))
	{
		u_int8_t octet[FSPEC_SIZE_4_BYTE] =
		{
			0
		}, b = 0;
		// 1 octet
		octet[ 0 ] = 0;

		if (cat->item011_170.track_status.monosensor == 1)
			octet[0] |= 0x80;
		if (cat->item011_170.track_status.ground_bit_set == 1)
			octet[0] |= 0x40;
		if (cat->item011_170.track_status.most_reliable_height == 1)
			octet[0] |= 0x20;
		// by default b1 = 0 -> ALTITUDE_SRC_UNKNOWN;
		switch (cat->item011_170.track_status.geometric_altitude_source)
		{
		case AS_UNKNOWN:  b = 0;
			break;
		case AS_GPS:      b = 1;
			break;
		case AS_3D_RADAR: b = 2;
			break;
		case AS_TRIANG:   b = 3;
			break;
		case AS_COVERAGE: b = 4;
			break;
		case AS_TABLE:    b = 5;
			break;
		case AS_FICTION:  b = 6;
			break;
		case AS_MLAT:     b = 7;
			break;
		default:
			b = 0;
			break;
		}
		octet[0] |= (b << 2);
		if (cat->item011_170.track_status.confirm == 1)
			octet[0] |= 0x02;

		// 2 octet

		octet[1] = 0;
		if (cat->item011_170.track_status.simulate == 1)
			octet[1] |= 0x80;
		if (cat->item011_170.track_status.last_message == 1)
			octet[1] |= 0x40;
		if (cat->item011_170.track_status.first_message == 1)
			octet[1] |= 0x20;
		b         = cat->item011_170.track_status.mode4_interrogate & 0x03;
		octet[1] |= (b << 3);
		if (cat->item011_170.track_status.military_emrg == 1)
			octet[1] |= 0x04;
		if (cat->item011_170.track_status.military_id == 1)
			octet[1] |= 0x02;
		if (octet[1] > 0)
		{
			octet[0] |= 0x01;
		}

		// 3 octet
		octet[2] = 0;
		if (cat->item011_170.track_status.amalgamation == 1)
			octet[2] |= 0x80;
		if (cat->item011_170.track_status.spi_sign == 1)
			octet[2] |= 0x40;
		if (cat->item011_170.track_status.coast_track == 1)
			octet[2] |= 0x20;
		if (cat->item011_170.track_status.flight_in_plan == 1)
			octet[2] |= 0x10;
		if (cat->item011_170.track_status.ads_inconsist == 1)
			octet[2] |= 0x08;
		if (octet[2] > 0)
		{
			octet[0] |= 0x01;
			octet[1] |= 0x01;
		}

		// 4 octet
		octet[3] = 0;
		if (cat->item011_170.track_status.psr_coast == 1)
			octet[3] |= 0x40;
		if (cat->item011_170.track_status.ssr_coast == 1)
			octet[3] |= 0x20;
		if (cat->item011_170.track_status.mode_s_coast == 1)
			octet[3] |= 0x10;
		if (cat->item011_170.track_status.ads_coast == 1)
			octet[3] |= 0x08;
		if (cat->item011_170.track_status.suc_set == 1)
			octet[3] |= 0x04;
		if (cat->item011_170.track_status.aac_conflict == 1)
			octet[3] |= 0x02;

		if (octet[3] > 0)
		{
			octet[0] |= 0x01;
			octet[1] |= 0x01;
			octet[2] |= 0x01;
		}

		code[ cnt ] = octet[0];
		cnt        += 1;
		b           = 1;
		while (octet[ b - 1 ] & 0x01)
		{
			code[ cnt ] = octet[ b ];
			cnt        += 1;
			b          += 1;
			if (b == FSPEC_SIZE_4_BYTE)
				break;
		}
	}
	// I011/290  System Track Update Ages
	if (cat->item011_fspec.code[fspec_pos] & 0x02)
	{
		u_int8_t b    = 0;
		int      diff = cnt;

		if ((cat->item011_290.trk_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.psr_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.ssr_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.mds_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.ads_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.ads_es_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.mfl_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.mda_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.md1_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.md2_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.loop_age >= CMR_AGE_TIME) ||
		    (cat->item011_290.mlt_age >= CMR_AGE_TIME))
		{
			// 1 octet
			cnt            += 2;
			code[ diff ]    = 0;
			code[ diff + 1] = 0;

			if (cat->item011_290.psr_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.psr_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x80;
				}
			}
			if (cat->item011_290.ssr_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.ssr_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x40;
				}
			}
			if (cat->item011_290.mda_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.mda_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x20;
				}
			}

			if (cat->item011_290.mfl_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.mfl_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x10;
				}
			}

			if (cat->item011_290.mds_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.mds_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x08;
				}
			}

			if ((cat->item011_290.ads_age >= CMR_AGE_TIME) &&
			    (cat->item011_290.ads_age < DEFAULT_TIME_AGES))
			{
				int ccc = (u_int16_t) (cat->item011_290.ads_age / CMR_AGE_TIME);
				if (ccc > DEFAULT_U_INT16_CODE)
					ccc = DEFAULT_U_INT16_CODE;
				code[cnt]     = ccc >> 8;
				cnt          += 1;
				code[cnt]     = ccc;
				cnt          += 1;
				code[ diff ] |= 0x04;
			}

			if (cat->item011_290.ads_es_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.ads_es_age);
				if (b > 0)
				{
					code[ cnt ]   = b;
					cnt          += 1;
					code[ diff ] |= 0x02;
				}
			}
			// 2 octet
			if (cat->item011_290.md1_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.md1_age);
				if (b > 0)
				{
					code[ cnt ]      = b;
					cnt             += 1;
					code[ diff + 1] |= 0x80;
				}
			}

			if (cat->item011_290.md2_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.md2_age);
				if (b > 0)
				{
					code[ cnt ]      = b;
					cnt             += 1;
					code[ diff + 1] |= 0x40;
				}
			}

			if (cat->item011_290.loop_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.loop_age);
				if (b > 0)
				{
					code[ cnt ]      = b;
					cnt             += 1;
					code[ diff + 1] |= 0x20;
				}
			}

			if (cat->item011_290.trk_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.trk_age);
				if (b > 0)
				{
					code[ cnt ]      = b;
					cnt             += 1;
					code[ diff + 1] |= 0x10;
				}
			}
			if (cat->item011_290.mlt_age >= CMR_AGE_TIME)
			{
				b = packAgeTime(cat->item011_290.mlt_age);
				if (b > 0)
				{
					code[ cnt ]      = b;
					cnt             += 1;
					code[ diff + 1] |= 0x08;
				}
			}

			if ((code[diff] == 0) && (code[diff + 1] == 0))
			{
				cnt -= 2;
			}
			else
			{
				if (code[diff + 1] > 0)
				{
					code[diff] |= 0x01;
				}
				else
				{
					memmove(code + diff + 1, code + diff + 2, cnt - diff - 2);
					cnt -= 1;
				}
			}
		}

		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x02;

	}

	// I011/430  Phase of Flight
	fspec_pos = 2;
	if (cat->item011_fspec.code[fspec_pos] & 0x80)
	{
		if (cat->item011_430.phase_fligth != DEFAULT_U_INT8_CODE)
		{
			code[ cnt ] = cat->item011_430.phase_fligth;
			cnt        += 1;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x80;
	}
	// I011/090  measured FL
	if (cat->item011_fspec.code[fspec_pos] & 0x40)
	{
		if (cat->item011_090.measured_fligth_level != DEFAULT_ALTITUDE)
		{
			int ccc = (int) (cat->item011_090.measured_fligth_level / CMR_ASTX_H_25F); // m
			code[cnt] = ccc >> 8;
			cnt      += 1;
			code[cnt] = ccc;
			cnt      += 1;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= 0x40;
	}
	// I011/093  calc H bar
	if (cat->item011_fspec.code[fspec_pos] & 0x20)
	{
		if (cat->item011_093.calc_b_height != DEFAULT_ALTITUDE)
		{
			int ccc = (int) (cat->item011_093.calc_b_height / CMR_ASTX_H_25F);         // m
			code[cnt] = (ccc >> 8) & 0x7f;
			if (cat->item011_093.qnh_correction == 1)
				code[cnt] |= 0x80;
			cnt      += 1;
			code[cnt] = ccc;
			cnt      += 1;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x20;
	}
	// I011/092  calc H geo
	if (cat->item011_fspec.code[fspec_pos] & 0x10)
	{
		if (cat->item011_092.calc_g_height != DEFAULT_ALTITUDE)
		{
			int ccc = (int) (cat->item011_092.calc_g_height / CMR_ASTX_H_6_25F); // m
			code[cnt] = ccc >> 8;
			cnt      += 1;
			code[cnt] = ccc;
			cnt      += 1;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x10;
	}
	// I011/215  speed vh
	if (cat->item011_fspec.code[fspec_pos] & 0x08)
	{
		if (cat->item011_215.vh != DEFAULT_VELOCITY)
		{
			int ccc = (int) (cat->item011_215.vh / CMR_ASTX_VH);                 // m/Sec
			code[cnt] = ccc >> 8;
			cnt      += 1;
			code[cnt] = ccc;
			cnt      += 1;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x08;
	}
	// I011/270  VO size param
	if (cat->item011_fspec.code[fspec_pos] & 0x04)
	{
		int diff = cnt;

		if ((cat->item011_270.length != DEFAULT_U_INT8_CODE) ||
		    (cat->item011_270.width != DEFAULT_U_INT8_CODE) ||
		    (cat->item011_270.orientation != DEFAULT_RADIAN_GEO_POS))
		{
			TReturnOneInt ret = packTargetSizeAndOrientation(cat->item011_270.length,
			                                                 cat->item011_270.width,
			                                                 cat->item011_270.orientation);
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

		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x04;
	}
	// I011/390  plan info
	if (cat->item011_fspec.code[fspec_pos] & 0x02)
	{
		int diff = cnt;
		if (cat->item011_390)
		{
			TCommonObmenData* pl = packElem390PlanData(&cat->item011_390->fpdata, 0x00ffff00);

			if (pl)
			{
				int ccc = getObmenDataLenght(pl);
				if ((ccc > 0) && ((cnt + ccc) < DEFAULT_CODE_SIZE))
				{
					u_int8_t* p = NULL;
					p = (u_int8_t*) getObmenData(pl);
					if (p != NULL)
					{
						memcpy(code + cnt, p, ccc);
						cnt += ccc;
					}
				}
				freeObmenData(pl);
			}
		}

		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x02;
	}

	fspec_pos = 3;
	// I011/300  Vehicle Fleet Identification
	if (cat->item011_fspec.code[fspec_pos] & 0x80)
	{
		if (cat->item011_300.fleet_id != DEFAULT_U_INT8_CODE)
		{
			code[cnt] = cat->item011_300.fleet_id;
			cnt      += 1;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x80;
	}
	// I011/310  Pre-programmed Message
	if (cat->item011_fspec.code[fspec_pos] & 0x40)
	{
		if (cat->item011_310.work_programm_code != DEFAULT_U_INT8_CODE)
		{
			code[cnt] = cat->item011_310.work_programm_code & 0x7f;
			if (cat->item011_310.in_trouble == 1)
				code[cnt] |= 0x80;
			cnt += 1;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x40;
	}
	// I011/500  accuracy
	if (cat->item011_fspec.code[fspec_pos] & 0x20)
	{
		int diff = cnt, ccc;

		if ((cat->item011_500.accuracy.x_component) ||
		    (cat->item011_500.accuracy.y_component) ||
		    (cat->item011_500.accuracy.lat_component) ||
		    (cat->item011_500.accuracy.longit_component) ||
		    (cat->item011_500.accuracy.h_geo_component) ||
		    (cat->item011_500.accuracy.vx_component) ||
		    (cat->item011_500.accuracy.vy_component) ||
		    (cat->item011_500.accuracy.vh_component) ||
		    (cat->item011_500.accuracy.ax_component) ||
		    (cat->item011_500.accuracy.ay_component))
		{
			cnt         += 1;
			code[ diff ] = 0;
			// CKO on XY
			if ((cat->item011_500.accuracy.x_component > 0) ||
			    (cat->item011_500.accuracy.y_component > 0))
			{
				ccc = (int) (cat->item011_500.accuracy.x_component / 0.25);
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt] = ccc;
				cnt      += 1;
				ccc       = (int) (cat->item011_500.accuracy.y_component / 0.25);
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt]     = ccc;
				cnt          += 1;
				code[ diff ] |= 0x80;
			}
			// CKO geo
			if ((cat->item011_500.accuracy.lat_component > 0) ||
			    (cat->item011_500.accuracy.longit_component > 0))
			{
				ccc = (int) ((cat->item011_500.accuracy.lat_component * DEG) / CMR_GEO_31);
				if (ccc > DEFAULT_U_INT16_CODE)
					ccc = DEFAULT_U_INT16_CODE;
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
				ccc       = (int) ((cat->item011_500.accuracy.longit_component * DEG) / CMR_GEO_31);
				if (ccc > DEFAULT_U_INT16_CODE)
					ccc = DEFAULT_U_INT16_CODE;
				code[cnt]     = ccc >> 8;
				cnt          += 1;
				code[cnt]     = ccc;
				cnt          += 1;
				code[ diff ] |= 0x40;
			}
			// CKO on H geo
			if (cat->item011_500.accuracy.h_geo_component > 0)
			{
				ccc = (unsigned short) (cat->item011_500.accuracy.h_geo_component / 0.5);
				if (ccc > DEFAULT_U_INT16_CODE)
					ccc = DEFAULT_U_INT16_CODE;
				code[cnt]     = ccc >> 8;
				cnt          += 1;
				code[cnt]     = ccc;
				cnt          += 1;
				code[ diff ] |= 0x20;
			}
			// CKO VXVY
			if ((cat->item011_500.accuracy.vx_component > 0) ||
			    (cat->item011_500.accuracy.vy_component > 0))
			{
				ccc = (unsigned char) (cat->item011_500.accuracy.vx_component / 0.1);
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt] = ccc;
				cnt      += 1;
				ccc       = (unsigned char) (cat->item011_500.accuracy.vy_component / 0.1);
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt]     = ccc;
				cnt          += 1;
				code[ diff ] |= 0x10;
			}

			// CKO on VH
			if (cat->item011_500.accuracy.vh_component > 0)
			{
				ccc = (unsigned char) (cat->item011_500.accuracy.vh_component / 0.1);        // m/sec
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt]     = ccc;
				cnt          += 1;
				code[ diff ] |= 0x08;
			}

			// CKO on AXAY
			if ((cat->item011_500.accuracy.ax_component != 0) ||
			    (cat->item011_500.accuracy.ay_component != 0))
			{
				ccc = (unsigned char) (cat->item011_500.accuracy.ax_component / 0.01);       // m/sec^2
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt] = ccc;
				cnt      += 1;
				ccc       = (unsigned char) (cat->item011_500.accuracy.ay_component / 0.01); // m/sec^2
				if (ccc > DEFAULT_U_INT8_CODE)
					ccc = DEFAULT_U_INT8_CODE;
				code[cnt]     = ccc;
				cnt          += 1;
				code[ diff ] |= 0x04;
			}

			if (code[ diff ] == 0)
			{
				cnt = diff;
			}
		}

		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x20;
	}

	// I011/600  Alert Messages
	if (cat->item011_fspec.code[fspec_pos] & 0x10)
	{
		int diff = cnt;

		if ((cat->item011_600.alert_type != DEFAULT_U_INT8_CODE) ||
		    (cat->item011_600.alert_number != DEFAULT_U_INT8_CODE))
		{
			code[cnt] = ((cat->item011_600.alert_level & 0x03) << 5);
			if (cat->item011_600.alert_acknowledged == 1)
				code[ cnt ] |= 0x80;
			cnt      += 1;
			code[cnt] = cat->item011_600.alert_type;
			cnt      += 1;
			code[cnt] = cat->item011_600.alert_number;
			cnt      += 1;
		}

		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x10;
	}
	// I011/605  Tracks in Alert
	if (cat->item011_fspec.code[fspec_pos] & 0x08)
	{
		int size = 0;
		if (cat->item011_605.fusion_track_num)
		{
			code[cnt] = cat->item011_605.all_track_in_alert;
			cnt      += 1;
			size      = cat->item011_605.all_track_in_alert * sizeof(u_int16_t);
			memcpy(code + cnt, cat->item011_605.fusion_track_num, size);
			cnt += size;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x08;
	}
	// I011/610  Holdbar Status
	if (cat->item011_fspec.code[fspec_pos] & 0x04)
	{
		int size = 0;
		if (cat->item011_610.holdbars)
		{
			code[cnt] = cat->item011_610.all_holdbars;
			cnt      += 1;
			size      = cat->item011_610.all_holdbars * sizeof(u_int16_t);
			memcpy(code + cnt, cat->item011_610.holdbars, size);
			cnt += size;
		}
		else
			cat->item011_fspec.code[fspec_pos] &= ~0x04;
	}
	// I011/SP
	if (cat->item011_fspec.code[fspec_pos] & 0x02)
	{
		int diff = cnt, ccc;

		if (cat->item011_sp)
		{
			TCommonObmenData* sp_elem = NULL;
			unsigned char*    p       = NULL;

			// Если нет упакованных данных от клиента, упаковываются
			// данные по умолчанию
			if (cat->item011_sp->head.code_size == 0)
			{
				sp_elem = pack011defaultSPdata(cat->item011_sp->el_sp);
				if (sp_elem)
				{
					ccc = getObmenDataLenght(sp_elem);
					if (ccc > 0)
						p = (unsigned char*) getObmenData(sp_elem);
				}
			}
			else
			{
				ccc = cat->item011_sp->head.code_size;
				if (cat->item011_sp->head.code)
					p = (unsigned char*) cat->item011_sp->head.code;
			}

			if (p != NULL)
			{
				memcpy(code + cnt, p, ccc);
				cnt += ccc;
			}

			if (sp_elem)
				freeObmenData(sp_elem);
		}

		if ((cnt - diff) == 0)
			cat->item011_fspec.code[fspec_pos] &= ~0x02;
	}

	if (cnt > FSPEC_SIZE_5_BYTE)                       // уложены данные
	{
		fspec_pos = packFspecToCodogramm(code, cat->item011_fspec.code, FSPEC_SIZE_5_BYTE);
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
 * @brief Запись типа сообщения в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param type_message - тип сообщения
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011MessageType(TCat011* cat, u_int8_t type_message)
{
	int res = -1;


	if (cat == NULL)
		return res;

	if (cat)
	{
		cat->item011_000.message_type = type_message;
		res                           = 0;
		if (cat->item011_fspec.code)
			cat->item011_fspec.code[0] |= 0x40;
	}
	return res;
}

/**
 * @brief Запрос типа сообщения из структуры категории 011
 * @param cat_lib - указатель на структуру категории 011
 * @return  - структура данных возврата:
 *         переменная val - содержит значение типа сообщения
 */
TReturnOneInt get011MessageType(TCat011* cat)
{
	TReturnOneInt ret;

	ret.update = 0;
	ret.val    = 0;

	if (cat == NULL)
		return ret;

	if (cat)
	{
		if (cat->item011_000.message_type < DEFAULT_U_INT8_CODE)
		{
			ret.update = 1;
			ret.val    = cat->item011_000.message_type;
		}
	}
	return ret;
}

/**
 * @brief Запись типа выполняемого задания в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param type_mission  - тип выполняемого задания
 * @param trouble       - признак препятствия исполнению задания
 *                        0 - препятствий нет
 *                        1 - есть проблемы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011PreprogrammedMessage(TCat011* cat, u_int8_t type_mission, u_int8_t trouble)
{
	int res = -1;



	if (cat == NULL)
		return res;

	if (cat)
	{
		cat->item011_310.work_programm_code = type_mission;
		cat->item011_310.in_trouble         = trouble;
		res                                 = 0;
		if (cat->item011_fspec.code)
			cat->item011_fspec.code[3] |= 0x40;
	}
	return res;
}

/**
 * @brief Запрос типа выполняемого задания из структуры категории 011
 * @param cat_lib - указатель на структуру категории 011
 * @return  - структура данных возврата:
 *         переменная val1 - содержит значение типа выполняемого задания
 *         переменная val2 - содержит признак препятствия исполнению задания
 *                              0 - препятствий нет
 *                              1 - есть проблемы
 */
TReturnPairInt get011PreprogrammedMessage(TCat011* cat)
{
	TReturnPairInt ret;

	ret.update = 0;
	ret.val1   = 0;
	ret.val2   = 0;

	if (cat == NULL)
		return ret;

	if (cat)
	{
		if (cat->item011_310.work_programm_code < DEFAULT_U_INT8_CODE)
		{
			ret.update = 1;
			ret.val1   = cat->item011_310.work_programm_code;
			ret.val2   = cat->item011_310.in_trouble;
		}
	}
	return ret;
}

/**
 * @brief Запись этапа выполнения полета в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param fligth_phase  - этап выполнения полета
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011PhaseOfFlight(TCat011* cat, Cat11PhaseOfFlight flight_phase)
{
	int res = -1;


	if (cat == NULL)
		return res;

	if (cat)
	{
		cat->item011_430.phase_fligth = flight_phase;
		res                           = 0;
		if (cat->item011_fspec.code)
			cat->item011_fspec.code[2] |= 0x80;
	}
	return res;
}

/**
 * @brief Запрос этапа выполнения полета из структуры категории 011
 * @param cat_lib - указатель на структуру категории 011
 * @return  - структура данных возврата:
 *         переменная val - содержит значение этапа выполнения полета
 */
TReturnOneInt get011PhaseOfFlight(TCat011* cat)
{
	TReturnOneInt ret;

	ret.update = 0;
	ret.val    = 0;

	if (cat == NULL)
		return ret;

	if (cat)
	{
		if (cat->item011_430.phase_fligth < DEFAULT_U_INT8_CODE)
		{
			ret.update = 1;
			ret.val    = cat->item011_430.phase_fligth;
		}
	}
	return ret;
}

/**
 * @brief Запись сообщения тревоги по наземной ситуации в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param fligth_phase  - этап выполнения полета
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011AlertMessage(TCat011* cat, TCat011AlertMessage* alert)
{
	int res = -1;

	if ((cat == NULL) || (alert == NULL))
		return res;

	if (cat)
	{
		cat->item011_600.alert_acknowledged = alert->alert_acknowledged;
		cat->item011_600.alert_level        = alert->alert_level;
		cat->item011_600.alert_type         = alert->alert_type;
		cat->item011_600.alert_number       = alert->alert_number;

		res = 0;
		if (cat->item011_fspec.code)
			cat->item011_fspec.code[3] |= 0x10;
	}
	return res;
}

/**
 * @brief Запрос тревоги по наземной ситуации из структуры категории 011
 * @param cat_lib - указатель на структуру категории 011
 * @return  - структура данных возврата:
 *         переменная val - содержит значение тревоги по наземной ситуации
 */
TCat011AlertMessage get011AlertMessage(TCat011* cat)
{
	TCat011AlertMessage ret;

	// bzero(&ret, sizeof(TCat011AlertMessage));
	memset(&ret, 0, sizeof(TCat011AlertMessage));

	if (cat == NULL)
		return ret;

	if (cat)
	{
		if ((cat->item011_600.alert_type < DEFAULT_U_INT8_CODE) &&
		    (cat->item011_600.alert_level < DEFAULT_U_INT8_CODE))
		{
			ret.alert_acknowledged = cat->item011_600.alert_acknowledged;
			ret.alert_level        = cat->item011_600.alert_level;
			ret.alert_type         = cat->item011_600.alert_type;
			ret.alert_number       = cat->item011_600.alert_number;
		}
	}
	return ret;
}

/**
 * @brief Запись данных полученных от АЗН (Mode S) в структуру категории
 * @param cat_lib - указатель на структуру категории 011
 * @param fligth_phase  - этап выполнения полета
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set011TrackAircraftRelatedData(TCat011* cat, TCat011AdsBRelatedData* ads_data)
{
	int res = -1, size = 0;

	if ((cat == NULL) || (ads_data == NULL))
		return res;

	if ((cat) && (ads_data))
	{
		if (cat->item011_380 == NULL)
		{
			cat->item011_380 = (Cat011ed380AdsBRelatedData*) malloc(sizeof(Cat011ed380AdsBRelatedData));
			if (cat->item011_380)
				// bzero(cat->item011_380, sizeof(Cat011ed380AdsBRelatedData));
				memset(cat->item011_380, 0, sizeof(Cat011ed380AdsBRelatedData));
		}

		if (cat->item011_380)
		{

			if (cat->item011_380->mode_s_msg)
			{
				free(cat->item011_380->mode_s_msg);
				cat->item011_380->mode_s_msg        = NULL;
				cat->item011_380->mode_s_data_count = 0;
			}

			if ((ads_data->mode_s_msg) && (ads_data->mode_s_data_count > 0))
			{
				cat->item011_380->mode_s_data_count = ads_data->mode_s_data_count;
				size                                = cat->item011_380->mode_s_data_count * sizeof(u_int64_t);

				cat->item011_380->mode_s_msg = (u_int64_t*)malloc(size);

				if (cat->item011_380->mode_s_msg)
					memcpy(cat->item011_380->mode_s_msg, ads_data->mode_s_msg, size);
			}
			strcpy(cat->item011_380->aircraft_type, ads_data->aircraft_type);
			cat->item011_380->acas_operational           = ads_data->acas_operational;
			cat->item011_380->aircraft_ident_capability  = ads_data->aircraft_ident_capability;
			cat->item011_380->altitude_report_capability = ads_data->altitude_report_capability;
			cat->item011_380->bits_b1b                   = ads_data->bits_b1b;
			cat->item011_380->bit_b1a                    = ads_data->bit_b1a;
			cat->item011_380->communication_capability   = ads_data->communication_capability;
			cat->item011_380->differencial_correction    = ads_data->differencial_correction;
			cat->item011_380->emitter_category           = ads_data->emitter_category;
			cat->item011_380->flight_status              = ads_data->flight_status;
			cat->item011_380->mode_s_available           = ads_data->mode_s_available;
			cat->item011_380->multiple_navigational      = ads_data->multiple_navigational;
			cat->item011_380->specific_service           = ads_data->specific_service;
			cat->item011_380->target_address             = ads_data->target_address;
			cat->item011_380->uat_available              = ads_data->uat_available;
			cat->item011_380->vdl_available              = ads_data->vdl_available;

			res = 0;
			if (cat->item011_fspec.code)
				cat->item011_fspec.code[1] |= 0x10;
		}
	}
	return res;
}

/**
 * @brief Выборка данных полученных от АЗН (Mode S) из контейнера категории
 * @param cat - указатель на структуру категории 011
 * @return указатель на структуру с данными АЗН - операция выполнена,
 *         NULL отказ исполнения
 */
TCat011AdsBRelatedData* get011TrackAircraftRelatedData(TCat011* cat)
{
	TCat011AdsBRelatedData* ret  = NULL;
	unsigned int            size = 0;

	if (cat == NULL)
		return ret;

	if (cat->item011_380 == NULL)
		return ret;

	if (cat->item011_380)
	{
		ret = (TCat011AdsBRelatedData*) malloc(sizeof(TCat011AdsBRelatedData));

		if (ret)
		{
			// bzero(ret, sizeof(TCat011AdsBRelatedData));
			memset(ret, 0, sizeof(TCat011AdsBRelatedData));
			if ((cat->item011_380->mode_s_msg) && (cat->item011_380->mode_s_data_count > 0))
			{
				ret->mode_s_data_count = cat->item011_380->mode_s_data_count;
				size                   = ret->mode_s_data_count * sizeof(u_int64_t);
				ret->mode_s_msg        = (u_int64_t*)malloc(size);
				if (ret->mode_s_msg)
					memcpy(ret->mode_s_msg, cat->item011_380->mode_s_msg, size);
			}
			strcpy(ret->aircraft_type, cat->item011_380->aircraft_type);
			ret->acas_operational           = cat->item011_380->acas_operational;
			ret->aircraft_ident_capability  = cat->item011_380->aircraft_ident_capability;
			ret->altitude_report_capability = cat->item011_380->altitude_report_capability;
			ret->bits_b1b                   = cat->item011_380->bits_b1b;
			ret->bit_b1a                    = cat->item011_380->bit_b1a;
			ret->communication_capability   = cat->item011_380->communication_capability;
			ret->differencial_correction    = cat->item011_380->differencial_correction;
			ret->emitter_category           = cat->item011_380->emitter_category;
			ret->flight_status              = cat->item011_380->flight_status;
			ret->mode_s_available           = cat->item011_380->mode_s_available;
			ret->multiple_navigational      = cat->item011_380->multiple_navigational;
			ret->specific_service           = cat->item011_380->specific_service;
			ret->target_address             = cat->item011_380->target_address;
			ret->uat_available              = cat->item011_380->uat_available;
			ret->vdl_available              = cat->item011_380->vdl_available;
		}
	}

	return ret;
}

void free011AdsBRelatedData(TCat011AdsBRelatedData* ads_011)
{
	if (ads_011)
	{
		if (ads_011->mode_s_msg)
			free(ads_011->mode_s_msg);
		free(ads_011);
	}
	return;
}

int viewAdsRawData(unsigned char* code, u_int64_t* cnt, u_int64_t code_size)
{
	unsigned char ads_spec[FSPEC_SIZE_5_BYTE] =
	{
		0
	};
	int       ccc = 0;
	u_int64_t k   = *cnt;

	if (code == NULL)
		return -1;

	if (checkOverloadFunc(k, 2, code_size) < 0)
	{
		return -1;
	}

	ccc = getCategoryFspecSize(code, k, code_size);

	if ((ccc > 0) && (ccc <= FSPEC_SIZE_2_BYTE))
	{
		memcpy(ads_spec, code + k, ccc);
		k += ccc;
	}
	else
	{
		fprintf(stderr, "proc[pid-%d] decodeCodogramm cat 011 in decode 011/380 spec ERROR\n", getpid());
		return -1;
	}
	// Mode S Data
	if (ads_spec[ 0 ] & 0x80)
	{
		// rep factor
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			return -1;
		}
		k += 1;

		if (code[ k - 1] > 0)
		{
			if (checkOverloadFunc(k, code[ k - 1] * sizeof(u_int64_t), code_size) < 0)
			{
				return -1;
			}

			k += (code[ k - 1] * sizeof(u_int64_t));
		}
	}
	// Adress IKAO VO
	if (ads_spec[ 0 ] & 0x40)
	{
		if (checkOverloadFunc(k, 3, code_size) < 0)
		{
			return -1;
		}
		k += 3;
	}
	// Communication/ACAS
	if (ads_spec[ 0 ] & 0x10)
	{
		if (checkOverloadFunc(k, 3, code_size) < 0)
		{
			return -1;
		}
		k += 3;
	}

	// Aircraft Type
	if (ads_spec[ 1 ] & 0x80)
	{
		if (checkOverloadFunc(k, 4, code_size) < 0)
		{
			return -1;
		}
		k += 4;
	}
	// Emitter Category
	if (ads_spec[ 1 ] & 0x40)
	{
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			return -1;
		}
		k += 1;
	}
	// Available Technologies
	if (ads_spec[ 1 ] & 0x10)
	{
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			return -1;
		}
		k += 1;
	}

	*cnt = k;

	return 0;
}

/**
 * @brief Выборка "сырых" данных из записи категории 011 без распаковки
 * @param codogramm - указатель на контейнер с кодограммой
 * @param current_pos - начальное положение первого байта записи в теле кодограммы
 * @return - указатель на структуру контейнер Астерикс
 */
TCatAsterix* get011recordRawData(TCommonObmenData* codogramm, u_int64_t current_pos)
{
	unsigned char* code    = NULL;
	int            ccc     = 0;
	u_int64_t      k       = current_pos, cdgm_size = 0;
	TCatAsterix*   asterix = NULL;
	TCat011*       cat     = NULL;

	if (codogramm == NULL)
	{
		fprintf(stderr, "proc[pid-%d] get011recordRawData cat 011 data pointer ERROR\n", getpid());
		return asterix;
	}

	cdgm_size = getObmenDataLenght(codogramm);

	if (cdgm_size == 0)
	{
		fprintf(stderr, "proc[pid-%d] get011recordRawData cat 011 null size data ERROR\n", getpid());
		return asterix;
	}

	if (checkOverloadFunc(k, cdgm_size, cdgm_size) < 0)
	{
		return asterix;
	}

	code = (unsigned char*) getObmenData(codogramm);
	if (code == NULL)
	{
		fprintf(stderr, "proc[pid-%d] get011recordRawData cat 011 data array ERROR\n", getpid());
		return asterix;
	}

	asterix = createCatAsterixData(CODT_CAT_011);


	if (asterix)
	{
		cat = get011category(asterix);

		set011catDefaultData(cat);

	}
	else
	{
		fprintf(stderr, "proc[pid-%d] get011recordRawData cat 011 create category ERROR\n", getpid());
		return asterix;
	}

	if (cat->item011_fspec.code == NULL)
	{
		freeCatAsterixData(asterix);
		fprintf(stderr, "proc[pid-%d] get011recordRawData fspec header code NULL ERROR\n", getpid());
		return NULL;
	}

	ccc = getCategoryFspecSize(code, k, cdgm_size);

	if ((ccc > 0) && (ccc <= FSPEC_SIZE_5_BYTE) && (k + ccc < cdgm_size))
	{
		if (cat->item011_fspec.code)
		{
			memcpy(cat->item011_fspec.code, code + k, ccc);
			cat->item011_fspec.code_size = ccc;
		}
		k += ccc;
	}
	else
	{
		freeCatAsterixData(asterix);
		fprintf(stderr, "proc[pid-%d] get011recordRawData FSPEC ERROR\n", getpid());
		return NULL;
	}

	if (cat->item011_fspec.code[0] & 0x80)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_010.head, CAT011_EL010, 2, code + k, "Data Source Identifier");
		k += 2;
	}

	if (cat->item011_fspec.code[0] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_000.head, CAT011_EL000, 1, code + k, "Message Type");
		k++;
	}

	if (cat->item011_fspec.code[0] & 0x20)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_015.head, CAT011_EL015, 1, code + k, "Service Identification");
		k++;
	}

	if (cat->item011_fspec.code[0] & 0x10)
	{
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_140.head, CAT011_EL140, 3, code + k, "Time of Track Information");
		k += 3;
	}
	if (cat->item011_fspec.code[0] & 0x08)
	{
		if (checkOverloadFunc(k, 8, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item011_041.head, CAT011_EL041, 8, code + k, "Calculated Track Position (WGS-84)");
		k += 8;
	}

	if (cat->item011_fspec.code[0] & 0x04)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item011_042.head, CAT011_EL042, 4, code + k, "Calculated Position in Cartesian");
		k += 4;
	}

	if (cat->item011_fspec.code[0] & 0x02)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_202.head, CAT011_EL202, 4, code + k, "Calculated Track Velocity in Cartesian");
		k += 4;
	}

	if (cat->item011_fspec.code[1] & 0x80)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_210.head, CAT011_EL210, 2, code + k, "Calculated Acceleration in Cartesian");
		k += 2;
	}

	if (cat->item011_fspec.code[1] & 0x40)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_060.head, CAT011_EL060, 2, code + k, "Mode-3/A Code");
		k += 2;
	}

	if (cat->item011_fspec.code[1] & 0x20)
	{
		if (checkOverloadFunc(k, 7, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_245.head, CAT011_EL245, 7, code + k, "Target Identification");
		k += 7;
	}

	if (cat->item011_fspec.code[1] & 0x10)
	{
		int differ = k;
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		if (viewAdsRawData(code, &k, cdgm_size) < 0)
		{
			fprintf(stderr, "proc[pid-%d] get011recordRawData collect I011/380 ERROR", getpid());
			freeCatAsterixData(asterix);
			return NULL;
		}
		else
		{
			cat->item011_380 = (Cat011ed380AdsBRelatedData*) malloc(sizeof(Cat011ed380AdsBRelatedData));
			if (cat->item011_380)
			{
				// bzero(cat->item011_380, sizeof(Cat011ed380AdsBRelatedData));
				memset(cat->item011_380, 0, sizeof(Cat011ed380AdsBRelatedData));
				setElementHeader(&cat->item011_380->head, CAT011_EL380, k - differ, code + differ, "Ads-B Related Data");
			}
		}
	}

	if (cat->item011_fspec.code[1] & 0x08)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_161.head, CAT011_EL161, 2, code + k, "Track Number");
		k += 2;
	}

	if (cat->item011_fspec.code[1] & 0x04)
	{
		int differ = k;
		// 1 octet
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		k += 1;
		// 2 octet
		if (code[k - 1] & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
			// 3 octet
			if (code[ k - 1 ] & 0x01)
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
				}                          // 4 octet
			}                                  // 3 octet
		}                                          // 2 octet
		setElementHeader(&cat->item011_170.head, CAT011_EL170, k - differ, code + differ, "Track Status");
	}

	// I062/290  news times
	if (cat->item011_fspec.code[1] & 0x02)
	{
		int      differ = k;
		u_int8_t octet1, octet2;

		// 1 octet
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		octet1 = code[k];
		k     += 1;

		// 2 octet
		if (code[ k - 1 ] & 0x01)
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
		if (octet1 & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet1 & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet1 & 0x04)                         // ADS_C time
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		if (octet1 & 0x02)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
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
		if (octet1 & 0x10)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		if (octet2 & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		setElementHeader(&cat->item011_290.head, CAT011_EL290, k - differ, code + differ, "System Track Update Ages");
	}

	if (cat->item011_fspec.code[2] & 0x80)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_430.head, CAT011_EL430, 1, code + k, "Phase of Flight");
		k += 1;
	}
	// I011/090  measured FL
	if (cat->item011_fspec.code[2] & 0x40)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_090.head, CAT011_EL090, 2, code + k, "Measured Flight Level");
		k += 2;
	}
	// I062/093  calc H bar
	if (cat->item011_fspec.code[2] & 0x20)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_093.head, CAT011_EL093, 2, code + k, "Calculated Track Barometric Altitude");
		k += 2;
	}
	// I011/092  calc H geo
	if (cat->item011_fspec.code[2] & 0x10)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_092.head, CAT011_EL092, 2, code + k, "Calculated Track Geometric Altitude");
		k += 2;
	}
	// I011/215  speed vh
	if (cat->item011_fspec.code[2] & 0x08)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_215.head, CAT011_EL215, 2, code + k, "Calculated Rate Of Climb or Descent");
		k += 2;
	}
	// I011/270  VO size param
	if (cat->item011_fspec.code[2] & 0x04)
	{
		int diff = k;
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
		setElementHeader(&cat->item011_270.head, CAT011_EL270, k - diff, code + diff, "Target Size and Orientation");
	}
	// I011/390  plan info
	if (cat->item011_fspec.code[2] & 0x02)
	{
		int diff = k;
		if (view390PlanData(code, &k, cdgm_size) < 0)
		{
			fprintf(stderr, "proc[pid-%d] get011recordRawData collect I011/390 ERROR", getpid());
			freeCatAsterixData(asterix);
			return NULL;
		}
		else
		{
			if (cat->item011_390 == NULL)
				cat->item011_390 = (FlightPlanRelatedData*) malloc(sizeof(FlightPlanRelatedData));
			if (cat->item011_390)
			{
				// bzero(cat->item011_390, sizeof(FlightPlanRelatedData));
				memset(cat->item011_390, 0, sizeof(FlightPlanRelatedData));
				setElementHeader(&cat->item011_390->head, CAT011_EL390, k - diff, code + diff, "Flight Plan Related Data");
			}
		}
	}

	// I011/300  Vehicle Fleet Identification
	if (cat->item011_fspec.code[3] & 0x80)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_300.head, CAT011_EL300, 1, code + k, "Vehicle Fleet Identification");
		k += 1;
	}
	// I011/310  Pre-programmed Message
	if (cat->item011_fspec.code[3] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_310.head, CAT011_EL310, 1, code + k, "Pre-programmed Message");
		k += 1;
	}
	// I011/500  accuracy
	if (cat->item011_fspec.code[3] & 0x20)
	{
		int      diff = k;
		u_int8_t octet;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		octet = code[k];
		k    += 1;

		// CKO on XY
		if (octet & 0x80)                          // Estimated Accuracy Of Track Position (Cartesian)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		// CKO geo
		if (octet & 0x40)                          // Estimated Accuracy Of Track Position (WGS-84)
		{
			if (checkOverloadFunc(k, 4, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 4;
		}
		// CKO on H geometric
		if (octet & 0x20)                          // Estimated Accuracy Of Calculated Track Geometric Altitude
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		// CKO VXVY
		if (octet & 0x10)                          // Estimated Accuracy Of Track Velocity (Cartesian)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		// CKO on VH
		if (octet & 0x08)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
		}
		// CKO on AXAY
		if (octet & 0x04)
		{
			if (checkOverloadFunc(k, 2, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 2;
		}
		setElementHeader(&cat->item011_500.head, CAT011_EL500, k - diff, code + diff, "Estimated Accuracies");
	}

	// I011/600  Alert Messages
	if (cat->item011_fspec.code[3] & 0x10)
	{
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item011_600.head, CAT011_EL600, 3, code + k, "Alert Messages ");
		k += 3;
	}

	// I011/605  Tracks in Alert
	if (cat->item011_fspec.code[3] & 0x08)
	{
		int diff = k;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		k += 1;

		if (code[ k - 1 ] > 0)
		{
			if (checkOverloadFunc(k, sizeof(u_int16_t) * code[ k - 1 ], cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += (sizeof(u_int16_t) * code[ k - 1 ]);
		}
		setElementHeader(&cat->item011_605.head, CAT011_EL605, k - diff, code + diff, "Tracks in Alert");
	}

	// I011/605  Holdbar Status
	if (cat->item011_fspec.code[3] & 0x04)
	{
		int diff = k;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		k += 1;

		if (code[ k - 1 ] > 0)
		{
			if (checkOverloadFunc(k, sizeof(u_int16_t) * code [ k - 1 ], cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += (sizeof(u_int16_t) * code[ k - 1 ]);
		}
		setElementHeader(&cat->item011_610.head, CAT011_EL610, k - diff, code + diff, "Holdbar Status");
	}

	if (cat->item011_fspec.code[3] & 0x02)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		k += 1;
		if (code[ k - 1 ] > 0)                     // Есть данные поля SP
		{
			if (checkOverloadFunc(k, code[ k - 1 ] - 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}

			if (cat->item011_sp == NULL)
				cat->item011_sp = (Cat011specialPuposeField*)malloc(sizeof(Cat011specialPuposeField));

			if (cat->item011_sp)
			{
				// bzero(cat->item011_sp, sizeof(Cat011specialPuposeField));
				memset(cat->item011_sp, 0, sizeof(Cat011specialPuposeField));
				setElementHeader(&cat->item011_sp->head, CAT011_ELSP, code[ k - 1 ], code + k - 1, "I011 Special Purpose Field");
			}
		}
	}

	return asterix;
}

/**
 * @brief Запрос данных о содержимом FSPEC записи
 * @param cat - указатель структуру категории 011
 * @return - указатель на массив с байтами FSPEC записи
 */
u_int8_t* get011recordFspec(TCat011* cat, size_t* size)
{
	u_int8_t* ret = NULL;
	size_t    res = 0;

	if (cat == NULL)
	{
		*size = res;
		return ret;
	}

	if (cat->item011_fspec.code)
	{
		ret = cat->item011_fspec.code;
		res = cat->item011_fspec.code_size;
	}

	*size = res;
	return ret;
}

TCommonObmenData* get011HoldbarStatus(TCat011* cat)
{
	TCommonObmenData* ret = NULL;

	if (cat == NULL)
		return ret;

	if ((cat->item011_610.all_holdbars  > 0) &&
	    (cat->item011_610.holdbars))
	{
		ret = initObmenData(cat->item011_610.all_holdbars * sizeof(u_int16_t));
		if (ret)
			setObmenData(ret, cat->item011_610.holdbars, cat->item011_610.all_holdbars * sizeof(u_int16_t));
	}
	return ret;
}

int set011HoldbarStatus(TCat011* cat, TCommonObmenData* holdbar)
{
	unsigned char* data;
	int res = -1, ccc = 0;

	if ((cat == NULL) || (holdbar == NULL))
		return res;

	ccc = getObmenDataLenght(holdbar);
	data = (unsigned char*)getObmenData(holdbar);	// for Windows
	if ((ccc <= 0) || !data)
		return res;

	if (cat->item011_610.holdbars)
	{
		free(cat->item011_610.holdbars);
		cat->item011_610.holdbars = NULL;
	}

	cat->item011_610.all_holdbars = ccc / 2;
	cat->item011_610.holdbars     = (u_int16_t*)malloc(ccc);

	if (cat->item011_610.holdbars)
	{
		memcpy(cat->item011_610.holdbars, data, ccc);

		if (cat->item011_fspec.code)
			cat->item011_fspec.code[3] |= 0x04;

		res = 0;
	}
	return res;
}

TCommonObmenData* get011AlertTracks(TCat011* cat)
{
	TCommonObmenData* ret = NULL;

	if (cat == NULL)
		return ret;

	if ((cat->item011_605.all_track_in_alert > 0) &&
	    (cat->item011_605.fusion_track_num))
	{
		ret = initObmenData(cat->item011_605.all_track_in_alert * sizeof(u_int16_t));
		if (ret)
			setObmenData(ret, cat->item011_605.fusion_track_num, cat->item011_605.all_track_in_alert * sizeof(u_int16_t));
	}
	return ret;
}

int set011AlertTracks(TCat011* cat, TCommonObmenData* alert_tr)
{
	u_int8_t* data;
	int res = -1, ccc = 0;

	if ((cat == NULL) || (alert_tr == NULL))
		return res;

	ccc = getObmenDataLenght(alert_tr);
	data = (u_int8_t*)getObmenData(alert_tr);
	if ((ccc <= 0) || !data)
		return res;

	if (cat->item011_605.fusion_track_num)
	{
		free(cat->item011_605.fusion_track_num);
		cat->item011_605.fusion_track_num = NULL;
	}

	cat->item011_605.all_track_in_alert = ccc / 2;
	cat->item011_605.fusion_track_num   = (u_int16_t*)malloc(ccc);

	if (cat->item011_605.fusion_track_num)
	{
		memcpy(cat->item011_605.fusion_track_num, data, ccc);

		if (cat->item011_fspec.code)
			cat->item011_fspec.code[3] |= 0x08;

		res = 0;
	}
	return res;
}
