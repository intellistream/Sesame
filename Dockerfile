FROM gcc:11 AS build

WORKDIR /Sesame

COPY . .

RUN apt update && apt install -y curl cmake && \
    rm -rf build && mkdir -p build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && make benchmark -j8

FROM ubuntu/mysql

ENV MYSQL_ROOT_PASSWORD=Sesame@ICDM2024
ENV MYSQL_DATABASE=test
ENV meta_cmd="mysql --password=${MYSQL_ROOT_PASSWORD} test"
ENV HOME=/
ENV PATH=$PATH:${HOME}/Sesame/build/benchmark
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/Sesame/build
ENV meta="{bench.id=2024 bench.meta.host=localhost bench.meta.port=3306 bench.meta.db-name=test bench.meta.user=root bench.meta.pass=Sesame@ICDM2024 }mark-time bench.begin"
# create database test
RUN apt update && apt install -y libgomp1 python3 curl git python3-pip && \
    pip3 install pandas matplotlib click numpy && \
    cd /usr/local/bin && curl --proto '=https' --tlsv1.2 -sSf https://raw.githubusercontent.com/innerr/ticat/main/install.sh | /bin/bash && \
    ticat hub.add wzru/sesame.ticat && \
    mkdir -p ${HOME}/Sesame/build ${HOME}/Sesame/benchmark

COPY --from=build /Sesame/build/ ${HOME}/Sesame/build
COPY datasets ${HOME}/Sesame/datasets
COPY scripts ${HOME}/Sesame/scripts
