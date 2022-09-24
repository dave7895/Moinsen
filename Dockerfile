# We chose Alpine to build the image because it has good support for creating
# statically-linked, small programs.
FROM alpine:3.16 AS base

# Create separate targets for each phase, this allows us to cache intermediate
# stages when using Google Cloud Build, and makes the final deployment stage
# small as it contains only what is needed.
FROM base AS devtools

# Install the typical development tools for C++, and
# the base OS headers and libraries.
RUN apk update && \
    apk add \
        build-base \
        cmake \
        curl \
        git \
        gcc \
        g++ \
        libc-dev \
        linux-headers \
        ninja \
        pkgconfig \
        tar \
        unzip \
        zip \
        neofetch

# Use `vcpkg`, a package manager for C++, to install 
WORKDIR /usr/local/vcpkg
ENV VCPKG_FORCE_SYSTEM_BINARIES=1
RUN curl -sSL "https://github.com/Microsoft/vcpkg/archive/2022.05.10.tar.gz" | \
    tar --strip-components=1 -zxf - \
    && ./bootstrap-vcpkg.sh -disableMetrics
RUN ./vcpkg install functions-framework-cpp
    # Copy the source code to /v/source and compile it.
FROM devtools AS build
COPY . /v/source
WORKDIR /v/source
RUN du -d 2 -h
# make the libchess static library
#RUN rm -rf /v/source/**/build/
RUN ls -lh
RUN ls -lh libchess
RUN git clone https://github.com/kz04px/libchess
RUN cmake -S/v/source/libchess -B/v/source/libchess/build -GNinja -DCMAKE_BUILD_TYPE=Release
RUN cmake --build /v/source/libchess/build --target libchess-static
RUN mv /v/source/docker/* /v/source/
# Run the CMake configuration step, setting the options to create
# a statically linked C++ program
RUN cat /v/source/CMakeLists.txt
RUN cmake -S/v/source -B/v/binary -GNinja \
    -DCMAKE_TOOLCHAIN_FILE=/usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE=Release
# Compile the binary and strip it to reduce its size.
RUN cmake --build /v/binary
RUN strip /v/binary/inferEx
RUN ls -lh /v/binary/inferEx
# Create the final deployment image, using `scratch` (the empty Docker image)
# as the starting point. Effectively we create an image that only contains
# our program.
FROM scratch AS cloud-run-hello
WORKDIR /r

# Copy the program from the previously created stage and make it the entry point.
COPY --from=build /v/binary/inferEx /r

ENTRYPOINT [ "/r/inferEx" ]
