#include <iostream>
#include <string>

#include "Chat.h"
#include "NetworkConfig.h"
#include "Socket.h"

using namespace std;

int socket_file_descriptor, connection;
struct sockaddr_in serveraddress, client;
char messageCS[MESSAGE_LENGTH];

int main()
{
	try {
        // PORT and IP from file network.config 
        NetworkConfig config;
	    int PORT = config.getPort();
        std::string IP = config.getIP();
        IP.erase(IP.find(' '), 1);
        std::cout << IP << std::endl;

        // Создадим сокет
        socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_file_descriptor == -1)
        {
            std::cout << "Creation of Socket failed!" << std::endl;
            exit(1);
        }

        // Установим адрес сервера
        serveraddress.sin_addr.s_addr = inet_addr(IP.c_str());
        // Зададим номер порта
        serveraddress.sin_port = htons(PORT);
        // Используем IPv4
        serveraddress.sin_family = AF_INET;
        // Установим соединение с сервером
        connection = connect(socket_file_descriptor, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
        if (connection == -1)
        {
            std::cout << "Connection with the server failed.!" << std::endl;
            exit(1);
        }


        // Запустим чат
		std::shared_ptr<bool> isRun = std::make_shared<bool>(true);
		Chat::getInstance()->attach(isRun);

		while (*isRun) {
			Chat::getInstance()->process();
		}
	}
	catch (std::bad_alloc& error) {
		std::cerr << "Ошибка выделения памяти: " << error.what() << std::endl;
	}
	catch (std::exception& error) {
		std::cerr << error.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Неопределённое исключение" << std::endl;
	}

    const std::string request = "end";
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

    // закрываем сокет, завершаем соединение
    close(socket_file_descriptor);

	return EXIT_SUCCESS;
}