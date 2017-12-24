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
 *
 * RomSettings.cpp
 *
 * The interface to describe games as RL environments. It provides terminal and
 *  reward information.
 * *****************************************************************************
 */
#include "RomSettings.hpp"
#include "RleSystem.hxx"
#include "../libretro.h"

using namespace rle;

bool RomSettings::isLegal(const Action& a) const {
  return true;
}

ActionVect RomSettings::getStartingActions() {
    return ActionVect();
}

ActionVect RomSettings::getExtraActions() {
    return ActionVect();
}
ActionVect RomSettings::getMinimalActionSet() {
  ActionVect actions;
  for (unsigned a = 0; a < AllActionsVector.size(); a++) {
    if (isMinimal(AllActionsVector.at(a)) && isLegal(AllActionsVector.at(a))) {
      actions.push_back(AllActionsVector.at(a));
    }
  }
  return actions;
}

ActionVect RomSettings::getAllActions() {
  ActionVect actions;
  for (unsigned a = 0; a < AllActionsVector.size(); a++) {
    if (isLegal(AllActionsVector.at(a))) {
      actions.push_back(AllActionsVector.at(a));
    }
  }
  return actions;
}


int RomSettings::readRam(const RleSystem* system, int offset) {

	return system->getRetroAgent()->readRam((offset));
}

void RomSettings::writeRam(const RleSystem* system, int offset, uint8_t data) {

	return system->getRetroAgent()->writeRam(offset, data );
}


/* extracts a decimal value from a byte */
int RomSettings::getDecimalScore(int index, const RleSystem* system) {

    int score = 0;
    int digits_val = readRam(system, index);
    digits_val&= 0xff;				// SN: added since values may be large. TODO : handle negative numbers
    int right_digit = digits_val & 15;
    int left_digit = digits_val >> 4;
    score += ((10 * left_digit) + right_digit);

    return score;
}

/* extracts a decimal value from 2 bytes */
int RomSettings::getDecimalScore(int lower_index, int higher_index, const RleSystem* system) {

    int score = 0;
    int lower_digits_val = readRam(system, lower_index);
    lower_digits_val &= 0xff;	// SN: added since values may be large. TODO : handle negative numbers
    int lower_right_digit = lower_digits_val & 0xf;
    int lower_left_digit = (lower_digits_val - lower_right_digit) >> 4;
    score += ((10 * lower_left_digit) + lower_right_digit);
    if (higher_index < 0) {
        return score;
    }
    int higher_digits_val = readRam(system, higher_index);
    higher_digits_val &= 0xff;	// SN: added since values may be large. TODO : handle negative numbers
    int higher_right_digit = higher_digits_val & 0xf;
    int higher_left_digit = (higher_digits_val - higher_right_digit) >> 4;
    score += ((1000 * higher_left_digit) + 100 * higher_right_digit);
    return score;
}


/* extracts a decimal value from 3 bytes */
int RomSettings::getDecimalScore(int lower_index, int middle_index, int higher_index, const RleSystem* system) {

    int score = getDecimalScore(lower_index, middle_index, system);
    int higher_digits_val = readRam(system, higher_index);
    higher_digits_val &= 0xff;	// SN: added since values may be large. TODO : handle negative numbers
    int higher_right_digit = higher_digits_val & 15;
    int higher_left_digit = (higher_digits_val - higher_right_digit) >> 4;
    score += ((100000 * higher_left_digit) + 10000 * higher_right_digit);
    return score;
}

/* extracts a decimal value from a byte */
int RomSettings::getDecimalScoreWord(int index, const RleSystem* system) {

    int score = 0;
    int digits_val = readRam(system, index);
    score += digits_val;
    return score;
}

/* extracts a decimal value from 2 words */
int RomSettings::getDecimalScoreWord(int lower_index, int higher_index, const RleSystem* system) {
    int score = 0;
    int lower_digits_val = readRam(system, lower_index);
    int higher_digits_val = readRam(system, higher_index);
    score += lower_digits_val;
    score += 10 * higher_digits_val;
    return score;
}

/* extracts a decimal value from 3 words */
int RomSettings::getDecimalScoreWord(int lower_index, int middle_index, int higher_index, const RleSystem* system) {
    int score = 0;
    int lower_digits_val = readRam(system, lower_index);
    int middle_digits_val = readRam(system, middle_index);
    int higher_digits_val = readRam(system, higher_index);
    score += lower_digits_val;
    score += 10 * middle_digits_val;
    score += 100 * higher_digits_val;
    return score;
}

/* extracts a decimal value from words specified in vector */
int RomSettings::getDecimalScoreWords(std::vector<int> indexes, const RleSystem* system) {
    int score = 0;
    int multiplier = 1;
    for(auto ind : indexes){
    	score += multiplier * readRam(system, ind);
    	multiplier *= 10;
    }
    return score;
}
