FROM fedora:38

# install dependencies
WORKDIR /opt
RUN dnf update -y && \
    dnf install -y cmake gcc-c++ make openssl-devel zlib-devel boost-devel fmt-devel git && \
    dnf clean all

# install glaze (header-only JSON library)
RUN git clone --depth 1 https://github.com/stephenberry/glaze.git && \
    cd glaze && cmake -B build && cmake --install build && \
    cd .. && rm -rf glaze

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
