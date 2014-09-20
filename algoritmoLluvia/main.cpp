#include "lluvia.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    lluvia w;
    w.show();

    return a.exec();
}
