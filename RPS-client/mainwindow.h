#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>
#include <QCoreApplication>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString xmlFilePath;
    void setXmlMessage(const QString &filePath, const QString &newText);

    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QByteArray incomingData;

public slots:
    void sendXmlToESP(const QString &xmlFilePath);
    void receiveDataFromESP();
    void getXmlMessage();

    void on_buttonProcess_clicked();
};

#endif // MAINWINDOW_H
