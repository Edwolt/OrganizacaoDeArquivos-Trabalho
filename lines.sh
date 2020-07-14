#!/bin/sh

echo "Source:";
wc -l src/*.c src/opcoes/*.c;
echo;

echo "Header:";
wc -l src/*.h src/opcoes/*.h;
echo;

echo "All:"
cat src/* src/opcoes/* | wc -l;
echo;