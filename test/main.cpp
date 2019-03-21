#define LIB_MULTI_WSTRING
#include "../src/multilingual.h"
#include <iostream>
#include <string>

void writekeys(multilingual::Translator& t) {
	t.load();
	std::cout << "Fell back? " << (t.fellback() ? "Yes.\n" : "No.\n");
	std::wcout << t.get(L"user.name") << '\n';
	std::wcout << t.get(L"user.age") << '\n';
	std::wcout << t[LM_KEY(user.height)] << '\n';
	std::wcout << t.get(L"gb_key") << '\n';
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

	local_string username = LMS("");
	local_string userage = LMS("");
	local_string userheight = LMS("");
	local_string gb_key = LMS("");

	lm_listener_array larr = {
		{ username, LM_KEY(user.name) },
		{ userage, LM_KEY(user.age) },
		{ userheight, LM_KEY(user.height) }
	};

	t.setlisteners(larr);

	t.addlistener(gb_key, LM_KEY(gb_key));

	t.enable_listeners();

	t.load();

	std::wcout << username << '\n' << userage << '\n' << userheight << '\n' << gb_key << '\n';

	t.setdir("lang/");
	t.setlang("en_GB");

	t.load();

	std::wcout << username << '\n' << userage << '\n' << userheight << '\n' << gb_key << '\n';

	return 0;
}
