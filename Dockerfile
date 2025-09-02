FROM ubuntu:22.04

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y build-essential cmake pkg-config libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
RUN apt-get install -y libx11-6 mesa-utils libgl1-mesa-dev libglu1-mesa-dev
RUN rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN make

CMD ["./run"]
