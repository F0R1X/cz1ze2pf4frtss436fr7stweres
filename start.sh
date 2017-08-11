#!/bin/bash

./chenzhouphz -f ./conf/chenzhouphz.conf -D
./chenzhou3phz -f ./conf/chenzhou3phz.conf -D
./guiyangzpphz -f ./conf/guiyangphz.conf -D
./yzstart.sh
./yxstart.sh
./mtstart.sh
./lwstart.sh
./jhstart.sh