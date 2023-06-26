/**
\file User.h
\brief ����� �������� ������ � ������������
����� ������������� � ���� ��������� ������������:
- ��� (���) - �� ���� �� ����� �������� ������ �������������
- ����� - ��� �� �������� �� ����� �������� � ���
- ������
*/

#pragma once
#include <fstream>
#include <string>

class User {
public:
	User();
	User(const std::string& name,
		const std::string& login,
		const std::string& password);
	/**
	����������� �����������
	*/
	User(const User& u);

	/**
	\return ��� ������������
	*/
	std::string getName() const;

	/**
	\return ����� ������������
	*/
	std::string getLogin() const;

	/**
	\return ������ ������������
	*/
	std::string getPassword() const;

	/**
	������ ������������ ���
	\param[in] name ���
	*/
	void setName(const std::string& name);

	/**
	������ ������������ �����
	\param[in] login �����
	*/
	void setLogin(const std::string& login);

	/**
	������ ������������ ������
	\param[in] password ������
	*/
	void setPassword(const std::string& password);

	/**
	��������� �������� ����� ������ - ������ ������
	*/
	void reset();

private:
	std::string name_;		///<���
	std::string login_;		///<�����
	std::string password_;	///<������
};