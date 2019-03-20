#ifndef LIB_MULTILINGUAL_H
#define LIB_MULTILINGUAL_H

#include <map>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>

typedef lm_lang_id	std::string;	// language type
typedef lm_dir		std::string;	// directory type

#ifdef LIB_MULTI_WSTRING // force use of std::wstring instead of std::string
typedef lm_key		std::wstring;	// string dictionary key type
typedef lm_val		std::wstring;	// string dictionary value type
#else
typedef lm_key		std::string;
typedef lm_val		std::string;
#endif

typedef lm_listener_array std::map<lm_val&, lm_key>; // map refs to strings with their values
// this would be used if you want your strings to be updated automagically

namespace multilingual {
	class Translator {
	  private:
		lm_dir		_dir = "./";
		lm_lang_id	_fallback = "en_US";
		lm_lang_id	_lang = "en_US";

		std::map<lm_key, lm_val> _strvals;
		std::map<lm_key, lm_val> _fallbackvals; // en_US by default

		lm_listener_array& _listeners; // update listeners

		bool _updatelisteners = false;

		bool _fellback = false; // whether or not we actually had to use the fallback language
		
	  public:
		Translator(lm_lang_id lang): _lang(lang) {};
		Translator(lm_lang_id lang, lm_lang_id fallback): _lang(lang), _fallback(fallback) {};
		Translator(lm_lang_id lang, lm_lang_id fallback, lm_dir dir): _lang(lang), _fallback(fallback), _dir(dir) {};

		// load languages files (.lang) in the directory
		// returns the number of key/value pairs found
		// if update listeners are enabled, this will update them accordingly
		unsigned int load();

		// change which language to load
		// this does not load the file!
		void setlang(lm_lang_id language);

		// get a string by the given key
		// returns empty string if not found
		lm_val get(lm_key key) const;

		// get a string by the given key
		// returns empty string when not found
		lm_val operator[] (lm_key key) const;

		// set the listeners array and set to update
		void setlisteners(lm_listener_array &listeners);

		// force all listeners to be updated
		// returns false if listeners are disabled
		bool update_listeners();

		// disable update listeners
		// returns false if listeners were not enabled to start with
		bool disable_listeners();

		// whether or not the fallback language had to be used
		bool fellback() const;
	};
};

using namespace multilingual;

unsigned int Translator::load() {
	std::ifstream infile(_dir + _lang + ".lang");
	if (!infile) {
		infile = std::ifstream(_dir + _fallback + ".lang");
		if (!infile) {
			throw std::runtime_error("Neither " + _dir + _lang + ".lang nor " + _dir + _fallback + ".lang could be found.");
		}

		_fellback = true;
	}

	// load from the file
	unsigned int count = 0;
	std::string line, temp;
	std::stringstream ss;
	while (std::getline(infile, line)) {
		bool valid = false;
		for (int i = 0; i < line.length(); i++) {
			if (line[i] == '=') {
				valid = true;
				break;
			}
		}
		if (!valid) continue;

		// process the line
		ss = std::stringstream(line);
		std::getline(ss, temp, '\n');
	}
};

void Translator::setlang(lm_lang_id language) {
	_lang = language;
};

lm_val Translator::get(lm_key key) const {
	return _strvals[key];
};

lm_val Translator::operator[] (lm_key key) const {
	return get(key);
};

void Translator::setlisteners(lm_listener_array &listeners) {
	_listeners(listeners);
	_updatelisteners = true;
};

bool Translator::update_listeners() {
	if (!_updatelisteners) return false;

	// TODO

	return true;
};

bool Translator::disable_listeners() {
	if (!_updatelisteners) return false;
	
	_updatelisteners = false;

	return true;
};

bool Translator::fellback() {
	return _fellback;
};

#endif
