#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(std::string server_ip, int port, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    client(port, server_ip)
{
    ui->setupUi(this);
    this->setWindowTitle("Smart Wallet");
    ui->statusBar->setStyleSheet("color: red");
    ui->stackedWidget->setCurrentIndex(LOGIN_PAGE);
}

MainWindow::~MainWindow()
{
    delete ui;

    client.closeSockfd();
}

int MainWindow::runClient()
{
    int ret = client.createSocket();
    if(ret == -1)
    {
        std::cout << "Error on create socket\n";
        return -1;
    }

    int sockfd = client.connectToServer();
    if(sockfd < 0)
    {
        std::cout << "Error on connect\n";
        return -1;
    }

    return 0;
}

void MainWindow::on_newuser_button_clicked()
{
    ui->statusBar->clearMessage();

    ui->stackedWidget->setCurrentIndex(REGISTER_PAGE);
}

void MainWindow::on_login_button_clicked()
{
    ui->statusBar->clearMessage();

    std::string national_id = ui->input_id->text().toStdString();

    if(!client.isValidId(national_id))
    {
        ui->statusBar->showMessage("Invalid id. Please enter 14 digits");
    }
    else
    {
        std::string response_msg;

        RESPONSE_STATUS status = client.login(national_id, response_msg);
        if(status == INVALID_ID || status == FAIL)
        {
             ui->statusBar->showMessage(QString::fromStdString(response_msg));
        }
        else if(status == SUCCESS)
        {
            ui->welcome_label->setText(QString::fromStdString("Welcome " + response_msg));
            ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
        }
    }

}

void MainWindow::on_register_button_clicked()
{
    ui->statusBar->clearMessage();

    std::string name = ui->input_name->text().toStdString();
    std::string national_id = ui->input_id_2->text().toStdString();
    std::string age = ui->input_age->text().toStdString();    
    std::string gender = ui->gender_combox->currentText().toStdString();

    std::cout << "national_id= " << national_id << std::endl;

    if(name.empty())
    {
        ui->statusBar->showMessage("Please enter your name");
    }
    else if(national_id.empty())
    {
        ui->statusBar->showMessage("Please enter your national id");
    }
    else if(!client.isValidId(national_id))
    {
        ui->statusBar->showMessage("Invalid id. Please enter 14 digits");
    }
    else if(age.empty())
    {
        ui->statusBar->showMessage("Please enter your age");
    }
    else if(!client.isValidAge(std::stoi(age)))
    {
        ui->statusBar->showMessage("Invalid age. (18 - 200)");
    }
    else
    {
        RESPONSE_STATUS status = client.register_new_client(name, national_id, age, gender);
        if(status == INVALID_ID)
        {
            ui->statusBar->showMessage("Server response: Invalid ID!");
        }
        else if(status == INVALID_AGE)
        {
            ui->statusBar->showMessage("Server response: Invalid age!");
        }
        else if(status == FAIL)
        {
            ui->statusBar->showMessage("Server response: Failed!");
        }
        else if(status == SUCCESS)
        {
            ui->welcome_label->setText(QString::fromStdString("Welcome " + name));
            ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
        }
    }
}

void MainWindow::on_deposit_button_clicked()
{
    ui->statusBar->clearMessage();

    client.setRequestType(DEPOSIT);
    ui->stackedWidget->setCurrentIndex(AMOUNT_PAGE);
}

void MainWindow::on_withdraw_button_clicked()
{
    ui->statusBar->clearMessage();

    client.setRequestType(WITHDRAW);
    ui->stackedWidget->setCurrentIndex(AMOUNT_PAGE);
}

void MainWindow::on_checkbalance_button_clicked()
{
    ui->statusBar->clearMessage();

    unsigned long long balance = 0;

    RESPONSE_STATUS status = client.checkBalance(balance);
    if(status == FAIL)
    {
        ui->response_label->setText("Server response: Failed!");
        ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);
    }
    else if(status == SUCCESS)
    {
        ui->response_label->setText(QString::fromStdString(
                                    "Your current balance is " + std::to_string(balance)));
        ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);
    }
}

void MainWindow::on_ok_button_clicked()
{
    ui->statusBar->clearMessage();

    long long amount = ui->input_amount->text().toLongLong();

    if (amount <= 0)
    {
        ui->statusBar->showMessage("Please enter a valid amount");
    }
    else
    {
        RESPONSE_STATUS status = client.sendTransaction(amount);
        if(status == NOT_ENOUGH_MONEY)
        {
            ui->response_label->setText("Transaction Failed: Not enough money!");
            ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);
        }
        else if(status == INVALID_AMOUNT)
        {
            ui->response_label->setText("Server response: Invalid amount!");
            ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);
        }
        else if(status == FAIL)
        {
            ui->response_label->setText("Server response: Failed!");
            ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);
        }
        else if(status == SUCCESS)
        {
            ui->response_label->setText("Transaction done successfully");
            ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);

            ui->undo_button->setEnabled(true);
        }

        ui->input_amount->clear();
    }
}

void MainWindow::on_ok_button_2_clicked()
{
    ui->statusBar->clearMessage();
    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}

void MainWindow::on_undo_button_clicked()
{
    ui->statusBar->clearMessage();

    unsigned long long amount = 0;
    bool empty_stack = false;
    REQUEST_TYPE type;

    RESPONSE_STATUS status = client.undo(amount, empty_stack, type);
    if(status == FAIL)
    {
        ui->statusBar->showMessage("Server response: Failed!");
    }
    else if(status == SUCCESS)
    {
        if(type == DEPOSIT)
        {
            ui->statusBar->showMessage(QString::fromStdString(
                           "Undo last transaction (DEPOSIT " + std::to_string(amount) + "$): success!"));
        }
        else if(type == WITHDRAW)
        {
            ui->statusBar->showMessage(QString::fromStdString(
                           "Undo last transaction (WITHDRAW " + std::to_string(amount) + "$): success!"));
        }

        if(empty_stack)
        {
            ui->undo_button->setDisabled(true);
        }

        if(!ui->redo_button->isEnabled())
        {
            ui->redo_button->setEnabled(true);
        }
    }
}

void MainWindow::on_redo_button_clicked()
{
    ui->statusBar->clearMessage();
    unsigned long long amount = 0;
    bool empty_stack = false;
    REQUEST_TYPE type;

    RESPONSE_STATUS status = client.redo(amount, empty_stack, type);

    if(status == FAIL)
    {
        ui->statusBar->showMessage("Server response: Failed!");
    }
    else if(status == SUCCESS)
    {
        if(type == DEPOSIT)
        {
            ui->statusBar->showMessage(QString::fromStdString(
                           "Redo last transaction (DEPOSIT " + std::to_string(amount) + "$): success!"));
        }
        else if(type == WITHDRAW)
        {
            ui->statusBar->showMessage(QString::fromStdString(
                           "Redo last transaction (WITHDRAW " + std::to_string(amount) + "$): success!"));
        }

        if(empty_stack)
        {
            ui->redo_button->setDisabled(true);
        }
    }
}

void MainWindow::on_exit_button_clicked()
{
    client.bye();

    this->close();
}

void MainWindow::on_statistics_button_clicked()
{
    QVector<double> x(12), y(12);   // plot data

    std::string month_rates;
    RESPONSE_STATUS status = client.getStats(month_rates);

    if(status == FAIL)
    {
        ui->statusBar->showMessage("Server response: Failed!");
    }
    else if(status == SUCCESS)
    {
        // Split data by spaces
        std::istringstream iss(month_rates);
        std::string month, amount;
        for (int i=0; i<12; ++i)
        {
          std::getline(iss, month, ' ');
          std::getline(iss, amount, ' ');
          x[i] = QCPAxisTickerDateTime::dateTimeToKey(QDate(1970, std::stoi(month), 1));
          y[i] = std::stoi(amount);
        }

        QCPTextElement *title = new QCPTextElement(ui->custom_plot);
        title->setText("Your Withdrawal Rate throughout the Year");
        title->setFont(QFont("sans", 12, QFont::Bold));
        ui->custom_plot->plotLayout()->insertRow(0);
        ui->custom_plot->plotLayout()->addElement(0,0, title);

        ui->custom_plot->yAxis->setRange(0, 60000);

        QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
        ui->custom_plot->xAxis->setTicker(dateTimeTicker);

        ui->custom_plot->xAxis->setRange(QCPAxisTickerDateTime::dateTimeToKey(QDate(1970, 1, 1)), QCPAxisTickerDateTime::dateTimeToKey(QDate(1970, 12, 1)));
        dateTimeTicker->setDateTimeFormat("MMM");

        // create graph and assign data to it
        ui->custom_plot->addGraph();
        ui->custom_plot->graph(0)->setData(x, y);

        QPen blueDotPen;
        blueDotPen.setColor(QColor(30, 40, 255, 150));
        blueDotPen.setStyle(Qt::DotLine);
        blueDotPen.setWidthF(4);
        ui->custom_plot->graph()->setPen(blueDotPen);

        ui->custom_plot->xAxis->ticker()->setTickCount(12);
        ui->custom_plot->yAxis->ticker()->setTickCount(6);

        ui->custom_plot->xAxis->setLabel("Month");
        ui->custom_plot->yAxis->setLabel("Withdrawal rate");

        ui->custom_plot->replot();

        ui->stackedWidget->setCurrentIndex(STATISTICS_PAGE);
    }
}

void MainWindow::on_back_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}
