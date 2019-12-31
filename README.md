千葉工業大学先進工学部の2019年度のロボットシステム学の課題1

デバイスドライバ製作

## 簡易的なLED信号機をコントロールする.

[![](https://img.youtube.com/vi/ZT8Q45xaxX0/0.jpg)](https://www.youtube.com/watch?v=ZT8Q45xaxX0)

https://youtu.be/ZT8Q45xaxX0

## 参考

課題の製作にあたり，以下の講義スライド上のコードの一部を利用しています．

https://github.com/ryuichiueda/robosys2019

## 環境
raspberry pi 3B+
raspbian (4.19)


## 実行方法


LEDと200Ω程度の抵抗を接続する．
GPIOの配置はmyled.cを参照してください．

```
$ make                          #プログラムのコンパイル
$ sudo insmod myled.ko          #コンパイルしたデバイスドライバのインストール 
$ sudo chmod 666 /dev/myled0  
$ echo 0 > /dev/myled0          #車側の信号機を緑に
$ echo 1 > /dev/myled0　　　　　　#車側の信号機を赤に
```
