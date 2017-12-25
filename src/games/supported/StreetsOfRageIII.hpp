/* *****************************************************************************
 * The line 67 is based on Xitari's code, from Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
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

#ifndef __STREETS_OF_RAGE_III_SETTINGS_HPP__
#define __STREETS_OF_RAGE_III_SETTINGS_HPP__
/* RL wrapper for AtariCollection settings */

#include "../GenesisSettings.hpp"
#include <unordered_set>

namespace rle {

class StreetsOfRageIIISettings : public GenesisSettings {

    public:

		StreetsOfRageIIISettings();

        // reset
        void reset();

        // the rom-name
        const char* rom() const { return "streets_of_rage_iii"; }

        // create a new instance of the rom
        RomSettings* clone() const;

        // process the latest information from ALE
        void step(const RleSystem& system);


       // saves the state of the rom settings
       void saveState( Serializer & ser );


        // loads the state of the rom settings
        void loadState( Deserializer & des );

        
		virtual const int lives() { return 0; }

        virtual ActionVect getStartingActions(const RleSystem& system);
        virtual ActionVect getExtraActions(const RleSystem& system);

		void startingOperations(RleSystem& system);
    private:
        reward_t m_score;
		int m_start_level;
        int m_end_level;
        int m_current_level;
        int m_lives;
        int m_health;
        int m_difficulty;
        int m_screen;

};

} // namespace rle

#endif // __STREETS_OF_RAGE_III_SETTINGS_HPP__
