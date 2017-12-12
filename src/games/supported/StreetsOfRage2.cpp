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

#include "StreetsOfRage2.hpp"
#include <iostream>


using namespace rle;


StreetsOfRage2Settings::StreetsOfRage2Settings() {
    reset();


 
      minimalActions = {JOYPAD_NOOP,
    				JOYPAD_DOWN,      //Walk down
    				JOYPAD_UP,        // Walk up
						JOYPAD_LEFT,      // Walk left
						JOYPAD_RIGHT,     // Walk right
						
						JOYPAD_GENESIS_A,  // Special attack
						JOYPAD_GENESIS_B,  // Regular attack

						JOYPAD_GENESIS_C,  // Jump
						
						JOYPAD_GENESIS_C | JOYPAD_LEFT, // Jump left
						JOYPAD_GENESIS_C | JOYPAD_RIGHT, // Jump right

						JOYPAD_GENESIS_B | JOYPAD_GENESIS_C,   // Rear attack or Super slam 
						JOYPAD_GENESIS_B | JOYPAD_LEFT,//Blitz attack or  
						JOYPAD_GENESIS_B | JOYPAD_RIGHT,
						JOYPAD_GENESIS_B | JOYPAD_GENESIS_C | JOYPAD_DOWN,//Drop attack
						JOYPAD_GENESIS_A | JOYPAD_GENESIS_B   //Directed special attack

    };
}


/* create a new instance of the rom */
RomSettings* StreetsOfRage2Settings::clone() const {
    RomSettings* rval = new StreetsOfRage2Settings();
    *rval = *this;
    return rval;
}


void StreetsOfRage2Settings::step(const RleSystem& system) {
//  Read out current score, health, lives, kills  
	reward_t score = 0; //getDecimalScore(0xEF99, 0xEF96, &system);	
  m_lives = readRam(&system, 0xEF82);
	m_health = readRam(&system, 0xEF80);

//  update the reward
	m_reward = score - m_score;
  m_score = score;

//	Update terminal status
  if ((m_lives == 0) &&(m_health == 0)){
      m_terminal = true;
    }
	
// Get level information
	m_current_level = readRam(&system, 0xFC42);
  int m_end_level = system.settings()->getInt("SOR2_end_level");
  int m_progress_1 = readRam(&system, 0xFC44);
  int m_progress_2 = readRam(&system, 0xFCCE);
  
  // Level 1
  if (m_end_level == 1){
       if ((m_current_level == 0) && (m_progress_1 == 0) && (m_progress_2 == 12)){
           m_terminal = true;
       }
  }// Level 2
  else if (m_end_level == 2){
       if ((m_current_level == 2) && (m_progress_1 == 0) && (m_progress_2 == 12)){
           m_terminal = true;
       }
  }// Level 3
  else if (m_end_level == 3){ 
      if ((m_current_level == 4) && (m_progress_1 == 0) && (m_progress_2 == 16)){
           m_terminal = true;
      }      
  }// Level 4
  else if (m_end_level == 4){
      if ((m_current_level == 6) && (m_progress_1 == 0) && (m_progress_2 == 22)){
           m_terminal = true;
      }
  }// Level 5
  else if (m_end_level == 5){ 
      if ((m_current_level == 8) && (m_progress_1 == 0) && (m_progress_2 == 10)){
           m_terminal = true;
       }
  }// Level 6
  else if (m_end_level == 6){
      if ((m_current_level == 10) && (m_progress_1 == 0) && (m_progress_2 == 10)){
           m_terminal = true;
       }
  }// Level 7
  else if (m_end_level == 7){
      if ((m_current_level == 12) && (m_progress_1 == 0) && (m_progress_2 == 18)){
           m_terminal = true;
       }
  }// Level 8
  else if (m_end_level == 8){
      if ((m_current_level == 16) && (m_progress_1 == 0) && (m_progress_2 == 10)){
           m_terminal = true;
       }
  } 
};



/* reset the state of the game */
void StreetsOfRage2Settings::reset() {
    m_reward   = 0;
    m_score = 0;
    m_terminal = false;
}


/* saves the state of the rom settings */
void StreetsOfRage2Settings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}


// loads the state of the rom settings
void StreetsOfRage2Settings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect StreetsOfRage2Settings::getStartingActions(const RleSystem& system){
	int num_of_nops(100);
	ActionVect startingActions;
	
//	wait for intro to end
	INSERT_NOPS(5*num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_START)
	INSERT_NOPS(0.4*num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_START)

//	select 1 player
	INSERT_NOPS(0.5*num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_START)
	INSERT_NOPS(3*num_of_nops)

//	choose character
	string player_1_character = system.settings()->getString("SOR2_player_1_character");
	if("axel" == player_1_character){
 		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}else if("max" == player_1_character){
		INSERT_ACTION_SINGLE_A(JOYPAD_LEFT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}else if("blaze" == player_1_character){
		INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}else if("skate" == player_1_character){
		INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}


//  wait for level to begin
	INSERT_NOPS(4 * num_of_nops)
 

	return startingActions;
}



void StreetsOfRage2Settings::startingOperations(RleSystem& system){
	//set difficulty
	int difficulty = system.settings()->getInt("SOR2_difficulty");
	if(1 == difficulty){
		writeRam(&system, 0xFD04, 0x0);
	}else if(2 == difficulty){
		writeRam(&system, 0xFD04, 0x2);
	}else if(3 == difficulty){
		writeRam(&system, 0xFD04, 0x4);
	}else if(4 == difficulty){
		writeRam(&system, 0xFD04, 0x6);
	}else if(5 == difficulty){
		writeRam(&system, 0xFD04, 0x8);
	}else if(6 == difficulty){
		writeRam(&system, 0xFD04, 0x10);
	}

	//set number of continues. By Default continues set to zero.
	writeRam(&system,0xEFA4,0x0); 
	
	//set start level	
	int start_level = system.settings()->getInt("SOR2_start_level");
	writeRam(&system, 0xFD0E, (start_level-1) * 0x1);

	//set number of lives 
	//(This sets the number of lives for the game. Would need to do 
	//this differently if you wanted to set the number of lives differently in
	//2 player mode.)
	int num_lives = system.settings()->getInt("SOR2_lives"); 
	writeRam(&system, 0xFD06, (num_lives-1) * 0x1);

}

