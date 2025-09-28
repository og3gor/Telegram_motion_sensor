#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <dialoginstruction.h>
#include <dialogsettings.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_UpdateCOMPorts_clicked();
    void on_ButtonConnect_clicked();

    void on_TokenIntruction_clicked();

    void on_SetupDevice_clicked();

    void on_ReturnDefaultSettings_clicked();

private:
    Ui::MainWindow *ui;
    DialogInstruction instform;
    DialogSettings setform;


};
#endif // MAINWINDOW_H
