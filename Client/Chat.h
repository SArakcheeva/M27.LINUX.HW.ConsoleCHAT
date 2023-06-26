/**
\file Chat.h
\brief Класс управляет логикой работы чата

Управляет процессом входа / выхода / регистрации нового пользователя в системе,
отправкой сообщений и т.д.
Логика работы представлена в виде конечного автомата - паттерн State.
Логика определяется текущим состоянием и условиями переходами между ними.
Объект класса должен быть ОДИН - паттерн Singleton
*/

#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "User.h"
#include "Socket.h"
//Классы-обработчики состояний
#include "State.h"
#include "StartState.h"
#include "SignIn.h"
#include "Registration.h"
#include "LoginCorrect.h"
#include "LoginIncorrect.h"
#include "LoginUnique.h"
#include "LoginNonunique.h"
#include "PasswordCorrect.h"
#include "PasswordIncorrect.h"
#include "UserInChat.h"
#include "EnteringAddressee.h"
#include "AddresseeIsMissing.h"


class Chat {
public:
    /**
    Единая точка доступа к единственному объекту класса
    \return Указатель на объект класса
    */
    static Chat* getInstance();

    //Объект класса нельзя копировать и перемещать
    Chat(const Chat& other) = delete;
    Chat(Chat&& other) = delete;
    Chat& operator= (const Chat& other) = delete;
    Chat& operator= (Chat&& other) = delete;

    /**
    Логика работы чата
    */
    void process();

    /**
    Перейти в заданное состояние
    \param[in] newState Умный указатель на объект - новое состояние
    */
    void transitionTo(std::unique_ptr<State> newState);

    /**
    \return Указатель на текущего пользователя чата
    */
    User* getUser();

    /**
    Установить Признак продолжения работы программы
    */
    void attach(std::shared_ptr<bool> isRun);

    /**
    Завершить работу программы
    */
    void exit();

    /**
    Вывод в консоль списка имён зарегистрированных пользователей
    */
    void printUserList();

    /**
    Разделить строку на подстроки, используя делиметр
    */
    std::vector<std::string> split(const std::string & s, char delim); 

    /**
    Вывод в консоль сообщений текущему пользователю чата
    */
    void printMessagesToUser();

    /**
    Удалить аккаунт пользователя
    */
    void removeAccount();

    /**
    Проверка Логина/Пароля/Ника на наличие запрещённых символов
    \param[in] inputValue Введенное значение
    \return Признак корректного значения (нет запрещённых символов)
    */
    bool isCorrectValue(const std::string& inputValue);

private:
    /**
    Конструктор private - потому что нельзя создавать объект извне класса
    */
    explicit Chat();

    static Chat* instance_;         ///<Указатель на единственный объект класса
    User* user_;                    ///<Текущий пользователь чата (чтобы передавать параметры пользователя между состояниями)
    std::unique_ptr<State> state_;  ///<Текущее состояние
    std::shared_ptr<bool> isRun_;   ///<Признак продолжения работы программы
};