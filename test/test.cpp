#include "multilingual.h"
#include <iostream>
#include <string>

using translator = multilingual::Translator;

void writekeys(translator& t) {
	t.load();
	std::cout << "Fell back? " << (t.fellback() ? "Yes.\n" : "No.\n");
	std::cout << t.get("user.name") << '\n';
	std::cout << t.get("user.age") << '\n';
	std::cout << t[LM_KEY(user.height)] << '\n';
	std::cout << t.get("gb_key") << '\n';
}

int main(void) {
	std::cout << "Loading en_US.lang\n";
	translator t("en_US");
	writekeys(t);
	std::cout << "Loading lang/en_GB.lang\n";
	t.setdir("lang/");
	t.setlang("en_GB");
	writekeys(t);
	std::cout << "Loading dummy file...\n";
	t.setfallback("en_GB");
	t.setlang("en_NOWHERE");
	writekeys(t);


	std::cout << "Testing listeners.\n";
	t.setdir("./");
	t.setlang("en_US");
	t.setfallback("en_US");

	std::string username = "";
	std::string userage = "";
	std::string userheight = "";
	std::string gb_key = "";

	translator::listener_array_t larr = {
		{ username, LM_KEY(user.name) },
		{ userage, LM_KEY(user.age) },
		{ userheight, LM_KEY(user.height) }
	};

	t.setlisteners(larr);

	t.addlistener(gb_key, LM_KEY(gb_key));

	t.enable_listeners();

	t.load();

	std::cout << username << '\n' << userage << '\n' << userheight << '\n' << gb_key << '\n';

	t.setdir("lang/");
	t.setlang("en_GB");

	t.load();

	std::cout << username << '\n' << userage << '\n' << userheight << '\n' << gb_key << '\n';

	return 0;
}
