FROM ubuntu:18.04

ARG RB_VERSION=''

RUN apt-get update && apt-get install -y libsdl2-dev libsdl2-mixer-dev libfreeimage-dev libfreetype6-dev libeigen3-dev libcurl4-openssl-dev libasound2-dev libgl1-mesa-dev build-essential cmake gettext libavcodec-dev libavdevice-dev libavfilter-dev libavformat-dev libavresample-dev libavutil-dev libswresample-dev libswscale-dev libpostproc-dev

WORKDIR /src

ADD . /src

RUN cmake .

RUN (test -x "RB_VERSION" && echo ok || true ) && make -j8

RUN ./emulationstation_test --gtest_output="xml:report.xml"

