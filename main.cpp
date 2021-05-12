#include "glwidget.h"
#include <QApplication>
#include <QTextEdit>
#include <QGridLayout>
#include "cmdedit.h"

#include <minipt/minipt.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget *pWindow = new QWidget;

    CmdEdit *pCmdEdit = new CmdEdit;

    GlWidget *pGlWidget = new GlWidget;

    pGlWidget->setFixedHeight(720);

    pWindow->setFixedSize(1366, 768);

    QObject::connect(pCmdEdit,SIGNAL(SendCmd(const QString&)),pGlWidget,SLOT(ExecuteCmd(const QString&)));

    QGridLayout *pGridLayout = new QGridLayout;

    pGridLayout->addWidget(pGlWidget, 0, 0, 2, 2);
    pGridLayout->addWidget(pCmdEdit, 3, 0, 1, 2);

    pWindow->setLayout(pGridLayout);

    pWindow->show();

    return a.exec();
}
