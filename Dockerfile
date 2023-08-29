FROM fedora:38

# install dependencies
WORKDIR /opt
RUN dnf update -y && \
    dnf install -y cmake gcc-c++ make openssl-devel zlib-devel libuv-devel git && \
    git clone https://github.com/hoytech/uWebSockets.git && \
    cd uWebSockets && \
    make && \
    make install && \
    cd .. && \
    dnf clean all
# setup repo
WORKDIR /app
COPY . /app
RUN git submodule update --init --recursive && \
    rm -rf build
# build the project
RUN mkdir build && \
    cd build && \
    cmake .. -DBUILD_EXAMPLES=On && \
    make
# run the example
ENTRYPOINT ["sh", "-c", "./build/example$EXAMPLE_NUMBER"]
