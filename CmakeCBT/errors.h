#ifndef ERRORS_H
#include <string>
#include <QMessageBox>
#include <Qstring>
class errors
{
public:
    static void Error(std::string msg);
    static void MSG(std::string msg);
};

#endif // ERRORS_H

#define ERRORS_H
