#ifndef USER_H
#define USER_H

#include "Header.h"

namespace ContactList
{

	class User
	{
	private:
		int m_userId;
		std::string_view m_userName;
		std::string_view m_userPass;

	public:
		User() = default;
		User(const User &orig) = delete;

		User(const int &t_userId, std::string_view t_userName, std::string_view t_userPass);

		int getUserId() const;
		void setUserId(const int &t_userId);

		std::string_view getUserName() const;
		void setUserName(std::string_view t_userName);

		std::string_view getUserPass() const;
		void setUserPass(std::string_view t_userPass);

		virtual ~User();
	};

}

#endif /* USER_H */