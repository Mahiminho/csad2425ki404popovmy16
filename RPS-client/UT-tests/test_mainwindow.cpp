#include <QTest>
#include <QSignalSpy>
#include "../inc/mainwindow.h"
#include "../ui_mainwindow.h"

namespace Ui {
    class MainWindow;
}

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testButtonManAI();
    void testButtonManMan();
    void testButtonAIAIRM();
    void testButtonAIAIWS();
    void testButtonExit();

private:
    MainWindow *mainWindow;
};

void TestMainWindow::initTestCase()
{
    mainWindow = new MainWindow();
    mainWindow->show();
}

void TestMainWindow::cleanupTestCase()
{
    delete mainWindow;
}

void TestMainWindow::testButtonManAI()
{
    QPushButton *button = mainWindow->findChild<QPushButton*>("buttonManAI");
    QVERIFY(button != nullptr);
    QSignalSpy spy(mainWindow, &MainWindow::hide);
    QTest::mouseClick(button, Qt::LeftButton, Qt::NoModifier, QPoint(), 0);
    QCOMPARE(spy.count(), 0);
    QFile file("gameMode.txt");
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&file);
    QString content = in.readAll();
    QCOMPARE(content, QString("1"));
}

void TestMainWindow::testButtonManMan()
{
    QPushButton *button = mainWindow->findChild<QPushButton*>("buttonManMan");
    QVERIFY(button != nullptr);
    QSignalSpy spy(mainWindow, &MainWindow::hide);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 0);
    QFile file("gameMode.txt");
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&file);
    QString content = in.readAll();
    QCOMPARE(content, QString("2"));
}

void TestMainWindow::testButtonAIAIRM()
{
    QPushButton *button = mainWindow->findChild<QPushButton*>("buttonAIAIRM");
    QVERIFY(button != nullptr);
    QSignalSpy spy(mainWindow, &MainWindow::hide);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 0);
    QFile file("gameMode.txt");
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&file);
    QString content = in.readAll();
    QCOMPARE(content, QString("3"));
}

void TestMainWindow::testButtonAIAIWS()
{
    QPushButton *button = mainWindow->findChild<QPushButton*>("buttonAIAIWS");
    QVERIFY(button != nullptr);
    QSignalSpy spy(mainWindow, &MainWindow::hide);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 0);
    QFile file("gameMode.txt");
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&file);
    QString content = in.readAll();
    QCOMPARE(content, QString("4"));
}

void TestMainWindow::testButtonExit()
{
    QPushButton *button = mainWindow->findChild<QPushButton*>("buttonExit");
    QVERIFY(button != nullptr);
    QSignalSpy spy(QApplication::instance(), &QApplication::aboutToQuit);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 0);
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"