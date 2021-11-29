QT += core network
QT -= gui

TARGET = UdpClient2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    getdata.cpp

HEADERS += \
    getdata.h

