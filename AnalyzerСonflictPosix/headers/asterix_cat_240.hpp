#ifndef ASTERIX_CAT_240_HPP
#define ASTERIX_CAT_240_HPP

#include "asterix.hpp"
#include "asterix_internal.hpp"

namespace Cat240
{
using Asterix::DataSourceId;

struct VideoHeader
{
	double   start_az;
	double   end_az;
	uint32_t start_rg;
	double   cell_dur;
};

enum BitDepth
{
	D_NORMAL    = 1,
	D_MONO      = 2,
	D_HIGH      = 3,
	D_COMPOSITE = 4,
};

enum Compression
{
	C_NO_COMRESSION,
	C_DEFLATE,
};

struct VideoResolution
{
	BitDepth depth;
	union
	{
		uint8_t spare;
		uint8_t compression;
	};
};

enum MessageType
{
	VideoSummaryMessage = 1,
	VideoMessage        = 2,
};

struct VideoData
{
	uint32_t* data;

	void resize(size_t size);
	VideoData();
	~VideoData();
};

/// Категория 240
struct Cat240 :
	public AsterixRecord
{
	struct Reader;
	struct Writer;

	/// Структура-описание полей asterix
	struct Uap
	{
		Asterix::UapInfo info;
		struct Serializer;
		void (Serializer::* serialize)(Cat240&);              ///< Функция-обработчик
	};

	static const int RE_FRN   = 13;
	static const int RE_END   = 0;
	static const int LAST_FRN = 15;
	static const int UAP_SIZE = LAST_FRN + RE_END + 1;
	static const Uap UAP[UAP_SIZE];

	ASTERIX_DECLARE(DataSourceId, Cat240, 1, _dsi);               ///< Data Source Identifier
	ASTERIX_DECLARE(uint8_t,      Cat240, 2, _message_type);      ///< Message Type
	ASTERIX_DECLARE(uint32_t,     Cat240, 3, _msg_index);         ///< Video Record Header

	ASTERIX_DECLARE_ARRAY(char, Cat240, 4, _summary);             ///< Video Summary

	ASTERIX_DECLARE(VideoHeader,     Cat240,  5, _header_ns);     ///< Video Header Nano
	ASTERIX_DECLARE(VideoHeader,     Cat240,  6, _header_fs);     ///< Video Header Femto
	ASTERIX_DECLARE(VideoResolution, Cat240,  7, _resolution);    ///< Video Cells Resolution
	ASTERIX_DECLARE(uint16_t,        Cat240,  8, _block_counter); ///< Video Blocks Counter

	ASTERIX_DECLARE(VideoData,       Cat240,  9, _video);         ///< Video Block (сетевой порядок байтов)

	ASTERIX_DECLARE(double,          Cat240, 10, _time);          ///< Time of Day

	ASTERIX_DECLARE(uint8_t,         Cat240, 13, _re);            ///< Reserved Expansion Field
	ASTERIX_DECLARE_ARRAY(uint8_t, Cat240, 14, _sp);              ///< Special Purpose Field

private:

	size_t get_items_count() const;

public:

	/// Сжатие видеосигнала в соответствии с выбранным алгоритмом сжатия
	void compress_video();
	/// Распаковка видеосигнала в соответствии с выбранным алгоритмом сжатия
	void uncompress_video();

	/// Упаковка видеосигнала из отдельных отсчетов
	/*! \param[in] data Отсчеты для упаковки
	 *  \param[in] size Количество отсчетов
	 */
	void set_video_unpacked(uint8_t* data, size_t size);
	void set_video_unpacked(uint32_t* data, size_t size);         ///< \overload

	/// Распаковка видеосигнала в отдельные отсчеты
	/*! \param[in,out] data Указатель на размещенный malloc указатель
	 *  \param[out] size    Указатель для возврата количества отсчетов
	 */
	void get_video_unpacked(uint8_t** data, size_t* size) const;
	void get_video_unpacked(uint32_t** data, size_t* size) const; ///< \overload

	Cat240() : AsterixRecord(240) {};
	static AsterixRecord* decode(const char* data, size_t size, AsterixError& err, size_t& read_size);
	static char*          code(const AsterixRecord* record, size_t& write_size);
};

struct Cat240::Uap::Serializer
{
	virtual void dataSourceId(Cat240& r) = 0;
	virtual void messageType(Cat240& r)  = 0;
	virtual void recordHeader(Cat240& r) = 0;
	virtual void summary(Cat240& r)      = 0;
	virtual void headerNs(Cat240& r)     = 0;
	virtual void headerFs(Cat240& r)     = 0;
	virtual void resolution(Cat240& r)   = 0;
	virtual void blockCounter(Cat240& r) = 0;
	virtual void video(Cat240& r)        = 0;
	virtual void timeOfDay(Cat240& r)    = 0;
	virtual void reserved(Cat240& r)     = 0;
	virtual void reserved_end(Cat240& r) = 0;
	virtual void special(Cat240& r)      = 0;

	template <size_t N> void skip(Cat240& r);

	virtual ~Serializer() {};
};

}                                                          // namespace Cat240

#endif                                                     /* ASTERIX_CAT_240_HPP */
