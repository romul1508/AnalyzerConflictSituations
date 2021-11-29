// #include <iostream>
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



// using namespace std;


int main(int argc, char *argv[])
{
    int err = 0;
    printf("Start service \"Analyzer conflict\"!\n");


//    #ifdef __linux__
//        //linux code goes here
//        ConfiguratorAppDestroyer ConfiguratorApp::conf_app_destroyer;
//        ConfiguratorAirportsDestroyer ConfiguratorAirports::conf_airports_destroyer;
//    #elif _WIN32
//    // windows code goes here

//    #else
//        // #error "OS not supported!"
//    #endif

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

    // testLoadAppSetXML();		// проверка правильности загрузки насроечных данных
    // testLoadAirportsSetXML();	// проверка правильности загрузки насроечных данных аэродромов
    // testDetermCorrectnCalcCoordPolygonvertices();		// определение правильности расчета координат вершин полигона

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
