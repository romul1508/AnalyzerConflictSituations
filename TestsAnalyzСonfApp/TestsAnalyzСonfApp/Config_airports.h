/** author-developer: Roman Ermakov */
#ifndef _CONFIG_AIRPORTS_H_
#define _CONFIG_AIRPORTS_H_

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include "Singleton.h"
#include "SAXparser.h"

// using namespace std;

namespace ern{
#define BUFF_CONFIG_AIRPORTS_INFO	200
/** Конфигуратор аэропортов создан с использованием улучшенной версии синглетона Мейерса
	Однако следует учесть, что если будут созданы еще классы одиночки,
	например, для работы с БД, то такая реализация может привести к
	проблеме висячих ссылок и для таких случаев потребуется другая реализация,
	с использованием библиотеки Loki.
	Более того, синглетон Мейерса является крайне неподходящим для приложений,
	работающих в многопоточном режиме.
*/

class ConfiguratorAirports;					// опережающее объявление

//--------------------------------

// template <typename T> T* Singleton<T>::singletonInstance;
// template <typename T> SingletonsDestroyer Singleton<T>::destroyer;
// Log * Singleton<Log>::singletonInstance = 0;
// SingletonsDestroyer Singleton<Log>::destroyer;

// template <typename T>
class  ConfiguratorAirportsDestroyer : public SingletonDestroyer<ConfiguratorAirports>
{
  protected:
		friend class SingletonDestroyer<ConfiguratorAirports>;
		// virtual ~LoggerDestroyer() {  }
  public:
	  ConfiguratorAirportsDestroyer() : SingletonDestroyer<ConfiguratorAirports>() { }    
};
//---------------------------------------
// template <typename T> T* Singleton<T>::singletonInstance;
// ConfiguratorAirports * Singleton<ConfiguratorAirports>::singletonInstance = 0;
#ifdef __linux__
    //linux code goes here
    // template <typename T> T* Singleton<T>::singletonInstance;
#elif _WIN32
    // windows code goes here
   ConfiguratorAirports * Singleton<ConfiguratorAirports>::singletonInstance = 0;
#else
// #error "OS not supported!"
#endif
//----------------------------------------
 class ConfiguratorAirports : public Singleton<ConfiguratorAirports>
 {
	private:
		 static ConfiguratorAirportsDestroyer conf_airports_destroyer;


		 ConfiguratorAirports() : Singleton<ConfiguratorAirports>()
		 {
			 conf_airports_destroyer.initialize(this);
		 }		 	 
		 
	 protected:
			char name_section[BUFF_NAME_AIRPORTS];	
			double height_cta;
			double lat_cta;
			double lon_cta;
			int mag_decl_cta;
			char caption_vpp1[BUFF_CAPTION_VPP];
			int id_vpp1;
			char cat_vpp1;
			double angle_vpp1;
			double runway_lat_vpp1;
			double runway_lon_vpp1;
			int lenght_vpp1;
			int width_vpp1;
			char sign_left_vpp1[BUFF_SIGN_VPP];
			char sign_right_vpp1[BUFF_SIGN_VPP];

			double north_wgs_lat_vpp1;
			double north_wgs_lon_vpp1; 
			double south_wgs_lat_vpp1;
			double south_wgs_lon_vpp1;
			double west_wgs_lat_vpp1;
			double west_wgs_lon_vpp1;
			double east_wgs_lat_vpp1;
			double east_wgs_lon_vpp1;

			char caption_vpp2[BUFF_CAPTION_VPP];
			int id_vpp2;
			char cat_vpp2;
			double angle_vpp2;
			double runway_lat_vpp2;
			double runway_lon_vpp2;
			int lenght_vpp2;
			int width_vpp2;
			char sign_left_vpp2[BUFF_SIGN_VPP];
			char sign_right_vpp2[BUFF_SIGN_VPP];

			double north_wgs_lat_vpp2;
			double north_wgs_lon_vpp2; 
			double south_wgs_lat_vpp2;
			double south_wgs_lon_vpp2;
			double west_wgs_lat_vpp2;
			double west_wgs_lon_vpp2;
			double east_wgs_lat_vpp2;
			double east_wgs_lon_vpp2;

			friend class Singleton<ConfiguratorAirports>;
			 
	 public:
			// загружает данные в конфигуратор приложения из xml-файла
			void LoadXML(const char* name_file) 
		 {
				int ch;
				unsigned int dwBufferLen;
				char* strXML;
				
				sax_pars_xml::LPAirportSection pAirportSection;
				sax_pars_xml::LPAddAirportsStruct pAddAirportsStruct;

				sax_pars_xml::AirportsXMLWorker* pAirportsXMLWorker;
				//--------------------
				// Определим размер xml - строки
				FILE* xml;
				xml = fopen(name_file, "r");
	
				ch = getc(xml);
				dwBufferLen = 0;
	
				while(ch != EOF){
					ch = getc(xml);
					dwBufferLen ++;	    
				}
				fclose(xml);
				//--------------------------
				strXML = (char*) calloc (dwBufferLen+1, sizeof(char));
        
				 // теперь заполним ее из файла
				xml = fopen(name_file, "r");
				ch = getc(xml);
				dwBufferLen = 0;
				strXML[dwBufferLen] = (char)ch;

				while(ch != EOF){
					ch = getc(xml);
					dwBufferLen ++;
					strXML[dwBufferLen] = (char)ch;
				}
				strXML[dwBufferLen] = '\0';
				fclose(xml);

				// Разберем данный xml
				pAirportSection = (sax_pars_xml::TAirportSection*)calloc(1, sizeof(sax_pars_xml::TAirportSection));
				pAddAirportsStruct = (sax_pars_xml::TAddAirportsStruct*)calloc(1, sizeof(sax_pars_xml::TAddAirportsStruct));
				// pAirportsXMLWorker = (sax_pars_xml::AirportsXMLWorker*)calloc(1, sizeof(sax_pars_xml::AirportsXMLWorker));
				pAirportsXMLWorker = new sax_pars_xml::AirportsXMLWorker();
				pAirportsXMLWorker->saxParser(pAirportSection, strXML, dwBufferLen, pAddAirportsStruct);
				
				// теперь можно перегрузить данные загруженные из XML в соответствующие поля класса ConfiguratorAirports
				strncpy(name_section, pAirportSection[0].name_section, BUFF_NAME_AIRPORTS);
				height_cta = pAirportSection[0].height_cta;
				lat_cta = pAirportSection[0].lat_cta;
				lon_cta = pAirportSection[0].lon_cta;
				mag_decl_cta = pAirportSection[0].mag_decl_cta;
				memset(caption_vpp1, 0, BUFF_CAPTION_VPP);
				strncpy(caption_vpp1, pAirportSection[0].caption_vpp1, BUFF_CAPTION_VPP);
				id_vpp1 = pAirportSection[0].id_vpp1;
				cat_vpp1 = pAirportSection[0].cat_vpp1;
				angle_vpp1 = pAirportSection[0].angle_vpp1;
				runway_lat_vpp1 = pAirportSection[0].runway_lat_vpp1;
				runway_lon_vpp1 = pAirportSection[0].runway_lon_vpp1;
				lenght_vpp1 = pAirportSection[0].lenght_vpp1;
				width_vpp1 = pAirportSection[0].width_vpp1;
				memset(sign_left_vpp1, 0, BUFF_SIGN_VPP);
				strncpy(sign_left_vpp1, pAirportSection[0].sign_left_vpp1, BUFF_SIGN_VPP);
				strncpy(sign_right_vpp1, pAirportSection[0].sign_right_vpp1, BUFF_SIGN_VPP);
				north_wgs_lat_vpp1 = pAirportSection[0].north_wgs_lat_vpp1;
				north_wgs_lon_vpp1 = pAirportSection[0].north_wgs_lon_vpp1;
				south_wgs_lat_vpp1 = pAirportSection[0].south_wgs_lat_vpp1;
				south_wgs_lon_vpp1 = pAirportSection[0].south_wgs_lon_vpp1;
				west_wgs_lat_vpp1 = pAirportSection[0].west_wgs_lat_vpp1;
				west_wgs_lon_vpp1 = pAirportSection[0].west_wgs_lon_vpp1;
				east_wgs_lat_vpp1 = pAirportSection[0].east_wgs_lat_vpp1;
				east_wgs_lon_vpp1 = pAirportSection[0].east_wgs_lon_vpp1;

				strncpy(caption_vpp2, pAirportSection[0].caption_vpp2, BUFF_CAPTION_VPP);
				id_vpp2 = pAirportSection[0].id_vpp2;
				cat_vpp2 = pAirportSection[0].cat_vpp2;
				angle_vpp2 = pAirportSection[0].angle_vpp2;
				runway_lat_vpp2 = pAirportSection[0].runway_lat_vpp2;
				runway_lon_vpp2 = pAirportSection[0].runway_lon_vpp2;
				lenght_vpp2 = pAirportSection[0].lenght_vpp2;
				width_vpp2 = pAirportSection[0].width_vpp2;
				memset(sign_left_vpp2, 0, BUFF_SIGN_VPP);
				strncpy(sign_left_vpp2, pAirportSection[0].sign_left_vpp2, BUFF_SIGN_VPP);
				memset(sign_right_vpp2, 0, BUFF_SIGN_VPP);
				strncpy(sign_right_vpp2, pAirportSection[0].sign_right_vpp2, BUFF_SIGN_VPP);
				north_wgs_lat_vpp2 = pAirportSection[0].north_wgs_lat_vpp2;
				north_wgs_lon_vpp2 = pAirportSection[0].north_wgs_lon_vpp2;
				south_wgs_lat_vpp2 = pAirportSection[0].south_wgs_lat_vpp2;
				south_wgs_lon_vpp2 = pAirportSection[0].south_wgs_lon_vpp2;
				west_wgs_lat_vpp2 = pAirportSection[0].west_wgs_lat_vpp2;
				west_wgs_lon_vpp2 = pAirportSection[0].west_wgs_lon_vpp2;
				east_wgs_lat_vpp2 = pAirportSection[0].east_wgs_lat_vpp2;
				east_wgs_lon_vpp2 = pAirportSection[0].east_wgs_lon_vpp2;
				//----------------------------
				// free(pAirportsXMLWorker);
				delete pAirportsXMLWorker;
				free(pAirportSection);
				free(pAddAirportsStruct);
				free(strXML);
		 }
		     
			// сохраняет обновленный конфигуратор приложения в xml-файле
			void UpdateXML(const char* name_file)
			 {
				std::string str_xml = "<airports>\n\t<airport>\n\t\t<name>";
				str_xml += name_section;
				str_xml += "</name>\n\t\t<height_cta>";
				str_xml += height_cta;
				str_xml += "</height_cta>\n\t\t<lat_cta>";
				str_xml += lat_cta;
				str_xml += "</lat_cta>\n\t\t<lon_cta>";
				str_xml += lon_cta;
				str_xml += "</lon_cta>\n\t\t<mag_decl_cta>";
				str_xml += mag_decl_cta;
				str_xml += "</mag_decl_cta>\n\t\t<caption_vpp1>";
				str_xml += caption_vpp1;
				str_xml += "</caption_vpp1>\n\t\t<id_vpp1>";
				str_xml += id_vpp1;
				str_xml += "</id_vpp1>\n\t\t<cat_vpp1>";
				str_xml += cat_vpp1;
				str_xml += "</cat_vpp1>\n\t\t<angle_vpp1>";
				str_xml += angle_vpp1;
				str_xml += "</angle_vpp1>\n\t\t<runway_lat_vpp1>";
				str_xml += runway_lat_vpp1;
				str_xml += "</runway_lat_vpp1>\n\t\t<runway_lon_vpp1>";
				str_xml += runway_lon_vpp1;
				str_xml += "</runway_lon_vpp1>\n\t\t<lenght_vpp1>";
				str_xml += lenght_vpp1;
				str_xml += "</lenght_vpp1>\n\t\t<width_vpp1>";
				str_xml += width_vpp1;
				str_xml += "</width_vpp1>\n\t\t<sign_left_vpp1>";
				str_xml += sign_left_vpp1;
				str_xml += "</sign_left_vpp1>\n\t\t<sign_right_vpp1>";
				str_xml += sign_right_vpp1;
				str_xml += "</sign_right_vpp1>\n\t\t<north_wgs_lat_vpp1>";
				str_xml += north_wgs_lat_vpp1;
				str_xml += "</north_wgs_lat_vpp1>\n\t\t<north_wgs_lon_vpp1>";
				str_xml += north_wgs_lon_vpp1;
				str_xml += "</north_wgs_lon_vpp1>\n\t\t<south_wgs_lat_vpp1>";
				str_xml += south_wgs_lat_vpp1;
				str_xml += "</south_wgs_lat_vpp1>\n\t\t<south_wgs_lon_vpp1>";
				str_xml += south_wgs_lon_vpp1;
				str_xml += "</south_wgs_lon_vpp1>\n\t\t<west_wgs_lat_vpp1>";
				str_xml += west_wgs_lat_vpp1;
				str_xml += "</west_wgs_lat_vpp1>\n\t\t<west_wgs_lon_vpp1>";
				str_xml += west_wgs_lon_vpp1;
				str_xml += "</west_wgs_lon_vpp1>\n\t\t<east_wgs_lat_vpp1>";
				str_xml += east_wgs_lat_vpp1;
				str_xml += "</east_wgs_lat_vpp1>\n\t\t<east_wgs_lon_vpp1>";
				str_xml += east_wgs_lon_vpp1;
				str_xml += "</east_wgs_lon_vpp1>\n\t\t<caption_vpp2>";
				str_xml += caption_vpp2;
				str_xml += "</caption_vpp2>\n\t\t<id_vpp2>";
				str_xml += id_vpp2;
				str_xml += "</id_vpp2>\n\t\t<cat_vpp2>";
				str_xml += cat_vpp2;
				str_xml += "</cat_vpp2>\n\t\t<angle_vpp2>";
				str_xml += angle_vpp2;
				str_xml += "</angle_vpp2>\n\t\t<runway_lat_vpp2>";
				str_xml += runway_lat_vpp2;
				str_xml += "</runway_lat_vpp2>\n\t\t<runway_lon_vpp2>";
				str_xml += runway_lon_vpp2;
				str_xml += "</runway_lon_vpp2>\n\t\t<lenght_vpp2>";
				str_xml += lenght_vpp2;
				str_xml += "</lenght_vpp2>\n\t\t<width_vpp2>";
				str_xml += width_vpp2;
				str_xml += "</width_vpp2>\n\t\t<sign_left_vpp2>";
				str_xml += sign_left_vpp2;
				str_xml += "</sign_left_vpp2>\n\t\t<sign_right_vpp2>";
				str_xml += sign_right_vpp2;
				str_xml += "</sign_right_vpp2>\n\t\t<north_wgs_lat_vpp2>";
				str_xml += north_wgs_lat_vpp2;
				str_xml += "</north_wgs_lat_vpp2>\n\t\t<north_wgs_lon_vpp2>";
				str_xml += north_wgs_lon_vpp2;
				str_xml += "</north_wgs_lon_vpp2>\n\t\t<south_wgs_lat_vpp2>";
				str_xml += south_wgs_lat_vpp2;
				str_xml += "</south_wgs_lat_vpp2>\n\t\t<south_wgs_lon_vpp2>";
				str_xml += south_wgs_lon_vpp2;
				str_xml += "</south_wgs_lon_vpp2>\n\t\t<west_wgs_lat_vpp2>";
				str_xml += west_wgs_lat_vpp2;
				str_xml += "</west_wgs_lat_vpp2>\n\t\t<west_wgs_lon_vpp2>";
				str_xml += west_wgs_lon_vpp2;
				str_xml += "</west_wgs_lon_vpp2>\n\t\t<east_wgs_lat_vpp2>";
				str_xml += east_wgs_lat_vpp2;
				str_xml += "</east_wgs_lat_vpp2>\n\t\t<east_wgs_lon_vpp2>";
				str_xml += east_wgs_lon_vpp2;
				str_xml += "</east_wgs_lon_vpp2>\n\t</airport>\n</airports>\n";

				FILE* pConfigAppXML;
				pConfigAppXML = fopen(name_file, "wt");
				fprintf(pConfigAppXML, str_xml.c_str());				
				fclose(pConfigAppXML);
			 }

			// на вход метода подается указатель на строку к заполнению, 
			// второй параметр - ее максимальный размер
			// метод возвращает количество записанных символов в строку
			int getNameSection(char* strNameSection, int size_str)
			{
				if(size_str > BUFF_NAME_AIRPORTS)
					strncpy(strNameSection, name_section, BUFF_NAME_AIRPORTS);
				else
					strncpy(strNameSection, name_section, size_str);
				return	strlen(strNameSection);
			}
			//------------------------
			double getHeightCTA() const
			{
				return this->height_cta;
			}
			//-----------------------
			double getLatCTA() const
			{
				return this->lat_cta;
			}
			//-----------------------
			double getLonCTA() const
			{
				return this->lon_cta;
			}
			//-----------------------
			int getMagDeclCTA() const
			{
				return this->mag_decl_cta;
			}
			//------------------------
			int getCaptionVPP1(char* strCaptionVPP1, int size_str)
			{
				if(size_str > BUFF_CAPTION_VPP)
					strncpy(strCaptionVPP1, caption_vpp1, BUFF_CAPTION_VPP);
				else
					strncpy(strCaptionVPP1, caption_vpp1, size_str);
				return	strlen(strCaptionVPP1);
			}
			//-------------------------
			int getIdVpp1() const
			{
				return this->id_vpp1;
			}
			//-------------------------
			char getCatVPP1() const
			{
				return this->cat_vpp1;
			}
			//----------------------------
			double getAngleVPP1() const
			{
				return this->angle_vpp1;
			}
			//---------------------------
			double getRunwayLatVPP1() const
			{
				return this->runway_lat_vpp1;
			}
			//----------------------------
			double getRunwayLonVPP1() const
			{
				return this->runway_lon_vpp1;
			}
			//---------------------------
			int getLenghtVPP1() const
			{
				return this->lenght_vpp1;
			}
			//---------------------------
			int getWidthVPP1() const
			{
				return this->width_vpp1;
			}
			//----------------------------
			int getSignLeftVPP1(char* strSignLeftVPP1, int size_str)
			{
				if(size_str > BUFF_SIGN_VPP)
					strncpy(strSignLeftVPP1, sign_left_vpp1, BUFF_SIGN_VPP);
				else
					strncpy(strSignLeftVPP1, sign_left_vpp1, size_str);
				return	strlen(strSignLeftVPP1);
			}
			//--------------------------------
			int getSignRightVPP1(char* strSignRightVPP1, int size_str)
			{
				if(size_str > BUFF_SIGN_VPP)
					strncpy(strSignRightVPP1, sign_right_vpp1, BUFF_SIGN_VPP);
				else
					strncpy(strSignRightVPP1, sign_right_vpp1, size_str);
				return	strlen(strSignRightVPP1);
			}
			//------------------------------
			double getNorthWgsLatVPP1() const
			{
				return this->north_wgs_lat_vpp1;
			}
			//------------------------------
			double getNorthWgsLonVPP1() const
			{
				return this->north_wgs_lon_vpp1;
			}
			//-----------------------------
			double getSouthWgsLatVPP1() const
			{
				return this->south_wgs_lat_vpp1;
			}
			//-----------------------------
			double getSouthWgsLonVPP1() const
			{
				return this->south_wgs_lon_vpp1;
			}
			//------------------------------
			double getWestWgsLatVPP1() const
			{
				return this->west_wgs_lat_vpp1;
			}
			//------------------------------
			double getWestWgsLonVPP1() const
			{
				return this->west_wgs_lon_vpp1;
			}
			//------------------------------
			double getEastWgsLatVPP1() const
			{
				return this->east_wgs_lat_vpp1;
			}
			//-----------------------------
			double getEastWgsLonVPP1() const
			{
				return this->east_wgs_lon_vpp1;
			}
			//------------------------------
			int getCaptionVPP2(char* strCaptionVPP2, int size_str)
			{
				if(size_str > BUFF_CAPTION_VPP)
					strncpy(strCaptionVPP2, caption_vpp2, BUFF_CAPTION_VPP);
				else
					strncpy(strCaptionVPP2, caption_vpp2, size_str);
				return	strlen(strCaptionVPP2);
			}
			//-----------------------------
			int getIdVPP2() const
			{
				return this->id_vpp2;
			}
			//----------------------------
			char getCatVPP2() const
			{
				return this->cat_vpp2;
			}
			//---------------------------
			double getAngleVPP2() const
			{
				return this->angle_vpp2;
			}
			//---------------------------
			double getRunwayLatVPP2() const
			{
				return this->runway_lat_vpp2;
			}
			//---------------------------
			double getRunwayLonVPP2() const
			{
				return this->runway_lon_vpp2;
			}
			//---------------------------
			int getLenghtVPP2() const
			{
				return this->lenght_vpp2;
			}
			//---------------------------
			int getWidthVPP2() const
			{
				return this->width_vpp2;
			}
			//---------------------------
			int getSignLeftVPP2(char* strSignLeftVPP2, int size_str)
			{
				if(size_str > BUFF_SIGN_VPP)
					strncpy(strSignLeftVPP2, sign_left_vpp2, BUFF_SIGN_VPP);
				else
					strncpy(strSignLeftVPP2, sign_left_vpp2, size_str);
				return	strlen(strSignLeftVPP2);
			}
			//---------------------------
			int getSignRightVPP2(char* strSignRightVPP2, int size_str)
			{
				if(size_str > BUFF_SIGN_VPP)
					strncpy(strSignRightVPP2, sign_right_vpp2, BUFF_SIGN_VPP);
				else
					strncpy(strSignRightVPP2, sign_right_vpp2, size_str);
				return	strlen(strSignRightVPP2);
			}
			//---------------------------
			double getNorthWgsLatVPP2() const
			{
				return this->north_wgs_lat_vpp2;
			}
			//------------------------------
			double getNorthWgsLonVPP2() const
			{
				return this->north_wgs_lon_vpp2;
			}
			//-----------------------------
			double getSouthWgsLatVPP2() const
			{
				return this->south_wgs_lat_vpp2;
			}
			//-----------------------------
			double getSouthWgsLonVPP2() const
			{
				return this->south_wgs_lon_vpp2;
			}
			//------------------------------
			double getWestWgsLatVPP2() const
			{
				return this->west_wgs_lat_vpp2;
			}
			//------------------------------
			double getWestWgsLonVPP2() const
			{
				return this->west_wgs_lon_vpp2;
			}
			//------------------------------
			double getEastWgsLatVPP2() const
			{
				return this->east_wgs_lat_vpp2;
			}
			//-----------------------------
			double getEastWgsLonVPP2() const
			{
				return this->east_wgs_lon_vpp2;
			}
			//-----------------------------
 };

 // ConfiguratorAirportsDestroyer ConfiguratorAirports::conf_airports_destroyer;
 
}

#endif /* _CONFIG_AIRPORTS_H_ */