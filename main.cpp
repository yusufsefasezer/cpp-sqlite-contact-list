#include "App.h"

using namespace ContactList;

int main() {

	App *pContactListe = new App();

	pContactListe->run();

	delete pContactListe;

	return 0;
}