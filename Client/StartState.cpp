#include "StartState.h"
#include <iostream>

//Возможный выбор пользователя
enum class EnumChoiceSS : unsigned
{
    SIGN_IN = 1,
    REGISTRATION,
    EXIT
};



StartState::StartState() : State("StartState")
{
};



void StartState::handle(Chat& chat)
{
    std::cout << "| 1 - Log in to CHAT | 2 - Registration | 3 - Exit | :  ";
    std::string input;
    std::getline(std::cin >> std::ws, input);

    //Введено более одного символа
    if (input.length() > 1) {
        chat.transitionTo(std::move(std::make_unique<StartState>()));
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
            chat.transitionTo(std::move(std::make_unique<StartState>()));
        }
    }
}



void StartState::handleChoice(Chat& chat, int choice_)
{
    EnumChoiceSS choice = static_cast<EnumChoiceSS>(choice_);
    switch (choice) {
    case EnumChoiceSS::SIGN_IN: {
        chat.transitionTo(std::move(std::make_unique<SignIn>()));
        break;
    }
    case EnumChoiceSS::REGISTRATION: {
        chat.transitionTo(std::move(std::make_unique<Registration>()));
        break;
    }
    case EnumChoiceSS::EXIT: {
        std::cout << "Exit the program";
        chat.exit();
        break;
    }
    default: {
        std::cin.clear();
        chat.transitionTo(std::move(std::make_unique<StartState>()));
        break;
    }
    }
}