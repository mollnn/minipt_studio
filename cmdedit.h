#ifndef CMDEDIT_H
#define CMDEDIT_H

#include <QTextEdit>
#include <QKeyEvent>


class CmdEdit : public QTextEdit
{
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent* event);
public:
    CmdEdit(QWidget *parent = nullptr);
signals:
    void SendCmd(const QString& cmd);
};

#endif // CMDEDIT_H
