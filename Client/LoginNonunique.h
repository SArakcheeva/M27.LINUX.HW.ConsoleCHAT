/**
\file LoginNonunique.h
\brief Класс-обработчик состояния "ЛОГИН НЕУНИКАЛЬНЫЙ"
*/

#pragma once

#include "State.h"

class LoginNonunique final : public State {
    public:
        /**
        Конструктор по-умолчанию
        */
        LoginNonunique();

        /**
        Обработчик состояния "ЛОГИН НЕУНИКАЛЬНЫЙ"
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