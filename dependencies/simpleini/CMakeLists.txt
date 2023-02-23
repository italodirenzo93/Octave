cmake_minimum_required(VERSION 3.3)
project(SIMPLEINI)

add_library(simpleini include/SimpleIni.h include/ConvertUTF.h ConvertUTF.c)
target_include_directories(simpleini PUBLIC include/)
