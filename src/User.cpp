#include "User.h"

namespace ContactList
{

	User::User(const int &t_userId,
						 std::string_view t_userName,
						 std::string_view t_userPass)
			: m_userId(t_userId),
				m_userName(t_userName),
				m_userPass(t_userPass) {}

	int User::getUserId() const
	{
		return m_userId;
	}

	void User::setUserId(const int &t_userId)
	{
		m_userId = t_userId;
	}

	std::string_view User::getUserName() const
	{
		return m_userName;
	}

	void User::setUserName(std::string_view t_userName)
	{
		m_userName = t_userName;
	}

	std::string_view User::getUserPass() const
	{
		return m_userPass;
	}

	void User::setUserPass(std::string_view t_userPass)
	{
		m_userPass = t_userPass;
	}

	User::~User() {}

}