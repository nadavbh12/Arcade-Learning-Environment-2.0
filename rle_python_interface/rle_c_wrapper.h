#ifndef __RLE_C_WRAPPER_H__
#define __RLE_C_WRAPPER_H__

#include <rle_interface.hpp>
#include <cstring>

using namespace rle;

extern "C" {
  // Declares int rgb_prlette[256]
  RLEInterface *RLE_new() {return new RLEInterface();}
  void RLE_del(RLEInterface *rle){delete rle;}
  const char *getString(RLEInterface *rle, const char *key){return rle->getString(key).c_str();}
  int getInt(RLEInterface *rle,const char *key) {return rle->getInt(key);}
  bool getBool(RLEInterface *rle,const char *key){return rle->getBool(key);}
  float getFloat(RLEInterface *rle,const char *key){return rle->getFloat(key);}
  void setString(RLEInterface *rle,const char *key,const char *value){rle->setString(key,value);}
  void setInt(RLEInterface *rle,const char *key,int value){rle->setInt(key,value);}
  void setBool(RLEInterface *rle,const char *key,bool value){rle->setBool(key,value);}
  void setFloat(RLEInterface *rle,const char *key,float value){rle->setFloat(key,value);}
  void loadROM(RLEInterface *rle,const char *rom_file, const char *core_file){rle->loadROM(rom_file,core_file);}
  int act(RLEInterface *rle,int actionA, int actionB){return rle->act((Action)actionA, (Action)actionB);}
  bool game_over(RLEInterface *rle){return rle->game_over();}
  void reset_game(RLEInterface *rle){rle->reset_game();}
  void getLegalActionSet(RLEInterface *rle,int *actions){
    ActionVect action_vect = rle->getLegalActionSet();
    for(unsigned int i = 0;i < rle->getLegalActionSet().size();i++){
      actions[i] = action_vect[i];
    }
  }
  int getLegalActionSize(RLEInterface *rle){return rle->getLegalActionSet().size();}
  void getMinimalActionSet(RLEInterface *rle,int *actions){
    ActionVect action_vect = rle->getMinimalActionSet();
    for(unsigned int i = 0;i < rle->getMinimalActionSet().size();i++){
      actions[i] = action_vect[i];
    }
  }
  int getMinimalActionSize(RLEInterface *rle){return rle->getMinimalActionSet().size();}
  int getFrameNumber(RLEInterface *rle){return rle->getFrameNumber();}
  int lives(RLEInterface *rle){return rle->lives();}
  int getEpisodeFrameNumber(RLEInterface *rle){return rle->getEpisodeFrameNumber();}
  void getRAM(RLEInterface *rle, unsigned char *ram);
  int getRAMSize(RLEInterface *rle){return rle->getRAM().size();}
  int getScreenWidth(RLEInterface *rle){return rle->getScreen().width();}
  int getScreenHeight(RLEInterface *rle){return rle->getScreen().height();}
  int getScreenBpp(RLEInterface *rle){return rle->getScreen().getBpp();}

  void getScreenRGB(RLEInterface *rle, unsigned char *output_buffer);

  void getScreenGrayscale(RLEInterface *rle, unsigned char *output_buffer);
  void getScreenRGBWithDims(RLEInterface *rle, unsigned char *output_buffer, size_t width, size_t height);
  void getScreenGrayscaleWithDims(RLEInterface *rle, unsigned char *output_buffer, size_t width, size_t height);

  void saveState(RLEInterface *rle){rle->saveState();}
  void loadState(RLEInterface *rle){rle->loadState();}
//  RLEState* cloneState(RLEInterface *rle){return new RLEState(rle->cloneState());}
//  void restoreState(RLEInterface *rle, RLEState* state){rle->restoreState(*state);}
//  RLEState* cloneSystemState(RLEInterface *rle){return new RLEState(rle->cloneSystemState());}
//  void restoreSystemState(RLEInterface *rle, RLEState* state){rle->restoreSystemState(*state);}
//  void deleteState(RLEState* state){delete state;}
//  void saveScreenPNG(RLEInterface *rle,const char *filename){rle->saveScreenPNG(filename);}
}

#endif
