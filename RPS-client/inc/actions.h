#ifndef ACTIONS_H
#define ACTIONS_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "result.h"

namespace Ui {
class Actions;
}

/**
 * @class Actions
 * @brief The Actions class represents the main window where users can perform specific actions, such as saving data to a file, creating new entries, or loading existing ones.
 *
 * This class provides functionality for interacting with the user interface and handling file operations.
 */
class Actions : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs an Actions object.
     * @param parent The parent widget of this window, typically nullptr if no parent is specified.
     */
    explicit Actions(QWidget *parent = nullptr);

    /**
     * @brief Destroys the Actions object, releasing any allocated resources.
     */
    ~Actions();

    /**
     * @brief Writes data to a specified file.
     * @param fileName The name of the file to write to.
     * @param data The content to be written into the file.
     *
     * This function attempts to open the file in write mode and write the provided data into it.
     */
    void writeToFile(const QString &fileName, const QString &data);

public:
    Ui::Actions *ui; ///< Pointer to the UI elements of the Actions class.
    Result *res; ///< Pointer to the Result object used in this class for managing results.

signals:
    /**
     * @brief Signal emitted to indicate an action has occurred.
     */
    void sigAct();

    /**
     * @brief Signal emitted to set or update the score.
     */
    void sigSetScore();

public slots:
    /**
     * @brief Slot that handles the "Back" button click event.
     */
    void on_buttonBack_clicked();

    /**
     * @brief Slot that handles the "New" button click event, initiating the process for a new entry.
     */
    void on_buttonNew_clicked();

    /**
     * @brief Slot that handles the "Load" button click event, initiating the process to load existing data.
     */
    void on_buttonLoad_clicked();
};

#endif // ACTIONS_H
