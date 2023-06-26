#include "Logger.h"
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

using namespace std;


Logger::Logger()
{
	fs_.open(logFile, fstream::in | fstream::out | fstream::app);

	// perms - Этот тип представляет права доступа к файлам	
	auto permissions = std::filesystem::perms::group_all | std::filesystem::perms::others_all;

	std::filesystem::permissions(logFile, permissions, std::filesystem::perm_options::remove);
}


Logger::~Logger()
{
	fs_.close();
}


std::vector<std::string> Logger::readFromFile() {

	shared_mutex_.lock_shared();

	std::vector<std::string> result;
	if (fs_.is_open()) {
		std::string line;
        
    	// Сначала надо установить позицию для чтения в начало файла
		fs_.seekg(0, std::ios_base::beg);

		while (std::getline(fs_, line)) {
			result.push_back(line);
		}
	}
	shared_mutex_.unlock_shared();

	return result;
}


void Logger::writeToFile(Message mess_)
{
	shared_mutex_.lock();
    
    // Сначала надо установить позицию для записи в конец файла
    fs_.seekp(0, std::ios_base::end);
	fs_ << mess_.getNameFrom() + " " + mess_.getNameTo() + " " + mess_.getSendTime() + " " + mess_.getText() << "\n";

	shared_mutex_.unlock();
}