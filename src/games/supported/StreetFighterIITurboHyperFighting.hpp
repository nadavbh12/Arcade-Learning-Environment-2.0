#ifndef __STREET_FIGHTER_II_TURBO_HYPER_FIGHTING_SETTINGS_HPP__
#define __STREET_FIGHTER_II_TURBO_HYPER_FIGHTING_SETTINGS_HPP__
/* RL wrapper for AtariCollection settings */

#include "../SnesSettings.hpp"

namespace rle {

struct StreetFighterIITurboHyperFightingSettings : public SnesSettings {

    public:

        StreetFighterIITurboHyperFightingSettings();

        // reset
        virtual void reset();

        // the rom-name
        virtual const char* rom() const { return "street_fighter_ii_turbo_hyper_fighting"; }

        // create a new instance of the rom
        virtual RomSettings* clone() const;

        // process the latest information from ALE
        virtual void step(const RleSystem& system);

        // saves the state of the rom settings
        virtual void saveState( Serializer & ser );

        // loads the state of the rom settings
        virtual void loadState( Deserializer & des );

        virtual const int lives() { return 0; }

        virtual ActionVect selectChar(int character_index);

        virtual int getCharacterIndex(const RleSystem& system);

        virtual ActionVect getStartingActions(const RleSystem& system);

        virtual ActionVect selectSpeedActions(const RleSystem& system);

        virtual ActionVect selectDifficultyActions(const RleSystem& system);


    protected:
        int m_wins;
        int o_wins;
        int m_health;
        int o_health;
        bool match_ended;
        int m_speed = 3;        // Default speed value. Valid range [0-4]
        int m_difficulty = 3;   // Default difficulty value. Valid range [0-7]
};

} // namespace rle

#endif // __STREET_FIGHTER_II_TURBO_HYPER_FIGHTING_SETTINGS_HPP__