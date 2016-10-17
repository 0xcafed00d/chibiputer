#include <Arduino.h>
#include "chibi_io.h"
#include "chibi_core.h"
#include "chibi_monitor.h"

int commons[] = {12, 11, 10, 9};
int segments[] = {4, A1, A5, A3, A2, 3, A0, A4};
int padcols[] = {8, 7, 6, 5, 2};

Chibi::IO io;
Chibi::Core core;
Chibi::Monitor monitor;

void setup() {
	Serial.begin(9600);
	io.init(commons, segments, padcols);
	core.reset(true);
	monitor.init(&io, &core);
}

void loop() {
	io.update();
	monitor.update();
}
