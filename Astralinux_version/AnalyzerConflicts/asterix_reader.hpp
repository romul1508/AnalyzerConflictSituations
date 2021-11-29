#ifndef ASTERIX_READER_HPP
#define ASTERIX_READER_HPP

#include <stdint.h>
#include <stdio.h>
// #include "arpa\inet.h" for Linux
// #include <winsock2.h> // for Windows

#ifdef __linux__
    // linux code goes here
    #include <arpa/inet.h>
#elif _WIN32
    // windows code goes here
    #include <winsock2.h>
#else
// #error "OS not supported!"
#endif


class AsterixReader
{
private:

	FILE* _file;

protected:

	const char* _in_data;
	size_t      _in_size;

	void flush()
	{
		_in_size = ftell(_file);
	}

public:

	static uint8_t bit_reverse(uint8_t v)
	{
		uint8_t r = 0;
		for (int i = 0; i < 8; ++i)
			r |= !!(v & (1 << i)) << (7 - i);
		return r;
	}

	AsterixReader(const char* data, size_t size)
		: _in_data(data), _in_size(size)
	{
		// _file = fmemopen((void*) _in_data, size, "rb");//for Linux
	}

	bool have_error()
	{
		return feof(_file) || ferror(_file);
	}

	virtual ~AsterixReader()
	{
		fclose(_file);
	}

	size_t bytesRead()
	{
		return ftell(_file);
	}

	void skipOneByte()
	{
		readUint8();
	}

	uint64_t readFxFlags()
	{
		uint8_t  byte  = 0;
		uint64_t flags = 0;
		bool     cont  = false;
		int      count = 0;
		do
		{
			byte   = readUint8();
			cont   = byte & 0x1;
			byte  &= ~0x1;
			flags |= uint64_t(byte) << count * 8;
			count++;
		}
		while (cont);
		return flags;
	}

	uint64_t readFspec()
	{
		uint8_t  byte  = 0;
		uint64_t flags = 0;
		int      count = 0;
		bool     cont  = false;
		do
		{
			byte   = bit_reverse(readUint8());
			cont   = byte & (1 << 7);
			byte  &= ~(1 << 7);
			flags |= uint64_t(byte) << (1 + count * 7);
			count++;
		}
		while (cont);
		return flags;
	}

	static uint64_t readFspec(uint8_t* in, size_t size)
	{
		uint8_t  byte  = 0;
		uint64_t flags = 0;
		int      count = 0;
		bool     cont  = false;
		if (size == 0)
			return 0;
		do
		{
			byte   = bit_reverse(*in++);
			cont   = byte & (1 << 7);
			byte  &= ~(1 << 7);
			flags |= uint64_t(byte) << (1 + count * 7);
			++count;
			--size;
		}
		while (size && cont);
		return flags;
	}

	uint8_t readUint8()
	{
		uint8_t v = 0;
		return fread(&v, sizeof(v), 1, _file) == 1 ? v : 0;
	}

	int8_t readInt8()
	{
		int8_t v = 0;
		return fread(&v, sizeof(v), 1, _file) == 1 ? v : 0;
	}

	uint16_t readUint16()
	{
		uint16_t v = 0;
		return fread(&v, sizeof(v), 1, _file) == 1 ? ntohs(v) : 0;		
	}

	int16_t readInt16()
	{
		int16_t v = 0;
		return fread(&v, sizeof(v), 1, _file) == 1 ? ntohs(v) : 0; 
	}

	uint32_t readUint24()
	{
		uint8_t o1 = readUint8();
		uint8_t o2 = readUint8();
		uint8_t o3 = readUint8();
		return (o1 << 16) | (o2 << 8) | o3;
	}

	int32_t readInt24()
	{
		uint32_t ui = readUint24();
		return int32_t(ui << 8) >> 8;
	}

	uint32_t readUint32()
	{
		uint32_t v = 0;
		return fread(&v, sizeof(v), 1, _file) == 1 ? ntohl(v) : 0; 		
	}

	uint32_t readBeUint32()
	{
		uint32_t v;
		return fread(&v, sizeof(v), 1, _file) == 1 ? v : 0;
	}

	int32_t readInt32()
	{
		int32_t v = 0;
		return fread(&v, sizeof(v), 1, _file) == 1 ? ntohl(v) : 0; 		
	}

	uint64_t readUint48()
	{
		return
		        uint64_t(readUint8()) << 40 |
		        uint64_t(readUint8()) << 32 |
		        uint64_t(readUint8()) << 24 |
		        uint64_t(readUint8()) << 16 |
		        uint64_t(readUint8()) << 8 |
		        uint64_t(readUint8()) << 0;
	}

	int64_t readInt48()
	{
		uint64_t v = readUint48();
		return int64_t(v << 16) >> 16;
	}

	uint64_t readUint56()
	{
		return
		        uint64_t(readUint8()) << 48 |
		        uint64_t(readUint8()) << 40 |
		        uint64_t(readUint8()) << 32 |
		        uint64_t(readUint8()) << 24 |
		        uint64_t(readUint8()) << 16 |
		        uint64_t(readUint8()) << 8 |
		        uint64_t(readUint8()) << 0;
	}

	int64_t readInt56()
	{
		uint64_t v = readUint56();
		return int64_t(v << 8) >> 8;
	}

	uint64_t readUint64()
	{
		uint64_t v = 0;
		// return fread(&v, sizeof(v), 1, _file) == 1 ? be64toh(v) : 0; // for Linux
		return v;	// for windows
	}

	int64_t readInt64()
	{
		int64_t v;
		// return fread(&v, sizeof(v), 1, _file) == 1 ? be64toh(v) : 0;		// for Linux
		return v;	// for windows
	}

	static char getIa5(uint8_t c)
	{
		static const char IA5_TABLE[4][17] =
		{
			"\0ABCDEFGHIJKLMNO",
			"PQRSTUVWXYZ_____",
			" _______________",
			"0123456789______"
		};
		return IA5_TABLE[(c >> 4) & 0x3][c & 0xF];
	}

	template <int N>
	static int16_t sign_extend(uint16_t a)
	{
		struct sign_extend
		{
			int16_t v : N;
		}
		s;
		return s.v = a;
	}

	static
	void cp866_to_wide(char* str_in, wchar_t* str)
	{
		for (; *str_in; ++str, ++str_in)
		{
			switch ((uint8_t) *str_in)
			{
#define conv(f, t) case f: \
	*str = t; break;
				conv(0x80, L'А')
				conv(0x81, L'Б')
				conv(0x82, L'В')
				conv(0x83, L'Г')
				conv(0x84, L'Д')
				conv(0x85, L'Е')
				conv(0x86, L'Ж')
				conv(0x87, L'З')
				conv(0x88, L'И')
				conv(0x89, L'Й')
				conv(0x8a, L'К')
				conv(0x8b, L'Л')
				conv(0x8c, L'М')
				conv(0x8d, L'Н')
				conv(0x8e, L'О')
				conv(0x8f, L'П')
				conv(0x90, L'Р')
				conv(0x91, L'С')
				conv(0x92, L'Т')
				conv(0x93, L'У')
				conv(0x94, L'Ф')
				conv(0x95, L'Х')
				conv(0x96, L'Ц')
				conv(0x97, L'Ч')
				conv(0x98, L'Ш')
				conv(0x99, L'Щ')
				conv(0x9a, L'Ъ')
				conv(0x9b, L'Ы')
				conv(0x9c, L'Ь')
				conv(0x9d, L'Э')
				conv(0x9e, L'Ю')
				conv(0x9f, L'Я')
#undef conv
			default:
				*str = *str_in;
			}
		}
	}

	static
	void cp866_to_rw(char* str_in, wchar_t* str)
	{
		for (; *str_in; ++str, ++str_in)
		{
			switch ((uint8_t) *str_in)
			{
#define conv(f, t) case f: \
	*str = t; break;
				conv(0x8b, L'Л')
				conv(0x8f, L'П')
#undef conv
			default:
				*str = *str_in;
			}
		}
	}

};

#endif                                                     /* ASTERIX_READER_HPP */
