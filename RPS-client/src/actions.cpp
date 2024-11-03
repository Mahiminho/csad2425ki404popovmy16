#include "../inc/actions.h"
#include "ui_actions.h"

Actions::Actions(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Actions)
{
    ui->setupUi(this);
    res = new Result();
    connect(res, &Result::sigRes, this, &Actions::show);
    connect(this, &Actions::sigSetScore, res, &Result::setScore);
}

Actions::~Actions()
{
    delete ui;
}

void Actions::on_buttonBack_clicked()
{
    this->close();
    emit sigAct();
}

void Actions::on_buttonNew_clicked()
{
    writeToFile("scoreType.txt", "1");
    emit sigSetScore();
    hide();
    res->show();
}

void Actions::on_buttonLoad_clicked()
{
    writeToFile("scoreType.txt", "2");
    emit sigSetScore();
    hide();
    res->show();
}

void Actions::writeToFile(const QString &fileName, const QString &data) {
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "Failed to open scoreType.txt");
    }
}
