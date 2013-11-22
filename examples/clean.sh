#!/bin/sh
for p in '*.qtcreator.*' '*.user*'
do
    find . -name $p
    find . -name $p |xargs rm 2>/dev/null
done
