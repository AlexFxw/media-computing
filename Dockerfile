# This dockerfile is used to install opencv with C++ in ubuntu
FROM alexfxw/opencv-cpp:1.0
RUN apt-get -y install libomp-dev
ENTRYPOINT /bin/bash