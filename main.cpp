#include <QtWidgets/QApplication>
#include "BatchReplacer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BatchReplacer w;
    w.show();

    return a.exec();
}
