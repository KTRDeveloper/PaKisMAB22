FROM ubuntu:18.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y git gcc g++ make
RUN apt-get install unzip
RUN apt-get install build-essential -y
RUN apt-get install zlib1g-dev -y

ADD kissat /kissat
ADD painless-src /painless-src
ADD Makefile /Makefile
RUN make all
ADD bin /bin

CMD bin/starexec_run_sc2021


