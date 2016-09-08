//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// arduino driving 7 segment display and keypad
// 
// Made by Lee Witek
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/2655492-arduino-driving-7-segment-display-and-keypad


int commons[] = {13, 12, 11, 10};
int segments[] = {A0, A1, A2, A3, A4, A5, 5, 4};
int padcols[] = {9, 8, 7, 6};

void setupDisp () {
  for (int n = 0; n < 4; n++) {
  	pinMode(commons[n], INPUT);
  }
  
  for (int n = 0; n < 8; n++) {
  	pinMode(segments[n], OUTPUT);
	digitalWrite(segments[n], 0);	
  }
}

void setupPad () {
  for (int n = 0; n < 4; n++) {
  	pinMode(padcols[n], INPUT_PULLUP);
  }  
}

void selectDigit (int d) {
  for (int n = 0; n < 4; n++) {
    if (n == d){
  		pinMode(commons[n], OUTPUT);
		digitalWrite(commons[n], 0);	      
    } else {
  		pinMode(commons[n], INPUT);
    }
  }
}

void selectSegments (int s) {
  s++;
  for (int n = 0; n < 8; n++) {
    if (s & 1){
 		digitalWrite(segments[n], 1);	
    } else {
 		digitalWrite(segments[n], 1);	
    }
    s = s >> 1;
  }
}

int readPad () {
  int res = 0;
  for (int n = 0; n < 4; n++) {
    if (digitalRead(padcols[n]) == 0) {
      res = res | (1 << n);
    }
  }
  return res;
}




void setup() {
	setupDisp();
  	setupPad();
  	Serial.begin(9600);
}

int digit = 0; 

void loop() {
	
 selectDigit(-1);
 selectSegments(255);
 selectDigit(digit);
 int pad = readPad();

  Serial.print(pad);
  Serial.print(" ");
  
  digit++;
  if (digit == 4) {
    Serial.println();
    digit = 0;
  }  
  delay(200);
  
}
