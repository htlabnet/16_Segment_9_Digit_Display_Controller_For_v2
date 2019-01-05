//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//	hexSegClient.ino
//	16 segment 9 digit displayをarduinoから制御するための"サンプル"
//
//	Created by thotgamma. (https://gammalab.net)
//
//	16Segment9DigitDisplay公式:
//		https://htlab.net/products/16-segment-9-digit-display-board-3/
//		https://htlab.net/products/16-segment-9-digit-display-controller-2/
//
//  使い方
//    ArduinoのGNDをコントローラーのSerialピンヘッダのGNDに、
//    ArduinoのTXピンをコントローラーのSerialピンヘッダの**TXD**ピンに接続する。
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//16セグに表示する文字を「1つずつ」指定して
//設定するための関数
void setHexSegChar(int digit, char character, bool dotflag){
  uint8_t command = (dotflag ? 0b11110000 : 0b11100000) | digit;
  Serial.write(command);
	Serial.write(character);
}

//任意の文字列(9文字まで)を投げると適当に表示してくれる関数
//「HTLAB.NET!」など、文中に「.」が含まれていてもよしなに処理してくれるはず。
void setHexSegStr(String input){
	String buff = "";
	int dotFlag[input.length()];

  // まず入力文字列を「ドットを消去した文字列」と、
  // 「何番目の文字にドットを付与するか」というデータに分解する。
	for(int i = 0; i < input.length(); i++){
		if(input[i] == '.'){
			if(i == 0){ // 文頭に'.'がある場合は付与すべき文字がまだないので、空白文字を挿入しておく。
				buff += ' ';
			}
			dotFlag[buff.length()-1] = true;
		}else{
			buff += input[i];
			dotFlag[buff.length()-1] = false;
		}
	}

  // 生成したデータをもとに実際に表示する文字情報をコントローラに送信する。
	for(int i = 0; i < 9 && i < buff.length(); i++){
			setHexSegChar(i, buff[i], dotFlag[i] == true);
	}

  // 入力文字が9桁未満の場合、空きスペースを埋める。
	for(int i = buff.length(); i < 9; i++){
		setHexSegChar(i, ' ', false);
	}
}


void setup() {
  Serial.begin(9600);
	sample1("MAKER FAIRE TOKYO HTLABNET BOOTH! THIS IS 16 SEGMENT 9 DIGIT DISPLAY");
}

// 定型文表示の場合。ループで表示文を投げておくことをお勧めする。
void loop() {
  //setHexSegStr("HTLAB.NET!");
  delay(100);
}

// 適当なサンプル。長い文字列を入れると適当にスクロールして表示してくれる。
// "."を処理するコードは書いてない。誰かプルリクして(投げやり)。 
void sample1(String input){
  input += "         ";
	while(true){
	  for(int i = 0; i <= input.length()-9; i++){
      setHexSegStr(input.substring(i,i+9));
      delay(800);
	  }
	}
}

