#include "../RomUtils.hpp"
#include <iomanip>

#include "../RomUtils.hpp"
#include "StreetFighterIITurboHyperFighting.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

using namespace rle;


StreetFighterIITurboHyperFightingSettings::StreetFighterIITurboHyperFightingSettings() {
    reset();

    // TODO
    minimalActions = {  JOYPAD_NOOP,
                        JOYPAD_UP,
                        JOYPAD_DOWN,
                        JOYPAD_LEFT,
                        JOYPAD_RIGHT,
                        JOYPAD_R,
                        JOYPAD_A,
                        JOYPAD_B,
                        JOYPAD_Y,
                        JOYPAD_X,
                        JOYPAD_L,
                        JOYPAD_UP | JOYPAD_RIGHT,
                        JOYPAD_UP | JOYPAD_LEFT,
                        JOYPAD_DOWN | JOYPAD_Y,
                        JOYPAD_DOWN | JOYPAD_R,
                        JOYPAD_DOWN | JOYPAD_L,
                        JOYPAD_DOWN | JOYPAD_A,
                        JOYPAD_DOWN | JOYPAD_B,
                        JOYPAD_DOWN | JOYPAD_X,

                        JOYPAD_LEFT | JOYPAD_A,
                        JOYPAD_RIGHT | JOYPAD_A,
                        JOYPAD_LEFT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_Y,
                        JOYPAD_LEFT | JOYPAD_Y,
                        JOYPAD_RIGHT | JOYPAD_L,
                        JOYPAD_LEFT | JOYPAD_L,
                        JOYPAD_RIGHT | JOYPAD_R,
                        JOYPAD_LEFT | JOYPAD_R,
                        JOYPAD_RIGHT | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_B,
    };
}

/* create a new instance of the rom */
RomSettings* StreetFighterIITurboHyperFightingSettings::clone() const {
    RomSettings* rval = new StreetFighterIITurboHyperFightingSettings();
    *rval = *this;
    return rval;
}

/* process the latest information from RLE */
void StreetFighterIITurboHyperFightingSettings::step(const RleSystem& system) {
    int time = getDecimalScore(0x18f3, &system);

    // update the reward
    reward_t playerScore = getDecimalScore(0x6c3,0x6c4, &system);
    playerScore *= 100;

    int playerHealth = getDecimalScore(0x0636, &system);
    int opponentHealth = getDecimalScore(0x0836, &system);

    reward_t score = playerScore;

    string rewardStrategy = system.settings()->getString("SF2THF_reward_strategy");
    if("score" == rewardStrategy){
        m_reward = score - m_score;
    }else if("health" == rewardStrategy){
        m_reward = playerHealth - m_health;
    }else if("healthBalance" == rewardStrategy){
        m_reward = (playerHealth - m_health) - (opponentHealth - o_health);
    }else{
        throw RleException("SF2THF_reward_strategy illegal");
    }

    // Update variables
    m_health = playerHealth;
    o_health = opponentHealth;
    m_score = score;

    m_wins = getDecimalScore(0x5d0, &system);
    o_wins = getDecimalScore(0x7d0, &system);

    if (m_wins==2){
        m_terminal = true;
    }
    if(o_wins == 2){
        m_terminal = true;
    }
}

/* reset the state of the game */
void StreetFighterIITurboHyperFightingSettings::reset() {
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_wins    = 0;
    o_wins    = 0;
    m_health  = 0;
    o_health  = 0;
    match_ended = false;
    // Avoid resetting m_speed and m_difficulty as the remain across resets.
}

/* saves the state of the rom settings */
void StreetFighterIITurboHyperFightingSettings::saveState( Serializer & ser ) {
    ser.putInt(m_reward);
    ser.putInt(m_score);
    ser.putInt(m_wins);
    ser.putInt(o_wins);
    ser.putInt(m_health);
    ser.putInt(o_health);
    ser.putBool(m_terminal);
    ser.putBool(match_ended);
    ser.putInt(m_speed);
    ser.putInt(m_difficulty);
}

// loads the state of the rom settings
void StreetFighterIITurboHyperFightingSettings::loadState( Deserializer & des ) {
    m_reward = des.getInt();
    m_score = des.getInt();
    m_wins = des.getInt();
    o_wins = des.getInt();
    m_health = des.getInt();
    o_health = des.getInt();
    m_terminal = des.getBool();
    match_ended = des.getBool();
    m_speed = des.getInt();
    m_difficulty = des.getInt();
}

ActionVect StreetFighterIITurboHyperFightingSettings::selectChar(int character_index){
    ActionVect selectCharActions;

    if (character_index >= 6) {
        // The character is in the bottom row
        selectCharActions.push_back(JOYPAD_DOWN);
        selectCharActions.push_back(JOYPAD_NOOP);
        character_index = character_index - 6   ;
    }
    for(int i = 0; i < character_index; i++) {
        selectCharActions.push_back(JOYPAD_RIGHT);
        selectCharActions.push_back(JOYPAD_NOOP);
    }

    selectCharActions.push_back(JOYPAD_START);

    return selectCharActions;
}

int StreetFighterIITurboHyperFightingSettings::getCharacterIndex(const RleSystem& system){
    int character_index = 0; // Ryu by default
    string player1_character = system.settings()->getString("SF2THF_player1_character");
    if("ryu" == player1_character){
        character_index = 0;
    }else if("honda" == player1_character){
        character_index = 1;
    }else if("blanka" == player1_character){
        character_index = 2;
    }else if("guile" == player1_character){
        character_index = 3;
    }else if("balrog" == player1_character){
        character_index = 4;
    }else if("vega" == player1_character){
        character_index = 5;
    }else if("ken" == player1_character){
        character_index = 6;
    }else if("chun-li" == player1_character){
        character_index = 7;
    }else if("zangief" == player1_character){
        character_index = 8;
    }else if("dhalsim" == player1_character){
        character_index = 9;
    }else if("sagat" == player1_character){
        character_index = 10;
    }else if("bison" == player1_character){
        character_index = 11;
    }else{
        throw RleException("SF2THF_player1_character illegal");
    }
    return character_index;
}

ActionVect StreetFighterIITurboHyperFightingSettings::selectSpeedActions(const RleSystem& system){
    int target_speed = system.settings()->getInt("SF2THF_speed");
    if ((target_speed < 0) || (target_speed > 4)) {
        throw RleException("SF2THF_speed illegal. Expected setting value [0 - 4]");
    }
    Action action = JOYPAD_NOOP;
    ActionVect actions;
    if (m_speed < target_speed) {
        // Move right
        action = JOYPAD_RIGHT;
    } else if (m_speed > target_speed) {
        // Move left
        action = JOYPAD_LEFT;
    }

    for(int i = 0; i < abs(m_speed - target_speed); i++){
        actions.push_back(action);
        actions.push_back(JOYPAD_NOOP);
    }

    m_speed = target_speed;
    return actions;
}

ActionVect StreetFighterIITurboHyperFightingSettings::selectDifficultyActions(const RleSystem& system){
    int target_difficulty = system.settings()->getInt("SF2THF_difficulty");
    if ((target_difficulty < 0) || (target_difficulty > 7)) {
        throw RleException("SF2THF_difficulty illegal. Expected setting value [0 - 7]");
    }
    Action action = JOYPAD_NOOP;
    ActionVect actions;
    if (m_difficulty < target_difficulty) {
        // Move right
        action = JOYPAD_RIGHT;
    } else if (m_difficulty > target_difficulty) {
        // Move left
        action = JOYPAD_LEFT;
    }

    actions.push_back(JOYPAD_DOWN);
    actions.push_back(JOYPAD_NOOP);
    actions.push_back(JOYPAD_DOWN);
    actions.push_back(JOYPAD_NOOP);
    actions.push_back(JOYPAD_START);
    actions.push_back(JOYPAD_NOOP);

    int num_of_nops(100);
    ActionVect startingActions;

    // wait for transition to end
    for(int i = 0; i<5*num_of_nops; i++){
        actions.push_back(JOYPAD_NOOP);
    }

    for(int i = 0; i < abs(m_difficulty - target_difficulty); i++){
        actions.push_back(action);
        actions.push_back(JOYPAD_NOOP);
    }

    actions.push_back(JOYPAD_START);

    // wait for transition to end
    for(int i = 0; i<5*num_of_nops; i++){
        actions.push_back(JOYPAD_NOOP);
    }

    actions.push_back(JOYPAD_START);

    // wait for transition to end
    for(int i = 0; i<5*num_of_nops; i++){
        actions.push_back(JOYPAD_NOOP);
    }

    m_difficulty = target_difficulty;
    return actions;
}

ActionVect StreetFighterIITurboHyperFightingSettings::getStartingActions(const RleSystem& system){
    int i, num_of_nops(100);
    ActionVect startingActions;

    // wait for intro to end
    for(i = 0; i<5*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // second animation
    startingActions.push_back(JOYPAD_START);

    // Wait transition
    for(i = 0; i<5*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // main Screen
    startingActions.push_back(JOYPAD_START);

    for(i = 0; i<5*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // Select speed
    ActionVect speedActions = selectSpeedActions(system);
    startingActions.insert(startingActions.end(), speedActions.begin(), speedActions.end());

    // Select difficulty
    ActionVect difficultyActions = selectDifficultyActions(system);
    startingActions.insert(startingActions.end(), difficultyActions.begin(), difficultyActions.end());

    // Start
    startingActions.push_back(JOYPAD_START);

    for(i = 0; i<5*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // Start the game
    startingActions.push_back(JOYPAD_START);

    for(i = 0; i<5*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // Select character by index
    ActionVect selectCharActions = selectChar(getCharacterIndex(system));
    startingActions.insert(startingActions.end(), selectCharActions.begin(), selectCharActions.end());

    // Wait for animation to finish
    for(i = 0; i<5*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // Skip vs screen
    startingActions.push_back(JOYPAD_START);

    // Wait for transition to finish
    for(i = 0; i<1*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    return startingActions;
}