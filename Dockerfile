# First attempt. Don't judge me too harshly

FROM ubuntu

MAINTAINER Derek 

RUN apt-get update; \
    apt-get -y upgrade

# install tools
RUN apt-get -y install g++ cmake git

# install opencv
RUN git clone https://github.com/opencv/opencv.git; cd opencv
RUN mkdir -p opencv/release; cd opencv/release; \
    cmake -D CMAKE_BUILD_TYPE=release ..; \
    make

# create working directory
WORKDIR /app
ADD . /app
RUN cd /app/build; cmake ..; make


CMD ["/app/roboVision"]
