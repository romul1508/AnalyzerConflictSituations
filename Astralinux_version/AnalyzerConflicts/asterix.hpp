#ifndef ASTERIX_HPP
#define ASTERIX_HPP

#include <string>
#include <vector>
#include <list>
#include <stdint.h>

#include "common_lib_asterix.h"

enum DecodeMode
{
	A_DECODE_SINGLE,                                   ///< Готовый пакет
	A_DECODE_DELIMITED,                                ///< Пакет с разделителями и вставками
};

enum CodeMode
{
	A_CODE_SINGLE,                                     ///< Одиночная запись
	A_CODE_DELIMITED,                                  ///< Пакет с разделителями и вставками
};

enum AsterixError
{
	A_NO_ERROR,                                        ///< Нет ошибки
	A_EMPTY_BUFFER,                                    ///< Буфер пуст
	A_BUFFER_NOT_FILLED,                               ///< Не достаточно данных в буфере
	A_CATEGORY_NOT_SUPPORTED,                          ///< Не поддерживаемая категория
	A_DECODE_WRONG_SIZE,                               ///< Размер кодограммы не совпадает
};

std::string getStatusString(AsterixError);

struct AsterixRecord
{
	uint8_t category;                                  ///< Номер категории

	template <typename T, typename C, T (C::* field), int frn, int LAST_FRN, int RE_FRN, int RE_END>
	static const T* get(const C* record);

	template <typename T, typename C, T (C::* field), int frn, int LAST_FRN, int RE_FRN, int RE_END>
	static void set(C* record, const T* data);

	template <typename T, typename C, std::vector<T>(C::* field), int frn>
	static const T* get(const C* record, size_t* size);

	template <typename T, typename C, std::vector<T>(C::* field), int frn>
	static void set(C* record, const T* data, size_t size);

	template <typename T, typename C, std::vector<T>(C::* field), int frn>
	static void set_add(C* record, const T data);

	uint64_t _fspec;                                   ///< Маска доступных полей

protected:

	AsterixRecord(uint8_t cat) : category(cat), _fspec(0) {};

public:

	virtual ~AsterixRecord() {};
};

enum ByteStuffState
{
	BS_GARBAGE,                                        // Возникла ошибка при распаковке
	BS_INDETERMINATE,
	BS_EXPECT_02,
	BS_EXPECT_10_OR_03,
	BS_NORMAL
};

class AsterixDecoder
{
private:

	typedef AsterixRecord* (* DecoderFun) (const char* data, size_t size, AsterixError& err, size_t& read_size);
	DecoderFun decoders[256];                          ///< Декодирующие функции

	uint8_t _current_record_cat;                       ///< Номер читаемой в данный момент категории
	size_t  _data_block_left;                          ///< Количество байт до конца блока данных

	AsterixError _error_status;                        ///< Тип последней ошибки
	DecodeMode   _mode;                                ///< Режим работы

	ByteStuffState _bs_state;

	std::vector<char> _input_buffer;                   ///< Буфер не распакованных кодограмм
	std::vector<char>::iterator iter;
	std::vector<char> _decode_buffer;                  ///< Буфер распакованных кодограмм

	AsterixRecord* readRecord();
	AsterixRecord* decodeAsterix();
	void           init(bool old_cat21);

public:

	AsterixDecoder(DecodeMode, bool old_cat21);
	void           feed(const uint8_t* data, size_t size);
	AsterixRecord* decode();
	AsterixError   getStatus();
	std::string    getStatusString();

	void resetBuffers();
};

class AsterixCoder
{
private:

	typedef char* (* CoderFun) (const AsterixRecord* record, size_t& write_size);
	CoderFun coders[256];                              ///< Кодирующие функции

	typedef std::pair<char*, size_t> Item;
	typedef std::list<Item>          Buffer;
	uint8_t _current_record_cat;                       ///< Номер записываемой в данный момент категории

	AsterixError _error_status;                        ///< Тип последней ошибки
	CodeMode     _mode;                                ///< Режим работы
	size_t       _mtu;                                 ///< Максимальный размер кодограммы

	Buffer _output_buffer;                             ///< Буфер запакованных кодограмм
	Buffer _code_buffer;                               ///< Буфер не запакованных кодограмм
	void   produceOutput();
	void   init();

public:

	AsterixCoder(CodeMode, size_t mtu = 0);
	const uint8_t* take(size_t* size);
	void           code(AsterixRecord*);
	AsterixError   getStatus();
	std::string    getStatusString();
};

namespace Asterix
{
// Макрос для поддержки перевода полей
#define UAP_NAME(x) x

struct UapInfo
{
	uint8_t     frn;                                   ///< Field Reference Number
	uint64_t    mask;                                  ///< Маска FRN
	uint16_t    data_item;                             ///< Asterix Data Item
	const char* information;                           ///< Описание поля
	uint8_t     length;                                ///< Размер поля в байтах, ноль для полей переменного размера
};

struct DataSourceId
{
	uint8_t sac;
	uint8_t sic;

	uint16_t to_raw() const
	{
		return sac << 8 | sic;
	}

	void from_raw(uint16_t raw)
	{
		sac = raw >> 8;
		sic = raw & 0xFF;
	}

};

struct Position
{
	double wgs84_lon;
	double wgs84_lat;
};

struct PositionCartesian
{
	double x;
	double y;
};

struct Velocity
{
	double vx;
	double vy;
};

struct PolarVelocity
{
	double heading;
	double speed;
};

struct Acceleration
{
	double ax;
	double ay;
};

struct Callsign
{
	static const int           CALLSIGN_SIZE = 9;
	TargetIdentificationSource sti;
	char                       callsign[CALLSIGN_SIZE];
};


struct ModeS
{
	uint64_t data : 56;
	uint8_t  bds;
};

enum FleetType
{
	FT_UNKNOWN                   = 0,
	FT_ATC_EQUIPMENT_MAINTENANCE = 1,
	FT_AIRPORT_MAINTENANCE       = 2,
	FT_FIRE                      = 3,
	FT_BIRD_SCARER               = 4,
	FT_SNOW_PLOUGH               = 5,
	FT_RUNWAY_SWEEPER            = 6,
	FT_EMERGENCY_VEHICLE         = 7,
	FT_POLICE                    = 8,
	FT_BUS                       = 9,
	FT_TUG                       = 10,
	FT_GRASS_CUTTER              = 11,
	FT_FUEL                      = 12,
	FT_BAGGAGE                   = 13,
	FT_CATERING                  = 14,
	FT_AIRCRAFT_MAINTENANCE      = 15,
	FT_FLYCO                     = 16,
};

enum ProgrammedMessageType
{
	PM_NO_OPERATION     = 0,
	PM_TOW              = 1,
	PM_FOLLOW_ME        = 2,
	PM_RUNWAY_CHECK     = 3,
	PM_EMERGENCY        = 4,
	PM_WORK_IN_PROGRESS = 5
};

const char* getFleetTypeString(uint8_t ft);

const char* getProgrammedMessageString(uint8_t pm);

struct ProggramedMessage
{
	bool    trouble : 1;
	uint8_t type    : 7;
};

enum ReportSource
{
	RS_TARGET_TRANSPONDER = 0,
	RS_FIELD_MONITOR      = 1
};

struct Size
{
	double length;
	double width;
	double orientation;
};

struct CodeFlags
{
	bool not_validated : 1;
	bool garbled       : 1;
	bool not_extracted : 1;
};

struct Mode3A
{
	CodeFlags flags;
	uint16_t  code;
};

struct GroundVector
{
	bool   range_exceeded;
	double heading;
	double speed;
};

struct AcasRA
{
	uint8_t  typ  : 5;
	uint8_t  styp : 3;
	uint16_t ara  : 14;
	uint8_t  rac  : 4;
	bool     rat  : 1;
	bool     mte  : 1;
	uint8_t  tti  : 2;
	uint32_t tid  : 26;
};

enum ComCapability
{
	COM_NO_CAP         = 0,
	COM_COMM_AB        = 1,
	COM_COMMA_B_UPLINK = 2,
	COM_COMM_AB_UPLINK = 3,
	COM_LEVEL5         = 4,
	COM_NA1            = 5,
	COM_NA2            = 6,
	COM_NA3            = 7,
};


char*
make_byte_stuff(const char* at, const char* end, size_t* size);

std::pair<ByteStuffState, const char*>
unmake_byte_stuff(const char* at, const char* end, ByteStuffState _bs_state, std::vector<char>* out, bool single);

}                                                          // namespace Asterix

#endif                                                     /* ASTERIX_HPP */
