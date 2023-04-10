# MicroPython ESP32 Aquestalk 拡張
 MicroPythonのcモジュールから Aquestalk ESP32を呼び出せるようにしました。
 ただし音声の入力データ形式は、ローマ字表記音声記号列(ASCII)のみ対応しています。

## AquesTalk ESP32
評価版を下記からダウンロードすることができます。 

https://www.a-quest.com/products/aquestalk_esp32.html

使用制限などについては各自ご確認願います。

## ビルド

1. MicroPythonのソースコードをダウンロードします。

1. 下記のフォルダにaquestalk-esp32を展開します。
micropython/examples/usercmodule/VoiceSynth/

1. 下記のフォルダに移動します。
micropython/port/esp32

1. cモジュールを含めてビルドします。

   1.  esp-idfの設定をする。
       ```bash
       cd ~/esp/esp-idf  #各自のesp-idfのフォルダに合わせる。
       . ./export.sh
       ```
   1. MicroPythonをビルドします。
       ```bash
       make clean
       make submodules
       make USER_C_MODULES=../../../examples/usercmodule/VoiceSynth/micropython.cmake

## 使い方

```python
#こんにちはと発話する。
import aquestalk
wav_samples = bytearray(320*2)
wav_samples_mv = memoryview(wav_samples)
aq=aquestalk.myclass(320,'') #Init
koe='konnichiwa'
aq.SetKoe(koe,100,320*8)
r=0
while r==0:
    r=aq.SyntheFrame(wav_samples_mv)
    audio_out.write(wav_samples_mv) #I2SなどでPCMデータを送る。
```

## 例

MicroPython ESP32による例

micropython/examples/usercmodule/VoiceSynth/example/aquestalk.py


## プログラムをテストしている様子

[動作例](https://twitter.com/I_HaL/status/1636701151935758336 "動作例")



