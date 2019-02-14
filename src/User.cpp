#include "User.h"

namespace ContactList {

	User::User(const int& t_userId, const string& t_userName, const string& t_userPass) :
	m_userId(t_userId), m_userName(t_userName), m_userPass(t_userPass) {
	}

	int User::getUserId() const {
		return m_userId;
	}

	void User::setUserId(const int& t_userId) {
		m_userId = t_userId;
	}

	string User::getUserName() const {
		return m_userName;
	}

	void User::setUserName(const string& t_userName) {
		m_userName = t_userName;
	}

	string User::getUserPass() const {
		return m_userPass;
	}

	void User::setUserPass(const string& t_userPass) {
		m_userPass = t_userPass;
	}

	User::~User() {
	}

}