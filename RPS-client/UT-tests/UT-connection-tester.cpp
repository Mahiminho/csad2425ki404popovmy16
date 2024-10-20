#include <QTest>
#include "../mainwindow.h"

class UTConnectionTester : public QObject {
    Q_OBJECT

private slots:
    // void testSerialPortInitialization() {
    //     MainWindow window;

    //     QVERIFY(window.serialPort->isOpen());
    //     QCOMPARE(window.serialPort->baudRate(), QSerialPort::Baud9600);
    //     QCOMPARE(window.serialPort->dataBits(), QSerialPort::Data8);
    //     QCOMPARE(window.serialPort->parity(), QSerialPort::NoParity);
    //     QCOMPARE(window.serialPort->stopBits(), QSerialPort::OneStop);
    //     QCOMPARE(window.serialPort->flowControl(), QSerialPort::NoFlowControl);
    // }

    // void testSendXmlToESP() {
    //     MainWindow window;

    //     QFile testFile(window.xmlFilePath);
    //     testFile.open(QIODevice::WriteOnly | QIODevice::Text);
    //     testFile.write("<user><configs>Test Data</configs></user>");
    //     testFile.close();

    //     window.sendXmlToESP(window.xmlFilePath);

    //     QVERIFY(window.serialPort->isOpen());
    //     QCOMPARE(window.serialPort->bytesToWrite(), (qint64)(strlen("<user><configs>Test Data</configs></user>\n")));
    // }

    void testSetXmlMessage() {
        MainWindow window;

        QFile testFile(window.xmlFilePath);
        testFile.open(QIODevice::WriteOnly | QIODevice::Text);
        testFile.write("<root><configs>Old Data</configs></root>");
        testFile.close();

        window.setXmlMessage(window.xmlFilePath, "New Data");

        testFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QDomDocument doc;
        doc.setContent(&testFile);
        testFile.close();
        QDomElement root = doc.documentElement();
        QDomNodeList configsList = root.elementsByTagName("configs");
        QDomElement configsElement = configsList.at(0).toElement();
        QCOMPARE(configsElement.text(), QString("New Data"));
    }
};

QTEST_MAIN(UTConnectionTester)
#include "UT-connection-tester.moc"
