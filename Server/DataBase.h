/**
\file DataBase.h
\brief Модуль "База данных" - содержит данные о Пользователях
Предоставляет функции работы с базой данных Пользователей:
- проверить есть ли заданный Логин в Базе
- проверить корректный ли Пароль
- добавить пользователя в Базу
*/

#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Message.h"
#include "User.h"

#include "/usr/include/mysql/mysql.h"

using namespace std;


class DataBase final {
public:
	/**
	Заполнить базу начальными значениями
	*/
	DataBase();

	/**
	Деструктор
	*/
	~DataBase();

	/**
	Connections to DB
	*/
	void connectionDB();

	/**
	\return Создать новую базу данных
	*/
	void createDataBase();

	/**
	\return Создать таблицу Users
	*/	
	void createUsersTable();

	/**
	Добавить в базу заданного пользователя
	\param[in] user Пользователь которого добавить
	*/
	bool addUser(const User& user);

	/**
	\return Создать таблицу Messages
	*/
	void createMessagesTable();

	/**
	\return Определить id пользователя
	*/
	int findIdByNickname(const std::string& nickname);

	/**
	\return Определить nickname пользователя по id
	*/
	std::string findNicknameById(int id);
	
	/**
	Заполнить таблицу Messages сообщениями от одного пользователя всем
	\param[in] message Сообщение
	\return Признак, что сообщение занесено в БД
	*/
	bool addMessageForAllUsers(const Message& message);

	/**
	Добавить в таблицу Messages сообщение от одного пользователя другому.
	\param[in] message Сообщение
	\return Признак, что сообщение занесено в БД
	*/
	bool addMessageForUser(const Message& message);

	/**
	Проверить есть ли в базе заданный Логин
	\param[in] login Логин
	\return Признак наличия заданного Логина в базе
	*/
	bool isExistLogin(const std::string& login);

	/**
	Проверить соответствует ли Пароль заданному Логину в БД
	\param[in] login Логин
	\param[in] password Пароль
	\return Признак правильный ли Пароль
	*/
	bool isCorrectPassword(const std::string& login, const std::string& password);

	/**
	Проверить есть ли в базе заданный Ник
	\param[in] name Ник
	\return Признак наличия заданного Ника в базе
	*/
	bool isExistName(const std::string& nickname);

	/**
	\return Количество зарегистрированных пользователей
	*/
	int getNumberUser();

	/**
	Загрузить имена зарегистрированных пользователей
	\return map с именами зарегистрированных пользователей
	*/
	std::map<int, std::string> loadUserNames();

	/**
	Загрузить сообщения, адресованные заданному пользователю
	\param[in] addressee Адресат сообщений
	\return вектор в который поместить сообщения
	*/
	std::vector<Message> loadMessages(const User& addressee);

	/**
	Удалить из базы все сообщения, адресованные пользователю с заданным ником
	\param[in] nickname Ник пользователя
	*/
	void removeMessagesToUser(const std::string& nickname);

	/**
	Удалить заданного пользователя из базы
	\param[in] user Пользователь которого удалить
	*/
	void removeUser(const User& user);

	/**
	\return Вывести содержимое запроса в консоль
	*/
	void showResult();

	/**
	\return Показать таблицу Users
	*/
	void showUsersTable();

	/**
	\return Показать таблицу Messages
	*/
	void showMessagesTable();

	/**
	Вернуть ник по логину.
	Если логин не зарегистрирован - возвращает пустую строку
	\param[in] login Логин
	\return Ник пользователя
	*/
	std::string getNameByLogin(const std::string& login);


private:
	MYSQL* conn_;		///<Дескриптор соединения c MySql
	MYSQL_RES* res_;	///<Результат запроса
	MYSQL_ROW row_;		///<Строка в результирующем наборе данны
};