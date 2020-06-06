#ifndef LED_H
#define LED_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class LED; }
QT_END_NAMESPACE

enum class LedColor {Red, Green, Blue};

class LED : public QMainWindow
{
    Q_OBJECT

public:
    LED(QWidget *parent = nullptr);
    ~LED();

private slots:
    void connectArduino();
    void onBlueDialValueChanged(int value);
    void onGreenDialValueChanged(int value);
    void onRedDialValueChanged(int value);

private:
    void setLed(LedColor color, int value);

    Ui::LED *ui;
    QSerialPort *arduino;
};
#endif // LED_H
