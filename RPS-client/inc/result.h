#ifndef RESULT_H
#define RESULT_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>
#include <QCoreApplication>
#include "choice.h"

namespace Ui {
class Result;
}

/**
 * @class Result
 * @brief The Result class manages the results display, communication with an external device, and file operations.
 *
 * This class allows for reading and writing files, managing serial communication, and handling XML messages.
 */
class Result : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Result object.
     * @param parent The parent widget of this window, typically nullptr if no parent is specified.
     */
    explicit Result(QWidget *parent = nullptr);

    /**
     * @brief Destroys the Result object, releasing any allocated resources.
     */
    ~Result();

    QString gameMode_local; ///< Stores the current game mode as a QString.
    QString score_local; ///< Stores the current score as a QString.
    QString xmlFilePath; ///< Path to the XML file used for communication.
    QSerialPort *serialPort; ///< Pointer to the QSerialPort for serial communication.
    QByteArray incomingData; ///< Stores incoming data from the serial port.
    Choice *cho; ///< Pointer to the Choice object for managing choices in the game.

    /**
     * @brief Reads data from a specified file.
     * @param fileName The name of the file to read from.
     * @return The content of the file as a QString.
     */
    QString readFromFile(const QString &fileName);

    /**
     * @brief Writes data to a specified file.
     * @param fileName The name of the file to write to.
     * @param data The content to be written into the file.
     */
    void writeToFile(const QString &fileName, const QString &data);

    /**
     * @brief Sets the XML message with specified game parameters.
     * @param filePath The path of the XML file.
     * @param gameMode_local The current game mode.
     * @param choice1 Player 1's choice.
     * @param choice2 Player 2's choice.
     *
     * This function prepares an XML message with game details for communication.
     */
    void setXmlMessage(const QString &filePath, QString gameMode_local, QString choice1, QString choice2);

    /**
     * @brief Sends an XML message to an ESP device.
     * @param xmlFilePath The path of the XML file to send.
     */
    void sendXmlToESP(const QString &xmlFilePath);

    /**
     * @brief Receives data from the ESP device via serial port.
     */
    void receiveDataFromESP();

    /**
     * @brief Retrieves the XML message received from the ESP device.
     */
    void getXmlMessage();

    int choice1; ///< Choice made by Player 1.
    int choice2; ///< Choice made by Player 2.

    /**
     * @brief Sets the choice for Player 1.
     */
    void setChoice1();

    /**
     * @brief Sets the choice for Player 2.
     */
    void setChoice2();

private:
    Ui::Result *ui; ///< Pointer to the UI elements of the Result class.

signals:
    /**
     * @brief Signal emitted to indicate a result action has occurred.
     */
    void sigRes();

    /**
     * @brief Signal emitted to pass the result to the Choice interface.
     */
    void sigResToChoice();

    /**
     * @brief Signal emitted to display the current game mode.
     * @param gameMode The current game mode to display.
     */
    void sigShowGameMode(int gameMode);

private slots:
    /**
     * @brief Slot that handles the "Back" button click event.
     */
    void on_buttonBack_clicked();

    /**
     * @brief Slot that handles the "Next" button click event.
     */
    void on_buttonNext_clicked();

    /**
     * @brief Slot that handles the "Save" button click event, saving the current results.
     */
    void on_buttonSave_clicked();

public slots:
    /**
     * @brief Sets the current score.
     */
    void setScore();
};

#endif // RESULT_H
