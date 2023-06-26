#include "Chat.h"
#include <iostream>
#include <vector>
#include <list>
#include <sstream>

//Начальная инициализация указателя на статический объект класса
Chat* Chat::instance_ = nullptr;


Chat* Chat::getInstance()
{    
    if (instance_ == nullptr) {
        instance_ = new Chat();
    }
    return instance_;
}


void Chat::process()
{
    state_->handle(*this);
}


Chat::Chat() : state_(std::make_unique<StartState>()), user_(new User), isRun_(nullptr)
{

};


void Chat::transitionTo(std::unique_ptr<State> newState)
{
    state_ = std::move(newState);
}


User* Chat::getUser()
{
    return user_;
}


void Chat::attach(std::shared_ptr<bool> isRun)
{
    isRun_ = isRun;
}


void Chat::exit()
{
    *isRun_ = false;
}


std::vector<std::string> Chat::split(const std::string & s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

void Chat::printUserList()
{
    //Загрузить список пользователей и вывести на экран

    //ЗАПРОС СЕРВЕРУ - СПИСОК ВСЕХ ПОЛЬЗОВАТЕЛЕЙ
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, "6");
    ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

    // Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //ответ СТРОКА ИМЕН
    std::vector<std::string> userNames = split(std::string(messageCS), '|');
	for (const auto& name : userNames) {
		std::cout << name << "; ";
	}
	std::cout << std::endl;
}


void Chat::printMessagesToUser()
{               
    //ЗАПРОС СЕРВЕРУ
    const std::string request = "7|" + user_->getLogin() + "|" + user_->getPassword() + "|" + user_->getName();
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Загрузить сообщения и вывести на экран
    const std::string server_answer = std::string(messageCS);
    std::vector<std::string> userMess = split(server_answer, '||');

    if (userMess.empty()) {
        std::cout << "No messages for you.\n";
    }
    else {
        for (const auto& mess : userMess) {
            if(mess.length() > 0) {
                std::cout << mess << std::endl;
            }
        }
    }
	std::cout << std::endl;
}


void Chat::removeAccount()
{
    //ЗАПРОС СЕРВЕРУ - УДАЛИТЬ ПОЛЬЗОВАТЕЛЯ
    const std::string request = "4|" + user_->getLogin() + "|" + user_->getPassword() + "|" + user_->getName();
    bzero(messageCS, sizeof(messageCS));
    strcpy(messageCS, request.c_str());
    ssize_t bytes = write(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Ждем ответа от сервера
    bzero(messageCS, sizeof(messageCS));
    read(socket_file_descriptor, messageCS, sizeof(messageCS));

    //Convert answer to int (ответ 1 или 0)
    unsigned isRemove = 0;
    sscanf(messageCS, "%d", &isRemove);
    if (isRemove == 1) {
        user_->reset();
        std::cout << "Your account has been deleted.\n";
    }
    else {
        std::cout << "Error deleting from the DB. Your account hasn't been deleted\n";
    }
}


bool Chat::isCorrectValue(const std::string& inputValue)
{
    //Можно вводить символы латинского алфавита и арабские цифры
    const std::string permissionedChars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t pos = inputValue.find_first_not_of(permissionedChars);
    if (pos != std::string::npos) {
        return false;
    }
    return true;
}