#include "multilingual.h"

// shorten the use of multilingual::basic_translator<_str>::val_t and such
// this is undef'd at the end
#define T_TYPE(t) typename multilingual::basic_translator<_str>::t

template<typename _str>
inline _str& str_trim(_str& s) {
	// trim right
	s.erase(s.find_last_not_of((const typename _str::value_type *)" ") + 1);
	// trim left
	s.erase(0, s.find_first_not_of((const typename _str::value_type *)" "));
	return s;
};

template<typename _str>
unsigned int multilingual::basic_translator<_str>::load() {
	_fellback = false; // if this isn't the first time loading a language...
	if (!_strvals.empty())
		_strvals.clear();
	s_ifstream infile(_dir + _lang + ".lang");
	if (!infile) {
		if (_lang == _fallback)
			throw std::runtime_error(_dir + _lang + ".lang could not be loaded.\n");

		infile = s_ifstream(_dir + _fallback + ".lang");
		if (!infile) {
			throw std::runtime_error("Neither " + _dir + _lang + ".lang nor " + _dir + _fallback + ".lang could be found.");
		}

		_fellback = true;
	}

	// load from the file
	unsigned int count = 0;
	_str line, nkey, nval;
	s_sstream ss;
	while (std::getline(infile, line)) {
		bool valid = false;
		for (int i = 0; i < line.length(); i++) {
			if (line[i] == (typename _str::value_type)'=') {
				valid = true;
				break;
			}
		}
		if (!valid) continue;

		// grab the key
		ss = s_sstream(line);
		std::getline(ss, nkey, (typename _str::value_type)'=');

		// trim whitespace from the start and end of the key
		// whitespace inside the key is not recommended
		// it will be replaced with underscores
		str_trim(nkey);
		for (std::size_t i = 0; i < nkey.length(); i++) {
			if (nkey[i] == (typename _str::value_type)' ')
				nkey[i] = (typename _str::value_type)'_';
		}

		if (nkey.empty())
			continue;

		// grab the value
		std::getline(ss, nval, (typename _str::value_type)'\n');
		
		str_trim(nval);

		_strvals[nkey] = nval;

		nkey.clear();
		nval.clear();

		count++;
	}

	infile.close();

	if (_updatelisteners)
		update_listeners();

	return count;
};

template<typename _str>
void multilingual::basic_translator<_str>::setdir(std::string newdir) {
	_dir = newdir;
};

template<typename _str>
void multilingual::basic_translator<_str>::setfallback(std::string newlang) {
	_fallback = newlang;
};

template<typename _str>
void multilingual::basic_translator<_str>::setlang(std::string language) {
	_lang = language;
};

template<typename _str>
T_TYPE(val_t) multilingual::basic_translator<_str>::get(T_TYPE(key_t) key) const {
	try {
		return _strvals.at(key);
	} catch (const std::out_of_range& e) {
		return key;
	}
};

template<typename _str>
T_TYPE(val_t) multilingual::basic_translator<_str>::operator[] (T_TYPE(key_t) key) const {
	return get(key);
};

template<typename _str>
void multilingual::basic_translator<_str>::setlisteners(T_TYPE(listener_array_t) & listeners) {
	_listeners = listener_array_t(listeners);
	_updatelisteners = true;
};

template<typename _str>
void multilingual::basic_translator<_str>::addlistener(multilingual::Listener<_str> listener) {
	_listeners.push_back(listener);
};

template<typename _str>
void multilingual::basic_translator<_str>::addlistener(T_TYPE(val_t) & str, T_TYPE(key_t) key) {
	_listeners.push_back({ str, key });
};

template<typename _str>
bool multilingual::basic_translator<_str>::update_listeners() {
	if (!_updatelisteners) return false;

	for (auto& l : _listeners) {
		l.str = get(l.key);
	}

	return true;
};

template<typename _str>
bool multilingual::basic_translator<_str>::enable_listeners() {
	if (_updatelisteners) return false;

	_updatelisteners = true;

	return true;
};

template<typename _str>
bool multilingual::basic_translator<_str>::disable_listeners() {
	if (!_updatelisteners) return false;
	
	_updatelisteners = false;

	return true;
};

template<typename _str>
bool multilingual::basic_translator<_str>::fellback() const {
	return _fellback;
};

template<typename _str>
bool multilingual::basic_translator<_str>::haskey(T_TYPE(key_t) key) const {
	try {
		T_TYPE(val_t) r = _strvals.at(key);
		return true;
	} catch (const std::out_of_range& e) {
		return false;
	}
};

#undef T_TYPE
