#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    serialPort(new QSerialPort(this))
{
    ui->setupUi(this);

    // signal for tracking data from ESP32
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::receiveDataFromESP);

    // find device with vendor id 1A86 - "server"
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.hasVendorIdentifier() && info.vendorIdentifier() == 0x1A86) {
            //QMessageBox::information(this, "About port", "Found ESP on port: " + info.portName());
            serialPort->setPort(info);
        }
    }

    // set parameters for messanging
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite)) {
        //QMessageBox::critical(this, "Error", "Failed to open serial port!");
    }

    xmlFilePath = "../../config.xml";
}

MainWindow::~MainWindow() {
    // close serial port
    if (serialPort->isOpen())
        serialPort->close();
    delete ui;
}

// send xml on ESP32
void MainWindow::sendXmlToESP(const QString &xmlFilePath) {
    // open file
    QFile file(xmlFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open XML file!");
        return;
    }

    // read all data from file
    QByteArray xmlData = file.readAll();
    file.close();

    // send data on serial port
    if (serialPort->isOpen()) {
        serialPort->write(xmlData);
        serialPort->write("\n");
    }
}

// recieve data from ESP32
void MainWindow::receiveDataFromESP() {
    // check if incoming data contains '\n'
    incomingData += serialPort->readAll();
    if (incomingData.contains('\n')) {
        QFile file(xmlFilePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << incomingData;
            file.close();
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to get data!");
        }
        incomingData.clear();
    }
    getXmlMessage();
}

// set message in xml file
void MainWindow::setXmlMessage(const QString &filePath, const QString &newText) {
    // open file for reading
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open file for reading!");
        return;
    }

    // get data from xml
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::critical(this, "Error", "Failed to get data from xml!");
        file.close();
        return;
    }
    file.close();

    // find tag "configs"
    QDomElement root = doc.documentElement();
    QDomNodeList configsList = root.elementsByTagName("configs");
    if (configsList.isEmpty()) {
        QMessageBox::critical(this, "Error", "Configs not found!");
        return;
    }

    // set new text
    QDomElement configsElement = configsList.at(0).toElement();
    if (!configsElement.isNull()) {
        configsElement.firstChild().setNodeValue(newText);
    } else {
        QMessageBox::critical(this, "Error", "Configs is null!");
        return;
    }

    // open file for writing
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open file for writing!");
        return;
    }

    // write modified XML to file as a single line
    QTextStream out(&file);
    QString xmlString = doc.toString();
    xmlString.remove(QRegExp("[\\n\\r\\t]"));
    out << xmlString;
    file.close();
}

// get message from xml file
void MainWindow::getXmlMessage() {
    // open xml files
    QFile file(xmlFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open file!");
    }

    // get content from xml file
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::critical(this, "Error", "Failed to get content from file!");
        file.close();
    }
    file.close();

    // find tag configs
    QDomElement root = doc.documentElement();
    QDomNodeList configsList = root.elementsByTagName("configs");
    if (configsList.isEmpty()) {
        QMessageBox::critical(this, "Error", "Failed to find tag!");
    }

    // get value from tag
    QDomElement configsElement = configsList.at(0).toElement();
    if (!configsElement.isNull()) {
        ui->lineMessage->setText(configsElement.text());
    } else {
        QMessageBox::critical(this, "Error", "Failed to get value from tag!");
    }
}

// perfomance button
void MainWindow::on_buttonProcess_clicked()
{
    setXmlMessage(xmlFilePath, ui->lineMessage->text());
    sendXmlToESP(xmlFilePath);
}
