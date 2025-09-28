#include "dialoginstruction.h"
#include "ui_dialoginstruction.h"

DialogInstruction::DialogInstruction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInstruction)
{
    ui->setupUi(this);

    QString imagePath1 = "D:/Qt/Project/CursachOnESP32/images/telegram1.jpg";
    QString imagePath2 = "D:/Qt/Project/CursachOnESP32/images/telegram2.jpg";
    QString imagePath3 = "D:/Qt/Project/CursachOnESP32/images/botfather3.jpg";
    QString imagePath4 = "D:/Qt/Project/CursachOnESP32/images/botfather4.jpg";
    QString imagePath5 = "D:/Qt/Project/CursachOnESP32/images/botfather5.jpg";
    QString imagePath6 = "D:/Qt/Project/CursachOnESP32/images/Idbot6.jpg";
    QString imagePath7 = "D:/Qt/Project/CursachOnESP32/images/getid7.jpg";


    QString htmlText =
            "<html><head><style>p, li { white-space: pre-wrap; }</style></head><body style='font-family:\"MS Shell Dlg 2\"; font-size:12pt; font-weight:400; font-style:normal; text-align:center;'>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>Зайдите в Google Play или App Store, скачайте и установите Telegram.</p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'><img src='file:///" + imagePath1 + "'></p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>Откройте Telegram и выполните следующие действия, чтобы создать бота Telegram. Сначала найдите «botfather » и щелкните BotFather, как показано ниже.</p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'><img src='file:///" + imagePath2 + "'></p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>В следующем окне вам будет предложено нажать на кнопку /start.</p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'><img src='file:///" + imagePath3 + "'></p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>Введите /newbot и следуйте инструкциям по созданию бота. Дайте ему название и логин.</p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'><img src='file:///" + imagePath4 + "'></p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>Если ваш бот успешно создан, вы получите сообщение со ссылкой для доступа к нему и токеном бота . Сохраните токен бота, он вам понадобится, чтобы обеспечить взаимодействие ESP32 с ботом.</p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'><img src='file:///" + imagePath5 + "'></p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>ОГРАНИЧЬТЕ ДОСТУП К БОТУ</p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'> Любой, кто знает ваш логин бота, может его использовать. Чтобы убедиться, что мы игнорируем сообщения, поступающие не из нашей учетной записи Telegram, вы можете указать свой ID Telegram. В итоге, ESP будет проверять ID перед тем, как обрабатывать сообщение, полученное ботом Telegram. В своем аккаунте Telegram найдите «IDBot» </p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'><img src='file:///" + imagePath6 + "'></p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>Начните чат с этим ботом и введите /getid . Вы получите ответ с вашим идентификатором пользователя. Сохраните этот ID.</p>"
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'><img src='file:///" + imagePath7 + "'></p>"
            "</body></html>";

    ui->textEdit->setHtml(htmlText);
}



DialogInstruction::~DialogInstruction()
{
    delete ui;
}
