/**
\file Registration.h
\brief Класс-обработчик состояния "РЕГИСТРАЦИЯ ПОЛЬЗОВАТЕЛЯ"
*/

#pragma once

#include "State.h"

class Registration final : public State {
    public:
        /**
        Конструктор по-умолчанию
        */
        Registration();

        /**
        Обработчик состояния "РЕГИСТРАЦИЯ ПОЛЬЗОВАТЕЛЯ"
        */
        virtual void handle(Chat& chat) override;
};