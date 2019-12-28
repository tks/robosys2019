# robosys2019にて作成したデバイスドライバ

簡易的なLED信号機をコントロールする.


### 使い方
```
$ make                          #プログラムのコンパイル
$ sudo insmod myled.ko          #コンパイルしたデバイスドライバのインストール 
$ sudo chmod 666 /dev/myled0  
$ echo 0 > /dev/myled0          #車側の信号機を緑に
$ echo 1 > /dev/myled0　　　　　　#車側の信号機を赤に
```
