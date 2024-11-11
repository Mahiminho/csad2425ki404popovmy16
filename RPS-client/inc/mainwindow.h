#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "actions.h"

namespace Ui {
class MainWindow;
}

/**
 * @class MainWindow
 * @brief The MainWindow class represents the main application window where users can choose game modes and interact with the application.
 *
 * This class provides options for selecting different game modes and includes a method for saving data to a file.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MainWindow object.
     * @param parent The parent widget of this window, typically nullptr if no parent is specified.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the MainWindow object, releasing any allocated resources.
     */
    ~MainWindow();

    /**
     * @brief Writes data to a specified file.
     * @param fileName The name of the file to write to.
     * @param data The content to be written into the file.
     *
     * This function attempts to open the file in write mode and write the provided data into it.
     */
    void writeToFile(const QString &fileName, const QString &data);

private slots:
    /**
     * @brief Slot that handles the "Man vs AI" button click event, initiating a game mode where a human plays against an AI.
     */
    void on_buttonManAI_clicked();

    /**
     * @brief Slot that handles the "Man vs Man" button click event, initiating a game mode where two humans play against each other.
     */
    void on_buttonManMan_clicked();

    /**
     * @brief Slot that handles the "AI vs AI with Random Moves" button click event, initiating a game mode where two AIs play with random moves.
     */
    void on_buttonAIAIRM_clicked();

    /**
     * @brief Slot that handles the "AI vs AI with Weighted Strategy" button click event, initiating a game mode where two AIs play with a weighted strategy.
     */
    void on_buttonAIAIWS_clicked();

    /**
     * @brief Slot that handles the "Exit" button click event, closing the application.
     */
    void on_buttonExit_clicked();

public:
    Ui::MainWindow *ui; ///< Pointer to the UI elements of the MainWindow class.
    Actions *act; ///< Pointer to an Actions object, allowing interaction with actions related to the main window.

};

#endif // MAINWINDOW_H
