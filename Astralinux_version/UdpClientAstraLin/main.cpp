#include <QCoreApplication>
#include "getdata.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GetData pusk;

    return a.exec();
}

