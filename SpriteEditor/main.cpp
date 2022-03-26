#include "view.h"
#include "model.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    model model;
    View w(model);
    w.show();
    return a.exec();
}