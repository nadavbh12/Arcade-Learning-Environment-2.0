/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "../RomUtils.hpp"
#include <iomanip>

#include "../RomUtils.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

#include "StreetsOfRage.hpp"
#include <iostream>


using namespace rle;


StreetsOfRageSettings::StreetsOfRageSettings() {
    reset();


 
      minimalActions = {JOYPAD_NOOP,
    				// JOYPAD_DOWN,      //Walk down
   					// JOYPAD_UP,        // Walk up
						// JOYPAD_LEFT,      // Walk left
						JOYPAD_RIGHT,     // Walk right
						
//						JOYPAD_GENESIS_A,  // Special attack
						// JOYPAD_GENESIS_B,  // Regular attack

  					// JOYPAD_GENESIS_C,  // Jump
						
  					// JOYPAD_GENESIS_C | JOYPAD_LEFT, // Jump left
						JOYPAD_GENESIS_C | JOYPAD_RIGHT, // Jump right

						// JOYPAD_GENESIS_B | JOYPAD_GENESIS_C,   // Rear attack or Super slam 
						// JOYPAD_GENESIS_B | JOYPAD_LEFT,//Blitz attack or  
						// JOYPAD_GENESIS_B | JOYPAD_RIGHT,
						// JOYPAD_GENESIS_B | JOYPAD_GENESIS_C | JOYPAD_DOWN,//Drop attack

    };
}


/* create a new instance of the rom */
RomSettings* StreetsOfRageSettings::clone() const {
    RomSettings* rval = new StreetsOfRageSettings();
    *rval = *this;
    return rval;
}


void StreetsOfRageSettings::step(const RleSystem& system) {
//  Read out current score, health, lives, stage progress
	reward_t score = 0; 	
  reward_t health = readRam(&system, 0xB832);
  reward_t screen = readRam(&system, 0xFF00);
  reward_t distance = getDecimalScore(0xB810, &system);
  m_lives = readRam(&system, 0xFF21);
	
// update the reward
	m_reward = score;

//	Update terminal status
  if ((m_lives == 0) && (health == 0) && (screen == 22)){
      std::cout << "fail" << std::endl;
      m_terminal = true;
  }

  // Get level information
	m_current_level = readRam(&system, 0xFF02) + 1;
  int m_end_level = system.settings()->getInt("SOR_end_level");
  
  // Update terminal status
  if ((m_current_level == m_end_level) && (screen == 24)){
       m_terminal=true;
  }
}
/* reset the state of the game */
void StreetsOfRageSettings::reset(){
    m_reward   = 0;
//    m_score    = 0;
    m_terminal = false;
    m_distance = 0;
}


/* saves the state of the rom settings */
void StreetsOfRageSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}


// loads the state of the rom settings
void StreetsOfRageSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect StreetsOfRageSettings::getStartingActions(const RleSystem& system){
	int num_of_nops(100);
	ActionVect startingActions;
	
//	wait for intro to end
	INSERT_NOPS(5*num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_START)
	INSERT_NOPS(0.4*num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_START)


  //	select 1 player
  INSERT_NOPS(1*num_of_nops)
  INSERT_ACTION_SINGLE_A(JOYPAD_START)
  INSERT_NOPS(4*num_of_nops)


//	choose character
	string player_1_character = system.settings()->getString("SOR_player_1_character");
	if("adam" == player_1_character){
 		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}else if("axel" == player_1_character){
		INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}else if("blaze" == player_1_character){
		INSERT_ACTION_SINGLE_A(JOYPAD_LEFT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}

  INSERT_NOPS(0.5 * num_of_nops)

	return startingActions;
}



void StreetsOfRageSettings::startingOperations(RleSystem& system){
  //set number of lives (Can be 1, 3, 5, or 7)
  int num_lives = system.settings()->getInt("SOR_lives"); 
  writeRam(&system, 0xFFCA, ((num_lives - 1) / 2 ) * 0x1); 

  //set difficulty
	int difficulty = system.settings()->getInt("SOR_difficulty");
	if(1 == difficulty){
		writeRam(&system, 0xFFC6, 0x0);
	}else if(2  == difficulty){
		writeRam(&system, 0xFFC6, 0x1);
	}else if(3 == difficulty){
		writeRam(&system, 0xFFC6, 0x2);
	}else if(4 == difficulty){
		writeRam(&system, 0xFFC6, 0x3);
	}

	//set start level	
	int start_level = system.settings()->getInt("SOR_start_level");
	writeRam(&system, 0xFF02, (start_level-1)* 0x1);

}

ActionVect StreetsOfRageSettings::getExtraActions(const RleSystem& system){
  int num_of_nops(100);
  ActionVect startingActions;
  
//  wait for level to begin
  INSERT_NOPS(4.5 * num_of_nops)


  return startingActions;
}