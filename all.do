#!/bin/sh

export REDO_JOBS=$(nproc)

redo-ifchange beeworlds
