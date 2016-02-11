#!/bin/bash

cd Build

premake4 gmake
make clean
make
