#ifndef _UTIL_H_
#define _UTIL_H_

// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 14.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

/**
 * @brief Перевод между алфавитами ASCII и IA-5
 * @param bs - символ алвавита для конвертации
 * @param rew - признак направления конвертации:
 *              0 - IA5 -> ASCII
 *              1 - ASCII -> IA5
 * @return символ конвертированный
 */
char asciiAndIa5alphabet(char bs, unsigned char rew);


#endif /* _UTIL_H_ */