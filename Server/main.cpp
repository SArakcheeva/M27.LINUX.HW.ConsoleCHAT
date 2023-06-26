#include <fstream>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>

#include "NetworkConfig.h"
#include "DataBase.h"

using namespace std;

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

struct sockaddr_in serveraddress, client;
socklen_t length;
int sockert_file_descriptor, connection, bind_status, connection_status;
char messageCS[MESSAGE_LENGTH];


std::vector<std::string> split(const std::string & s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}


void fn_socket()
{    
    DataBase database;    

    // PORT number from network.config file
	NetworkConfig config;
	int PORT = config.getPort();

    // Создадим сокет
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        std::cout << "Socket creation failed.!" << std::endl;
        exit(1);
    }

    // Преобразуем в нужный формат
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);

    // Зададим номер порта для связи
    serveraddress.sin_port = htons(PORT);

    // Используем IPv4
    serveraddress.sin_family = AF_INET;

    // Привяжем сокет
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress,
        sizeof(serveraddress));
    if (bind_status == -1) {
        std::cout << "Socket binding failed.!" << std::endl;
        exit(1);
    }

    // Поставим сервер на прием данных 
    connection_status = listen(sockert_file_descriptor, 5);
    if (connection_status == -1) {
        std::cout << "Socket is unable to listen for new connections.!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Server is listening for new connection: " << std::endl;
    }

    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);
    if (connection == -1) {
        std::cout << "Server is unable to accept the data from client.!" << std::endl;
        exit(1);
    }

    // Communication Establishment
    while (1) {
        bzero(messageCS, MESSAGE_LENGTH);
        read(connection, messageCS, sizeof(messageCS));
        if (strncmp("end", messageCS, 3) == 0) {
            std::cout << "Client Exited." << std::endl;
            std::cout << "Server is Exiting..!" << std::endl;
            break;
        }
        std::cout << "Data received from client: " << messageCS << std::endl;

        
        try {
            std::vector<std::string> vector_messageCS = split(std::string(messageCS), '|');
            bzero(messageCS, MESSAGE_LENGTH);
            
            if(vector_messageCS.size() < 1) {
                std::cout << "Incorrect request from Client." << std::endl;
            }
            else {

                int id_cmd = atoi( vector_messageCS[0].c_str() );
                switch(id_cmd)
                {
                    case 0: {/*Проверить есть ли в базе заданный Логин*/
                        bool _isExistLogin = database.isExistLogin(vector_messageCS[1]);//true->ok
                        strcpy(messageCS, std::to_string(_isExistLogin).c_str());
                        break;
                    }
                    case 1: {/*Проверить соответствует ли Пароль заданному Логину*/
                        bool _isCorrectPassword = database.isCorrectPassword(vector_messageCS[1], vector_messageCS[2]);
                        std::cout << messageCS << std::endl;
                        strcpy(messageCS, std::to_string(_isCorrectPassword).c_str());
                        break;
                    }
                    case 2: {/*Проверить есть ли в базе заданный Ник*/
                        bool _isExistName = database.isExistName(vector_messageCS[1]);
                        strcpy(messageCS, std::to_string(_isExistName).c_str());
                        break;
                    }
                    case 3: {/*Добавить в базу заданного пользователя*/
                    	User new_user(vector_messageCS[3], vector_messageCS[1], vector_messageCS[2]);
                        bool res3 = database.addUser(new_user);
                        strcpy(messageCS, std::to_string(res3).c_str());
                        break;
                    }
                    case 4: {/*Удалить заданного пользователя из базы*/
                        User del_user(vector_messageCS[3], vector_messageCS[1], vector_messageCS[2]);
                        database.removeUser(del_user);
                        strcpy(messageCS, "1");
                        break;
                    }
                    case 5: {/*Количество зарегистрированных пользователей*/
                        int _numberUser = 0;
                        _numberUser = database.getNumberUser();
                        strcpy(messageCS, std::to_string(_numberUser).c_str());
                        break;
                    }
                    case 6: {/*Загрузить имена зарегистрированных пользователей*/
                        std::map<int, std::string> userNames = database.loadUserNames();
                        std::string allUsers;
                        for (auto name : userNames) {
                            allUsers = allUsers + name.second + "|";
                        }
                        strcpy(messageCS, allUsers.c_str());
                        break;
                    }
                    case 7: {/*Загрузить сообщения, адресованные заданному пользователю*/
                        User mess_user(vector_messageCS[3], vector_messageCS[1], vector_messageCS[2]);
                        std::vector<Message> messagesToUser = database.loadMessages(mess_user);              //Заполнить вектор - сообщениями пользователю
                        std::string user_messages;
                        if (messagesToUser.empty()) {
                            user_messages = "There are no messages for you.";
                        }
                        else {                            
                            for (auto message : messagesToUser) {
                                user_messages = message.getNameFrom() + " to " + message.getNameTo() + " [" + message.getSendTime() + "] :  " + message.getText() + "||";
                            }
                        }
                        strcpy(messageCS, user_messages.c_str());
                        break;
                    }
                    case 8: {/*Добавить в базу сообщение от одного пользователя другому*/
                        Message message(vector_messageCS[1], vector_messageCS[2], vector_messageCS[3], vector_messageCS[4]);	//Создать сообщение
                        bool res8 = false;
                        if(message.getNameTo() == "all")				//Поместить в базу сообщений
                            res8 = database.addMessageForAllUsers(message);
                        else
                            res8 = database.addMessageForUser(message);
                        std::string res8_str = res8 ? "1" : "0";
                        strcpy(messageCS, res8_str.c_str());                                            
                        break;
                    }
                    case 9: {/*Загрузить из базы Ник пользователя*/
                        const std::string name = database.getNameByLogin(vector_messageCS[1]);
                        strcpy(messageCS, name.c_str());
                        break;
                    }
                    default: 
                        break;
                }    
            }   

	    }
        catch (...) {
		    std::cerr << "Exception: incorrect data!" << std::endl;
            bzero(messageCS, MESSAGE_LENGTH);
            strcpy(messageCS, "0");
	    }

        ssize_t bytes = write(connection, messageCS, sizeof(messageCS));
        // Если передали >= 0  байт, значит пересылка прошла успешно
        if (bytes >= 0) {
            cout << messageCS << " Data successfully sent to the client.!" << endl;
        }
    }

    // закрываем сокет, завершаем соединение
    close(sockert_file_descriptor);
}


int main()
{    
    fn_socket();
    
    return 0;
}