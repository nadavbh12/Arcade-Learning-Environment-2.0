//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: Settings.cxx,v 1.125 2007/08/22 13:55:40 stephena Exp $
//============================================================================

#include <cassert>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
using namespace std;

//#include "OSystem.hxx"
#include "bspf.hxx"
#include "Settings.hxx"
#include "RleSystem.hxx"

using namespace rle;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings::Settings(){
    setDefaultSettings();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings::~Settings() {}

//void Settings::loadConfig(const char* config_file) {
//	string line, key, value;
//	string::size_type equalPos, garbage;
//
//	ifstream in(config_file);
//	if (!in || !in.is_open()) {
//		rle::Logger::Warning << "Warning: couldn't load settings file: "
//				<< config_file << std::endl;
//		return;
//	}
//
//	while (getline(in, line)) {
//		// Strip all whitespace and tabs from the line
//		while ((garbage = line.find("\t")) != string::npos)
//			line.erase(garbage, 1);
//
//		// Ignore commented and empty lines
//		if ((line.length() == 0) || (line[0] == ';'))
//			continue;
//
//		// Search for the equal sign and discard the line if its not found
//		if ((equalPos = line.find("=")) == string::npos)
//			continue;
//
//		// Split the line into key/value pairs and trim any whitespace
//		key = line.substr(0, equalPos);
//		value = line.substr(equalPos + 1, line.length() - key.length() - 1);
//		key = trim(key);
//		value = trim(value);
//
//		// Check for absent key or value
//		if ((key.length() == 0) || (value.length() == 0))
//			continue;
//	}
//
//	in.close();
//}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//void Settings::loadConfig() {
//	loadConfig(myRleSystem->configFile().c_str());
//}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//string Settings::loadCommandLine(int argc, char** argv) {
//	for (int i = 1; i < argc; ++i) {
//		// strip off the '-' character
//		string key = argv[i];
//		if (key[0] == '-') {
//			key = key.substr(1, key.length());
//
//			// Take care of the arguments which are meant to be executed immediately
//			// (and then Stella should exit)
//			if (key == "help" || key == "listrominfo") {
//				usage();
//				setExternal(key, "true");
//				return "";
//			}
//
//			// Take care of arguments without an option
//			if (key == "rominfo" || key == "debug" || key == "holdreset"
//					|| key == "holdselect" || key == "holdbutton0") {
//				setExternal(key, "true");
//				continue;
//			}
//
//			if (++i >= argc) {
//				rle::Logger::Error << "Missing argument for '" << key << "'"
//						<< endl;
//				return "";
//			}
//			string value = argv[i];
//			setExternal(key, value);
//		} else
//			return key;
//	}
//
//	return "";
//}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::usage() {

	cerr
			<< "\n"
			" ***************************************************************************\n"
			" * Welcome to A.L.E (Arcade Learning Environment)\n"
			" * (Powered by LibRetro)\n"
			" ***************************************************************************\n"
			"\n"
			" Usage: rle [options ...] -core_file corefile romfile\n"
			"\n"
			" Main arguments:\n"
			"   -help -- prints out help information\n"
			"   -game_controller [fifo|fifo_named"
#ifdef __USE_RLGLUE
			"|rlglue"
#endif
			"] (default: unset)\n"
			"      Defines how Stella communicates with the player agent:\n"
			"            - 'fifo':       Control occurs through FIFO pipes\n"
			"            - 'fifo_named': Control occurs through named FIFO pipes\n"
#ifdef __USE_RLGLUE
			"            - 'rlglue':     External control via RL-Glue\n"
#endif
			"   -random_seed [n|time] (default: time)\n"
			"     Sets the seed used for random number generation\n"
#ifdef __USE_SDL
			"   -display_screen [true|false] (default: false)\n"
			"     Displays the game screen\n"
#endif
#ifdef SOUND_SUPPORT
			"   -sound [true|false] (default: false)\n"
			"     Enable game sounds\n"
#endif
			"\n"
			" Environment arguments:\n"
			"   -max_num_frames m (default: 0)\n"
			"     The program will quit after this number of frames. 0 means never.\n"
			"   -max_num_frames_per_episode m (default: 0)\n"
			"     Ends each episode after this number of frames. 0 means never.\n"
			"   -color_averaging [true|false] (default: false)\n"
			"     Phosphor blends screens to reduce flicker\n"
			"   -record_screen_dir [save_directory]\n"
			"     Saves game screen images to save_directory\n"
			"   -repeat_action_probability (default: 0.25)\n"
			"     Stochasticity in the environment. It is the probability the previous "
			"action will repeated without executing the new one.\n"
			"\n"
			" FIFO Controller arguments:\n"
			"   -run_length_encoding [true|false] (default: true)\n"
			"     Encodes data using run-length encoding\n"
			"\n"
#ifdef __USE_RLGLUE
			" RL-Glue Controller arguments:\n"
			"   -send_rgb [true|false] (default: false)\n"
			"     Sends RGB values for each pixel instead of the pallette index values\n"
			"   -restricted_action_set [true|false] (default: false)\n"
			"     Agents use a smaller set of actions (RL-Glue interfaces only)\n"
			"\n"
#endif
			" Misc. arguments:\n"
			"   -ld [A/B] (default: B)\n"
			"     Left player difficulty. B means easy.\n"
			"   -rd [A/B] (default: B)\n"
			"     Right player difficulty. B means easy.\n"
			"\n"; // Closing the std::cerr statement
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//void Settings::saveConfig(const char* outFile) {
//	ofstream out(outFile);
//	if (!out || !out.is_open()) {
//		rle::Logger::Error << "Error: Couldn't save settings file\n";
//		return;
//	}
//
//	out << ";  Stella configuration file" << endl << ";" << endl
//			<< ";  Lines starting with ';' are comments and are ignored."
//			<< endl << ";  Spaces and tabs are ignored." << endl << ";" << endl
//			<< ";  Format MUST be as follows:" << endl << ";    command = value"
//			<< endl << ";" << endl
//			<< ";  Commmands are the same as those specified on the commandline,"
//			<< endl << ";  without the '-' character." << endl << ";" << endl
//			<< ";  Values are the same as those allowed on the commandline."
//			<< endl
//			<< ";  Boolean values are specified as 1 (or true) and 0 (or false)"
//			<< endl << ";" << endl;
//
//	out.close();
//}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setInt(const string& key, const int value) {
	verifyVariableExistence(intSettings, key);
	intSettings.at(key) =  value;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setFloat(const string& key, const float value) {
	verifyVariableExistence(floatSettings, key);
	floatSettings.at(key) =  value;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setBool(const string& key, const bool value) {
	verifyVariableExistence(boolSettings, key);
	boolSettings.at(key) =  value;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setString(const string& key, const string& value) {
	verifyVariableExistence(stringSettings, key);
	stringSettings.at(key) =  value;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::getInt(const string& key, bool strict) const {
	try{
		return intSettings.at(key);
	}catch(std::out_of_range&){
		if (strict) {
			rle::Logger::Error << "No value found for key: " << key << ". ";
			rle::Logger::Error << "Make sure all the settings files are loaded."
					<< endl;
			exit(-1);
		} else {
			return -1;
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
float Settings::getFloat(const string& key, bool strict) const {
	try{
		return floatSettings.at(key);
	}catch(std::out_of_range&){
		if (strict) {
			rle::Logger::Error << "No value found for key: " << key << ". ";
			rle::Logger::Error << "Make sure all the settings files are loaded."
					<< endl;
			exit(-1);
		} else {
			return -1.0;
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Settings::getBool(const string& key, bool strict) const {
	try{
		return boolSettings.at(key);
	}catch(std::out_of_range&){
		if (strict) {
			rle::Logger::Error << "No value found for key: " << key << ". ";
			rle::Logger::Error << "Make sure all the settings files are loaded."
					<< endl;
			exit(-1);
		} else {
			return false;
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const string& Settings::getString(const string& key, bool strict) const {
	try{
		return stringSettings.at(key);
	}catch(std::out_of_range&){
		if (strict) {
			rle::Logger::Error << "No value found for key: " << key << ". ";
			rle::Logger::Error << "Make sure all the settings files are loaded."
					<< endl;
			exit(-1);
		} else {
			static std::string EmptyString("");
			return EmptyString;
		}
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setDefaultSettings() {

	// Controller settings
	intSettings.emplace(std::make_pair("max_num_frames", 0));
	intSettings.emplace(std::make_pair("max_num_frames_per_episode", 0));

	// FIFO controller settings
	boolSettings.emplace(std::make_pair("run_length_encoding", true));

	// Environment customization settings
	boolSettings.emplace(std::make_pair("restricted_action_set", false));
	intSettings.emplace("random_seed", 0);
	boolSettings.emplace(std::make_pair("color_averaging", true));
	boolSettings.emplace(std::make_pair("send_rgb", false));
	intSettings.emplace(std::make_pair("frame_skip", 1));
	floatSettings.emplace(std::make_pair("repeat_action_probability", 0.25));
	stringSettings.emplace(std::make_pair("rom_file", ""));
	stringSettings.emplace(std::make_pair("core_file", ""));
	boolSettings.emplace(std::make_pair("two_players", false));

	// Record settings
	intSettings.emplace(std::make_pair("fragsize", 64)); // fragsize to 64 ensures proper sound sync
	stringSettings.emplace(std::make_pair("record_screen_dir", ""));
	stringSettings.emplace(std::make_pair("record_sound_filename", ""));

	// Display Settings
	boolSettings.emplace(std::make_pair("display_screen", false));
	boolSettings.emplace(std::make_pair("sound", false));

	// Game-Specific Settings

	// Mortal Kombat
	boolSettings.emplace(std::make_pair("MK_random_position", true));
	stringSettings.emplace(std::make_pair("MK_difficulty", "medium"));
	stringSettings.emplace(std::make_pair("MK_player1_character", "cage"));
	stringSettings.emplace(std::make_pair("MK_player2_character", "scorpion"));
	intSettings.emplace(std::make_pair("MK_opponent_character", 0));

  // Streets Of Rage 2
  boolSettings.emplace(std::make_pair("SOR2_test", false));
  intSettings.emplace(std::make_pair("SOR2_start_level", 1));
  intSettings.emplace(std::make_pair("SOR2_end_level", 1));
  intSettings.emplace(std::make_pair("SOR2_difficulty", 1));
  intSettings.emplace(std::make_pair("SOR2_lives", 3));
  stringSettings.emplace(std::make_pair("SOR2_player_1_character", "max"));
  stringSettings.emplace(std::make_pair("SOR2_game_type", "single"));

  // Super Mario Kart
  stringSettings.emplace(std::make_pair("SUPER_MARIO_KART_player1_character", "bowser"));

  //Super Double Dragon
  stringSettings.emplace(std::make_pair("DDRAGON_reward_strategy", "score"));
        
	for (map<string, string>::iterator it = stringSettings.begin();
			it != stringSettings.end(); it++) {
		this->setString(it->first, it->second);
	}

	for (map<string, float>::iterator it = floatSettings.begin();
			it != floatSettings.end(); it++) {
		this->setFloat(it->first, it->second);
	}

	for (map<string, bool>::iterator it = boolSettings.begin();
			it != boolSettings.end(); it++) {
		this->setBool(it->first, it->second);
	}

	for (map<string, int>::iterator it = intSettings.begin();
			it != intSettings.end(); it++) {
		this->setInt(it->first, it->second);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<typename ValueType>
void Settings::verifyVariableExistence(map<string, ValueType> dict,
		string key) {
	if (dict.find(key) == dict.end()) {
		throw std::runtime_error(
				"The key " + key + " you are trying to set does not exist.\n");
	}
}
