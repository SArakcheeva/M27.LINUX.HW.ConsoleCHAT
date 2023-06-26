#include "SignIn.h"
#include "Chat.h"
#include "Socket.h"
#include <iostream>
#include <memory>
#include <string.h>

SignIn::SignIn() : State("SignIn"){};

void SignIn::handle(Chat &chat)
{
    std::cout << "Enter Login (valid characters 'a'-'z', 'A'-'Z', '0'-'9'): ";
    std::string login;
    std::getline(std::cin >> std::ws, login);

    // Допустимые символы
    if (chat.isCorrectValue(login))
    {
        // ЗАПРОС СЕРВЕРУ - ЕСТЬ ЛИ ЛОГИН В БД
        const std::string request = "0|" + login;
        bzero(messageCS, sizeof(messageCS));
        strcpy(messageCS, request.c_str());
        ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

        // Ждем ответа от сервера
        bzero(messageCS, sizeof(messageCS));
        read(socket_file_descriptor, messageCS, sizeof(messageCS));
        
        //Convert answer to int
        unsigned existLogin_ = 0;
        sscanf(messageCS, "%d", &existLogin_);

        // Логин зарегистрирован
        if (existLogin_ == 1)
        {
            chat.getUser()->setLogin(login);
            chat.transitionTo(std::move(std::make_unique<LoginCorrect>()));
        }
        // Логин не зарегистрирован
        else
        {
            std::cout << "Login not found!\n";
            chat.transitionTo(std::move(std::make_unique<LoginIncorrect>()));
        }
    }
    // Недопустимые символы
    else
    {
        std::cout << "Invalid characters.\n";
        std::cin.clear();
        chat.transitionTo(std::move(std::make_unique<SignIn>()));
    }
}