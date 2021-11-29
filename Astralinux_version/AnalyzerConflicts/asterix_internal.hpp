#ifndef ASTERIX_COMMON_INTERNAL_HPP
#define ASTERIX_COMMON_INTERNAL_HPP

#include "unistd.h"
#include <vector>
#include "asterix.hpp"

/// Геттер поля по указателю
/// \tparam T тип поля
/// \tparam C класс категории
/// \tparam field указатель на поле
/// \tparam frn Field Reference Number
template <typename T, typename C, T (C::* field), int frn, int LAST_FRN, int RE_FRN, int RE_END>
const T* AsterixRecord::get(const C* record)
{
	bool re_field = frn >= LAST_FRN ? !!(record->_fspec & (1ULL << RE_FRN)) : true;
	if (record && re_field && (record->_fspec & (1ULL << frn)))
		return &(record->*field);
	else
		return 0;
}

/// Сеттер поля по указателю
/// \tparam T тип поля
/// \tparam C класс категории
/// \tparam field указатель на поле
/// \tparam frn Field Reference Number
template <typename T, typename C, T (C::* field), int frn, int LAST_FRN, int RE_FRN, int RE_END>
void AsterixRecord::set(C* record, const T* data)
{
	static const uint64_t frn_mask = (1ULL << LAST_FRN) - 1;
	static const uint64_t re_mask  = 1ULL << RE_FRN | 1ULL << (LAST_FRN + RE_END);
	if (record)
	{
		bool re_field = frn >= LAST_FRN;
		if (data)
		{
			record->_fspec |= (1ULL << frn);
			if (re_field)
			{
				record->_fspec |= re_mask;
			}
			record->*field = *data;
		}
		else
		{
			record->_fspec &= ~(1ULL << frn);
			if (re_field)
			{
				if ((record->_fspec & frn_mask & ~re_mask) == 0)
					record->_fspec &= ~re_mask;
			}
		}
	}
}

/// Декларация геттера поля
/// \param field_type тип поля
/// \param cat_t имя класса категории
/// \param frn Field Reference Number
/// \param field_name имя поля
#define ASTERIX_DECLARE_GET(field_type, cat_t, frn, field_name)                                         \
	const field_type * get ## field_name () const {                                                 \
		return get<field_type, cat_t, &cat_t::field_name, frn, LAST_FRN, RE_FRN, RE_END>(this); \
	}

/// Декларация сеттера поля
/// \param field_type тип поля
/// \param cat_t имя класса категории
/// \param frn Field Reference Number
/// \param field_name имя поля
#define ASTERIX_DECLARE_SET(field_type, cat_t, frn, field_name)                                               \
	void set ## field_name (const field_type * data) {                                                    \
		return set<field_type, cat_t, &cat_t::field_name, frn, LAST_FRN, RE_FRN, RE_END>(this, data); \
	}

/// Декларация геттера описания поля
/// \param cat_t имя класса категории
/// \param frn Field Reference Number
/// \param field_name имя поля
#define ASTERIX_DECLARE_UAP(cat_t, frn, field_name)      \
	static const cat_t::Uap & uap ## field_name () { \
		return cat_t::UAP[frn];                  \
	}

/// Декларация поля UAP
/// \param cat_t имя класса категории
/// \param frn Field Reference Number
/// \param data_item Asterix Data Item
/// \param info Описание поля
/// \param length Размер поля в байтах, ноль для полей переменного размера
/// \param fun Функция-обработчик
#define ASTERIX_UAP_FIELD(cat_t, frn, data_item, info, length, fun)    \
	{                                                              \
		{                                                      \
			frn,  1ULL << (frn),  data_item,  info, length \
		},                                                     \
		&cat_t::Uap::Serializer::fun                           \
	}

/// Декларация поля структуры, геттера и сеттера
/// \param field_type тип поля
/// \param cat_t имя класса категории
/// \param frn Field Reference Number
/// \param field_name имя поля
#define ASTERIX_DECLARE(field_type, cat_t, frn, field_name)      \
private:                                                         \
	field_type field_name;                                   \
public:                                                          \
	ASTERIX_DECLARE_GET(field_type, cat_t, frn, field_name); \
	ASTERIX_DECLARE_SET(field_type, cat_t, frn, field_name); \
	ASTERIX_DECLARE_UAP(cat_t, frn, field_name);

// ****************************************************************************

/// Геттер поля-массива по указателю
/// \tparam T тип элемента
/// \tparam C класс категории
/// \tparam field указатель на поле-массив
/// \tparam frn Field Reference Number
template <typename T, typename C, std::vector<T>(C::* field), int frn>
const T* AsterixRecord::get(const C* record, size_t* size)
{
	if (record && (record->_fspec & (1ULL << frn)))
	{
		if (size)
			*size = (record->*field).size();
		return (record->*field).data();
	}
	else
	{
		if (size)
			*size = 0;
		return NULL;
	}
}

/// Сеттер поля-массива по указателю
/// \tparam T тип элемента
/// \tparam C класс категории
/// \tparam field указатель на поле-массив
/// \tparam frn Field Reference Number
template <typename T, typename C, std::vector<T>(C::* field), int frn>
void AsterixRecord::set(C* record, const T* data, size_t size)
{
	if (record)
	{
		if (data)
		{
			record->_fspec |= (1ULL << frn);
			(record->*field).resize(size);
			for (size_t i = 0; i < size; ++i)
				(record->*field)[i] = data[i];
		}
		else
		{
			record->_fspec &= ~(1ULL << frn);
			(record->*field).clear();
		}
	}
}

/// Добавление в массив
/// \tparam T тип элемента
/// \tparam C класс категории
/// \tparam field указатель на поле-массив
/// \tparam frn Field Reference Number
template <typename T, typename C, std::vector<T>(C::* field), int frn>
void AsterixRecord::set_add(C* record, const T data)
{
	if (record)
	{
		record->_fspec |= (1ULL << frn);
		(record->*field).resize((record->*field).size() + 1);
		(record->*field).back() = data;
	}
}

/// Декларация геттера поля-массива
/// \param field_type тип элемента
/// \param cat_t имя класса категории
/// \param frn Field Reference Number
/// \param field_name имя поля
#define ASTERIX_DECLARE_ARRAY_GET(field_type, cat_t, frn, field_name)               \
	const field_type * get ## field_name (size_t * size) const {                \
		return get<field_type, cat_t, &cat_t::field_name, frn>(this, size); \
	}

/// Декларация сеттера поля-массива
/// \param field_type тип элемента
/// \param cat_t имя класса категории
/// \param frn Field Reference Number
/// \param field_name имя поля
#define ASTERIX_DECLARE_ARRAY_SET(field_type, cat_t, frn, field_name)                     \
	void set ## field_name (const field_type * data, size_t size) {                   \
		return set<field_type, cat_t, &cat_t::field_name, frn>(this, data, size); \
	}

/// Декларация функции добавления
/// \param field_type тип элемента
/// \param cat_t имя класса категории
/// \param frn Field Reference Number
/// \param field_name имя поля
#define ASTERIX_DECLARE_ARRAY_ADD(field_type, cat_t, frn, field_name)                   \
	void add ## field_name (const field_type data) {                                \
		return set_add<field_type, cat_t, &cat_t::field_name, frn>(this, data); \
	}

/// Декларация поля-массива структуры, геттера и сеттера
/// \param field_type тип элемента
/// \param cat_t имя класса категории
/// \param frn Field Reference Number
/// \param field_name имя поля
#define ASTERIX_DECLARE_ARRAY(field_type, cat_t, frn, field_name)      \
private:                                                               \
	std::vector<field_type> field_name;                            \
public:                                                                \
	ASTERIX_DECLARE_ARRAY_GET(field_type, cat_t, frn, field_name); \
	ASTERIX_DECLARE_ARRAY_SET(field_type, cat_t, frn, field_name); \
	ASTERIX_DECLARE_ARRAY_ADD(field_type, cat_t, frn, field_name); \
	ASTERIX_DECLARE_UAP(cat_t, frn, field_name);



#endif                                                     /* ASTERIX_COMMON_INTERNAL_HPP */
