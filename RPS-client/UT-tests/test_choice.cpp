#include <QTest>
#include <QSignalSpy>
#include "../inc/choice.h"
#include "../ui_choice.h"

namespace Ui {
    class Choice;
}

class TestChoice : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testButtonNextClicked();
    void testButtonNext2Clicked();
    void testShowGameMode();

private:
    Choice *choice;
};

void TestChoice::initTestCase()
{
    choice = new Choice();
}

void TestChoice::cleanupTestCase()
{
    delete choice;
}

void TestChoice::testButtonNextClicked()
{
    // Simulate radio button selection
    choice->ui->radioRock->setChecked(true);
    choice->on_buttonNext_clicked();
    QCOMPARE(choice->choiceP1, 1);

    choice->ui->radioPaper->setChecked(true);
    choice->on_buttonNext_clicked();
    QCOMPARE(choice->choiceP1, 2);

    choice->ui->radioScissors->setChecked(true);
    choice->on_buttonNext_clicked();
    QCOMPARE(choice->choiceP1, 3);
}

void TestChoice::testButtonNext2Clicked()
{
    // Simulate radio button selection
    choice->ui->radioRock->setChecked(true);
    choice->on_buttonNext2_clicked();
    QCOMPARE(choice->choiceP2, 1);

    choice->ui->radioPaper->setChecked(true);
    choice->on_buttonNext2_clicked();
    QCOMPARE(choice->choiceP2, 2);

    choice->ui->radioScissors->setChecked(true);
    choice->on_buttonNext2_clicked();
    QCOMPARE(choice->choiceP2, 3);
}

void TestChoice::testShowGameMode()
{
    choice->showGameMode(1);
    QCOMPARE(choice->gameModeChoice, 1);
    QVERIFY(choice->isVisible());

    choice->showGameMode(2);
    QCOMPARE(choice->gameModeChoice, 2);
    QVERIFY(choice->isVisible());
}

QTEST_MAIN(TestChoice)
#include "test_choice.moc"