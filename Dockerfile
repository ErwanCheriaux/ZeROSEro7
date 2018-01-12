# Execute command as root
# Build the Dokerfile : docker build -t dock . 
# Run : docker run -ti dock

#FROM rfc1149/rose-dev
FROM tuetuopay/arm-none-eabi
#FROM tuetuopay/clang

WORKDIR /dock
COPY . .

ENTRYPOINT /bin/bash
