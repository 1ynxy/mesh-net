#include "config.h"

// Constructors & Destructors

Config::Config() {

}

Config::Config(const Config& config) {
	bools = config.bools;
	ints = config.ints;
	floats = config.floats;
	strings = config.strings;
}

// Member Functions

bool Config::get_bool(const std::string& key) {
	mut.lock();

	if (bools.find(key) != bools.end()) {
		mut.unlock();
		
		return bools[key];
	}

	mut.unlock();

	return false;
}

int Config::get_int(const std::string& key) {
	mut.lock();

	if (ints.find(key) != ints.end()) {
		mut.unlock();
		
		return ints[key];
	}

	mut.unlock();

	return 0;
}

float Config::get_float(const std::string& key) {
	mut.lock();

	if (floats.find(key) != floats.end()) {
		mut.unlock();

		return floats[key];
	}

	mut.unlock();

	return 0.0f;
}

std::string Config::get_string(const std::string& key) {
	mut.lock();

	if (strings.find(key) != strings.end()) {
		mut.unlock();

		return strings[key];
	}

	mut.unlock();

	return "";
}

void Config::set_bool(const std::string& key, bool value, bool overwrite) {
	mut.lock();

	if (!overwrite && bools.find(key) != bools.end()) {
		mut.unlock();

		return;
	}

	bools[key] = value;

	mut.unlock();
}

void Config::set_int(const std::string& key, int value, bool overwrite) {
	mut.lock();

	if (!overwrite && ints.find(key) != ints.end()) {
		mut.unlock();

		return;
	}

	ints[key] = value;

	mut.unlock();
}

void Config::set_float(const std::string& key, float value, bool overwrite) {
	mut.lock();

	if (!overwrite && floats.find(key) != floats.end()) {
		mut.unlock();

		return;
	}

	floats[key] = value;

	mut.unlock();
}

void Config::set_string(const std::string& key, const std::string& value, bool overwrite) {
	mut.lock();

	if (!overwrite && strings.find(key) != strings.end()) {
		mut.unlock();

		return;
	}

	strings[key] = value;

	mut.unlock();
}

bool Config::load(const std::string& path) {
	int state = 0;

	std::string data = "";

	if (!file.read(path, data)) return false;

	// Load Data

	std::string type = "", key = "", value = "";

	for (char c : data) {
		if (state != 2 && c == ' ') continue;

		if (state == 0) {
			if (c == ':') state++;
			else type += c;
		}
		else if (state == 1) {
			if (c == '=') state++;
			else key += c;
		}
		else if (state == 2) {
			if (c == '\n') {
				state = 0;

				if (type == "b" && (value == "true" || value == "on" || value == "1")) set_bool(key, true);
				else if (type == "i") set_int(key, std::stoi(value));
				else if (type == "f") set_float(key, std::stof(value));
				else set_string(key, value);

				type = key = value = "";
			}
			else value += c;
		}
	}

	return true;
}
