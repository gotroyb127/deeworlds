#!/bin/sh

redo-ifchange link obj.list
redo-ifchange $(cat obj.list)

./link -o $3 $(cat obj.list)
