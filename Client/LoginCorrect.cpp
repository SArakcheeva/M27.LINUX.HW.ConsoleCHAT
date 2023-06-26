#include "LoginCorrect.h"
#include "Chat.h"
#include <iostream>
#include <vector>
#include <memory>

LoginCorrect::LoginCorrect() : State("LoginCorrect")
{
};



void LoginCorrect::handle(Chat& chat)
{
    std::cout << "Enter the Password: ";
    std::string password;
    std::getline(std::cin >> std::ws, password);

    //Допустимые символы
    if (chat.isCorrectValue(password)) {
        //Пароль правильный
        //ЗАПРОС СЕРВЕРУ - ПАРОЛЬ ПРАВИЛЬНЫЙ
        std::string request = "1|" + chat.getUser()->getLogin() + "|" + password;
        bzero(messageCS, sizeof(messageCS));
        strcpy(messageCS, request.c_str());
        ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

        // Ждем ответа от сервера
        bzero(messageCS, sizeof(messageCS));
        read(socket_file_descriptor, messageCS, sizeof(messageCS));
        
        //Convert answer to int
        unsigned pass_isCorrect = 0; 
        sscanf(messageCS, "%d", &pass_isCorrect);
        if (pass_isCorrect == 1) {
            chat.getUser()->setPassword(password);
            
            //Загрузить из базы и задать Ник текущего пользователя
            //ЗАПРОС СЕРВЕРУ - ПАРОЛЬ ПРАВИЛЬНЫЙ
            request = "9|" + chat.getUser()->getLogin();
            bzero(messageCS, sizeof(messageCS));
            strcpy(messageCS, request.c_str());
            ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

            // Ждем ответа от сервера
            bzero(messageCS, sizeof(messageCS));
            read(socket_file_descriptor, messageCS, sizeof(messageCS));
            const std::string name = messageCS;//ИЗ ОТВЕТА С СЕРВЕРА
            if(name.length() == 0) {
                std::cout << "Incorrect data from server!\n";
                chat.transitionTo(std::move(std::make_unique<LoginCorrect>()));
            }
            else {
                chat.getUser()->setName(name);
                std::cout << chat.getUser()->getName() << ", welcome to CHAT!\n";
                chat.transitionTo(std::move(std::make_unique<UserInChat>()));
                chat.printMessagesToUser();
            }
        }
        //Пароль неверный
        else {
            std::cout << "Password is incorrect!\n";
            chat.transitionTo(std::move(std::make_unique<PasswordIncorrect>()));
       }
    }

    //Недопустимые символы
    else {
        std::cout << "Invalid characters.\n";
        std::cin.clear();
        chat.transitionTo(std::move(std::make_unique<LoginCorrect>()));
    }
}