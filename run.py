#_*conding:utf-8_*_
''' 
__author__ = YuyongKang, NenghengZheng
 __date__ = '2020/12/1' 
 __filename__ = 'run.py.py'
 __IDE__ = 'PyCharm'
 __copyright__ = Shenzhen University ,Electronic and infromation college,
 Intelligent speech and artificial hearing Lab
 '''

import numpy as np
import os
import librosa
import scipy
import time
import argparse

def create_folder(fd):
    if not os.path.exists(fd):
        os.makedirs(fd)

#input_path = 'F:\音频库\opus_demo\\input_path'
input_path = 'E:\主机虚拟机共享文件\\thchs30_openslr_20170902\data_thchs30\\train'

raw_path = 'F:\音频库\opus_demo\\raw_path'
wav16_path = 'F:\音频库\opus_demo\\wav16_path'
opus_path = 'F:\音频库\opus_demo\\opus_path'
pcm_path = 'F:\音频库\opus_demo\\pcm_path'
wav_path = 'F:\音频库\opus_demo\\wav_path'

create_folder(raw_path)
create_folder(wav16_path)
create_folder(opus_path)
create_folder(pcm_path)
create_folder(wav_path)

opus_cmd = 'F:\opus\opus-1.3.1.tar\opus-1.3.1\win32\VS2015\Win32\Debug\opus_demo.exe'
sox_cmd = 'E:\SOX\sox-14-4-2\sox.exe'
# file = np.loadtxt(r"F:/opus/opus数据库/opus数据库/NN/DataSet1//wav_name.txt",
#                   dtype=str,unpack=True,delimiter=',')
# wav_file = file[0:152]
input_list = os.listdir(input_path)
input_list = input_list[0:2000]
for name in input_list:
    nameflag = name.split('.')
    flag = nameflag[-1]
    if flag !='wav':
        input_list.remove(name)

# input_list = wav_file
i = 0
time_all_start = time.time()
for name in input_list:
    nameflag = name.split('.')
    name_split = nameflag[0]
    flag = nameflag[-1]
    if flag !='wav':
        continue
    input_dir = os.path.join(input_path, name)
    print(name)
    time1 = time.time()
    # name = name.split(' ')[1]
    # print(name)
    # input_dir = os.path.join(input_path, name)
    # name_split = name.split('.')[0]
    # i += 1
    # if i == 151:
    #     break
    # print(i,name)
    wav16_dir = os.path.join(wav16_path, name_split + '.wav')

    aa = os.system(sox_cmd + ' ' + '-r 16000' + ' ' + input_dir + ' ' + '-r 16000' + ' '+  wav16_dir)


    raw_dir = os.path.join(raw_path, name_split + '.raw')

    a = os.system(sox_cmd + ' ' + '-t wav -c 1 -e signed-integer -b 16 -r  16000' + ' ' + wav16_dir + ' ' + raw_dir)

    opus_dir = os.path.join(opus_path, name_split + '.opus')
    b = os.system(opus_cmd + ' ' + '-e voip 16000 1 24000 -bandwidth WB -complexity 1' + ' ' + raw_dir + ' ' + opus_dir)

    pcm_dir = os.path.join(pcm_path, name_split + '.pcm')
    c = os.system(opus_cmd + ' ' + '-d 16000 1' + ' ' + opus_dir + ' ' + pcm_dir)

    out_dir = os.path.join(wav_path, name_split + '.wav')
    d = os.system(sox_cmd + ' ' + '-t raw -c 1 -e signed-integer -b 16 -r 16000' + ' ' + pcm_dir + ' ' + out_dir)

    tt = np.loadtxt("F:\\opus\\opus数据库\\opus数据库\\NN\\DataSet1\\pred\\one_seed\\A11_217\\tt.txt")
    np.savetxt("F:\\opus\\opus数据库\\opus数据库\\NN\\DataSet1\\pred\\test_seed\\de\\tt.txt", tt, fmt="%d")
    np.savetxt("F:\\opus\\opus数据库\\opus数据库\\NN\\DataSet1\\pred\\test_seed\\en\\tt.txt", tt, fmt="%d")
    time2 = time.time()
    print("time is %fs"%(time2-time1))

time_all_end = time.time()
print('all run time is %fs'%(time_all_end-time_all_start))





