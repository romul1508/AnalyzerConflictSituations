/*
 *
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include "unistd.h"
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "cat_asterix.h"
#include "cat_010_asterix.h"

/**
 * @brief Установка значений по умолчанию для параметров структуры
 *        категории 010 Астерикс
 * @param cat - указатель на структуру категории
 * @return 0 - успешное выполнение, -1 отказ исполнения
 */
int set010catDefaultData(TCat010* cat)
{
	if (cat == NULL)
		return -1;

	cat->item010_010.sac_sic               = DEFAULT_U_INT16_CODE;
	cat->item010_000.message_type          = DEFAULT_U_INT8_CODE;
	cat->item010_161.track_number          = DEFAULT_U_INT16_CODE;
	cat->item010_060.m3a.board_number_code = DEFAULT_RBS_UVD_NUM_CODE;
	cat->item010_060.m3a.change            = DEFAULT_U_INT8_CODE;
	cat->item010_140.time_of_track         = DEFAULT_TIME_OF_TRACK;
	cat->item010_042.x                     = DEFAULT_CARTESIAN_POS;
	cat->item010_042.y                     = DEFAULT_CARTESIAN_POS;
	cat->item010_041.latitude              = DEFAULT_RADIAN_GEO_POS;
	cat->item010_041.longitude             = DEFAULT_RADIAN_GEO_POS;
	cat->item010_091.calc_g_height         = DEFAULT_ALTITUDE;
	cat->item010_090.fl_binary.altitude    = DEFAULT_ALTITUDE;
	cat->item010_202.vx                    = DEFAULT_VELOCITY;
	cat->item010_202.vy                    = DEFAULT_VELOCITY;
	cat->item010_210.ax                    = DEFAULT_ACCELERATION;
	cat->item010_210.ay                    = DEFAULT_ACCELERATION;
	cat->item010_270.length                = DEFAULT_U_INT8_CODE;
	cat->item010_270.width                 = DEFAULT_U_INT8_CODE;
	cat->item010_270.orientation           = DEFAULT_RADIAN_GEO_POS;
	return 0;
}

/**
 * @brief Выдача указателя на структуру категории 010 из
 *        обобщенной структуры Астерикс
 * @param cat - указатель на структуру Астерикс
 * @return указатель на структуру категории 010 Астерикс,
 *         при отказе значение NULL
 */
TCat010* get010category(TCatAsterix* cat)
{
	if (cat)
	{
		if (cat->cat_num == CODT_CAT_010)
			return &cat->category.c010;
		else
			return NULL;
	}
	else
		return NULL;
}

/**
 * @brief Распаковка поля SP категории 010 (по умолчанию)
 * @param code - указатель на массив с кодограммой
 * @param cnt - номер начального байта кодированных данных в кодограмме
 * @return - указатель на структуру с распакованными данными
 */
TSpData* decode010defaultSPdata(unsigned char* code, u_int64_t* cnt, u_int64_t code_size)
{
	int           zn       = 1;
	u_int64_t     k        = *cnt;
	unsigned char fspec[2] =
	{
		0
	}, b              = 0;
	unsigned short bb = 0;
	TSpData*       sp = NULL;

	if (code == NULL)
		return sp;

	sp = malloc(sizeof(TSpData));

	if (sp)
		bzero(sp, sizeof(TSpData));
	else
		return sp;

	if (checkOverloadFunc(k, 1, code_size) < 0)
	{
		free(sp);
		return NULL;
	}

	k += 1;                                            // size

	if (checkOverloadFunc(k, 1, code_size) < 0)
	{
		free(sp);
		return NULL;
	}
	fspec[1] = code[k];
	k       += 1;                                      // fspec1

//    if(fspec[1]&0x01)
//    {
//        fspec[0] = code[k];	k+=1;//fspec2
//    }
//    if(fspec[0]&0x01)
//    {
//        fprintf(stdout, "obmen[pid-%d] (ABN-%d) decode SP element: Illegal size fspec SP!!! %08X\n", getpid(), getTractNum( ctx ), fspec );
//        return;
//    }

	if (fspec[1] & 0x80)                               // OGP NRZ SP01
	{

		if (checkOverloadFunc(k, 1, code_size) < 0)
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
	if (fspec[1] & 0x40)                               // bort number NRZ SP02 [3 byte]
	{
		if (checkOverloadFunc(k, 3, code_size) < 0)
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
	if (fspec[1] & 0x20)                               // H_bar + Toplivo SP03
	{
		if (checkOverloadFunc(k, 3, code_size) < 0)
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

		bb = ((code[ k ] & 0x3F) << 8) | code[ k + 1];
		if (code[ k ] & 0x20)
		{
			bb |= 0xC000;
			bb  = ~(bb - 1);
			zn  = -1;
		}
		else
			zn = 1;
		sp->altitude_barometric = bb * CMR_UVD_HEIGTH * zn;
		k                      += 2;
		sp->interrogate_system  = IST_NRZ;
	}

	if (fspec[1] & 0x08)                               // reserv in flag
	{
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			free(sp);
			return NULL;
		}
		b  = code[k];
		k += 1;
		if (b & 0x02)
			sp->interrogate_system = IST_UVD;  // UVD prezent
	}

	if (fspec[1] & 0x04)                               // bort num from UVD SP04 UVD_Num[3 byte]
	{
		if (checkOverloadFunc(k, 3, code_size) < 0)
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
	if (fspec[1] & 0x02)                               // Hbar + Toplivo from UVD SP05
	{
		if (checkOverloadFunc(k, 3, code_size) < 0)
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

		bb = ((code[ k ] & 0x3F) << 8) | code[ k + 1];
		if (code[ k ] & 0x20)
		{
			bb |= 0xC000;
			bb  = ~(bb - 1);
			zn  = -1;
		}
		else
			zn = 1;
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
 * @brief Распаковка записи категории 010(ed.1.1 March 2007) формата АСТЕРИКС
 * @param ptr - указатель на данные с кодограммой АСТЕРИКС
 * @param current_pos - номер позиционного байта начала записи в кодограмме
 * @return - указатель на структуру категории Астерикс
 */
TCatAsterix* decode010catRecord(TCommonObmenData* codogramm, size_t* current_pos)
{
	unsigned char b = 0, fspec[FSPEC_SIZE_4_BYTE] =
	{
		0
	};
	unsigned char* code    = NULL;
	unsigned short bb      = 0;
	int            ccc     = 0, i;
	u_int64_t      k       = *current_pos, cdgm_size = 0;
	TCatAsterix*   asterix = NULL;
	TCat010*       cat     = NULL;

	if (codogramm == NULL)
	{
		fprintf(stderr, "proc[pid-%d] decode010catRecord null TCommonObmenData data ERROR\n", getpid());
		return asterix;
	}
	cdgm_size = getObmenDataLenght(codogramm);
	if (cdgm_size == 0)
	{
		fprintf(stderr, "proc[pid-%d] decode010catRecord null size data ERROR\n", getpid());
		return asterix;
	}

	if (checkOverloadFunc(k, cdgm_size, cdgm_size) < 0)
	{
		return asterix;
	}

	code = (unsigned char*) getObmenData(codogramm);

	if (code == NULL)
	{
		fprintf(stderr, "proc[pid-%d] decode010catRecord data array ERROR\n", getpid());
		return asterix;
	}

	asterix = createCatAsterixData(CODT_CAT_010);

	if (asterix)
	{
		cat = &asterix->category.c010;

		set010catDefaultData(cat);

	}
	else
	{
		fprintf(stderr, "proc[pid-%d] decodeCodogramm cat 010 create category ERROR\n", getpid());
		return asterix;
	}

	if (checkOverloadFunc(k, FSPEC_SIZE_4_BYTE, cdgm_size) < 0)
	{
		freeCatAsterixData(asterix);
		return NULL;
	}

	ccc = getCategoryFspecSize(code, k, cdgm_size);

	if ((ccc > 0) && (ccc <= FSPEC_SIZE_4_BYTE) && (k + ccc < cdgm_size))
	{
		if (cat->item010_fspec.code)
		{
			memset(cat->item010_fspec.code, 0, FSPEC_SIZE_4_BYTE);
			memcpy(cat->item010_fspec.code, code + k, ccc);
			cat->item010_fspec.code_size = ccc;
		}
		memcpy(fspec, code + k, ccc);
		k += ccc;
	}
	else
	{
		freeCatAsterixData(asterix);
		fprintf(stderr, "proc[pid-%d] decode010catRecord FSPEC ERROR\n", getpid());
		return NULL;
	}

	if (fspec[0] & 0x80)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item010_010.sac_sic = (code[ k ] << 8) | code[ k + 1 ];
		k                       += 2;
	}

	if (fspec[0] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		b = code[ k ];
		k++;
		switch (b)
		{
		case 1: cat->item010_000.message_type = MESSAGE_TYPE_TRACK;
			break;
		case 2: cat->item010_000.message_type = MESSAGE_TYPE_START_UPDATE_CIRCLE;
			break;
		case 3: cat->item010_000.message_type = MESSAGE_TYPE_PERIODIC_STATUS_MSG;
			break;
		case 4: cat->item010_000.message_type = MESSAGE_TYPE_EVENT_STATUS_MSG;
			break;
		}
	}

	if (fspec[0] & 0x20)
	{
		TargetReportDescriptor* trd = NULL;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		b = code[ k ];
		k++;

		trd = malloc(sizeof(TargetReportDescriptor));

		if (trd)
			bzero(trd, sizeof(TargetReportDescriptor));

		if (trd)
		{
			trd->source_type = b >> 5;
			if (b & 0x10)
				trd->differencial_correction = 1;
			if (b & 0x08)
				trd->chain1_2 = 1;
			if (b & 0x04)
				trd->ground_bit_set = 1;
			if (b & 0x02)
				trd->corrupted_reply = 1;
		}
		if (b & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}

			b = code[ k ];
			k++;

			if (trd)
			{
				if (b & 0x80)
					trd->simulated_target = 1;
				if (b & 0x40)
					trd->test_target = 1;
				if (b & 0x20)
					trd->rab = 1;
				trd->loop_status = (b & 0x18) >> 3;
				trd->source_type = (b & 0x06) >> 1;
			}

			if (b & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}

				b = code[ k ];
				k++;
				if (trd)
				{
					if (b & 0x80)
						trd->spi_set = 1;
				}
			}
		}
		cat->item010_020.track_descriptor = trd;
	}

	if (fspec[0] & 0x10)
	{
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		ccc                            = (code[ k ] << 16) | (code[ k + 1 ] << 8) | code[ k + 2 ];
		cat->item010_140.time_of_track = CMR_ASTX_TIME * ccc; // seconds
		k                             += 3;
	}
	if (fspec[0] & 0x08)
	{
		double ddd = 0.0, zn = 1.0;

		if (checkOverloadFunc(k, 8, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		for (i = 0; i < 2; i += 1)
		{
			uint32_t ccc = ((uint32_t)(code[k]) << 24) | (code[k + 1] << 16) | (code[k + 2] << 8) | code[k + 3];
			if (code[k] & 0x80)
			{
				zn  = -1.0;
				ccc = ~(ccc - 1);
			}
			else
				zn = 1.0;

			ddd = (zn * ccc * CMR_GEO_31) / DEG;

			if (i == 0)
				cat->item010_041.latitude = ddd;
			else
				cat->item010_041.longitude = ddd;

			k += 4;
		}
	}

	if (fspec[0] & 0x04)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		bb                             = (code[ k ] << 8) | code[ k + 1 ];
		cat->item010_040.polar.azimuth = (bb * CMR_ASTX_AZ) / DEG;

		bb                              = (code[ k + 2 ] << 8) | code[ k + 3 ];
		cat->item010_040.polar.distance = bb;      // meters
		k                              += 4;
	}

	if (fspec[0] & 0x02)
	{
		int zn = 1;

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

		cat->item010_042.x = zn * bb;                        // meters

		bb = (code[ k + 2 ] << 8) | code[ k + 3 ];
		if (code[ k + 2 ] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		else
			zn = 1;

		cat->item010_042.y = zn * bb;                        // meters
		k                 += 4;

	}

	if (fspec[1] & 0x80)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		bb                            = (code[ k ] << 8) | code[ k + 1 ];
		cat->item010_200.ground_speed = CMR_ASTX_SPEED * bb; // m/sec

		bb                           = (code[ k + 2 ] << 8) | code[ k + 3 ];
		cat->item010_200.track_angle = (CMR_ASTX_AZ * bb) / DEG;

		k += 4;
	}

	if (fspec[1] & 0x40)
	{
		int zn = 1;

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

		cat->item010_202.vx = zn * CMR_VXVY * bb;  // m/sec

		bb = (code[ k + 2 ] << 8) | code[ k + 3 ];
		if (code[ k + 2 ] & 0x80)
		{
			bb = ~(bb - 1);
			zn = -1;
		}
		else
			zn = 1;

		cat->item010_202.vy = zn * CMR_VXVY * bb;  // m/sec

		k += 4;
	}


	if (fspec[1] & 0x20)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item010_161.track_number = ((code[ k ] & 0x7f) << 8) | code[ k + 1 ];
		k                            += 2;
	}

	if (fspec[1] & 0x10)
	{
		TrackStatus010* ts = NULL;

		ts = malloc(sizeof(TrackStatus010));

		if (ts)
			bzero(ts, sizeof(TrackStatus010));
		// 1 octet
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		b  = code[k];
		k += 1;
		if (ts)
		{
			if (b & 0x80)
				ts->first_message = 1;
			else
				ts->confirm = 1;
			if (b & 0x40)
				ts->last_message = 1;
			if ((b & 0x20) || (b & 0x10))
				ts->coast_track = 1;
			if (b & 0x08)
				ts->horizontal_manoeuvre = 1;
			if (b & 0x04)
				ts->slant_correction = 1;
			if (b & 0x02)
				ts->smooth_position = 1;
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
			if (ts)
			{
				ts->movement_mode = b >> 6;
				ts->doubt         = (b & 0x38) >> 3;
				ts->merge         = (b & 0x06) >> 1;
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
				if (ts)
				{
					if (b & 0x80)
						ts->ghost_track = 1;
				}
			}                                  // 3 octet
		}                                          // 2 octet

		cat->item010_170.track_status = ts;
	}

	if (fspec[1] & 0x08)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item010_060.m3a.board_number_code = (code[ k ] << 8) | code[ k + 1 ];
		k                                     += 2;
	}

	if (fspec[1] & 0x04)
	{
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item010_220.target_address = (code[ k ] << 16) | (code[ k + 1] << 8) | code[k + 2];
		k                              += 3;
	}

	if (fspec[1] & 0x02)
	{
		if (checkOverloadFunc(k, 7, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		bzero(cat->item010_245.target_id, sizeof(cat->item010_245.target_id));
		cat->item010_245.target_id_src = code[k] >> 6;
		k                             += 1;
		convertIA5codeToAscii((unsigned char*) cat->item010_245.target_id, code, &k);
	}

	if (fspec[2] & 0x80)
	{
		int size = 0;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item010_250.mode_s_data_count = code[ k ];
		k++;

		if (cat->item010_250.mode_s_data_count > 0)
		{
			size = cat->item010_250.mode_s_data_count * sizeof(u_int64_t);
			if (checkOverloadFunc(k, size, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}

			cat->item010_250.mode_s_msg = malloc(size);

			if (cat->item010_250.mode_s_msg)
			{
				bzero(cat->item010_250.mode_s_msg, size);
				memcpy(cat->item010_250.mode_s_msg, code + k, size);
			}

			k += size;
		}
	}

	// I010/300  Vehicle Fleet Identification
	if (fspec[2] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item010_300.fleet_id = code[k];
		k                        += 1;
	}

	// I010/090  Flight Level in Binary Representation
	if (fspec[2] & 0x20)
	{
		int zn = 1;

		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		if (!(code[ k ] & 0x80))
			cat->item010_090.fl_binary.valid = 1;
		if (code[ k ] & 0x40)
			cat->item010_090.fl_binary.garbled = 1;

		bb = ((code[ k ] & 0x3F) << 8) | code[ k + 1];
		if (code[k] & 0x20)
		{
			bb |= 0xC000;
			zn  = -1;
			bb  = ~(bb - 1);
		}
		cat->item010_090.fl_binary.altitude = (double) (zn * bb * CMR_ASTX_H_25F); // m
		k                                  += 2;
	}
	// I010/091  calc H geo
	if (fspec[2] & 0x10)
	{
		int zn = 1;

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
		cat->item010_091.calc_g_height = (double) (bb * zn * CMR_ASTX_H_6_25F); // m
		k                             += 2;
	}
	// I010/270  VO size param
	if (fspec[2] & 0x08)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		b                       = code[k];
		k                      += 1;
		cat->item010_270.length = b >> 1;                                       // metr
		if (b & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			b                            = code[k];
			k                           += 1;
			cat->item010_270.orientation = ((b >> 1) * 2.81) / DEG;         // grad
			if (b & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				b                      = code[k];
				k                     += 1;
				cat->item010_270.width = b >> 1;                        // metr
			}
		}
	}
	// I010/550  System Status
	if (fspec[2] & 0x04)
	{
		SystemStatus* st = NULL;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		b  = code[k];
		k += 1;

		st = malloc(sizeof(SystemStatus));

		if (st)
		{
			bzero(st, sizeof(SystemStatus));
			st->operation_status = b >> 6;
			if (b & 0x20)
				st->overload = 1;
			if (b & 0x10)
				st->time_source_valid = 1;
			if (b & 0x08)
				st->diversity = 1;
			if (b & 0x04)
				st->test_target_fail = 1;
		}
		cat->item010_550.system_status = st;
	}
	// I010/310  Pre-programmed Message
	if (fspec[2] & 0x02)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		if (code[ k ] & 0x80)
			cat->item010_310.in_trouble = 1;
		cat->item010_310.work_programm_code = code[k] & 0x7F;
		k                                  += 1;
	}
	// I010/500  Standard Deviation of Position
	if (fspec[3] & 0x80)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		// CKO on X
		cat->item010_500.accuracy.x_component = code[k] * 0.25;
		k                                    += 1;
		// CKO on Y
		cat->item010_500.accuracy.y_component = code[k] * 0.25;
		k                                    += 1;
		// CKO on XY
		bb                                         = (code[k] << 8) | code[k + 1];
		k                                         += 2;
		cat->item010_500.accuracy.xy_cov_component = bb * 0.25;
	}

	// I010/280  Presence
	if (fspec[3] & 0x40)
	{
		int size = 0, i, zn = 1;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		cat->item010_280.presense_count = code[ k ];
		k                              += 1;

		if (checkOverloadFunc(k, cat->item010_280.presense_count * 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		if (cat->item010_280.presense_count > 0)
		{
			size = cat->item010_280.presense_count * sizeof(PolarPosition);

			cat->item010_280.presense = malloc(size);

			for (i = 0; i < cat->item010_280.presense_count; i++)
			{
				b  = code[ k ];
				k += 1;
				if (b & 0x80)
				{
					b  = ~(b - 1);
					zn = -1;
				}
				else
					zn = 1;
				if (cat->item010_280.presense)
					cat->item010_280.presense[i].distance = 1.0 * zn * b; // metr

				b  = code[ k ];
				k += 1;
				if (b & 0x80)
				{
					b  = ~(b - 1);
					zn = -1;
				}
				else
					zn = 1;
				if (cat->item010_280.presense)
					cat->item010_280.presense[i].azimuth = (zn * b * 0.15) / DEG;
			}
		}
	}

	// I010/131  Amplitude of Primary Plot
	if (fspec[3] & 0x20)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		cat->item010_131.amplitude = code[ k ];
		k                         += 1;
	}

	if (fspec[3] & 0x10)
	{
		int zn = 1;

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

		cat->item010_210.ax = zn * CMR_VXVY * b;   // m/sec^2

		b = code[ k + 1 ];
		if (code[ k + 1 ] & 0x80)
		{
			b  = ~(b - 1);
			zn = -1;
		}
		else
			zn = 1;

		cat->item010_210.ay = zn * CMR_VXVY * b;   // m/sec^2

		k += 2;
	}

	if (fspec[3] & 0x04)
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

			cat->item010_sp = malloc(sizeof(Cat010specialPuposeField));

			if (cat->item010_sp)
			{
				bzero(cat->item010_sp, sizeof(Cat010specialPuposeField));
				cat->item010_sp->el_sp = decode010defaultSPdata(code, &k, cdgm_size);
			}
		}
	}

	*current_pos = k;

	return asterix;
}

/**
 * @brief Упаковка поля SP в категории 010 (по умолчанию)
 * @param sp_elem - указатель на поле данных SP
 * @return - кодированная последовательность в байтах для укладки в кодограмму
 */

TCommonObmenData* pack010defaultSPdata(TSpData* sp)
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
 * @brief Упаковка записи категории 010 (ed.1.1 March 2007) Астерикс
 * @param cat - указатель на структуру категории 010
 * @return - указатель на структуру с кодированными данными записи категории Астерикс
 */
TCommonObmenData* pack010catRecord(TCat010* cat)
{
	unsigned char     b  = 0;
	unsigned short    bb = 0;
	int               ccc, i;
	double            ddd = 0;
	u_int8_t          code[DEFAULT_CODE_SIZE], fspec_pos = 0;
	size_t            cnt = 0;
	TCommonObmenData* msg = NULL;

	if (cat == NULL)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 010 ERROR - nothing function argument\n", getpid());
		return msg;
	}

	if (cat->item010_fspec.code == NULL)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 010 ERROR - fspec code not set\n", getpid());
		return msg;
	}

	if (cat->item010_010.sac_sic == 0)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 010 ERROR - sic sac data NULL\n", getpid());
		return msg;
	}

	switch (cat->item010_000.message_type)
	{
	case MESSAGE_TYPE_TRACK:
		b = cat->item010_000.message_type;
		if (cat->item010_020.track_descriptor == NULL)
		{
			fprintf(stderr, "proc[pid-%d] Pack cat 010 ERROR - target descriptor not present\n", getpid());
			return msg;
		}
		break;

	case MESSAGE_TYPE_START_UPDATE_CIRCLE: b = cat->item010_000.message_type;
		break;
	case MESSAGE_TYPE_PERIODIC_STATUS_MSG: b = cat->item010_000.message_type;
		break;
	case MESSAGE_TYPE_EVENT_STATUS_MSG: b = cat->item010_000.message_type;
		break;
	default:
		b = 0;
		break;
	}

	if (b == 0)
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 010 ERROR - message type unknown\n", getpid());
		return msg;
	}

	if ((cat->item010_140.time_of_track < 0) &&
	    (cat->item010_140.time_of_track >= DEFAULT_TIME_OF_TRACK))
	{
		fprintf(stderr, "proc[pid-%d] Pack cat 010 ERROR - time of Day not present\n", getpid());
		return msg;
	}

	bzero(code, DEFAULT_CODE_SIZE);

	cnt += FSPEC_SIZE_4_BYTE;

	// I010/010 sic_sac ident code
	// SAC = 0 fixed value
	if (cat->item010_fspec.code[fspec_pos] & 0x80)
	{
		bb        = cat->item010_010.sac_sic;
		code[cnt] = bb >> 8;
		cnt      += 1;
		code[cnt] = bb;
		cnt      += 1;
	}

	// I010/000 Message type
	if (cat->item010_fspec.code[fspec_pos] & 0x40)
	{
		code[cnt] = cat->item010_000.message_type;
		cnt      += 1;
	}

	if (cat->item010_000.message_type == MESSAGE_TYPE_TRACK)
	{
		// I010/020  Target Report Descriptor
		if ((cat->item010_020.track_descriptor) && // Target Report Descriptor
		    (cat->item010_fspec.code[ fspec_pos] & 0x20))
		{
			u_int8_t octet[TRACK_STATUS_EXTENT] =
			{
				0
			};
			TargetReportDescriptor* trd = NULL;
			trd = (TargetReportDescriptor*) cat->item010_020.track_descriptor;

			octet[TRACK_STATUS_EXTENT - 1] = (trd->source_type & 0x07) << 5;
			if (trd->differencial_correction == 1)
				octet[TRACK_STATUS_EXTENT - 1] |= 0x10;
			if (trd->chain1_2 == 1)
				octet[TRACK_STATUS_EXTENT - 1] |= 0x08;
			if (trd->ground_bit_set == 1)
				octet[TRACK_STATUS_EXTENT - 1] |= 0x04;
			if (trd->corrupted_reply == 1)
				octet[TRACK_STATUS_EXTENT - 1] |= 0x02;

			if (trd->simulated_target == 1)
				octet[TRACK_STATUS_EXTENT - 2] |= 0x80;
			if (trd->test_target == 1)
				octet[TRACK_STATUS_EXTENT - 2] |= 0x40;
			if (trd->rab == 1)
				octet[TRACK_STATUS_EXTENT - 2] |= 0x20;
			octet[TRACK_STATUS_EXTENT - 2] |= ((trd->loop_status & 0x03) << 3);
			octet[TRACK_STATUS_EXTENT - 2] |= ((trd->source_type & 0x03) << 1);

			if (octet[TRACK_STATUS_EXTENT - 2] > 0)
				octet[TRACK_STATUS_EXTENT - 1] |= 0x01;

			octet[TRACK_STATUS_EXTENT - 3] = 0;
			if (trd->spi_set == 1)
				octet[TRACK_STATUS_EXTENT - 3] |= 0x80;

			if (octet[TRACK_STATUS_EXTENT - 3] > 0)
			{
				octet[TRACK_STATUS_EXTENT - 2] |= 0x01;
				octet[TRACK_STATUS_EXTENT - 1] |= 0x01;
			}

			code[ cnt ] = octet[TRACK_STATUS_EXTENT - 1];
			cnt        += 1;
			i           = 1;
			while (code[ cnt - 1 ] & 0x01)
			{
				code[ cnt ] = octet[TRACK_STATUS_EXTENT - 1 - i];
				cnt        += 1;
				i          += 1;
			}
		}
	}
	else
	{
		if (cat->item010_fspec.code[ fspec_pos] & 0x20)
			cat->item010_fspec.code[ fspec_pos ] &= ~0x20;
	}
	// I010/140  time message
	// time on server
	// in sec from 00:00
	if (cat->item010_fspec.code[fspec_pos] & 0x10)
	{
		ccc       = (int) (cat->item010_140.time_of_track / CMR_ASTX_TIME);
		code[cnt] = ccc >> 16;
		cnt      += 1;
		code[cnt] = ccc >> 8;
		cnt      += 1;
		code[cnt] = ccc;
		cnt      += 1;
	}

	if (cat->item010_000.message_type == MESSAGE_TYPE_TRACK)
	{
		// I011/041  WGS-84 coord
		if (cat->item010_fspec.code[fspec_pos] & 0x08)
		{
			int diff = cnt;
			if ((cat->item010_041.latitude != DEFAULT_RADIAN_GEO_POS) ||
			    (cat->item010_041.longitude != DEFAULT_RADIAN_GEO_POS))
			{
				for (i = 0; i < 2; i++)
				{
					if (i == 0)
						ddd = cat->item010_041.latitude;
					if (i == 1)
						ddd = cat->item010_041.longitude;
					ccc = (int) (ddd / (CMR_GEO_31 * M_PI / 180.));
//                    ccc = (int)((ddd*DEG)/CMR_GEO_31);
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
				cat->item010_fspec.code[fspec_pos] &= ~0x08;
		}

		// I010/040  Polar coord
		if (cat->item010_fspec.code[fspec_pos] & 0x04)
		{
			int diff = cnt;

			if ((cat->item010_040.polar.azimuth != DEFAULT_RADIAN_AZIMUTH_POS) ||
			    (cat->item010_040.polar.distance != DEFAULT_CARTESIAN_POS))
			{
				bb              = (unsigned short) ((cat->item010_040.polar.azimuth * DEG) / CMR_ASTX_AZ + 0.5);
				code[ cnt ]     = bb >> 8;
				code[ cnt + 1 ] = bb;

				bb              = cat->item010_040.polar.distance; // meters
				code[ cnt + 2 ] = bb >> 8;
				code[ cnt + 3 ] = bb;
				cnt            += 4;
			}
			if ((cnt - diff) == 0)
				cat->item010_fspec.code[fspec_pos] &= ~0x04;
		}
		// I010/042  XY coord
		if (cat->item010_fspec.code[fspec_pos] & 0x02)
		{
			int diff = cnt;

			if ((cat->item010_042.x != DEFAULT_CARTESIAN_POS) ||
			    (cat->item010_042.y != DEFAULT_CARTESIAN_POS))
			{
				for (i = 0; i < 2; i++)
				{
					if (i == 0)
						ccc = (int) (cat->item010_042.x);
					if (i == 1)
						ccc = (int) (cat->item010_042.y);

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
				cat->item010_fspec.code[fspec_pos] &= ~0x02;
		}
		fspec_pos = 1;
		// I010/200  speed, course
		if (cat->item010_fspec.code[fspec_pos] & 0x80)
		{
			int diff = cnt;

			if ((cat->item010_200.ground_speed != DEFAULT_VELOCITY) ||
			    (cat->item010_200.track_angle != DEFAULT_RADIAN_AZIMUTH_POS))
			{
				bb              = (unsigned short) (cat->item010_200.ground_speed / CMR_ASTX_SPEED);
				code[ cnt ]     = bb >> 8;
				code[ cnt + 1 ] = bb;

				bb              = (unsigned short) ((cat->item010_200.track_angle * DEG) / CMR_ASTX_AZ);
				code[ cnt + 2 ] = bb >> 8;
				code[ cnt + 3 ] = bb;

				cnt += 4;
			}

			if ((cnt - diff) == 0)
				cat->item010_fspec.code[fspec_pos] &= ~0x80;
		}
		// I010/202  speed vx vy
		if (cat->item010_fspec.code[fspec_pos] & 0x40)
		{
			int diff = cnt;

			if ((cat->item010_202.vx != DEFAULT_VELOCITY) ||
			    (cat->item010_202.vy != DEFAULT_VELOCITY))
			{
				for (i = 0; i < 2; i++)
				{
					if (i == 0)
						ccc = (int) (cat->item010_202.vx / CMR_VXVY);
					if (i == 1)
						ccc = (int) (cat->item010_202.vy / CMR_VXVY);
					code[cnt] = ccc >> 8;
					cnt      += 1;
					code[cnt] = ccc;
					cnt      += 1;
				}
			}

			if ((cnt - diff) == 0)
				cat->item010_fspec.code[fspec_pos] &= ~0x40;
		}
		// I010/161  Track Number
		if (cat->item010_fspec.code[fspec_pos] & 0x20)
		{
			if (cat->item010_161.track_number != DEFAULT_U_INT16_CODE)
			{
				bb        = cat->item010_161.track_number;
				code[cnt] = bb >> 8;
				cnt      += 1;
				code[cnt] = bb;
				cnt      += 1;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x20;
		}
		// I011/170  Track Status only + I011/161  Track Number
		if (cat->item010_fspec.code[fspec_pos] & 0x10)
		{
			if ((cat->item010_170.track_status) && (cat->item010_fspec.code[fspec_pos] & 0x20))
			{
				u_int8_t octet[TRACK_STATUS_EXTENT] =
				{
					0
				};
				TrackStatus010* ts = (TrackStatus010*) cat->item010_170.track_status;

				if (ts)
				{
					// 1 octet
					if (ts->first_message == 1)
						octet[TRACK_STATUS_EXTENT - 1] |= 0x80;
					if (ts->last_message == 1)
						octet[TRACK_STATUS_EXTENT - 1] |= 0x40;
					if (ts->coast_track == 1)
						octet[TRACK_STATUS_EXTENT - 1] |= 0x10;
					if (ts->horizontal_manoeuvre == 1)
						octet[TRACK_STATUS_EXTENT - 1] |= 0x08;
					if (ts->slant_correction == 1)
						octet[TRACK_STATUS_EXTENT - 1] |= 0x04;
					if (ts->smooth_position == 1)
						octet[TRACK_STATUS_EXTENT - 1] |= 0x02;
					// 2 octet
					if (ts->movement_mode > 0)
						octet[TRACK_STATUS_EXTENT - 2] |= ((ts->movement_mode & 0x03) << 6);
					if (ts->doubt > 0)
						octet[TRACK_STATUS_EXTENT - 2] |= ((ts->doubt & 0x07) << 3);
					if (ts->merge > 0)
						octet[TRACK_STATUS_EXTENT - 2] |= ((ts->merge & 0x03) << 1);
					if (octet[TRACK_STATUS_EXTENT - 2] > 0)
					{
						octet[TRACK_STATUS_EXTENT - 1] |= 0x01;
					}
					// 3 octet
					if (ts->ghost_track == 1)
						octet[TRACK_STATUS_EXTENT - 3] |= 0x80;
					if (octet[TRACK_STATUS_EXTENT - 3] > 0)
					{
						octet[TRACK_STATUS_EXTENT - 2] |= 0x01;
						octet[TRACK_STATUS_EXTENT - 1] |= 0x01;
					}

					code[ cnt ] = octet[TRACK_STATUS_EXTENT - 1];
					cnt        += 1;
					i           = 1;
					while (code[ cnt - 1 ] & 0x01)
					{
						code[ cnt ] = octet[TRACK_STATUS_EXTENT - 1 - i];
						cnt        += 1;
						i          += 1;
					}
				}
				else
				{
					cat->item010_fspec.code[fspec_pos] &= ~0x10;
				}
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x10;
		}
		// I010/060  Mode 3A
		if (cat->item010_fspec.code[fspec_pos] & 0x08)
		{
			if (cat->item010_060.m3a.board_number_code > DEFAULT_RBS_UVD_NUM_CODE)
			{
				bb        = cat->item010_060.m3a.board_number_code;
				code[cnt] = bb >> 8;
				cnt      += 1;
				code[cnt] = bb;
				cnt      += 1;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x08;
		}
		// I010/220  Target Address
		if (cat->item010_fspec.code[fspec_pos] & 0x04)
		{
			if ((cat->item010_220.target_address != DEFAULT_U_INT32_CODE) &&
			    (cat->item010_220.target_address > 0))
			{
				ccc       = cat->item010_220.target_address;
				code[cnt] = ccc >> 16;
				cnt      += 1;
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x04;
		}
		// I010/245  Ident VO IA-5
		if (cat->item010_fspec.code[fspec_pos] & 0x02)
		{
			u_int64_t cnt_pos = 0;
			if (strlen(cat->item010_245.target_id) > 0)
			{
				b = 0;
				switch (cat->item010_245.target_id_src)
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
				convertAsciiToIA5code(code, (unsigned char*) cat->item010_245.target_id, &cnt_pos);
				cnt = cnt_pos;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x02;
		}
		// I010/250  Mode S MB Data
		fspec_pos = 2;
		if (cat->item010_fspec.code[fspec_pos] & 0x080)
		{
			if ((cat->item010_250.mode_s_data_count != DEFAULT_U_INT8_CODE) &&
			    (cat->item010_250.mode_s_data_count > 0) &&
			    (cat->item010_250.mode_s_msg))
			{
				int size = cat->item010_250.mode_s_data_count * sizeof(u_int64_t);

				if ((cnt + size) < DEFAULT_CODE_SIZE)
				{
					code[ cnt ] = cat->item010_250.mode_s_data_count;
					cnt        += 1;
					for (i = 0; i < cat->item010_250.mode_s_data_count; i++)
					{
						memcpy(&code[cnt], &cat->item010_250.mode_s_msg[i], sizeof(u_int64_t));
						cnt += 8;
					}
				}
				else
				{
					cat->item010_fspec.code[fspec_pos] &= ~0x80;
				}
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x80;
		}
		// I010/300  Vehicle Fleet Identification
		if (cat->item010_fspec.code[fspec_pos] & 0x40)
		{
			if (cat->item010_300.fleet_id != DEFAULT_U_INT8_CODE)
			{
				code[cnt] = cat->item010_300.fleet_id;
				cnt      += 1;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x40;
		}
		// I010/090  Flight Level in Binary Representation
		if (cat->item010_fspec.code[fspec_pos] & 0x20)
		{
			if (cat->item010_090.fl_binary.altitude != DEFAULT_ALTITUDE)
			{
				code[ cnt ] = 0;
				if (cat->item010_090.fl_binary.valid == 0)
					code[ cnt ] |= 0x80;
				if (cat->item010_090.fl_binary.garbled == 1)
					code[ cnt ] |= 0x80;

				ccc        = (int) (cat->item010_090.fl_binary.altitude / CMR_ASTX_H_25F); // m
				code[cnt] |= ((ccc >> 8) & 0x3F);
				cnt       += 1;
				code[cnt]  = ccc;
				cnt       += 1;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x20;
		}
		// I010/091  calc H geo
		if (cat->item010_fspec.code[fspec_pos] & 0x10)
		{
			if (cat->item010_091.calc_g_height != DEFAULT_ALTITUDE)
			{
				ccc       = (int) (cat->item010_091.calc_g_height / CMR_ASTX_H_6_25F);     // m
				code[cnt] = ccc >> 8;
				cnt      += 1;
				code[cnt] = ccc;
				cnt      += 1;
			}
			else
				cat->item010_fspec.code[1] &= ~0x10;
		}
		// I010/270  VO size param
		if (cat->item010_fspec.code[fspec_pos] & 0x08)
		{
			if ((cat->item010_270.length != DEFAULT_U_INT8_CODE) ||
			    (cat->item010_270.width != DEFAULT_U_INT8_CODE) ||
			    (cat->item010_270.orientation != DEFAULT_RADIAN_GEO_POS))
			{
				TReturnOneInt ret = packTargetSizeAndOrientation(cat->item010_270.length,
				                                                 cat->item010_270.width,
				                                                 cat->item010_270.orientation);
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
				else
				{
					cat->item010_fspec.code[fspec_pos] &= ~0x08;
				}
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x08;
		}
	}
	else
	{
		cat->item010_fspec.code[0] &= ~0x2f;
		cat->item010_fspec.code[1]  = 0;
		cat->item010_fspec.code[2] &= ~0xfa;
	}

	if ((cat->item010_000.message_type == MESSAGE_TYPE_PERIODIC_STATUS_MSG) ||
	    (cat->item010_000.message_type == MESSAGE_TYPE_EVENT_STATUS_MSG) ||
	    (cat->item010_000.message_type == MESSAGE_TYPE_START_UPDATE_CIRCLE))
	{
		// I010/550  System Status
		fspec_pos = 2;
		if (cat->item010_fspec.code[fspec_pos] & 0x04)
		{
			if (cat->item010_550.system_status)
			{
				SystemStatus* st = (SystemStatus*) cat->item010_550.system_status;
				if (st)
				{
					code[cnt] = (st->operation_status & 0x03) << 6;
					if (st->overload == 1)
						code[cnt] |= 0x20;
					if (st->time_source_valid == 1)
						code[cnt] |= 0x10;
					if (st->diversity == 1)
						code[cnt] |= 0x08;
					if (st->test_target_fail == 1)
						code[cnt] |= 0x04;
					cnt += 1;
				}
				else
					cat->item010_fspec.code[fspec_pos] &= ~0x04;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x04;
		}
	}
	else
		cat->item010_fspec.code[fspec_pos] &= ~0x04;

	if (cat->item010_000.message_type == MESSAGE_TYPE_TRACK)
	{
		// I010/310 Pre-programmed Message
		if (cat->item010_fspec.code[fspec_pos] & 0x02)
		{
			if (cat->item010_310.work_programm_code != DEFAULT_U_INT8_CODE)
			{
				code[cnt] = cat->item010_310.work_programm_code & 0x7f;
				if (cat->item010_310.in_trouble == 1)
					code[cnt] |= 0x80;
				cnt += 1;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x02;
		}
		// I010/500 accuracy
		fspec_pos = 3;
		if (cat->item010_fspec.code[fspec_pos] & 0x80)
		{
			if ((cat->item010_500.accuracy.x_component > 0) ||
			    (cat->item010_500.accuracy.y_component > 0) ||
			    (cat->item010_500.accuracy.xy_cov_component > 0))
			{
				code[ cnt ] = cat->item010_500.accuracy.x_component / 0.25;
				cnt        += 1;
				code[ cnt ] = cat->item010_500.accuracy.y_component / 0.25;
				cnt        += 1;
				bb          = cat->item010_500.accuracy.xy_cov_component / 0.25;
				code[cnt]   = bb >> 8;
				cnt        += 1;
				code[cnt]   = bb;
				cnt        += 1;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x80;
		}
		// I010/280 Presence
		if (cat->item010_fspec.code[fspec_pos] & 0x40)
		{
			if ((cat->item010_280.presense_count != DEFAULT_U_INT8_CODE) &&
			    (cat->item010_280.presense_count > 0) &&
			    (cat->item010_280.presense))
			{

				code[cnt] = cat->item010_280.presense_count;
				cnt      += 1;
				for (i = 0; i < cat->item010_280.presense_count; i++)
				{
					ccc         = (int) cat->item010_280.presense[i].distance;
					code[ cnt ] = ccc; // metr
					cnt        += 1;
					ccc         = (int) (cat->item010_280.presense[i].azimuth / (0.15 * M_PI / 180.0));
					code[ cnt ] = ccc;
					cnt        += 1;
				}
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x40;
		}
		// I010/131  Amplitude of Primary Plot
		if (cat->item010_fspec.code[fspec_pos] & 0x20)
		{
			if (cat->item010_131.amplitude > 0)
			{
				code[cnt] = cat->item010_131.amplitude;
				cnt      += 1;
			}
			else
				cat->item010_fspec.code[fspec_pos] &= ~0x20;
		}
		// I010/210  speed ax ay
		if (cat->item010_fspec.code[fspec_pos] & 0x10)
		{
			int diff = cnt;

			if ((cat->item010_210.ax != DEFAULT_ACCELERATION) ||
			    (cat->item010_210.ay != DEFAULT_ACCELERATION))
			{
				for (i = 0; i < 2; i++)
				{
					if (i == 0)
						ccc = (int) (cat->item010_210.ax / CMR_VXVY);
					if (i == 1)
						ccc = (int) (cat->item010_210.ay / CMR_VXVY);
					code[cnt] = ccc;
					cnt      += 1;
				}
			}

			if ((cnt - diff) == 0)
				cat->item010_fspec.code[fspec_pos] &= ~0x10;
		}
		// I010/SP
		if (cat->item010_fspec.code[fspec_pos] & 0x04)
		{
			int diff = cnt;

			if (cat->item010_sp)
			{
				TCommonObmenData* sp_elem = NULL;
				unsigned char*    p       = NULL;

				// Если нет упакованных данных от клиента, упаковываются
				// данные по умолчанию
				if (cat->item010_sp->head.code_size == 0)
				{
					sp_elem = pack010defaultSPdata(cat->item010_sp->el_sp);
					if (sp_elem)
					{
						ccc = getObmenDataLenght(sp_elem);
						if (ccc > 0)
							p = (unsigned char*) getObmenData(sp_elem);
					}
				}
				else
				{
					ccc = cat->item010_sp->head.code_size;
					if (cat->item010_sp->head.code)
						p = (unsigned char*) cat->item010_sp->head.code;
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
				cat->item010_fspec.code[fspec_pos] &= ~0x04;
		}
	}
	else
	{
		cat->item010_fspec.code[2] &= ~0x02;
		cat->item010_fspec.code[3] &= ~0xf4;
	}

	if (cnt > FSPEC_SIZE_4_BYTE)                       // уложены данные
	{
		for (i = FSPEC_SIZE_4_BYTE - 1; i >= 0; i--)
		{
			if (cat->item010_fspec.code[i] > 0)
			{
				switch (i)
				{
				case 3:
					code[0] = cat->item010_fspec.code[0] | 0x01;
					code[1] = cat->item010_fspec.code[1] | 0x01;
					code[2] = cat->item010_fspec.code[2] | 0x01;
					code[3] = cat->item010_fspec.code[3];
					ccc     = 4;
					break;
				case 2:
					code[0] = cat->item010_fspec.code[0] | 0x01;
					code[1] = cat->item010_fspec.code[1] | 0x01;
					code[2] = cat->item010_fspec.code[2];
					ccc     = 3;
					break;
				case 1:
					code[0] = cat->item010_fspec.code[0] | 0x01;
					code[1] = cat->item010_fspec.code[1];
					ccc     = 2;
					break;
				case 0:
					code[0] = cat->item010_fspec.code[0];
					ccc     = 1;
					break;
				}
				break;
			}
		}

		cnt -= (FSPEC_SIZE_4_BYTE - 1 - i);
		memmove(code + ccc, code + FSPEC_SIZE_4_BYTE, cnt);

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
 * @param cat_lib - указатель на структуру категории 010
 * @param type_message - тип сообщения
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set010MessageType(TCat010* cat, u_int8_t type_message)
{
	int res = -1;

	if ((cat == NULL) || (type_message == 0))
		return res;

	if (cat)
	{
		cat->item010_000.message_type = type_message;
		if (cat->item010_fspec.code)
			cat->item010_fspec.code[0] |= 0x40;
		res = 0;
	}
	return res;
}

/**
 * @brief Запрос типа сообщения из структуры категории 010
 * @param cat_lib - указатель на структуру категории 010
 * @return  - структура данных возврата:
 *         переменная val - содержит значение типа сообщения
 */
TReturnOneInt get010MessageType(TCat010* cat)
{
	TReturnOneInt ret;

	ret.update = 0;
	ret.val    = 0;

	if (cat == NULL)
		return ret;

	if (cat)
	{
		if (cat->item010_000.message_type < DEFAULT_U_INT8_CODE)
		{
			ret.update = 1;
			ret.val    = cat->item010_000.message_type;
		}
	}
	return ret;
}

/**
 * @brief Запись типа выполняемого задания в структуру категории
 * @param cat_lib - указатель на структуру категории 010
 * @param type_mission  - тип выполняемого задания
 * @param trouble       - признак препятствия исполнению задания
 *                        0 - препятствий нет
 *                        1 - есть проблемы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set010PreprogrammedMessage(TCat010* cat, u_int8_t type_mission, u_int8_t trouble)
{
	int res = -1;

	if (cat == NULL)
		return res;

	if (cat)
	{
		cat->item010_310.work_programm_code = type_mission;
		cat->item010_310.in_trouble         = trouble;
		if (cat->item010_fspec.code)
			cat->item010_fspec.code[2] |= 0x02;
		res = 0;
	}
	return res;
}

/**
 * @brief Запрос типа выполняемого задания из структуры категории 010
 * @param cat_lib - указатель на структуру категории 010
 * @return  - структура данных возврата:
 *         переменная val1 - содержит значение типа выполняемого задания
 *         переменная val2 - содержит признак препятствия исполнению задания
 *                              0 - препятствий нет
 *                              1 - есть проблемы
 */
TReturnPairInt get010PreprogrammedMessage(TCat010* cat)
{
	TReturnPairInt ret;

	ret.update = 0;
	ret.val1   = 0;
	ret.val2   = 0;

	if (cat == NULL)
		return ret;

	if (cat)
	{
		if (cat->item010_310.work_programm_code < DEFAULT_U_INT8_CODE)
		{
			ret.update = 1;
			ret.val1   = cat->item010_310.work_programm_code;
			ret.val2   = cat->item010_310.in_trouble;
		}
	}
	return ret;
}

/**
 * @brief Запись данных описателя цели в структуру категории
 * @param cat - указатель на структуру категории 010
 * @param target_descriptor  - указатель на структуру описателя цели
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set010TargetReportDescriptor(TCat010* cat, TargetReportDescriptor* target_descriptor)
{
	int res = -1;

	if ((cat == NULL) || (target_descriptor == NULL))
		return res;

	if ((cat) && (target_descriptor))
	{
		if (cat->item010_020.track_descriptor == NULL)
			cat->item010_020.track_descriptor = malloc(sizeof(TargetReportDescriptor));

		if (cat->item010_020.track_descriptor)
		{
			memcpy(cat->item010_020.track_descriptor, target_descriptor, sizeof(TargetReportDescriptor));

			if (cat->item010_fspec.code)
				cat->item010_fspec.code[0] |= 0x20;

			res = 0;
		}
	}
	return res;
}

/**
 * @brief Запись данных статуса цели в структуру категории
 * @param cat - указатель на структуру категории 010
 * @param trs_tatus  - указатель на структуру статуса цели
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set010TrackStatus(TCat010* cat, TrackStatus010* trs_tatus)
{
	int res = -1;

	if ((cat == NULL) || (trs_tatus == NULL))
		return res;

	if ((cat) && (trs_tatus))
	{
		if (cat->item010_170.track_status == NULL)
			cat->item010_170.track_status = (TrackStatus010*) malloc(sizeof(TrackStatus010));

		if (cat->item010_170.track_status)
		{
			memcpy(cat->item010_170.track_status, trs_tatus, sizeof(TrackStatus010));

			if (cat->item010_fspec.code)
				cat->item010_fspec.code[1] |= 0x10;

			res = 0;
		}
	}
	return res;
}

/**
 * @brief Запись "сырых" данных полученных от приемника Mode S в структуру категории
 * @param cat - указатель на структуру категории 010
 * @param mbDataMass  - указатель данные с сообщениями Mode S
 *                      (содержимое заполняется пользователем в формате указанном в
 *                       стандарта категории 010)
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set010ModeSMBdata(TCat010* cat, TCommonObmenData* mbDataMass)
{
	int res = -1, ccc = 0;


	if ((cat == NULL) || (mbDataMass == NULL))
		return res;

	if ((cat) && (mbDataMass))
	{
		u_int8_t* mds_data;
		ccc = getObmenDataLenght(mbDataMass);
		mds_data = (u_int8_t*)getObmenData(mbDataMass);  // for windows
		if ((ccc > 0) && (mds_data))
		{
			if (cat->item010_250.mode_s_msg)
			{
				free(cat->item010_250.mode_s_msg);
				cat->item010_250.mode_s_msg = NULL;
			}

			cat->item010_250.mode_s_data_count = ccc / 8;
			cat->item010_250.mode_s_msg        = malloc(ccc);

			if (cat->item010_250.mode_s_msg)
			{
				memcpy(cat->item010_250.mode_s_msg, mds_data, ccc);

				if (cat->item010_fspec.code)
					cat->item010_fspec.code[2] |= 0x80;

				res = 0;
			}
		}
	}
	return res;
}

/**
 * @brief Запись данных о состоянии системы обработки РЛИ в структуру категории
 * @param cat - указатель на структуру категории 010
 * @param sys_stat  - указатель на структуру технического состояния системы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set010systemStatus(TCat010* cat, SystemStatus* sys_stat)
{
	int res = -1;


	if ((cat == NULL) || (sys_stat == NULL))
		return res;

	if ((cat) && (sys_stat))
	{
		if (cat->item010_550.system_status == NULL)
			cat->item010_550.system_status = (SystemStatus*) malloc(sizeof(SystemStatus));

		if (cat->item010_550.system_status)
		{
			memcpy(cat->item010_550.system_status, sys_stat, sizeof(SystemStatus));
			res = 0;
			if (cat->item010_fspec.code)
				cat->item010_fspec.code[2] |= 0x04;
		}
	}
	return res;
}

/**
 * @brief Запись данных об амплитуде эха координатной точки в структуру категории
 * @param cat - указатель на структуру категории 010
 * @param plot_amplitude  - значение амплитуды эха координатной точки
 *                          передается в  у.е. от 1 .... 255.
 *                          0 - уровень собственных шумов приемника
 *                          255 - макимум сигнала на выходе приемника
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set010plotAmplitude(TCat010* cat, u_int8_t plot_amplitude)
{
	int res = -1;

	if ((cat == NULL) || (plot_amplitude == 0))
		return res;

	if (cat)
	{
		cat->item010_131.amplitude = plot_amplitude;
		if (cat->item010_fspec.code)
			cat->item010_fspec.code[3] |= 0x20;
		res = 0;
	}
	return res;
}

/**
 * @brief Выборка данных описателя цели из структуры категории
 * @param cat - указатель на структуру категории 010
 * @return - структура описателя цели, NULL - данные отсутствуют
 */
TargetReportDescriptor* get010TargetReportDescriptor(TCat010* cat)
{

	if (cat == NULL)
		return NULL;

	if (cat->item010_020.track_descriptor)
	{
		return (TargetReportDescriptor*) cat->item010_020.track_descriptor;
	}
	return NULL;
}

/**
 * @brief Выборка данных статуса трассы из структуры категории
 * @param cat - указатель на структуру категории 010
 * @return - указатель на структуру статуса трассы, NULL - данные отсутствуют
 */
TrackStatus010* get010TrackStatus(TCat010* cat)
{

	if (cat == NULL)
		return NULL;

	if (cat->item010_170.track_status)
	{
		return (TrackStatus010*) cat->item010_170.track_status;
	}
	return NULL;
}

/**
 * @brief Запрос данных о состоянии системы обработки РЛИ из структуры категории
 * @param cat - указатель на структуру категории 010
 * @return - указатель на структуру технического состояния системы
 *           NULL - если данные отсутствуют
 */
SystemStatus* get010systemStatus(TCat010* cat)
{

	if (cat == NULL)
		return NULL;

	if (cat->item010_550.system_status)
	{
		return (SystemStatus* ) cat->item010_550.system_status;
	}
	return NULL;
}

/**
 * @brief Запрос данных об амплитуде эха координатной точки из структуры категории
 * @param cat - указатель на структуру категории 010
 * @return  - структура данных возврата:
 *         переменная val1 - значение амплитуды эха координатной точки
 *                           передается в  у.е. от 0 .... 255.
 *                           0 - уровень собственных шумов приемника
 *                           255 - макимум сигнала на выходе приемника
 */
TReturnOneInt get010plotAmplitude(TCat010* cat)
{
	TReturnOneInt ret;

	ret.update = 0;
	ret.val    = 0;

	if (cat == NULL)
		return ret;

	if (cat)
	{
		if (cat->item010_131.amplitude > 0)
		{
			ret.update = 1;
			ret.val    = cat->item010_131.amplitude;
		}
	}
	return ret;
}

/**
 * @brief Запрос данных о содержимом FSPEC записи
 * @param cat - указатель структуру категории 010
 * @return - указатель на массив с байтами FSPEC записи
 */
u_int8_t* get010recordFspec(TCat010* cat, size_t* size)
{
	u_int8_t* ret = NULL;
	size_t    res = 0;

	if (cat == NULL)
	{
		*size = res;
		return ret;
	}

	if (cat->item010_fspec.code)
	{
		ret = cat->item010_fspec.code;
		res = cat->item010_fspec.code_size;
	}

	*size = res;
	return ret;
}

/**
 * @brief Запись "сырых" данных полученных от приемника Mode S в структуру категории
 * @param cat - указатель на структуру категории 010
 * @return - указатель на данные с сообщениями Mode S
 *           (содержимое разбирается пользователем в формате указанном в
 *            стандарте категории 010),
 *            при возврате NULL данные отсутствуют
 */
TCommonObmenData* get010ModeSMBdata(TCat010* cat)
{
	TCommonObmenData* ret = NULL;


	if (cat == NULL)
		return ret;

	if ((cat->item010_250.mode_s_data_count > 0) &&
	    (cat->item010_250.mode_s_msg))
	{
		ret = initObmenData(cat->item010_250.mode_s_data_count * sizeof(u_int64_t));
		if (ret)
			setObmenData(ret, cat->item010_250.mode_s_msg, cat->item010_250.mode_s_data_count * sizeof(u_int64_t));
	}
	return ret;
}

/**
 * @brief Запись данных об облаке ярких точек отметки в структуру категории
 * @param cat - указатель на структуру категории 010
 * @param presence  - указатель на структуру с облаком ярких точек
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int set010PresenceData(TCat010* cat, PresenceData* presence)
{
	int res = -1, i;


	if ((cat == NULL) || (presence == NULL))
		return res;

	if ((cat) && (presence))
	{
		cat->item010_280.presense_count = presence->presence_count;
		if (cat->item010_280.presense_count > 0)
		{
			if (cat->item010_280.presense)
			{
				free(cat->item010_280.presense);
				cat->item010_280.presense = NULL;
			}

			cat->item010_280.presense = malloc(sizeof(PolarPosition) * cat->item010_280.presense_count);
			if (cat->item010_280.presense)
			{
				for (i = 0; i < cat->item010_280.presense_count; i += 1)
				{
					cat->item010_280.presense[i].azimuth  = presence->presence_recs[i].azimuth;
					cat->item010_280.presense[i].distance = presence->presence_recs[i].distance;
				}
				res = 0;
				if (cat->item010_fspec.code)
					cat->item010_fspec.code[3] |= 0x40;
			}
		}
	}
	return res;
}

/**
 * @brief Выборка данных об облаке ярких точек отметки
 * @param cat - указатель на структуру категории 010
 * @return - указатель на структуру с облаком ярких точек
 * @remarks - освобождение памяти через вызов функции freePresenceData()
 */
PresenceData* get010PresenceData(TCat010* cat)
{
	int           i;
	PresenceData* pres = NULL;


	if (cat == NULL)
		return pres;

	if (cat)
	{
		if ((cat->item010_280.presense_count > 0) && (cat->item010_280.presense))
		{
			pres = (PresenceData*) malloc(sizeof(PresenceData));

			if (pres)
			{
				pres->presence_count = cat->item010_280.presense_count;
				pres->presence_recs  = (PresencePositionPolar*) malloc(sizeof(PresencePositionPolar) * pres->presence_count);

				if (pres->presence_recs)
				{
					for (i = 0; i < pres->presence_count; i += 1)
					{
						pres->presence_recs[i].azimuth  = cat->item010_280.presense[i].azimuth;
						pres->presence_recs[i].distance = cat->item010_280.presense[i].distance;
					}
				}
			}
		}
	}
	return pres;
}

/**
 * @brief Освобождение памяти после использования структуры PresenceData
 * @param pres - указатель на структуру PresenceData
 * @return - EXIT_SUCCESS - успешном выполнении, EXIT_FAILURE - отказ выполнения
 */
int freePresenceData(PresenceData* pres)
{
	if (pres == NULL)
		return EXIT_FAILURE;

	if (pres)
	{
		if (pres->presence_recs)
			free(pres->presence_recs);
		free(pres);
	}
	return EXIT_SUCCESS;
}

/**
 * @brief Выборка "сырых" данных из записи категории 010 без распаковки
 * @param codogramm - указатель на контейнер с кодограммой
 * @param current_pos - начальное положение первого байта записи в теле кодограммы
 * @return - указатель на структуру контейнер Астерикс
 */
TCatAsterix* get010recordRawData(TCommonObmenData* codogramm, u_int64_t current_pos)
{
	unsigned char* code    = NULL;
	int            ccc     = 0;
	u_int64_t      k       = current_pos, cdgm_size = 0;
	TCatAsterix*   asterix = NULL;
	TCat010*       cat     = NULL;

	if (codogramm == NULL)
	{
		fprintf(stderr, "proc[pid-%d] get010recordRawData null TCommonObmenData data ERROR\n", getpid());
		return asterix;
	}
	cdgm_size = getObmenDataLenght(codogramm);
	if (cdgm_size == 0)
	{
		fprintf(stderr, "proc[pid-%d] get010recordRawData null size data ERROR\n", getpid());
		return asterix;
	}

	if (checkOverloadFunc(k, cdgm_size, cdgm_size) < 0)
	{
		return asterix;
	}

	code = (unsigned char*) getObmenData(codogramm);

	if (code == NULL)
	{
		fprintf(stderr, "proc[pid-%d] get010recordRawData data array ERROR\n", getpid());
		return asterix;
	}

	asterix = createCatAsterixData(CODT_CAT_010);

	if (asterix)
	{
		cat = &asterix->category.c010;
	}
	else
	{
		fprintf(stderr, "proc[pid-%d] get010recordRawData cat 010 create category ERROR\n", getpid());
		return asterix;
	}

	if (cat->item010_fspec.code == NULL)
	{
		freeCatAsterixData(asterix);
		fprintf(stderr, "proc[pid-%d] get010recordRawData fspec header code NULL ERROR\n", getpid());
		return NULL;
	}

	ccc = getCategoryFspecSize(code, k, cdgm_size);

	if ((ccc > 0) && (ccc <= FSPEC_SIZE_4_BYTE) && (k + ccc < cdgm_size))
	{
		if (cat->item010_fspec.code)
		{
			memcpy(cat->item010_fspec.code, code + k, ccc);
			cat->item010_fspec.code_size = ccc;
		}
		k += ccc;
	}
	else
	{
		freeCatAsterixData(asterix);
		fprintf(stderr, "proc[pid-%d] get010recordRawData FSPEC ERROR\n", getpid());
		return NULL;
	}

	if (cat->item010_fspec.code[0] & 0x80)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_010.head, CAT010_EL010, 2, code + k, "Data Source Identifier");
		k += 2;
	}

	if (cat->item010_fspec.code[0] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_000.head, CAT010_EL000, 1, code + k, "Message Type");
		k++;
	}

	if (cat->item010_fspec.code[0] & 0x20)
	{
		int differ = k;

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

		setElementHeader(&cat->item010_020.head, CAT010_EL020, k - differ, code + differ, "Target Report Descriptor");
	}

	if (cat->item010_fspec.code[0] & 0x10)
	{
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item010_140.head, CAT010_EL140, 3, code + k, "Time of Track Information");
		k += 3;
	}
	if (cat->item010_fspec.code[0] & 0x08)
	{
		if (checkOverloadFunc(k, 8, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_041.head, CAT010_EL041, 8, code + k, "Calculated Track Position (WGS-84)");
		k += 8;
	}

	if (cat->item010_fspec.code[0] & 0x04)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_040.head, CAT010_EL040, 4, code + k, "Measured Position in Polar");
		k += 4;
	}

	if (cat->item010_fspec.code[0] & 0x02)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item010_042.head, CAT010_EL042, 4, code + k, "Calculated Position in Cartesian");
		k += 4;
	}

	if (cat->item010_fspec.code[1] & 0x80)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item010_200.head, CAT010_EL200, 4, code + k, "Calculated Track Velocity in Polar");
		k += 4;
	}

	if (cat->item010_fspec.code[1] & 0x40)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item010_202.head, CAT010_EL202, 4, code + k, "Calculated Track Velocity in Cartesian");
		k += 4;
	}


	if (cat->item010_fspec.code[1] & 0x20)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_161.head, CAT010_EL161, 2, code + k, "Track Number");
		k += 2;
	}

	if (cat->item010_fspec.code[1] & 0x10)
	{
		int differ = k;

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
			if (code[ k - 1] & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				k += 1;
			}                                  // 3 octet
		}                                          // 2 octet

		setElementHeader(&cat->item010_170.head, CAT010_EL170, k - differ, code + differ, "Track Status");
	}

	if (cat->item010_fspec.code[1] & 0x08)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_060.head, CAT010_EL060, 2, code + k, "Mode-3/A Code");
		k += 2;
	}

	if (cat->item010_fspec.code[1] & 0x04)
	{
		if (checkOverloadFunc(k, 3, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_220.head, CAT010_EL220, 3, code + k, "Target Address");
		k += 3;
	}

	if (cat->item010_fspec.code[1] & 0x02)
	{
		if (checkOverloadFunc(k, 7, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_245.head, CAT010_EL245, 7, code + k, "Target Identification");
		k += 7;
	}

	if (cat->item010_fspec.code[2] & 0x80)
	{
		int differ = k, size = 0;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		size = code[ k ];
		k++;

		if (size > 0)
		{
			size *= sizeof(u_int64_t);
			if (checkOverloadFunc(k, size, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += size;
		}
		setElementHeader(&cat->item010_250.head, CAT010_EL250, k - differ, code + differ, "Mode S MB Data");
	}

	// I010/300  Vehicle Fleet Identification
	if (cat->item010_fspec.code[2] & 0x40)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_300.head, CAT010_EL300, 1, code + k, "Vehicle Fleet Identification");
		k += 1;
	}

	// I010/090  Flight Level in Binary Representation
	if (cat->item010_fspec.code[2] & 0x20)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item010_090.head, CAT010_EL090, 2, code + k, "Flight Level in Binary Representation");
		k += 2;
	}
	// I010/091  calc H geo
	if (cat->item010_fspec.code[2] & 0x10)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_091.head, CAT010_EL091, 2, code + k, "Measured Height - Geometric Altitude");
		k += 2;
	}
	// I010/270  VO size param
	if (cat->item010_fspec.code[2] & 0x08)
	{
		int diff = k;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		k += 1;
		if (code[k - 1] & 0x01)
		{
			if (checkOverloadFunc(k, 1, cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += 1;
			if (code[k - 1] & 0x01)
			{
				if (checkOverloadFunc(k, 1, cdgm_size) < 0)
				{
					freeCatAsterixData(asterix);
					return NULL;
				}
				k += 1;
			}
		}
		setElementHeader(&cat->item010_270.head, CAT010_EL270, k - diff, code + diff, "Target Size and Orientation");
	}
	// I010/550  System Status
	if (cat->item010_fspec.code[2] & 0x04)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_550.head, CAT010_EL550, 1, code + k, "System Status");
		k += 1;
	}
	// I010/310  Pre-programmed Message
	if (cat->item010_fspec.code[2] & 0x02)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		setElementHeader(&cat->item010_310.head, CAT010_EL310, 1, code + k, "Pre-programmed Message");
		k += 1;
	}
	// I010/500  Standard Deviation of Position
	if (cat->item010_fspec.code[3] & 0x80)
	{
		if (checkOverloadFunc(k, 4, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item010_500.head, CAT010_EL500, 4, code + k, "Standard Deviation of Position");
		k += 4;
	}

	// I010/280  Presence
	if (cat->item010_fspec.code[3] & 0x40)
	{
		int diff = k, size = 0;

		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		size = code[ k ];
		k   += 1;

		if (checkOverloadFunc(k, size * 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		k += (size * 2);
		setElementHeader(&cat->item010_280.head, CAT010_EL280, k - diff, code + diff, "Presence");
	}

	// I010/131  Amplitude of Primary Plot
	if (cat->item010_fspec.code[3] & 0x20)
	{
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item010_131.head, CAT010_EL131, 1, code + k, "Amplitude of Primary Plot");
		k += 1;
	}

	if (cat->item010_fspec.code[3] & 0x10)
	{
		if (checkOverloadFunc(k, 2, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}

		setElementHeader(&cat->item010_210.head, CAT011_EL210, 2, code + k, "Calculated Acceleration in Cartesian");
		k += 2;
	}

	if (cat->item010_fspec.code[3] & 0x04)
	{
		int diff = k;
		if (checkOverloadFunc(k, 1, cdgm_size) < 0)
		{
			freeCatAsterixData(asterix);
			return NULL;
		}
		k += 1;

		if (cat->item010_sp == NULL)
			cat->item010_sp = (Cat010specialPuposeField*) malloc(sizeof(Cat010specialPuposeField));

		if (cat->item010_sp)
		{
			bzero(cat->item010_sp, sizeof(Cat010specialPuposeField));
		}

		if (code[ k - 1 ] > 0)                     // Есть данные поля SP
		{
			if (checkOverloadFunc(k - 1, code[ k - 1 ], cdgm_size) < 0)
			{
				freeCatAsterixData(asterix);
				return NULL;
			}
			k += (code[ k - 1 ] - 1);
		}

		if (cat->item010_sp)
			setElementHeader(&cat->item010_sp->head, CAT010_ELSP, k - diff, code + diff, "I010 Special Purpose Field");
	}

	return asterix;
}
