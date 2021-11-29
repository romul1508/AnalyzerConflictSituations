#include "asterix.h"
#include "asterix_internal.h"
#include "asterix_cat_11.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strstream>
#include <algorithm>

char* Asterix::make_byte_stuff(const char* at, const char* end, size_t* size)
{
	*size = end - at + std::count(at, end, 0x10) + 4;
	char* r     = (char*) malloc(*size);
	char* r_end = r;

	*r_end++ = 0x10;
	*r_end++ = 0x02;

	while (at != end)
	{
		if (*at != 0x10)
		{
			const char* ten = std::find(at, end, 0x10);
			memcpy(r_end, at, ten - at);
			r_end += ten - at;
			at     = ten;
		}
		else
		{
			memcpy(r_end, "\x10\x10", 2);
			r_end += 2;
			at++;
		}
	}

	*r_end++ = 0x10;
	*r_end++ = 0x03;

	return r;
}

std::pair<ByteStuffState, const char*>
Asterix::unmake_byte_stuff(const char* at, const char* end, ByteStuffState _bs_state, std::vector<char>* out, bool single)
{
	while (at != end)
	{
		switch (_bs_state)
		{
		case BS_GARBAGE:
			_bs_state = BS_INDETERMINATE;
			++at;
			break;
		case BS_INDETERMINATE: {
			const char* ten = std::find(at, end, 0x10);
			if (ten != end)
			{
				// Найдена десятка
				_bs_state = BS_EXPECT_02;
				at        = ten + 1;
			}
			else
				at = ten;
			break;
		}
		case BS_EXPECT_02:
			if (*at == 0x02)
				_bs_state = BS_NORMAL;
			else
				_bs_state = BS_INDETERMINATE;
			++at;
			break;
		case BS_EXPECT_10_OR_03:
			if (*at == 0x10)
			{
				_bs_state = BS_NORMAL;
				if (out)
					out->push_back(0x10);
				++at;
			}
			else if (*at == 0x03)
			{
				_bs_state = BS_INDETERMINATE;
				++at;
				if (single)
					return std::make_pair(_bs_state, at);
			}
			else
			{
				_bs_state = BS_GARBAGE;
				if (single)
					return std::make_pair(_bs_state, at);
			}
			break;
		case BS_NORMAL: {
			const char* ten = std::find(at, end, 0x10);

			if (out)
				out->insert(out->end(), at, ten);
			if (ten != end)
			{
				_bs_state = BS_EXPECT_10_OR_03;
				at        = ten + 1;
			}
			else
				at = ten;
			break;
		}
		}
	}
	return std::make_pair(_bs_state, at);
}

std::string getStatusString(AsterixError err)
{
	switch (err)
	{
	case A_NO_ERROR:
		return "Нет ошибок";
	case A_EMPTY_BUFFER:
		return "Буфер пуст";
	case A_BUFFER_NOT_FILLED:
		return "Не достаточно данных в буфере";
	case A_CATEGORY_NOT_SUPPORTED:
		return "Категория не поддерживается";
	case A_DECODE_WRONG_SIZE:
		return "Неверный размер кодограммы";
	}
	return "";
}

void AsterixDecoder::init(bool old_cat21)
{	
	decoders[11] = Cat11::Cat11::decode;	
}

void AsterixCoder::init()
{	
	coders[11]  = Cat11::Cat11::code;	
}

AsterixDecoder::AsterixDecoder(DecodeMode mode, bool old_cat21)
	: decoders(), _current_record_cat(0), _data_block_left(0), _mode(mode), _bs_state(BS_INDETERMINATE)
{
	init(old_cat21);
}

void AsterixDecoder::feed(const uint8_t* data, size_t size)
{
	if (data)
		_input_buffer.insert(_input_buffer.end(), data, data + size);
}

AsterixRecord* AsterixDecoder::readRecord()
{
	const char* data   = _decode_buffer.data();
	DecoderFun  decode = decoders[_current_record_cat];

	size_t         read_size = 0;
	AsterixRecord* record    = decode(data, _data_block_left, _error_status, read_size);

	if (record)
	{
		_decode_buffer.erase(_decode_buffer.begin(), _decode_buffer.begin() + read_size);
		_data_block_left -= read_size;
	}
	else
	{
		_decode_buffer.clear();
		_data_block_left = 0;
	}

	if (!_data_block_left)
		_current_record_cat = 0;

	return record;
}

const int HEADER_SIZE = 3;

AsterixRecord* AsterixDecoder::decodeAsterix()
{
	if (_current_record_cat)
		return readRecord();

	size_t      size = _decode_buffer.size();
	const char* data = _decode_buffer.data();

	if (size < HEADER_SIZE)
	{
		_error_status = A_EMPTY_BUFFER;
		return NULL;
	}

	uint8_t  cat    = data[0];
	uint16_t a_size = (uint8_t(data[1]) << 8) + uint8_t(data[2]);

	if (a_size > size)
	{
		_error_status = A_BUFFER_NOT_FILLED;
		return NULL;
	}
	else if (a_size < HEADER_SIZE)
	{
		_error_status = A_DECODE_WRONG_SIZE;
		_decode_buffer.clear();
		return NULL;
	}
	else
	{
		a_size -= HEADER_SIZE;
	}

	if (decoders[cat])
	{
		_current_record_cat = cat;
		_data_block_left    = a_size;
		_decode_buffer.erase(_decode_buffer.begin(), _decode_buffer.begin() + HEADER_SIZE);

		return readRecord();
	}
	else
	{
		_error_status    = A_CATEGORY_NOT_SUPPORTED;
		_data_block_left = 0;
		_decode_buffer.erase(_decode_buffer.begin(), _decode_buffer.begin() + a_size + HEADER_SIZE);
		return NULL;
	}
}

AsterixRecord* AsterixDecoder::decode()
{
	switch (_mode)
	{
	case A_DECODE_SINGLE:
		_decode_buffer.insert(_decode_buffer.end(), _input_buffer.begin(), _input_buffer.end());
		_input_buffer.clear();
		return decodeAsterix();
	case A_DECODE_DELIMITED:
	{
		if (_input_buffer.empty())
			return decodeAsterix();
				
		char* at  = &*_input_buffer.begin();
		char* end;
		iter = _input_buffer.begin(); 
		while(iter != _input_buffer.end())
		{
				end = &*iter;
				iter++;
		}	
		end++;
		std::pair<ByteStuffState, const char*> bs = Asterix::unmake_byte_stuff(at, end, _bs_state, &_decode_buffer, false);
		_bs_state = bs.first;

		AsterixRecord* r = decodeAsterix();
		_input_buffer.clear();
		return r;
	}
	}

	return NULL;
}

AsterixError AsterixDecoder::getStatus()
{
	return _error_status;
}

std::string AsterixDecoder::getStatusString()
{
	return ::getStatusString(_error_status);
}

AsterixCoder::AsterixCoder(CodeMode mode, size_t mtu)
	: coders(), _current_record_cat(0), _mode(mode), _mtu(mtu < HEADER_SIZE ? (1 << 16) - 1 : mtu)
{
	init();
}

const uint8_t* AsterixCoder::take(size_t* size)
{
	produceOutput();
	if (_output_buffer.empty())
	{
		if (size)
			*size = 0;
		_error_status = A_EMPTY_BUFFER;
		return NULL;
	}
	std::pair<char*, size_t> data = _output_buffer.front();
	_output_buffer.pop_front();
	if (size)
		*size = data.second;
	return reinterpret_cast<uint8_t*>(data.first);
}

void AsterixCoder::produceOutput()
{ // for Linux and Windows
	size_t size_left     = _mtu;
	size_t max_size_left = _mtu;
	if (_current_record_cat == 0)
		return;
	bool delimit = false;
	static const uint8_t STUFF_SIZE = 2;

	switch (_mode)
	{
	case A_CODE_DELIMITED:
		// static const uint8_t STUFF_SIZE = 2; 
		delimit = true;
	case A_CODE_SINGLE: 
		{
			char*   data;
			size_t  size;

			uint8_t dummy[HEADER_SIZE];

			//------------------------------
			#ifdef __linux__ 
				 //linux code goes here
				FILE*   str = open_memstream(&data, &size); // Linux	
				fwrite(dummy, 1, sizeof(dummy), str);	// Linux
			#elif _WIN32
				 // windows code goes here
				size = sizeof(dummy);
				data = new char[size];
				std::ostrstream open_mem(data, size); // так будет работать в ОС Linux and Windows
				open_mem << dummy;
			#else
				// #error "OS not supported!"
			#endif
			//-------------------------------

			size_left -= HEADER_SIZE;

			if (delimit)
				size_left -= HEADER_SIZE - STUFF_SIZE * 2;
			max_size_left = size_left;

			Buffer::iterator it = _code_buffer.begin();
			for (Buffer::iterator end = _code_buffer.end(); it != end; ++it)
			{
				if (!it->first)
					continue;

				size_t coded_size = it->second;
				if (delimit)
					coded_size += std::count(it->first, it->first + it->second, 0x10);

				if (coded_size > size_left)
				{
					// Если кодограммa слишком большая, все равно пропустить
					if (size_left != max_size_left)
						break;
					size_left = 0;
				}
				else
					size_left -= it->second;

				#ifdef __linux__ 
					//linux code goes here
					fwrite(it->first, 1, it->second, str); // for Linux
				#elif _WIN32
					 // windows code goes here
								
					// создаем аналог функции fwrite(it->first, 1, it->second, str); // for Linux
					size_t  count_elem_first = it->second;  // количество элементов, которые нужно добавить к потоку open_mem
					// size_t  size_in_byte;							// их размер в байтах (1)
					
					char* it_first= it->first;

					for(int i=0; i<it->second; i++)
						open_mem << it_first[i];

				#else
					// #error "OS not supported!"
				#endif

				free(it->first);
			}

			#ifdef __linux__ 
				//linux code goes here
				fclose(str); // for Linux
			#elif _WIN32
				// windows code goes here
				// деструктор класса open_mem	вызывается автоматически
				// при выходе из данного метода
			#else
				// #error "OS not supported!"
			#endif
			

			data[0] = _current_record_cat;
			data[1] = size >> 8;
			data[2] = size;

			_code_buffer.erase(_code_buffer.begin(), it);
			if (!delimit)
			{
				_output_buffer.push_back(std::make_pair(data, size));
			}
			else
			{
				size_t delim_size;
				char*  stuffed = Asterix::make_byte_stuff(data, data + size, &delim_size);
				_output_buffer.push_back(std::make_pair(stuffed, delim_size));
				
				#ifdef __linux__ 
					//linux code goes here
					free(data);
				#elif _WIN32
					// windows code goes here
					delete data;
				#else
				// #error "OS not supported!"
				#endif
			}

			if (_code_buffer.empty())
				_current_record_cat = 0;
			break;
		}
	} // конец switch 
}

void AsterixCoder::code(AsterixRecord* record)
{
	if (!record)
		return;
	if (record->category != _current_record_cat)
		produceOutput();
	_current_record_cat = record->category;

	CoderFun code = coders[_current_record_cat];

	if (!code)
	{
		_error_status = A_CATEGORY_NOT_SUPPORTED;
		return;
	}

	size_t write_size = 0;
	char*  data       = code(record, write_size);

	_code_buffer.push_back(std::make_pair(data, write_size));
	_error_status = A_NO_ERROR;
}

AsterixError AsterixCoder::getStatus()
{
	return _error_status;
}

std::string AsterixCoder::getStatusString()
{
	return ::getStatusString(_error_status);
}

const char* Asterix::getFleetTypeString(uint8_t ft)
{
	static const char* vehicle_fleet_name[] =
	{
		"Unknown",
		"ATC equipment maintenance",
		"Airport maintenance",
		"Fire",
		"Bird scarer",
		"Snow plough",
		"Runway sweeper",
		"Emergency",
		"Police",
		"Bus",
		"Tug (push/tow)",
		"Grass cutter",
		"Fuel",
		"Baggage",
		"Catering",
		"Aircraft maintenance",
		"Flyco",
	};
	if (ft <= FT_FLYCO)
		return vehicle_fleet_name[ft];
	else
		return NULL;
}

const char* Asterix::getProgrammedMessageString(uint8_t pm)
{
	static const char* operation_name[] =
	{
		"Unknown",
		"Towing aircraft",
		"Follow me",
		"Runway check",
		"Emergency",
		"Work in progress"
	};
	if (pm <= PM_WORK_IN_PROGRESS)
		return operation_name[pm];
	else
		return NULL;
}

void AsterixDecoder::resetBuffers()
{
	_input_buffer.clear();
	_decode_buffer.clear();
	_current_record_cat = 0;
	_data_block_left    = 0;
}
