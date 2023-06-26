#include "AddresseeIsMissing.h"
#include "Chat.h"
#include <iostream>
#include <memory>

//Возможный выбор пользователя
enum class EnumChoiceAIM : unsigned
{
    INPUT_AGAIN = 1,
    CANCEL
};


AddresseeIsMissing::AddresseeIsMissing() : State("AddresseeIsMissing")
{
};


void AddresseeIsMissing::handle(Chat& chat)
{
    std::cout << "| 1 - Enter receiver Nickname again | 2 - Cancel sending message | :  ";
    std::string input;
    std::getline(std::cin >> std::ws, input);

    //Введено более одного символа
    if (input.length() > 1) {
        chat.transitionTo(std::move(std::make_unique<AddresseeIsMissing>()));
    }
    //Введён один символ
    else {
        try {
            int choice = std::stoi(input);
            handleChoice(chat, choice);
        }
        catch (const std::invalid_argument&) {
            chat.transitionTo(std::move(std::make_unique<AddresseeIsMissing>()));
        }
    }
}


void AddresseeIsMissing::handleChoice(Chat& chat, int choice_)
{
    EnumChoiceAIM choice = static_cast<EnumChoiceAIM>(choice_);
    switch (choice) {
    case EnumChoiceAIM::INPUT_AGAIN: {
        chat.transitionTo(std::move(std::make_unique<EnteringAddressee>()));
        break;
    }
    case EnumChoiceAIM::CANCEL: {
        chat.transitionTo(std::move(std::make_unique<UserInChat>()));
        break;
    }
    default: {
        std::cin.clear();
        chat.transitionTo(std::move(std::make_unique<AddresseeIsMissing>()));
        break;
    }
    }
}