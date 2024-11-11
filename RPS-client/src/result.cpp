#include "../inc/result.h"
#include "ui_result.h"
#include "../inc/mainwindow.h"
#include <QMessageBox>

Result::Result(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Result),
    serialPort(new QSerialPort(this))
{
    ui->setupUi(this);

    // signal for tracking data from ESP32
    connect(serialPort, &QSerialPort::readyRead, this, &Result::receiveDataFromESP);

    // find device with vendor id 1A86 - "server"
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.hasVendorIdentifier() && info.vendorIdentifier() == 0x1A86) {
            //QMessageBox::information(this, "About port", "Found ESP on port: " + info.portName());
            serialPort->setPort(info);
        }
    }

    // set parameters for messanging
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite)) {
        //QMessageBox::critical(this, "Error", "Failed to open serial port!");
    }

    xmlFilePath = "../../config.xml";
    writeToFile(xmlFilePath, "<user> <mode>1</mode> <p1>3</p1> <p2>0</p2> <ai1>1</ai1> <ai2>2</ai2> <win>2</win></user>");

    cho = new Choice();
    connect(this, &Result::sigResToChoice, cho, &Choice::show);

    choice1 = 0;
    choice2 = 0;

    connect(cho, &Choice::sigPassCho1, this, &Result::setChoice1);
    connect(cho, &Choice::sigPassCho2, this, &Result::setChoice2);

    connect(this, &Result::sigShowGameMode, cho, &Choice::showGameMode);
}

Result::~Result()
{
    delete ui;
}

void Result::setChoice1() {
    choice1 = cho->choiceP1;
    setXmlMessage(xmlFilePath, gameMode_local, QString::number(choice1), QString::number(choice2));
    sendXmlToESP(xmlFilePath);
}

void Result::setChoice2() {
    choice2 = cho->choiceP2;
    setXmlMessage(xmlFilePath, gameMode_local, QString::number(choice1), QString::number(choice2));
    sendXmlToESP(xmlFilePath);
}

void Result::on_buttonBack_clicked()
{
    this->close();
    emit sigRes();
}

void Result::on_buttonNext_clicked()
{
    gameMode_local = readFromFile("gameMode.txt");
    if (gameMode_local == "1")
    {
        emit sigShowGameMode(gameMode_local.toInt());
    }
    else if (gameMode_local == "2")
    {
        emit sigShowGameMode(gameMode_local.toInt());
    }
    else if (gameMode_local == "3")
    {
        setXmlMessage(xmlFilePath, gameMode_local, "0", "0");
        sendXmlToESP(xmlFilePath);
    }
    else if (gameMode_local == "4")
    {
        setXmlMessage(xmlFilePath, gameMode_local, "0", "0");
        sendXmlToESP(xmlFilePath);
    }
}

QString Result::readFromFile(const QString &fileName) {
    QFile file(fileName);
    QString content;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        content = in.readAll();
        file.close();
    } else {
        QApplication::quit();
    }

    return content;
}

void Result::setScore() {
    QString scoreType = readFromFile("scoreType.txt");
    gameMode_local = readFromFile("gameMode.txt");

    if (scoreType == "1") {
        ui->labelP1score->setText("0");
        ui->labelP2score->setText("0");
    } else if (scoreType == "2") {
        QString scoreFileName = "";
        if (gameMode_local == "1")
            scoreFileName = "../../saved/ManVsAI_score.txt";
        else if (gameMode_local == "2")
            scoreFileName = "../../saved/ManVsMan_score.txt";
        else if (gameMode_local == "3")
            scoreFileName = "../../saved/AI2RM_score.txt";
        else if (gameMode_local == "4")
            scoreFileName = "../../saved/AI2WS_score.txt";

        QString scoreFileContent = readFromFile(scoreFileName);
        QStringList numbers = scoreFileContent.split(" ", Qt::SkipEmptyParts);
        ui->labelP1score->setText(numbers[0]);
        ui->labelP2score->setText(numbers[1]);
    }
}

void Result::writeToFile(const QString &fileName, const QString &data) {
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "Failed to open scoreType.txt");
    }
}

void Result::on_buttonSave_clicked()
{
    QString scoreFileName = "";
    if (gameMode_local == "1")
        scoreFileName = "../../saved/ManVsAI_score.txt";
    else if (gameMode_local == "2")
        scoreFileName = "../../saved/ManVsMan_score.txt";
    else if (gameMode_local == "3")
        scoreFileName = "../../saved/AI2RM_score.txt";
    else if (gameMode_local == "4")
        scoreFileName = "../../saved/AI2WS_score.txt";

    QString p1Score = ui->labelP1score->text();
    QString p2Score = ui->labelP2score->text();

    QString totalScore = p1Score + " " + p2Score;

    writeToFile(scoreFileName, totalScore);
}

void Result::setXmlMessage(const QString &filePath, QString gameMode_local, QString choice1, QString choice2) {
    // open file for reading
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open file for reading!");
        return;
    }

    // get data from xml
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::critical(this, "Error", "Failed to get data from xml!");
        file.close();
        return;
    }
    file.close();

    // find tag "mode" and set text
    QDomElement root = doc.documentElement();
    QDomNodeList mode = root.elementsByTagName("mode");
    if (mode.isEmpty()) {
        QMessageBox::critical(this, "Error", "Mode not found!");
        return;
    }
    QDomElement modeEl = mode.at(0).toElement();
    if (!modeEl.isNull()) {
        modeEl.firstChild().setNodeValue(gameMode_local);
    } else {
        QMessageBox::critical(this, "Error", "Mode is null!");
        return;
    }

    // find tag "p1" and set text
    QDomNodeList p1 = root.elementsByTagName("p1");
    if (p1.isEmpty()) {
        QMessageBox::critical(this, "Error", "P1 not found!");
        return;
    }
    QDomElement p1El = p1.at(0).toElement();
    if (!p1El.isNull()) {
        p1El.firstChild().setNodeValue(choice1);
    } else {
        QMessageBox::critical(this, "Error", "P1 is null!");
        return;
    }

    // find tag "p2" and set text
    QDomNodeList p2 = root.elementsByTagName("p2");
    if (p2.isEmpty()) {
        QMessageBox::critical(this, "Error", "P2 not found!");
        return;
    }
    QDomElement p2El = p2.at(0).toElement();
    if (!p2El.isNull()) {
        p2El.firstChild().setNodeValue(choice2);
    } else {
        QMessageBox::critical(this, "Error", "P2 is null!");
        return;
    }

    // open file for writing
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open file for writing!");
        return;
    }

    // write modified XML to file as a single line
    QTextStream out(&file);
    QString xmlString = doc.toString();
    xmlString.remove(QRegExp("[\\n\\r\\t]"));
    out << xmlString;
    file.close();
}

void Result::sendXmlToESP(const QString &xmlFilePath) {
    // open file
    QFile file(xmlFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open XML file!");
        return;
    }

    // read all data from file
    QByteArray xmlData = file.readAll();
    file.close();

    // send data on serial port
    if (serialPort->isOpen()) {
        serialPort->write(xmlData);
        serialPort->write("\n");
    }
}

void Result::receiveDataFromESP() {
    incomingData += serialPort->readAll();

    // find end of message
    int endIndex = incomingData.indexOf('\n');
    if (endIndex != -1) {
        QString fullMessage = incomingData.left(endIndex + 1);

        QFile file(xmlFilePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << fullMessage;
            file.close();
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to get data!");
        }

        // get just message
        incomingData.remove(0, endIndex + 1);
        getXmlMessage();
    }
}

void Result::getXmlMessage() {
    // open xml files
    QFile file(xmlFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open file!");
    }

    // get content from xml file
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::critical(this, "Error", "Failed to get content from file!");
        file.close();
    }
    file.close();

    // find tag 'mode' and get value
    QDomElement root = doc.documentElement();
    QDomNodeList mode = root.elementsByTagName("mode");
    if (mode.isEmpty()) {
        QMessageBox::critical(this, "Error", "Failed to find tag!");
    }
    QDomElement modeEl = mode.at(0).toElement();
    if (modeEl.isNull()) {
        QMessageBox::critical(this, "Error", "Failed to get value from tag!");
    }

    // find tag 'p1' and get value
    QDomNodeList p1 = root.elementsByTagName("p1");
    if (p1.isEmpty()) {
        QMessageBox::critical(this, "Error", "Failed to find tag!");
    }
    QDomElement p1El = p1.at(0).toElement();
    if (p1El.isNull()) {
        QMessageBox::critical(this, "Error", "Failed to get value from tag!");
    }

    // find tag 'p2' and get value
    QDomNodeList p2 = root.elementsByTagName("p2");
    if (p2.isEmpty()) {
        QMessageBox::critical(this, "Error", "Failed to find tag!");
    }
    QDomElement p2El = p2.at(0).toElement();
    if (p2El.isNull()) {
        QMessageBox::critical(this, "Error", "Failed to get value from tag!");
    }

    // find tag 'ai1' and get value
    QDomNodeList ai1 = root.elementsByTagName("ai1");
    if (ai1.isEmpty()) {
        QMessageBox::critical(this, "Error", "Failed to find tag!");
    }
    QDomElement ai1El = ai1.at(0).toElement();
    if (ai1El.isNull()) {
        QMessageBox::critical(this, "Error", "Failed to get value from tag!");
    }

    // find tag 'ai2' and get value
    QDomNodeList ai2 = root.elementsByTagName("ai2");
    if (ai2.isEmpty()) {
        QMessageBox::critical(this, "Error", "Failed to find tag!");
    }
    QDomElement ai2El = ai2.at(0).toElement();
    if (ai2El.isNull()) {
        QMessageBox::critical(this, "Error", "Failed to get value from tag!");
    }

    // find tag 'win' and get value
    QDomNodeList win = root.elementsByTagName("win");
    if (win.isEmpty()) {
        QMessageBox::critical(this, "Error", "Failed to find tag!");
    }
    QDomElement winEl = win.at(0).toElement();
    if (winEl.isNull()) {
        QMessageBox::critical(this, "Error", "Failed to get value from tag!");
    }

    // process data
    if (modeEl.text() == "1") {
        // man 1
        if (p1El.text() == "1") {
            ui->labelP1pic->setPixmap(QPixmap(":/rock.png"));
            ui->labelP1type->setText("Rock");
        } else if (p1El.text() == "2") {
            ui->labelP1pic->setPixmap(QPixmap(":/paper.png"));
            ui->labelP1type->setText("Paper");
        } else if (p1El.text() == "3") {
            ui->labelP1pic->setPixmap(QPixmap(":/scissors.png"));
            ui->labelP1type->setText("Scissors");
        }

        // ai 1
        if (ai1El.text() == "1") {
            ui->labelP2pic->setPixmap(QPixmap(":/rock.png"));
            ui->labelP2type->setText("Rock");
        } else if (ai1El.text() == "2") {
            ui->labelP2pic->setPixmap(QPixmap(":/paper.png"));
            ui->labelP2type->setText("Paper");
        } else if (ai1El.text() == "3") {
            ui->labelP2pic->setPixmap(QPixmap(":/scissors.png"));
            ui->labelP2type->setText("Scissors");
        }

        // winner and score
        if (winEl.text() == "0") {
            ui->labelResult->setText("Draw");
        } else if (winEl.text() == "1") {
            ui->labelResult->setText("Player1");
            int val = ui->labelP1score->text().toInt();
            val += 1;
            ui->labelP1score->setText(QString::number(val));
        } else if (winEl.text() == "2") {
            ui->labelResult->setText("Player2");
            int val = ui->labelP2score->text().toInt();
            val += 1;
            ui->labelP2score->setText(QString::number(val));
        }
    }
    else if (modeEl.text() == "2") {
        // man 1
        if (p1El.text() == "1") {
            ui->labelP1pic->setPixmap(QPixmap(":/rock.png"));
            ui->labelP1type->setText("Rock");
        } else if (p1El.text() == "2") {
            ui->labelP1pic->setPixmap(QPixmap(":/paper.png"));
            ui->labelP1type->setText("Paper");
        } else if (p1El.text() == "3") {
            ui->labelP1pic->setPixmap(QPixmap(":/scissors.png"));
            ui->labelP1type->setText("Scissors");
        }

        // man 2
        if (p2El.text() == "1") {
            ui->labelP2pic->setPixmap(QPixmap(":/rock.png"));
            ui->labelP2type->setText("Rock");
        } else if (p2El.text() == "2") {
            ui->labelP2pic->setPixmap(QPixmap(":/paper.png"));
            ui->labelP2type->setText("Paper");
        } else if (p2El.text() == "3") {
            ui->labelP2pic->setPixmap(QPixmap(":/scissors.png"));
            ui->labelP2type->setText("Scissors");
        }

        // winner and score
        if (winEl.text() == "0") {
            ui->labelResult->setText("Draw");
        } else if (winEl.text() == "1") {
            ui->labelResult->setText("Player1");
            int val = ui->labelP1score->text().toInt();
            val += 1;
            ui->labelP1score->setText(QString::number(val));
        } else if (winEl.text() == "2") {
            ui->labelResult->setText("Player2");
            int val = ui->labelP2score->text().toInt();
            val += 1;
            ui->labelP2score->setText(QString::number(val));
        }
    }
    else if (modeEl.text() == "3" || modeEl.text() == "4") {
        // ai 1
        if (ai1El.text() == "1") {
            ui->labelP1pic->setPixmap(QPixmap(":/rock.png"));
            ui->labelP1type->setText("Rock");
        } else if (ai1El.text() == "2") {
            ui->labelP1pic->setPixmap(QPixmap(":/paper.png"));
            ui->labelP1type->setText("Paper");
        } else if (ai1El.text() == "3") {
            ui->labelP1pic->setPixmap(QPixmap(":/scissors.png"));
            ui->labelP1type->setText("Scissors");
        }

        // ai 2
        if (ai2El.text() == "1") {
            ui->labelP2pic->setPixmap(QPixmap(":/rock.png"));
            ui->labelP2type->setText("Rock");
        } else if (ai2El.text() == "2") {
            ui->labelP2pic->setPixmap(QPixmap(":/paper.png"));
            ui->labelP2type->setText("Paper");
        } else if (ai2El.text() == "3") {
            ui->labelP2pic->setPixmap(QPixmap(":/scissors.png"));
            ui->labelP2type->setText("Scissors");
        }

        // winner and score
        if (winEl.text() == "0") {
            ui->labelResult->setText("Draw");
        } else if (winEl.text() == "1") {
            ui->labelResult->setText("Player1");
            int val = ui->labelP1score->text().toInt();
            val += 1;
            ui->labelP1score->setText(QString::number(val));
        } else if (winEl.text() == "2") {
            ui->labelResult->setText("Player2");
            int val = ui->labelP2score->text().toInt();
            val += 1;
            ui->labelP2score->setText(QString::number(val));
        }
    }
}
