#ifndef ACTIONS_H
#define ACTIONS_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "result.h"

namespace Ui {
class Actions;
}

class Actions : public QMainWindow
{
    Q_OBJECT

public:
    explicit Actions(QWidget *parent = nullptr);
    ~Actions();

    void writeToFile(const QString &fileName, const QString &data);

private:
    Ui::Actions *ui;
    Result *res;

signals:
    void sigAct();
    void sigSetScore();

private slots:
    void on_buttonBack_clicked();
    void on_buttonNew_clicked();
    void on_buttonLoad_clicked();
};

#endif // ACTIONS_H
