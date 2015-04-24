#include <QtWidgets/QApplication>
#include "BatchRename.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BatchRename w;
    w.show();

    return a.exec();
}
