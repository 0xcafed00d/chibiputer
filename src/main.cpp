#include <Arduino.h>
#include "chibi_io.h"

int commons[] = {9, 10, 11, 12};
int segments[] = {4, A1, A5, A3, A2, 3, A0, A4};
int padcols[] = {8, 7, 6, 5, 2};

Chibi::IO io;

void setup() {
	Serial.begin(9600);
	io.init(commons, segments, padcols);
}

int val = 0;

void loop() {
	io.displayHexValue(val++);
	io.update();
	delay(1);
}
