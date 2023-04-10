# The MIT License (MIT)
# Copyright (c) 2023 I_HaL
# https://opensource.org/licenses/MIT

# Aquestalkの例題プログラム
# ・I2Sは各自で設定してください。
#     MicroPython ESPはBuiltIn DACを非サポートです。
#                 2023/04/10 MicroPython Version 1.19.1現在
# ・このプログラムを実行すると
#    「こんにちは」と現在の日時と時刻を発話します。
#

import machine
from machine import I2S
from machine import Pin
import sys
import aquestalk

#======= USER CONFIGURATION =======
SAMPLE_RATE_IN_HZ = 8000
# channelformat setting:
# I2Sについては各自で設定してください。
audio_out = I2S(
    0, 
#    sck=bck_pin, ws=ws_pin, sd=sdout_pin, 
    mode=I2S.DACTX,  #ESP32 built in DAC
    bits=16,
    format=I2S.MONO,
    rate=SAMPLE_RATE_IN_HZ,
    ibuf=1024)

# allocate sample arrays
#   memoryview used to reduce heap allocation in while loop
wav_samples = bytearray(320*2)
wav_samples_mv = memoryview(wav_samples)
print('Starting')
# continuously read audio samples from the WAV file 
# and write them to an I2S DAC
###########
aq=aquestalk.myclass(320,'') #Init

def Synthe(koe):
    cnt=0
    aq.SetKoe(koe,100,320*8)
    while True:
        try:
            r=aq.SyntheFrame(wav_samples_mv)
            if r==0:
                num_read = aq.pLen()*2
            else:
                num_read = 0
                break
            #audio_out.shift(buf=wav_samples_mv,bits=16,shift=1)
            num_written = 0
            # end of WAV file?
            if num_read == 0:
                break
            else:
                cnt=cnt+1
                # loop until all SyntheFrame are written to the I2S peripheral
                #print(wav_samples[0:20])
                while num_written < num_read:
                    print(".",end='')
                    num_written += audio_out.write(wav_samples_mv)#, timeout=0)
        except (KeyboardInterrupt, Exception) as e:
            print('caught exception {} {}'.format(type(e).__name__, e))
            break
    print()
###########

#Synthe('denwaba^ngo-wa')
Synthe('konnichiwa')
###########
rtc = machine.RTC()
r=rtc.datetime()
#(2023, 4, 10, 0, 17, 13, 26, 326153)
print(r)
str='<NUMK VAL={:d} COUNTER=nenn>,<NUMK VAL={:d} COUNTER=gatu>;<NUMK VAL={:d} COUNTER=nichi>.'\
     .format(r[0],r[1],r[2])
Synthe(str)
str='<NUMK VAL={:d} COUNTER=ji>;<NUMK VAL={:d} COUNTER=funn>/<NUMK VAL={:d} COUNTER=byo->desu.'\
     .format(r[4],r[5],r[6])
Synthe(str)

audio_out.deinit()
print('Done')
