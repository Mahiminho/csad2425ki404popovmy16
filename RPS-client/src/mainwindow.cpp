#include "../inc/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    act = new Actions();
    connect(act, &Actions::sigAct, this, &MainWindow::show);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonManAI_clicked()
{
    writeToFile("gameMode.txt", "1");
    hide();
    act->show();
}

void MainWindow::on_buttonManMan_clicked()
{
    writeToFile("gameMode.txt", "2");
    hide();
    act->show();
}

void MainWindow::on_buttonAIAIRM_clicked()
{
    writeToFile("gameMode.txt", "3");
    hide();
    act->show();
}

void MainWindow::on_buttonAIAIWS_clicked()
{
    writeToFile("gameMode.txt", "4");
    hide();
    act->show();
}

void MainWindow::on_buttonExit_clicked()
{
    QApplication::quit();
}

void MainWindow::writeToFile(const QString &fileName, const QString &data) {
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "Failed to open gameMode.txt");
    }
}
