# ephemeris_jcg

ビルド方法
==========

`make`

（やり直す場合は、 `make clean` をしてから）

準備
====

* 海保サイトからダウンロードした各年の係数ファイル(`na99-data.txt` という名称のファイル)を `txt` ディレクトリに配置する。
* 説明書に記載の ΔT の値を一覧にしたファイルを `txt` ディレクトリに配置する。

実行方法
========

`./ephemeris_jcg [YYYYMMDDHHMMSSMMMMMMMMM]`

* コマンドライン引数に指定するのは時刻は UT1（世界時1）である。
* UT1（世界時1）は「年・月・日・時・分・秒・ナノ秒」を最大23桁で指定する。
* UT1（世界時1）を指定しない場合は、システム日時を UT1 とみなす。
* UT1（世界時1）を先頭から部分的に指定した場合は、指定していない部分を 0 とみなす。
