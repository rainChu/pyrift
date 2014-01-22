from ctypes import *
import os
import sys
import threading
import time
import ctypes

pr = None

root = os.path.dirname(__file__)

if sys.platform in ["linux", "linux2"]:
    try:
        pr = CDLL(os.path.join(root, "_pyrift.so"))
    except OSError:
        pr = CDLL("_pyrift.so")
elif sys.platform == "win32":
    pr = CDLL("pyrift")

# Set initial values in the DLL    
pr.prepare()

def _raise_errors(error_code):
    if error_code == 1:
        raise Exception("Could not detect an Oculus Rift", "NO_DEVICE")
    elif error_code == 2:
        raise Exception("Device not initialized; call initialize() first.", "NOT_INITIALIZED")

def initialize():
    _raise_errors(pr.initialize())

def get_orientation():
    yaw = ctypes.c_float()
    pitch = ctypes.c_float()
    roll = ctypes.c_float()
    
    _raise_errors(pr.get_orientation(ctypes.byref(yaw), ctypes.byref(pitch), ctypes.byref(roll)))
    return yaw.value, pitch.value, roll.value

def get_orientation_quaternion():
    x = ctypes.c_float()
    y = ctypes.c_float()
    z = ctypes.c_float()
    w = ctypes.c_float()

    _raise_errors(pr.get_orientation_quaternion(ctypes.byref(x), ctypes.byref(y), ctypes.byref(z), ctypes.byref(w)))
    return x.value, y.value, z.value, w.value


