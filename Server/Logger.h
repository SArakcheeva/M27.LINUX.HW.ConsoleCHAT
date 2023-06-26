/**
\file Logger.h
\brief Модуль "логгирование сообщений" - работа с файлом логов.
в классе два метода:
- запись строки логов в файл;
- чтение строк из файла.
*/
#pragma once
#include <iostream>
#include <fstream>
#include <shared_mutex>
#include <string>
#include <vector>

#include "Message.h"

using namespace std;

constexpr auto logFile = "log.txt";


class Logger
{
public:
	Logger();
	~Logger();

	void writeToFile(Message mess_);
	std::vector<std::string> readFromFile();

private:
	std::fstream fs_;
	std::shared_mutex shared_mutex_;
};