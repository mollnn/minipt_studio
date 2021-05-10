#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("minipt studio");
    w.setFixedSize(1366, 768);
    w.show();
    return a.exec();
}
