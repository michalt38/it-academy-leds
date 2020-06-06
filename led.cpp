#include "led.h"
#include "ui_led.h"

#include <QMessageBox>

LED::LED(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LED)
{
    ui->setupUi(this);
    arduino = new QSerialPort();

    for(auto serialPort: QSerialPortInfo::availablePorts())
    {
        ui->portsComboBox->addItem(serialPort.portName());
    }

    ui->portsComboBox->model()->sort(0);

    connect(ui->connectionButton, &QPushButton::clicked, this, &LED::connectArduino);

    connect(ui->redDial, &QDial::valueChanged, this, &LED::onRedDialValueChanged);
    connect(ui->greenDial, &QDial::valueChanged, this, &LED::onGreenDialValueChanged);
    connect(ui->blueDial, &QDial::valueChanged, this, &LED::onBlueDialValueChanged);
}

void LED::connectArduino()
{
    if(!arduino->isOpen())
    {
        arduino->setPortName(ui->portsComboBox->currentText());
        if(!arduino->open(QSerialPort::ReadWrite))
        {
            QMessageBox::critical(this, "Error", "Could not open serial port!");
            return;
        }

        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);

        ui->connectionButton->setText("Disconnect");
        ui->connectionStatusLabel->setText("Connected");

        ui->dialgroupBox->setEnabled(true);
    }
    else
    {
        arduino->close();

        ui->connectionButton->setText("Connect");
        ui->connectionStatusLabel->setText("Disconnected");

        ui->dialgroupBox->setDisabled(true);
    }
}

void LED::onBlueDialValueChanged(int value)
{
    setLed(LedColor::Blue, value);
}

void LED::onGreenDialValueChanged(int value)
{
    setLed(LedColor::Green, value);
}

void LED::onRedDialValueChanged(int value)
{
    setLed(LedColor::Red, value);
}

void LED::setLed(LedColor color, int value)
{
    if(!arduino->isWritable())
    {
        QMessageBox::critical(this, "Error", "Could not write to serial port!");
        return;
    }

    QString ledName;
    switch(color)
    {
    case LedColor::Red:
        ledName = "RED";
        break;
    case LedColor::Green:
        ledName = "GREEN";
        break;
    case LedColor::Blue:
        ledName = "BLUE";
        break;
    }

    arduino->write(QString(ledName + " " + QString::number(value) + "\n").toStdString().c_str());
}

LED::~LED()
{
    delete ui;
}
