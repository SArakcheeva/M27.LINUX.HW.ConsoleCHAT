/**
\file UserInChat.h
\brief Класс-обработчик состояния "ПОЛЬЗОВАТЕЛЬ В ЧАТЕ"
*/

#pragma once

#include "State.h"

class UserInChat final : public State {
    public:
        /**
        Конструктор по-умолчанию
        */
        UserInChat();

        /**
        Обработчик состояния "ПОЛЬЗОВАТЕЛЬ В ЧАТЕ"
        */
        virtual void handle(Chat& chat) override;

    private:
        /**
        Обработчик ввода пользователя в зависимости от введённого числа
        \param[in] chat Умный указатель на объект чата для которого выполнять обработку
        \param[in] choice Число которое ввёл пользователь
        */
        void handleChoice(Chat& chat, int choice);
};