#include "../src/multilingual.h"
#include <iostream>
#include <string>

void writekeys(multilingual::Translator& t) {
	t.load();
	std::cout << "Fell back? " << (t.fellback() ? "Yes.\n" : "No.\n");
	std::cout << t.get("user.name") << '\n';
	std::cout << t.get("user.age") << '\n';
	std::cout << t["user.height"] << '\n';
	std::cout << t.get("gb_key") << '\n';
}

int main(void) {
	std::cout << "Loading en_US.lang\n";
	multilingual::Translator t("en_US");
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

	lm_listener_array larr = {
		{ username, "user.name" },
		{ userage, "user.age" },
		{ userheight, "user.height" }
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
