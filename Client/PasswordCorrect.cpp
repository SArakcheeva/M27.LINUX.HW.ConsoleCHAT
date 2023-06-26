#include "PasswordCorrect.h"
#include "Chat.h"
#include <iostream>
#include <memory>

PasswordCorrect::PasswordCorrect() : State("PasswordCorrect")
{
};



void PasswordCorrect::handle(Chat& chat)
{
    std::cout << "Enter Nickname : ";
    std::string name;
    std::getline(std::cin >> std::ws, name);

    //Допустимые символы
    if (chat.isCorrectValue(name)) {
        //Такой ник уже зарегистрирован
        
        //ЗАПРОС СЕРВЕРУ - ЕСТЬ ЛИ ИМЯ ПОЛЬЗОВАТЕЛЯ В БД
        const std::string request = "2|" + name;
        bzero(messageCS, sizeof(messageCS));
        strcpy(messageCS, request.c_str());
        ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

        // Ждем ответа от сервера
        bzero(messageCS, sizeof(messageCS));
        read(socket_file_descriptor, messageCS, sizeof(messageCS));

        //Convert answer to int
        unsigned name_isCorrect = 0;
        sscanf(messageCS, "%d", &name_isCorrect);
        if (name_isCorrect == 1) {
            std::cout << "User with this Nickname is registered already\n";
            chat.transitionTo(std::move(std::make_unique<PasswordCorrect>()));
        }

        //Ник уникальный
        else {
            chat.getUser()->setName(name);

            //ЗАПРОС СЕРВЕРУ - ДОБАВИТЬ ПОЛЬЗОВАТЕЛЯ В БД
            const std::string request = "3|" + chat.getUser()->getLogin() + "|" + chat.getUser()->getPassword() + "|" + chat.getUser()->getName();
            bzero(messageCS, sizeof(messageCS));
            strcpy(messageCS, request.c_str());
            ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

            // Ждем ответа от сервера
            bzero(messageCS, sizeof(messageCS));
            read(socket_file_descriptor, messageCS, sizeof(messageCS));

            //Convert answer to int
            unsigned s_answer = 0;
            sscanf(messageCS, "%d", &s_answer);
            if (s_answer == 1) {
                std::cout << "You have successfully registered!\n"
                          << chat.getUser()->getName() << ", welcome to CHAT!\n";

                chat.transitionTo(std::move(std::make_unique<UserInChat>()));
            }
            else {
                std::cout << "Error writing to the DB!\n";
                chat.transitionTo(std::move(std::make_unique<PasswordCorrect>()));
            }
        }
    }

    //Недопустимые символы
    else {
        std::cout << "Invalid characters.\n";
        std::cin.clear();
        chat.transitionTo(std::move(std::make_unique<PasswordCorrect>()));
    }
}