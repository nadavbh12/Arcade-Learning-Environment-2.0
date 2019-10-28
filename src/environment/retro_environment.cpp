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
 *  stella_environment.cpp
 *
 *  A class that wraps around the Stella core to provide users with a typical
 *  reinforcement learning environment interface.
 *
 **************************************************************************** */

#include "retro_environment.hpp"
#include "libretro.h"
#include <sstream>

using namespace rle;



RetroEnvironment::RetroEnvironment(pRleSystem rlesystem, pRomSettings settings) :
  m_rlesystem(rlesystem),
  m_settings(settings),
  m_phosphor_blend( ),	// TODO pass RleSystem
  m_screen(m_rlesystem->getRetroAgent()->getHeight(),
		  m_rlesystem->getRetroAgent()->getWidth()),
  m_ram(m_rlesystem->getRetroAgent()->getRamSize(),
		m_rlesystem->getRetroAgent()->getRamAddress()),
  m_player_a_action(PLAYER_A | JOYPAD_NOOP),
  m_player_b_action(PLAYER_B | JOYPAD_NOOP) {

  m_num_reset_steps = 4;

  m_max_num_frames_per_episode = m_rlesystem->settings()->getInt("max_num_frames_per_episode");
  m_colour_averaging = m_rlesystem->settings()->getBool("color_averaging");

  m_repeat_action_probability = m_rlesystem->settings()->getFloat("repeat_action_probability");

  m_frame_skip = m_rlesystem->settings()->getInt("frame_skip");
  if (m_frame_skip < 1) {
    rle::Logger::Warning << "Warning: frame skip set to < 1. Setting to 1." << std::endl;
    m_frame_skip = 1;
  }

  // If so desired, we record all emulated frames to a given directory
  std::string recordDir = m_rlesystem->settings()->getString("record_screen_dir");

  getPixelFormat(*m_screen.m_pixelFormat);

  if (!recordDir.empty()) {
    rle::Logger::Info << "Recording screens to directory: " << recordDir << std::endl;


//    TODO SN : implement screen_exporter
    // Create the screen exporter
//    m_screen_exporter.reset(new ScreenExporter(m_rlesystem->colourPrlette(), recordDir));
  }

}
void RetroEnvironment::getPixelFormat(struct pixelFormat &pixel_format){
	uint32_t rmask, gmask, bmask, amask;
	uint32_t rShift, gShift, bShift, aShift;

	pixel_format.Bpp=m_rlesystem->getRetroAgent()->getBpp()/8; //in bytes
	m_rlesystem->getRetroAgent()->getRgbMask(rmask,  gmask,  bmask,  amask);
	m_rlesystem->getRetroAgent()->getRgbShift(rShift,  gShift,  bShift,  aShift);
	//mask
	pixel_format.rmask=rmask;
	pixel_format.gmask=gmask;
	pixel_format.bmask=bmask;
	pixel_format.amask=amask;
	//shift
	pixel_format.rShift=rShift;
	pixel_format.gShift=gShift;
	pixel_format.bShift=bShift;
	pixel_format.aShift=aShift;
	// Fill
	pixel_format.rFill = 8 - (gShift+bShift); // rShift+gShift+bShift - rShift = 16 - 11 = 5
	pixel_format.gFill = 8 - (rShift-gShift); // rShift+gShift+bShift - rShift = 16 - 11 = 5
	pixel_format.bFill = 8 - (gShift-bShift); // rShift+gShift+bShift - rShift = 16 - 11 = 5

}

/** Resets the system to its start state. */
void RetroEnvironment::reset() {
  m_state.resetEpisodeFrameNumber();

  // Reset the emulator
  m_rlesystem->getRetroAgent()->reset();
  bool audio = m_rlesystem->settings()->getBool("sound", true);
  m_rlesystem->getRetroAgent()->audioEnable(false);

  // NOOP for 60 steps in the deterministic environment setting, or some random amount otherwise
  int noopSteps;
  noopSteps = 60;
  emulateStart(PLAYER_A | JOYPAD_NOOP, PLAYER_B | JOYPAD_NOOP, noopSteps);
  // reset for n steps
  emulateStart(JOYPAD_RESET, PLAYER_B | JOYPAD_NOOP, m_num_reset_steps);

  // reset the rom (after emulating, in case the NOOPs led to reward)
  m_settings->reset();

  // Apply necessary actions specified by the rom itself
  ActionVect startingActions = m_settings->getStartingActions(*m_rlesystem);
  for (size_t i = 0; i < startingActions.size(); i++){
	  if((startingActions[i] & PLAYER_B) > 0){
		  emulateStart(JOYPAD_NOOP, startingActions[i]);
	  }else{
		  emulateStart(startingActions[i], PLAYER_B | JOYPAD_NOOP);
	  }
    // uncomment to view screen of starting actions
//  m_rlesystem->p_display_screen->display_screen();
  }
  m_settings->startingOperations(*m_rlesystem);

  startingActions = m_settings->getExtraActions(*m_rlesystem);
  for (size_t i = 0; i < startingActions.size(); i++){
    if((startingActions[i] & PLAYER_B) > 0){
        emulateStart(JOYPAD_NOOP, startingActions[i]);
    }else{
        emulateStart(startingActions[i], PLAYER_B | JOYPAD_NOOP);
    }


  m_rlesystem->getRetroAgent()->audioEnable(audio);
}

}

/** Save/restore the environment state. */
void RetroEnvironment::save() {
  // Store the current state into a new object
  RLEState new_state = cloneState();
  m_saved_states.push(new_state);
}

void RetroEnvironment::load() {
  // Get the state on top of the stack
  RLEState& target_state = m_saved_states.top();

  // Deserialize it into 'm_state'
  restoreState(target_state);
  m_saved_states.pop();
}

RLEState RetroEnvironment::cloneState() {
  return m_state.save(m_rlesystem, m_settings, false);
}

void RetroEnvironment::restoreState(const RLEState& target_state) {
  m_state.load(m_rlesystem, m_settings, target_state, false);
}

RLEState RetroEnvironment::cloneSystemState() {
  return m_state.save(m_rlesystem, m_settings, true);
}

void RetroEnvironment::restoreSystemState(const RLEState& target_state) {
  m_state.load(m_rlesystem, m_settings, target_state, true);
}

void RetroEnvironment::noopIllegalActions(Action & player_a_action, Action & player_b_action) {
  if ( ((player_a_action & PLAYER_A) == PLAYER_A) &&
        !m_settings->isLegal(player_a_action)) {
    player_a_action = PLAYER_A | JOYPAD_NOOP;
  }
  // Also drop RESET, which doesn't play nice with our clean notions of RL environments
  else if (player_a_action == JOYPAD_RESET)
    player_a_action = PLAYER_A |JOYPAD_NOOP;

  if ( ((player_b_action & PLAYER_B) == PLAYER_B) &&
        !m_settings->isLegal(player_b_action) ) {
    player_b_action = PLAYER_B | JOYPAD_NOOP;
  }
  else if (player_b_action == (PLAYER_B | JOYPAD_RESET))
    player_b_action = PLAYER_B | JOYPAD_NOOP;
}

reward_t RetroEnvironment::act(Action player_a_action, Action player_b_action) {

  // Total reward received as we repeat the action
  reward_t sum_rewards = 0;

  Random& rng = m_rlesystem->rng();

  // Apply the same action for a given number of times... note that act() will refuse to emulate
  //  past the terminal state
  for (size_t i = 0; i < m_frame_skip; i++) {

    // Stochastically drop actions, according to m_repeat_action_probability
    if (rng.nextDouble() >= m_repeat_action_probability)
      m_player_a_action = player_a_action;
    // @todo Possibly optimize by avoiding call to rand() when player B is "off" ?
    if (rng.nextDouble() >= m_repeat_action_probability)
      m_player_b_action = player_b_action;

//    TODO SN : fix record of sound
    // If so desired, request one frame's worth of sound (this does nothing if recording
    // is not enabled)
//    m_rlesystem->sound().recordNextFrame();

    // Similarly record screen as needed
//    if (m_screen_exporter.get() != NULL)
//        m_screen_exporter->saveNext(m_screen);

    // Use the stored actions, which may or may not have changed this frame
    sum_rewards += oneStepAct(m_player_a_action, m_player_b_action);
  }

  return sum_rewards;
}

/** Applies the given actions (e.g. updating paddle positions when the paddle is used)
  *  and performs one simulation step in Stella. */
reward_t RetroEnvironment::oneStepAct(Action player_a_action, Action player_b_action) {
  // Once in a terminal state, refuse to go any further (special actions must be handled
  //  outside of this environment; in particular reset() should be called rather than passing
  //  RESET or SYSTEM_RESET.
  if (isTerminal())
    return 0;

  if(player_a_action == JOYPAD_SYSTEM_RESET) //Shai: added return 0 upon restart
	  return 0;
  // Convert illegal actions into NOOPs; actions such as reset are always legal
  noopIllegalActions(player_a_action, player_b_action);

  // Emulate in the emulator
  emulate(player_a_action, player_b_action);
  // Increment the number of frames seen so far
  m_state.incrementFrame();

  return m_settings->getReward();
}

bool RetroEnvironment::isTerminal() {
  return (m_settings->isTerminal() ||
    (m_max_num_frames_per_episode > 0 &&
     m_state.getEpisodeFrameNumber() >= m_max_num_frames_per_episode));
}

void RetroEnvironment::emulate(const Action& player_a_action, const Action& player_b_action, size_t num_steps) {

    // In joystick mode we only need to set the action events once
//    m_state.setActionJoysticks(player_a_action, player_b_action);
	m_rlesystem->getRetroAgent()->SetActions(player_a_action,player_b_action);

    for (size_t t = 0; t < num_steps; ++t) {
    	m_rlesystem->step();
    	m_settings->step(*m_rlesystem);
    }

  processScreen();
}

void RetroEnvironment::emulateStart(Action player_a_action, Action player_b_action, size_t num_steps) {
	m_rlesystem->getRetroAgent()->SetActions(player_a_action,player_b_action);
	for (size_t t = 0; t < num_steps; ++t) {
    	m_rlesystem->step();
    }
}

/** Accessor methods for the environment state. */
void RetroEnvironment::setState(const RLEState& state) {
  m_state = state;
}

const RLEState& RetroEnvironment::getState() const {
  return m_state;
}

void RetroEnvironment::processScreen() {
    // Copy screen over and we're done!
	size_t height = m_rlesystem->getRetroAgent()->getHeight();
	size_t width = m_rlesystem->getRetroAgent()->getWidth();
	if(m_screen.height() != height || m_screen.width() != width){
		m_screen.setDims(width, height);
	}
	int Bpp = m_rlesystem->getRetroAgent()->getBpp() / 8;
	int pitch = m_rlesystem->getRetroAgent()->getPitch();
	uint8_t* buffer = m_rlesystem->getCurrentFrameBuffer();
	for(unsigned i = 0 ; i < height; ++i){
		memcpy((uint8_t*)m_screen.getArray() + i*width *Bpp , buffer + i*pitch, width * Bpp);
	}
}
