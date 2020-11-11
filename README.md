# slib
## [English](#About)/[日本語](#このライブラリについて)

## About
This library is a collection of code that the author has been writing to simplify C++ coding.  

1. libsobj  
This library defines the main core objects (SObjects) and their inherited objects.  
* Examples of defined objects:  
Numbers (SNumber)  
Strings (SString)  
Object array (SArray)  
Associative array(SDictionary)  
Date (SDate)  
Byte sequence (SData)  
File (SFile)  
Shape(SFigure)  
Image (SImage)  
Table (STable)  
etc...  
  
Other features:  
* Fractions and complex numbers are available in SNumber object.  
* Japanese calender is available in SDate object.  
* Support for Japanese file path and Japanese output to command prompt on Windows.  
etc.  

2. libsapp  
This library makes it easy to create CUI applications. If you set up your application information in an associative array format first, you can have it automatically generate usage and help text. Additionally, a determination whether or not the runtime argument is a required option is also performed.  
This saves you from having to code the same function every time you create a small CUI app.  
　　
3. libsnodeapp  
This library is designed to create native plugins for node.js easily.  
It includes functions to convert javascript objects received from node.js via Napi into the objects defined in libsobj.  

4. libsbioinfo  
It is a library of bioinformatics-related functions created for research purposes.

## License
Please see the documents in license folder.

---

## このライブラリについて
このライブラリは、C++のコーディングを簡易にするために、昔から作者がチマチマと書き足してきたコードです。  

1. libsobj
メインとなる基幹オブジェクト(SObject)と、その継承を受けたオブジェクトを定義しているライブラリです。  
* 定義されているオブジェクト例：  
数値(SNumber)  
文字列(SString)  
オブジェクト配列(SArray)  
連想配列(SDictionary)  
日付(SDate)  
バイト列(SData)  
ファイル(SFile)  
図形(SFigure)  
画像(SImage)  
表(STable)  
など...  
  
その他の特徴としては  
* 仕事柄、科学や教育方面での応用のために、数値は分数や複素数に対応している  
* 日本の元号式の日付を解釈、出力できる  
* Windowsでの日本語ファイルパスや、コマンドプロンプトへの日本語出力に対応  
などがあります。  

2. libsapp  
このライブラリは、CUIアプリを簡単に作成するためのライブラリです。  
最初に連想配列形式でアプリケーションの情報を設定しておくと、使用法やヘルプの文言を自動生成したり、実行時引数の必須オプションの有無の判別などを行わせることができます。  
ちょっとしたCUIアプリを作るときに、似たような関数の使い回しをしなくて済みます。  
　　
3. libsnodeapp  
このライブラリは、node.js用のネイティブプラグイン作成の手間を省くためのライブラリです。  
node.jsからNapiを介して受け取ったjavascriptオブジェクトを、libsobjで定義した各オブジェクトに変換する関数などを含んでいます。  

4. libsbioinfo  
研究用に作成したバイオインフォマティクス関連の機能をまとめたライブラリです。


## ライセンス
licenseフォルダ内を参照してください。
