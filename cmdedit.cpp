#include "cmdedit.h"
#include <QDebug>

CmdEdit::CmdEdit(QWidget *parent)
    :QTextEdit(parent)
{
}

void CmdEdit::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        SendCmd(this->toPlainText());
        this->clear();
    }
    else
    {
        QTextEdit::keyPressEvent(event);
    }
}
