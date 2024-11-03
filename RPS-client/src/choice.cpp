#include "../inc/choice.h"
#include "../inc/mainwindow.h"
#include "ui_choice.h"

Choice::Choice(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Choice)
{
    ui->setupUi(this);

    //ui->radioPaper->setChecked(true);
}

Choice::~Choice()
{
    delete ui;
}

void Choice::on_buttonNext_clicked()
{
    int choice = 0;
    if (ui->radioRock->isChecked())
        choice = 1;
    else if (ui->radioPaper->isChecked())
        choice = 2;
    else if (ui->radioScissors->isChecked())
        choice = 3;
    choiceP1 = choice;
    emit sigPassCho1();

    //this->close();
    emit sigCloseCho1();

    ui->radioRock->setAutoExclusive(false);
    ui->radioRock->setChecked(false);
    ui->radioPaper->setAutoExclusive(false);
    ui->radioPaper->setChecked(false);
    ui->radioScissors->setAutoExclusive(false);
    ui->radioScissors->setChecked(false);

    if (gameModeChoice == 1) {
        hide();
    }
}


void Choice::on_buttonNext2_clicked()
{
    int choice = 0;
    if (ui->radioRock->isChecked())
        choice = 1;
    else if (ui->radioPaper->isChecked())
        choice = 2;
    else if (ui->radioScissors->isChecked())
        choice = 3;
    choiceP2 = choice;
    emit sigPassCho2();

    ui->radioRock->setAutoExclusive(false);
    ui->radioRock->setChecked(false);
    ui->radioPaper->setAutoExclusive(false);
    ui->radioPaper->setChecked(false);
    ui->radioScissors->setAutoExclusive(false);
    ui->radioScissors->setChecked(false);

    this->close();
    emit sigCloseCho2();
}

void Choice::showGameMode(int gameMode)
{
    gameModeChoice = gameMode;
    this->show();
}

