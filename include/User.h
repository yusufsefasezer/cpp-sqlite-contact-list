#ifndef USER_H
#define USER_H

#include "Header.h"

namespace ContactList {

	class User {
	private:
		int m_userId;
		string m_userName;
		string m_userPass;

	public:
		User() = default;
		User(const User& orig) = delete;

		User(const int& t_userId, const string& t_userName, const string& t_userPass);

		int getUserId() const;
		void setUserId(const int& t_userId);

		string getUserName() const;
		void setUserName(const string& t_userName);

		string getUserPass() const;
		void setUserPass(const string& t_userPass);

		virtual ~User();
	};

}

#endif /* USER_H */