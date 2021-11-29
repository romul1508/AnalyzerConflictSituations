#include "asterix_cat_240.hpp"
#include "asterix_template.hpp"
#include <stdlib.h>
// #include <zlib.h>	// for Linux
#include <string.h>

const Ratio<360, 1 << 16>        AZ_LSB;
const Ratio<1, 1000000000>       CELL_DUR_NS;
const Ratio<1, 1000000000000000> CELL_DUR_FS;
const Ratio<1, 128>              TIME_LSB;

namespace Cat240
{
void VideoData::resize(size_t size)
{
	data = (uint32_t*) realloc(data, size * sizeof(*data));
}

VideoData::VideoData()
	: data(NULL) {}
VideoData::~VideoData()
{
	free(data);
}

template <> void Cat240::Uap::Serializer::skip<0>(Cat240&) {};

const Cat240::Uap Cat240::UAP[UAP_SIZE] =
{
	ASTERIX_UAP_FIELD(Cat240,        0,          0, "Field Extension Indicator", 0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat240,        1,         10, "Data Source Identifier",    2, dataSourceId),
	ASTERIX_UAP_FIELD(Cat240,        2,          0, "Message Type",              1, messageType),
	ASTERIX_UAP_FIELD(Cat240,        3,         20, "Video Record Header",       3, recordHeader),
	ASTERIX_UAP_FIELD(Cat240,        4,         30, "Video Summary",             1, summary),
	ASTERIX_UAP_FIELD(Cat240,        5,         40, "Video Header Nano",         1, headerNs),
	ASTERIX_UAP_FIELD(Cat240,        6,         41, "Video Header Femto",        0, headerFs),
	ASTERIX_UAP_FIELD(Cat240,        7,         48, "Video Cells Resolution",    0, resolution),
	ASTERIX_UAP_FIELD(Cat240,        8,         49, "Video Blocks Counter",      8, blockCounter),
	ASTERIX_UAP_FIELD(Cat240,        9,         50, "Video Block",               2, video),
	ASTERIX_UAP_FIELD(Cat240,       10,        140, "Time of Day",               1, timeOfDay),
	ASTERIX_UAP_FIELD(Cat240,       11,          0, "Spare",                     0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat240,       12,          0, "Spare",                     0, skip<0> ),
	ASTERIX_UAP_FIELD(Cat240,       13,          0, "Reserved Expansion Field",  0, reserved),
	ASTERIX_UAP_FIELD(Cat240,       14,          0, "Special Purpose Field",     0, special),
	ASTERIX_UAP_FIELD(Cat240, LAST_FRN + RE_END, 0, "Reserved Expansion Field",  0, reserved_end),
};

void Cat240::set_video_unpacked(uint8_t* data, size_t size)
{
#define get_item(block, i, block_size)            ((data[block * block_size + i] & ((1 << 32 / block_size) - 1)) << (32 / block_size) * (block_size - 1 - i))
#define checked_item(block, count, i, block_size) ((size_t(block * block_size + i) < count) ? get_item(block, i, block_size) : 0)
	uint16_t full_block_count = 0;
	switch (_resolution.depth)
	{
	case D_NORMAL:
		_block_counter = (size + 7) / 8;
		_video.resize(_block_counter);
		full_block_count = size / 8;

		for (uint16_t block = 0; block < full_block_count; ++block)
		{
			uint32_t packed =
				get_item(block, 0, 8) | get_item(block, 1, 8) |
				get_item(block, 2, 8) | get_item(block, 3, 8) |
				get_item(block, 4, 8) | get_item(block, 5, 8) |
				get_item(block, 6, 8) | get_item(block, 7, 8);
			// _video.data[block] = htonl(packed); // for Linux
		}

		if (full_block_count != _block_counter)
		{
			uint32_t packed =
				checked_item(full_block_count, size, 0, 8) | checked_item(full_block_count, size, 1, 8) |
				checked_item(full_block_count, size, 2, 8) | checked_item(full_block_count, size, 3, 8) |
				checked_item(full_block_count, size, 4, 8) | checked_item(full_block_count, size, 5, 8) |
				checked_item(full_block_count, size, 6, 8) | checked_item(full_block_count, size, 7, 8);
			// _video.data[full_block_count] = htonl(packed); for Linux
		}
		_fspec |= (1ULL << 8);
		_fspec |= (1ULL << 9);
		break;
	case D_MONO:
		_block_counter = (size + 31) / 32;
		_video.resize(_block_counter);
		full_block_count = size / 32;

		for (uint16_t block = 0; block < full_block_count; ++block)
		{
			uint32_t packed =
				get_item(block,  0, 32) | get_item(block,  1, 32) | get_item(block,  2, 32) | get_item(block,  3, 32) |
				get_item(block,  4, 32) | get_item(block,  5, 32) | get_item(block,  6, 32) | get_item(block,  7, 32) |
				get_item(block,  8, 32) | get_item(block,  9, 32) | get_item(block, 10, 32) | get_item(block, 11, 32) |
				get_item(block, 12, 32) | get_item(block, 13, 32) | get_item(block, 14, 32) | get_item(block, 15, 32) |
				get_item(block, 16, 32) | get_item(block, 17, 32) | get_item(block, 18, 32) | get_item(block, 19, 32) |
				get_item(block, 20, 32) | get_item(block, 21, 32) | get_item(block, 22, 32) | get_item(block, 23, 32) |
				get_item(block, 24, 32) | get_item(block, 25, 32) | get_item(block, 26, 32) | get_item(block, 27, 32) |
				get_item(block, 28, 32) | get_item(block, 29, 32) | get_item(block, 30, 32) | get_item(block, 31, 32);
			// _video.data[block] = htonl(packed);	// for Linux
		}
		if (full_block_count != _block_counter)
		{
			uint32_t packed =
				checked_item(full_block_count, size,  0, 32) | checked_item(full_block_count, size,  1, 32) |
				checked_item(full_block_count, size,  2, 32) | checked_item(full_block_count, size,  3, 32) |
				checked_item(full_block_count, size,  4, 32) | checked_item(full_block_count, size,  5, 32) |
				checked_item(full_block_count, size,  6, 32) | checked_item(full_block_count, size,  7, 32) |
				checked_item(full_block_count, size,  8, 32) | checked_item(full_block_count, size,  9, 32) |
				checked_item(full_block_count, size, 10, 32) | checked_item(full_block_count, size, 11, 32) |
				checked_item(full_block_count, size, 12, 32) | checked_item(full_block_count, size, 13, 32) |
				checked_item(full_block_count, size, 14, 32) | checked_item(full_block_count, size, 15, 32) |
				checked_item(full_block_count, size, 16, 32) | checked_item(full_block_count, size, 17, 32) |
				checked_item(full_block_count, size, 18, 32) | checked_item(full_block_count, size, 19, 32) |
				checked_item(full_block_count, size, 20, 32) | checked_item(full_block_count, size, 21, 32) |
				checked_item(full_block_count, size, 22, 32) | checked_item(full_block_count, size, 23, 32) |
				checked_item(full_block_count, size, 24, 32) | checked_item(full_block_count, size, 25, 32) |
				checked_item(full_block_count, size, 26, 32) | checked_item(full_block_count, size, 27, 32) |
				checked_item(full_block_count, size, 28, 32) | checked_item(full_block_count, size, 29, 32) |
				checked_item(full_block_count, size, 30, 32) | checked_item(full_block_count, size, 31, 32);
			// _video.data[full_block_count] = htonl(packed);	// for Linux
		}
		_fspec |= (1ULL << 8);
		_fspec |= (1ULL << 9);
		break;
	case D_HIGH:
		_block_counter = (size + 3) / 4;
		_video.resize(_block_counter);
		full_block_count = size / 4;

		for (uint16_t block = 0; block < full_block_count; ++block)
		{
			uint32_t packed =
				get_item(block, 0, 4) |
				get_item(block, 1, 4) |
				get_item(block, 2, 4) |
				get_item(block, 3, 4);
			// _video.data[block] = htonl(packed); // for Linux
		}
		if (full_block_count != _block_counter)
		{
			uint32_t packed =
				checked_item(full_block_count, size, 0, 4) |
				checked_item(full_block_count, size, 1, 4) |
				checked_item(full_block_count, size, 2, 4) |
				checked_item(full_block_count, size, 3, 4);
			// _video.data[full_block_count] = htonl(packed); // for Linux
		}
		_fspec |= (1ULL << 8);
		_fspec |= (1ULL << 9);
		break;
	case D_COMPOSITE:
		// В другой функции
		break;
	}
#undef checked_item
#undef get_item
}

void Cat240::set_video_unpacked(uint32_t* data, size_t size)
{
	switch (_resolution.depth)
	{
	case D_NORMAL:
	case D_MONO:
	case D_HIGH:
		// В другой функции
		break;
	case D_COMPOSITE:
		_block_counter = size;
		_video.resize(_block_counter);
		for (uint16_t block = 0; block < _block_counter; ++block)
		{
			// _video.data[block] = htonl(data[block]);	// for Linux
		}

		_fspec |= (1ULL << 8);
		_fspec |= (1ULL << 9);
		break;
	}
}

size_t Cat240::get_items_count() const
{
	if (!get_block_counter())
		return 0;
	switch (_resolution.depth)
	{
	case D_NORMAL:
		return _block_counter * 8;
	case D_MONO:
		return _block_counter * 32;
	case D_HIGH:
		return _block_counter * 4;
	case D_COMPOSITE:
		return _block_counter;
	}
	return 0;
}

void Cat240::get_video_unpacked(uint8_t** data, size_t* size) const
{
	size_t count = get_items_count();
	if (!get_video() || !count || !data || !size)
	{
		if (size)
			*size = 0;
		return;
	}

	if (void* ndata = realloc(*data, count * sizeof(uint8_t)))
	{
		*data = static_cast<uint8_t*>(ndata);
		*size = count;
	}
	else
	{
		*size = 0;
		return;
	}

#define get_item(block, i, block_size) ((*data)[block * block_size + i] = (packed >> (32 / block_size) * (block_size - 1 - i)) & ((1 << 32 / block_size) - 1))
	switch (_resolution.depth)
	{
	case D_NORMAL:
		for (uint16_t block = 0; block < _block_counter; ++block)
		{
			/*uint32_t packed = ntohl(_video.data[block]);	// for Linux
			get_item(block, 0, 8), get_item(block, 1, 8),
			get_item(block, 2, 8), get_item(block, 3, 8),
			get_item(block, 4, 8), get_item(block, 5, 8),
			get_item(block, 6, 8), get_item(block, 7, 8);*/
		}

		break;
	case D_MONO:
		for (uint16_t block = 0; block < _block_counter; ++block)
		{
			/*uint32_t packed = ntohl(_video.data[block]);		// for Linux
			get_item(block,  0, 32), get_item(block,  1, 32), get_item(block,  2, 32), get_item(block,  3, 32),
			get_item(block,  4, 32), get_item(block,  5, 32), get_item(block,  6, 32), get_item(block,  7, 32),
			get_item(block,  8, 32), get_item(block,  9, 32), get_item(block, 10, 32), get_item(block, 11, 32),
			get_item(block, 12, 32), get_item(block, 13, 32), get_item(block, 14, 32), get_item(block, 15, 32),
			get_item(block, 16, 32), get_item(block, 17, 32), get_item(block, 18, 32), get_item(block, 19, 32),
			get_item(block, 20, 32), get_item(block, 21, 32), get_item(block, 22, 32), get_item(block, 23, 32),
			get_item(block, 24, 32), get_item(block, 25, 32), get_item(block, 26, 32), get_item(block, 27, 32),
			get_item(block, 28, 32), get_item(block, 29, 32), get_item(block, 30, 32), get_item(block, 31, 32);*/
		}
		break;
	case D_HIGH:
		for (uint16_t block = 0; block < _block_counter; ++block)
		{
			/*uint32_t packed = ntohl(_video.data[block]);		// for Linux
			get_item(block, 0, 4),
			get_item(block, 1, 4),
			get_item(block, 2, 4),
			get_item(block, 3, 4);*/
		}
		break;
	case D_COMPOSITE:
		*size = 0;
		// В другой функции
		break;
	}
#undef get_item
}

void Cat240::get_video_unpacked(uint32_t** data, size_t* size) const
{
	size_t count = get_items_count();
	if (!get_video() || !count || !data || !size)
	{
		if (size)
			*size = 0;
		return;
	}

	if (void* ndata = realloc(*data, count * sizeof(uint32_t)))
	{
		*data = static_cast<uint32_t*>(ndata);
		*size = count;
	}
	else
	{
		*size = 0;
		return;
	}

	switch (_resolution.depth)
	{
	case D_NORMAL:
	case D_MONO:
	case D_HIGH:
		*size = 0;
		// В другой функции
		break;
	case D_COMPOSITE:
		for (uint16_t block = 0; block < _block_counter; ++block)
		{
			// (*data)[block] = ntohl(_video.data[block]);	// for Linux
		}
		break;
	}
}

void Cat240::compress_video()
{	// for Linux
	/*if (!get_video() || !get_block_counter())
		return;
	const size_t CHUNK = 256;
	if (_resolution.compression == C_DEFLATE)
	{
		int      ret;
		z_stream strm;
		strm.zalloc = Z_NULL;
		strm.zfree  = Z_NULL;
		strm.opaque = Z_NULL;

		int level = Z_BEST_SPEED;
		ret = deflateInit(&strm, level);

		if (ret != Z_OK)
		{
			fprintf(stderr, "deflateInit: rc = %d\n", ret);
			_block_counter = 0;
			_video.resize(0);
			return;
		}

		strm.avail_in = _block_counter * sizeof(uint32_t);
		strm.next_in  = reinterpret_cast<uint8_t*>(_video.data);

		uint8_t* new_video     = NULL;
		size_t   bytes_written = 0;
		do
		{
			size_t was = bytes_written;
			new_video = (uint8_t*) realloc(new_video, was + CHUNK);

			strm.avail_out = CHUNK;
			strm.next_out  = new_video + was;
			ret            = deflate(&strm, Z_FINISH);

			if (ret != Z_OK && ret != Z_STREAM_END)
			{
				fprintf(stderr, "deflate: rc = %d\n", ret);
				_block_counter = 0;
				_video.resize(0);
				return;
			}

			size_t have = CHUNK - strm.avail_out;
			bytes_written = bytes_written + have;
		}
		while (strm.avail_out == 0);

		deflateEnd(&strm);

		size_t extra_bytes = size_t(-bytes_written) % sizeof(uint32_t);
		new_video = (uint8_t*) realloc(new_video, bytes_written + extra_bytes);
		memset(new_video + bytes_written, 0, extra_bytes);
		bytes_written = bytes_written + extra_bytes;

		free(_video.data);
		_video.data    = (uint32_t*) new_video;
		_block_counter = bytes_written / sizeof(uint32_t);
	}*/
}

void Cat240::uncompress_video()
{	// for Linux
	/*if (!get_video() || !get_block_counter())
		return;
	const size_t CHUNK = 256;
	if (_resolution.compression == C_DEFLATE)
	{
		int      ret;
		z_stream strm;
		strm.zalloc   = Z_NULL;
		strm.zfree    = Z_NULL;
		strm.opaque   = Z_NULL;
		strm.avail_in = _block_counter * sizeof(uint32_t);
		strm.next_in  = reinterpret_cast<uint8_t*>(_video.data);
		ret           = inflateInit(&strm);

		if (ret != Z_OK)
		{
			fprintf(stderr, "inflateInit: rc = %d\n", ret);
			_block_counter = 0;
			_video.resize(0);
			return;
		}

		uint8_t* new_video  = NULL;
		size_t   bytes_read = 0;
		do
		{
			new_video      = (uint8_t*) realloc(new_video, bytes_read + CHUNK);
			strm.avail_out = CHUNK;
			strm.next_out  = new_video + bytes_read;
			ret            = inflate(&strm, Z_NO_FLUSH);

			if (ret != Z_OK && ret != Z_STREAM_END)
			{
				fprintf(stderr, "inflate: rc = %d\n", ret);
				_block_counter = 0;
				_video.resize(0);
				return;
			}

			size_t have = CHUNK - strm.avail_out;
			bytes_read = bytes_read + have;
		}
		while (strm.avail_out == 0);

		inflateEnd(&strm);

		free(_video.data);
		_block_counter = bytes_read / sizeof(uint32_t);
		_video.data    = (uint32_t*) new_video;
	}*/
}

struct Cat240::Reader :
	public Uap::Serializer, public AsterixReaderT
{
public:

	Reader(const char* data, size_t size) :
		AsterixReaderT(data, size) {};

	void readRecord(Cat240& v)
	{
		Asterix::readRecord(v, this);
	}

private:

	void dataSourceId(Cat240& r)
	{
		r._dsi.sac = readInt<8, unsigned>();
		r._dsi.sic = readInt<8, unsigned>();
	}

	void messageType(Cat240& r)
	{
		r._message_type = readInt<8, unsigned>();
	}

	void recordHeader(Cat240& r)
	{
		r._msg_index = readInt<32, unsigned>();
	}

	void summary(Cat240& r)
	{
		uint8_t len = readInt<8, unsigned>();
		r._summary.resize(len);
		for (int i = 0; i < len; ++i)
			r._summary[i] = readInt<8, signed>();
	}

	void headerNs(Cat240& r)
	{
		VideoHeader& h = r._header_ns;
		h.start_az = readDouble<16, unsigned>(AZ_LSB);
		h.end_az   = readDouble<16, unsigned>(AZ_LSB);
		h.start_rg = readInt<32, unsigned>();
		h.cell_dur = readDouble<32, unsigned>(CELL_DUR_NS);
	}

	void headerFs(Cat240& r)
	{
		VideoHeader& h = r._header_fs;
		h.start_az = readDouble<16, unsigned>(AZ_LSB);
		h.end_az   = readDouble<16, unsigned>(AZ_LSB);
		h.start_rg = readInt<32, unsigned>();
		h.cell_dur = readDouble<32, unsigned>(CELL_DUR_FS);
	}

	void resolution(Cat240& r)
	{
		r._resolution.spare = readInt<8, unsigned>();
		r._resolution.depth = BitDepth(readInt<8, unsigned>());
	}

	void blockCounter(Cat240& r)
	{
		r._block_counter = readInt<16, unsigned>();
	}

	void video(Cat240& r)
	{
		r._video.resize(r._block_counter);
		for (size_t i = 0; i < r._block_counter; ++i)
			r._video.data[i] = readBeUint32();
	}

	void timeOfDay(Cat240& r)
	{
		r._time = readDouble<24, unsigned>(TIME_LSB);
	}

	void reserved(Cat240& r)
	{
		AsterixReaderT::reserved(&r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat240& r)
	{
		AsterixReaderT::reserved_end();
	}

	void special(Cat240& r)
	{
		AsterixReaderT::special(r._sp);
	}

};

struct Cat240::Writer :
	public Uap::Serializer, public AsterixWriterT
{
public:

	Writer(char*& data, size_t& size) :
		AsterixWriterT(data, size) {};

	void writeRecord(const Cat240& v)
	{
		Asterix::writeRecord(v, this);
	}

private:

	void dataSourceId(Cat240& r)
	{
		writeInt<8, unsigned>(r._dsi.sac);
		writeInt<8, unsigned>(r._dsi.sic);
	}

	void messageType(Cat240& r)
	{
		writeInt<8, unsigned>(r._message_type);
	}

	void recordHeader(Cat240& r)
	{
		writeInt<32, unsigned>(r._msg_index);
	}

	void summary(Cat240& r)
	{
		uint8_t len = r._summary.size();
		writeInt<8, unsigned>(len);
		r._summary.resize(len);
		for (int i = 0; i < len; ++i)
			writeInt<8, signed>(r._summary[i]);
	}

	void headerNs(Cat240& r)
	{
		VideoHeader& h = r._header_ns;
		writeDoubleWrap<16>(AZ_LSB, h.start_az);
		writeDoubleWrap<16>(AZ_LSB, h.end_az);
		writeInt<32, unsigned>(h.start_rg);
		writeDouble<32, unsigned>(CELL_DUR_NS, h.cell_dur);
	}

	void headerFs(Cat240& r)
	{
		VideoHeader& h = r._header_fs;
		writeDoubleWrap<16>(AZ_LSB, h.start_az);
		writeDoubleWrap<16>(AZ_LSB, h.end_az);
		writeInt<32, unsigned>(h.start_rg);
		writeDouble<32, unsigned>(CELL_DUR_FS, h.cell_dur);
	}

	void resolution(Cat240& r)
	{
		writeInt<8, unsigned>(r._resolution.spare);
		writeInt<8, unsigned>(r._resolution.depth);
	}

	void blockCounter(Cat240& r)
	{
		writeInt<16, unsigned>(r._block_counter);
	}

	void video(Cat240& r)
	{
		for (size_t i = 0; i < r._block_counter; ++i)
			writeBeUint32(r._video.data[i]);
	}

	void timeOfDay(Cat240& r)
	{
		writeDouble<24, unsigned>(TIME_LSB, r._time);
	}

	void reserved(Cat240& r)
	{
		AsterixWriterT::reserved(r._fspec, LAST_FRN, RE_END);
	}

	void reserved_end(Cat240& r)
	{
		AsterixWriterT::reserved_end();
	}

	void special(Cat240& r)
	{
		AsterixWriterT::special(r._sp);
	}

};

AsterixRecord* Cat240::decode(const char* data, size_t size, AsterixError& err, size_t& read_size)
{
	if (size == 0)
	{
		err = A_EMPTY_BUFFER;
		return NULL;
	}
	Reader reader(data, size);

	Cat240* rec = new Cat240;
	reader.readRecord(*rec);
	read_size = reader.bytesRead();

	err = A_NO_ERROR;
	return rec;
}

char* Cat240::code(const AsterixRecord* record, size_t& write_size)
{
	if (record->category != 240)
		return NULL;
	char* data;
	{
		Writer writer(data, write_size);
		writer.writeRecord(*(const Cat240*) record);
	}
	return data;
}

}                                                          // namespace Cat240
