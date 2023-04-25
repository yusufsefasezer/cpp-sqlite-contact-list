#include "App.h"

namespace ContactList
{

	int App::createMenu(const std::vector<std::string_view> &t_options)
	{
		int choice = -1,
				len = t_options.size();

		do
		{
			std::cout << "### Menu ###" << std::endl;
			for (int i = 0; i < len; i++)
			{
				std::cout << (i + 1) << ". " << t_options[i] << std::endl;
			}

			std::cout << "Enter your choice: ";
			std::cin >> choice;
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			std::cin.sync();

			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<int>::max(), '\n');
				continue;
			}
		} while (choice < 1 || choice > len);

		return choice;
	}

	void App::showMessage()
	{
		int message_length = m_message.size();
		char prev;

		if (message_length == 0)
		{
			return;
		}

		prev = std::cout.fill('/');
		std::cout.width(message_length);
		std::cout << "" << std::endl;
		std::cout.fill(prev);

		std::cout << m_message << std::endl;

		prev = std::cout.fill('\\');
		std::cout.width(message_length);
		std::cout << "" << std::endl;
		std::cout.fill(prev);

		m_message = "";
	}

	void App::clearScreen()
	{
		system("cls||clear");
	}

	void App::run()
	{
		this->createDatabase();

		this->menu();
	}

	void App::menu()
	{
		std::vector<std::string_view> menu_options{"Register", "Login", "Exit"};
		int result = -1;

		while (1)
		{

			result = this->createMenu(menu_options);
			switch (result)
			{
			case 1:
				// Register
				this->registerUser();
				clearScreen();
				showMessage();
				break;
			case 2:
				// Lgoin
				this->loginUser();
				clearScreen();
				showMessage();
				if (this->pCurrent != nullptr)
				{
					this->userMenu();
				}
				break;
			case 3:
				// Exit
				pCurrent = nullptr;
				return;
				break;
			default:
				std::cout << "Please select correct option." << std::endl;
				break;
			}
		}
	}

	void App::createDatabase()
	{

		if (sqlite3_open("contacts.db", &m_sql_cnn) != SQLITE_OK)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(m_sql_cnn);
			exit(EXIT_FAILURE);
		}

		// USER
		sqlite3_prepare_v2(m_sql_cnn, "SELECT name FROM sqlite_master WHERE type='table' AND name='USER';", -1, &m_sql_stmt, nullptr);
		sqlite3_step(m_sql_stmt);

		if (sqlite3_data_count(m_sql_stmt) != 1)
		{

			sqlite3_prepare_v2(m_sql_cnn, "CREATE TABLE USER("
																		"userId INTEGER PRIMARY KEY AUTOINCREMENT, "
																		"userName TEXT NOT NULL UNIQUE, "
																		"userPass TEXT NOT NULL);",
												 -1, &m_sql_stmt, nullptr);

			if (sqlite3_step(m_sql_stmt) != SQLITE_DONE)
			{
				std::cerr << "SQL error: " << sqlite3_errmsg(m_sql_cnn);
				exit(EXIT_FAILURE);
			}
			else
			{
				std::cout << "User table created successfully." << std::endl;
			}
		}

		// PERSON
		sqlite3_prepare_v2(m_sql_cnn, "SELECT name FROM sqlite_master WHERE type='table' AND name='PERSON';", -1, &m_sql_stmt, nullptr);
		sqlite3_step(m_sql_stmt);

		if (sqlite3_data_count(m_sql_stmt) != 1)
		{

			sqlite3_prepare_v2(m_sql_cnn, "CREATE TABLE PERSON("
																		"personId INTEGER PRIMARY KEY AUTOINCREMENT, "
																		"userId INTEGER NOT NULL, "
																		"firstName TEXT NOT NULL, "
																		"lastName TEXT NOT NULL, "
																		"phoneNumber TEXT NOT NULL, "
																		"emailAddress TEXT NOT NULL UNIQUE, "
																		"address TEXT NOT NULL, "
																		"note TEXT NOT NULL);",
												 -1, &m_sql_stmt, nullptr);

			if (sqlite3_step(m_sql_stmt) != SQLITE_DONE)
			{
				std::cerr << "SQL error: " << sqlite3_errmsg(m_sql_cnn);
				exit(EXIT_FAILURE);
			}
			else
			{
				std::cout << "Person table created successfully." << std::endl;
			}
		}
	}

	void App::registerUser()
	{
		std::string userName = "";
		std::string userPass = "";

		std::cout << "User Name: ";
		std::getline(std::cin, userName);

		std::cout << "User Pass: ";
		std::getline(std::cin, userPass);

		m_sql = "INSERT INTO USER (userId, userName, userPass) VALUES (NULL, ?, ?);";
		sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
		sqlite3_bind_text(m_sql_stmt, 1, userName.c_str(), userName.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(m_sql_stmt, 2, userPass.c_str(), userPass.length(), SQLITE_TRANSIENT);

		if (sqlite3_step(m_sql_stmt) != SQLITE_DONE)
		{
			m_message = "SQL error: ";
			m_message += sqlite3_errmsg(m_sql_cnn);
		}
		else
		{
			m_message = "User registered successfully.";
		}
	}

	void App::userMenu()
	{
		std::vector<std::string_view> menu_options{"List", "Search", "Add New", "Edit", "Delete", "Exit"};
		int result = -1;

		while (1)
		{

			result = this->createMenu(menu_options);
			switch (result)
			{
			case 1:
				// List
				this->listContact();
				break;
			case 2:
				// Search
				this->searchContact();
				break;
			case 3:
				// List
				this->addContact();
				clearScreen();
				showMessage();
				break;
			case 4:
				// Edit
				this->editContact();
				clearScreen();
				showMessage();
				break;
			case 5:
				// Delete
				this->deleteContact();
				clearScreen();
				showMessage();
				break;
			case 6:
				// Exit
				pCurrent = nullptr;
				return;
				break;
			default:
				std::cout << "Please select correct option." << std::endl;
				break;
			}
		}
	}

	void App::loginUser()
	{
		std::string userName = "";
		std::string userPass = "";

		std::cout << "User Name: ";
		std::getline(std::cin, userName);

		std::cout << "User Pass: ";
		std::getline(std::cin, userPass);

		m_sql = "SELECT COUNT (userId) FROM USER WHERE userName = ? AND userPass = ?";
		sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
		sqlite3_bind_text(m_sql_stmt, 1, userName.c_str(), userName.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(m_sql_stmt, 2, userPass.c_str(), userPass.length(), SQLITE_TRANSIENT);
		sqlite3_step(m_sql_stmt);

		if (sqlite3_column_int(m_sql_stmt, 0) == 1)
		{

			m_sql = "SELECT * FROM USER WHERE userName = ? AND userPass = ?";
			sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
			sqlite3_bind_text(m_sql_stmt, 1, userName.c_str(), userName.length(), SQLITE_TRANSIENT);
			sqlite3_bind_text(m_sql_stmt, 2, userPass.c_str(), userPass.length(), SQLITE_TRANSIENT);
			sqlite3_step(m_sql_stmt);

			int userId = sqlite3_column_int(m_sql_stmt, 0);
			userName = (const char *)sqlite3_column_text(m_sql_stmt, 1);
			userPass = (const char *)sqlite3_column_text(m_sql_stmt, 2);

			pCurrent = new User();
			pCurrent->setUserId(userId);
			pCurrent->setUserName(userName);
			pCurrent->setUserPass(userPass);
		}
		else
		{
			m_message = "Username or password is wrong!";
		}
	}

	void App::addContact()
	{
		std::string firstName = "";
		std::string lastName = "";
		std::string phoneNumber = "";
		std::string emailAddress = "";
		std::string address = "";
		std::string note = "";

		std::cout << "First Name: ";
		std::getline(std::cin, firstName);

		std::cout << "Last Name: ";
		std::getline(std::cin, lastName);

		std::cout << "Phone Number: ";
		std::getline(std::cin, phoneNumber);

		std::cout << "Email Address: ";
		std::getline(std::cin, emailAddress);

		std::cout << "Address: ";
		std::getline(std::cin, address);

		std::cout << "Note: ";
		std::getline(std::cin, note);

		m_sql = "INSERT INTO PERSON (personId, userId, firstName, lastName, phoneNumber, emailAddress, address, note) "
						"VALUES (NULL, ?, ?, ?, ?, ?, ?, ?);";
		sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
		sqlite3_bind_int(m_sql_stmt, 1, pCurrent->getUserId());
		sqlite3_bind_text(m_sql_stmt, 2, firstName.c_str(), firstName.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(m_sql_stmt, 3, lastName.c_str(), lastName.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(m_sql_stmt, 4, phoneNumber.c_str(), phoneNumber.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(m_sql_stmt, 5, emailAddress.c_str(), emailAddress.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(m_sql_stmt, 6, address.c_str(), address.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(m_sql_stmt, 7, note.c_str(), note.length(), SQLITE_TRANSIENT);

		if (sqlite3_step(m_sql_stmt) != SQLITE_DONE)
		{
			m_message = "SQL error: ";
			m_message += sqlite3_errmsg(m_sql_cnn);
		}
		else
		{
			m_message = "Person added successfully.";
		}
	}

	void App::searchContact()
	{
		std::string userName = "";

		std::cout << "User Name: ";
		std::getline(std::cin, userName);

		m_sql = "SELECT * FROM PERSON WHERE userId = ? AND firstName LIKE ?";
		sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
		sqlite3_bind_int(m_sql_stmt, 1, pCurrent->getUserId());
		sqlite3_bind_text(m_sql_stmt, 2, userName.c_str(), userName.length(), SQLITE_TRANSIENT);

		clearScreen();

		while (sqlite3_step(m_sql_stmt) == SQLITE_ROW)
		{
			std::cout << sqlite3_column_text(m_sql_stmt, 0) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 2) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 3) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 4) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 5) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 6) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 7) << std::endl;
		}
	}

	void App::listContact()
	{
		m_sql = "SELECT * FROM PERSON WHERE userId = ?";
		sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
		sqlite3_bind_int(m_sql_stmt, 1, pCurrent->getUserId());

		clearScreen();

		while (sqlite3_step(m_sql_stmt) == SQLITE_ROW)
		{
			std::cout << sqlite3_column_text(m_sql_stmt, 0) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 2) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 3) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 4) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 5) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 6) << " | ";
			std::cout << sqlite3_column_text(m_sql_stmt, 7) << std::endl;
		}
	}

	void App::editContact()
	{
		std::string personId = "";

		std::cout << "Person ID: ";
		std::getline(std::cin, personId);

		m_sql = "SELECT COUNT (personId) FROM PERSON WHERE personId = ? AND userId = ?";
		sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
		sqlite3_bind_text(m_sql_stmt, 1, personId.c_str(), personId.length(), SQLITE_TRANSIENT);
		sqlite3_bind_int(m_sql_stmt, 2, pCurrent->getUserId());
		sqlite3_step(m_sql_stmt);

		if (sqlite3_column_int(m_sql_stmt, 0) == 1)
		{

			std::string firstName = "";
			std::string lastName = "";
			std::string phoneNumber = "";
			std::string emailAddress = "";
			std::string address = "";
			std::string note = "";

			std::cout << "First Name: ";
			std::getline(std::cin, firstName);

			std::cout << "Last Name: ";
			std::getline(std::cin, lastName);

			std::cout << "Phone Number: ";
			std::getline(std::cin, phoneNumber);

			std::cout << "Email Address: ";
			std::getline(std::cin, emailAddress);

			std::cout << "Address: ";
			std::getline(std::cin, address);

			std::cout << "Note: ";
			std::getline(std::cin, note);

			m_sql = "UPDATE PERSON SET "
							"firstName = ?, "
							"lastName = ?, "
							"phoneNumber = ?, "
							"emailAddress = ?, "
							"address = ?, "
							"note = ? "
							"WHERE personId = ? AND userId = ?;";
			sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
			sqlite3_bind_text(m_sql_stmt, 1, firstName.c_str(), firstName.length(), SQLITE_TRANSIENT);
			sqlite3_bind_text(m_sql_stmt, 2, lastName.c_str(), lastName.length(), SQLITE_TRANSIENT);
			sqlite3_bind_text(m_sql_stmt, 3, phoneNumber.c_str(), phoneNumber.length(), SQLITE_TRANSIENT);
			sqlite3_bind_text(m_sql_stmt, 4, emailAddress.c_str(), emailAddress.length(), SQLITE_TRANSIENT);
			sqlite3_bind_text(m_sql_stmt, 5, address.c_str(), address.length(), SQLITE_TRANSIENT);
			sqlite3_bind_text(m_sql_stmt, 6, note.c_str(), note.length(), SQLITE_TRANSIENT);
			sqlite3_bind_text(m_sql_stmt, 7, personId.c_str(), personId.length(), SQLITE_TRANSIENT);
			sqlite3_bind_int(m_sql_stmt, 8, pCurrent->getUserId());

			if (sqlite3_step(m_sql_stmt) != SQLITE_DONE)
			{
				m_message = "SQL error: ";
				m_message += sqlite3_errmsg(m_sql_cnn);
			}
			else
			{
				m_message = "Person updated successfully.";
			}
		}
		else
		{
			m_message = "No contacts found.";
		}
	}

	void App::deleteContact()
	{
		std::string personId = "";

		std::cout << "Person ID: ";
		std::getline(std::cin, personId);

		m_sql = "SELECT COUNT (personId) FROM PERSON WHERE personId = ? AND userId = ?";

		sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
		sqlite3_bind_text(m_sql_stmt, 1, personId.c_str(), personId.length(), SQLITE_TRANSIENT);
		sqlite3_bind_int(m_sql_stmt, 2, pCurrent->getUserId());
		sqlite3_step(m_sql_stmt);

		if (sqlite3_column_int(m_sql_stmt, 0) == 1)
		{

			m_sql = "DELETE FROM PERSON WHERE personId = ? AND userId = ?";
			sqlite3_prepare_v2(m_sql_cnn, m_sql.c_str(), -1, &m_sql_stmt, nullptr);
			sqlite3_bind_text(m_sql_stmt, 1, personId.c_str(), personId.length(), SQLITE_TRANSIENT);
			sqlite3_bind_int(m_sql_stmt, 2, pCurrent->getUserId());

			if (sqlite3_step(m_sql_stmt) != SQLITE_DONE)
			{
				m_message = "SQL error: ";
				m_message += sqlite3_errmsg(m_sql_cnn);
			}
			else
			{
				m_message = "Person deleted successfully.";
			}
		}
		else
		{
			m_message = "No contacts found.";
		}
	}

	App::~App()
	{
		if (pCurrent != nullptr)
		{
			delete pCurrent;
		}
		sqlite3_finalize(m_sql_stmt);
		sqlite3_close(m_sql_cnn);
	}

}