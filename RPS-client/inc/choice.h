#ifndef CHOICE_H
#define CHOICE_H

#include <QMainWindow>

namespace Ui {
class Choice;
}

class Choice : public QMainWindow
{
    Q_OBJECT

public:
    explicit Choice(QWidget *parent = nullptr);
    ~Choice();

    int choiceP1;
    int choiceP2;
    int gameModeChoice;

signals:
    void sigCho();
    void sigPassCho1();
    void sigPassCho2();
    void sigCloseCho1();
    void sigCloseCho2();

private slots:
    void on_buttonNext_clicked();

    void on_buttonNext2_clicked();

public slots:
    void showGameMode(int gameMode);

private:
    Ui::Choice *ui;
};

#endif // CHOICE_H
