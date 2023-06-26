#include "Registration.h"
#include "Chat.h"
#include <iostream>
#include <memory>

Registration::Registration() : State("Registration")
{
};



void Registration::handle(Chat& chat)
{
    std::cout << "Enter Login (valid characters 'a'-'z', 'A'-'Z', '0'-'9'): ";
    std::string login;
    std::getline(std::cin >> std::ws, login); //Функция ввода ws использует пробелы из входного потока

    //Допустимые символы
    if (chat.isCorrectValue(login)) {
        chat.getUser()->setLogin(login);
        //Такой логин уже зарегистрирован
        //ЗАПРОС СЕРВЕРУ - ЛОГИН ЕСТЬ В БД
        const std::string request = "0|" + login;
        bzero(messageCS, sizeof(messageCS));
        strcpy(messageCS, request.c_str());
        ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

        // Ждем ответа от сервера
        bzero(messageCS, sizeof(messageCS));
        read(socket_file_descriptor, messageCS, sizeof(messageCS));
        
        //Convert answer to int (ответ 1 или 0)
        unsigned login_isExist = 0;
        sscanf(messageCS, "%d", &login_isExist);
        
        if (login_isExist == 1) {
            std::cout << "Login is registered already!\n";
            chat.transitionTo(std::move(std::make_unique<LoginNonunique>()));
        }
        //Логин уникальный
        else {
            chat.transitionTo(std::move(std::make_unique<LoginUnique>()));
        }
    }

    //Недопустимые символы
    else {
        std::cout << "Invalid characters.\n";
        std::cin.clear();
        chat.transitionTo(std::move(std::make_unique<Registration>()));
    }
}