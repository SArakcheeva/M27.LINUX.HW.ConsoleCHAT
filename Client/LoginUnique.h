/**
\file LoginUnique.h
\brief Класс-обработчик состояния "ЛОГИН УНИКАЛЬНЫЙ"
*/

#pragma once

#include "State.h"

class LoginUnique final : public State {
    public:
        /**
        Конструктор по-умолчанию
        */
        LoginUnique();

        /**
        Обработчик состояния "ЛОГИН УНИКАЛЬНЫЙ"
        */
        virtual void handle(Chat& chat) override;
};