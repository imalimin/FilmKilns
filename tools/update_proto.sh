#!/bin/bash

protoc -I=../../src/FkImage/pb/proto --cpp_out=../../src/FkImage/pb --java_out=./fkandroid/src/main/java FkSize.proto FkRational.proto FkIntVec3.proto FkFloatVec3.proto FkImageLayer.proto FkPictureModel.proto