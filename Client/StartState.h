/**
\file StartState.h
\brief Класс-обработчик состояния "НИКТО НЕ ЗАЛОГИНЕН"
*/

#pragma once
#include <memory>

#include "State.h"
#include "Chat.h"

class StartState final : public State {
    public:
        /**
        Конструктор по-умолчанию
        */
        StartState();

        /**
        Обработчик состояния "НИКТО НЕ ЗАЛОГИНЕН"
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