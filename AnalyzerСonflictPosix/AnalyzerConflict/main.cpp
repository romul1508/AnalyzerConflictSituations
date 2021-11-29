// AnalyzerConflict implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 12.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved


#include "../AnalyzerConflictService.h"

//#ifdef __linux__
////linux code goes here
//    #include "../Config_app.h"
//    #include "../Config_airports.h"
//#elif _WIN32
//// windows code goes here

//#else
//// #error "OS not supported!"
//#endif

// The AnalyzerConflict software module implements some of the functions of A-SMGCS 
// (Advanced Surface Movement Guidance and Control System).  
// AnalyzerConflict is a console software module. 
// Mostly it is intended for educational purposes. 
// The program module is implemented in the C ++ programming language
// in the development environment - Qt Creator. 
// Dispenses with Qt functionality to perform functionality. 
// The program functions in LINUX-like operating systems.

int main(int argc, char *argv[])
{
    int err = 0;
    printf("Start service \"Analyzer conflict\"!\n");

    ern::Log::getInstance().Write("Start service \"Analyzer conflict\"!\n");

    // const char* conf_file_name = "conf_app.xml";
    // const char* conf_data_airodrome_name = "conf_airports.xml";
    // const char* csv_file_cat11 = "cat11_";
    #ifdef __linux__
        //linux code goes here
        const char* conf_file_name = "conf_app.xml";
        const char* conf_data_airodrome_name = "conf_airports.xml";
    #elif _WIN32
    // windows code goes here

    #else
        // #error "OS not supported!"
    #endif

    ern::ConfiguratorApp::getInstance().LoadXML(conf_file_name);
    ern::ConfiguratorAirports::getInstance().LoadXML(conf_data_airodrome_name);

	// checking the correct loading of the configuration data
    // testLoadAppSetXML();		
    
	// checking the correctness of loading the adjustment data of aerodromes
	// testLoadAirportsSetXML();	
    
	// determination of the correctness of the calculation of the coordinates 
	// of the vertices of the polygon
	// testDetermCorrectnCalcCoordPolygonvertices();		

    ern::ConfiguratorApp::getInstance().setRunService(1);
    ern::ConfiguratorApp::getInstance().UpdateXML(conf_file_name);

    if(StartWinsock())
    {
        AsterixWorker* ptrAsterixWorker = new AsterixWorker();
        if(server(ptrAsterixWorker)!=1)
        {
            delete ptrAsterixWorker;
            return 1;
        }
        delete ptrAsterixWorker;
    }
    else
        err = 1;


    return err;
}
