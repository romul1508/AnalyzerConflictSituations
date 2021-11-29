#include"Util.h"

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

	if (!rew)                                    // convertation IA5 -> ASCII
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
	{                                             // convertation   ASCII -> IA5
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