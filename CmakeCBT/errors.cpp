#include "errors.h"

void errors::Error(std::string msg)
{
    const QString cmsg = QString::fromStdString(msg);
    QMessageBox messageBox;
    messageBox.critical(0,"Error",cmsg);
    messageBox.setFixedSize(500,200);
}

void errors::MSG(std::string msg)
{
    const QString cmsg = QString::fromStdString(msg);
    QMessageBox messageBox;
    messageBox.information(0,"Info",cmsg);
    messageBox.setFixedSize(500,200);
}
