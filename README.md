# M5Stack のシリアル出力を別の M5Stack に ESP-NOW でブロードキャストする

`Serial`の代わりに`EspNowSerial`を使用することで，送信側（イニシエータ）になっている M5Stack がシリアルに出力した内容をシリアルに書くと同時に ESP-NOW で受信側（レスポンダ）になっている別の M5Stack に送信してシリアルに出力します。ESP-NOWは1度に最大250バイトしか送れないので，それ以上のサイズを送る場合は250バイトごとに分割して送信します。

イニシエータになっている M5Stack がシリアルに何を出力しているのかを確認するのに使用します。

## イニシエータ（Initiator）とレスポンダ（Responder）

ここでは，シリアル出力をブロードキャストする側をイニシエータ，ブロードキャストされてきたシリアル出力を受信してシリアルに出力する側をレスポンダと呼びます。イニシエータは複数でも問題ありません。

## 使い方

Windows 11 の PlatformIO 環境で開発・動作確認をしています。

今のところ，M5Stack BASIC と ATOM シリーズの設定を書いてありますが，ESP32 が載っている M5Stack シリーズならどれでも対応できると思います。

用意している環境は以下の通りです。接続したポートに応じてそれぞれの`upload_port`を修正してください。

- `m5atom-initiator`  
  ATOM をイニシエータにする
- `m5atom-responder`  
  ATOM をレスポンダにする
- `m5stack-initiator`  
  M5Stack をイニシエータにする
- `m5stack-responder`  
  M5Stack をレスポンダにする

## サンプルプログラム

<a href="https://gyazo.com/ba76c2d09c579dc066f06056abb5fdf2"><img src="https://i.gyazo.com/ba76c2d09c579dc066f06056abb5fdf2.jpg" alt="動作の様子" width="600"/></a>

サンプルプログラム（`main.cpp`）では，イニシエータの A ボタンを押すと，レスポンダ側のシリアルにイニシエータの MAC アドレスと送信データ，送信データのダンプが表示されます。

サンプルではデフォルトの送信先としてブロードキャストアドレス（`FF:FF:FF:FF:FF:FF`）を指定しています。

`[env]`の`build_flags`から`-DESPNOW_BROADCAST`を外すとイニシエータ 1 つとレスポンダ 1 つの 1:1 で動作する例になります。`peer`にそれぞれ相手の MAC アドレスを指定してください。
