#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "actions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void writeToFile(const QString &fileName, const QString &data);

private slots:
    void on_buttonManAI_clicked();
    void on_buttonManMan_clicked();
    void on_buttonAIAIRM_clicked();
    void on_buttonAIAIWS_clicked();
    void on_buttonExit_clicked();

public:
    Ui::MainWindow *ui;
    Actions *act;
};

#endif // MAINWINDOW_H
