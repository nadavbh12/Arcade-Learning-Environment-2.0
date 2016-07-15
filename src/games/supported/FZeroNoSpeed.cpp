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
#include "AleSystem.hxx"
#include "FZeroNoSpeed.hpp"

using namespace ale;


FZeroNoSpeedSettings::FZeroNoSpeedSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_B,	// speed
						JOYPAD_Y,	// break
    };
}


/* create a new instance of the rom */
RomSettings* FZeroNoSpeedSettings::clone() const {
    RomSettings* rval = new FZeroNoSpeedSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void FZeroNoSpeedSettings::step(const AleSystem& system) {
//    uint8_t* address = system.getRetroAgent().getRamAddress(RETRO_MEMORY_SYSTEM_RAM);

//	uint32_t time = 60 * readRam(&system, 0xAC) + readRam(&system, 0xAE);

	// update the reward
	uint32_t playerScore = 1000*readRam(&system, 0xA23) + 100*readRam(&system, 0xA24) +10*readRam(&system, 0xA25); //give extra points according sto speed to maintain highspeed
//	uint32_t speedScore = 100*readRam(&system, 0xA58) + 10*readRam(&system, 0xA59) +readRam(&system, 0xA5A); //give extra points according sto speed to maintain highspeed
	uint32_t score = playerScore;
//	uint32_t lifeBar = readRam(&system, 0xE23);
//	DEBUG2("score is: " << score);
	uint32_t loseInd1 = readRam(&system , 0xC1);
	uint32_t loseInd2 = readRam(&system , 0x50); //lose indication

	uint32_t loseInd = (loseInd2 ==0) && (loseInd1>0); //lose if time has start (larger than 0) and lose indicator is on
	m_reward = score - m_score;
//	DEBUG2("lose is : " << loseInd);
    m_score = score;

    if (loseInd == 1){
    	m_terminal = true;
    }

}

/* is end of game */
bool FZeroNoSpeedSettings::isTerminal() const {
    return m_terminal;
};


/* get the most recently observed reward */
reward_t FZeroNoSpeedSettings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool FZeroNoSpeedSettings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
}


/* reset the state of the game */
void FZeroNoSpeedSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}



/* saves the state of the rom settings */
void FZeroNoSpeedSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void FZeroNoSpeedSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect FZeroNoSpeedSettings::getStartingActions(){
	int i, num_of_nops(100);
	ActionVect startingActions;
//	startingActions.reserve(num_of_xs*num_of_nops);

	// wait for intro to end
	startingActions.insert(startingActions.end(), num_of_nops, JOYPAD_NOOP);
	// press start to begin
	startingActions.push_back(JOYPAD_START);
	// select arcade
	startingActions.insert(startingActions.end(), num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	// select 1 vs cpu
	startingActions.insert(startingActions.end(), num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	// select team
	startingActions.insert(startingActions.end(), num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	// select team
	startingActions.insert(startingActions.end(), num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	// wait for race countdown
	startingActions.insert(startingActions.end(), 5*num_of_nops, JOYPAD_NOOP);

	return startingActions;
}
