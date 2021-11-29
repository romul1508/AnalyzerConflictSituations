#ifndef _SAX_PARS_RELIS_
#define _SAX_PARS_RELIS_

#include "SAXparser.h"

namespace sax_pars_xml{


void SaxParsXMLWorker::trim(const char* strContentXMLElem, char* strChistContentXMLElem, unsigned int* lenStrChistContentXMLElem){
	// Убирает пробелы из начала и из конца строки
	int k, j, begChistName, endChistName;
	endChistName = 0;
	size_t lenStrNameElem = strlen(strContentXMLElem);

	for(k=0;k<lenStrNameElem;k++){
		if(strContentXMLElem[k] != ' '){
			begChistName = k;
			break;
		}
	}
	for(k=lenStrNameElem-1;k>0;k--){
		if(strContentXMLElem[k] != ' '){
			endChistName = k;
			break;
		}
	}

	if(*lenStrChistContentXMLElem == 0){
		// Если память под строку без пробелов не выделена
		// то заполним  только поле с размером такой строки и выходим
	    *lenStrChistContentXMLElem = endChistName - begChistName + 2;
		return;
	}


	for(k = 0, j = begChistName; k<(endChistName - begChistName + 1); k++, j++)
		strChistContentXMLElem[k] =  strContentXMLElem[j];
	strChistContentXMLElem[k+1] = '\0';

}

// при старте документа счетчик секций обнуляется
// состояния входов в элементы принимают значения false
// в случае успеха, возвращает 1, в противном случае 0
int AirportsXMLWorker::startDocument(void* addStruct){
    if(!addStruct)
	{
		ern::Log::getInstance().Write("Error, the parameter is incorrect in method: \n");
		ern::Log::getInstance().Write("int AirportsXMLWorker::startDocument(void*)\n");
		return 0;
	}
	else
	{
		LPAddAirportsStruct pAddAirportsStruct = (LPAddAirportsStruct) addStruct;

		pAddAirportsStruct->indexPointSection = 0;
		pAddAirportsStruct->isElemNameAirport = false;
		pAddAirportsStruct->isElemHeightCTA = false;
		pAddAirportsStruct->isElemLatCTA = false;
		pAddAirportsStruct->isElemLonCTA = false;
		pAddAirportsStruct->isElemMagDeclCTA = false;
		pAddAirportsStruct->isElemCaptionVPP1 = false;
		pAddAirportsStruct->isElemIdVPP1 = false;
		pAddAirportsStruct->isElemCatVPP1 = false;
		pAddAirportsStruct->isElemAngleVPP1 = false;
		pAddAirportsStruct->isElemRunwayLatVPP1 = false;
		pAddAirportsStruct->isElemRunwayLonVPP1 = false;
		pAddAirportsStruct->isElemLenghtVPP1 = false;
		pAddAirportsStruct->isElemWidthVPP1 = false;
		pAddAirportsStruct->isElemSignLeftVPP1 = false;
		pAddAirportsStruct->isElemSignRightVPP1 = false;
		pAddAirportsStruct->isElemNorthWgsLatVPP1 = false;
		pAddAirportsStruct->isElemNorthWgsLonVPP1 = false;
		pAddAirportsStruct->isElemSouthWgsLatVPP1 = false;	
		pAddAirportsStruct->isElemSouthWgsLonVPP1 = false;	
		pAddAirportsStruct->isElemWestWgsLatVPP1 = false;
		pAddAirportsStruct->isElemWestWgsLonVPP1 = false;
		pAddAirportsStruct->isElemEastWgsLatVPP1 = false;
		pAddAirportsStruct->isElemEastWgsLonVPP1 = false;

		pAddAirportsStruct->isElemCaptionVPP2 = false;
		pAddAirportsStruct->isElemIdVPP2 = false;
		pAddAirportsStruct->isElemCatVPP2 = false;
		pAddAirportsStruct->isElemAngleVPP2 = false;
		pAddAirportsStruct->isElemRunwayLatVPP2 = false;
		pAddAirportsStruct->isElemRunwayLonVPP2 = false;
		pAddAirportsStruct->isElemLenghtVPP2 = false;
		pAddAirportsStruct->isElemWidthVPP2 = false;
		pAddAirportsStruct->isElemSignLeftVPP2 = false;
		pAddAirportsStruct->isElemSignRightVPP2 = false;
		pAddAirportsStruct->isElemNorthWgsLatVPP2 = false;
		pAddAirportsStruct->isElemNorthWgsLonVPP2 = false;
		pAddAirportsStruct->isElemSouthWgsLatVPP2 = false;	
		pAddAirportsStruct->isElemSouthWgsLonVPP2 = false;	
		pAddAirportsStruct->isElemWestWgsLatVPP2 = false;
		pAddAirportsStruct->isElemWestWgsLonVPP2 = false;
		pAddAirportsStruct->isElemEastWgsLatVPP2 = false;
		pAddAirportsStruct->isElemEastWgsLonVPP2 = false;
		return 1;
	}	
}

// изменяет статус искомого по const char* nameElem элемента, 
// в который произошло вхождение
// в случае успешного выполнение метода возвращает 1, в противном случае 0
int AirportsXMLWorker::startElement(const char* nameElem, void* addStruct){
	// Обработчик события вхождения в XML-элемент
	int err;
	if (!nameElem || !addStruct)
	{
		ern::Log::getInstance().Write("Error, initialized the input parameters in the method: \n");
		ern::Log::getInstance().Write("int AirportsXMLWorker::startElement(const char*, void*)\n");
		return 0;
	}
	
	err = 1;
	LPAddAirportsStruct pAddAirportsStruct = (LPAddAirportsStruct) addStruct;
	if(!strcmp(nameElem, "name") )
		pAddAirportsStruct->isElemNameAirport = true;
	else if(!strcmp(nameElem, "height_cta"))
		pAddAirportsStruct->isElemHeightCTA = true; 
	else if(!strcmp(nameElem, "lat_cta"))
		pAddAirportsStruct->isElemLatCTA = true; 
	else if(!strcmp(nameElem, "lon_cta"))
		pAddAirportsStruct->isElemLonCTA = true;		 
	else if(!strcmp(nameElem, "mag_decl_cta"))
		pAddAirportsStruct->isElemMagDeclCTA = true;
	else if(!strcmp(nameElem, "caption_vpp1"))
		pAddAirportsStruct->isElemCaptionVPP1 = true;
	else if(!strcmp(nameElem, "id_vpp1"))
		pAddAirportsStruct->isElemIdVPP1 = true;
	else if(!strcmp(nameElem, "cat_vpp1"))
		pAddAirportsStruct->isElemCatVPP1 = true;
	else if(!strcmp(nameElem, "angle_vpp1"))
		pAddAirportsStruct->isElemAngleVPP1 = true;
	else if(!strcmp(nameElem, "runway_lat_vpp1"))
		pAddAirportsStruct->isElemRunwayLatVPP1 = true;
	else if(!strcmp(nameElem, "runway_lon_vpp1"))
		pAddAirportsStruct->isElemRunwayLonVPP1 = true;
	else if(!strcmp(nameElem, "lenght_vpp1"))
		pAddAirportsStruct->isElemLenghtVPP1 = true;
	else if(!strcmp(nameElem, "width_vpp1"))
		pAddAirportsStruct->isElemWidthVPP1 = true;
	else if(!strcmp(nameElem, "sign_left_vpp1"))
		pAddAirportsStruct->isElemSignLeftVPP1 = true;
	else if(!strcmp(nameElem, "sign_right_vpp1"))
		pAddAirportsStruct->isElemSignRightVPP1 = true;
	else if(!strcmp(nameElem, "north_wgs_lat_vpp1"))
		pAddAirportsStruct->isElemNorthWgsLatVPP1 = true;
	else if(!strcmp(nameElem, "north_wgs_lon_vpp1"))
		pAddAirportsStruct->isElemNorthWgsLonVPP1 = true;
	else if(!strcmp(nameElem, "south_wgs_lat_vpp1"))
		pAddAirportsStruct->isElemSouthWgsLatVPP1 = true;
	else if(!strcmp(nameElem, "south_wgs_lon_vpp1"))
		pAddAirportsStruct->isElemSouthWgsLonVPP1 = true;
	else if(!strcmp(nameElem, "west_wgs_lat_vpp1"))
		pAddAirportsStruct->isElemWestWgsLatVPP1 = true;
	else if(!strcmp(nameElem, "west_wgs_lon_vpp1"))
		pAddAirportsStruct->isElemWestWgsLonVPP1 = true;
	else if(!strcmp(nameElem, "east_wgs_lat_vpp1"))
		pAddAirportsStruct->isElemEastWgsLatVPP1 = true;
	else if(!strcmp(nameElem, "east_wgs_lon_vpp1"))
		pAddAirportsStruct->isElemEastWgsLonVPP1 = true;
	else if(!strcmp(nameElem, "caption_vpp2"))
		pAddAirportsStruct->isElemCaptionVPP2 = true;
	else if(!strcmp(nameElem, "id_vpp2"))
		pAddAirportsStruct->isElemIdVPP2 = true;
	else if(!strcmp(nameElem, "cat_vpp2"))
		pAddAirportsStruct->isElemCatVPP2 = true;
	else if(!strcmp(nameElem, "angle_vpp2"))
		pAddAirportsStruct->isElemAngleVPP2 = true;
	else if(!strcmp(nameElem, "runway_lat_vpp2"))
		pAddAirportsStruct->isElemRunwayLatVPP2 = true;
	else if(!strcmp(nameElem, "runway_lon_vpp2"))
		pAddAirportsStruct->isElemRunwayLonVPP2 = true;
	else if(!strcmp(nameElem, "lenght_vpp2"))
		pAddAirportsStruct->isElemLenghtVPP2 = true;
	else if(!strcmp(nameElem, "width_vpp2"))
		pAddAirportsStruct->isElemWidthVPP2 = true;
	else if(!strcmp(nameElem, "sign_left_vpp2"))
		pAddAirportsStruct->isElemSignLeftVPP2 = true;
	else if(!strcmp(nameElem, "sign_right_vpp2"))
		pAddAirportsStruct->isElemSignRightVPP2 = true;
	else if(!strcmp(nameElem, "north_wgs_lat_vpp2"))
		pAddAirportsStruct->isElemNorthWgsLatVPP2 = true;
	else if(!strcmp(nameElem, "north_wgs_lon_vpp2"))
		pAddAirportsStruct->isElemNorthWgsLonVPP2 = true;
	else if(!strcmp(nameElem, "south_wgs_lat_vpp2"))
		pAddAirportsStruct->isElemSouthWgsLatVPP2 = true;
	else if(!strcmp(nameElem, "south_wgs_lon_vpp2"))
		pAddAirportsStruct->isElemSouthWgsLonVPP2 = true;
	else if(!strcmp(nameElem, "west_wgs_lat_vpp2"))
		pAddAirportsStruct->isElemWestWgsLatVPP2 = true;
	else if(!strcmp(nameElem, "west_wgs_lon_vpp2"))
		pAddAirportsStruct->isElemWestWgsLonVPP2 = true;
	else if(!strcmp(nameElem, "east_wgs_lat_vpp2"))
		pAddAirportsStruct->isElemEastWgsLatVPP2 = true;
	else if(!strcmp(nameElem, "east_wgs_lon_vpp2"))
		pAddAirportsStruct->isElemEastWgsLonVPP2 = true;
	/*else
	{
		char msgErr[80];
		sprintf(msgErr, "Error, XML-element %s not found in the method: \n", nameElem);
		ern::Log::getInstance().Write(msgErr);
		ern::Log::getInstance().Write("int AirportsXMLWorker::startElement(const char*, void*)\n");
		return 0;
	}*/
	return err;
}

// typedef struct
//	{
//		int indexPointSection;	
//		bool isElemNameAirport;					//		char name_section[BUFF_NAME_AIRPORTS];
//		bool isElemHeightCTA;					//		double height_cta;
//		bool isElemLatCTA;						//		double lat_cta;
//		bool isElemLonCTA;						//		double lon_cta;
//		bool isElemMagDeclCTA;					//		int mag_decl_cta;
//		bool isElemCaptionVPP1;					//		char caption_vpp1[BUFF_CAPTION_VPP];
//		bool isElemIdVPP1;						//		int id_vpp1;
//		bool isElemCatVPP1;						//		char cat_vpp1;
//		bool isElemAngleVPP1;					//		double angle_vpp1;
//		bool isElemRunwayLatVPP1;			//		double runway_lat_vpp1;
//		bool isElemRunwayLonVPP1;			//		double runway_lon_vpp1;
//		bool isElemLenghtVPP1;					//		int lenght_vpp1;
//		bool isElemWidthVPP1;					//		int width_vpp1;
//		bool isElemSignLeftVPP1;				//		char sign_left_vpp1[BUFF_SIGN_VPP];
//		bool isElemSignRightVPP1;				//		char sign_right_vpp1[BUFF_SIGN_VPP];

//		bool isElemNorthWgsLatVPP1;			//		double north_wgs_lat_vpp1;
//		bool isElemNorthWgsLonVPP1;		//		double north_wgs_lon_vpp1;
//		bool isElemSouthWgsLatVPP1;				//		double south_wgs_lat_vpp1
//		bool isElemSouthWgsLonVPP1;				//		double south_wgs_lon_vpp1
//		bool isElemWestWgsLatVPP1;					//		double west_wgs_lat_vpp1;
//		bool isElemWestWgsLonVPP1;				//		double west_wgs_lon_vpp1;
//		bool isElemEastWgsLatVPP1;					//		double east_wgs_lat_vpp1;
//		bool isElemEastWgsLonVPP1;					//		double east_wgs_lon_vpp1;

//		bool isElemCaptionVPP2;					//		char caption_vpp2[BUFF_CAPTION_VPP];
//		bool isElemIdVPP2;						//		int id_vpp2;
//		bool isElemCatVPP2;						//		char cat_vpp2;
//		bool isElemAngleVPP2;					//		double angle_vpp2;
//		bool isElemRunwayLatVPP2;			//		double runway_lat_vpp2;
//		bool isElemLenghtVPP2;					//		double runway_lon_vpp2;
//		bool isElemWidthVPP2;					//		int width_vpp2;
//		bool isElemSignLeftVPP2;				//		char sign_left_vpp2[BUFF_SIGN_VPP];
//		bool isElemSignRightVPP2;				//		char sign_right_vpp2[BUFF_SIGN_VPP];

//		bool isElemNorthWgsLatVPP2;			//		double north_wgs_lat_vpp2;
//		bool isElemNorthWgsLonVPP2;		//		double north_wgs_lon_vpp2;
//		bool isElemSouthWgsLatVPP2;				//		double south_wgs_lat_vpp2;
//		bool isElemSouthWgsLonVPP2;				//		double south_wgs_lon_vpp2;
//		bool isElemWestWgsLatVPP2;					//		double west_wgs_lat_vpp2;
//		bool isElemWestWgsLonVPP2;				//		double west_wgs_lon_vpp2;
//		bool isElemEastWgsLatVPP2;					//		double east_wgs_lat_vpp2;
//		bool isElemEastWgsLonVPP2;					//		double east_wgs_lon_vpp2;
//	} TAddAirportsStruct;

// инициализирует найденный xml-элемент соответствующим значением
// в случае ошибки возвращает 0, в противном случае 1
int AirportsXMLWorker::characters(const char* buff, void* point_section, void* addStruct){
	int err;
	if (!buff || !point_section || !addStruct)
	{
		ern::Log::getInstance().Write("Error, initialized the input parameters in the method: \n");
		ern::Log::getInstance().Write("int AirportsXMLWorker::characters(const char*, void*, void*)\n");
		return 0;
	}
	err = 1;

	LPAddAirportsStruct pAddAirportsStruct = (LPAddAirportsStruct) addStruct;
    LPAirportSection lpAirportSections = (LPAirportSection)point_section;

	if(pAddAirportsStruct->isElemNameAirport){
		strncpy(lpAirportSections[pAddAirportsStruct->indexPointSection].name_section, buff, BUFF_NAME_AIRPORTS);
		pAddAirportsStruct->isElemNameAirport = false;
	}
	else if(pAddAirportsStruct->isElemHeightCTA){
		lpAirportSections[pAddAirportsStruct->indexPointSection].height_cta = atof(buff);
		pAddAirportsStruct->isElemHeightCTA = false;
	}
	else if(pAddAirportsStruct->isElemLatCTA){
		lpAirportSections[pAddAirportsStruct->indexPointSection].lat_cta = atof(buff);
		pAddAirportsStruct->isElemLatCTA = false;
	}
	else if(pAddAirportsStruct->isElemLonCTA){
		lpAirportSections[pAddAirportsStruct->indexPointSection].lon_cta = atof(buff);
		pAddAirportsStruct->isElemLonCTA = false;
	}
	else if(pAddAirportsStruct->isElemMagDeclCTA){
		lpAirportSections[pAddAirportsStruct->indexPointSection].mag_decl_cta = atoi(buff);
		pAddAirportsStruct->isElemMagDeclCTA = false;
	}
	else if(pAddAirportsStruct->isElemCaptionVPP1){
		strncpy(lpAirportSections[pAddAirportsStruct->indexPointSection].caption_vpp1, buff, BUFF_CAPTION_VPP);
		pAddAirportsStruct->isElemCaptionVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemIdVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].id_vpp1 = atoi(buff);
		pAddAirportsStruct->isElemIdVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemCatVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].cat_vpp1 = buff[0];
		pAddAirportsStruct->isElemCatVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemAngleVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].angle_vpp1 = atof(buff);
		pAddAirportsStruct->isElemAngleVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemRunwayLatVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].runway_lat_vpp1 = atof(buff);
		pAddAirportsStruct->isElemRunwayLatVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemRunwayLonVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].runway_lon_vpp1 = atof(buff);
		pAddAirportsStruct->isElemRunwayLonVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemLenghtVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].lenght_vpp1 = atoi(buff);
		pAddAirportsStruct->isElemLenghtVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemWidthVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].width_vpp1 = atoi(buff);
		pAddAirportsStruct->isElemWidthVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemSignLeftVPP1){
		strncpy(lpAirportSections[pAddAirportsStruct->indexPointSection].sign_left_vpp1, buff, BUFF_SIGN_VPP);
		pAddAirportsStruct->isElemSignLeftVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemSignRightVPP1){
		strncpy(lpAirportSections[pAddAirportsStruct->indexPointSection].sign_right_vpp1, buff, BUFF_SIGN_VPP);
		pAddAirportsStruct->isElemSignRightVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemNorthWgsLatVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].north_wgs_lat_vpp1 = atof(buff);
		pAddAirportsStruct->isElemNorthWgsLatVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemNorthWgsLonVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].north_wgs_lon_vpp1 = atof(buff);
		pAddAirportsStruct->isElemNorthWgsLonVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemSouthWgsLatVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].south_wgs_lat_vpp1 = atof(buff);
		pAddAirportsStruct->isElemSouthWgsLatVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemSouthWgsLonVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].south_wgs_lon_vpp1 = atof(buff);
		pAddAirportsStruct->isElemSouthWgsLonVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemWestWgsLatVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].west_wgs_lat_vpp1 = atof(buff);
		pAddAirportsStruct->isElemWestWgsLatVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemWestWgsLonVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].west_wgs_lon_vpp1 = atof(buff);
		pAddAirportsStruct->isElemWestWgsLonVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemEastWgsLatVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].east_wgs_lat_vpp1 = atof(buff);
		pAddAirportsStruct->isElemEastWgsLatVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemEastWgsLonVPP1){
		lpAirportSections[pAddAirportsStruct->indexPointSection].east_wgs_lon_vpp1 = atof(buff);
		pAddAirportsStruct->isElemEastWgsLonVPP1 = false;
	}
	else if(pAddAirportsStruct->isElemCaptionVPP2){
		strncpy(lpAirportSections[pAddAirportsStruct->indexPointSection].caption_vpp2, buff, BUFF_CAPTION_VPP);
		pAddAirportsStruct->isElemCaptionVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemIdVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].id_vpp2 = atoi(buff);
		pAddAirportsStruct->isElemIdVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemCatVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].cat_vpp2 = buff[0];
		pAddAirportsStruct->isElemCatVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemAngleVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].angle_vpp2 = atof(buff);
		pAddAirportsStruct->isElemAngleVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemRunwayLatVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].runway_lat_vpp2 = atof(buff);
		pAddAirportsStruct->isElemRunwayLatVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemRunwayLonVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].runway_lon_vpp2 = atof(buff);
		pAddAirportsStruct->isElemRunwayLonVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemLenghtVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].lenght_vpp2 = atoi(buff);
		pAddAirportsStruct->isElemLenghtVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemWidthVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].width_vpp2 = atoi(buff);
		pAddAirportsStruct->isElemWidthVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemSignLeftVPP2){
		strncpy(lpAirportSections[pAddAirportsStruct->indexPointSection].sign_left_vpp2, buff, BUFF_SIGN_VPP);
		pAddAirportsStruct->isElemSignLeftVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemSignRightVPP2){
		strncpy(lpAirportSections[pAddAirportsStruct->indexPointSection].sign_right_vpp2, buff, BUFF_SIGN_VPP);
		pAddAirportsStruct->isElemSignRightVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemNorthWgsLatVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].north_wgs_lat_vpp2 = atof(buff);
		pAddAirportsStruct->isElemNorthWgsLatVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemNorthWgsLonVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].north_wgs_lon_vpp2 = atof(buff);
		pAddAirportsStruct->isElemNorthWgsLonVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemSouthWgsLatVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].south_wgs_lat_vpp2 = atof(buff);
		pAddAirportsStruct->isElemSouthWgsLatVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemSouthWgsLonVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].south_wgs_lon_vpp2 = atof(buff);
		pAddAirportsStruct->isElemSouthWgsLonVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemWestWgsLatVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].west_wgs_lat_vpp2 = atof(buff);
		pAddAirportsStruct->isElemWestWgsLatVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemWestWgsLonVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].west_wgs_lon_vpp2 = atof(buff);
		pAddAirportsStruct->isElemWestWgsLonVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemEastWgsLatVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].east_wgs_lat_vpp2 = atof(buff);
		pAddAirportsStruct->isElemEastWgsLatVPP2 = false;
	}
	else if(pAddAirportsStruct->isElemEastWgsLonVPP2){
		lpAirportSections[pAddAirportsStruct->indexPointSection].east_wgs_lon_vpp2 = atof(buff);
		pAddAirportsStruct->isElemEastWgsLonVPP2 = false;
	}
	else
		err = 0;	

	return err;
}

int AirportsXMLWorker::endElement(const char* nameElem, void* addStruct){
	// Обработчик события выхода из XML-элемента
	int err;

	if (!nameElem || !addStruct)
	{
		ern::Log::getInstance().Write("Error, initialized the input parameters in the method: \n");
		ern::Log::getInstance().Write("int AirportsXMLWorker::endElement(const char*, void*)\n");
		return 0;
	}

	err = 1;
	LPAddAirportsStruct pAddAirportsStruct = (LPAddAirportsStruct) addStruct;
    if(!strcmp(nameElem, "airport") )
		pAddAirportsStruct->indexPointSection = pAddAirportsStruct->indexPointSection + 1;
	return err;
}

int AirportsXMLWorker::saxParser(void* point_sect, char* strXML, unsigned int strLen, void* addStruct){
	// разбирает xml-файл и возвращает результат выполнения операции
	int i, j, indexOtkrTeg, indexZakrTegStart, indexZakrTegEnd, codeErr;
	char strNameElement[40];
	memset(strNameElement, 0, 40);
	codeErr = 0;
	bool isStartElem, isEndElem;
	// Переведем все настройки структуры addStruct в начальное состояние 
	startDocument(addStruct);

	isStartElem = false;
    isEndElem = false;
	for(i=0; i<strLen; i++){
		if(strXML[i]=='<' && strXML[i+1]!='/'){ // Это значит, что вошли в StartElement
           isStartElem = true;
           indexOtkrTeg = i; 
		}else if(strXML[i]=='>'){
            if(isStartElem){ // Это означает, что выходим из StartElement
                indexZakrTegStart = i;
				int k;
				int lenStrNameElem = indexZakrTegStart - indexOtkrTeg;
				char* strNameElem = (char*) calloc (lenStrNameElem + 1, sizeof(char));
				memset(strNameElem, 0, lenStrNameElem + 1);
                for(j = indexOtkrTeg+1, k=0; j<indexZakrTegStart; j++, k++)
                    strNameElem[k] =  strXML[j];
                strNameElem[k] = '\0';
								
				for(k=0;k<strlen(strNameElem); k++)
					strNameElement[k] = strNameElem[k];

				strNameElement[k] = '\0';
				startElement((const char*)strNameElem, addStruct);

				free(strNameElem);
				isStartElem = false;
			}else if(isEndElem){  // Это значит, что выходим из EndElement
                indexZakrTegEnd = i;
                int k;
				int lenStrNameElem = indexZakrTegEnd - indexOtkrTeg;
				char* strNameElem = (char*) calloc (lenStrNameElem, sizeof(char));
				memset(strNameElem, 0, lenStrNameElem);
                for(j = indexOtkrTeg+1, k=0; j<indexZakrTegEnd; j++, k++)
                    strNameElem[k] =  strXML[j];
                strNameElem[k] = '\0';
                

				if(!strcmp((const char*)strNameElement, (const char*)strNameElem)){
                    // Это значит, что имеет текстовое содержимое
					// и нужно вызывать characters
					unsigned int len; 
					unsigned int lenStrChistContentXml;
                    int lenStrContentXml = indexOtkrTeg - 1 - indexZakrTegStart;
                    // char* strContentXml = (char*) calloc (lenStrContentXml, sizeof(char));
					char strContentXml[SIZE_CONT];
					memset(strContentXml, 0, SIZE_CONT);
					// char* strChistContentXml = 0;
					char strChistContentXml[SIZE_CONT];
					memset(strChistContentXml, 0, SIZE_CONT);
					//strChistContentXml = (char*)memset(strChistContentXml, 0, SIZE_CONT); 

                    for(j = indexZakrTegStart+1, k=0; j<indexOtkrTeg -1 && k+1 < lenStrContentXml; j++, k++)
                        strContentXml[k] =  strXML[j];
                    strContentXml[k + 1] = '\0';
				

					// Очистим контент от крайних пробелов
					lenStrChistContentXml = 0;
                    trim((const char*) strContentXml, strChistContentXml, &lenStrChistContentXml);

					len = strlen(strContentXml);
					if(len == (lenStrChistContentXml - 1)){ // таких пробелов нет
						characters(strContentXml, point_sect, addStruct);					    
					}
					else{
						//char* strChistContentXml = (char*) calloc (lenStrChistContentXml, sizeof(char));
						memset(strChistContentXml, 0, SIZE_CONT);
                        strChistContentXml[0] = '\0';

						// strChistContentXml[lenStrChistContentXml - 1] = '\0';
						trim((const char*) strContentXml, strChistContentXml, &lenStrChistContentXml);						
						characters(strChistContentXml, point_sect, addStruct);
					}				
                    

				}					

                endElement((const char*)strNameElem, addStruct);


				free(strNameElem);
				
                isEndElem = false;
			}

		}else if(strXML[i]=='<' && strXML[i+1]=='/'){ //  Это значит, что вошли EndElement
		    isEndElem = true;
            indexOtkrTeg = i + 1;
		}
	}
	 

	return codeErr;
}

//--------------------------------------------------------------------------------------------
/*<config_app>
<point_section>
  <airport_name_en>Pulkovo</airport_name_en>
	<port_main_client>0</port_main_client>
  <port_test_client_cons>54633</port_test_client_cons>
  <port_test_client_gui>54634</port_test_client_gui>
  <ip_main_client>192.168.2.209</ip_main_client>
  <ip_test_client_cons>192.168.2.209</ip_test_client_cons>
  <ip_test_client_gui>192.168.2.209</ip_test_client_gui>
  <recording_mode>0</recording_mode>
  <table_count>1</table_count>
  <record_count>1</record_count>
  <start_service>0</start_service>
</point_section>
</config_app>*/
//------------------------------------------------------------------------------------------
// здесь будет реализация методов класса ConfigAppXMLWorker
//	typedef struct
//	{
//		char airport_name_en[BUFF_NAME_AIRPORTS];
//		int port_main_client;
//		int port_test_client_cons;
//		int port_test_client_gui;
//		char ip_main_client[BUFF_IP_CLIENT];
//		char ip_test_client_cons[BUFF_IP_CLIENT];
//		char ip_test_client_gui[BUFF_IP_CLIENT];
//		char recording_mode;
//		int table_count;
//		int record_count;
//	} TAirportSection;
//--------------------------------------------------------------------------------------
// при старте документа счетчик секций обнуляется
// состояния входов в элементы принимают значения false
// в случае успеха, возвращает 1, в противном случае 0
int ConfigAppXMLWorker::startDocument(void* addStruct){
    if(!addStruct)
	{
		ern::Log::getInstance().Write("Error, the parameter is incorrect in method: \n");
		ern::Log::getInstance().Write("int ConfigAppXMLWorker::startDocument(void*)\n");
		return 0;
	}
	else
	{
		LPAddConfigAppStruct pAddConfigAppStruct = (LPAddConfigAppStruct) addStruct;
		pAddConfigAppStruct->indexPointSection = 0;
		pAddConfigAppStruct->isAirportNameEn = false;
		pAddConfigAppStruct->isPortMainClient = false;
		pAddConfigAppStruct->isPortTestClientCons = false;
		pAddConfigAppStruct->isPortTestClientGui = false;
		pAddConfigAppStruct->isIpMainClient = false;
		pAddConfigAppStruct->isIpTestClientCons = false;
		pAddConfigAppStruct->isIpTestClientGui = false;
		pAddConfigAppStruct->isRecordingMode = false;
		pAddConfigAppStruct->isTableCount = false;
		pAddConfigAppStruct->isRecordCount = false;
		pAddConfigAppStruct->isRunService = false;
		return 1;
	}	
}

// изменяет статус искомого по const char* nameElem элемента, 
// в который произошло вхождение
// в случае успешного выполнение метода возвращает 1, в противном случае 0
int ConfigAppXMLWorker::startElement(const char* nameElem, void* addStruct){
	// Обработчик события вхождения в XML-элемент
	int err;
	if (!nameElem || !addStruct)
	{
		ern::Log::getInstance().Write("Error, initialized the input parameters in the method: \n");
		ern::Log::getInstance().Write("int ConfigAppXMLWorker::startElement(const char*, void*)\n");
		return 0;
	}
	
	err = 1;
	LPAddConfigAppStruct pAddConfigAppStruct = (LPAddConfigAppStruct) addStruct;
	if(!strcmp(nameElem, "airport_name_en") )
		pAddConfigAppStruct->isAirportNameEn = true;
	else if(!strcmp(nameElem, "port_main_client"))
		pAddConfigAppStruct->isPortMainClient = true; 
	else if(!strcmp(nameElem, "port_test_client_cons"))
		pAddConfigAppStruct->isPortTestClientCons = true; 
	else if(!strcmp(nameElem, "port_test_client_gui"))
		pAddConfigAppStruct->isPortTestClientGui = true;		 
	else if(!strcmp(nameElem, "ip_main_client"))
		pAddConfigAppStruct->isIpMainClient = true;
	else if(!strcmp(nameElem, "ip_test_client_cons"))
		pAddConfigAppStruct->isIpTestClientCons = true;
	else if(!strcmp(nameElem, "ip_test_client_gui"))
		pAddConfigAppStruct->isIpTestClientGui = true;
	else if(!strcmp(nameElem, "recording_mode"))
		pAddConfigAppStruct->isRecordingMode = true;
	else if(!strcmp(nameElem, "table_count"))
		pAddConfigAppStruct->isTableCount = true;
	else if(!strcmp(nameElem, "record_count"))
		pAddConfigAppStruct->isRecordCount = true;
	else if(!strcmp(nameElem, "run_service"))
		pAddConfigAppStruct->isRunService = true;
	/*else
	{
		char msgErr[80];
		sprintf(msgErr, "Error, XML-element %s not found in the method: \n", nameElem);
		ern::Log::getInstance().Write(msgErr);
		ern::Log::getInstance().Write("int ConfigAppXMLWorker::startElement(const char*, void*)\n");
		return 0;
	}*/
	return err;
}

// инициализирует найденный xml-элемент соответствующим значением
// в случае ошибки возвращает 0, в противном случае 1
int ConfigAppXMLWorker::characters(const char* buff, void* point_section, void* addStruct){
	int err;
	if (!buff || !point_section || !addStruct)
	{
		ern::Log::getInstance().Write("Error, initialized the input parameters in the method: \n");
		ern::Log::getInstance().Write("int ConfigAppXMLWorker::characters(const char*, void*, void*)\n");
		return 0;
	}
	err = 1;

	LPAddConfigAppStruct pAddConfigAppStruct = (LPAddConfigAppStruct) addStruct;
    LPConfAppSection lpConfAppSections = (LPConfAppSection)point_section;

	if(pAddConfigAppStruct->isAirportNameEn){
		strncpy(lpConfAppSections[pAddConfigAppStruct->indexPointSection].airport_name_en, buff, BUFF_NAME_AIRPORTS);
		pAddConfigAppStruct->isAirportNameEn = false;
	}
	else if(pAddConfigAppStruct->isPortMainClient){
		lpConfAppSections[pAddConfigAppStruct->indexPointSection].port_main_client = atoi(buff);
		pAddConfigAppStruct->isPortMainClient = false;
	}
	else if(pAddConfigAppStruct->isPortTestClientCons){
		lpConfAppSections[pAddConfigAppStruct->indexPointSection].port_test_client_cons = atoi(buff);
		pAddConfigAppStruct->isPortTestClientCons = false;
	}
	else if(pAddConfigAppStruct->isPortTestClientGui){
		lpConfAppSections[pAddConfigAppStruct->indexPointSection].port_test_client_gui = atoi(buff);
		pAddConfigAppStruct->isPortTestClientGui = false;
	}
	else if(pAddConfigAppStruct->isIpMainClient){
		strncpy(lpConfAppSections[pAddConfigAppStruct->indexPointSection].ip_main_client, buff, BUFF_IP_CLIENT);
		pAddConfigAppStruct->isIpMainClient = false;
	}
	else if(pAddConfigAppStruct->isIpTestClientCons){
		strncpy(lpConfAppSections[pAddConfigAppStruct->indexPointSection].ip_test_client_cons, buff, BUFF_IP_CLIENT);
		pAddConfigAppStruct->isIpTestClientCons = false;
	}
	else if(pAddConfigAppStruct->isIpTestClientGui){
		strncpy(lpConfAppSections[pAddConfigAppStruct->indexPointSection].ip_test_client_gui, buff, BUFF_IP_CLIENT);
		pAddConfigAppStruct->isIpTestClientGui = false;
	}
	else if(pAddConfigAppStruct->isRecordingMode){
		lpConfAppSections[pAddConfigAppStruct->indexPointSection].recording_mode = atoi(buff);
		pAddConfigAppStruct->isRecordingMode = false;
	}
	else if(pAddConfigAppStruct->isTableCount){
		lpConfAppSections[pAddConfigAppStruct->indexPointSection].table_count = atoi(buff);
		pAddConfigAppStruct->isTableCount = false;
	}
	else if(pAddConfigAppStruct->isRecordCount){
		lpConfAppSections[pAddConfigAppStruct->indexPointSection].record_count = atoi(buff);
		pAddConfigAppStruct->isRecordCount = false;
	}
	else if(pAddConfigAppStruct->isRunService){
		lpConfAppSections[pAddConfigAppStruct->indexPointSection].run_service = atoi(buff);
		pAddConfigAppStruct->isRunService = false;
	}
	else
		err = 0;	

	return err;
}

int ConfigAppXMLWorker::endElement(const char* nameElem, void* addStruct){
	// Обработчик события выхода из XML-элемента
	int err;

	if (!nameElem || !addStruct)
	{
		ern::Log::getInstance().Write("Error, initialized the input parameters in the method: \n");
		ern::Log::getInstance().Write("int ConfigAppXMLWorker::endElement(const char*, void*)\n");
		return 0;
	}

	err = 1;
	LPAddConfigAppStruct pAddConfigAppStruct = (LPAddConfigAppStruct) addStruct;
    if(!strcmp(nameElem, "point_section") )
		pAddConfigAppStruct->indexPointSection = pAddConfigAppStruct->indexPointSection + 1;
	return err;
}

int ConfigAppXMLWorker::saxParser(void* point_sect, char* strXML, unsigned int strLen, void* addStruct){
	// разбирает xml-файл и возвращает результат выполнения операции
	int i, j, indexOtkrTeg, indexZakrTegStart, indexZakrTegEnd, codeErr;
	char strNameElement[40];
	memset(strNameElement, 0, 40);
	codeErr = 0;
	bool isStartElem, isEndElem;
	// Переведем все настройки структуры addStruct в начальное состояние 
	startDocument(addStruct);

	isStartElem = false;
    isEndElem = false;
	for(i=0; i<strLen; i++){
		if(strXML[i]=='<' && strXML[i+1]!='/'){ // Это значит, что вошли в StartElement
           isStartElem = true;
           indexOtkrTeg = i; 
		}else if(strXML[i]=='>'){
            if(isStartElem){ // Это означает, что выходим из StartElement
                indexZakrTegStart = i;
				int k;
				int lenStrNameElem = indexZakrTegStart - indexOtkrTeg;
				char* strNameElem = (char*) calloc (lenStrNameElem + 1, sizeof(char));
				memset(strNameElem, 0, lenStrNameElem + 1);
                for(j = indexOtkrTeg+1, k=0; j<indexZakrTegStart; j++, k++)
                    strNameElem[k] =  strXML[j];
                strNameElem[k] = '\0';
								
				for(k=0;k<strlen(strNameElem); k++)
					strNameElement[k] = strNameElem[k];

				strNameElement[k] = '\0';
				startElement((const char*)strNameElem, addStruct);

				free(strNameElem);
				isStartElem = false;
			}else if(isEndElem){  // Это значит, что выходим из EndElement
                indexZakrTegEnd = i;
                int k;
				int lenStrNameElem = indexZakrTegEnd - indexOtkrTeg;
				char* strNameElem = (char*) calloc (lenStrNameElem, sizeof(char));
				memset(strNameElem, 0, lenStrNameElem);
                for(j = indexOtkrTeg+1, k=0; j<indexZakrTegEnd; j++, k++)
                    strNameElem[k] =  strXML[j];
                strNameElem[k] = '\0';
                

				if(!strcmp((const char*)strNameElement, (const char*)strNameElem)){
                    // Это значит, что имеет текстовое содержимое
					// и нужно вызывать characters
					unsigned int len; 
					unsigned int lenStrChistContentXml;
                    int lenStrContentXml = indexOtkrTeg - 1 - indexZakrTegStart;
                    // char* strContentXml = (char*) calloc (lenStrContentXml, sizeof(char));
					char strContentXml[SIZE_CONT];
					memset(strContentXml, 0, SIZE_CONT);
					// char* strChistContentXml = 0;
					char strChistContentXml[SIZE_CONT];
					memset(strChistContentXml, 0, SIZE_CONT);
					//strChistContentXml = (char*)memset(strChistContentXml, 0, SIZE_CONT); 

                    for(j = indexZakrTegStart+1, k=0; j<indexOtkrTeg -1 && k+1 < lenStrContentXml; j++, k++)
                        strContentXml[k] =  strXML[j];
                    strContentXml[k + 1] = '\0';
				

					// Очистим контент от крайних пробелов
					lenStrChistContentXml = 0;
                    trim((const char*) strContentXml, strChistContentXml, &lenStrChistContentXml);

					len = strlen(strContentXml);
					if(len == (lenStrChistContentXml - 1)){ // таких пробелов нет
						characters(strContentXml, point_sect, addStruct);					    
					}
					else{
						//char* strChistContentXml = (char*) calloc (lenStrChistContentXml, sizeof(char));
						memset(strChistContentXml, 0, SIZE_CONT);
                        strChistContentXml[0] = '\0';						
						// strChistContentXml[lenStrChistContentXml - 1] = '\0';
						trim((const char*) strContentXml, strChistContentXml, &lenStrChistContentXml);						
						characters(strChistContentXml, point_sect, addStruct);
					}				
                    

				}					

                endElement((const char*)strNameElem, addStruct);


				free(strNameElem);
				
                isEndElem = false;
			}

		}else if(strXML[i]=='<' && strXML[i+1]=='/'){ //  Это значит, что вошли EndElement
		    isEndElem = true;
            indexOtkrTeg = i + 1;
		}
	}
	 

	return codeErr;
}

}
//int main(){
//	int ch;
//	unsigned int dwBufferLen;
//	char* strXML;
//    sax_pars_xml::LPointSections point_sections;
//	sax_pars_xml::LPAddStruct pAddStruct;
//	// Определим размер xml - строки
//    FILE* xml;
//    xml = fopen("test.xml", "r");
//	
//	ch = getc(xml);
//	dwBufferLen = 0;
//	
//	while(ch != EOF){
//        ch = getc(xml);
//	    dwBufferLen ++;	    
//	}
//	fclose(xml);
//    strXML = (char*) calloc (dwBufferLen, sizeof(char));
//        
//    // теперь заполним ее из файла
//    xml = fopen("test.xml", "r");
//    ch = getc(xml);
//	dwBufferLen = 0;
//	strXML[dwBufferLen] = (char)ch;
//
//	while(ch != EOF){
//        ch = getc(xml);
//	    dwBufferLen ++;
//	    strXML[dwBufferLen] = (char)ch;
//	}
//	strXML[dwBufferLen] = '\0';
//    fclose(xml);
//
//	// Разберем данный xml
//	point_sections = (sax_pars_xml::TPointSection*) calloc(2, sizeof(sax_pars_xml::TPointSection));
//	pAddStruct     = (sax_pars_xml::TAddStruct*) calloc(1, sizeof(sax_pars_xml::TAddStruct));
//
//    saxParser(point_sections, strXML, dwBufferLen, pAddStruct);
//
//	FILE* result;
//	result = fopen("result.txt", "wt");
//	fprintf(result, "In first section: start_section = %4.4f\n", point_sections[0].start_section);
//	fprintf(result, "In first section: finish_section = %4.4f\n", point_sections[0].finish_section);
//	fprintf(result, "In second section: start_section = %4.4f\n", point_sections[1].start_section);
//	fprintf(result, "In second section: finish_section = %4.4f\n", point_sections[1].finish_section);
//    fclose(result);
//
//	free(point_sections);
//	free(pAddStruct);
//	free(strXML);
//
//   return 0;
//}

#endif

