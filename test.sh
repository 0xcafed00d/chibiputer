#!/bin/bash

pio run -t clean -e local && pio run -e local && .pioenvs/local/program

