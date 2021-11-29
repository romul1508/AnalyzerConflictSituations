#ifndef ASTERIX_WRITER_HPP
#define ASTERIX_WRITER_HPP

#include <stdint.h>
#include <stdio.h>
// #include <arpa/inet.h>

class AsterixWriter
{
private:

	FILE* _file;

protected:

	char**  _out_data;
	size_t* _out_size;

	void flush()
	{
		fflush(_file);
	}

public:

	static uint8_t bit_reverse(uint8_t v)
	{
		uint8_t r = 0;
		for (int i = 0; i < 8; ++i)
			r |= !!(v & (1 << i)) << (7 - i);
		return r;
	}

	AsterixWriter(char*& data, size_t& size)
		: _out_data(&data), _out_size(&size)
	{
		// _file = open_memstream(&data, &size); // for Linux
	}

	virtual ~AsterixWriter()
	{
		fclose(_file);
	}

	void skipOneByte()
	{
		writeUint8(0x00);
	}

	void writeFxFlags(uint64_t flags)
	{
		bool cont = false;
		do
		{
			uint8_t byte = flags & 0xFE;
			flags = flags >> 8;
			cont  = flags != 0;
			writeUint8(byte | cont);
		}
		while (cont);
	}

	void writeFspec(uint64_t flags)
	{
		flags = flags >> 1;
		bool cont = false;
		do
		{
			uint8_t byte = flags & 0x7F;
			flags = flags >> 7;
			cont  = flags != 0;
			byte |= cont << 7;
			writeUint8(bit_reverse(byte));
		}
		while (cont);
	}

	void writeUint8(uint8_t v)
	{
		fwrite(&v, sizeof(v), 1, _file);
	}

	void writeInt8(int8_t v)
	{
		fwrite(&v, sizeof(v), 1, _file);
	}

	void writeUint16(uint16_t v)
	{
		v = htons(v);	
		fwrite(&v, sizeof(v), 1, _file);
	}

	void writeInt16(int16_t v)
	{
		v = htons(v);	
		fwrite(&v, sizeof(v), 1, _file);
	}

	void writeUint24(uint32_t v)
	{
		writeUint8(v >> 16);
		writeUint8(v >> 8);
		writeUint8(v >> 0);
	}

	void writeInt24(int32_t v)
	{
		writeUint24(v);
	}

	void writeUint32(uint32_t v)
	{
		v = htonl(v);	
		fwrite(&v, sizeof(v), 1, _file);
	}

	void writeBeUint32(uint32_t v)
	{
		fwrite(&v, sizeof(v), 1, _file);
	}

	void writeInt32(int32_t v)
	{
		v = htonl(v);	
		fwrite(&v, sizeof(v), 1, _file);
	}

	void writeUint48(uint64_t v)
	{
		writeUint8(v >> 40);
		writeUint8(v >> 32);
		writeUint8(v >> 24);
		writeUint8(v >> 16);
		writeUint8(v >> 8);
		writeUint8(v >> 0);
	}

	void writeInt48(int64_t v)
	{
		writeUint48(v);
	}

	void writeUint56(uint64_t v)
	{
		writeUint8(v >> 48);
		writeUint8(v >> 40);
		writeUint8(v >> 32);
		writeUint8(v >> 24);
		writeUint8(v >> 16);
		writeUint8(v >> 8);
		writeUint8(v >> 0);
	}

	void writeInt56(int64_t v)
	{
		writeUint56(v);
	}

	void writeUint64(uint64_t v)
	{
		 // v = htobe64(v);		// for Linux
		// fwrite(&v, sizeof(v), 1, _file);
	}

	void writeInt64(int64_t v)
	{
		// v = htobe64(v);	// for Linux
		// fwrite(&v, sizeof(v), 1, _file);
	}

	uint8_t getIa5(char c)
	{
		if (c >= '0' && c <= '9')
			return c;
		if (c == ' ')
			return 0x20;
		if (c >= 'A' && c <= 'Z')
			return c - '@';
		return 0x20;
	}

};

#endif                                                     /* ASTERIX_WRITER_HPP */
