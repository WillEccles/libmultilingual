#ifndef LIB_MULTILINGUAL_H
#define LIB_MULTILINGUAL_H

#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// this is used to allow for a sort of tokenization
// i.e. ML_KEY(myclass.name) directly translates to "myclass.name"
// this is likely to be unused for 99% of usecases, but it's here
#define LM_KEY(key) #key

typedef std::string	lm_lang_id;	// language type
typedef std::string	lm_dir;	// directory type

#ifdef LIB_MULTI_WSTRING // force use of std::wstring instead of std::string
typedef std::wstring	lm_str;
#else
typedef std::string		lm_str;
#endif

typedef lm_str		lm_key;	// string dictionary key type
typedef lm_str		lm_val;	// string dictionary value type

typedef std::map<lm_key, lm_val>	lm_dict;

namespace multilingual {
	struct Listener {
		lm_val	&str; // the string to update
		lm_key	key; // the key to use to update the string
	};
};

typedef std::vector<multilingual::Listener>	lm_listener_array;

namespace multilingual {
	class Translator {
	  private:
		lm_dir		_dir = "./";
		lm_lang_id	_fallback = "en_US";
		lm_lang_id	_lang = "en_US";

		lm_dict _strvals;

		lm_listener_array _listeners; // update listeners

		bool _updatelisteners = false;

		bool _fellback = false; // whether or not we actually had to use the fallback language
		
	  public:
		Translator(lm_lang_id lang): _lang(lang) {};
		Translator(lm_lang_id lang, lm_lang_id fallback): _lang(lang), _fallback(fallback) {};
		Translator(lm_lang_id lang, lm_lang_id fallback, lm_dir dir): _lang(lang), _fallback(fallback), _dir(dir) {};

		// load languages files (.lang) in the directory
		// - returns the number of key/value pairs found
		// - if update listeners are enabled, this will update them accordingly
		// - throws std::runtime_error if neither the language file nor the fallback
		// could be loaded.
		unsigned int load();

		// set a new directory
		void setdir(lm_dir newdir);

		// set a new fallback language
		void setfallback(lm_lang_id newlang);

		// change which language to load
		// - this does not load the file!
		void setlang(lm_lang_id language);

		// get a string by the given key
		// - returns empty string if not found
		lm_val get(lm_key key) const;

		// get a string by the given key
		// - returns empty string when not found
		lm_val operator[] (lm_key key) const;

		// set the listeners array and set to update
		void setlisteners(lm_listener_array& listeners);

		// add a new listener to the list
		void addlistener(multilingual::Listener listener);
		void addlistener(lm_val& str, lm_key key);

		// force all listeners to be updated
		// - returns false if listeners are disabled
		bool update_listeners();

		// enable listeners
		// - if listeners were already enabled, returns false
		bool enable_listeners();

		// disable update listeners
		// - returns false if listeners were not enabled to start with
		bool disable_listeners();

		// whether or not the fallback language had to be used
		bool fellback() const;
	};

	namespace util {
		inline lm_str& str_trim(std::string& s) {
			// trim right
			s.erase(s.find_last_not_of(" ") + 1);
			// trim left
			s.erase(0, s.find_first_not_of(" "));
			return s;
		};
	};
};

// UNDEF THIS AT THE END!!!
#define MTrans multilingual::Translator

unsigned int MTrans::load() {
	_fellback = false; // if this isn't the first time loading a language...
	if (!_strvals.empty())
		_strvals.clear();
	std::ifstream infile(_dir + _lang + ".lang");
	if (!infile) {
		if (_lang == _fallback)
			throw std::runtime_error(_dir + _lang + ".lang could not be loaded.\n");

		infile = std::ifstream(_dir + _fallback + ".lang");
		if (!infile) {
			throw std::runtime_error("Neither " + _dir + _lang + ".lang nor " + _dir + _fallback + ".lang could be found.");
		}

		_fellback = true;
	}

	// load from the file
	unsigned int count = 0;
	std::string line, nkey, nval;
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

		// grab the key
		ss = std::stringstream(line);
		std::getline(ss, nkey, '=');

		// trim whitespace from the start and end of the key
		// whitespace inside the key is not recommended
		// it will be replaced with underscores
		multilingual::util::str_trim(nkey);
		for (std::size_t i = 0; i < nkey.length(); i++) {
			if (nkey[i] == ' ')
				nkey[i] = '_';
		}

		if (nkey.empty())
			continue;

		// grab the value
		std::getline(ss, nval, '\n');
		
		multilingual::util::str_trim(nval);

		_strvals[nkey] = nval;

		nkey.clear();
		nval.clear();

		count++;
	}

	if (_updatelisteners)
		update_listeners();

	return count;
};

void MTrans::setdir(lm_dir newdir) {
	_dir = newdir;
};

void MTrans::setfallback(lm_lang_id newlang) {
	_fallback = newlang;
};

void MTrans::setlang(lm_lang_id language) {
	_lang = language;
};

lm_val MTrans::get(lm_key key) const {
	try {
		return _strvals.at(key);
	} catch (const std::out_of_range& e) {
		return key;
	}
};

lm_val MTrans::operator[] (lm_key key) const {
	return get(key);
};

void MTrans::setlisteners(lm_listener_array& listeners) {
	_listeners = lm_listener_array(listeners);
	_updatelisteners = true;
};

void MTrans::addlistener(multilingual::Listener listener) {
	_listeners.push_back(listener);
};

void MTrans::addlistener(lm_val& str, lm_key key) {
	_listeners.push_back({ str, key });
};

bool MTrans::update_listeners() {
	if (!_updatelisteners) return false;

	for (auto& l : _listeners) {
		l.str = get(l.key);
	}

	return true;
};

bool MTrans::enable_listeners() {
	if (_updatelisteners) return false;

	_updatelisteners = true;

	return true;
};

bool MTrans::disable_listeners() {
	if (!_updatelisteners) return false;
	
	_updatelisteners = false;

	return true;
};

bool MTrans::fellback() const {
	return _fellback;
};

#undef MTrans

#endif
