#include "EnteringAddressee.h"
#include "Chat.h"
#include <iostream>
#include <memory>
#include <ctime>


EnteringAddressee::EnteringAddressee() : State("EnteringAddressee")
{
};



void EnteringAddressee::handle(Chat& chat)
{
    bool _OK = true;
    std::cout << "Enter Nickname (all - send to all): ";
    std::string nameAdressee;
    std::getline(std::cin >> std::ws, nameAdressee);

    //Зарегистрирован только один пользователь
    //ЗАПРОС СЕРВЕРУ - СКОЛЬКО ПОЛЬЗОВАТЕЛЕЙ В ЧАТЕ
    bzero(messageCS, sizeof(messageCS));
    const std::string request = "5";
    strcpy(messageCS, request.c_str());
    ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Convert answer to int
    unsigned countUsers = 0;
    sscanf(messageCS, "%d", &countUsers);

    //ответ от сервера - число
    if (countUsers == 0) {
        _OK = false;
        std::cout << "Incorrect data from server!\n";
        chat.transitionTo(std::move(std::make_unique<UserInChat>()));
    }
    else if (countUsers == 1) {
        _OK = false;
        std::cout << "You are the only CHAT user!\n";
        chat.transitionTo(std::move(std::make_unique<UserInChat>()));
    }
    else {
        //ЗАПРОС СЕРВЕРУ - ЕСТЬ ЛИ ИМЯ ПОЛУЧАТЕЛЯ В БД
        const std::string request = "2|" + nameAdressee;
        bzero(messageCS, sizeof(messageCS));
        strcpy(messageCS, request.c_str());
        ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

        // Ждем ответа от сервера
        bzero(messageCS, sizeof(messageCS));
        read(socket_file_descriptor, messageCS, sizeof(messageCS));

        //Convert answer to int (1 - yes, 0 - no)
        unsigned nameAddressee_isCorrect = 0;
        sscanf(messageCS, "%d", &nameAddressee_isCorrect);
  
        //Неверное имя адресата        
        if ( (nameAdressee != "all") && (nameAddressee_isCorrect == 0) ) {
            _OK = false;
            std::cout << "User with such a nickname is not found.\n";
            chat.transitionTo(std::move(std::make_unique<AddresseeIsMissing>()));
        }
    }

    //Адресат корректный
    if(_OK == true) {
        std::cout << "Enter a message: ";
        std::string textMessage;
        std::getline(std::cin >> std::ws, textMessage);

        //Текст введён
        if (!textMessage.empty()) {
            //Создать сообщение
            //ЗАПРОС СЕРВЕРУ - ОТПРАВИТЬ СООБЩЕНИЕ  
            std::time_t t = std::time(nullptr);
            std::tm* now = std::localtime(&t); 
            char time_buffer[128];
            strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", now);

            const std::string request = "8|" + chat.getUser()->getName() + "|" + nameAdressee + "|" + time_buffer + "|" + textMessage;
            bzero(messageCS, sizeof(messageCS));
            strcpy(messageCS, request.c_str());
            ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

            //Ждем ответа от сервера
            bzero(messageCS, sizeof(messageCS));
            read(socket_file_descriptor, messageCS, sizeof(messageCS));
            
            //Convert answer to int ( 1 - ОК )
            unsigned server_answer = 0;
            sscanf(messageCS, "%d", &server_answer);
            if (server_answer == 1) {
                std::cout << "The message has been sent!\n";
            }
        }

        //Текст не введён
        else {
            std::cout << "The message has not been sent (text is missing)\n";
        }

        chat.transitionTo(std::move(std::make_unique<UserInChat>()));
    }
}