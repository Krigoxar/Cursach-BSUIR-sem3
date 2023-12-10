#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include "model.h"
#include <QMainWindow>

namespace Ui {
class StudentWindow;
}

class StudentWindow : public QMainWindow
{
    Q_OBJECT

    AccountDB* acc;
    Ui::StudentWindow *ui;
    QDate date;
public:
    explicit StudentWindow(QWidget *parent = nullptr);

    void Setdate(QDate date);
    void UpdateWindow();

    ~StudentWindow();
    void setAccAndFill(AccountDB* Acc);
};

#endif // STUDENTWINDOW_H
