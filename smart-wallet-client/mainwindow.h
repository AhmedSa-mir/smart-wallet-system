#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "client.h"

enum PAGE_INDEX {LOGIN_PAGE, REGISTER_PAGE, MAIN_PAGE, STATISTICS_PAGE, AMOUNT_PAGE, RESPONSE_PAGE};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::string server_ip, int port, QWidget *parent = 0);
    ~MainWindow();
    int runClient();

private slots:
    void on_newuser_button_clicked();

    void on_login_button_clicked();

    void on_register_button_clicked();

    void on_deposit_button_clicked();

    void on_withdraw_button_clicked();

    void on_checkbalance_button_clicked();

    void on_ok_button_clicked();

    void on_ok_button_2_clicked();

    void on_undo_button_clicked();

    void on_redo_button_clicked();

    void on_exit_button_clicked();

    void on_statistics_button_clicked();

    void on_back_button_clicked();

    void on_back_button_2_clicked();

    void on_back_button_3_clicked();

private:
    Ui::MainWindow *ui;
    Client client;
};

#endif // MAINWINDOW_H
