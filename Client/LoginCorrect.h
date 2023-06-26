/**
\file LoginCorrect.h
\brief Класс-обработчик состояния "ЛОГИН КОРРЕКТНЫЙ"
*/

#pragma once

#include "State.h"

class LoginCorrect final : public State {
    public:
        /**
        Конструктор по-умолчанию
        */
        LoginCorrect();

        /**
        Обработчик состояния "ЛОГИН КОРРЕКТНЫЙ"
        */
        virtual void handle(Chat& chat) override;
};