#ifndef ASTERIX_TEMPLATE_HPP
#define ASTERIX_TEMPLATE_HPP

#include "asterix_reader.hpp"
#include "asterix_writer.hpp"

#include <vector>
#include <math.h>

namespace Detail
{
template <int, typename>
struct IntSize {};
#define specialize_type(t, k, s) template <> struct IntSize<k, s> { typedef t Type; };
specialize_type(uint8_t,   8, unsigned);
specialize_type(uint16_t, 16, unsigned);
specialize_type(uint32_t, 24, unsigned);
specialize_type(uint32_t, 32, unsigned);
specialize_type(uint64_t, 48, unsigned);
specialize_type(uint64_t, 56, unsigned);
specialize_type(uint64_t, 64, unsigned);
specialize_type(int8_t,    8, signed);
specialize_type(int16_t,  16, signed);
specialize_type(int32_t,  24, signed);
specialize_type(int32_t,  32, signed);
specialize_type(int64_t,  48, signed);
specialize_type(int64_t,  56, signed);
specialize_type(int64_t,  64, signed);
#undef specialize_type

template <typename A, typename B, typename T>
struct IfSameType;

template <typename A, typename T>
struct IfSameType<A, A, T>
{
	typedef T Type;
};

template <typename S>
struct is_signed;

template <>
struct is_signed<signed>
{
	enum
	{
		value = true
	};
};
template <>
struct is_signed<unsigned>
{
	enum
	{
		value = false
	};
};
};

template <ssize_t N, size_t D> struct Ratio
{
	Ratio() {}
	operator double() const
	{
		return N / double(D);
	}

};

class AsterixReaderT :
	public AsterixReader
{
	size_t  _re_start;
	uint8_t _re_size;

public:

	template <int I>
	typename Detail::IntSize<I, signed>::Type readInt()
	{
		return readInt<I, signed>();
	}

	template <int I, typename S>
	typename Detail::IfSameType<S, signed, typename Detail::IntSize<I, signed>::Type>::Type readInt()
	{
		switch (I)
		{
		case 8:  return readInt8();
		case 16: return readInt16();
		case 24: return readInt24();
		case 32: return readInt32();
		case 48: return readInt48();
		case 56: return readInt56();
		case 64: return readInt64();
		}
	}

	template <int I, typename S>
	typename Detail::IfSameType<S, unsigned, typename Detail::IntSize<I, S>::Type>::Type readInt()
	{
		switch (I)
		{
		case 8: return readUint8();
		case 16: return readUint16();
		case 24: return readUint24();
		case 32: return readUint32();
		case 48: return readUint48();
		case 56: return readUint56();
		case 64: return readUint64();
		}
	}

	template <int I, typename T>
	T readEnum()
	{
		return T(readInt<I, unsigned>());
	}

	template <int I, typename S>
	double readDouble(double lsb)
	{
		return readInt<I, S>() * lsb;
	}

	template <int I, typename S, ssize_t N, size_t D>
	double readDouble(Ratio<N, D> )
	{
		return readInt<I, S>() * N / double(D);
	}

	template <int I>
	double readDouble(double lsb)
	{
		return readDouble<I, signed>(lsb);
	}

	template <int I, ssize_t N, size_t D>
	double readDouble(Ratio<N, D> )
	{
		return readDouble<I, signed>(Ratio<N, D>());
	}

	AsterixReaderT(const char* data, size_t size) :
		AsterixReader(data, size), _re_start(), _re_size() {};

	void reserved(uint64_t* fspec, uint8_t LAST_FRN, uint8_t RE_END)
	{
		flush();
		_re_start = _in_size;

		uint8_t count = readInt<8, unsigned>();
		_re_size = count;
		uint8_t flags = 0;
		if (count >= 2)
			flags |= bit_reverse(readInt<8, unsigned>());
		flags  |= 1ULL << (LAST_FRN + RE_END);
		*fspec |= uint64_t(flags) << LAST_FRN;
	}

	void reserved_end()
	{
		flush();
		size_t pos = _in_size;
		for (size_t i = pos; i < _re_start + _re_size; ++i)
			readInt<8, unsigned>();
	}

	void special(std::vector<uint8_t>& sp)
	{
		uint8_t count = readInt<8, unsigned>();
		sp.resize(count);
		while (--count)
			sp.push_back(readInt<8, unsigned>());
	}

};

class AsterixWriterT :
	public AsterixWriter
{
	size_t _re_start;

public:

	template <int I>
	void writeInt(typename Detail::IntSize<I, signed>::Type v)
	{
		switch (I)
		{
		case 8:
			writeInt8(v);
			break;
		case 16:
			writeInt16(v);
			break;
		case 24:
			writeInt24(v);
			break;
		case 32:
			writeInt32(v);
			break;
		case 48:
			writeInt48(v);
			break;
		case 56:
			writeInt56(v);
			break;
		case 64:
			writeInt64(v);
			break;
		}
	}

	template <int I, typename S>
	void writeInt(typename Detail::IntSize<I, S>::Type v)
	{
		switch (I)
		{
		case 8:
			writeUint8(v);
			break;
		case 16:
			writeUint16(v);
			break;
		case 24:
			writeUint24(v);
			break;
		case 32:
			writeUint32(v);
			break;
		case 48:
			writeUint48(v);
			break;
		case 56:
			writeUint56(v);
			break;
		case 64:
			writeUint64(v);
			break;
		}
	}

	template <int I, typename T>
	void writeEnum(T v)
	{
		writeInt<I, unsigned>(v);
	}

	template <int I, typename S, ssize_t N, size_t D>
	void writeDouble(Ratio<N, D>, double v)
	{
		static const double MAX = (1LL << (I - Detail::is_signed<S>::value)) * 1. * N / D;
		static const double MIN = Detail::is_signed<S>::value ? -MAX : 0;
		// Использовано свойство NAN
		if (v >= MAX)
			writeInt<I, S>(MAX / N * D - 1);
		else if (Detail::is_signed<S>::value && v <= -MAX)
			writeInt<I, S>(MIN / N * D + 1);
		else
			writeInt<I, S>(v / N * D);
	}

	template <int I, ssize_t N, size_t D>
	void writeDoubleWrap(Ratio<N, D>, double v)
	{
		writeInt<I, unsigned>(v / N * D);
	}

	template <int I, ssize_t N, size_t D>
	void writeDouble(Ratio<N, D>, double v)
	{
		writeDouble<I, signed>(Ratio<N, D>(), v);
	}

	AsterixWriterT(char*& data, size_t& size) :
		AsterixWriter(data, size), _re_start() {};

	void reserved(uint64_t fspec, uint8_t LAST_FRN, uint8_t RE_END)
	{
		flush();
		_re_start = *_out_size;
		writeInt<8, unsigned>(0);                  // Заглушка для размера

		uint8_t flags = fspec >> LAST_FRN & ~(1ULL << RE_END);
		writeInt<8, unsigned>(bit_reverse(flags));
	}

	void reserved_end()
	{
		flush();
		size_t pos = *_out_size;
		(*_out_data)[_re_start] = pos - _re_start;
	}

	void special(std::vector<uint8_t>& sp)
	{
		uint8_t count = sp.size();
		writeInt<8, unsigned>(count);
		for (uint8_t i = 0; i < count; ++i)
			writeInt<8, unsigned>(sp[i]);
	}

};

namespace Asterix
{

template <typename Cat>
void callRecord(Cat& r, typename Cat::Uap::Serializer* s)
{
	const typename Cat::Uap * p = Cat::UAP;
	for (int i = 0; i < Cat::UAP_SIZE; ++i, ++p)
	{
		if ((r._fspec & p->info.mask) && p->serialize)
			(s->*p->serialize)(r);
	}
}

template <typename Cat, typename S>
void readRecord(Cat& v, S* s)
{
	v._fspec = s->readFspec();
	callRecord(v, s);
}

template <typename Cat, typename S>
void writeRecord(const Cat& v, S* s)
{
	uint64_t fspec = v._fspec & ((1ULL << Cat::LAST_FRN) - 1);
	s->writeFspec(fspec);
	callRecord(const_cast<Cat&>(v), s);
}

}                                                          // namespace Asterix

#endif                                                     /* ASTERIX_TEMPLATE_HPP */
