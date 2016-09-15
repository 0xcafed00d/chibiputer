#include <Arduino.h>

int commons[] = {9, 10, 11, 12};
int segments[] = {A0, A1, A2, A3, A4, A5, 3, 4};
int padcols[] = {8, 7, 6, 5, 2};

void setupIO() {
	// set all common select lines to high inpedence inputs
	for (int n = 0; n < 4; n++) {
		pinMode(commons[n], INPUT);
	}

	// set all segment lines to lo output
	for (int n = 0; n < 8; n++) {
		pinMode(segments[n], OUTPUT);
		digitalWrite(segments[n], 0);
	}

	for (int n = 0; n < 5; n++) {
		pinMode(padcols[n], INPUT_PULLUP);
	}
}

void selectDigit(int d) {
	for (int n = 0; n < 4; n++) {
		if (n == d) {
			pinMode(commons[n], OUTPUT);
			digitalWrite(commons[n], 0);
		} else {
			pinMode(commons[n], INPUT);
		}
	}
}

void selectSegments(int s) {
	s++;
	for (int n = 0; n < 8; n++) {
		if (s & 1) {
			digitalWrite(segments[n], 1);
		} else {
			digitalWrite(segments[n], 1);
		}
		s = s >> 1;
	}
}

int readPad() {
	int res = 0;
	for (int n = 0; n < 5; n++) {
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

	// Serial.print(pad);
	// Serial.print(" ");

	digit++;
	if (digit == 4) {
		// Serial.println();
		digit = 0;
	}
	delay(200);
}
