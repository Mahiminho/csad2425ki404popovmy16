#include <QTest>
#include "../inc/result.h"

namespace Ui {
class Result;
}

class TestResult : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void testSetChoice1();
    void testSetChoice2();
    void testReadFromFile();
    void testWriteToFile();
    void cleanupTestCase();

public:
    Result *result;
};

void TestResult::initTestCase() {
    result = new Result();
}

void TestResult::testSetChoice1() {
    result->cho->choiceP1 = 1;
    result->setChoice1();
    QCOMPARE(result->choice1, 1);
}

void TestResult::testSetChoice2() {
    result->cho->choiceP2 = 2;
    result->setChoice2();
    QCOMPARE(result->choice2, 2);
}

void TestResult::testReadFromFile() {
    QString content = result->readFromFile("test_file.txt");
    QCOMPARE(content, QString(""));
}

void TestResult::testWriteToFile() {
    QString testFileName = "output_file.txt";
    result->writeToFile(testFileName, "");

    QFile file(testFileName);
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QCOMPARE(content, QString(""));
}

void TestResult::cleanupTestCase() {
    delete result;
}

QTEST_MAIN(TestResult)
#include "UT-app-tester.moc"
