import numpy as np
import os
import librosa
import scipy
import time
import argparse

def create_folder(fd):
    if not os.path.exists(fd):
        os.makedirs(fd)

opus_cmd = 'F:\opus\opus-1.3.1.tar\opus-1.3.1\win32\VS2015\Win32\Debug\opus_demo.exe'
sox_cmd = 'E:\SOX\sox-14-4-2\sox.exe'

input_dir = 'E:\主机虚拟机共享文件\\thchs30_openslr_20170902\data_thchs30\\train\\A11_216.wav'
wav16_dir = 'F:\音频库\opus_demo\\wav16_path\\A11_216.wav'
raw_dir = 'F:\音频库\opus_demo\\raw_path\\A11_216.raw'
opus_dir = 'F:\音频库\opus_demo\\opus_path\\A11_216.opus'
pcm_dir = 'F:\音频库\opus_demo\\pcm_path\\A11_216.pcm'
out_dir = 'F:\音频库\opus_demo\\wav_path\\A11_216.wav'
time1 = time.time()
aa = os.system(sox_cmd + ' ' + '-r 16000' + ' ' + input_dir + ' ' + '-r 16000' + ' '+  wav16_dir)
a = os.system(sox_cmd + ' ' + '-t wav -c 1 -e signed-integer -b 16 -r  16000' + ' ' + wav16_dir + ' ' + raw_dir)
print("run encode")
b = os.system(opus_cmd + ' ' + '-e voip 16000 1 24000 -bandwidth WB -complexity 1' + ' ' + raw_dir + ' ' + opus_dir)
# c = os.system(opus_cmd + ' ' + '-d 16000 1' + ' ' + opus_dir + ' ' + pcm_dir)
# d = os.system(sox_cmd + ' ' + '-t raw -c 1 -e signed-integer -b 16 -r 16000' + ' ' + pcm_dir + ' ' + out_dir)
time2 = time.time()
print(time2-time1)

tt = np.loadtxt("F:\\opus\\opus数据库\\opus数据库\\NN\\DataSet1\\pred\\one_seed\\A11_217\\tt.txt")
np.savetxt("F:\\opus\\opus数据库\\opus数据库\\NN\\DataSet1\\pred\\test_seed\\de\\tt.txt",tt,fmt="%d")
np.savetxt("F:\\opus\\opus数据库\\opus数据库\\NN\\DataSet1\\pred\\test_seed\\en\\tt.txt",tt,fmt="%d")
