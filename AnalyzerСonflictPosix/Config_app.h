#ifndef _CONFIG_APP_H_
#define _CONFIG_APP_H_

// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 10.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include "Singleton.h"
#include "SAXparser.h"

namespace ern{
#define BUFF_CONFIG_APP_INFO	200


class ConfiguratorApp;					

//--------------------------------

// template <typename T>
class  ConfiguratorAppDestroyer : public SingletonDestroyer<ConfiguratorApp>
{
  protected:
		friend class SingletonDestroyer<ConfiguratorApp>;		
  public:
	  ConfiguratorAppDestroyer() : SingletonDestroyer<ConfiguratorApp>() { }    
};

#ifdef __linux__
    //linux code goes here
    template <typename ConfiguratorApp> ConfiguratorApp* Singleton<ConfiguratorApp>::singletonInstance = 0;
    
#elif _WIN32
    // windows code goes here
    ConfiguratorApp * Singleton<ConfiguratorApp>::singletonInstance = 0;
#else
// #error "OS not supported!"
#endif
//----------------------------------------
 class ConfiguratorApp : public Singleton<ConfiguratorApp>
 {
	private:
		 static ConfiguratorAppDestroyer conf_app_destroyer;


		 ConfiguratorApp() : Singleton<ConfiguratorApp>()
		 {
			 conf_app_destroyer.initialize(this);
		 }		 	 
		 
	 protected:
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
			friend class Singleton<ConfiguratorApp>;			
 
	 public:
		 // loads data into application configurator from xml file
		 void LoadXML(const char* name_file) 
		 {
				//---------------------------------------
			 	int ch;
				unsigned int dwBufferLen;
				char* strXML;
				
				sax_pars_xml::LPConfAppSection pConfAppSections;				
				sax_pars_xml::LPAddConfigAppStruct pAddConfigAppStruct;
				sax_pars_xml::ConfigAppXMLWorker* pConfigAppXMLWorker;
				//--------------------
				// Determine the size of the xml-string
				FILE* xml;
				// xml = fopen("test.xml", "r");
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
        
				 // now we will fill it from the file
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

				// Let's analyze the given xml
				pConfAppSections 
					= (sax_pars_xml::TConfAppSection*)calloc(1, 
					sizeof(sax_pars_xml::TConfAppSection)); 
				pAddConfigAppStruct 
					= (sax_pars_xml::TAddConfigAppStruct*) calloc(1, 
						sizeof(sax_pars_xml::TAddConfigAppStruct));
				pConfigAppXMLWorker = new sax_pars_xml::ConfigAppXMLWorker();
				pConfigAppXMLWorker->saxParser(pConfAppSections, strXML, dwBufferLen, 
					pAddConfigAppStruct);
				
				// then the data is loaded from XML into the corresponding fields 
				// of the ConfiguratorApp class 
				memset(airport_name_en, 0, BUFF_NAME_AIRPORTS);
				strncpy(airport_name_en, pConfAppSections[0].airport_name_en, BUFF_NAME_AIRPORTS);
				port_main_client = pConfAppSections[0].port_main_client;
				port_test_client_cons = pConfAppSections[0].port_test_client_cons;
				port_test_client_gui = pConfAppSections[0].port_test_client_gui;

				memset(ip_main_client, 0, BUFF_IP_CLIENT);
				strncpy(ip_main_client, pConfAppSections[0].ip_main_client, BUFF_IP_CLIENT);
				memset(ip_test_client_cons, 0, BUFF_IP_CLIENT);
				strncpy(ip_test_client_cons, pConfAppSections[0].ip_test_client_cons, BUFF_IP_CLIENT);
				memset(ip_test_client_gui, 0, BUFF_IP_CLIENT);
				strncpy(ip_test_client_gui, pConfAppSections[0].ip_test_client_gui, BUFF_IP_CLIENT);
				recording_mode =pConfAppSections[0].recording_mode;
				table_count = pConfAppSections[0].table_count;
				record_count = pConfAppSections[0].record_count;
				run_service = pConfAppSections[0].run_service;
				//------------------------------------
				// free(pConfigAppXMLWorker);
				delete pConfigAppXMLWorker;
				free(pConfAppSections);
				free(pAddConfigAppStruct);
				free(strXML);
		 }


		// saves updated app configurator in xml file
		 void UpdateXML(const char* name_file)
		 {
			 
			char str_xml[800];
			std::string string_xml = "<config_app>\n\t<point_section>\n\t\t<airport_name_en>";
			string_xml += airport_name_en;
			string_xml += "</airport_name_en>\n\t\t<port_main_client>";			
			//-----------------------------------------------------						
			sprintf(str_xml, "%s%d", string_xml.c_str(), port_main_client);
			string_xml = str_xml;
			//-----------------------------------------------------			
			string_xml += "</port_main_client>\n\t\t<port_test_client_cons>";
			sprintf(str_xml, "%s%d", string_xml.c_str(), port_test_client_cons);
			string_xml = str_xml;
			//----------------------------------------------------			
			string_xml += "</port_test_client_cons>\n\t\t<port_test_client_gui>";
			sprintf(str_xml, "%s%d", string_xml.c_str(), port_test_client_gui);
			string_xml = str_xml;
			//----------------------------------------------------			
			 string_xml += "</port_test_client_gui>\n\t\t<ip_main_client>";
			 string_xml += ip_main_client;
			 string_xml += "</ip_main_client>\n\t\t<ip_test_client_cons>";
			 string_xml += ip_test_client_cons;
			 string_xml += "</ip_test_client_cons>\n\t\t<ip_test_client_gui>";
			 string_xml += ip_test_client_gui;
			 //------------------------------------------			
			 string_xml += "</ip_test_client_gui>\n\t\t<recording_mode>";
			 sprintf(str_xml, "%s%d", string_xml.c_str(), (int)recording_mode);
			 string_xml = str_xml;
			//------------------------------------------			
			string_xml += "</recording_mode>\n\t\t<table_count>";
			sprintf(str_xml, "%s%d", string_xml.c_str(), table_count);
			string_xml = str_xml;
			//-----------------------------------------			
			string_xml += "</table_count>\n\t\t<record_count>";
			sprintf(str_xml, "%s%d", string_xml.c_str(), record_count);
			string_xml = str_xml;
			//----------------------------------------
			string_xml += "</record_count>\n\t\t<run_service>";
			sprintf(str_xml, "%s%d", string_xml.c_str(), run_service);
			string_xml = str_xml;
			string_xml += "</run_service>\n\t</point_section>\n</config_app>\n";

			 FILE* pConfigAppXML;
			 pConfigAppXML = fopen(name_file, "wt");
			 fprintf(pConfigAppXML, string_xml.c_str());				
			 fclose(pConfigAppXML);
		 }

		 // a pointer to the string to be filled is passed to the method input, 
		 // the second parameter is its maximum size; 
		 // the method returns the number of characters written into the string
		 int getAirportNameEn(char* strNameAirport, int size_str)
		 {
				if(size_str > BUFF_NAME_AIRPORTS)
					strncpy(strNameAirport, airport_name_en, BUFF_NAME_AIRPORTS);
				else
					strncpy(strNameAirport, airport_name_en, size_str);
				return	strlen(strNameAirport);
		 }

		 int getPortMainClient() const
		 {
			 return this->port_main_client;
		 }

		 int getPortTestClientCons() const
		 {
			 return this->port_test_client_cons;
		 }

		 int getPortTestClientGui() const
		 {
			 return this->port_test_client_gui;
		 }

		 int getIpMainClient(char* strIpMainClient, int size_str)
		 {
				if(size_str > BUFF_IP_CLIENT)
					strncpy(strIpMainClient, ip_main_client, BUFF_IP_CLIENT);
				else
					strncpy(strIpMainClient, ip_main_client, size_str);
				return	strlen(strIpMainClient);
		 }

		 int getIpTestClientCons(char* strIpTestClientCons, int size_str)
		 {
				if(size_str > BUFF_IP_CLIENT)
					strncpy(strIpTestClientCons, ip_test_client_cons, BUFF_IP_CLIENT);
				else
					strncpy(strIpTestClientCons, ip_test_client_cons, size_str);
				return	strlen(strIpTestClientCons);
		 }

		 int getIpTestClientGui(char* strIpTestClientGui, int size_str)
		 {
				if(size_str > BUFF_IP_CLIENT)
					strncpy(strIpTestClientGui, ip_test_client_gui, BUFF_IP_CLIENT);
				else
					strncpy(strIpTestClientGui, ip_test_client_gui, size_str);
				return	strlen(strIpTestClientGui);
		 }

		 char getRecordingMode() const
		 {
				return this->recording_mode;
		 }

		int getTableCount() const
		{
			return this->table_count;
		}

		int getRecordCount() const
		{
			return this->record_count;
		}

		int getRunService() const
		{
			return this->run_service;
		}

		void setTableCount(int table_count)
		{
			this->table_count = table_count;
		}

		void setRecordCount(int record_count) 
		{
			this->record_count = record_count;
		}

		void setRunService(int run_service) 
		{
			this->run_service = run_service;
		}
 };      

 
}

#endif /* _CONFIG_APP_H_ */
