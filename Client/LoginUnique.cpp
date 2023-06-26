#include "LoginUnique.h"
#include "Chat.h"
#include <iostream>
#include <memory>

LoginUnique::LoginUnique() : State("LoginUnique")
{
};



void LoginUnique::handle(Chat& chat)
{
    std::cout << "Enter Password: ";
    std::string password;
    std::getline(std::cin >> std::ws, password);

    //Допустимые символы
    if (chat.isCorrectValue(password)) {
        chat.getUser()->setPassword(password);
        chat.transitionTo(std::move(std::make_unique<PasswordCorrect>()));
    }

    //Недопустимые символы
    else {
        std::cout << "Invalid characters.\n";
        std::cin.clear();
        chat.transitionTo(std::move(std::make_unique<LoginUnique>()));
    }
}