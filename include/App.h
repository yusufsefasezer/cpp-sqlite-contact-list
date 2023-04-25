#ifndef APP_H
#define APP_H

#include "User.h"
#include <sqlite3.h>

namespace ContactList
{

	class App
	{
	private:
		User *pCurrent = nullptr;
		sqlite3 *m_sql_cnn;
		sqlite3_stmt *m_sql_stmt;
		std::string m_sql;
		std::string m_message;

		void menu();
		void createDatabase();

		// User
		void registerUser();
		void userMenu();
		void loginUser();

		// Contact
		void addContact();
		void searchContact();
		void listContact();
		void editContact();
		void deleteContact();

		int createMenu(const std::vector<std::string_view> &t_options);
		void showMessage();
		void clearScreen();

	public:
		App() = default;
		App(const App &orig) = delete;
		void run();
		virtual ~App();
	};

}
#endif /* APP_H */
