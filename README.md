# MIDI interface for Sharp MZ-40K(mz40kmidi)
シャープのワンボードマイクロコンピュータ・MZ-40Kのオプション鍵盤コネクタを利用して、これをMIDIに変換するインタフェースにてキーボードやシーケンサーから制御できるようにしてみました。
## 必要な物
- Arduino Uno及び互換機
- バニラシールドなどのユニバーサル基板
- MIDIシールド([MocoLUFA](http://morecatlab.akiba.coocan.jp/lab/index.php/aruino/midi-firmware-for-arduino-uno-moco/)などでArduinoをUSB-MIDIデバイス化する場合は不要)
- 部品を少々
## シールド
MZ-40K用オプションの鍵盤形キーボード・MZ-40K2を模擬するシールドを製作しました。

![shield](https://github.com/NibblesLab/mz40kmidi/blob/master/mz40kmidi.PNG)

キーボードのストローブ信号は必要時以外はハイインピーダンス状態で、LED表示など他の機能と兼ねて使用されており、またGNDが出ていないことからシンプルにArduinoに入力させてもうまくいきません。そこでフォトMOSリレーを最小限で使用することによりキーボードの模擬が行えるようになっています。

MZ-40Kの鍵盤キーボード用コネクタ(オプション(2))はこのように信号が並んでいます(トップビュー)。R0～R7がストローブ(入力)、K0～K3がスキャン(出力)です。

|R0|R1|R2|R3|R4|R5|R6|R7|K0|K1|K2|K3|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|

![shield](http://retropc.net/ohishi/museum/mz40kmidi_1.jpg)

MIDIシールドと重ねて使用します(MocoLUFA対応させたArduino Unoを使う場合は不要)。

![shield](http://retropc.net/ohishi/museum/mz40kmidi_2.jpg)
![shield](http://retropc.net/ohishi/museum/mz40kmidi_3.jpg)

## ソフト
Arduino MIDI Libralyを使用しています。基本的にはノートオンを受信したら相当するキーを押下状態にし、ノートオフを受信したら押下状態を解除するだけのプログラムです。ノートオフが来ないまま複数のノートオンが有効になっている時は、最後に来たノートオンの音を鳴らし、その音のノートオフが来た場合は最後から2番目の音を鳴らす…というように、ノートオンの降着優先で鳴らす音を選んでいます。nキーロールオーバーのような効果があるはずです。

また、鍵盤を押さえた(ノートオンが出ている)状態で、MIDIシールドのLEDが点灯します。

その他、[こちらのページ](http://retropc.net/ohishi/museum/mz40k2.htm)も参考にして下さい。

# Keypad interface for Sharp MZ-40K(mz40kKeypad)
上記のシールドを流用して、外部機器からUSBシリアルインタフェースを通じてMZ-40Kのキー入力を行うスケッチを追加しました。

## ソフト
mz40kmidiの代わりにmz40kKeypadを書き込んで下さい。適当なターミナルから数値・アルファベット・記号などを入力すると、MZ-40Kの相当するキーが押された状態になります。0～9・A～F以外の特殊キーは次のように割り当てています。

|入力文字|キー|
|:-:|:-:|
|@|ADR|
|R|READ|
|W|WRITE|
|L|CLEAR|

アルファベットは小文字でも受け付けます。なおRUNキーとSTOPキーは外部キーのマトリクスに含まれないので入力できません。この二つだけは本体のキーを操作してください。
