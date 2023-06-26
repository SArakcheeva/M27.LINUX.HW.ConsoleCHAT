#include "LoginNonunique.h"
#include "Chat.h"
#include <iostream>
#include <memory>

//Возможный выбор пользователя
enum class EnumChoiceLN : unsigned
{
    INPUT_AGAIN = 1,
    REGISTRATION
};



LoginNonunique::LoginNonunique() : State("LoginNonunique")
{
};



void LoginNonunique::handle(Chat& chat)
{
    std::cout << "| 1 - Enter by this Login | 2 - Back to registration | :  ";
    std::string input;
    std::getline(std::cin >> std::ws, input);

    //Введено более одного символа
    if (input.length() > 1) {
        chat.transitionTo(std::move(std::make_unique<LoginNonunique>()));
    }
    //Введён один символ
    else {
        //Попытка преобразовать символ в число
        try {
            int choice = std::stoi(input);
            handleChoice(chat, choice);
        }
        //Символ не число - вернуться в начало ко вводу
        catch (const std::invalid_argument&) {
            chat.transitionTo(std::move(std::make_unique<LoginNonunique>()));
        }
    }
}



void LoginNonunique::handleChoice(Chat& chat, int choice_)
{
    EnumChoiceLN choice = static_cast<EnumChoiceLN>(choice_);
    switch (choice) {
    case EnumChoiceLN::INPUT_AGAIN: {
        chat.transitionTo(std::move(std::make_unique<LoginCorrect>()));
        break;
    }
    case EnumChoiceLN::REGISTRATION: {
        chat.transitionTo(std::move(std::make_unique<Registration>()));
        break;
    }
    default: {
        std::cin.clear();
        chat.transitionTo(std::move(std::make_unique<LoginNonunique>()));
        break;
    }
    }
}