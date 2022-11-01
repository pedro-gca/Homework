syntax=docker/dockerfile:1
FROM ubuntu:18.04
RUN apt-get update
RUN apt-get -y install git g++ python3 python3-pip cmake make patch unrar
RUN apt-get -y install python3-dev pkg-config autoconf cvs bzr
RUN python3 -m pip install distro requests
RUN git clone https://gitlab.com/nsnam/bake
RUN export BAKE_HOME=`pwd`/bake
RUN export PATH=$PATH:$BAKE_HOME
RUN export PYTHONPATH=$PYTHONPATH:$BAKE_HOME
COPY / .
RUN python3 /bake/bake.py configure -e "ns-3.34"
RUN python3 /bake/bake.py deploy -vvv