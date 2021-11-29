/*
 *
 *
 */
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

#include "common_lib_asterix.h"
#include "cat_asterix.h"

/**
 * @brief Структура хранения данных для распаковки/упаковки в библиотеках обмена
 */
struct TCommonObmenData
{
	/**
	 * @brief тип данных
	 */
	int type_data;
	/**
	 * @brief длина массива данных
	 */
	size_t len;
	/**
	 * @brief указатель на массив данных
	 */
	void* data;
};

/**
 * @brief Перевод между алфавитами ASCII и IA-5
 * @param bs - символ алвавита для конвертации
 * @param rew - признак направления конвертации:
 *              0 - IA5 -> ASCII
 *              1 - ASCII -> IA5
 * @return символ конвертированный
 */
char asciiAndIa5alphabet(char bs, unsigned char rew)
{
	char b = 0;

	if (!rew)                                          // convertation IA5 -> ASCII
	{
		switch (bs)
		{
		case 1: b = 'A';
			break;
		case 2: b = 'B';
			break;
		case 3: b = 'C';
			break;
		case 4: b = 'D';
			break;
		case 5: b = 'E';
			break;
		case 6: b = 'F';
			break;
		case 7: b = 'G';
			break;
		case 8: b = 'H';
			break;
		case 9: b = 'I';
			break;
		case 10: b = 'J';
			break;
		case 11: b = 'K';
			break;
		case 12: b = 'L';
			break;
		case 13: b = 'M';
			break;
		case 14: b = 'N';
			break;
		case 15: b = 'O';
			break;
		case 16: b = 'P';
			break;
		case 17: b = 'Q';
			break;
		case 18: b = 'R';
			break;
		case 19: b = 'S';
			break;
		case 20: b = 'T';
			break;
		case 21: b = 'U';
			break;
		case 22: b = 'V';
			break;
		case 23: b = 'W';
			break;
		case 24: b = 'X';
			break;
		case 25: b = 'Y';
			break;
		case 26: b = 'Z';
			break;
		case 32: b = 0;
			break;
		case 48: b = '0';
			break;
		case 49: b = '1';
			break;
		case 50: b = '2';
			break;
		case 51: b = '3';
			break;
		case 52: b = '4';
			break;
		case 53: b = '5';
			break;
		case 54: b = '6';
			break;
		case 55: b = '7';
			break;
		case 56: b = '8';
			break;
		case 57: b = '9';
			break;
		default: b = 0;
			break;
		}
	}
	else
	{                                                  // convertation   ASCII -> IA5
		switch (bs)
		{
		case 'A': b = 1;
			break;
		case 'B': b = 2;
			break;
		case 'C': b = 3;
			break;
		case 'D': b = 4;
			break;
		case 'E': b = 5;
			break;
		case 'F': b = 6;
			break;
		case 'G': b = 7;
			break;
		case 'H': b = 8;
			break;
		case 'I': b = 9;
			break;
		case 'J': b = 10;
			break;
		case 'K': b = 11;
			break;
		case 'L': b = 12;
			break;
		case 'M': b = 13;
			break;
		case 'N': b = 14;
			break;
		case 'O': b = 15;
			break;
		case 'P': b = 16;
			break;
		case 'Q': b = 17;
			break;
		case 'R': b = 18;
			break;
		case 'S': b = 19;
			break;
		case 'T': b = 20;
			break;
		case 'U': b = 21;
			break;
		case 'V': b = 22;
			break;
		case 'W': b = 23;
			break;
		case 'X': b = 24;
			break;
		case 'Y': b = 25;
			break;
		case 'Z': b = 26;
			break;
		case ' ': b = 32;
			break;
		case '0': b = 48;
			break;
		case '1': b = 49;
			break;
		case '2': b = 50;
			break;
		case '3': b = 51;
			break;
		case '4': b = 52;
			break;
		case '5': b = 53;
			break;
		case '6': b = 54;
			break;
		case '7': b = 55;
			break;
		case '8': b = 56;
			break;
		case '9': b = 57;
			break;
		default: b = 32;
			break;
		}
	}
	return b;
}

/**
 * @brief Перевод из алфавита IA5 в алфавит ASCII
 * @param dest - указатель на массив укладки результата преобразования
 * @param ist - указатель на массив с исходными данными
 * @param cnt - номер начального байта исходной записи
 * @return - 0 - успешное выполнение, -1 отказ выполнения операции
 */
int convertIA5codeToAscii(unsigned char* dest, unsigned char* ist, u_int64_t* cnt)
{
	int           mas = *cnt;
	unsigned char b   = 0, i;

	if ((dest == NULL) || (ist == NULL))
		return -1;

	for (i = 0; i < 8; i += 4)
	{
		b           = ist[ mas ] >> 2;
		dest[i]     = asciiAndIa5alphabet(b, OFF);
		b           = ((ist[ mas ] & 0x03) << 4) | ((ist[ mas + 1] & 0xf0) >> 4);
		mas        += 1;
		dest[i + 1] = asciiAndIa5alphabet(b, OFF);
		b           = ((ist[ mas ] & 0x0f) << 2) | ((ist[ mas + 1] & 0xC0) >> 6);
		mas        += 1;
		dest[i + 2] = asciiAndIa5alphabet(b, OFF);
		b           = ist[ mas ] & 0x3F;
		mas        += 1;
		dest[i + 3] = asciiAndIa5alphabet(b, OFF);
	}
	*cnt = mas;
	return 0;
}

/**
 * @brief Перевод из алфавита ASCII в алфавит IA5
 * @param dest - указатель на массив укладки результата преобразования
 * @param ist - указатель на массив с исходными данными
 * @param cnt - номер начального байта исходной записи
 * @return - 0 - успешное выполнение, -1 отказ выполнения операции
 */
int convertAsciiToIA5code(unsigned char* dest, unsigned char* ist, u_int64_t* cnt)
{
	short         res = 0, mas = *cnt;
	unsigned char b   = 0, i, b1;

	if ((dest == NULL) || (ist == NULL))
		return -1;

	for (i = 0; i < 8; i += 4)
	{
		b          = ist[i];
		b1         = (unsigned char) asciiAndIa5alphabet(b, ON);
		dest[mas]  = b1 << 2;
		b          = ist[i + 1];
		b1         = (unsigned char) asciiAndIa5alphabet(b, ON);
		dest[mas] |= ((b1 >> 4) & 0x03);
		mas       += 1;
		dest[mas]  = b1 << 4;
		b          = ist[i + 2];
		b1         = (unsigned char) asciiAndIa5alphabet(b, ON);
		dest[mas] |= ((b1 >> 2) & 0x0f);
		mas       += 1;
		dest[mas]  = b1 << 6;
		b          = ist[i + 3];
		b1         = (unsigned char) asciiAndIa5alphabet(b, ON);
		dest[mas] |= (b1 & 0x3f);
		mas       += 1;
	}
	*cnt = mas;
	return res;
}

/**
 * @brief Выборка размера профиля пользователя сообщения категории АСТЕРИКС
 * @param codd - указатель на кодограмму формата АСТЕРИКС
 * @param cnt - начальная позиция первого байта профиля пользователя
 * @return - количество байт в профиле пользователя
 */
int getCategoryFspecSize(unsigned char* codd, u_int64_t cnt, u_int64_t size_codogramm)
{
	u_int64_t i, count = cnt;
	int       res = 0;

	if ((codd == NULL) || (cnt >= size_codogramm))
		return 0;

	for (i = cnt; i < cnt + FSPEC_SIZE_7_BYTE; i += 1)
	{
		if (!(codd[i] & 0x01))
		{
			count += 1;
			break;
		}
		count += 1;
		if (count == size_codogramm)
		{
			if (codd[i] & 0x01)
				count = cnt;
			break;
		}
	}

	res = count - cnt;

	return res;
}

/**
 * @brief Декодирование данных о запасе топлива на борту ВО
 * @param code - кодированное значение запаса топлива
 * @return - запас топлива на борту в %
 */
u_int8_t decodeUvdFuel(u_int8_t code)
{
	u_int8_t fuel = 0;

	if (code <= 10)
		fuel = code * 5;
	else
		fuel = code * 5 + (code - 10) * 5;

	return fuel;
}

/**
 * @brief Кодирование данных о запасе топлива на борту ВО
 * @param fuel - данные о запасе топлива на борту ВО в %
 * @return - кодированное значение запаса топлива
 */
u_int8_t packUvdFuel(u_int8_t fuel)
{
	u_int8_t code = 0;

	if (fuel <= 50)
		code = fuel / 10;
	else
		code = (fuel / 10) + 5;

	return code;
}

/**
 * @brief Инициализация структуры массива данных обмена библиотеки
 * @param data_len - размер данных в байтах
 * @return указатель на созданный массив данных
 */
TCommonObmenData* initObmenData(size_t data_len)
{
	TCommonObmenData* data = NULL;

	data = (TCommonObmenData*) malloc(sizeof(TCommonObmenData));
	if (!data)
		perror("");

	if (data)
	{
		// bzero(data, sizeof(TCommonObmenData));
		memset(data, 0, sizeof(TCommonObmenData));
		data->type_data = CODT_UNDEFINED;
		data->len       = data_len;
		data->data      = malloc(data_len);

		if (data->data)
		{
			memset(data->data, 0, data_len);
		}
		else
		{
			free(data);
			data = NULL;
		}
	}
	return data;
}

/**
 * @brief Освобождение памяти структуры обмена данными бибилиотеки
 * @param data - указатель на массив данных
 */
void freeObmenData(TCommonObmenData* data)
{
	if (data)
	{
		if (data->data)
		{
			free(data->data);
			data->data = NULL;
		}
		data->len       = 0;
		data->type_data = 0;
		free(data);
		data = NULL;
	}
	return;
}

/**
 * @brief Запись данных для обмена в структуру массива данных библиотеки
 * @param data      - указатель на структуру массива данных библиотеки
 * @param info      - указатель на данные
 * @param info_len  - длина данных в баййтах
 * @return  - результат записи
 */
int setObmenData(TCommonObmenData* data, void* info, size_t info_len)
{
	int res = EXIT_FAILURE;

	if ((data == NULL) || (info == NULL))
		return res;

	if ((info_len > 0) && (info_len <= data->len) && (data->data != NULL))
	{
		memcpy(data->data, info, info_len);
		data->len = info_len;
		res       = EXIT_SUCCESS;
	}
	return res;
}

/**
 * @brief Получение указателя на данные в массиве обмена бибилиотеки
 * @param data  - указатель на структуру обмена данными библиотеки
 * @return - указатель на массив данных
 */
void* getObmenData(TCommonObmenData* data)
{
	if (data)
	{
		return data->data;
	}
	else
	{
		return NULL;
	}
}

/**
 * @brief Получение длины данных в массиве обмена библиотеки
 * @param data - указатель на структуру данных обмена бибилиотеки
 * @return - длина данных массива обмена в байтах
 */
size_t getObmenDataLenght(TCommonObmenData* data)
{
	if (data)
	{
		return data->len;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief Получение признака типа данных в массиве обмена библиотеки
 * @param data - указатель на структуру данных обмена бибилиотеки
 * @return - признак типа данных в массиве обмена в байтах
 */
const int getObmenDataType(TCommonObmenData* data)
{
	if (data)
	{
		return data->type_data;
	}
	else
	{
		return CODT_UNDEFINED;
	}
}

/**
 * @brief Выставление признака типа данных в массиве обмена библиотеки
 * @param data - указатель на структуру данных обмена бибилиотеки
 * @return - результат присвоения типа данных в массиве обмена в байтах
 */
int setObmenDataType(TCommonObmenData* data, int type_data)
{
	if (data)
	{
		data->type_data = type_data;
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

/**
 * @brief Кодирование текущего времени суток (UTC) для кодограмм Астерикс
 * @return - кодированное время
 */
int makeTimeForAsterixCodo(void)
{
	int            res = 0;
	/*double         ddd;	// for Linux
	struct timeval my_time;
	struct tm*     TM;

	gettimeofday(&my_time, NULL);
	TM  = gmtime(&my_time.tv_sec);
	ddd = TM->tm_hour * 3600.0 + TM->tm_min * 60.0 + TM->tm_sec * 1.0 + my_time.tv_usec * 1e-6;
	res = (int) (ddd / CMR_ASTX_TIME);*/
	return res;
}

/**
 * @brief Функция выдачи номера категории Астерикс
 * @param cat_lib - указатель на структуру категории
 * @return - номер категории, 0 - в случае отказа
 */
int getCatergoryNum(struct TCatAsterix* cat_lib)
{
	int res = 0;

	if (cat_lib)
	{
		res = cat_lib->cat_num;
	}

	return res;
}

/**
 * @brief Функция выдачи максимального индекса категории Астерикс
 *        (для определения количества элементов в профиле пользователя категории)
 * @param cat_lib - указатель на структуру категории
 * @return - номер категории, 0 - в случае отказа
 */
int getCatIndexSize(struct TCatAsterix* cat_lib)
{
	int res = 0;

	if (cat_lib)
	{
		res = cat_lib->index_number;
	}
	return res;
}

void freeAdsData(AircraftDerivedData* ads)
{
	if (ads)
	{
		if (ads->head.code)
			free(ads->head.code);
		if (ads->route_point)
			free(ads->route_point);
		if (ads->mode_s_msg)
			free(ads->mode_s_msg);
		free(ads);
	}

	return;
}

/**
 * @brief Освобождение памяти отведенной под
 *        структуру заголовка элемента категории Астерикс
 * @param cat
 */
void freeElementHeaderCode(ElementHeader* el_header)
{
	if (el_header)
	{
		if (el_header->code)
			free(el_header->code);
	}

	return;
}

/**
 * @brief Освобождение памяти отведенной под
 *        структуру категории 062 Астерикс
 * @param cat - указатель на структуру
 */
void freeCat062(struct TCat062* cat)
{

	if (cat == NULL)
		return;

	if (cat)
	{
		// расположение по профилю UAP 062
		freeElementHeaderCode(&cat->item062_fspec);
		freeElementHeaderCode(&cat->item062_010.head);
		freeElementHeaderCode(&cat->item062_015.head);
		freeElementHeaderCode(&cat->item062_070.head);
		freeElementHeaderCode(&cat->item062_105.head);
		freeElementHeaderCode(&cat->item062_100.head);
		freeElementHeaderCode(&cat->item062_185.head);

		freeElementHeaderCode(&cat->item062_210.head);
		freeElementHeaderCode(&cat->item062_060.head);
		freeElementHeaderCode(&cat->item062_245.head);
		if (cat->item062_380)
		{
			freeAdsData(cat->item062_380);
		}
		freeElementHeaderCode(&cat->item062_040.head);
		freeElementHeaderCode(&cat->item062_080.head);
		if (cat->item062_080.track_status)
			free(cat->item062_080.track_status);
		freeElementHeaderCode(&cat->item062_290.head);

		freeElementHeaderCode(&cat->item062_200.head);
		freeElementHeaderCode(&cat->item062_295.head);
		freeElementHeaderCode(&cat->item062_136.head);
		freeElementHeaderCode(&cat->item062_130.head);
		freeElementHeaderCode(&cat->item062_135.head);
		freeElementHeaderCode(&cat->item062_220.head);
		if (cat->item062_390)
		{
			freeElementHeaderCode(&cat->item062_390->head);
			free(cat->item062_390);
		}

		freeElementHeaderCode(&cat->item062_270.head);
		freeElementHeaderCode(&cat->item062_300.head);
		freeElementHeaderCode(&cat->item062_110.head);
		freeElementHeaderCode(&cat->item062_120.head);
		// freeElementHeaderCode(&cat->item062_510.head);
		freeElementHeaderCode(&cat->item062_500.head);
		if (cat->item062_340)
		{
			freeElementHeaderCode(&cat->item062_340->head);
			if (cat->item062_340->measured_info)
				free(cat->item062_340->measured_info);
			free(cat->item062_340);
		}

		if (cat->item062_sp)
		{
			freeElementHeaderCode(&cat->item062_sp->head);
			if (cat->item062_sp->el_sp)
				free(cat->item062_sp->el_sp);
			free(cat->item062_sp);
		}
	}

	return;
}

/**
 * @brief Освобождение памяти отведенной под
 *        структуру категории 011 Астерикс
 * @param cat
 */
void freeCat011(struct TCat011* cat)
{
	if (cat == NULL)
		return;

	if (cat)
	{
		// расположение по профилю UAP 011
		freeElementHeaderCode(&cat->item011_fspec);
		freeElementHeaderCode(&cat->item011_000.head);
		freeElementHeaderCode(&cat->item011_010.head);
		freeElementHeaderCode(&cat->item011_015.head);
		freeElementHeaderCode(&cat->item011_140.head);
		freeElementHeaderCode(&cat->item011_041.head);
		freeElementHeaderCode(&cat->item011_042.head);
		freeElementHeaderCode(&cat->item011_202.head);

		freeElementHeaderCode(&cat->item011_210.head);
		freeElementHeaderCode(&cat->item011_060.head);
		freeElementHeaderCode(&cat->item011_245.head);
		if (cat->item011_380)
		{
			freeElementHeaderCode(&cat->item011_380->head);
			if (cat->item011_380->mode_s_msg)
				free(cat->item011_380->mode_s_msg);
			free(cat->item011_380);
		}
		freeElementHeaderCode(&cat->item011_161.head);
		freeElementHeaderCode(&cat->item011_170.head);
		freeElementHeaderCode(&cat->item011_290.head);

		freeElementHeaderCode(&cat->item011_430.head);
		freeElementHeaderCode(&cat->item011_090.head);
		freeElementHeaderCode(&cat->item011_092.head);
		freeElementHeaderCode(&cat->item011_093.head);
		freeElementHeaderCode(&cat->item011_215.head);
		freeElementHeaderCode(&cat->item011_270.head);
		if (cat->item011_390)
		{
			freeElementHeaderCode(&cat->item011_390->head);
			free(cat->item011_390);
		}

		freeElementHeaderCode(&cat->item011_300.head);
		freeElementHeaderCode(&cat->item011_310.head);
		freeElementHeaderCode(&cat->item011_500.head);
		freeElementHeaderCode(&cat->item011_600.head);
		freeElementHeaderCode(&cat->item011_605.head);
		freeElementHeaderCode(&cat->item011_610.head);

		if (cat->item011_605.fusion_track_num)
			free(cat->item011_605.fusion_track_num);
		if (cat->item011_610.holdbars)
			free(cat->item011_610.holdbars);
		if (cat->item011_sp)
		{
			freeElementHeaderCode(&cat->item011_sp->head);
			if (cat->item011_sp->el_sp)
				free(cat->item011_sp->el_sp);
			free(cat->item011_sp);
		}
		if (cat->item011_re)
		{
			freeElementHeaderCode(&cat->item011_re->head);
			if (cat->item011_re->el_re)
				free(cat->item011_re->el_re);
			free(cat->item011_re);
		}
	}
	return;
}

/**
 * @brief Освобождение памяти отведенной под
 *        структуру категории 010 Астерикс
 * @param cat
 */
void freeCat010(struct TCat010* cat)
{
	if (cat == NULL)
		return;

	if (cat)
	{
		// расположение по профилю UAP 010
		freeElementHeaderCode(&cat->item010_fspec);
		freeElementHeaderCode(&cat->item010_010.head);
		freeElementHeaderCode(&cat->item010_000.head);
		freeElementHeaderCode(&cat->item010_020.head);
		if (cat->item010_020.track_descriptor)
			free(cat->item010_020.track_descriptor);
		freeElementHeaderCode(&cat->item010_140.head);
		freeElementHeaderCode(&cat->item010_041.head);
		freeElementHeaderCode(&cat->item010_040.head);
		freeElementHeaderCode(&cat->item010_042.head);

		freeElementHeaderCode(&cat->item010_200.head);
		freeElementHeaderCode(&cat->item010_202.head);
		freeElementHeaderCode(&cat->item010_161.head);
		freeElementHeaderCode(&cat->item010_170.head);
		if (cat->item010_170.track_status)
			free(cat->item010_170.track_status);
		freeElementHeaderCode(&cat->item010_060.head);
		freeElementHeaderCode(&cat->item010_220.head);
		freeElementHeaderCode(&cat->item010_245.head);

		freeElementHeaderCode(&cat->item010_250.head);
		if (cat->item010_250.mode_s_msg)
			free(cat->item010_250.mode_s_msg);
		freeElementHeaderCode(&cat->item010_300.head);
		freeElementHeaderCode(&cat->item010_090.head);
		freeElementHeaderCode(&cat->item010_091.head);
		freeElementHeaderCode(&cat->item010_270.head);
		freeElementHeaderCode(&cat->item010_550.head);
		if (cat->item010_550.system_status)
			free(cat->item010_550.system_status);
		freeElementHeaderCode(&cat->item010_310.head);

		freeElementHeaderCode(&cat->item010_500.head);
		freeElementHeaderCode(&cat->item010_280.head);
		if (cat->item010_280.presense)
			free(cat->item010_280.presense);
		freeElementHeaderCode(&cat->item010_131.head);
		freeElementHeaderCode(&cat->item010_210.head);
		if (cat->item010_sp)
		{
			freeElementHeaderCode(&cat->item010_sp->head);
			if (cat->item010_sp->el_sp)
				free(cat->item010_sp->el_sp);
			free(cat->item010_sp);
		}
		if (cat->item010_re)
		{
			freeElementHeaderCode(&cat->item010_re->head);
			if (cat->item010_re->el_re)
				free(cat->item010_re->el_re);
			free(cat->item010_re);
		}
	}
	return;
}

/**
 * @brief Освобождение памяти занятой структурой категории Астерикс
 * @param cat - указатель на структуру категории Астерикс
 */
void freeCatAsterixData(TCatAsterix* cat)
{
	if (cat)
	{
		switch (cat->cat_num)
		{
		case CODT_CAT_062:
			freeCat062(&cat->category.c062);
			break;
		case CODT_CAT_011:
			freeCat011(&cat->category.c011);
			break;
		case CODT_CAT_010:
			freeCat010(&cat->category.c010);
			break;
		}
		free(cat);
		cat = NULL;
	}

	return;
}

/**
 * @brief Инициализация структуры Астерикс в памяти
 * @param cat_num - номер категории Астерикс
 * @return - указатель на созданную структуру, NULL - отказ
 */
TCatAsterix* createCatAsterixData(unsigned int cat_num)
{
	u_int32_t    c_num = 0;
	TCatAsterix* cat   = NULL;

	switch (cat_num)
	{
	case CODT_CAT_062:
		c_num = CODT_CAT_062;
		break;
	case CODT_CAT_011:
		c_num = CODT_CAT_011;
		break;
	case CODT_CAT_010:
		c_num = CODT_CAT_010;
		break;
	default:
		c_num = 0;
		break;
	}

	if (c_num == 0)
	{
		fprintf(stderr, "proc[pid-%d] create cat %d struct ERROR\n", getpid(), cat_num);
		return cat;
	}

	cat = (TCatAsterix*) malloc(sizeof(TCatAsterix));

	if (cat == NULL)
	{
		fprintf(stderr, "proc[pid-%d] create cat %d struct memory ERROR\n", getpid(), c_num);
	}
	else
	{
		// bzero(cat, sizeof(TCatAsterix));
		memset(cat, 0, sizeof(TCatAsterix));
		switch (c_num)
		{
		case CODT_CAT_062:
			cat->cat_num      = CODT_CAT_062;
			cat->index_number = CAT062_ELSP;
			if (cat->category.c062.item062_fspec.code == NULL)
				setElementHeader(&cat->category.c062.item062_fspec, 0, FSPEC_SIZE_5_BYTE, NULL, "Cat 062 User Application Profile");
			break;
		case CODT_CAT_011:
			cat->cat_num      = CODT_CAT_011;
			cat->index_number = CAT011_ELRE;
			if (cat->category.c011.item011_fspec.code == NULL)
				setElementHeader(&cat->category.c011.item011_fspec, 0, FSPEC_SIZE_5_BYTE, NULL, "Cat 011 User Application Profile");
			break;
		case CODT_CAT_010:
			cat->cat_num      = CODT_CAT_010;
			cat->index_number = CAT010_ELRE;
			if (cat->category.c010.item010_fspec.code == NULL)
				setElementHeader(&cat->category.c010.item010_fspec, 0, FSPEC_SIZE_4_BYTE, NULL, "Cat 010 User Application Profile");

		}
	}

	return cat;
}

/**
 * @brief Запись данных в заголовок элемента данных категории Астерикс
 * @param elhead - указатель структуру заголовка
 * @param index  - номер элемента в профиле пользователя
 * @param code_size - количество данных кодирующих элемент
 * @param code - указатель на кодированные данные
 * @param name - строчное название элемента данных
 * @return - результат записи
 *           -1 - отказ исполнения
 *            0 - установлен номер элемента
 *            1 - установлен номер, название элемента
 *            2 - установлен номер, кодированные данные
 *            3 - установлен номер, название, кодированные данные
 */
int setElementHeader(ElementHeader* elhead, u_int8_t index, u_int8_t code_size, u_int8_t* code, char* name)
{
	int res = 0;

	if (elhead == NULL)
	{
		return -1;
	}

	if (elhead)
	{
		elhead->fspec_index = index;
		elhead->code_size   = code_size;
		if (name)
		{
			// bzero(elhead->name, FSPEC_ELEMENT_NAME_SIZE);
			memset(elhead->name, 0, FSPEC_ELEMENT_NAME_SIZE);
			if (strlen(name) < FSPEC_ELEMENT_NAME_SIZE)
				strcpy(elhead->name, name);
			else
				strncpy(elhead->name, name, FSPEC_ELEMENT_NAME_SIZE - 1);
			res += 1;
		}
		if (code_size > 0)
		{
			if (elhead->code)
			{
				free(elhead->code);
				elhead->code = NULL;
			}

			if (elhead->code == NULL)
				elhead->code = (u_int8_t*)malloc(code_size);

			if (elhead->code)
			{
				// bzero(elhead->code, code_size);
				memset(elhead->code, 0, code_size);
				if (code)
				{
					memcpy(elhead->code, code, code_size);
					res += 2;
				}
			}
		}
	}
	return res;
}

/**
 * @brief Распаковка плановой информации в категорий категории 062, 011
 * @param code - указатель на массив кодограммы
 * @param cnt - номер начального байта плановой информации
 * @return - указатель на распакованную структуру элемента 390
 */
FlightPlanRelatedData* decodeElem390PlanData(unsigned char* code, u_int64_t* cnt, u_int64_t size_codogramm)
{
	unsigned char b = 0, p_spec[FSPEC_SIZE_5_BYTE] =
	{
		0
	};
	unsigned short         bb = 0;
	int                    i, ccc = 0;
	FlightPlanRelatedData* plan = NULL;
	u_int64_t              k    = *cnt;

	if (code == NULL)
		return plan;

	plan = (FlightPlanRelatedData*) malloc(sizeof(FlightPlanRelatedData));

	if (plan == NULL)
	{
		fprintf(stderr, "proc[pid-%d] decode Flight Plan Related Data (Item 390) create memory ERROR\n", getpid());
		return plan;
	}

	// bzero(plan, sizeof(FlightPlanRelatedData));
	memset(plan, 0, sizeof(FlightPlanRelatedData));

	if (checkOverloadFunc(k, 2, size_codogramm) < 0)
	{
		free(plan);
		return NULL;
	}

	ccc = getCategoryFspecSize(code, k, size_codogramm);

	if ((ccc > 0) && (ccc <= FSPEC_SIZE_3_BYTE))
	{
		memcpy(p_spec, code + k, ccc);
		k += ccc;
	}
	else
	{
		free(plan);
		fprintf(stderr, "proc[pid-%d] decode Flight Plan Related Data (Item 390) fspec create ERROR\n", getpid());
		return NULL;
	}
	// Plan sic_sac
	if (p_spec[0] & 0x80)
	{
		if (checkOverloadFunc(k, 2, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		bb                        = (code[k] << 8) | code[k + 1];
		k                        += 2;
		plan->fpdata.plan_sac_sic = bb;
	}
	// Callsign
	if (p_spec[0] & 0x40)
	{
		if (checkOverloadFunc(k, 7, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		memcpy(plan->fpdata.callsign, code + k, 7);
		k += 7;
	}
	// Plan ID
	if (p_spec[0] & 0x20)
	{
		if (checkOverloadFunc(k, 4, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}

		b = (code[k] & 0xC0) >> 6;

		if (b > 0)
		{
			;                                  // composed number(have 3 parts)
		}

		ccc = ((code[k] & 0x0f) << 24) | (code[k + 1] << 16) | (code[k + 2] << 8) | code[k + 3];
		k  += 4;

		if (b == 0)
			plan->fpdata.plan_identificator = ccc;
	}
	// Flight category
	if (p_spec[0] & 0x10)
	{
		u_int8_t b1 = 0;
		if (checkOverloadFunc(k, 1, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}

		b  = code[k];
		k += 1;

		b1 = b >> 6;
		if (b1 == 1)
			plan->fpdata.general_air_traffic = 1;
		if (b1 == 2)
			plan->fpdata.operational_air_traffic = 1;

		b1 = (b & 0x30) >> 4;
		switch (b1)
		{
		case 0: plan->fpdata.flight_rules = FRT_INSTRUMENTAL;
			break;
		case 1: plan->fpdata.flight_rules = FRT_VISUAL;
			break;
		case 3: plan->fpdata.flight_rules = FRT_CONTROLED_VISUAL;
			break;
		}
		b1 = (b & 0x0C) >> 2;                      // RVSM stat
		switch (b1)
		{
		case 0: plan->fpdata.rvsm_approve = RA_UNKNOWN;
			break;
		case 1: plan->fpdata.rvsm_approve = RA_APPROVED;
			break;
		case 2: plan->fpdata.rvsm_approve = RA_UNKNOWN;
			break;
		case 3: plan->fpdata.rvsm_approve = RA_UNKNOWN;
			break;
		}

		plan->fpdata.priority = !!(b & 0x02);
	}
	// Type aircraft
	if (p_spec[0] & 0x08)
	{
		if (checkOverloadFunc(k, 4, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		memcpy(plan->fpdata.type_aircraft, code + k, 4);
		k += 4;

	}
	// Turbulence
	if (p_spec[0] & 0x04)
	{
		if (checkOverloadFunc(k, 1, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		plan->fpdata.turbulence = (char) code[k];
		k                      += 1;
	}
	// Depart airport
	if (p_spec[0] & 0x02)
	{
		if (checkOverloadFunc(k, 4, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		memcpy(plan->fpdata.dep_airport, code + k, 4);
		k += 4;
	}
	// Dest airport
	if (p_spec[1] & 0x80)
	{
		if (checkOverloadFunc(k, 4, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		memcpy(plan->fpdata.dest_airport, code + k, 4);
		k += 4;
	}
	// VPP
	if (p_spec[1] & 0x40)
	{
		if (checkOverloadFunc(k, 3, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		memcpy(plan->fpdata.dest_runway, code + k, 3);
		k += 3;
	}
	// Current FL
	if (p_spec[1] & 0x20)
	{
		if (checkOverloadFunc(k, 2, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		bb                      = (code[k] << 8) | code[k + 1];
		k                      += 2;
		plan->fpdata.current_fl = (unsigned short) (bb * CMR_ASTX_H_25F);
	}
	// Current control position
	if (p_spec[1] & 0x10)
	{
		if (checkOverloadFunc(k, 2, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		plan->fpdata.current_ctrl_pos = (code[k] << 8) | code[k];
		k                            += 2;         // center
	}
	// TOD
	if (p_spec[1] & 0x08)
	{
		u_int8_t type_time = 0;
		if (checkOverloadFunc(k, 1, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		b                         = code[k];
		k                        += 1;             // rep factor
		plan->fpdata.fptime_count = b;
		// bzero(plan->fpdata.fptime, sizeof(TTimeDepArrival) * TODA_MAX_COUNT);
		memset(plan->fpdata.fptime, 0, sizeof(TTimeDepArrival) * TODA_MAX_COUNT);

		if (checkOverloadFunc(k, b * 4, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}

		for (i = 0; i < plan->fpdata.fptime_count; i++)
		{
			b         = code[k];
			k        += 1;
			type_time = b >> 3;
			if (type_time < TODA_MAX_COUNT)
			{
				plan->fpdata.fptime[type_time].active   = 1;
				plan->fpdata.fptime[type_time].curr_day = (b & 0x06) >> 1;
				b                                       = code[k];
				k                                      += 1;
				plan->fpdata.fptime[type_time].hour     = b;
				b                                       = code[k];
				k                                      += 1;
				plan->fpdata.fptime[type_time].min      = b;
				b                                       = code[k];
				k                                      += 1;
				if (!(b & 0x80))
					plan->fpdata.fptime[type_time].sec = (b & 0x3f);
			}
			else
				k += 4;
		}
	}
	// Aircraft stand
	if (p_spec[1] & 0x04)
	{
		if (checkOverloadFunc(k, 6, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		memcpy(plan->fpdata.aircraft_stand, code + k, 6);
		k += 6;
	}
	// Aircraft stand status
	if (p_spec[1] & 0x02)
	{
		if (checkOverloadFunc(k, 1, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		b                            = code[k];
		k                           += 1;
		plan->fpdata.stand_status    = b >> 6;
		plan->fpdata.stand_available = (b & 0x30) >> 4;
	}
	// SID proc
	if (p_spec[2] & 0x80)
	{
		if (checkOverloadFunc(k, 7, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		memcpy(plan->fpdata.sid, code + k, 7);
		k += 7;
	}
	// STAR proc
	if (p_spec[2] & 0x40)
	{
		if (checkOverloadFunc(k, 7, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		memcpy(plan->fpdata.star, code + k, 7);
		k += 7;
	}
	// Pre Emerg Mode 3A
	if (p_spec[2] & 0x20)
	{
		if (checkOverloadFunc(k, 2, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		bb                              = ((code[k] & 0x1f) << 8) | code[k + 1];
		k                              += 2;
		plan->fpdata.pre_emerg_m3a_code = bb;
	}
	// Pre Emerg Callsign
	if (p_spec[2] & 0x10)
	{
		if (checkOverloadFunc(k, 7, size_codogramm) < 0)
		{
			free(plan);
			return NULL;
		}
		memcpy(plan->fpdata.pre_emerg_callsign, code + k, 7);
		k += 7;
	}

	*cnt = k;

	return plan;
}

/**
 * @brief Упаковка плановой информации для сообщения категории Астерикс
 * @param plan - указатель на структуру с плановой информацией
 * @return - указатель на стуктуру с кодированными данными
 */
TCommonObmenData* packElem390PlanData(TFlightPlanData* plan, u_int32_t available_mask)
{
	unsigned char b = 0, code[DEFAULT_CODE_SIZE] =
	{
		0
	};
	int               i, ccc = 0, cnt = 0;
	u_int8_t          mask      = 0;
	TCommonObmenData* plan_data = NULL;

	if (plan == NULL)
		return plan_data;

	code[cnt] = 0;
	cnt      += 1;                                     // plan_spec
	code[cnt] = 0;
	cnt      += 1;                                     // plan_spec
	code[cnt] = 0;
	cnt      += 1;                                     // plan_spec

	// plan sic_sac
	ccc        = plan->plan_sac_sic;
	code[cnt]  = ccc >> 8;
	cnt       += 1;
	code[cnt]  = ccc;
	cnt       += 1;
	code[ 0 ] |= 0x80;

	mask = available_mask >> 16;
	// Callsign
	if ((strlen(plan->callsign) > 0) && (mask & 0x40))
	{
		memcpy(code + cnt, plan->callsign, 7);
		cnt       += 7;
		code[ 0 ] |= 0x40;
	}
	// plan ID
	if ((plan->plan_identificator > 0) && (mask & 0x20))
	{
		u_int32_t ccc = plan->plan_identificator;
		code[cnt]  = ccc >> 24;
		cnt       += 1;
		code[cnt]  = ccc >> 16;
		cnt       += 1;
		code[cnt]  = ccc >> 8;
		cnt       += 1;
		code[cnt]  = ccc;
		cnt       += 1;
		code[ 0 ] |= 0x20;
	}
	// Flight category
	if (mask & 0x10)
	{
		if ((plan->general_air_traffic) ||
		    (plan->operational_air_traffic) ||
		    (plan->flight_rules) ||
		    (plan->rvsm_approve) ||
		    (plan->priority))
		{
			b = 0;
			if (plan->general_air_traffic)
				b |= 0x40;
			if (plan->operational_air_traffic)
				b |= 0x80;
			if (plan->flight_rules == FRT_VISUAL)
				b |= 0x10;
			if (plan->flight_rules == FRT_CONTROLED_VISUAL)
				b |= 0x30;
			// RVSM stat
			if (plan->rvsm_approve == RA_EXTEMPT)
				b |= 0x08;
			if (plan->rvsm_approve == RA_APPROVED)
				b |= 0x04;
			if (plan->rvsm_approve == RA_NOT_APPROVED)
				b |= 0x0C;
			if (plan->priority)
				b |= 0x02;
			code[cnt]  = b;
			cnt       += 1;
			code[ 0 ] |= 0x10;
		}
	}

	// Type aircraft
	if ((strlen(plan->type_aircraft) > 0) && (mask & 0x08))
	{
		memcpy(code + cnt, plan->type_aircraft, 4);
		cnt       += 4;
		code[ 0 ] |= 0x08;
	}

	// Turbulence
	if ((plan->turbulence) && (mask & 0x04))
	{
		code[cnt]  = (unsigned char) plan->turbulence;
		cnt       += 1;
		code[ 0 ] |= 0x04;
	}

	// Depart airport
	if ((strlen(plan->dep_airport) > 0) && (mask & 0x02))
	{
		memcpy(code + cnt, plan->dep_airport, 4);
		cnt     += 4;
		code[0] |= 0x02;
	}
	mask = available_mask >> 8;
	// Dest airport
	if ((strlen(plan->dest_airport) > 0) && (mask & 0x80))
	{
		memcpy(code + cnt, plan->dest_airport, 4);
		cnt     += 4;
		code[1] |= 0x80;
	}
	// VPP
	if ((strlen(plan->dest_runway) > 0) && (mask & 0x40))
	{
		memcpy(code + cnt, plan->dest_runway, 3);
		cnt     += 3;
		code[1] |= 0x40;
	}
	// Current FL
	if ((plan->current_fl > 0) && (mask & 0x20))
	{
		ccc       = (unsigned short) (plan->current_fl / CMR_ASTX_H_25F);
		code[cnt] = ccc >> 8;
		cnt      += 1;
		code[cnt] = ccc;
		cnt      += 1;
		code[1]  |= 0x20;
	}
	// Current control position
	if ((plan->current_ctrl_pos) && (mask & 0x10))
	{
		ccc       = plan->current_ctrl_pos;
		code[cnt] = ccc >> 8;
		cnt      += 1;                             // center
		code[cnt] = ccc;
		cnt      += 1;                             // position
		code[1]  |= 0x10;
	}
	// TOD
	if (mask & 0x08)
	{
		ccc  = cnt;
		cnt += 1;                                  // rep factor
		b    = 0;
		for (i = 0; i < TODA_MAX_COUNT; i++)
		{
			if (plan->fptime[i].active)
			{
				code[cnt] = i << 3;
				if (plan->fptime[i].curr_day == PDA_YESTERDAY)
					code[cnt] |= 0x02;
				if (plan->fptime[i].curr_day == PDA_TOMORROW)
					code[cnt] |= 0x04;
				cnt      += 1;
				code[cnt] = plan->fptime[i].hour;
				cnt      += 1;
				code[cnt] = plan->fptime[i].min;
				cnt      += 1;
				code[cnt] = plan->fptime[i].sec;
				if (plan->fptime[i].sec == 0)
					code[cnt] |= 0x80;
				cnt += 1;
				b   += 1;
			}
		}
		if (b > 0)
		{
			code[ccc] = b;
			code[1]  |= 0x08;
		}
		else
		{
			cnt = ccc;
		}
	}
	// Aircraft stand
	if ((strlen(plan->aircraft_stand) > 0) && (mask & 0x04))
	{
		memcpy(code + cnt, plan->aircraft_stand, 6);
		cnt       += 6;
		code[ 1 ] |= 0x04;
		// Aircraft stand status
		code[cnt]  = (plan->stand_status & 0x03) << 6;
		code[cnt] |= ((plan->stand_available & 0x03) << 4);
		cnt       += 1;
		code[ 1 ] |= 0x02;
	}
	// SID proc
	mask = available_mask;
	if ((strlen(plan->sid) > 0) && (mask & 0x80))
	{
		memcpy(code + cnt, plan->sid, 7);
		cnt       += 7;
		code[ 2 ] |= 0x80;
	}
	// STAR proc
	if ((strlen(plan->star) > 0) && (mask & 0x40))
	{
		memcpy(code + cnt, plan->star, 7);
		cnt     += 7;
		code[2] |= 0x40;
	}
	// Pre Emerg Mode 3A
	if ((plan->pre_emerg_m3a_code > 0) && (mask & 0x20))
	{
		ccc       = plan->pre_emerg_m3a_code;
		code[cnt] = ccc >> 8;
		cnt      += 1;
		code[cnt] = ccc;
		cnt      += 1;
		code[2]  |= 0x20;
	}
	// Pre Emerg Callsign
	if ((strlen(plan->pre_emerg_callsign) > 0) && (mask & 0x10))
	{
		memcpy(code + cnt, plan->pre_emerg_callsign, 7);
		cnt     += 7;
		code[2] |= 0x10;
	}

	b = 1;
	if (code[2] > 0)
	{
		code[ 1 ] |= 0x01;
		code[ 0 ] |= 0x01;
		b          = 3;
	}
	else
	{
		if (code[ 1 ] > 0)
		{
			code[ 0 ] |= 0x01;
			b          = 2;
		}
	}

	memmove(code + b, code + 3, cnt - 3);
	cnt      -= (3 - b);
	plan_data = initObmenData(cnt);
	if (plan_data != NULL)
	{
		setObmenData(plan_data, code, cnt);
	}

	return plan_data;
}

/**
 * @brief Запись идентификатора источника в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param sac_sic_code - код источника данных
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setDataSourceIdentificator(TCatAsterix* cat_lib, u_int16_t sac_sic_code)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                        = 0;
			cat_lib->category.c062.item062_010.sac_sic = sac_sic_code;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[0] |= 0x80;
			break;
		case CODT_CAT_011:
			res                                        = 0;
			cat_lib->category.c011.item011_010.sac_sic = sac_sic_code;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[0] |= 0x80;
			break;
		case CODT_CAT_010:
			res                                        = 0;
			cat_lib->category.c010.item010_010.sac_sic = sac_sic_code;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[0] |= 0x80;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись идентификатора источника в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param service_id - код сервиса
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setServiceIdentificator(TCatAsterix* cat_lib, u_int8_t service_id)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                          = 0;
			cat_lib->category.c062.item062_015.sevice_id = service_id;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[0] |= 0x20;
			break;
		case CODT_CAT_011:
			res                                          = 0;
			cat_lib->category.c011.item011_015.sevice_id = service_id;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[0] |= 0x20;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись времени сообщения в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param time_of_track - время трека, в секундах
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTimeOfTrackInformation(TCatAsterix* cat_lib, double time_of_track)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                              = 0;
			cat_lib->category.c062.item062_070.time_of_track = time_of_track;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[0] |= 0x10;
			break;
		case CODT_CAT_011:
			res                                              = 0;
			cat_lib->category.c011.item011_140.time_of_track = time_of_track;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[0] |= 0x10;
			break;
		case CODT_CAT_010:
			res                                              = 0;
			cat_lib->category.c010.item010_140.time_of_track = time_of_track;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[0] |= 0x10;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись положения трассы в географических координатах в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param latitude - широта трассы, в радианах
 * @param longitude - долгота трека, в радианах
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackPositionWgs84(TCatAsterix* cat_lib, double latitude, double longitude)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                          = 0;
			cat_lib->category.c062.item062_105.latitude  = latitude;
			cat_lib->category.c062.item062_105.longitude = longitude;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[0] |= 0x08;

			break;
		case CODT_CAT_011:
			res                                          = 0;
			cat_lib->category.c011.item011_041.latitude  = latitude;
			cat_lib->category.c011.item011_041.longitude = longitude;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[0] |= 0x08;
			break;
		case CODT_CAT_010:
			res                                          = 0;
			cat_lib->category.c010.item010_041.latitude  = latitude;
			cat_lib->category.c010.item010_041.longitude = longitude;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[0] |= 0x08;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись положения трассы в прямоугольных координатах в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param x - координата Х положения трассы, в метрах
 * @param y - координата У положения трассы, в метрах
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackPositionCartesian(TCatAsterix* cat_lib, int x, int y)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                  = 0;
			cat_lib->category.c062.item062_100.x = x;
			cat_lib->category.c062.item062_100.y = y;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[0] |= 0x04;
			break;
		case CODT_CAT_011:
			res                                  = 0;
			cat_lib->category.c011.item011_042.x = x;
			cat_lib->category.c011.item011_042.y = y;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[0] |= 0x04;
			break;
		case CODT_CAT_010:
			res                                  = 0;
			cat_lib->category.c010.item010_042.x = x;
			cat_lib->category.c010.item010_042.y = y;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[0] |= 0x02;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись положения трассы в полярных координатах в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param azimuth - азимут положения трассы, в радианах
 * @param distance - дальность положения трассы, в метрах
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackPositionPolar(TCatAsterix* cat_lib, double azimuth, int distance)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_010:
			res                                               = 0;
			cat_lib->category.c010.item010_040.polar.azimuth  = azimuth;
			cat_lib->category.c010.item010_040.polar.distance = distance;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[0] |= 0x04;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения скорости трассы в прямоугольных координатах в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param vx - вектор скорости трассы по координате Х, в метрах/секунду
 * @param vy - вектор скорости трассы по координате У, в метрах/секунду
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackVelocityCartesian(TCatAsterix* cat_lib, double vx, double vy)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                   = 0;
			cat_lib->category.c062.item062_185.vx = vx;
			cat_lib->category.c062.item062_185.vy = vy;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[0] |= 0x02;
			break;
		case CODT_CAT_011:
			res                                   = 0;
			cat_lib->category.c011.item011_202.vx = vx;
			cat_lib->category.c011.item011_202.vy = vy;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[0] |= 0x02;
			break;
		case CODT_CAT_010:
			res                                   = 0;
			cat_lib->category.c010.item010_202.vx = vx;
			cat_lib->category.c010.item010_202.vy = vy;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[1] |= 0x40;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения скорости и курса трассы в полярных координатах в структуру категории
 * @param cat_lib   - указатель на структуру категории
 * @param velocity  - скорость трассы, в метрах/секунду
 * @param course    - курс трассы, в радианах
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackVelocityPolar(TCatAsterix* cat_lib, double velocity, double course)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_010:
			res                                             = 0;
			cat_lib->category.c010.item010_200.ground_speed = velocity;
			cat_lib->category.c010.item010_200.track_angle  = course;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[1] |= 0x80;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения ускорения по скорости трассы в прямоугольных координатах в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param ax - вектор ускорения трассы по координате Х, в метрах/секунду2
 * @param ay - вектор ускорения трассы по координате У, в метрах/секунду2
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackAccelerationCartesian(TCatAsterix* cat_lib, double ax, double ay)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                   = 0;
			cat_lib->category.c062.item062_210.ax = ax;
			cat_lib->category.c062.item062_210.ay = ay;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[1] |= 0x80;

			break;
		case CODT_CAT_011:
			res                                   = 0;
			cat_lib->category.c011.item011_210.ax = ax;
			cat_lib->category.c011.item011_210.ay = ay;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[1] |= 0x80;
			break;
		case CODT_CAT_010:
			res                                   = 0;
			cat_lib->category.c010.item010_210.ax = ax;
			cat_lib->category.c010.item010_210.ay = ay;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[3] |= 0x10;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения номера RBS Mode 3/A в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param mode3a_code - код номера в режиме 3/А
 * @param change_code - признак обновления данных номера в режиме 3/А
 *                      1 - код обновлен, 0 - код не обновлялся
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackMode3Acode(TCatAsterix* cat_lib, BoardNumCode* num3a_code)
{
	int res = -1;

	if ((cat_lib == NULL) || (num3a_code == NULL))
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res = 0;
			memcpy(&cat_lib->category.c062.item062_060.m3a, num3a_code, sizeof(BoardNumCode));
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[1] |= 0x40;
			break;
		case CODT_CAT_011:
			res = 0;
			memcpy(&cat_lib->category.c011.item011_060.m3a, num3a_code, sizeof(BoardNumCode));
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[1] |= 0x40;
			break;
		case CODT_CAT_010:
			res = 0;
			memcpy(&cat_lib->category.c010.item010_060.m3a, num3a_code, sizeof(BoardNumCode));
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[1] |= 0x08;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;

}

/**
 * @brief Запись значения идентификатора цели (позывной или регистрационный номер) в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param target_id - указатель на массив с идентификатором цели
 * @param tid_src -  признак источника идентификатора
 *                      0 - идентификатор (позывной или регистрационный номер)
 *                          получен непосредственно от объекта
 *                      1 - идентификатор (позывной) получен из другого источника
 *                      2 - идентификатор (регистрационный номер) получен из другого источника
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackTargetIdentification(TCatAsterix* cat_lib, char* target_id, u_int8_t tid_src)
{
	int res = -1;

	if ((cat_lib == NULL) || (target_id == NULL))
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res = 0;
			if (strlen(target_id) < sizeof(cat_lib->category.c062.item062_245.target_id))
				strcpy(cat_lib->category.c062.item062_245.target_id, target_id);
			else
				strncpy(cat_lib->category.c062.item062_245.target_id,
				        target_id,
				        sizeof(cat_lib->category.c062.item062_245.target_id) - 1);
			cat_lib->category.c062.item062_245.target_id_src = tid_src;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[1] |= 0x20;

			break;
		case CODT_CAT_011:
			res = 0;
			if (strlen(target_id) < sizeof(cat_lib->category.c011.item011_245.target_id))
				strcpy(cat_lib->category.c011.item011_245.target_id, target_id);
			else
				strncpy(cat_lib->category.c011.item011_245.target_id,
				        target_id,
				        sizeof(cat_lib->category.c011.item011_245.target_id) - 1);
			cat_lib->category.c011.item011_245.target_id_src = tid_src;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[1] |= 0x20;
			break;
		case CODT_CAT_010:
			res = 0;
			if (strlen(target_id) < sizeof(cat_lib->category.c010.item010_245.target_id))
				strcpy(cat_lib->category.c010.item010_245.target_id, target_id);
			else
				strncpy(cat_lib->category.c010.item010_245.target_id,
				        target_id,
				        sizeof(cat_lib->category.c010.item010_245.target_id) - 1);
			cat_lib->category.c010.item010_245.target_id_src = tid_src;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[1] |= 0x02;

			break;
		default:
			res = -1;
			break;
		}
	}
	return res;

}

/**
 * @brief Запись значения номера трассы в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param track_number - машинный номер трассы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackNumber(TCatAsterix* cat_lib, u_int32_t track_number)
{
	int res = -1;

	if ((cat_lib == NULL) || (track_number == 0))
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                             = 0;
			cat_lib->category.c062.item062_040.track_number = track_number;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[1] |= 0x08;

			break;
		case CODT_CAT_011:
			res                                             = 0;
			cat_lib->category.c011.item011_161.track_number = track_number;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[1] |= 0x08;
			break;
		case CODT_CAT_010:
			res                                             = 0;
			cat_lib->category.c010.item010_161.track_number = track_number;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[1] |= 0x20;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения адреса трасссы (24 бит. номер ИКАО) в структуру категории
 * @param cat_lib           - указатель на структуру категории
 * @param target_address    - значения адреса трасссы (24 бит. номер ИКАО)
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackTargetAddress(TCatAsterix* cat_lib, u_int32_t target_address)
{
	int res = -1;

	if ((cat_lib == NULL) || (target_address == 0))
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_010:
			res                                               = 0;
			cat_lib->category.c010.item010_220.target_address = target_address;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[1] |= 0x04;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения измеренного эшелона полета трека в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param measured_fligth_level - измеренный эшелон полета
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackMeasuredFlightLevel(TCatAsterix* cat_lib, double measured_fligth_level)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                                      = 0;
			cat_lib->category.c062.item062_136.measured_fligth_level = measured_fligth_level;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[2] |= 0x20;

			break;
		case CODT_CAT_011:
			res                                                      = 0;
			cat_lib->category.c011.item011_090.measured_fligth_level = measured_fligth_level;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[2] |= 0x40;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения статуса трассы в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param status  - указатель на структуру статуса трассы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackStatus(TCatAsterix* cat_lib, TTrackStatus* status)
{
	int res = -1;

	if ((cat_lib == NULL) || (status == NULL))
		return res;

	if ((cat_lib) && (status))
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:

			if (cat_lib->category.c062.item062_080.track_status == NULL)
				cat_lib->category.c062.item062_080.track_status = (TTrackStatus*)malloc(sizeof(TTrackStatus));
			if (cat_lib->category.c062.item062_080.track_status)
			{
				// bzero(cat_lib->category.c062.item062_080.track_status, sizeof(TTrackStatus));
				memset(cat_lib->category.c062.item062_080.track_status, 0, sizeof(TTrackStatus));
				memcpy(cat_lib->category.c062.item062_080.track_status, status, sizeof(TTrackStatus));
				res = 0;
				if (cat_lib->category.c062.item062_fspec.code)
					cat_lib->category.c062.item062_fspec.code[1] |= 0x04;
			}
			break;
		case CODT_CAT_011:
			res = 0;
			memcpy(&cat_lib->category.c011.item011_170.track_status, status, sizeof(TTrackStatus));
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[1] |= 0x04;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения времени устаревания системных данных о трассе в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param systime_age  - время устаревания системного времени
 *                       диапазон изменения 0.25 - 63.75 секунды
 *                       отсчитывается от последнего времени обновления соответствующих данных
 * @param time_type    - тип системного времени
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackSystemTimeAges(TCatAsterix* cat_lib, double systime_age, SystemTrackUpdateTimeType time_type)
{
	int res = -1;

	if ((cat_lib == NULL) || (systime_age == 0))
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res = 0;
			switch (time_type)
			{
			case STUTT_TRACK_AGE:
				cat_lib->category.c062.item062_290.trk_age = systime_age;
				break;
			case STUTT_PSR_AGE:
				cat_lib->category.c062.item062_290.psr_age = systime_age;
				break;
			case STUTT_SSR_AGE:
				cat_lib->category.c062.item062_290.ssr_age = systime_age;
				break;
			case STUTT_MODE_S_AGE:
				cat_lib->category.c062.item062_290.mds_age = systime_age;
				break;
			case STUTT_ADS_C_AGE:
				cat_lib->category.c062.item062_290.ads_age = systime_age;
				break;
			case STUTT_ADS_B_AGE:
				cat_lib->category.c062.item062_290.ads_es_age = systime_age;
				break;
			case STUTT_VDL_MODE4_AGE:
				cat_lib->category.c062.item062_290.vdl_age = systime_age;
				break;
			case STUTT_UAT_AGE:
				cat_lib->category.c062.item062_290.uat_age = systime_age;
				break;
			case STUTT_LOOP_AGE:
				cat_lib->category.c062.item062_290.loop_age = systime_age;
				break;
			case STUTT_MLAT_AGE:
				cat_lib->category.c062.item062_290.mlt_age = systime_age;
				break;
			default:
				res = -1;
				break;
			}
			if (res == 0)
				if (cat_lib->category.c062.item062_fspec.code)
					cat_lib->category.c062.item062_fspec.code[1] |= 0x02;
			break;
		case CODT_CAT_011:
			res = 0;
			switch (time_type)
			{
			case STUTT_TRACK_AGE:
				cat_lib->category.c011.item011_290.trk_age = systime_age;
				break;
			case STUTT_PSR_AGE:
				cat_lib->category.c011.item011_290.psr_age = systime_age;
				break;
			case STUTT_SSR_AGE:
				cat_lib->category.c011.item011_290.ssr_age = systime_age;
				break;
			case STUTT_MODE_S_AGE:
				cat_lib->category.c011.item011_290.mds_age = systime_age;
				break;
			case STUTT_ADS_C_AGE:
				cat_lib->category.c011.item011_290.ads_age = systime_age;
				break;
			case STUTT_ADS_B_AGE:
				cat_lib->category.c011.item011_290.ads_es_age = systime_age;
				break;
			case STUTT_MDA_AGE:
				cat_lib->category.c011.item011_290.mda_age = systime_age;
				break;
			case STUTT_MD1_AGE:
				cat_lib->category.c011.item011_290.md1_age = systime_age;
				break;
			case STUTT_MD2_AGE:
				cat_lib->category.c011.item011_290.md2_age = systime_age;
				break;
			case STUTT_MFL_AGE:
				cat_lib->category.c011.item011_290.mfl_age = systime_age;
				break;
			case STUTT_LOOP_AGE:
				cat_lib->category.c011.item011_290.loop_age = systime_age;
				break;
			case STUTT_MLAT_AGE:
				cat_lib->category.c011.item011_290.mlt_age = systime_age;
				break;
			default:
				res = -1;
				break;
			}
			if (res == 0)
				if (cat_lib->category.c011.item011_fspec.code)
					cat_lib->category.c011.item011_fspec.code[1] |= 0x02;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения времени устаревания данных о трассе в структуру категории
 *        полученных с борта воздушного средства
 * @param cat_lib - указатель на структуру категории
 * @param track_data_age  - время устаревания системного времени
 *                          диапазон изменения 0.25 - 63.75 секунды
 *                          отсчитывается от последнего времени обновления соответствующих данных
 * @param time_type    - тип времени трека
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackDataAges(TCatAsterix* cat_lib, double track_data_age, TrackDataAgesType age_time_type)
{
	int res = -1;

	if ((cat_lib == NULL) || (track_data_age == 0))
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res = 0;
			switch (age_time_type)
			{
			case TUAT_MFL_AGE:
				cat_lib->category.c062.item062_295.mfl_age = track_data_age;
				break;
			case TUAT_MD1_AGE:
				cat_lib->category.c062.item062_295.md1_age = track_data_age;
				break;
			case TUAT_MD2_AGE:
				cat_lib->category.c062.item062_295.md2_age = track_data_age;
				break;
			case TUAT_MDA_AGE:
				cat_lib->category.c062.item062_295.mda_age = track_data_age;
				break;
			case TUAT_MD4_AGE:
				cat_lib->category.c062.item062_295.md4_age = track_data_age;
				break;
			case TUAT_MD5_AGE:
				cat_lib->category.c062.item062_295.md5_age = track_data_age;
				break;
			case TUAT_MHG_AGE:
				cat_lib->category.c062.item062_295.mhg_age = track_data_age;
				break;
			case TUAT_IAS_AGE:
				cat_lib->category.c062.item062_295.ias_age = track_data_age;
				break;
			case TUAT_TAS_AGE:
				cat_lib->category.c062.item062_295.tas_age = track_data_age;
				break;
			case TUAT_SAL_AGE:
				cat_lib->category.c062.item062_295.sal_age = track_data_age;
				break;
			case TUAT_FSS_AGE:
				cat_lib->category.c062.item062_295.fss_age = track_data_age;
				break;
			case TUAT_TID_AGE:
				cat_lib->category.c062.item062_295.tid_age = track_data_age;
				break;
			case TUAT_COM_AGE:
				cat_lib->category.c062.item062_295.com_age = track_data_age;
				break;
			case TUAT_SAB_AGE:
				cat_lib->category.c062.item062_295.sab_age = track_data_age;
				break;
			case TUAT_ACS_AGE:
				cat_lib->category.c062.item062_295.acs_age = track_data_age;
				break;
			case TUAT_BVR_AGE:
				cat_lib->category.c062.item062_295.bvr_age = track_data_age;
				break;
			case TUAT_GVR_AGE:
				cat_lib->category.c062.item062_295.gvr_age = track_data_age;
				break;
			case TUAT_RAN_AGE:
				cat_lib->category.c062.item062_295.ran_age = track_data_age;
				break;
			case TUAT_TAR_AGE:
				cat_lib->category.c062.item062_295.tar_age = track_data_age;
				break;
			case TUAT_TAN_AGE:
				cat_lib->category.c062.item062_295.tan_age = track_data_age;
				break;
			case TUAT_GSP_AGE:
				cat_lib->category.c062.item062_295.gsp_age = track_data_age;
				break;
			case TUAT_VUN_AGE:
				cat_lib->category.c062.item062_295.vun_age = track_data_age;
				break;
			case TUAT_MET_AGE:
				cat_lib->category.c062.item062_295.met_age = track_data_age;
				break;
			case TUAT_EMC_AGE:
				cat_lib->category.c062.item062_295.emc_age = track_data_age;
				break;
			case TUAT_POS_AGE:
				cat_lib->category.c062.item062_295.pos_age = track_data_age;
				break;
			case TUAT_GAL_AGE:
				cat_lib->category.c062.item062_295.gal_age = track_data_age;
				break;
			case TUAT_PUN_AGE:
				cat_lib->category.c062.item062_295.pun_age = track_data_age;
				break;
			case TUAT_MB_AGE:
				cat_lib->category.c062.item062_295.mb_age = track_data_age;
				break;
			case TUAT_IAR_AGE:
				cat_lib->category.c062.item062_295.iar_age = track_data_age;
				break;
			case TUAT_MAC_AGE:
				cat_lib->category.c062.item062_295.mac_age = track_data_age;
				break;
			case TUAT_BPS_AGE:
				cat_lib->category.c062.item062_295.bps_age = track_data_age;
				break;
			default:
				res = -1;
				break;
			}

			if (res == 0)
				if (cat_lib->category.c062.item062_fspec.code)
					cat_lib->category.c062.item062_fspec.code[2] |= 0x40;

			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения измеренной геометрической высоты полета трека в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param geometric_altitude - измеренная наземными средствами, источник указывается
 *                             в статусе трассы
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackCalcGeometricAltitude(TCatAsterix* cat_lib, double geometric_altitude)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                              = 0;
			cat_lib->category.c062.item062_130.calc_g_height = geometric_altitude;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[2] |= 0x10;

			break;
		case CODT_CAT_011:
			res                                              = 0;
			cat_lib->category.c011.item011_092.calc_g_height = geometric_altitude;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[2] |= 0x10;
			break;
		case CODT_CAT_010:
			res                                              = 0;
			cat_lib->category.c010.item010_091.calc_g_height = geometric_altitude;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[2] |= 0x10;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись измеренной барометрической высоты полета трека в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param barometric_altitude - измеренная борту воздушного судна
 * @param qnh_correction - признак относительной барометрической высоты по QNH
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackCalcBarometricAltitude(TCatAsterix* cat_lib, double barometric_altitude, u_int8_t qnh_correction)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                               = 0;
			cat_lib->category.c062.item062_135.calc_b_height  = barometric_altitude;
			cat_lib->category.c062.item062_135.qnh_correction = qnh_correction;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[2] |= 0x08;

			break;
		case CODT_CAT_011:
			res                                               = 0;
			cat_lib->category.c011.item011_093.calc_b_height  = barometric_altitude;
			cat_lib->category.c011.item011_093.qnh_correction = qnh_correction;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[2] |= 0x20;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись скорости изменения высоты полета трека в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param vertical_rate - измеренная борту воздушного судна
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackCalcVerticalRate(TCatAsterix* cat_lib, double vertical_rate)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                   = 0;
			cat_lib->category.c062.item062_220.vh = vertical_rate;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[2] |= 0x04;

			break;
		case CODT_CAT_011:
			res                                   = 0;
			cat_lib->category.c011.item011_215.vh = vertical_rate;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[2] |= 0x08;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись данных плановой информации полета в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param fp_data - указатель на структуру плановой данных
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackFlightPlanData(TCatAsterix* cat_lib, TFlightPlanData* fp_data)
{
	int res = -1;

	if ((cat_lib == NULL) || (fp_data == NULL))
		return res;

	if ((cat_lib) && (fp_data))
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:

			if (cat_lib->category.c062.item062_390 == NULL)
				cat_lib->category.c062.item062_390 = (FlightPlanRelatedData*) malloc(sizeof(FlightPlanRelatedData));

			if (cat_lib->category.c062.item062_390)
			{
				res = 0;
				memset(cat_lib->category.c062.item062_390, 0, sizeof(FlightPlanRelatedData));
				memcpy(&cat_lib->category.c062.item062_390->fpdata, fp_data, sizeof(TFlightPlanData));
				if (cat_lib->category.c062.item062_fspec.code)
					cat_lib->category.c062.item062_fspec.code[2] |= 0x02;
			}
			else
			{
				cat_lib->category.c062.item062_390 = NULL;
			}
			break;
		case CODT_CAT_011:

			if (cat_lib->category.c011.item011_390 == NULL)
				cat_lib->category.c011.item011_390 = (FlightPlanRelatedData*) malloc(sizeof(FlightPlanRelatedData));

			if (cat_lib->category.c011.item011_390)
			{
				res = 0;
				memset(cat_lib->category.c011.item011_390, 0, sizeof(FlightPlanRelatedData));
				memcpy(&cat_lib->category.c011.item011_390->fpdata, fp_data, sizeof(TFlightPlanData));
				if (cat_lib->category.c011.item011_fspec.code)
					cat_lib->category.c011.item011_fspec.code[2] |= 0x02;
			}
			else
			{
				cat_lib->category.c062.item062_390 = NULL;
			}
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись данных о размерах и положении в прострастве в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param length - длина транспортного средства
 * @param width - ширина транспортного средства
 * @param orientation - положение транспортного средства в прострастве
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackTargetSize(TCatAsterix* cat_lib, u_int32_t length, u_int32_t width, float orientation)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                            = 0;
			cat_lib->category.c062.item062_270.length      = length;
			cat_lib->category.c062.item062_270.width       = width;
			cat_lib->category.c062.item062_270.orientation = orientation;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[3] |= 0x80;

			break;
		case CODT_CAT_011:
			res                                            = 0;
			cat_lib->category.c011.item011_270.length      = length;
			cat_lib->category.c011.item011_270.width       = width;
			cat_lib->category.c011.item011_270.orientation = orientation;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[2] |= 0x04;
			break;
		case CODT_CAT_010:
			res                                            = 0;
			cat_lib->category.c010.item010_270.length      = length;
			cat_lib->category.c010.item010_270.width       = width;
			cat_lib->category.c010.item010_270.orientation = orientation;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[2] |= 0x08;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись значения номера RBS Mode 2 в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param mode2_code - код номера в режиме 2
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackMode2code(TCatAsterix* cat_lib, BoardNumCode m2_code)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                   = 0;
			cat_lib->category.c062.item062_120.m2 = m2_code;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[3] |= 0x10;

			break;
		default:
			res = -1;
			break;
		}
	}
	return res;

}

/**
 * @brief Запись значения высоты полета полученного от борта ВО в структуру категории
 * @param cat_lib   - указатель на структуру категории
 * @param altitude  - значение высоты полета
 * @param mode_c_valid      - достоверность данных высоты (1 - данные достоверны, 0 - данные не достоверны)
 * @param mode_c_garbled    - искажение данных высоты (1 - данные искажены, 0 - данные не искажены)
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackModeCdata(TCatAsterix* cat_lib, BoardAltitudeCode mode_c)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_010:
			res                                          = 0;
			cat_lib->category.c010.item010_090.fl_binary = mode_c;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[2] |= 0x20;

			break;
		default:
			res = -1;
			break;
		}
	}
	return res;

}

/**
 * @brief Запись точностных данных местоположения трассы в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param vehicle_fleet_id - идентификатор транспортного стрества
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackVehicleFleetIdentification(TCatAsterix* cat_lib, u_int8_t vehicle_fleet_id)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                         = 0;
			cat_lib->category.c062.item062_300.fleet_id = vehicle_fleet_id;
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[3] |= 0x40;

			break;
		case CODT_CAT_011:
			res                                         = 0;
			cat_lib->category.c011.item011_300.fleet_id = vehicle_fleet_id;
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[3] |= 0x80;
			break;
		case CODT_CAT_010:
			res                                         = 0;
			cat_lib->category.c010.item010_300.fleet_id = vehicle_fleet_id;
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[2] |= 0x40;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись точностных данных местоположения трассы в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param accuracy - указатель на точностные данные
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackEstimateAccuracies(TCatAsterix* cat_lib, TTrackAccuracy* accuracy)
{
	int res = -1;

	if ((cat_lib == NULL) || (accuracy == NULL))
		return res;

	if ((cat_lib) && (accuracy))
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res = 0;
			memcpy(&cat_lib->category.c062.item062_500.accuracy, accuracy, sizeof(TTrackAccuracy));
			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[3] |= 0x04;

			break;
		case CODT_CAT_011:
			res = 0;
			memcpy(&cat_lib->category.c011.item011_500.accuracy, accuracy, sizeof(TTrackAccuracy));
			if (cat_lib->category.c011.item011_fspec.code)
				cat_lib->category.c011.item011_fspec.code[3] |= 0x20;
			break;
		case CODT_CAT_010:
			res = 0;
			memcpy(&cat_lib->category.c010.item010_500.accuracy, accuracy, sizeof(TTrackAccuracy));
			if (cat_lib->category.c010.item010_fspec.code)
				cat_lib->category.c010.item010_fspec.code[3] |= 0x80;
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись дополнительных данных траектории трассы ВО в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param sp_data - указатель на структуру дополнительных данных (УВД/НРЗ)
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackDefaultSpData(TCatAsterix* cat_lib, TSpData* sp_data)
{
	int res = -1;

	if ((cat_lib == NULL) || (sp_data == NULL))
		return res;

	if ((cat_lib) && (sp_data))
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:

			if (cat_lib->category.c062.item062_sp == NULL)
			{
				cat_lib->category.c062.item062_sp = (Cat062specialPuposeField*) malloc(sizeof(Cat062specialPuposeField));
				if (cat_lib->category.c062.item062_sp)
					// bzero(cat_lib->category.c062.item062_sp, sizeof(Cat062specialPuposeField));
					memset(cat_lib->category.c062.item062_sp, 0, sizeof(Cat062specialPuposeField));
			}

			if (cat_lib->category.c062.item062_sp)
			{
				if (cat_lib->category.c062.item062_sp->el_sp == NULL)
					cat_lib->category.c062.item062_sp->el_sp = (TSpData*) malloc(sizeof(TSpData));

				if (cat_lib->category.c062.item062_sp->el_sp)
				{
					// bzero(cat_lib->category.c062.item062_sp->el_sp, sizeof(TSpData));
					memset(cat_lib->category.c062.item062_sp->el_sp, 0, sizeof(TSpData));
					memcpy(cat_lib->category.c062.item062_sp->el_sp, sp_data, sizeof(TSpData));
					res = 0;
					if (cat_lib->category.c062.item062_fspec.code)
						cat_lib->category.c062.item062_fspec.code[4] |= 0x02;

				}
			}

			break;
		case CODT_CAT_011:

			if (cat_lib->category.c011.item011_sp == NULL)
			{
				cat_lib->category.c011.item011_sp = (Cat011specialPuposeField*) malloc(sizeof(Cat011specialPuposeField));
				if (cat_lib->category.c011.item011_sp)
					// bzero(cat_lib->category.c011.item011_sp, sizeof(Cat011specialPuposeField));
					memset(cat_lib->category.c011.item011_sp, 0, sizeof(Cat011specialPuposeField));
			}
			if (cat_lib->category.c011.item011_sp)
			{
				if (cat_lib->category.c011.item011_sp->el_sp == NULL)
					cat_lib->category.c011.item011_sp->el_sp = (TSpData*) malloc(sizeof(TSpData));
				if (cat_lib->category.c011.item011_sp->el_sp)
				{
					// bzero(cat_lib->category.c011.item011_sp->el_sp, sizeof(TSpData));
					memset(cat_lib->category.c011.item011_sp->el_sp, 0, sizeof(TSpData));
					res = 0;
					memcpy(cat_lib->category.c011.item011_sp->el_sp, sp_data, sizeof(TSpData));
					if (cat_lib->category.c011.item011_fspec.code)
						cat_lib->category.c011.item011_fspec.code[3] |= 0x02;
				}
			}
			break;
		case CODT_CAT_010:

			if (cat_lib->category.c010.item010_sp == NULL)
			{
				cat_lib->category.c010.item010_sp = (Cat010specialPuposeField*) malloc(sizeof(Cat010specialPuposeField));
				if (cat_lib->category.c010.item010_sp)
					// bzero(cat_lib->category.c010.item010_sp, sizeof(Cat010specialPuposeField));
					memset(cat_lib->category.c010.item010_sp, 0, sizeof(Cat010specialPuposeField));
			}
			if (cat_lib->category.c010.item010_sp)
			{
				if (cat_lib->category.c010.item010_sp->el_sp == NULL)
					cat_lib->category.c010.item010_sp->el_sp = (TSpData*) malloc(sizeof(TSpData));

				if (cat_lib->category.c010.item010_sp->el_sp)
				{
					// bzero(cat_lib->category.c010.item010_sp->el_sp, sizeof(TSpData));
					memset(cat_lib->category.c010.item010_sp->el_sp, 0, sizeof(TSpData));
					res = 0;
					memcpy(cat_lib->category.c010.item010_sp->el_sp, sp_data, sizeof(TSpData));
					if (cat_lib->category.c010.item010_fspec.code)
						cat_lib->category.c010.item010_fspec.code[3] |= 0x04;
				}
			}
			break;
		default:
			res = -1;
			break;
		}
	}
	return res;
}

/**
 * @brief Запись дополнительных данных траектории трассы ВО в структуру категории
 *        (данные закодированы пользователем и переданы в виде массива байт)
 * @param cat_lib - указатель на структуру категории
 * @param sp_data - указатель на массив байт с кодированными данными
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setTrackSpRawData(TCatAsterix* cat_lib, TCommonObmenData* sp_raw)
{
	int res = -1;

	if ((cat_lib == NULL) || (sp_raw == NULL))
		return res;

	if ((cat_lib) && (sp_raw))
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:

			if (cat_lib->category.c062.item062_sp == NULL)
			{
				cat_lib->category.c062.item062_sp = (Cat062specialPuposeField*) malloc(sizeof(Cat062specialPuposeField));
				if (cat_lib->category.c062.item062_sp)
					// bzero(cat_lib->category.c062.item062_sp, sizeof(Cat062specialPuposeField));
					memset(cat_lib->category.c062.item062_sp, 0, sizeof(Cat062specialPuposeField));
			}

			if (cat_lib->category.c062.item062_sp)
			{
				int       ccc     = getObmenDataLenght(sp_raw);
				u_int8_t* myspraw = (u_int8_t*) getObmenData(sp_raw);
				if (myspraw)
				{
					res = setElementHeader(&cat_lib->category.c062.item062_sp->head,
					                       CAT062_ELSP, ccc, myspraw, "Raw data");
					if (res <= 0)
						res = -1;
					if (res > 0)
					{
						res = 0;
						if (cat_lib->category.c062.item062_fspec.code)
							cat_lib->category.c062.item062_fspec.code[4] |= 0x02;
					}
				}
			}
			break;
		default:
			res = -1;
			break;
		}
	}

	freeObmenData(sp_raw);
	return res;
}

/**
 * @brief Запрос идентификатора источника в структуре категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 1
 */
TReturnOneInt getDataSourceIdentificator(TCatAsterix* cat_lib)
{
	TReturnOneInt ret;

	// bzero(&ret, sizeof(TReturnOneInt));
	memset(&ret, 0, sizeof(TReturnOneInt));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_010.sac_sic != DEFAULT_U_INT16_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_010.sac_sic;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_010.sac_sic != DEFAULT_U_INT16_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_010.sac_sic;
			}
			break;
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_010.sac_sic != DEFAULT_U_INT16_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c010.item010_010.sac_sic;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос типа сервиса для сообщения категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 1
 */
TReturnOneInt getServiceIdentificator(TCatAsterix* cat_lib)
{
	TReturnOneInt ret;

	// bzero(&ret, sizeof(TReturnOneInt));
	memset(&ret, 0, sizeof(TReturnOneInt));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_015.sevice_id != DEFAULT_U_INT8_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_015.sevice_id;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_015.sevice_id != DEFAULT_U_INT8_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_015.sevice_id;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос типа сервиса для сообщения категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 2
 */
TReturnOneDouble getTimeOfTrackInformation(TCatAsterix* cat_lib)
{
	TReturnOneDouble ret;

	// bzero(&ret, sizeof(TReturnOneDouble));
	memset(&ret, 0, sizeof(TReturnOneDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if ((cat_lib->category.c062.item062_070.time_of_track < DEFAULT_TIME_OF_TRACK) &&
			    (cat_lib->category.c062.item062_070.time_of_track >= 0))
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_070.time_of_track;
			}
			break;
		case CODT_CAT_011:
			if ((cat_lib->category.c011.item011_140.time_of_track < DEFAULT_TIME_OF_TRACK) &&
			    (cat_lib->category.c011.item011_140.time_of_track >= 0))
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_140.time_of_track;
			}
			break;
		case CODT_CAT_010:
			if ((cat_lib->category.c010.item010_140.time_of_track < DEFAULT_TIME_OF_TRACK) &&
			    (cat_lib->category.c010.item010_140.time_of_track >= 0))
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c010.item010_140.time_of_track;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос данных положения ВО в географических координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение широты в радианах
 *         переменная val2 - содержит значение долготы в радианах
 */
TReturnPairDouble getTrackPositionWgs84(TCatAsterix* cat_lib)
{
	TReturnPairDouble ret;

	// bzero(&ret, sizeof(TReturnPairDouble));
	memset(&ret, 0, sizeof(TReturnPairDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if ((cat_lib->category.c062.item062_105.latitude != DEFAULT_RADIAN_GEO_POS) &&
			    (cat_lib->category.c062.item062_105.longitude != DEFAULT_RADIAN_GEO_POS))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c062.item062_105.latitude;
				ret.val2   = cat_lib->category.c062.item062_105.longitude;
			}
			break;
		case CODT_CAT_011:
			if ((cat_lib->category.c011.item011_041.latitude != DEFAULT_RADIAN_GEO_POS) &&
			    (cat_lib->category.c011.item011_041.longitude != DEFAULT_RADIAN_GEO_POS))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c011.item011_041.latitude;
				ret.val2   = cat_lib->category.c011.item011_041.longitude;
			}
			break;
		case CODT_CAT_010:
			if ((cat_lib->category.c010.item010_041.latitude != DEFAULT_RADIAN_GEO_POS) &&
			    (cat_lib->category.c010.item010_041.longitude != DEFAULT_RADIAN_GEO_POS))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c010.item010_041.latitude;
				ret.val2   = cat_lib->category.c010.item010_041.longitude;
			}
			break;
		default:
			ret.update = 0;
			ret.val1   = 0;
			ret.val2   = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос данных положения ВО в прямоугольных координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 3:
 *         переменная val1 - содержит значение координаты Х, в метрах
 *         переменная val2 - содержит значение координаты У, в метрах
 */
TReturnPairInt getTrackPositionCartesian(TCatAsterix* cat_lib)
{
	TReturnPairInt ret;

	// bzero(&ret, sizeof(TReturnPairInt));
	memset(&ret, 0, sizeof(TReturnPairInt));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if ((cat_lib->category.c062.item062_100.x != DEFAULT_CARTESIAN_POS) &&
			    (cat_lib->category.c062.item062_100.y != DEFAULT_CARTESIAN_POS))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c062.item062_100.x;
				ret.val2   = cat_lib->category.c062.item062_100.y;
			}
			break;
		case CODT_CAT_011:
			if ((cat_lib->category.c011.item011_042.x != DEFAULT_CARTESIAN_POS) &&
			    (cat_lib->category.c011.item011_042.y != DEFAULT_CARTESIAN_POS))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c011.item011_042.x;
				ret.val2   = cat_lib->category.c011.item011_042.y;
			}
			break;
		case CODT_CAT_010:
			if ((cat_lib->category.c010.item010_042.x != DEFAULT_CARTESIAN_POS) &&
			    (cat_lib->category.c010.item010_042.y != DEFAULT_CARTESIAN_POS))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c010.item010_042.x;
				ret.val2   = cat_lib->category.c010.item010_042.y;
			}
			break;
		default:
			ret.update = 0;
			ret.val1   = 0;
			ret.val2   = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос данных положения ВО в полярных координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение азимута, в радианах
 *         переменная val2 - содержит значение дальности, в метрах
 */
TReturnPairDouble getTrackPositionPolar(TCatAsterix* cat_lib)
{
	TReturnPairDouble ret;

	// bzero(&ret, sizeof(TReturnPairDouble));
	memset(&ret, 0, sizeof(TReturnPairDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_010:
			if ((cat_lib->category.c010.item010_040.polar.azimuth != DEFAULT_RADIAN_AZIMUTH_POS) &&
			    (cat_lib->category.c010.item010_040.polar.distance != DEFAULT_CARTESIAN_POS))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c010.item010_040.polar.azimuth;
				ret.val2   = cat_lib->category.c010.item010_040.polar.distance;
			}
			break;
		default:
			ret.update = 0;
			ret.val1   = 0;
			ret.val2   = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос скорости движения ВО в полярной системе координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение скорости, метр/секунда
 *         переменная val2 - содержит значение курса, радианы
 */
TReturnPairDouble getTrackVelocityPolar(TCatAsterix* cat_lib)
{
	TReturnPairDouble ret;

	// bzero(&ret, sizeof(TReturnPairDouble));
	memset(&ret, 0, sizeof(TReturnPairDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_010:
			if ((cat_lib->category.c010.item010_200.ground_speed != DEFAULT_VELOCITY) &&
			    (cat_lib->category.c010.item010_200.track_angle != DEFAULT_RADIAN_AZIMUTH_POS))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c010.item010_200.ground_speed;
				ret.val2   = cat_lib->category.c010.item010_200.track_angle;
			}
			break;
		default:
			ret.update = 0;
			ret.val1   = 0;
			ret.val2   = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос скорости движения ВО в прямоугольных координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение скорости по оси Х, метр/секунда
 *         переменная val2 - содержит значение скорости по оси У, метр/секунда
 */
TReturnPairDouble getTrackVelocityCartesian(TCatAsterix* cat_lib)
{
	TReturnPairDouble ret;

	// bzero(&ret, sizeof(TReturnPairDouble));
	memset(&ret, 0, sizeof(TReturnPairDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if ((cat_lib->category.c062.item062_185.vx != DEFAULT_VELOCITY) &&
			    (cat_lib->category.c062.item062_185.vy != DEFAULT_VELOCITY))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c062.item062_185.vx;
				ret.val2   = cat_lib->category.c062.item062_185.vy;
			}
			break;
		case CODT_CAT_011:
			if ((cat_lib->category.c011.item011_202.vx != DEFAULT_VELOCITY) &&
			    (cat_lib->category.c011.item011_202.vy != DEFAULT_VELOCITY))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c011.item011_202.vx;
				ret.val2   = cat_lib->category.c011.item011_202.vy;
			}
			break;
		case CODT_CAT_010:
			if ((cat_lib->category.c010.item010_202.vx != DEFAULT_VELOCITY) &&
			    (cat_lib->category.c010.item010_202.vy != DEFAULT_VELOCITY))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c010.item010_202.vx;
				ret.val2   = cat_lib->category.c010.item010_202.vy;
			}
			break;
		default:
			ret.update = 0;
			ret.val1   = 0;
			ret.val2   = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос ускорения движения ВО в прямоугольных координатах из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата 4:
 *         переменная val1 - содержит значение ускорения по оси Х, метр/секунда^2
 *         переменная val2 - содержит значение ускорения по оси У, метр/секунда^2
 */
TReturnPairDouble getTrackAccelerationCartesian(TCatAsterix* cat_lib)
{
	TReturnPairDouble ret;

	// bzero(&ret, sizeof(TReturnPairDouble));
	memset(&ret, 0, sizeof(TReturnPairDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if ((cat_lib->category.c062.item062_210.ax != DEFAULT_ACCELERATION) &&
			    (cat_lib->category.c062.item062_210.ay != DEFAULT_ACCELERATION))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c062.item062_210.ax;
				ret.val2   = cat_lib->category.c062.item062_210.ay;
			}
			break;
		case CODT_CAT_011:
			if ((cat_lib->category.c011.item011_210.ax != DEFAULT_ACCELERATION) &&
			    (cat_lib->category.c011.item011_210.ay != DEFAULT_ACCELERATION))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c011.item011_210.ax;
				ret.val2   = cat_lib->category.c011.item011_210.ay;
			}
			break;
		case CODT_CAT_010:
			if ((cat_lib->category.c010.item010_210.ax != DEFAULT_ACCELERATION) &&
			    (cat_lib->category.c010.item010_210.ay != DEFAULT_ACCELERATION))
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c010.item010_210.ax;
				ret.val2   = cat_lib->category.c010.item010_210.ay;
			}
			break;
		default:
			ret.update = 0;
			ret.val1   = 0;
			ret.val2   = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос данных бортового номера ВО в режиме Mode 3/A из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных бортового номера, если номер борта равен 0
 *         данный элемент в сообщении не присутствовал
 */
BoardNumCode getTrackMode3Acode(TCatAsterix* cat_lib)
{
	BoardNumCode ret;

	// bzero(&ret, sizeof(BoardNumCode));
	memset(&ret, 0, sizeof(BoardNumCode));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_060.m3a.board_number_code > DEFAULT_RBS_UVD_NUM_CODE)
			{
				ret = cat_lib->category.c062.item062_060.m3a;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_060.m3a.board_number_code > DEFAULT_RBS_UVD_NUM_CODE)
			{
				ret = cat_lib->category.c011.item011_060.m3a;
			}
			break;
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_060.m3a.board_number_code > DEFAULT_RBS_UVD_NUM_CODE)
			{
				ret = cat_lib->category.c010.item010_060.m3a;
			}
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос бортовой высоты полета полученного от борта ВО из категории
 * @param cat_lib   - указатель на структуру категории
 * @return структура бортовой высоты, при отсутствии данных все значения структуры
 *         обнулены
 */
BoardAltitudeCode getTrackModeCdata(TCatAsterix* cat_lib)
{
	BoardAltitudeCode ret;

	// bzero(&ret, sizeof(BoardAltitudeCode));
	memset(&ret, 0, sizeof(BoardAltitudeCode));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_010:
			ret = cat_lib->category.c010.item010_090.fl_binary;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос данных идентификатора ВО
 *        (позывной полученный вне данных
 *          плановой информации) категории
 * @param cat_lib - указатель на структуру категории
 * @param[out] tid_src - указатель на признак источника идентификатора
 * @return указатель на структура данных возврата
 */
TCommonObmenData* getTrackTargetIdentification(TCatAsterix* cat_lib, u_int8_t* tid_src)
{
	TCommonObmenData* ret  = NULL;
	int               size = 0;


	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			size = strlen(cat_lib->category.c062.item062_245.target_id);
			if (size > 0)
			{
				ret = initObmenData(size);
				if (ret)
					setObmenData(ret, cat_lib->category.c062.item062_245.target_id, size);
				if (tid_src)
					*tid_src = cat_lib->category.c062.item062_245.target_id_src;
			}
			else if (tid_src)
				*tid_src = TIS_UNDEFINED;
			break;
		case CODT_CAT_011:
			size = strlen(cat_lib->category.c011.item011_245.target_id);
			if (size > 0)
			{
				ret = initObmenData(size);
				if (ret)
					setObmenData(ret, cat_lib->category.c011.item011_245.target_id, size);
				if (tid_src)
					*tid_src = cat_lib->category.c011.item011_245.target_id_src;
			}
			else if (tid_src)
				*tid_src = TIS_UNDEFINED;
			break;
		case CODT_CAT_010:
			size = strlen(cat_lib->category.c010.item010_245.target_id);
			if (size > 0)
			{
				ret = initObmenData(size);
				if (ret)
					setObmenData(ret, cat_lib->category.c010.item010_245.target_id, size);
				if (tid_src)
					*tid_src = cat_lib->category.c010.item010_245.target_id_src;
			}
			else if (tid_src)
				*tid_src = TIS_UNDEFINED;
			break;
		default:
			ret = NULL;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос значения адреса трасссы (24 бит. номер ИКАО) из категории
 * @param cat_lib           - указатель на структуру категории
 * @return структура данных возврата:
 *         переменная val содержит значение адреса трасссы (24 бит. номер ИКАО)
 */
TReturnOneInt getTrackTargetAddress(TCatAsterix* cat_lib)
{
	TReturnOneInt ret;


	// bzero(&ret, sizeof(TReturnOneInt));
	memset(&ret, 0, sizeof(TReturnOneInt));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_220.target_address > 0)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c010.item010_220.target_address;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос номера трассы ВО в структуре категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *         переменная val содержит номер трассы ВО
 */
TReturnOneInt getTrackNumber(TCatAsterix* cat_lib)
{
	TReturnOneInt ret;


	// bzero(&ret, sizeof(TReturnOneInt));
	memset(&ret, 0, sizeof(TReturnOneInt));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_040.track_number > 0)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_040.track_number;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_161.track_number > 0)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_161.track_number;
			}
			break;
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_161.track_number > 0)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c010.item010_161.track_number;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос статуса трассы ВО в структуре категории
 * @param cat_lib - указатель на структуру категории
 * @return указатель на структуру данных статуса трассы ВО
 *         переменная val содержит номер трассы ВО
 */
TTrackStatus* getTrackStatus(TCatAsterix* cat_lib)
{
	TTrackStatus* ret = NULL;


	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			ret = cat_lib->category.c062.item062_080.track_status;
			break;
		case CODT_CAT_011:
			ret = &cat_lib->category.c011.item011_170.track_status;
			break;
		default:
			ret = NULL;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос времени устаревания данных о трассе в структуре категории
 *        полученных с борта воздушного средства
 * @param cat_lib - указатель на структуру категории
 * @param time_type    - тип времени трека
 * @return структура вовращения данных:
 *          переменная val содержит время устаревания данных
 *                          диапазон изменения 0.25 - 63.75 секунды
 *                          отсчитывается от последнего времени обновления соответствующих данных
 */
TReturnOneDouble getTrackDataAges(TCatAsterix* cat_lib, TrackDataAgesType age_time_type)
{
	TReturnOneDouble ret;
	double           ddd = 0.0;


	// bzero(&ret, sizeof(TReturnOneDouble));
	memset(&ret, 0, sizeof(TReturnOneDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			switch (age_time_type)
			{
			case TUAT_MFL_AGE:
				ddd = cat_lib->category.c062.item062_295.mfl_age;
				break;
			case TUAT_MD1_AGE:
				ddd = cat_lib->category.c062.item062_295.md1_age;
				break;
			case TUAT_MD2_AGE:
				ddd = cat_lib->category.c062.item062_295.md2_age;
				break;
			case TUAT_MDA_AGE:
				ddd = cat_lib->category.c062.item062_295.mda_age;
				break;
			case TUAT_MD4_AGE:
				ddd = cat_lib->category.c062.item062_295.md4_age;
				break;
			case TUAT_MD5_AGE:
				ddd = cat_lib->category.c062.item062_295.md5_age;
				break;
			case TUAT_MHG_AGE:
				ddd = cat_lib->category.c062.item062_295.mhg_age;
				break;
			case TUAT_IAS_AGE:
				ddd = cat_lib->category.c062.item062_295.ias_age;
				break;
			case TUAT_TAS_AGE:
				ddd = cat_lib->category.c062.item062_295.tas_age;
				break;
			case TUAT_SAL_AGE:
				ddd = cat_lib->category.c062.item062_295.sal_age;
				break;
			case TUAT_FSS_AGE:
				ddd = cat_lib->category.c062.item062_295.fss_age;
				break;
			case TUAT_TID_AGE:
				ddd = cat_lib->category.c062.item062_295.tid_age;
				break;
			case TUAT_COM_AGE:
				ddd = cat_lib->category.c062.item062_295.com_age;
				break;
			case TUAT_SAB_AGE:
				ddd = cat_lib->category.c062.item062_295.sab_age;
				break;
			case TUAT_ACS_AGE:
				ddd = cat_lib->category.c062.item062_295.acs_age;
				break;
			case TUAT_BVR_AGE:
				ddd = cat_lib->category.c062.item062_295.bvr_age;
				break;
			case TUAT_GVR_AGE:
				ddd = cat_lib->category.c062.item062_295.gvr_age;
				break;
			case TUAT_RAN_AGE:
				ddd = cat_lib->category.c062.item062_295.ran_age;
				break;
			case TUAT_TAR_AGE:
				ddd = cat_lib->category.c062.item062_295.tar_age;
				break;
			case TUAT_TAN_AGE:
				ddd = cat_lib->category.c062.item062_295.tan_age;
				break;
			case TUAT_GSP_AGE:
				ddd = cat_lib->category.c062.item062_295.gsp_age;
				break;
			case TUAT_VUN_AGE:
				ddd = cat_lib->category.c062.item062_295.vun_age;
				break;
			case TUAT_MET_AGE:
				ddd = cat_lib->category.c062.item062_295.met_age;
				break;
			case TUAT_EMC_AGE:
				ddd = cat_lib->category.c062.item062_295.emc_age;
				break;
			case TUAT_POS_AGE:
				ddd = cat_lib->category.c062.item062_295.pos_age;
				break;
			case TUAT_GAL_AGE:
				ddd = cat_lib->category.c062.item062_295.gal_age;
				break;
			case TUAT_PUN_AGE:
				ddd = cat_lib->category.c062.item062_295.pun_age;
				break;
			case TUAT_MB_AGE:
				ddd = cat_lib->category.c062.item062_295.mb_age;
				break;
			case TUAT_IAR_AGE:
				ddd = cat_lib->category.c062.item062_295.iar_age;
				break;
			case TUAT_MAC_AGE:
				ddd = cat_lib->category.c062.item062_295.mac_age;
				break;
			case TUAT_BPS_AGE:
				ddd = cat_lib->category.c062.item062_295.bps_age;
				break;
			default:
				ddd = DEFAULT_TIME_AGES;
				break;
			}
			if (ddd != DEFAULT_TIME_AGES)
			{
				ret.update = 1;
				ret.val    = ddd;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запись значения времени устаревания системных данных о трассе в структуру категории
 * @param cat_lib - указатель на структуру категории
 * @param time_type    - тип системного времени
 * @return возвращаемая структура:
 *         переменная val соедержит время устаревания системных данных
 *                       диапазон изменения 0.25 - 63.75 секунды
 *                       отсчитывается от последнего времени обновления соответствующих данных
 */
TReturnOneDouble getTrackSystemTimeAges(TCatAsterix* cat_lib, SystemTrackUpdateTimeType time_type)
{
	TReturnOneDouble ret;
	double           ddd = 0.0;


	// bzero(&ret, sizeof(TReturnOneDouble));
	memset(&ret, 0, sizeof(TReturnOneDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			switch (time_type)
			{
			case STUTT_TRACK_AGE:
				ddd = cat_lib->category.c062.item062_290.trk_age;
				break;
			case STUTT_PSR_AGE:
				ddd = cat_lib->category.c062.item062_290.psr_age;
				break;
			case STUTT_SSR_AGE:
				ddd = cat_lib->category.c062.item062_290.ssr_age;
				break;
			case STUTT_MODE_S_AGE:
				ddd = cat_lib->category.c062.item062_290.mds_age;
				break;
			case STUTT_ADS_C_AGE:
				ddd = cat_lib->category.c062.item062_290.ads_age;
				break;
			case STUTT_ADS_B_AGE:
				ddd = cat_lib->category.c062.item062_290.ads_es_age;
				break;
			case STUTT_VDL_MODE4_AGE:
				ddd = cat_lib->category.c062.item062_290.vdl_age;
				break;
			case STUTT_UAT_AGE:
				ddd = cat_lib->category.c062.item062_290.uat_age;
				break;
			case STUTT_LOOP_AGE:
				ddd = cat_lib->category.c062.item062_290.loop_age;
				break;
			case STUTT_MLAT_AGE:
				ddd = cat_lib->category.c062.item062_290.mlt_age;
				break;
			default:
				ddd = DEFAULT_TIME_AGES;
				break;
			}
			if (ddd != DEFAULT_TIME_AGES)
			{
				ret.update = 1;
				ret.val    = ddd;
			}
			break;
		case CODT_CAT_011:
			switch (time_type)
			{
			case STUTT_TRACK_AGE:
				ddd = cat_lib->category.c011.item011_290.trk_age;
				break;
			case STUTT_PSR_AGE:
				ddd = cat_lib->category.c011.item011_290.psr_age;
				break;
			case STUTT_SSR_AGE:
				ddd = cat_lib->category.c011.item011_290.ssr_age;
				break;
			case STUTT_MODE_S_AGE:
				ddd = cat_lib->category.c011.item011_290.mds_age;
				break;
			case STUTT_ADS_C_AGE:
				ddd = cat_lib->category.c011.item011_290.ads_age;
				break;
			case STUTT_ADS_B_AGE:
				ddd = cat_lib->category.c011.item011_290.ads_es_age;
				break;
			case STUTT_MD1_AGE:
				ddd = cat_lib->category.c011.item011_290.md1_age;
				break;
			case STUTT_MD2_AGE:
				ddd = cat_lib->category.c011.item011_290.md2_age;
				break;
			case STUTT_MDA_AGE:
				ddd = cat_lib->category.c011.item011_290.mda_age;
				break;
			case STUTT_MFL_AGE:
				ddd = cat_lib->category.c011.item011_290.mfl_age;
				break;
			case STUTT_LOOP_AGE:
				ddd = cat_lib->category.c011.item011_290.loop_age;
				break;
			case STUTT_MLAT_AGE:
				ddd = cat_lib->category.c011.item011_290.mlt_age;
				break;
			default:
				ddd = DEFAULT_TIME_AGES;
				break;
			}
			if (ddd != DEFAULT_TIME_AGES)
			{
				ret.update = 1;
				ret.val    = ddd;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос измеренного эшелона полета ВО из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение эшелона полета ВО, в метрах
 */
TReturnOneDouble getTrackMeasuredFlightLevel(TCatAsterix* cat_lib)
{
	TReturnOneDouble ret;


	// bzero(&ret, sizeof(TReturnOneDouble));
	memset(&ret, 0, sizeof(TReturnOneDouble));


	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_136.measured_fligth_level < DEFAULT_ALTITUDE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_136.measured_fligth_level;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_090.measured_fligth_level < DEFAULT_ALTITUDE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_090.measured_fligth_level;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос расчитанной геометрической высоты полета ВО из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение геометрической высоты полета ВО, в метрах
 */
TReturnOneDouble getTrackCalcGeometricAltitude(TCatAsterix* cat_lib)
{
	TReturnOneDouble ret;


	// bzero(&ret, sizeof(TReturnOneDouble));
	memset(&ret, 0, sizeof(TReturnOneDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_130.calc_g_height < DEFAULT_ALTITUDE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_130.calc_g_height;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_092.calc_g_height < DEFAULT_ALTITUDE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_092.calc_g_height;
			}
			break;
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_091.calc_g_height < DEFAULT_ALTITUDE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c010.item010_091.calc_g_height;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос расчитанной барометрической высоты полета ВО из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 */
TReturnPairInt getTrackCalcBarometricAltitude(TCatAsterix* cat_lib)
{
	TReturnPairInt ret;

	// bzero(&ret, sizeof(TReturnPairInt));
	memset(&ret, 0, sizeof(TReturnPairInt));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_135.calc_b_height < DEFAULT_ALTITUDE)
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c062.item062_135.calc_b_height;
				ret.val2   = cat_lib->category.c062.item062_135.qnh_correction;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_093.calc_b_height < DEFAULT_ALTITUDE)
			{
				ret.update = 1;
				ret.val1   = cat_lib->category.c011.item011_093.calc_b_height;
				ret.val2   = cat_lib->category.c011.item011_093.qnh_correction;
			}
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос расчитанной вертикальной (по высоте) скорости полета ВО из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение вертикальной скорости полета ВО, метр/секуда
 */
TReturnOneDouble getTrackCalcVerticalRate(TCatAsterix* cat_lib)
{
	TReturnOneDouble ret;


	// bzero(&ret, sizeof(TReturnOneDouble));
	memset(&ret, 0, sizeof(TReturnOneDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_220.vh < DEFAULT_VELOCITY)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_220.vh;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_215.vh < DEFAULT_VELOCITY)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_215.vh;
			}
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос данных плановой информации полета из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - указатель на структуру плановых данных
 */
TFlightPlanData* getTrackFlightPlanData(TCatAsterix* cat_lib)
{
	TFlightPlanData* ret = NULL;


	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_390)
				ret = &cat_lib->category.c062.item062_390->fpdata;
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_390)
				ret = &cat_lib->category.c011.item011_390->fpdata;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос размера объекта (длины) из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение длины транспортного средства (ВО), в метрах
 */
TReturnOneInt getTrackTargetSizeLenght(TCatAsterix* cat_lib)
{
	TReturnOneInt ret;


	// bzero(&ret, sizeof(TReturnOneInt));
	memset(&ret, 0, sizeof(TReturnOneInt));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_270.length < DEFAULT_U_INT8_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_270.length;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_270.length < DEFAULT_U_INT8_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_270.length;
			}
			break;
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_270.length < DEFAULT_U_INT8_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c010.item010_270.length;
			}
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос размера объекта (ширина) из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение ширины транспортного средства (ВО), в метрах
 */
TReturnOneInt getTrackTargetSizeWidth(TCatAsterix* cat_lib)
{
	TReturnOneInt ret;


	// bzero(&ret, sizeof(TReturnOneInt));
	memset(&ret, 0, sizeof(TReturnOneInt));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_270.width < DEFAULT_U_INT8_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_270.width;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_270.width < DEFAULT_U_INT8_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_270.width;
			}
			break;
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_270.width < DEFAULT_U_INT8_CODE)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c010.item010_270.width;
			}
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос размера объекта (ширина) из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *          переменная val содержит значение ширины транспортного средства (ВО), в метрах
 */
TReturnOneDouble getTrackTargetSizeOrientation(TCatAsterix* cat_lib)
{
	TReturnOneDouble ret;

	// bzero(&ret, sizeof(TReturnOneDouble));
	memset(&ret, 0, sizeof(TReturnOneDouble));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_270.orientation < DEFAULT_RADIAN_AZIMUTH_POS)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_270.orientation;
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_270.orientation < DEFAULT_RADIAN_AZIMUTH_POS)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_270.orientation;
			}
			break;
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_270.orientation < DEFAULT_RADIAN_AZIMUTH_POS)
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c010.item010_270.orientation;
			}
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос данных бортового номера ВО в режиме Mode 2 из категории
 * @param cat_lib - указатель на структуру категории
 * @return структура данных возврата:
 *         переменная val - содержит значение бортового номера Mode 2
 */
BoardNumCode getTrackMode2code(TCatAsterix* cat_lib)
{
	BoardNumCode ret;

	// bzero(&ret, sizeof(BoardNumCode));
	memset(&ret, 0, sizeof(BoardNumCode));

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_120.m2.board_number_code > DEFAULT_RBS_UVD_NUM_CODE)
			{
				ret = cat_lib->category.c062.item062_120.m2;
			}
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос точностных данных трассовой информации из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - указатель на структуру точностных данных
 */
TTrackAccuracy* getTrackEstimateAccuracies(TCatAsterix* cat_lib)
{
	TTrackAccuracy* ret = NULL;


	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			ret = &cat_lib->category.c062.item062_500.accuracy;
			break;
		case CODT_CAT_011:
			ret = &cat_lib->category.c011.item011_500.accuracy;
			break;
		case CODT_CAT_010:
			ret = &cat_lib->category.c010.item010_500.accuracy;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос точностных данных трассовой информации из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - структура данных возврата:
 *         переменная val - содержит значение идентификатора транспортного средства
 */
TReturnOneInt getTrackVehicleFleetIdentification(TCatAsterix* cat_lib)
{
	TReturnOneInt ret;

	ret.update = 0;
	ret.val    = 0;

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if ((cat_lib->category.c062.item062_300.fleet_id < DEFAULT_U_INT8_CODE) &&
			    (cat_lib->category.c062.item062_300.fleet_id > 0))
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c062.item062_300.fleet_id;
			}
			break;
		case CODT_CAT_011:
			if ((cat_lib->category.c011.item011_300.fleet_id < DEFAULT_U_INT8_CODE) &&
			    (cat_lib->category.c011.item011_300.fleet_id > 0))
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c011.item011_300.fleet_id;
			}
			break;
		case CODT_CAT_010:
			if ((cat_lib->category.c010.item010_300.fleet_id < DEFAULT_U_INT8_CODE) &&
			    (cat_lib->category.c010.item010_300.fleet_id > 0))
			{
				ret.update = 1;
				ret.val    = cat_lib->category.c010.item010_300.fleet_id;
			}
			break;
		default:
			ret.update = 0;
			ret.val    = 0;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос результатов распаковки поля SP из структуры категории
 * @param cat_lib - указатель на структуру категории
 * @return  - указатель на структуру данных результатов распаковки поля SP
 *            (распаковка данных по умолчанию)
 */
TSpData* getTrackDefaultSpData(TCatAsterix* cat_lib)
{
	TSpData* ret = NULL;


	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_sp)
				if (cat_lib->category.c062.item062_sp->el_sp)
					ret = cat_lib->category.c062.item062_sp->el_sp;
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_sp)
				if (cat_lib->category.c011.item011_sp->el_sp)
					ret = cat_lib->category.c011.item011_sp->el_sp;
			break;
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_sp)
				if (cat_lib->category.c010.item010_sp->el_sp)
					ret = cat_lib->category.c010.item010_sp->el_sp;
			break;
		}
	}
	return ret;
}

/**
 * @brief Запрос содержимого поля SP из из структуры категории
 *        (нераспакованные данные поля SP, полученные в сообщении категории)
 * @param cat_lib - указатель на структуру категории
 * @return указатель на структуру данных возврата
 */
TCommonObmenData* getTrackSpRawData(TCatAsterix* cat_lib)
{
	TCommonObmenData* ret = NULL;


	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			if (cat_lib->category.c062.item062_sp)
			{
				if (cat_lib->category.c062.item062_sp->head.code_size > 0)
				{
					ret = initObmenData(cat_lib->category.c062.item062_sp->head.code_size);
					if (ret)
						setObmenData(ret,
						             cat_lib->category.c062.item062_sp->head.code,
						             cat_lib->category.c062.item062_sp->head.code_size);
				}
			}
			break;
		case CODT_CAT_011:
			if (cat_lib->category.c011.item011_sp)
			{
				if (cat_lib->category.c011.item011_sp->head.code_size > 0)
				{
					ret = initObmenData(cat_lib->category.c011.item011_sp->head.code_size);
					if (ret)
						setObmenData(ret,
						             cat_lib->category.c011.item011_sp->head.code,
						             cat_lib->category.c011.item011_sp->head.code_size);
				}
			}
			break;
		case CODT_CAT_010:
			if (cat_lib->category.c010.item010_sp)
			{
				if (cat_lib->category.c010.item010_sp->head.code_size > 0)
				{
					ret = initObmenData(cat_lib->category.c010.item010_sp->head.code_size);
					if (ret)
						setObmenData(ret,
						             cat_lib->category.c010.item010_sp->head.code,
						             cat_lib->category.c010.item010_sp->head.code_size);
				}
			}
			break;
		default:
			ret = NULL;
			break;
		}
	}
	return ret;
}

/**
 * @brief Установка обработки контрольной суммы для сообщения Астерикс
 * @param cat_lib - указатель на структуру категории
 * @param use_control_summ - код включения контрольной суммы
 *                           0 - не использовать контрольную сумму
 *                           1 - использовать контрольную сумму
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setControlSumMode(TCatAsterix* cat_lib, u_int8_t use_control_summ)
{
	int res = -1;


	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (use_control_summ)
		{
		case 0:
		case 1:
			res                       = 0;
			cat_lib->create_check_sum = use_control_summ;
			break;
		}
	}
	return res;
}

/**
 * @brief Установка проведения процедуры байтстаффинга для сообщения Астерикс
 * @param cat_lib - указатель на структуру категории
 * @param use_byte_staff - код включения функции байтстаффинга
 *                           0 - не использовать байтстаффинг
 *                           1 - использовать байтстаффинг
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setByteStaffingMode(TCatAsterix* cat_lib, u_int8_t use_byte_staff)
{
	int res = -1;


	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (use_byte_staff)
		{
		case 0:
		case 1:
			res                        = 0;
			cat_lib->create_byte_staff = use_byte_staff;
			break;
		}
	}
	return res;
}

/**
 * @brief Установка включения флагов открывания и закрывания для сообщения Астерикс
 * @param cat_lib - указатель на структуру категории
 * @param use_shell_flags - код включения флагов в сообщении:
 *                           0 - не использовать флаги
 *                           1 - использовать флаги
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setShellFlagsMode(TCatAsterix* cat_lib, u_int8_t use_shell_flags)
{
	int res = -1;


	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (use_shell_flags)
		{
		case 0:
		case 1:
			res                        = 0;
			cat_lib->create_flag_shell = use_shell_flags;
			break;
		}
	}
	return res;
}

/**
 * @brief Установка значения контрольного флага для сообщения Астерикс
 * @param cat_lib - указатель на структуру категории
 * @param control_flag - код контрольного флага в сообщении
 *                       ( по умолчанию значение контрольного флага = 0х10 )
 * @return 0 - операция выполнена, -1 отказ исполнения
 */
int setControlFlag(TCatAsterix* cat_lib, u_int8_t control_flag)
{
	int res = -1;


	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		res                       = 0;
		cat_lib->create_check_sum = control_flag;
	}
	return res;
}

/**
 * @brief Кодирование длины, ширины и положения объекта
 * @param lenght - длина объекта
 * @param width  - ширина объекта
 * @param orientation - положение носовой части относительно направления на север
 * @return -
 */
TReturnOneInt packTargetSizeAndOrientation(u_int8_t length, u_int8_t width, double orientation)
{
	TReturnOneInt ret;
	u_int8_t      b   = 0;
	int           ccc = 0;

	ret.update = 0;
	ret.val    = 0;

	if ((length != DEFAULT_U_INT8_CODE) ||
	    (width != DEFAULT_U_INT8_CODE) ||
	    (orientation != DEFAULT_RADIAN_GEO_POS))
	{
		if ((length > 0) && (length != DEFAULT_U_INT8_CODE))
			ret.val = length << 17;

		if (orientation != DEFAULT_RADIAN_GEO_POS)
		{
			ret.val |= (1 << 16);
			ccc      = (int) ((orientation * DEG) / 2.8125);
			ret.val |= ((ccc & 0xff) << 9);
			b        = 1;
		}

		if ((width > 0) && (width != DEFAULT_U_INT8_CODE))
		{
			if (b == 1)
				ret.val |= (1 << 16);
			ret.val |= (1 << 8);
			ret.val |= (width << 1);
		}
	}
	if (ret.val)
		ret.update = 1;
	return ret;
}

/**
 * @brief Проверка переполнения
 * @param current_pos - текущее положение отсчета данных
 * @param kd_size     - размер данных в байтах
 * @param ed_size     - размер элемента данных в байтах
 * @return 0 - нет переполнения, -1 - выход за предел с данными
 */
int checkOverloadFunc(u_int64_t current_pos, u_int16_t ed_size, u_int64_t kd_size)
{
	int res = -1;

	if ((current_pos + ed_size) <= kd_size)
	{
		return 0;
	}
	else
	{
		fprintf(stderr, "proc[pid-%d] Overload size data ERROR\n", getpid());
	}
	return res;
}

int view390PlanData(unsigned char* code, u_int64_t* cnt, u_int64_t code_size)
{
	unsigned char p_spec[FSPEC_SIZE_5_BYTE] =
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
	if (ccc > 0)
	{
		memcpy(p_spec, code + k, ccc);
		k += ccc;
	}
	// Plan sic_sac
	if (p_spec[0] & 0x80)
	{
		if (checkOverloadFunc(k, 2, code_size) < 0)
		{
			return -1;
		}
		k += 2;
	}
	// Callsign
	if (p_spec[0] & 0x40)
	{
		if (checkOverloadFunc(k, 7, code_size) < 0)
		{
			return -1;
		}
		k += 7;
	}
	// Plan ID
	if (p_spec[0] & 0x20)
	{
		if (checkOverloadFunc(k, 4, code_size) < 0)
		{
			return -1;
		}
		k += 4;
	}
	// Flight category
	if (p_spec[0] & 0x10)
	{
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			return -1;
		}
		k += 1;
	}
	// Type aircraft
	if (p_spec[0] & 0x08)
	{
		if (checkOverloadFunc(k, 4, code_size) < 0)
		{
			return -1;
		}
		k += 4;
	}
	// Turbulence
	if (p_spec[0] & 0x04)
	{
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			return -1;
		}
		k += 1;
	}
	// Depart airport
	if (p_spec[0] & 0x02)
	{
		if (checkOverloadFunc(k, 4, code_size) < 0)
		{
			return -1;
		}
		k += 4;
	}
	// Dest airport
	if (p_spec[1] & 0x80)
	{
		if (checkOverloadFunc(k, 4, code_size) < 0)
		{
			return -1;
		}
		k += 4;
	}
	// VPP
	if (p_spec[1] & 0x40)
	{
		if (checkOverloadFunc(k, 3, code_size) < 0)
		{
			return -1;
		}
		k += 3;
	}
	// Current FL
	if (p_spec[1] & 0x20)
	{
		if (checkOverloadFunc(k, 2, code_size) < 0)
		{
			return -1;
		}
		k += 2;
	}
	// Current control position
	if (p_spec[1] & 0x10)
	{
		if (checkOverloadFunc(k, 2, code_size) < 0)
		{
			return -1;
		}
		k += 2;                                    // center
	}
	// TOD
	if (p_spec[1] & 0x08)
	{
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			return -1;
		}
		k += 1;                                    // rep factor
		if (code[ k - 1] > 0)
		{
			if (checkOverloadFunc(k, 4 * code[ k - 1], code_size) < 0)
			{
				return -1;
			}
			k += (4 * code[ k - 1]);
		}
	}
	// Aircraft stand
	if (p_spec[1] & 0x04)
	{
		if (checkOverloadFunc(k, 6, code_size) < 0)
		{
			return -1;
		}
		k += 6;
	}
	// Aircraft stand status
	if (p_spec[1] & 0x02)
	{
		if (checkOverloadFunc(k, 1, code_size) < 0)
		{
			return -1;
		}
		k += 1;
	}
	// SID proc
	if (p_spec[2] & 0x80)
	{
		if (checkOverloadFunc(k, 7, code_size) < 0)
		{
			return -1;
		}
		k += 7;
	}
	// STAR proc
	if (p_spec[2] & 0x40)
	{
		if (checkOverloadFunc(k, 7, code_size) < 0)
		{
			return -1;
		}
		k += 7;
	}
	// Pre Emerg Mode 3A
	if (p_spec[2] & 0x20)
	{
		if (checkOverloadFunc(k, 2, code_size) < 0)
		{
			return -1;
		}
		k += 2;
	}
	// Pre Emerg Callsign
	if (p_spec[2] & 0x10)
	{
		if (checkOverloadFunc(k, 7, code_size) < 0)
		{
			return -1;
		}
		k += 7;
	}

	*cnt = k;

	return 0;
}

ElementHeader* getCat010Element(TCatAsterix* cat, u_int16_t el_index)
{
	ElementHeader* el_head = NULL;

	switch (el_index)
	{
	case CAT010_EL000:
		el_head = &cat->category.c010.item010_000.head;
		break;
	case CAT010_EL010:
		el_head = &cat->category.c010.item010_010.head;
		break;
	case CAT010_EL020:
		el_head = &cat->category.c010.item010_020.head;
		break;
	case CAT010_EL040:
		el_head = &cat->category.c010.item010_040.head;
		break;
	case CAT010_EL041:
		el_head = &cat->category.c010.item010_041.head;
		break;
	case CAT010_EL042:
		el_head = &cat->category.c010.item010_042.head;
		break;
	case CAT010_EL060:
		el_head = &cat->category.c010.item010_060.head;
		break;
	case CAT010_EL090:
		el_head = &cat->category.c010.item010_090.head;
		break;
	case CAT010_EL091:
		el_head = &cat->category.c010.item010_091.head;
		break;
	case CAT010_EL131:
		el_head = &cat->category.c010.item010_131.head;
		break;
	case CAT010_EL140:
		el_head = &cat->category.c010.item010_140.head;
		break;
	case CAT010_EL161:
		el_head = &cat->category.c010.item010_161.head;
		break;
	case CAT010_EL170:
		el_head = &cat->category.c010.item010_170.head;
		break;
	case CAT010_EL200:
		el_head = &cat->category.c010.item010_200.head;
		break;
	case CAT010_EL202:
		el_head = &cat->category.c010.item010_202.head;
		break;
	case CAT010_EL210:
		el_head = &cat->category.c010.item010_210.head;
		break;
	case CAT010_EL220:
		el_head = &cat->category.c010.item010_220.head;
		break;
	case CAT010_EL245:
		el_head = &cat->category.c010.item010_245.head;
		break;
	case CAT010_EL250:
		el_head = &cat->category.c010.item010_250.head;
		break;
	case CAT010_EL270:
		el_head = &cat->category.c010.item010_270.head;
		break;
	case CAT010_EL280:
		el_head = &cat->category.c010.item010_280.head;
		break;
	case CAT010_EL300:
		el_head = &cat->category.c010.item010_300.head;
		break;
	case CAT010_EL310:
		el_head = &cat->category.c010.item010_310.head;
		break;
	case CAT010_EL500:
		el_head = &cat->category.c010.item010_500.head;
		break;
	case CAT010_EL550:
		el_head = &cat->category.c010.item010_550.head;
		break;
	case CAT010_ELSP:
		if (cat->category.c010.item010_sp)
			el_head = &cat->category.c010.item010_sp->head;
		break;
	case CAT010_ELRE:
		if (cat->category.c010.item010_re)
			el_head = &cat->category.c010.item010_re->head;
		break;
	}

	return el_head;

}

ElementHeader* getCat011Element(TCatAsterix* cat, u_int16_t el_index)
{
	ElementHeader* el_head = NULL;

	switch (el_index)
	{
	case CAT011_EL000:
		el_head = &cat->category.c011.item011_000.head;
		break;
	case CAT011_EL010:
		el_head = &cat->category.c011.item011_010.head;
		break;
	case CAT011_EL015:
		el_head = &cat->category.c011.item011_015.head;
		break;
	case CAT011_EL041:
		el_head = &cat->category.c011.item011_041.head;
		break;
	case CAT011_EL042:
		el_head = &cat->category.c011.item011_042.head;
		break;
	case CAT011_EL060:
		el_head = &cat->category.c011.item011_060.head;
		break;
	case CAT011_EL090:
		el_head = &cat->category.c011.item011_090.head;
		break;
	case CAT011_EL092:
		el_head = &cat->category.c011.item011_092.head;
		break;
	case CAT011_EL093:
		el_head = &cat->category.c011.item011_093.head;
		break;
	case CAT011_EL140:
		el_head = &cat->category.c011.item011_140.head;
		break;
	case CAT011_EL161:
		el_head = &cat->category.c011.item011_161.head;
		break;
	case CAT011_EL170:
		el_head = &cat->category.c011.item011_170.head;
		break;
	case CAT011_EL202:
		el_head = &cat->category.c011.item011_202.head;
		break;
	case CAT011_EL210:
		el_head = &cat->category.c011.item011_210.head;
		break;
	case CAT011_EL215:
		el_head = &cat->category.c011.item011_215.head;
		break;
	case CAT011_EL245:
		el_head = &cat->category.c011.item011_245.head;
		break;
	case CAT011_EL270:
		el_head = &cat->category.c011.item011_270.head;
		break;
	case CAT011_EL290:
		el_head = &cat->category.c011.item011_290.head;
		break;
	case CAT011_EL300:
		el_head = &cat->category.c011.item011_300.head;
		break;
	case CAT011_EL310:
		el_head = &cat->category.c011.item011_310.head;
		break;
	case CAT011_EL380:
		if (cat->category.c011.item011_380)
			el_head = &cat->category.c011.item011_380->head;
		break;
	case CAT011_EL390:
		if (cat->category.c011.item011_390)
			el_head = &cat->category.c011.item011_390->head;
		break;
	case CAT011_EL430:
		el_head = &cat->category.c011.item011_430.head;
		break;
	case CAT011_EL500:
		el_head = &cat->category.c011.item011_500.head;
		break;
	case CAT011_EL600:
		el_head = &cat->category.c011.item011_600.head;
		break;
	case CAT011_EL605:
		el_head = &cat->category.c011.item011_605.head;
		break;
	case CAT011_EL610:
		el_head = &cat->category.c011.item011_610.head;
		break;
	case CAT011_ELSP:
		if (cat->category.c011.item011_sp)
			el_head = &cat->category.c011.item011_sp->head;
		break;
	case CAT011_ELRE:
		if (cat->category.c011.item011_re)
			el_head = &cat->category.c011.item011_re->head;
		break;

	}

	return el_head;
}

ElementHeader* getCat062Element(TCatAsterix* cat, u_int16_t el_index)
{
	ElementHeader* el_head = NULL;

	switch (el_index)
	{
	case CAT062_EL010:
		el_head = &cat->category.c062.item062_010.head;
		break;
	case CAT062_EL015:
		el_head = &cat->category.c062.item062_015.head;
		break;
	case CAT062_EL040:
		el_head = &cat->category.c062.item062_040.head;
		break;
	case CAT062_EL060:
		el_head = &cat->category.c062.item062_060.head;
		break;
	case CAT062_EL070:
		el_head = &cat->category.c062.item062_070.head;
		break;
	case CAT062_EL080:
		el_head = &cat->category.c062.item062_080.head;
		break;
	case CAT062_EL100:
		el_head = &cat->category.c062.item062_100.head;
		break;
	case CAT062_EL105:
		el_head = &cat->category.c062.item062_105.head;
		break;
	case CAT062_EL110:
		el_head = &cat->category.c062.item062_110.head;
		break;
	case CAT062_EL120:
		el_head = &cat->category.c062.item062_120.head;
		break;
	case CAT062_EL130:
		el_head = &cat->category.c062.item062_130.head;
		break;
	case CAT062_EL135:
		el_head = &cat->category.c062.item062_135.head;
		break;
	case CAT062_EL136:
		el_head = &cat->category.c062.item062_136.head;
		break;
	case CAT062_EL185:
		el_head = &cat->category.c062.item062_185.head;
		break;
	case CAT062_EL200:
		el_head = &cat->category.c062.item062_200.head;
		break;
	case CAT062_EL210:
		el_head = &cat->category.c062.item062_210.head;
		break;
	case CAT062_EL220:
		el_head = &cat->category.c062.item062_220.head;
		break;
	case CAT062_EL245:
		el_head = &cat->category.c062.item062_245.head;
		break;
	case CAT062_EL270:
		el_head = &cat->category.c062.item062_270.head;
		break;
	case CAT062_EL290:
		el_head = &cat->category.c062.item062_290.head;
		break;
	case CAT062_EL295:
		el_head = &cat->category.c062.item062_295.head;
		break;
	case CAT062_EL300:
		el_head = &cat->category.c062.item062_300.head;
		break;
	case CAT062_EL340:
		if (cat->category.c062.item062_340)
			el_head = &cat->category.c062.item062_340->head;
		break;
	case CAT062_EL380:
		if (cat->category.c062.item062_380)
			el_head = &cat->category.c062.item062_380->head;
		break;
	case CAT062_EL390:
		if (cat->category.c062.item062_390)
			el_head = &cat->category.c062.item062_390->head;
		break;
	case CAT062_EL500:
		el_head = &cat->category.c062.item062_500.head;
		break;
	case CAT062_EL510:
		el_head = &cat->category.c062.item062_510.head;
		break;
	case CAT062_ELSP:
		if (cat->category.c062.item062_sp)
			el_head = &cat->category.c062.item062_sp->head;
		break;
	}

	return el_head;
}

ElementHeader* getElementHeader(TCatAsterix* cat, u_int16_t ed_index)
{
	ElementHeader* el_head = NULL;

	if (cat == NULL)
		return el_head;

	if (getCatergoryNum(cat) == CODT_CAT_010)
	{
		el_head = getCat010Element(cat, ed_index);
		return el_head;
	}


	if (getCatergoryNum(cat) == CODT_CAT_011)
	{
		el_head = getCat011Element(cat, ed_index);
		return el_head;
	}

	if (getCatergoryNum(cat) == CODT_CAT_062)
	{
		el_head = getCat062Element(cat, ed_index);
		return el_head;
	}

	return el_head;
}

/**
 * @brief Запрос названия элемента из контейнера категории
 * @param cat - указатель на структуру категории Астерикс
 * @param cat_num - номер категориии
 * @param ed_index - индекс элемента категории в соответствии с профилем пользователя
 * @return - указатель на структуру с названием элемента
 */
TCommonObmenData* getCatElementName(TCatAsterix* cat,  u_int16_t ed_index)
{
	TCommonObmenData* ret     = NULL;
	ElementHeader*    el_head = NULL;
	size_t            el_size = 0;
	if (cat == NULL)
		return ret;


	el_head = getElementHeader(cat, ed_index);
	if (el_head)
	{
		el_size = strlen(el_head->name);
		if (el_size > 0)
		{
			ret = initObmenData(el_size);
			if (ret)
				setObmenData(ret, el_head->name, el_size);
		}
	}

	return ret;
}

/**
 * @brief Запрос кодированного содержимого элемента из контейнера категории
 * @param cat - указатель на структуру категории Астерикс
 * @param cat_num - номер категориии
 * @param ed_index - индекс элемента категории в соответствии с профилем пользователя
 * @return - указатель на структуру с кодированными данными элемента
 */
TCommonObmenData* getCatElementRawData(TCatAsterix* cat, u_int16_t ed_index)
{
	TCommonObmenData* ret     = NULL;
	ElementHeader*    el_head = NULL;
	size_t            el_size = 0;

	if (cat == NULL)
		return ret;


	el_head = getElementHeader(cat, ed_index);
	if (el_head)
	{
		el_size = el_head->code_size;
		if ((el_size > 0) && (el_head->code))
		{
			ret = initObmenData(el_size);
			if (ret)
				setObmenData(ret, el_head->code, el_size);
		}
	}

	return ret;
}

/**
 * @brief Упаковка данных о маневрировании объекта в структуру категории Астерикс
 * @param cat_lib - указатель на контейнер Астерикс
 * @param horizontal - вид маневра в горизонтальной плоскости
 * @param vertical   - вид маневра в вертикальной плоскости
 * @param speed      - вид маневра по скорости
 * @param alt_disc   - признак наличия расхождения данных по высоте измеренной и полученной с борта ВО
 *                     0 - нет расхождения
 *                     1 - есть расхождение
 * @return
 */
int setTrackModeMovement(TCatAsterix* cat_lib, TargetModeMovement horizontal, TargetModeMovement vertical, TargetModeMovement speed, u_int8_t alt_disc)
{
	int res = -1;

	if (cat_lib == NULL)
		return res;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			res                                                     = 0;
			cat_lib->category.c062.item062_200.transversal          = horizontal;
			cat_lib->category.c062.item062_200.long_acceleration    = speed;
			cat_lib->category.c062.item062_200.vertical_rate        = vertical;
			cat_lib->category.c062.item062_200.altitude_discrepancy = alt_disc;

			if (cat_lib->category.c062.item062_fspec.code)
				cat_lib->category.c062.item062_fspec.code[2] |= 0x80;
		}
	}

	return res;
}

/**
 * @brief Выборка данных о маневрировании объекта
 *          в горизонтальной плоскости, из контейнера категории
 * @return Вид маневрирования в горизонтальной плоскости
 */
// TargetModeMovement getTrackModeMovementHorizontal(TCatAsterix* cat_lib)
int getTrackModeMovementHorizontal(TCatAsterix* cat_lib)
{
	// TargetModeMovement ret = TMM_MOVE_STABLE;
	int ret = TMM_MOVE_STABLE;

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			ret = cat_lib->category.c062.item062_200.transversal;
			break;
		}
	}
	return ret;
}

/**
 * @brief Выборка данных о маневрировании объекта
 *          в вертикальной плоскости, из контейнера категории
 * @return Вид маневрирования в вертикальной плоскости
 */
// TargetModeMovement getTrackModeMovementVertical(TCatAsterix* cat_lib)
int getTrackModeMovementVertical(TCatAsterix* cat_lib)
{
	// TargetModeMovement ret = TMM_MOVE_STABLE;
	int ret = TMM_MOVE_STABLE;

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			ret = cat_lib->category.c062.item062_200.vertical_rate;
			break;
		}
	}
	return ret;
}

/**
 * @brief Выборка данных о маневрировании объекта
 *          по скорости, из контейнера категории
 * @return Вид маневрирования по скорости
 */
// TargetModeMovement getTrackModeMovementBySpeed(TCatAsterix* cat_lib)
int getTrackModeMovementBySpeed(TCatAsterix* cat_lib)
{
	// TargetModeMovement ret = TMM_MOVE_STABLE;
	int ret = TMM_MOVE_STABLE;

	if (cat_lib == NULL)
		return ret;

	if (cat_lib)
	{
		switch (cat_lib->cat_num)
		{
		case CODT_CAT_062:
			ret = cat_lib->category.c062.item062_200.long_acceleration;
			break;
		}
	}
	return ret;
}

/**
 * @brief Кодирование  времени устаревания параметра трассы
 * @param age_time - параметр времени в секундах (диапазон от 0.25 до 63.75 сек)
 * @return - кодированое значение времени диапазоне от 0 - 255
 */
u_int8_t packAgeTime(double age_time)
{
	int      ccc;
	u_int8_t ret = 0;

	if ((age_time >= CMR_AGE_TIME) &&
	    (age_time < DEFAULT_TIME_AGES))
	{
		ccc = (int) (age_time / CMR_AGE_TIME);
		if (ccc > DEFAULT_U_INT8_CODE)
			ccc = DEFAULT_U_INT8_CODE;
		ret = ccc;
	}
	return ret;
}

/**
 * @brief Укладка байтов профиля категории в массив с кодированными данными
 * @param dest - указатель на массив данных
 * @param src - указатель на массив с данными профиля пользователя
 * @param cat_max_fspec_size - максимальный размер профиля пользователя категории
 * @return - количество уложенных байт профиля пользователя
 */
u_int8_t packFspecToCodogramm(void* dest, void* src, u_int8_t cat_max_fspec_size)
{
	int       b         = cat_max_fspec_size - 1, st_fl = 0;
	u_int8_t  fspec_pos = cat_max_fspec_size;
	u_int8_t* s1, * s2;
	if ((dest == NULL) || (src == NULL))
		return 0;

	s1 = (u_int8_t*) src;
	s2 = (u_int8_t*) dest;
	while (b != -1)
	{
		if ((s1[ b ] == 0) && (st_fl == 0))
		{
			b--;
			continue;
		}
		else
		{
			if (st_fl == 0)
			{
				st_fl     = b;
				s2[ b ]   = s1[ b ];
				fspec_pos = b + 1;
				b--;
			}
			else
			{
				s2[ b ] = s1[ b ] | 0x01;
				b--;
			}
		}
	}

	return fspec_pos;
}

double decodeAgeTime(u_int16_t code_time)
{
	double ret = code_time * CMR_AGE_TIME;
	return ret;
}
