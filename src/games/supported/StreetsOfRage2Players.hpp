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

#ifndef __STREETS_OF_RAGE_TWO_PLAYERS_SETTINGS_HPP__
#define __STREETS_OF_RAGE_TWO_PLAYERS_SETTINGS_HPP__
/* RL wrapper for AtariCollection settings */

#include "../GenesisSettings.hpp"
#include <unordered_set>

namespace rle {

class StreetsOfRage2PlayersSettings : public GenesisSettings {

    public:

		StreetsOfRage2PlayersSettings();

        // reset
        void reset();

        // the rom-name
        const char* rom() const { return "streets_of_rage_2_players"; }

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
        reward_t player_1_health;
        reward_t player_2_health;
        int player_1_lives;
        int player_2_lives;
        int m_current_level;
	    int distance;
        int screen;
};

} // namespace rle

#endif // __STREETS_OF_RAGE_TWO_PLAYERS_SETTINGS_HPP__
