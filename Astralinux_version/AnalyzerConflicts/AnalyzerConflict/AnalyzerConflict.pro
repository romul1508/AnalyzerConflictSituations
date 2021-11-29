TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt



SOURCES += main.cpp \
    ../AnalyzerConflictService.cpp \
    ../asterix.cpp \
    ../asterix_cat_11.cpp \
    ../cat_011_asterix.cpp \
    ../common_lib_asterix.cpp \
    ../GeoCalc.cpp \
    ../multi_attr.cpp \
    ../Pulkovo.cpp \
    ../SAXparser.cpp \
    ../config_app.cpp \
    ../log.cpp

HEADERS += \
    ../AnalyzerConflictService.h \
    ../asterix.hpp \
    ../asterix_cat_11.hpp \
    ../asterix_internal.hpp \
    ../asterix_reader.hpp \
    ../asterix_template.hpp \
    ../asterix_writer.hpp \
    ../cat_011_asterix.h \
    ../cat_asterix.h \
    ../Common.h \
    ../common_lib_asterix.h \
    ../Config_airports.h \
    ../Config_app.h \
    ../GeoCalc.h \
    ../Log.h \
    ../multi_attr.h \
    ../Pulkovo.h \
    ../SAXparser.h \
    ../Singleton.h \
    ../unistd.h
