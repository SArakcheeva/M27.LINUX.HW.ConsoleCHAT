﻿/**
\file PasswordIncorrect.h
\brief Класс-обработчик состояния "ПАРОЛЬ НЕВЕРНЫЙ"
*/

#pragma once

#include "State.h"

class PasswordIncorrect final : public State {
    public:
        /**
        Конструктор по-умолчанию
        */
        PasswordIncorrect();

        /**
        Обработчик состояния "ПАРОЛЬ НЕВЕРНЫЙ"
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