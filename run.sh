#!/bin/bash

DIR="build"

if cmake -DGLFW_BUILD_DOCS=OFF -S . -B $DIR; then
	printf "\n"
	if make -j$(nproc) -C $DIR; then
		printf "\n"
		./$DIR/GAME
	else
		printf ">> build failed\n"
	fi
else
	printf ">> configure failed\n"
fi
