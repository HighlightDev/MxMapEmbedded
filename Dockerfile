FROM alpine:3.14

RUN apk update

# Install OpenRC
RUN apk add openrc

RUN apk add --no-cache build-base

RUN gcc --version && g++ --version && make --version
RUN which gcc g++ make ld ar

# Install CMake
RUN apk add cmake

# Install CMake extras
RUN apk add extra-cmake-modules

# Verify CMake
RUN cmake --version

RUN apk add clang

RUN apk add gdb

ENTRYPOINT ["/bin/sh"]