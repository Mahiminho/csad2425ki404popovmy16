#ifndef RESULT_H
#define RESULT_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>
#include <QCoreApplication>
#include "choice.h"

namespace Ui {
class Result;
}

class Result : public QMainWindow
{
    Q_OBJECT

public:
    explicit Result(QWidget *parent = nullptr);
    ~Result();

    QString gameMode_local;
    QString score_local;
    QString xmlFilePath;
    QSerialPort *serialPort;
    QByteArray incomingData;
    Choice *cho;

    QString readFromFile(const QString &fileName);
    void writeToFile(const QString &fileName, const QString &data);

    void setXmlMessage(const QString &filePath, QString gameMode_local, QString choice1, QString choice2);
    void sendXmlToESP(const QString &xmlFilePath);
    void receiveDataFromESP();
    void getXmlMessage();

    int choice1;
    int choice2;
    void setChoice1();
    void setChoice2();

private:
    Ui::Result *ui;

signals:
    void sigRes();
    void sigResToChoice();
    void sigShowGameMode(int gameMode);

private slots:
    void on_buttonBack_clicked();
    void on_buttonNext_clicked();
    void on_buttonSave_clicked();

public slots:
    void setScore();
};

#endif // RESULT_H
