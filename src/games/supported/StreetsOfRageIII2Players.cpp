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

#include "StreetsOfRageIII2Players.hpp"
#include <iostream>


using namespace rle;


StreetsOfRageIII2PlayersSettings::StreetsOfRageIII2PlayersSettings() {
    reset();

// Actions can also be set in a RLE gym wrapper
      minimalActions = {JOYPAD_NOOP,
    			// Moving
     //     JOYPAD_DOWN,      //Walk down
     //     JOYPAD_UP,        // Walk up
					// JOYPAD_LEFT,      // Walk left
					// JOYPAD_RIGHT,     // Walk right
     //    	JOYPAD_GENESIS_C, // Jump
     //     JOYPAD_LEFT | JOYPAD_GENESIS_C,
         // JOYPAD_RIGHT | JOYPAD_GENESIS_C,

		   //    // Special attacks
					// JOYPAD_GENESIS_A,
         // JOYPAD_LEFT | JOYPAD_GENESIS_A,
        // JOYPAD_RIGHT |JOYPAD_GENESIS_A,

     //      // Regular Attacks
	    		// JOYPAD_GENESIS_B,
					// JOYPAD_GENESIS_B | JOYPAD_GENESIS_C,   // Rear attack or Super slam

     //      // Attacks when holding enemy
     //     JOYPAD_GENESIS_B | JOYPAD_LEFT,
				 //  JOYPAD_GENESIS_B | JOYPAD_RIGHT,
    };
}


/* create a new instance of the rom */
RomSettings* StreetsOfRageIII2PlayersSettings::clone() const {
    RomSettings* rval = new StreetsOfRageIII2PlayersSettings();
    *rval = *this;
    return rval;
}


void StreetsOfRageIII2PlayersSettings::step(const RleSystem& system) {
// if (readRam(&system, 0xFB00) != 14){
//     m_terminal = true;
// }

// Begin code for testing
  if(system.settings()->getBool("SOR_test") == true){

      // Fix Agent health
      writeRam(&system, 0xDF6D, 0x34);

      // Make invincible
      writeRam(&system, 0xDF5D, 0xFF);

      // Freeze Time
     // writeRam(&system, 0xFC3C, 0x99);
}

// This setting gives all enemies minimal health and lives
  if((system.settings()->getBool("SOR_test") == true) || system.settings()->getInt("SOR_difficulty") == 0){
      // Fix enemy health 
       writeRam(&system, 0xE16D, 0x0);
       writeRam(&system, 0xE26D, 0x0);
       writeRam(&system, 0xE36D, 0x0);
       writeRam(&system, 0xE46D, 0x0);
       writeRam(&system, 0xE56D, 0x0);
       writeRam(&system, 0xE66D, 0x0);
      // Fix enemy lives
      // writeRam(&system, 0xE18B, 0x0);
      // writeRam(&system, 0xE28B, 0x0);
      // writeRam(&system, 0xE38B, 0x0);
      // writeRam(&system, 0xE48B, 0x0);
      // writeRam(&system, 0xE58B, 0x0);

  }
// End code for testing

//  Read out current score, health, lives, kills
// Score set to 0. Will read score from rle/gym wrapper
  reward_t score = 0; //getDecimalScore(0xEF99, 0xEF96, &system);
  player_1_lives = readRam(&system, 0xDFA1);
  player_2_lives = readRam(&system, 0xE0A1);
//  update the reward
  m_reward = score - m_score;
  m_score = score;


  if (system.settings()->getBool("SOR_2p_terminal_both_win") == true){
    if (((player_1_lives == 255) || (player_2_lives == 255)) && (readRam(&system, 0xFB00) == 14)){
      m_terminal = true;
    }
  }else{
    if ((player_1_lives == 255) && (player_2_lives == 255) && ((readRam(&system, 0xFB00) == 14)))
      m_terminal = true;
  }
  




  // Get level information
  m_current_level = readRam(&system, 0xFB04) + 1;
  m_end_level = system.settings()->getInt("SOR_end_level");

  // Get information on agent position
  int m_position = 256 * readRam(&system, 0xDF41) + readRam(&system, 0xDF40);
  int scene = readRam(&system, 0xFB02);
  m_screen = readRam(&system, 0xFB00);

  // Get boss information
  int boss_1_ID = readRam(&system, 0xE10C);
  int boss_present = readRam(&system, 0xE10C) + readRam(&system, 0xE20C) + readRam(&system, 0xE30C);
  int boss_1_health = readRam(&system, 0xE16D);
  int boss_2_health = readRam(&system, 0xE26D);
  int boss_3_health = readRam(&system, 0xE36D);
  int boss_health = boss_1_health + boss_2_health + boss_3_health;
  int boss_1_lives = readRam(&system, 0xE18A);
  int boss_lives = readRam(&system, 0xE18A) + readRam(&system, 0xE28A) + readRam(&system, 0xE38A);

  // Win if we accidentily go past the end level
  if(m_end_level < m_current_level){
    std::cout << "ADVANCED TO NEXT LEVEL" << std::endl;
    m_terminal = true;
  }
  // Win if round clear
  if ((system.settings()->getBool("SOR_round_clear") == true) && ((m_current_level != 7) || (m_current_level != 8))){
    if ((m_end_level == m_current_level) && (readRam(&system, 0xFB00) == 46)){
      std::cout << "ROUND CLEAR!" << std::endl;
      m_terminal = true;
    }
  }else{
  // Win if agent is in right place, with bosses present, and bosses dead
    if((m_end_level == 1) && (m_current_level == 1)){
        if ((scene == 2) && (m_position > 456) && (boss_present > 0) && ((boss_1_health + boss_1_lives == 0))){
            m_terminal = true;
        }
    }else if ((m_end_level == 2) && (m_current_level == 2)){
        if ((scene == 2) && (m_position > 100) && (boss_present > 0) && ((boss_health + boss_lives) == 0) ){
            std::cout << " won game!" << std::endl;
            m_terminal = true;
        }
    }else if ((m_end_level == 3) && (m_current_level == 3)){
      if ((scene == 2) && (m_position > 400) && (boss_present > 0) && ((boss_1_health + boss_1_lives) == 0)){
            std::cout << "won game" << std::endl;
            m_terminal = true;
        }
    }else if ((m_end_level == 4) && (m_current_level == 4)){
        if ((scene == 2) && (m_position> 300) && (boss_present > 0) && ((boss_1_health + boss_lives) == 0)){
            std::cout << "won game" << std::endl;
            m_terminal = true;
        }
    }else if ((m_end_level == 5) && (m_current_level == 5)){
        if ((scene == 7) && (m_position > 30) && (boss_1_ID  == 56) && ((boss_1_health + boss_1_lives) == 0)){
            std::cout << "won game" << std::endl;
            m_terminal = true;
        }
    }else if ((m_end_level == 6) && (m_current_level == 6)){
        if ((scene == 7) && (m_position > 50) && (boss_present > 0) && ((boss_1_health + boss_1_lives) == 0)){
            std::cout << "won game" << std::endl;
            m_terminal = true;
        }
    }else if ((m_end_level == 7) && (m_current_level == 7)){
        if ((scene == 4) && (m_position > 30) && (boss_present > 0) && ((boss_1_health + boss_1_lives) == 0)){
            std::cout << "won game" << std::endl;
            m_terminal = true;
        }
    }else if ((m_end_level == 8) && (m_current_level == 8)){
        if ((scene == 3) && (m_position > 30) && (boss_present > 0) && ((boss_1_health + boss_1_lives) == 0)){
            std::cout << "won game" << std::endl;
            m_terminal = true;
        }
    }

  }
}


/* reset the state of the game */
void StreetsOfRageIII2PlayersSettings::reset() {
    m_reward   = 0;
    m_score = 0;
    m_terminal = false;
}


/* saves the state of the rom settings */
void StreetsOfRageIII2PlayersSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}


// loads the state of the rom settings
void StreetsOfRageIII2PlayersSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect StreetsOfRageIII2PlayersSettings::getStartingActions(const RleSystem& system){
	int num_of_nops(100);
	ActionVect startingActions;

// // Wait for intro to end
    INSERT_NOPS((3 - readRam(&system, 0xFB00)) * num_of_nops)

    INSERT_ACTION_SINGLE_A(JOYPAD_START)

    INSERT_ACTION_SINGLE_A(JOYPAD_START)
    INSERT_NOPS(2 * num_of_nops)
    INSERT_ACTION_SINGLE_A(JOYPAD_START)
    INSERT_NOPS(2 * num_of_nops)

    INSERT_ACTION_SINGLE_A(JOYPAD_START)
    INSERT_NOPS(2 * num_of_nops)

    INSERT_ACTION_SINGLE_A(JOYPAD_DOWN)
    INSERT_NOPS(0.5 * num_of_nops)
    INSERT_ACTION_SINGLE_A(JOYPAD_START)
    INSERT_NOPS(2 * num_of_nops)

// // // // Choose Player 1 character
  string player_1 = system.settings()->getString("SOR_player_1_character");
  string player_2 = system.settings()->getString("SOR_player_2_character");

  if((player_1 == "axel") && (player_2 == "blaze")){
    //INSERT_ACTION_SINGLE_A(JOYPAD_START)
  }else if((player_1 == "axel") && (player_2 == "skate")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
  //  INSERT_ACTION_SINGLE_A(JOYPAD_START)
  }else if((player_1 == "axel") && (player_2 == "zan")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
  //  INSERT_ACTION_SINGLE_A(JOYPAD_START)
  }else if((player_1 == "zan") && (player_2 == "blaze")){
    INSERT_ACTION_SINGLE(JOYPAD_LEFT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)  
  }else if ((player_1 == "zan") && (player_2 == "skate")){
    INSERT_ACTION_SINGLE(JOYPAD_LEFT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)  
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)  
  }else if ((player_1 == "zan") && (player_2 == "axel")){
    INSERT_ACTION_SINGLE(JOYPAD_LEFT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)  
    INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B) 
  }else if((player_1 == "skate") && (player_2 == "blaze")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)  
  }else if((player_1 == "skate") && (player_2 == "axel")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)   
    INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)     
  }else if((player_1 == "skate") && (player_2 == "zan")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)   
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)  
  }else if((player_1 == "blaze") && (player_2 == "skate")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)      
  }else if ((player_1 == "blaze") && (player_2 == "zan")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)    
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)   
  }else if ((player_1 == "blaze") && (player_2 == "axel")){
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
    INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
    INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
    INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
  }

  INSERT_NOPS(1 * num_of_nops)

return startingActions;
}


void StreetsOfRageIII2PlayersSettings::startingOperations(RleSystem& system){
	/*This function sets the difficulty, number of continues, starting level,
  and number of lives by writing directly to RAM*/

	//set difficulty
	m_difficulty = system.settings()->getInt("SOR_difficulty");
    // Also setting enemy health low (see above).
  	if(0 == m_difficulty){
		writeRam(&system, 0xFF08, 0x2);
	}else if(1 == m_difficulty){
		writeRam(&system, 0xFF08, 0x2);
	}else if(2 == m_difficulty){
		writeRam(&system, 0xFF08, 0x4);
	}else if(3 == m_difficulty){
		writeRam(&system, 0xFF08, 0x6);
	}else if(4 == m_difficulty){
		writeRam(&system, 0xFF08, 0x8);
	}else if(5 == m_difficulty){
		writeRam(&system, 0xFF08, 0x0A);
	}

	//set number of continues. By Default continues set to zero.
	writeRam(&system, 0xDFA0, 0x1);
  writeRam(&system, 0xE0A0, 0x1);

	// //set start level for testing or to access Levels 7 and 8
	m_start_level = system.settings()->getInt("SOR_start_level");
	  if(m_start_level == 1){
      writeRam(&system, 0xFB04, 0x0);
    }else if(m_start_level == 2){
      writeRam(&system, 0xFB04, 0x1);
    }else if(m_start_level == 3){
      writeRam(&system, 0xFB04, 0x2);
    }else if(m_start_level == 4){
      writeRam(&system, 0xFB04, 0x3);
    }else if (m_start_level == 5){
      writeRam(&system, 0xFB04, 0x4);
    }else if (m_start_level == 6){
      writeRam(&system, 0xFB04, 0x5);
    }else if(m_start_level == 7){
      writeRam(&system, 0xFB04, 0x6);
      writeRam(&system, 0xFB02, 0x1);
    }else if (m_start_level == 8){
      writeRam(&system, 0xFB04, 0x7);
      writeRam(&system, 0xFB02, 0x1);
    }

    if(system.settings()->getBool("SOR_test") == true){
  		// Max out the difficulty
  		  writeRam(&system, 0xFF08, 0x0A);
  		// Go to the last scene of the chosen level
  		if(m_start_level == 1){
  			writeRam(&system, 0xFB02, 0x2);
  		}else if(m_start_level == 2){
  			writeRam(&system, 0xFB02, 0x2);
  		}else if(m_start_level == 3){
  			writeRam(&system, 0xFB02, 0x2);
  		}else if(m_start_level == 4){
  			writeRam(&system, 0xFB02, 0x2);
  		}else if (m_start_level == 5){
  			writeRam(&system, 0xFB02, 0x7);
  		}else if (m_start_level == 6){
  			writeRam(&system, 0xFB02, 0x7);
  		}else if(m_start_level == 7){
  			writeRam(&system, 0xFB02, 0x4);
  		}else if (m_start_level == 8){
  			writeRam(&system, 0xFB02, 0x3);
  		}
	 }

 //  // Set number of lives
  int m_lives = system.settings()->getInt("SOR_lives");
  writeRam(&system, 0xDF8A, m_lives  * 0x1);
  writeRam(&system, 0xE08A, m_lives * 0x1);
}

ActionVect StreetsOfRageIII2PlayersSettings::getExtraActions(const RleSystem& system){
    int num_of_nops(100);
    ActionVect startingActions;

    INSERT_NOPS(1 * num_of_nops)

  INSERT_ACTION_SINGLE(JOYPAD_START, A)
  INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
  INSERT_ACTION_SINGLE(JOYPAD_START, B)
//
// //  wait for level to begin
  int start_level = system.settings()->getInt("SOR_start_level");
  if (start_level == 1){
    INSERT_NOPS(3 * num_of_nops)
    INSERT_ACTION_SINGLE_A(JOYPAD_START)
    INSERT_NOPS(4 *  num_of_nops)
  }else if (start_level == 2){
    INSERT_NOPS(3.5 * num_of_nops)
  }else if (start_level == 3){
    INSERT_NOPS(4.6 * num_of_nops)
  }else if (start_level ==4){
    INSERT_NOPS(3.7 * num_of_nops)
  }else if (start_level == 5){
    INSERT_NOPS(3.7 * num_of_nops)
  }else if (start_level == 6){
    INSERT_NOPS(3.7 * num_of_nops)
  }else if (start_level == 7){
    INSERT_NOPS(1 * num_of_nops)
  }else if (start_level == 8){
    INSERT_NOPS(4 * num_of_nops)
  }

    return startingActions;
}
