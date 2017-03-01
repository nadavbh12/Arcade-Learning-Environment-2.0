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
#include "Roms.hpp"
#include "RomSettings.hpp"
#include "RomUtils.hpp"

// include the game implementations

/// snes games
#include "supported/AtariCollection.hpp"
#include "supported/ClassicKong.hpp"
#include "supported/FinalFight.hpp"
#include "supported/FZero.hpp"
#include "supported/FZeroNoSpeed.hpp"
#include "supported/GradiusIII.hpp"
#include "supported/MortalKombat2Players.hpp"
#include "supported/NBAGiveNGo.hpp"
#include "supported/SuperMarioWorld.hpp"
#include "supported/SuperMarioAllStars.hpp"
#include "supported/SuperMarioAllStarsNoRight.hpp"
#include "supported/TetrisAndDrMario.hpp"
#include "supported/Boxing.hpp"
#include "supported/Wolfenstein.hpp"
#include "supported/Aladdin.hpp"
#include "supported/StreetFighterII.hpp"
#include "supported/StreetFighterIITurboHyperFighting.hpp"
#include "supported/BustAMove.hpp"
#include "supported/SuperMarioKart.hpp"

// genesis games
#include "supported/SonicTheHedgehog.hpp"

// Atari Games
//#include "supported/Breakout.hpp"
//#include "supported/Carnival.hpp"
//#include "supported/Centipede.hpp"
//#include "supported/ChopperCommand.hpp"
//#include "supported/CrazyClimber.hpp"
//#include "supported/Defender.hpp"
//#include "supported/DemonAttack.hpp"
//#include "supported/DoubleDunk.hpp"
//#include "supported/ElevatorAction.hpp"
//#include "supported/Enduro.hpp"
//#include "supported/FishingDerby.hpp"
//#include "supported/Freeway.hpp"
//#include "supported/Frostbite.hpp"
//#include "supported/Gopher.hpp"
//#include "supported/Gravitar.hpp"
//#include "supported/Hero.hpp"
//#include "supported/IceHockey.hpp"
//#include "supported/JamesBond.hpp"
//#include "supported/JourneyEscape.hpp"
//#include "supported/Kangaroo.hpp"
//#include "supported/Krull.hpp"
//#include "supported/KungFuMaster.hpp"
//#include "supported/MontezumaRevenge.hpp"
//#include "supported/MsPacman.hpp"
//#include "supported/NameThisGame.hpp"
//#include "supported/Phoenix.hpp"
//#include "supported/Pitfall.hpp"
//#include "supported/Pong.hpp"
//#include "supported/Pooyan.hpp"
//#include "supported/PrivateEye.hpp"
//#include "supported/QBert.hpp"
//#include "supported/RiverRaid.hpp"
//#include "supported/RoadRunner.hpp"
//#include "supported/RoboTank.hpp"
#include "supported/Seaquest.hpp"
//#include "supported/Skiing.hpp"
//#include "supported/Solaris.hpp"
//#include "supported/SpaceInvaders.hpp"
//#include "supported/StarGunner.hpp"
//#include "supported/Tennis.hpp"
//#include "supported/Tetris.hpp"
//#include "supported/TimePilot.hpp"
//#include "supported/Tutankham.hpp"
//#include "supported/UpNDown.hpp"
//#include "supported/Venture.hpp"
//#include "supported/VideoPinball.hpp"
//#include "supported/WizardOfWor.hpp"
//#include "supported/YarsRevenge.hpp"
//#include "supported/Zaxxon.hpp"

using namespace rle;

/* list of supported games */
static const RomSettings *roms[]  = {
	// Atari Games
//    new AirRaidSettings(),
//    new AlienSettings(),
//    new AmidarSettings(),
//    new AssaultSettings(),
//    new AsterixSettings(),
//    new AsteroidsSettings(),
//    new AtlantisSettings(),
//    new BankHeistSettings(),
//    new BattleZoneSettings(),
//    new BeamRiderSettings(),
//    new BerzerkSettings(),
//    new BowlingSettings(),
    new BoxingSettings(),
//    new BreakoutSettings(),
//    new CarnivalSettings(),
//    new CentipedeSettings(),
//    new ChopperCommandSettings(),
//    new CrazyClimberSettings(),
//    new DefenderSettings(),
//    new DemonAttackSettings(),
//    new DoubleDunkSettings(),
//    new ElevatorActionSettings(),
//    new EnduroSettings(),
//    new FishingDerbySettings(),
//    new FreewaySettings(),
//    new FrostbiteSettings(),
//    new GopherSettings(),
//    new GravitarSettings(),
//    new HeroSettings(),
//    new IceHockeySettings(),
//    new JamesBondSettings(),
//    new JourneyEscapeSettings(),
//    new KangarooSettings(),
//    new KrullSettings(),
//    new KungFuMasterSettings(),
//    new MontezumaRevengeSettings(),
//    new MsPacmanSettings(),
//    new NameThisGameSettings(),
//    new PhoenixSettings(),
//    new PitfallSettings(),
//    new PongSettings(),
//    new PooyanSettings(),
//    new PrivateEyeSettings(),
//    new QBertSettings(),
//    new RiverRaidSettings(),
//    new RoadRunnerSettings(),
//    new RoboTankSettings(),
    new SeaquestSettings(),
//    new SkiingSettings(),
//    new SolarisSettings(),
//    new SpaceInvadersSettings(),
//    new StarGunnerSettings(),
//    new TennisSettings(),
//    new TetrisSettings(),
//    new TimePilotSettings(),
//    new TutankhamSettings(),
//    new UpNDownSettings(),
//    new VentureSettings(),
//    new VideoPinballSettings(),
//    new WizardOfWorSettings(),
//    new YarsRevengeSettings(),
//    new ZaxxonSettings(),

	// SNES games
	new AtariCollectionSettings(),
	new ClassicKongSettings(),
	new FinalFightSettings(),
	new FZeroSettings(),
	new FZeroNoSpeedSettings(),
	new GradiusIIISettings(),
    new MortalKombatSettings(),
    new MortalKombat2PlayersSettings(),
	new NBAGiveNGoSettings(),
    new SuperMarioAllStarsSettings(),
    new SuperMarioAllStarsNoRightSettings(),
    new SuperMarioKartSettings(),
    new SuperMarioWorldSettings(),
	new TetrisAndDrMarioSettings(),
	new WolfensteinSettings(),
	new AladdinSettings(),
	new StreetFighterIISettings(),
	new StreetFighterIITurboHyperFightingSettings(),
	new BustAMoveSettings(),
//	Genesis games
	new SonicTheHedgehogSettings()
};



/* looks for the RL wrapper corresponding to a particular rom title */
RomSettings *rle::buildRomRLWrapper(const std::string &rom, bool twoPlayers) {

    size_t slash_ind = rom.find_last_of("/\\");
    std::string rom_str = rom.substr(slash_ind + 1);
    size_t dot_idx = rom_str.find_first_of(".");
    rom_str = rom_str.substr(0, dot_idx);
    std::transform(rom_str.begin(), rom_str.end(), rom_str.begin(), ::tolower);

    if(twoPlayers){
    	rom_str += "_2_players";
    }
    for (size_t i=0; i < sizeof(roms)/sizeof(roms[0]); i++) {
        if (rom_str == roms[i]->rom()) return roms[i]->clone();
    }
    std::cerr << COLOR_RED << "ERROR: " << rom << " ROM file name is invalid." << endl;
    std:: cerr << "Rename the ROM so it is underscore separated. For example: super_mario_world.sfc" << COLOR_RESET << endl;
    exit(1);
}

