#include <QTest>
#include <QSignalSpy>
#include "../inc/actions.h"

namespace Ui {
    class Actions;
}

class TestActions : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testButtonBackClicked();
    void testButtonNewClicked();
    void testWriteToFile();

private:
    Actions *actions;
};

void TestActions::initTestCase()
{
    actions = new Actions();
}

void TestActions::cleanupTestCase()
{
    delete actions;
}

void TestActions::testButtonBackClicked()
{
    QSignalSpy spy(actions, &Actions::sigAct);
    actions->on_buttonBack_clicked();
    QCOMPARE(spy.count(), 1);
}

void TestActions::testButtonNewClicked()
{
    QSignalSpy spy(actions, &Actions::sigSetScore);
    actions->on_buttonNew_clicked();
    QCOMPARE(spy.count(), 1);
    QFile file("scoreType.txt");
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&file);
    QString content = in.readAll();
    QCOMPARE(content, QString("1"));
    file.close();
}

void TestActions::testWriteToFile()
{
    actions->writeToFile("testFile.txt", "testData");
    QFile file("testFile.txt");
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&file);
    QString content = in.readAll();
    QCOMPARE(content, QString("testData"));
    file.close();
    file.remove();
}

QTEST_MAIN(TestActions)
#include "test_actions.moc"