#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Поиск доступных COM портов
    ui->COMportSelection->addItem("Список COM портов");

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
            ui->COMportSelection->addItem(serialPortInfo.portName());
    }

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_UpdateCOMPorts_clicked()
{
    ui->COMportSelection->clear();
    ui->COMportSelection->addItem("Список COM портов");
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->COMportSelection->addItem(serialPortInfo.portName());
    }
}


void MainWindow::on_ButtonConnect_clicked()
{

    serialPort.setPortName(this->ui->COMportSelection->currentText());
    serialPort.setBaudRate(QSerialPort::Baud115200);
    // Открываем порт
    if (!serialPort.open(QIODevice::ReadWrite)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось подключится к порту.\n Проверьте выбранный порт.");
        return;
    }
    // Включаем режим настройки на устройстве
    serialPort.write("\'");
    serialPort.waitForBytesWritten();
    QString buffer;
    while (serialPort.waitForReadyRead(100)) {
        buffer.append(serialPort.readAll());
    }
    ui->debugWindow->appendPlainText(buffer);
    // Закрываем порт
    serialPort.close();
}


void MainWindow::on_TokenIntruction_clicked()
{
    instform.setModal(true);
    instform.exec();
}


void MainWindow::on_SetupDevice_clicked()
{
    serialPort.setBaudRate(QSerialPort::Baud115200);
    // Открываем порт
    if (!serialPort.open(QIODevice::ReadWrite)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось подключится к порту.\n Проверьте выбранный порт.");
        return;
    } else {
    // Открытие окна с параметрами настройки
    setform.setModal(true);
    setform.exec();
    // Чтение вывода устроства (об изменении параметров)
    QString buffer;
    while (serialPort.waitForReadyRead(7)) {
        buffer.append(serialPort.readAll());
    }

    ui->debugWindow->appendPlainText(buffer);
    // Закрытие порта
    serialPort.close();
    }
}


void MainWindow::on_ReturnDefaultSettings_clicked()
{
    serialPort.setBaudRate(QSerialPort::Baud115200);
    // Открываем порт
    if (!serialPort.open(QIODevice::ReadWrite)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось подключится к порту.\n Проверьте выбранный порт.");
        return;
    }
    // Включаем режим: настроки по умолчанию
    serialPort.write("\n");
    serialPort.waitForBytesWritten();
    // Чтение вывода устроства (об изменении параметров)
    QString buffer;
    while (serialPort.waitForReadyRead(7)) {
        buffer.append(serialPort.readAll());
    }

    ui->debugWindow->appendPlainText(buffer);
    // Закрываем порт
    serialPort.close();
}

