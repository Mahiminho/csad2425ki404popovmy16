#include <QTest>
#include <QSignalSpy>
#include "../inc/result.h"
#include "../ui_result.h"

namespace Ui {
    class Result;
}

class TestResult : public QObject
{
    Q_OBJECT

private slots:
    void testSetChoice1();
    void testSetChoice2();
    void testOnButtonBackClicked();
    void testOnButtonNextClicked();
    void testReadFromFile();
    void testSetScore();
    void testWriteToFile();
    void testOnButtonSaveClicked();
    void testSetXmlMessage();
    void testSendXmlToESP();
    void testReceiveDataFromESP();
    void testGetXmlMessage();
};

void TestResult::testSetChoice1()
{
    Result result;
    result.cho->choiceP1 = 1;
    result.setChoice1();
    QCOMPARE(result.choice1, 1);
}

void TestResult::testSetChoice2()
{
    Result result;
    result.cho->choiceP2 = 2;
    result.setChoice2();
    QCOMPARE(result.choice2, 2);
}

void TestResult::testOnButtonBackClicked()
{
    Result result;
    QSignalSpy spy(&result, &Result::sigRes);
    result.on_buttonBack_clicked();
    QCOMPARE(spy.count(), 1);
}

void TestResult::testOnButtonNextClicked()
{
    Result result;
    result.writeToFile("gameMode.txt", "1");
    result.on_buttonNext_clicked();
    QCOMPARE(result.gameMode_local, "1");
}

void TestResult::testReadFromFile()
{
    Result result;
    result.writeToFile("test.txt", "test content");
    QString content = result.readFromFile("test.txt");
    QCOMPARE(content, "test content");
}

void TestResult::testSetScore()
{
    Result result;
    result.writeToFile("scoreType.txt", "1");
    result.writeToFile("gameMode.txt", "1");
    result.setScore();
    QCOMPARE(result.ui->labelP1score->text(), "0");
    QCOMPARE(result.ui->labelP2score->text(), "0");
}

void TestResult::testWriteToFile()
{
    Result result;
    result.writeToFile("test.txt", "test content");
    QString content = result.readFromFile("test.txt");
    QCOMPARE(content, "test content");
}

void TestResult::testOnButtonSaveClicked()
{
    Result result;
    result.gameMode_local = "1";
    result.ui->labelP1score->setText("1");
    result.ui->labelP2score->setText("2");
    result.on_buttonSave_clicked();
    QString content = result.readFromFile("../../saved/ManVsAI_score.txt");
    QCOMPARE(content, "1 2");
}

void TestResult::testSetXmlMessage()
{
    Result result;
    result.setXmlMessage("../../config.xml", "1", "1", "2");
    QString content = result.readFromFile("../../config.xml");
    QVERIFY(content.contains("<mode>1</mode>"));
    QVERIFY(content.contains("<p1>1</p1>"));
    QVERIFY(content.contains("<p2>2</p2>"));
}

void TestResult::testSendXmlToESP()
{
    Result result;
    result.sendXmlToESP("../../config.xml");
    // This test requires a connected ESP32 device to verify the data sent.
}

void TestResult::testReceiveDataFromESP()
{
    Result result;
    result.incomingData = "<user> <mode>1</mode> <p1>1</p1> <p2>2</p2> <ai1>1</ai1> <ai2>2</ai2> <win>2</win></user>\n";
    result.receiveDataFromESP();
    QString content = result.readFromFile("../../config.xml");
    QVERIFY(content.contains("<mode>1</mode>"));
    QVERIFY(content.contains("<p1>1</p1>"));
    QVERIFY(content.contains("<p2>2</p2>"));
}

void TestResult::testGetXmlMessage()
{
    Result result;
    result.writeToFile("../../config.xml", "<user> <mode>1</mode> <p1>1</p1> <p2>2</p2> <ai1>1</ai1> <ai2>2</ai2> <win>2</win></user>");
    result.getXmlMessage();
    QCOMPARE(result.ui->labelP1type->text(), "Rock");
    QCOMPARE(result.ui->labelP2type->text(), "Rock");
    QCOMPARE(result.ui->labelResult->text(), "Player2");
}

QTEST_MAIN(TestResult)
#include "test_result.moc"