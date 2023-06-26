/**
\file AddresseeIsMissing.h
\brief Класс-обработчик состояния "АДРЕСАТ ОТСУТСТВУЕТ"
*/

#pragma once

#include "State.h"

class AddresseeIsMissing final : public State {// хорошо использовать ключевое слово final – в тех классах, которые не предполагают наследования.  
                                               // Считается, что применение этого ключевого слова повышает производительность
    public:
        /**
        Конструктор по-умолчанию
        */
        AddresseeIsMissing();

        /**
        Обработчик состояния "АДРЕСАТ ОТСУТСТВУЕТ"
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