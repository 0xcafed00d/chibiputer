#include <Arduino.h>
#include "chibi_io.h"

int commons[] = {9, 10, 11, 12};
int segments[] = {A0, A1, A2, A3, A4, A5, 3, 4};
int padcols[] = {8, 7, 6, 5, 2};

Chibi::IO io;

void setup() {
	Serial.begin(9600);
	io.init(commons, segments, padcols);
}

void loop() {
	io.update();
	delay(50);
}
