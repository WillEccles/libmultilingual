#ifndef LIB_MULTILINGUAL_H
#define LIB_MULTILINGUAL_H

#include <map>
#include <string>
#include <fstream>

namespace multilingual {
	class Translator {
		std::string _dir = "";
		std::string _fallback = "en_US";
		std::string _lang;

		std::map<std::string, std::string> strvals;
		
	  public:
		Translator(std::string lang): _lang(lang) {};
		Translator(std::string lang, std::string fallback): _lang(lang), _fallback(fallback) {};
		Translator(std::string lang, std::string fallback, std::string dir): _lang(lang), _fallback(fallback), _dir(dir) {};

		// load languages files (.lang) in the directory
		// returns the number of key/value pairs found
		unsigned int load() {
			
		};

		// get a string by the given key
		// returns empty string if not found
		const std::string get(std::string key) {
			return strvals[key];
		};

		// get a string by the given key
		// returns empty string when not found
		const std::string operator[] (std::string key) {
			return get(key);
		};
	};
};

#endif
