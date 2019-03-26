#ifndef LIB_MULTILINGUAL_H
#define LIB_MULTILINGUAL_H

#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// TODO: (maybe) allow for "categories" of some sort
// TODO: allow for combining multiple languages into one dictionary

// expands to the proper type of string constant
// for example, LM_KEY(class.member) becomes "class.member"
// also, LM_WKEY(class.member) becomes L"class.member"
#define LM_KEY(key) #key
#define LM_WKEY(key) L###key

namespace multilingual {
	/* Type Declarations */

	// class used to localize strings
	// using the typedefs found below is highly recommended
	// Translator = basic_translator<std::string>
	// WTranslator = basic_translator<std::wstring>
	template<typename _str>
	class basic_translator;

	// struct type used to encapsulate a string update listener
	template<typename _str>
	struct Listener;

	/* Convenience typedefs. */
	typedef basic_translator<std::string>	Translator; // standard std::string Translator class
	typedef basic_translator<std::wstring>	WTranslator; // std::wstring Translator class

	/* Type Definitions */

	template<typename _str>
	class basic_translator {
	  public:
		typedef _str	string_t;
		typedef _str	key_t;
		typedef _str	val_t;
		typedef std::vector<multilingual::Listener<_str> > listener_array_t;

	  private:
		typename _str::value_type	s_char_t;
		typedef std::basic_ifstream<typename _str::value_type>	s_ifstream;
		typedef std::basic_stringstream<typename _str::value_type>	s_sstream;

		std::string	_dir = "./";
		std::string	_lang = "en_US";
		std::string	_fallback = "en_US";

		std::map<key_t, val_t> _strvals;

		listener_array_t _listeners; // update listeners

		bool _updatelisteners = false;

		bool _fellback = false; // whether or not we actually had to use the fallback language
		
	  public:
		basic_translator(std::string lang): _lang(lang) {};
		basic_translator(std::string lang, std::string fallback): _lang(lang), _fallback(fallback) {};
		basic_translator(std::string lang, std::string fallback, std::string dir): _lang(lang), _fallback(fallback), _dir(dir) {};

		// load languages files (.lang) in the directory
		// - returns the number of key/value pairs found
		// - if update listeners are enabled, this will update them accordingly
		// - throws std::runtime_error if neither the language file nor the fallback
		//   could be loaded.
		unsigned int load();

		// set a new directory
		void setdir(std::string newdir);

		// set a new fallback language
		void setfallback(std::string newlang);

		// change which language to load
		// - this does not load the file!
		void setlang(std::string language);

		// get a string by the given key
		// - returns empty string if not found
		val_t get(key_t key) const;

		// get a string by the given key
		// - returns empty string when not found
		val_t operator[] (key_t key) const;

		// set the listeners array and set to update
		void setlisteners(listener_array_t& listeners);

		// add a new listener to the list
		void addlistener(multilingual::Listener<_str> listener);
		void addlistener(val_t& str, key_t key);

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

		// returns whether or not the dictionary contains a given key
		bool haskey(key_t key) const;
	};
	
	template<typename _str>
	struct Listener {
		_str	&str; // the string to update
		_str	key; // the key to use to update the string
	};
};

#endif
