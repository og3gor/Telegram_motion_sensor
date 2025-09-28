#include "dialogsettings.h"
#include "ui_dialogsettings.h"


DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}


void DialogSettings::on_ButtonDownload_clicked()
{
    // В ранее окрытый порт записываем данные
    QString WIFI = ui->plainTextWIFI->toPlainText();
    QString Pass = ui->plainTextPass->toPlainText();
    QString Token = ui->plainTextToken->toPlainText();
    QString Id = ui->plainTextEditChatId->toPlainText();

    if (WIFI.isEmpty() || Pass.isEmpty() || Token.isEmpty() || Id.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Одно или несколько полей пусты. Введите значения во все поля.");
        return;
    } else {
        // Получаем строки в формате UTF-8
        QByteArray wifiBytes = WIFI.toUtf8();
        QByteArray passBytes = Pass.toUtf8();
        QByteArray tokenBytes = Token.toUtf8();
        QByteArray idBytes = Id.toUtf8();

        // Отправляем строки посимвольно или побайтово
        serialPort.write(wifiBytes);
        serialPort.waitForBytesWritten();
        serialPort.write("\r");
        serialPort.waitForBytesWritten();
        serialPort.write(passBytes);
        serialPort.waitForBytesWritten();
        serialPort.write("\r");
        serialPort.waitForBytesWritten();
        serialPort.write(tokenBytes);
        serialPort.waitForBytesWritten();
        serialPort.write("\r");
        serialPort.waitForBytesWritten();
        serialPort.write(idBytes);
        serialPort.waitForBytesWritten();
        serialPort.write("\r");
        serialPort.waitForBytesWritten();
        // Выход из режима настройки
        serialPort.write("\t");
        serialPort.waitForBytesWritten();
        }
    close();
}


void DialogSettings::on_pushClose_clicked()
{
    // Выход из режима настройки
    serialPort.write("\t");
    serialPort.waitForBytesWritten();
    close();
}

