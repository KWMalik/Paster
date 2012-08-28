#include <QApplication>
#include "PasterWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PasterWindow w;
    w.show();
    
    return a.exec();
}
