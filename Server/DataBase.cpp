#include "DataBase.h"

#include <map>
#include <list>
#include <assert.h>
#include <iostream>
#include <fstream>

#include "NetworkConfig.h"
#include "User.h"
#include "Message.h"
#include "Logger.h"

namespace
{
	// База зарегистрированных пользователей
	std::string hostDB;
	std::string userDB;
	std::string passwdDB;
	std::string nameDB;
}


DataBase::DataBase()
{
	// Data to DB connection from network.config file
	NetworkConfig config;
    hostDB = config.getHostDB();
	hostDB.erase(hostDB.find(' '), 1);

    userDB = config.getUserDB();
	userDB.erase(userDB.find(' '), 1);

    passwdDB = config.getPasswdDB();
	passwdDB.erase(passwdDB.find(' '), 1);

	nameDB = config.getNameDB();
	nameDB.erase(nameDB.find(' '), 1);

    std::cout << "ConnectionDB settings: " << hostDB << "; " << userDB << "; " << passwdDB << "; " << nameDB << std::endl;

	// Connections to DB
	connectionDB();
	
	// Создать новую базу данных
	createDataBase();

	// Создать таблицу Users
	createUsersTable();

	// Заполнить таблицу Users данными
	User u1("G", "Ger", "123");
	User u2("S", "Sve", "qwe");	
	addUser(u1);
	addUser(u2);
    
	// Создать таблицу Messages
	createMessagesTable();
}


DataBase::~DataBase()
{
	// Закрываем соединение с сервером базы данных
	mysql_close(conn_);

	system("Pause");
}


void DataBase::connectionDB()
{
	// Получаем дескриптор соединения
	// необходимо вызвать функцию mysql_init() для инициализации дескриптора соединения с базой данных
	conn_ = mysql_init(NULL);

	if (conn_ == NULL) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		std::cout << "Error: can't create MySQL-descriptor" << std::endl;
	}

	// Подключаемся к серверу - для этого с нашим дескриптором нужно вызвать функцию mysql_real_connect()
	// Функция возвращает дескриптор соединения, переданный в первом параметре, 
	// если соединение было успешным или NULL если соединение было неудачным
	//if (!mysql_real_connect(conn_, "localhost", "root", "12061965avMysql", "testdb", NULL, NULL, 0)) {
	if (!mysql_real_connect(conn_, hostDB.c_str(), userDB.c_str(), passwdDB.c_str(), NULL, NULL, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		// для обработки ошибок вызывается функция mysql_error()
		std::cout << "Error: can't connect to database " << mysql_error(conn_) << std::endl;
	}
	else {
		// Если соединение успешно установлено выводим фразу — "Success!"
		std::cout << "Success!" << std::endl;
	}
	// задаёт набор символов по умолчанию, который будет использоваться при обмене данными с сервером баз данных
	mysql_set_character_set(conn_, "utf8");
	/* querystring = "SET CHARSET UTF8"; //задаем кодировку
	 conn.query(querystring).execute(); //выполняем запрос
	В данном примере указана кодировка UTF8, как основная на современных Linux системах. Вам следует задать кодировку вашей системы, для корретной обработки киррилицы.*/
	
	//Смотрим изменилась ли кодировка на нужную, по умолчанию идёт latin1
	std::cout << "connection characterset: " << mysql_character_set_name(conn_) << endl;
}


void DataBase::createDataBase()
{
	std::string request = "CREATE DATABASE " + nameDB;
	mysql_query(conn_, request.c_str());
	std::cout << "CREATE DATABASE " << nameDB << ". " << mysql_error(conn_) << std::endl;

	request = "USE " + nameDB;
	mysql_query(conn_, request.c_str());
	std::cout << "USE " << nameDB << ". " << mysql_error(conn_) << std::endl;
}


void DataBase::createUsersTable()
{
	mysql_query(conn_, "CREATE TABLE IF NOT EXISTS users (id INT AUTO_INCREMENT PRIMARY KEY, login VARCHAR(255) NOT NULL UNIQUE, nickname VARCHAR(255) NOT NULL UNIQUE, password VARCHAR(255) NOT NULL)");
	std::cout << "CREATE TABLE users. " << mysql_error(conn_) << std::endl;
}


bool DataBase::addUser(const User &user)
{
	std::string query = "INSERT INTO users(login, nickname, password) VALUES('" + user.getLogin() + "', '" + user.getName() + "', '" + user.getPassword() + "')";
	mysql_query(conn_, query.c_str());

	std::string err = mysql_error(conn_);
	if (err.empty())
		return true;
	else
		return false;

	std::cout << "INSERT INTO users. " << err << std::endl;
}


void DataBase::createMessagesTable()
{
	mysql_query(conn_, "CREATE TABLE IF NOT EXISTS messages (id INT AUTO_INCREMENT PRIMARY KEY, id_from INT REFERENCES users(id), id_to INT REFERENCES users(id), send_date DATETIME NOT NULL, text VARCHAR(255) NOT NULL)");
	std::cout << "CREATE TABLE messages. " << mysql_error(conn_) << std::endl;
}


int DataBase::findIdByNickname(const std::string& nickname)
{
	std::string query = "SELECT id FROM users WHERE nickname = '" + nickname + "'";
	mysql_query(conn_, query.c_str());

	//Можно определить, вернул ли данный запрос результирующий набор, проверкой, возвращает ли 0 функция mysql_store_result()
	res_ = mysql_store_result(conn_);	
	row_ = mysql_fetch_row(res_);
	mysql_free_result(res_);
	int id = 0;
	try {
		id = atoi(row_[0]);
	}
	catch (...) {
		id = -1;
	}
	return id;
}


std::string DataBase::findNicknameById(int id)
{
	std::string query1 = "SELECT nickname FROM users WHERE id = " + std::to_string(id);
	mysql_query(conn_, query1.c_str());

	//Можно определить, вернул ли данный запрос результирующий набор, проверкой, возвращает ли 0 функция mysql_store_result()
	res_ = mysql_store_result(conn_);
	row_ = mysql_fetch_row(res_);
	mysql_free_result(res_);
	std::string nick;
	try {
		nick = row_[0];
	}
	catch (...) {
	}
	return nick;
}


bool DataBase::addMessageForAllUsers(const Message& message)
{
	std::map<int, std::string> userNames = loadUserNames();
	if (userNames.size() == 0) {
		std::cout << "No information about registered users in the database" << std::endl;
		return false;
	}
	else {
		for (const auto user : userNames) {
			Message newMess(message.getNameFrom(), user.second, message.getSendTime(), message.getText());	//Создать сообщение
			addMessageForUser(newMess);
		}
		return true;
	}
}


bool DataBase::addMessageForUser(const Message& message)
{
	// определим id пользователя
	int id_from = findIdByNickname(message.getNameFrom());
	int id_to = findIdByNickname(message.getNameTo());
	if (id_from > 0 && id_to > 0) {
		std::string query = "INSERT INTO messages(id_from, id_to, send_date, text) VALUES(" + std::to_string(id_from) + ", " + std::to_string(id_to) + ", '" + message.getSendTime() + "', '" + message.getText() + "')";
		std::cout << query << std::endl;
		mysql_query(conn_, query.c_str());
		std::string err = mysql_error(conn_);

		// add logFile
		Logger logger;
		logger.writeToFile(message);

		if (err.empty()) {
			std::cout << "OK" << std::endl;		
			return true;
		}
		else
			std::cout << mysql_error(conn_) << std::endl;
	}
	else
		std::cout << "Error when entering data." << std::endl;

	return false;
}


bool DataBase::isExistLogin(const std::string& login)
{
	std::string query = "SELECT * FROM users WHERE login = '" + login + "'";
	mysql_query(conn_, query.c_str());

	res_ = mysql_store_result(conn_);
	int row_countDB = res_->row_count;
	mysql_free_result(res_);

	if (row_countDB == 1)
		return true;
	else
		return false;
}


bool DataBase::isCorrectPassword(const std::string& login, const std::string& password)
{
	std::string query = "SELECT password FROM users WHERE login = '" + login + "'";
	mysql_query(conn_, query.c_str());

	res_ = mysql_store_result(conn_);
	if (res_->row_count == 0) // в БД не найден пароль по указанному логину
		return false;

	row_ = mysql_fetch_row(res_);	
	std::string passInDB = row_[0];
	mysql_free_result(res_);

	if (passInDB == password)
		return true;
	else
		return false;
}


bool DataBase::isExistName(const std::string& nickname)
{
	bool result = false;
	
	std::string query = "SELECT * FROM users WHERE nickname = '" + nickname + "'";
	mysql_query(conn_, query.c_str());

	res_ = mysql_store_result(conn_);
	int row_countDB = res_->row_count;
	mysql_free_result(res_);

	if (row_countDB == 1)
		return true;
	else
		return false;
}


int DataBase::getNumberUser()
{
	mysql_query(conn_, "SELECT count(*) FROM users");
	res_ = mysql_store_result(conn_);
	row_ = mysql_fetch_row(res_);
	mysql_free_result(res_);

	int countUsers = 0;
	try {
		countUsers = atoi(row_[0]);
	}
	catch (const char* error) {
		std::cerr << error << std::endl;
	}

	return countUsers;
}


std::map<int, std::string> DataBase::loadUserNames()
{
	std::map<int, std::string> userNames;
	std::string row_str;
	mysql_query(conn_, "SELECT id, nickname FROM users");

	// Выводим все что есть в таблице через цикл
	if (res_ = mysql_store_result(conn_)) {
		while (row_ = mysql_fetch_row(res_)) {
			if(mysql_num_fields(res_) == 2) {
				std::pair<int, std::string> new_pair = std::make_pair(atoi(row_[0]), row_[1]);
				userNames.insert(new_pair);
			}
		}
	}
	else
		std::cout << "Ошибка MySql номер " << mysql_error(conn_);

	mysql_free_result(res_);
	
	return userNames;
}


std::vector<Message> DataBase::loadMessages(const User& addressee)
{
	std::vector<Message> destination;
	std::map<int, std::string> userNames = loadUserNames();
	int userId = findIdByNickname(addressee.getName());
	if (userId > 0) {
		std::string query = "SELECT * FROM messages WHERE (messages.id_from = " + std::to_string(userId) + " OR messages.id_to = " + std::to_string(userId) + ") ORDER by messages.send_date asc";
		mysql_query(conn_, query.c_str());
		// Выводим все что есть в таблице через цикл
		if (res_ = mysql_store_result(conn_)) {
			while (row_ = mysql_fetch_row(res_)) {
				if (mysql_num_fields(res_) == 5)
				{
					try {
						size_t userNamesSize = userNames.size();	
						std::string str_from = userNames.at(atoi(row_[1]));
						std::string str_to = userNames.at(atoi(row_[2]));
						Message messageInDB(str_from, str_to, row_[3], row_[4]);
						destination.push_back(messageInDB);
					}
					catch(...)
					{
						std::cout << "Exception!!!";
					}
				}
			}
		}
		else
			std::cout << "Error MySql number " << mysql_error(conn_);
	}
	else
		std::cout << "No user with nickname <" + addressee.getName() + "> in the database.";

	mysql_free_result(res_);

	return destination;
}


void DataBase::removeMessagesToUser(const std::string& nickname)
{
	int idDel = findIdByNickname(nickname);
	std::string idDel_str = std::to_string(idDel);
	std::string query = "DELETE FROM messages WHERE(id_from = " + idDel_str + " OR id_to = " + idDel_str + ")";
	mysql_query(conn_, query.c_str());
}


void DataBase::removeUser(const User &user)
{
	// Удалить сначала все сообщения данного пользователя
	removeMessagesToUser(user.getName());

	// Удалить пользователя
	std::string query = "DELETE FROM users WHERE nickname = '" + user.getName() + "'";
	mysql_query(conn_, query.c_str());
}


void DataBase::showResult()
{
	// Выводим все что есть в таблице через цикл
	if (res_ = mysql_store_result(conn_)) {
		while (row_ = mysql_fetch_row(res_)) {
			for (int i = 0; i < mysql_num_fields(res_); i++) {
				std::cout << row_[i] << "  ";
			}
			std::cout << std::endl;
		}
	}
	else
		std::cout << "Ошибка MySql номер " << mysql_error(conn_);

	// После использования результата обязательно освободи ресурсы перед следующим запросом!
	mysql_free_result(res_);
}


void DataBase::showUsersTable()
{
	mysql_query(conn_, "SELECT * FROM users");
	showResult();
}


void DataBase::showMessagesTable()
{
	mysql_query(conn_, "SELECT * FROM messages");
	showResult();
}


std::string DataBase::getNameByLogin(const std::string& login)
{
	std::string nickname;

	std::string query = "SELECT nickname FROM users WHERE login = '" + login + "'";
	mysql_query(conn_, query.c_str());

	res_ = mysql_store_result(conn_);
	if (res_->row_count > 0) {	// найдет nickname по указанному логину в БД 
		row_ = mysql_fetch_row(res_);
		nickname = row_[0];
		mysql_free_result(res_);
	}
	return nickname;
}