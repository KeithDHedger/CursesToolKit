#!/bin/bash

./remakesourcelist
libtoolize --copy
aclocal
autoheader
touch NEWS README AUTHORS ChangeLog
automake --add-missing --copy
autoconf
./configure $@

