# Execute command as root
# Build the Dokerfile : docker build -t dock . 
# Run : docker run -ti dock

FROM tuetuopay/arm-none-eabi

WORKDIR /dock
COPY . .

ENTRYPOINT /bin/bash
