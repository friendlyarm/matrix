#!/bin/bash

export PATH=/usr/local/Trolltech/QtEmbedded-4.8.6-arm/bin/:$PATH

[ -e /usr/local/Trolltech/QtEmbedded-4.8.6-arm ] || tar xvzf qte.tgz -C /
qmake -project
qmake -spec linux-g++
make
