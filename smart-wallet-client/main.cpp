#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Server IP & Port arguments required
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <IP> <port>\n";
        return -1;
    }

    // Get IP and port number (passed as arguments)
    std::string server_ip(argv[1]);
    int port = std::stoi(argv[2]);

    QApplication a(argc, argv);
    MainWindow w(server_ip, port);

    int ret = w.runClient();
    if(ret != 0)
    {
        std::cout << "Error on client connection setup\n";
        return -1;
    }

    w.show();

    return a.exec();
}
