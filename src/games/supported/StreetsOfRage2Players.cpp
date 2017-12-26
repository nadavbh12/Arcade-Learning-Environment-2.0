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

#include "StreetsOfRage2Players.hpp"
#include <iostream>


using namespace rle;


StreetsOfRage2PlayersSettings::StreetsOfRage2PlayersSettings() {
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
RomSettings* StreetsOfRage2PlayersSettings::clone() const {
    RomSettings* rval = new StreetsOfRage2PlayersSettings();
    *rval = *this;
    return rval;
}


void StreetsOfRage2PlayersSettings::step(const RleSystem& system) {
//  Read out current score, health, lives, stage progress
	reward_t score = 0; 	
  player_1_health = readRam(&system, 0xB832);
  player_2_health = readRam(&system, 0xB8B2);
  player_1_lives = readRam(&system, 0xFF21);
  player_2_lives = readRam(&system, 0xFF22);
  screen = readRam(&system, 0xFF00);
  distance = getDecimalScore(0xB810, &system);
	

  //  Update terminal status
    if (system.settings()->getBool("SOR_2p_terminal_both_win") == true){
      if( ((player_1_health == 0) && (player_1_lives == 0))|| ((player_2_health == 0) && (player_2_lives == 0))){
        m_terminal = true;
      }
    }else{
      if((player_1_health == 0) && (player_1_lives == 0) && (player_2_health == 0) && (player_2_lives == 0)){
        m_terminal = true;
      }
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
void StreetsOfRage2PlayersSettings::reset(){
 
//    m_score    = 0;
    m_terminal = false;
    distance = 0;
}


/* saves the state of the rom settings */
void StreetsOfRage2PlayersSettings::saveState( Serializer & ser ) {

  ser.putInt(m_score);
  ser.putBool(m_terminal);
}


// loads the state of the rom settings
void StreetsOfRage2PlayersSettings::loadState( Deserializer & des ) {

  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect StreetsOfRage2PlayersSettings::getStartingActions(const RleSystem& system){
	int num_of_nops(100);
	ActionVect startingActions;
	
//	wait for intro to end
	INSERT_NOPS(5*num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_START)
	INSERT_NOPS(0.4*num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_START)


  //	select 1 player
  INSERT_NOPS(1 * num_of_nops)
  INSERT_ACTION_SINGLE_A(JOYPAD_DOWN)
  INSERT_NOPS(1 * num_of_nops)
  INSERT_ACTION_SINGLE_A(JOYPAD_START)
  INSERT_NOPS(4*num_of_nops)


//	choose character
	string player_1 = system.settings()->getString("SOR_player_1_character");
  string player_2 = system.settings()->getString("SOR_player_2_character");

	if((player_1 == "adam") && (player_2 == "blaze")){
 		INSERT_ACTION_SINGLE(JOYPAD_START, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
    INSERT_ACTION_SINGLE(JOYPAD_START, B)
	}else if((player_1 == "adam") && (player_2 == "axel")){
		INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
		INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
		INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
    INSERT_ACTION_SINGLE(JOYPAD_START, B)
	}else if((player_1 == "axel") && (player_2 == "adam")){
		INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
		INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
		INSERT_ACTION_SINGLE(JOYPAD_START, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT,  B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
    INSERT_ACTION_SINGLE(JOYPAD_START, B)
  }else if ((player_1 == "axel") && (player_2 == "blaze")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
    INSERT_ACTION_SINGLE(JOYPAD_START, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
    INSERT_ACTION_SINGLE(JOYPAD_START, B)
  }else if ((player_1 == "blaze") && (player_2 == "adam")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
    INSERT_ACTION_SINGLE(JOYPAD_START, B)
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
    INSERT_ACTION_SINGLE(JOYPAD_START, A)
  } else if ((player_1 == "blaze") && (player_2 == "axel")){
    INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
    INSERT_ACTION_SINGLE(JOYPAD_START, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
    INSERT_ACTION_SINGLE(JOYPAD_LEFT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
    INSERT_ACTION_SINGLE(JOYPAD_START, A) 
  }

  INSERT_NOPS(0.5 * num_of_nops)

	return startingActions;
}



void StreetsOfRage2PlayersSettings::startingOperations(RleSystem& system){
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

ActionVect StreetsOfRage2PlayersSettings::getExtraActions(const RleSystem& system){
  int num_of_nops(100);
  ActionVect startingActions;
  
//  wait for level to begin
  INSERT_NOPS(4.5 * num_of_nops)


  return startingActions;
}