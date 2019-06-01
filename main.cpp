#include "snowmainwnd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SnowMainWnd w;
    w.show();

    return a.exec();
}
