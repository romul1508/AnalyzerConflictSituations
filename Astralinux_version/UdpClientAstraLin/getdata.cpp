#include "getdata.h"
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>


GetData::GetData(QObject *parent):QObject(parent)
{
    udpsock  = new QUdpSocket(this);
    udpsock->bind(QHostAddress::LocalHost,24322);
    connect(udpsock,SIGNAL(readyRead()),SLOT(SlotDatagram()));

}
GetData::~GetData()
{

}

void GetData::SlotDatagram()
{
    QByteArray ba;
    do
    {
        ba.resize(udpsock->pendingDatagramSize());
        udpsock->readDatagram(ba.data(),ba.size());
        qDebug()<<"datagram: "<<ba.data();
    }while(udpsock->hasPendingDatagrams());


    //QDataStream in(&ba, QIODevice::ReadOnly);
    //in.setVersion(QDataStream::Qt_5_3);



}

