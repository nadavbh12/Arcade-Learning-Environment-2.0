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

#include "StreetsOfRageII.hpp"
#include <iostream>


using namespace rle;


StreetsOfRageIISettings::StreetsOfRageIISettings() {
    reset();

    minimalActions = {JOYPAD_NOOP,
            JOYPAD_DOWN,      //Walk down
            JOYPAD_UP,        // Walk up
            JOYPAD_LEFT,      // Walk left
            JOYPAD_RIGHT,     // Walk right

            JOYPAD_GENESIS_C,  // Jump
            JOYPAD_GENESIS_C | JOYPAD_LEFT, // Jump left
            JOYPAD_GENESIS_C | JOYPAD_RIGHT, // Jump right
            
            JOYPAD_GENESIS_A,  // Special attack
            JOYPAD_LEFT | JOYPAD_GENESIS_A,
            JOYPAD_RIGHT |JOYPAD_GENESIS_A,

            JOYPAD_GENESIS_B,  // Regular attack
            JOYPAD_GENESIS_B | JOYPAD_GENESIS_C,   // Rear attack or Super slam 
            JOYPAD_GENESIS_B | JOYPAD_LEFT,//Blitz attack or  
            JOYPAD_GENESIS_B | JOYPAD_RIGHT,

    };
}


/* create a new instance of the rom */
RomSettings* StreetsOfRageIISettings::clone() const {
    RomSettings* rval = new StreetsOfRageIISettings();
    *rval = *this;
    return rval;
}


void StreetsOfRageIISettings::step(const RleSystem& system) {
// Begin code for testing
  if(system.settings()->getBool("SOR_test") == true){
      // Fix Agent health
      writeRam(&system, 0xEF80, 0x68);
      // Make invincible
      writeRam(&system, 0xEF50, 0x14);
      // Freeze Time
      //writeRam(&system, 0xFC3C, 0x99);
  }

// This setting gives all enemies minimal health
 if((system.settings()->getBool("SOR_test") == true) || system.settings()->getInt("SOR_difficulty") == 0){
      // Fix enemy health
      writeRam(&system, 0xF180, 0x0);
      writeRam(&system, 0xF280, 0x0);
      writeRam(&system, 0xF380, 0x0);
      writeRam(&system, 0xF480, 0x0);
      writeRam(&system, 0xF580, 0x0);
      writeRam(&system, 0xF680, 0x0);
}

// End code for testing

//  Read out current score (Score set to 0. Will read score from RAM in rle/gym wrapper)
    reward_t score = 0; //getDecimalScore(0xEF99, 0xEF96, &system);
    m_lives = readRam(&system, 0xEF82);
    m_health = readRam(&system, 0xEF80);

//  Update the reward
    m_reward = score - m_score;
    m_score = score;

//	Update terminal status
    if (m_lives == 255){
      if (system.settings()->getBool("SOR_verbose") == true){
        std::cout << "Game over! Restarting." << std::endl;
      }
      m_terminal = true;
    }
    // Get level information
    m_current_level = (readRam(&system, 0xFC42) / 2) + 1;
    m_end_level = system.settings()->getInt("SOR_end_level");

//    int screen = readRam(&system, 0xFC02);
    int m_progress_1 = readRam(&system, 0xFC44);
    int m_progress_2 = readRam(&system, 0xFCCE);

    // Get boss information
    int boss_health = readRam(&system, 0xF180);
    int boss_lives = readRam(&system, 0xF182);

    if ((system.settings()->getBool("SOR_round_clear") == true) || (m_end_level == 8)){
        if (((m_current_level - 1) == m_end_level) && (m_progress_1 == 0) && (m_progress_2 > 10)){
          if (system.settings()->getBool("SOR_verbose") == true){
            std::cout << "Round "<< m_end_level << " clear!" << std::endl;
          }
        m_terminal = true;
        }
    }// Level 1
    else if ((m_end_level == 1) && (m_current_level == 1)) {
       if ((m_progress_1 == 12) && (m_progress_2 == 12) && ((boss_health + boss_lives) == 0)){
        if (system.settings()->getBool("SOR_verbose") == true){
           std::cout << "Beat level " << m_end_level << std::endl;
        }
        m_terminal = true;
       }
    }// Level 2
    else if ((m_end_level == 2) && (m_current_level == 2) && ((boss_health + boss_lives) == 0) ){
       if ((m_progress_1 == 12) && (m_progress_2 == 12)){
       if (system.settings()->getBool("SOR_verbose") == true){
           std::cout << "Beat level " << m_end_level << std::endl;
        }
        m_terminal = true;
       }
    }// Level 3
    else if ((m_end_level == 3) && (m_current_level == 3) && ((boss_health + boss_lives) == 0) ){
       if ((m_progress_1 == 16) && (m_progress_2 == 16)){
       if (system.settings()->getBool("SOR_verbose") == true){
           std::cout << "Beat level " << m_end_level << std::endl;
        }
        m_terminal = true;
       }
    }// Level 4
    else if ((m_end_level == 4) && (m_current_level == 4) && ((boss_health + boss_lives) == 0) ){
      if ((m_progress_1 == 22) && (m_progress_2 == 22)){
       if (system.settings()->getBool("SOR_verbose") == true){
           std::cout << "Beat level " << m_end_level << std::endl;
        }
        m_terminal = true;
       }
     }// Level 5
    else if ((m_end_level == 5) && (m_current_level == 5) && ((boss_health + boss_lives) == 0) ){
       if ((m_progress_1 == 10) && (m_progress_2 == 10)){
       if (system.settings()->getBool("SOR_verbose") == true){
           std::cout << "Beat level " << m_end_level << std::endl;
        }
        m_terminal = true;
       }
    }// Level 6
    else if ((m_end_level == 6) && (m_current_level == 6) && ((boss_health + boss_lives) == 0) ){
        if ((m_progress_1 == 10) && (m_progress_2 == 10)){
        if (system.settings()->getBool("SOR_verbose") == true){
           std::cout << "Beat level " << m_end_level << std::endl;
        }
        m_terminal = true;
       }
    }// Level 7
    else if ((m_end_level == 7) && (m_current_level == 7) && ((boss_health + boss_lives) == 0) ){
        if ((m_progress_1 == 18) && (m_progress_2 == 18)){
          if (system.settings()->getBool("SOR_verbose") == true){
           std::cout << "Beat level " << m_end_level << std::endl;
        }
        m_terminal = true;
       }
     } // Level 8 (see above)
}

/* reset the state of the game */
void StreetsOfRageIISettings::reset() {
    m_reward   = 0;
    m_score = 0;
    m_terminal = false;
}


/* saves the state of the rom settings */
void StreetsOfRageIISettings::saveState( Serializer & ser ) {
    ser.putInt(m_reward);
    ser.putInt(m_score);
    ser.putBool(m_terminal);
}


// loads the state of the rom settings
void StreetsOfRageIISettings::loadState( Deserializer & des ) {
    m_reward = des.getInt();
    m_score = des.getInt();
    m_terminal = des.getBool();
}


ActionVect StreetsOfRageIISettings::getStartingActions(const RleSystem& system){
    int num_of_nops(100);
    ActionVect startingActions;

    //	wait for intro to end
    INSERT_NOPS(5 * num_of_nops)
    INSERT_ACTION_SINGLE_A(JOYPAD_START)
    INSERT_NOPS(0.4*num_of_nops)
    INSERT_ACTION_SINGLE_A(JOYPAD_START)
    INSERT_NOPS(0.5*num_of_nops)

    INSERT_ACTION_SINGLE_A(JOYPAD_START)
    INSERT_NOPS(3 * num_of_nops)
    //
    string player_1_character = system.settings()->getString("SOR_player_1_character");
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

    return startingActions;
}

    void StreetsOfRageIISettings::startingOperations(RleSystem& system){
        //set difficulty
        m_difficulty = system.settings()->getInt("SOR_difficulty");
         if(0 == m_difficulty){
        // Also setting enemy health low (see above).
        writeRam(&system, 0xFD04, 0x0);
        }else if(1 == m_difficulty){
          writeRam(&system, 0xFD04, 0x0);
        }else if(2 == m_difficulty){
            writeRam(&system, 0xFD04, 0x2);
        }else if(3 == m_difficulty){
            writeRam(&system, 0xFD04, 0x4);
        }else if(4 == m_difficulty){
            writeRam(&system, 0xFD04, 0x6);
        }else if(5 == m_difficulty){
            writeRam(&system, 0xFD04, 0x8);
        }else if(6 == m_difficulty){
            writeRam(&system, 0xFD04, 0x0A);
        }

        // Set number of continues. By Default continues set to zero.
        writeRam(&system,0xEFA4, 0x0);

        // Set start level
        m_start_level = system.settings()->getInt("SOR_start_level");
        writeRam(&system, 0xFD0E, (m_start_level-1) * 0x1);

        // Set number of lives
        m_lives = system.settings()->getInt("SOR_lives");
        writeRam(&system, 0xFD06, (m_lives-1) * 0x1);
    }


ActionVect StreetsOfRageIISettings::getExtraActions(const RleSystem& system){
    int num_of_nops(100);
    ActionVect startingActions;

//  wait for level to begin
    INSERT_NOPS(4 * num_of_nops)

    return startingActions;
}
