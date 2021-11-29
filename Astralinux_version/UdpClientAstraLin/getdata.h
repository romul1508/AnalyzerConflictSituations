#ifndef GETDATA_H
#define GETDATA_H

#include <QObject>
#include <QDebug>
#include <QUdpSocket>

class GetData : public QObject
{
    Q_OBJECT
public:
    explicit GetData(QObject* parent = 0);
    ~GetData();






private:

    QUdpSocket          *udpsock;

private slots:
    void SlotDatagram();

};




#endif // GETDATA_H

