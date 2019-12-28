# 簡易的なLED信号機をコントロールする.

[![](https://img.youtube.com/vi/ZT8Q45xaxX0/0.jpg)](https://www.youtube.com/watch?v=ZT8Q45xaxX0)

https://youtu.be/ZT8Q45xaxX0


### 使い方
```
$ make                          #プログラムのコンパイル
$ sudo insmod myled.ko          #コンパイルしたデバイスドライバのインストール 
$ sudo chmod 666 /dev/myled0  
$ echo 0 > /dev/myled0          #車側の信号機を緑に
$ echo 1 > /dev/myled0　　　　　　#車側の信号機を赤に
```
