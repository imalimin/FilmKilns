#!/usr/bin/env bash
INPUT=$1
OUTPUT=${INPUT}.gif
PALETTEGEN="${OUTPUT}.png"
WIDTH=$2
FPS=$3

if [ "$INPUT" = "" ]; then
  echo "Need an input file"
  exit 1
fi

if [ "$WIDTH" = "" ]; then
  WIDTH=320
fi

if [ "$FPS" = "" ]; then
  FPS=20
fi

ffmpeg -v warning -i $INPUT  -vf scale=-1:-1:flags=lanczos,palettegen -y $PALETTEGEN
ffmpeg -v warning -i $INPUT -i $PALETTEGEN -r ${FPS} -lavfi "fps=${FPS},scale=${WIDTH}:-1:flags=lanczos [x]; [x][1:v] paletteuse" -y $OUTPUT
rm $PALETTEGEN
