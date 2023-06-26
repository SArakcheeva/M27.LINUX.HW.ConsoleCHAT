#include "UserInChat.h"
#include "Chat.h"
#include <iostream>
#include <memory>

//Возможный выбор пользователя
enum class EnumChoiceUIC : unsigned
{
    SEND_MESSAGE = 1,
    READ_MESSAGE,
    SHOW_USERS,
    EXIT,
    REMOVE_ACCOUT
};



UserInChat::UserInChat() : State("UserInChat")
{
};



void UserInChat::handle(Chat& chat)
{
    std::cout << "| 1 - Send message | 2 - Read messages | 3 - User list | 4 - Exiting the CHAT | 5 - Delete Account | :  ";
    std::string input;
    std::getline(std::cin >> std::ws, input);

    //Введено более одного символа
    if (input.length() > 1) {
        chat.transitionTo(std::move(std::make_unique<UserInChat>()));
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



void UserInChat::handleChoice(Chat& chat, int choice_)
{
    EnumChoiceUIC choice = static_cast<EnumChoiceUIC>(choice_);
    switch (choice) {
    case EnumChoiceUIC::SEND_MESSAGE: {
        chat.transitionTo(std::move(std::make_unique<EnteringAddressee>()));
        break;
    }
    case EnumChoiceUIC::READ_MESSAGE: {
        chat.printMessagesToUser();
        break;
    }
    case EnumChoiceUIC::SHOW_USERS: {
        chat.printUserList();
        break;
    }
    case EnumChoiceUIC::EXIT: {
        chat.transitionTo(std::move(std::make_unique<StartState>()));
        chat.getUser()->reset();
        break;
    }
    case EnumChoiceUIC::REMOVE_ACCOUT: {
        chat.removeAccount();
        chat.transitionTo(std::move(std::make_unique<StartState>()));
        break;
    }
    default: {
        std::cin.clear();
        chat.transitionTo(std::move(std::make_unique<UserInChat>()));
        break;
    }
    }
}