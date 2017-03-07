# rle_python_interface.py
# Author: Ben Goodrich
# This directly implements a python version of the arcade learning
# environment interface.

from ctypes import *
import numpy as np
from numpy.ctypeslib import as_ctypes
import os
import sys

rle_lib = cdll.LoadLibrary(os.path.join(os.path.dirname(__file__),
                                        'librle_c.so'))

rle_lib.RLE_new.argtypes = None
rle_lib.RLE_new.restype = c_void_p
rle_lib.RLE_del.argtypes = [c_void_p]
rle_lib.RLE_del.restype = None
rle_lib.getString.argtypes = [c_void_p, c_char_p]
rle_lib.getString.restype = c_char_p
rle_lib.getInt.argtypes = [c_void_p, c_char_p]
rle_lib.getInt.restype = c_int
rle_lib.getBool.argtypes = [c_void_p, c_char_p]
rle_lib.getBool.restype = c_bool
rle_lib.getFloat.argtypes = [c_void_p, c_char_p]
rle_lib.getFloat.restype = c_float
rle_lib.setString.argtypes = [c_void_p, c_char_p, c_char_p]
rle_lib.setString.restype = None
rle_lib.setInt.argtypes = [c_void_p, c_char_p, c_int]
rle_lib.setInt.restype = None
rle_lib.setBool.argtypes = [c_void_p, c_char_p, c_bool]
rle_lib.setBool.restype = None
rle_lib.setFloat.argtypes = [c_void_p, c_char_p, c_float]
rle_lib.setFloat.restype = None
rle_lib.loadROM.argtypes = [c_void_p, c_char_p, c_char_p]
rle_lib.loadROM.restype = None
rle_lib.act.argtypes = [c_void_p, c_int, c_int]
rle_lib.act.restype = c_int
rle_lib.game_over.argtypes = [c_void_p]
rle_lib.game_over.restype = c_bool
rle_lib.reset_game.argtypes = [c_void_p]
rle_lib.reset_game.restype = None
rle_lib.getLegalActionSet.argtypes = [c_void_p, c_void_p]
rle_lib.getLegalActionSet.restype = None
rle_lib.getLegalActionSize.argtypes = [c_void_p]
rle_lib.getLegalActionSize.restype = c_int
rle_lib.getMinimalActionSet.argtypes = [c_void_p, c_void_p]
rle_lib.getMinimalActionSet.restype = None
rle_lib.getMinimalActionSize.argtypes = [c_void_p]
rle_lib.getMinimalActionSize.restype = c_int
rle_lib.getFrameNumber.argtypes = [c_void_p]
rle_lib.getFrameNumber.restype = c_int
rle_lib.lives.argtypes = [c_void_p]
rle_lib.lives.restype = c_int
rle_lib.getEpisodeFrameNumber.argtypes = [c_void_p]
rle_lib.getEpisodeFrameNumber.restype = c_int
rle_lib.getRAM.argtypes = [c_void_p, c_void_p]
rle_lib.getRAM.restype = None
rle_lib.getRAMSize.argtypes = [c_void_p]
rle_lib.getRAMSize.restype = c_int
rle_lib.getScreenWidth.argtypes = [c_void_p]
rle_lib.getScreenWidth.restype = c_int
rle_lib.getScreenHeight.argtypes = [c_void_p]
rle_lib.getScreenHeight.restype = c_int
rle_lib.getScreenBpp.argtypes = [c_void_p]
rle_lib.getScreenBpp.restype = c_int
rle_lib.getScreenRGB.argtypes = [c_void_p, c_void_p]
rle_lib.getScreenRGB.restype = None
rle_lib.getScreenGrayscale.argtypes = [c_void_p, c_void_p]
rle_lib.getScreenGrayscale.restype = None
rle_lib.saveState.argtypes = [c_void_p]
rle_lib.saveState.restype = None
rle_lib.loadState.argtypes = [c_void_p]
rle_lib.loadState.restype = None
#rle_lib.cloneState.argtypes = [c_void_p]
#rle_lib.cloneState.restype = c_void_p
#rle_lib.restoreState.argtypes = [c_void_p, c_void_p]
#rle_lib.restoreState.restype = None
#rle_lib.cloneSystemState.argtypes = [c_void_p]
#rle_lib.cloneSystemState.restype = c_void_p
#rle_lib.restoreSystemState.argtypes = [c_void_p, c_void_p]
#rle_lib.restoreSystemState.restype = None
#rle_lib.deleteState.argtypes = [c_void_p]
#rle_lib.deleteState.restype = None
#rle_lib.saveScreenPNG.argtypes = [c_void_p, c_char_p]
#rle_lib.saveScreenPNG.restype = None

class RLEInterface(object):
    def __init__(self):
        self.obj = rle_lib.RLE_new()

    def getString(self, key):
        return rle_lib.getString(self.obj, key)

    def getInt(self, key):
        return rle_lib.getInt(self.obj, key)

    def getBool(self, key):
        return rle_lib.getBool(self.obj, key)

    def getFloat(self, key):
        return rle_lib.getFloat(self.obj, key)

    def setString(self, key, value):
      rle_lib.setString(self.obj, key, value)

    def setInt(self, key, value):
      rle_lib.setInt(self.obj, key, value)

    def setBool(self, key, value):
      rle_lib.setBool(self.obj, key, value)

    def setFloat(self, key, value):
      rle_lib.setFloat(self.obj, key, value)

    def loadROM(self, rom_file, core):
        _ROOT = os.path.abspath(os.path.dirname(__file__))
        if 'snes' == core:
            core_path = _ROOT + '/snes9x2010_libretro.so'
        elif 'atari' == core:
            core_path = _ROOT + '/stella_libretro.so'
        elif 'genesis' == core or 'game_gear' == core or 'sg1000' == core:
            core_path = _ROOT + '/genesis_plus_gx_libretro.so'
        else:
            raise ValueError('core must be atari|snes|genesis|game_gear|sg1000')

        if (sys.version_info > (3, 0)):
            rle_lib.loadROM(self.obj, str.encode(rom_file), str.encode(core_path))
        else:
            rle_lib.loadROM(self.obj, rom_file, core_path)

    def act(self, actionA, actionB = 0):
        return rle_lib.act(self.obj, int(actionA), int(actionB))

    def game_over(self):
        return rle_lib.game_over(self.obj)

    def reset_game(self):
        rle_lib.reset_game(self.obj)

    def getLegalActionSet(self):
        act_size = rle_lib.getLegalActionSize(self.obj)
        act = np.zeros((act_size), dtype=np.intc)
        rle_lib.getLegalActionSet(self.obj, as_ctypes(act))
        return act

    def getMinimalActionSet(self):
        act_size = rle_lib.getMinimalActionSize(self.obj)
        act = np.zeros((act_size), dtype=np.intc)
        rle_lib.getMinimalActionSet(self.obj, as_ctypes(act))
        return act

    def getFrameNumber(self):
        return rle_lib.getFrameNumber(self.obj)

    def lives(self):
        return rle_lib.lives(self.obj)

    def getEpisodeFrameNumber(self):
        return rle_lib.getEpisodeFrameNumber(self.obj)

    def getScreenDims(self):
        """returns a tuple that contains (screen_width, screen_height)
        """
        width = rle_lib.getScreenWidth(self.obj)
        height = rle_lib.getScreenHeight(self.obj)
        return (width, height)

    def getScreenRGB(self, screen_data=None):
        """This function fills screen_data with the data in RGB format
        screen_data MUST be a numpy array of uint8. This can be initialized like so:
        screen_data = np.empty((height,width,4), dtype=np.uint8)
        The channels of the image are RGBA, A being Alpha. Currently unused.
        If it is None,  then this function will initialize it.
        """
        if screen_data is None:
            width = rle_lib.getScreenWidth(self.obj)
            height = rle_lib.getScreenHeight(self.obj)
            screen_data = np.empty((height, width, 4), dtype=np.uint8)
        rle_lib.getScreenRGB(self.obj, as_ctypes(screen_data[:]))
        return screen_data

    def getScreenRGBWithWithDims(self, width, height, screen_data=None):
        """This is another version of 'getScreenRGB' 
        so that screen dimensions are able to be specified explicitly.
        """
        if screen_data is None:
            screen_data = np.empty((height, width, 4), dtype=np.uint8)
        rle_lib.getScreenRGBWidthDims(self.obj, as_ctypes(screen_data[:]), width, height)
        return screen_data

    def getScreenGrayscale(self, screen_data=None):
        """This function fills screen_data with the data in grayscrle
        screen_data MUST be a numpy array of uint8. This can be initialized like so:
        screen_data = np.empty((height,width,1), dtype=np.uint8)
        If it is None,  then this function will initialize it.
        """
        if(screen_data is None):
            width = rle_lib.getScreenWidth(self.obj)
            height = rle_lib.getScreenHeight(self.obj)
            screen_data = np.empty((height, width,1), dtype=np.uint8)
        rle_lib.getScreenGrayscale(self.obj, as_ctypes(screen_data[:]))
        return screen_data

    def getScreenGrayscaleWithDims(self, width, height, screen_data=None):
        """This is another version of 'getScreenGrayscale' 
        so that screen dimensions are able to be specified explicitly.
        """
        if(screen_data is None):
            screen_data = np.empty((height, width,1), dtype=np.uint8)
        rle_lib.getScreenGrayscaleWithDims(self.obj, as_ctypes(screen_data[:]), width, height)
        return screen_data

    def getRAMSize(self):
        return rle_lib.getRAMSize(self.obj)

    def getRAM(self, ram=None):
        """This function grabs the game's RAM.
        ram MUST be a numpy array of uint8/int8. This can be initialized like so:
        ram = np.array(ram_size, dtype=uint8)
        Notice: It must be ram_size where ram_size can be retrieved via the getRAMSize function.
        If it is None,  then this function will initialize it.
        """
        if(ram is None):
            ram_size = rle_lib.getRAMSize(self.obj)
            ram = np.zeros(ram_size, dtype=np.uint8)
        rle_lib.getRAM(self.obj, as_ctypes(ram))
        return ram

#    def saveScreenPNG(self, filename):
#        """Save the current screen as a png file"""
#        return rle_lib.saveScreenPNG(self.obj, filename)

    def saveState(self):
        """Saves the state of the system"""
        return rle_lib.saveState(self.obj)

    def loadState(self):
        """Loads the state of the system"""
        return rle_lib.loadState(self.obj)

#    def cloneState(self):
#        """This makes a copy of the environment state. This copy does *not*
#        include pseudorandomness, making it suitable for planning
#        purposes. By contrast, see cloneSystemState.
#        """
#        return rle_lib.cloneState(self.obj)

#    def restoreState(self, state):
#        """Reverse operation of cloneState(). This does not restore
#        pseudorandomness, so that repeated calls to restoreState() in
#        the stochastic controls setting will not lead to the same
#        outcomes.  By contrast, see restoreSystemState.
#        """
#        rle_lib.restoreState(self.obj, state)

#    def cloneSystemState(self):
#        """This makes a copy of the system & environment state, suitable for
#        serialization. This includes pseudorandomness and so is *not*
#        suitable for planning purposes.
#        """
#        return rle_lib.cloneSystemState(self.obj)

#    def restoreSystemState(self, state):
#        """Reverse operation of cloneSystemState."""
#        rle_lib.restoreSystemState(self.obj, state)

#    def deleteState(self, state):
#        """ Deallocates the RLEState """
#        rle_lib.deleteState(state)

    def __del__(self):
        rle_lib.RLE_del(self.obj)
