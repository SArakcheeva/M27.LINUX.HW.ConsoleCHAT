/**
\file PasswordCorrect.h
\brief Класс-обработчик состояния "ПАРОЛЬ КОРРЕКТНЫЙ"
*/

#pragma once

#include "State.h"

class PasswordCorrect final : public State {
    public:
        /**
        Конструктор по-умолчанию
        */
        PasswordCorrect();

        /**
        Обработчик состояния "ПАРОЛЬ КОРРЕКТНЫЙ"
        */
        virtual void handle(Chat& chat) override;
};