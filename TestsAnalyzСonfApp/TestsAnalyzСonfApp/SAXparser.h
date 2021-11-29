/** author-developer: Roman Ermakov */
#ifndef _SAX_PARS_
#define _SAX_PARS_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Log.h"
namespace sax_pars_xml{
	/*typedef struct
	{
		double start_section;	
		double finish_section;	
	} TPointSection;*/

	/*typedef struct
	{
		int indexPointSection;	
		bool isElemStartSection, isElemFinishSection;	
	} TAddStruct;*/

	#define BUFF_NAME_AIRPORTS		25
	#define BUFF_CAPTION_VPP			20
	#define BUFF_SIGN_VPP					10
	//---------------------------------------
	#define	BUFF_IP_CLIENT				16

	typedef struct
	{
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
	} TAirportSection;

	typedef struct
	{
		int indexPointSection;	
		bool isElemNameAirport;
		bool isElemHeightCTA;
		bool isElemLatCTA;
		bool isElemLonCTA;
		bool isElemMagDeclCTA;
		bool isElemCaptionVPP1;
		bool isElemIdVPP1;
		bool isElemCatVPP1;
		bool isElemAngleVPP1;
		bool isElemRunwayLatVPP1;
		bool isElemRunwayLonVPP1;
		bool isElemLenghtVPP1;
		bool isElemWidthVPP1;
		bool isElemSignLeftVPP1;
		bool isElemSignRightVPP1;

		bool isElemNorthWgsLatVPP1;
		bool isElemNorthWgsLonVPP1; 
		bool isElemSouthWgsLatVPP1;
		bool isElemSouthWgsLonVPP1;
		bool isElemWestWgsLatVPP1;
		bool isElemWestWgsLonVPP1;
		bool isElemEastWgsLatVPP1;
		bool isElemEastWgsLonVPP1;

		bool isElemCaptionVPP2;
		bool isElemIdVPP2;
		bool isElemCatVPP2;
		bool isElemAngleVPP2;
		bool isElemRunwayLatVPP2;
		bool isElemRunwayLonVPP2;
		bool isElemLenghtVPP2;
		bool isElemWidthVPP2;
		bool isElemSignLeftVPP2;
		bool isElemSignRightVPP2;

		bool isElemNorthWgsLatVPP2;
		bool isElemNorthWgsLonVPP2; 
		bool isElemSouthWgsLatVPP2;
		bool isElemSouthWgsLonVPP2;
		bool isElemWestWgsLatVPP2;
		bool isElemWestWgsLonVPP2;
		bool isElemEastWgsLatVPP2;
		bool isElemEastWgsLonVPP2;
	} TAddAirportsStruct;
	//----------------------------------
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

	typedef struct
	{
		char airport_name_en[BUFF_NAME_AIRPORTS];
		int port_main_client;
		int port_test_client_cons;
		int port_test_client_gui;
		char ip_main_client[BUFF_IP_CLIENT];
		char ip_test_client_cons[BUFF_IP_CLIENT];
		char ip_test_client_gui[BUFF_IP_CLIENT];
		char recording_mode;
		int table_count;
		int record_count;
		int run_service;
	} TConfAppSection;
	//----------------------------------
	typedef struct
	{
		int indexPointSection;	
		bool isAirportNameEn;
		bool isPortMainClient;
		bool isPortTestClientCons;
		bool isPortTestClientGui;
		bool isIpMainClient;
		bool isIpTestClientCons;
		bool isIpTestClientGui;
		bool isRecordingMode;
		bool isTableCount;
		bool isRecordCount;
		bool isRunService;
	} TAddConfigAppStruct;
	//---------------------------------

	#define SIZE_CONT 25

	// typedef TPointSection*	LPointSections;
	// typedef TAddStruct*     LPAddStruct;
	typedef TAddAirportsStruct* LPAddAirportsStruct;
	typedef TAirportSection*		LPAirportSection;
	typedef TAddConfigAppStruct* LPAddConfigAppStruct;
	typedef TConfAppSection*	LPConfAppSection;
	
	class SaxParsXMLWorker
	{
	public:
		void trim(const char* strContentXMLElem, char* strChistContentXMLElem, unsigned int* lenStrChistContentXMLElem);
		
		virtual int startDocument(void* addStruct) = 0;
		// virtual int startDocument() = 0;
		
		virtual int startElement(const char* nameElem, void* addStruct) = 0;
		// virtual int startElement(const char* nameElem) = 0;

		virtual int characters(const char* buff, void* point_section, void* addStruct) = 0;
		// virtual int characters(const char* buff) = 0;
		
		virtual int endElement(const char* nameElem, void* addStruct) = 0;
		// virtual int endElement(const char* nameElem) = 0;
		
		virtual int saxParser(void* point_sect, char* strXML, unsigned int strLen, void* addStruct) = 0;		
	};
	//--------------------------------------------------
	class AirportsXMLWorker : public SaxParsXMLWorker
	{
	 public:
		 int startDocument(void* addStruct);
		 int startElement(const char* nameElem, void* addStruct);
		 int characters(const char* buff, void* point_section, void* addStruct);
		 int endElement(const char* nameElem, void* addStruct);
		 int saxParser(void* point_sect, char* strXML, unsigned int strLen, void* addStruct);
	};
	//-------------------------------------------------
	class ConfigAppXMLWorker : public SaxParsXMLWorker
	{
	 public:
		 int startDocument(void* addStruct);
		 int startElement(const char* nameElem, void* addStruct);
		 int characters(const char* buff, void* point_section, void* addStruct);
		 int endElement(const char* nameElem, void* addStruct);
		 int saxParser(void* point_sect, char* strXML, unsigned int strLen, void* addStruct);
	};
}
#endif