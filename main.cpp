#include "App.h"

int main(int argc, char const *argv[])
{
	std::shared_ptr<ContactList::App> app(new ContactList::App());

	app->run();

	return 0;
}