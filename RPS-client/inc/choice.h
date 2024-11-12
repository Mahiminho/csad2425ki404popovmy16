#ifndef CHOICE_H
#define CHOICE_H

#include <QMainWindow>

namespace Ui {
class Choice;
}

/**
 * @class Choice
 * @brief The Choice class represents a window where players make their choices and select the game mode.
 *
 * This class manages user choices for two players and allows setting of the game mode.
 */
class Choice : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Choice object.
     * @param parent The parent widget of this window, typically nullptr if no parent is specified.
     */
    explicit Choice(QWidget *parent = nullptr);

    /**
     * @brief Destroys the Choice object, releasing any allocated resources.
     */
    ~Choice();

    int choiceP1; ///< Player 1's choice in the game.
    int choiceP2; ///< Player 2's choice in the game.
    int gameModeChoice; ///< The selected game mode.

signals:
    /**
     * @brief Signal emitted when a general choice action is made.
     */
    void sigCho();

    /**
     * @brief Signal emitted to pass Player 1's choice.
     */
    void sigPassCho1();

    /**
     * @brief Signal emitted to pass Player 2's choice.
     */
    void sigPassCho2();

    /**
     * @brief Signal emitted to indicate closing of the Player 1 choice interface.
     */
    void sigCloseCho1();

    /**
     * @brief Signal emitted to indicate closing of the Player 2 choice interface.
     */
    void sigCloseCho2();

private slots:
    /**
     * @brief Slot that handles the "Next" button click event for advancing the choice selection process.
     */
    void on_buttonNext_clicked();

    /**
     * @brief Slot that handles the second "Next" button click event for advancing further in the choice process.
     */
    void on_buttonNext2_clicked();

public slots:
    /**
     * @brief Sets and displays the game mode based on the given mode.
     * @param gameMode The selected game mode to display.
     */
    void showGameMode(int gameMode);

private:
    Ui::Choice *ui; ///< Pointer to the UI elements of the Choice class.
};

#endif // CHOICE_H
