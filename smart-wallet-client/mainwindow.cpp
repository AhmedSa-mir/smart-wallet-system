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
        Request request;
        Response response;

        request.type = LOGIN;
        strcpy(request.data, national_id.c_str());
        request.size = strlen(request.data);

        client.sendRequest(request);
        client.recvResponse(response);

        if(response.status == INVALID_ID)
        {
             ui->statusBar->showMessage("Server response: Invalid ID!");
        }
        else if(response.status == FAIL)
        {
             ui->statusBar->showMessage("Server response: Fail!");
        }
        else if(response.status == SUCCESS)
        {
            std::string name(response.data);
            ui->welcome_label->setText(QString::fromStdString("Welcome " + name));

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
        Request request;
        Response response;

        request.type = REGISTER;
        std::string msg_content = name + " " + national_id + " " + age;
        strcpy(request.data, msg_content.c_str());
        request.size = strlen(request.data);

        client.sendRequest(request);
        client.recvResponse(response);

        if(response.status == INVALID_ID)
        {
            ui->statusBar->showMessage("Server response: Invalid ID!");
        }
        else if(response.status == INVALID_AGE)
        {
            ui->statusBar->showMessage("Server response: Invalid age!");
        }
        else if(response.status == FAIL)
        {
            ui->statusBar->showMessage("Server response: Fail!");
        }
        else if(response.status == SUCCESS)
        {
            std::string name(response.data);
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

    Request request;
    Response response;

    request.type = CHECK_BALANCE;
    memset(request.data, 0, MSG_MAX_SIZE);
    request.size = 0;

    client.sendRequest(request);
    client.recvResponse(response);

    if(response.status == FAIL)
    {
        std::cout << "Server response: Fail!";
    }
    else if(response.status == SUCCESS)
    {
        std::string balance(response.data);
        ui->response_label->setText(QString::fromStdString("Your current balance is " + balance));

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
        Request request;
        Response response;

        request.type = client.getRequestType();
        strcpy(request.data, std::to_string(amount).c_str());
        request.size = strlen(request.data);

        client.sendRequest(request);
        client.recvResponse(response);

        if(response.status == NOT_ENOUGH_MONEY)
        {
            ui->response_label->setText("Not enough money!");
            ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);
        }
        else if(response.status == INVALID_AMOUNT)
        {
            ui->response_label->setText("Server response: Invalid amount!");
            ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);
        }
        else if(response.status == FAIL)
        {
            ui->response_label->setText("Server response: Fail!");
            ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);
        }
        else if(response.status == SUCCESS)
        {
            ui->response_label->setText("Transaction done successfully");
            ui->stackedWidget->setCurrentIndex(RESPONSE_PAGE);

            client.pushUndoStack(request);
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

    Request request = client.popUndoStack();
    Response response;

    if(request.type == DEPOSIT)
    {
        request.type = WITHDRAW;
    }
    else if(request.type == WITHDRAW)
    {
        request.type = DEPOSIT;
    }

    client.sendRequest(request);
    client.recvResponse(response);

    if(response.status == FAIL)
    {
        ui->statusBar->showMessage("Server response: Fail!");
        client.pushUndoStack(request);    // return it into the stack again
    }
    else if(response.status == SUCCESS)
    {
        ui->statusBar->showMessage("Undo last transaction: success!");

        if(client.isUndoStackEmpty())
        {
            ui->undo_button->setDisabled(true);
        }

        client.pushRedoStack(request);
        if(!ui->redo_button->isEnabled())
        {
            ui->redo_button->setEnabled(true);
        }
    }
}

void MainWindow::on_redo_button_clicked()
{
    ui->statusBar->clearMessage();

    Request request = client.popRedoStack();
    Response response;

    if(request.type == DEPOSIT)
    {
        request.type = WITHDRAW;
    }
    else if(request.type == WITHDRAW)
    {
        request.type = DEPOSIT;
    }

    client.sendRequest(request);
    client.recvResponse(response);

    if(response.status == FAIL)
    {
        ui->statusBar->showMessage("Server response: Fail!");
        client.pushRedoStack(request);    // return it into the stack again
    }
    else if(response.status == SUCCESS)
    {
        ui->statusBar->showMessage("Redo last transaction: success!");

        if(client.isRedoStackEmpty())
        {
            ui->redo_button->setDisabled(true);
        }
    }
}

void MainWindow::on_exit_button_clicked()
{
    Request request;

    request.type = BYE;
    memset(request.data, 0, MSG_MAX_SIZE);
    request.size = 0;

    client.sendRequest(request);

    this->close();
}
