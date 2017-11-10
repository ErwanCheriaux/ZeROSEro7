# Guide to install additionnal tools, flash and debug the program on Debian

Contact enguerrand in case of failure.

## Install stlink

- clone stlink repo :

```sh
git clone https://github.com/texane/stlink.git && cd stlink
```
- build stlink :

You need to have the following packages installed :
build-essential
cmake
libusb-1.0

```sh
make release
make debug
```
- install system wide (if you are admin) :

```sh
cd build/Release; sudo make install
cp etc/udev/rules.d/* /etc/udev/rules.d
sudo udevadm control --reload-rules
sudo udevadm trigger
sudo addgroup stlink || sudo groupadd stlink
sudo usermod -a -G stlink $USER
```


- OR install to a directory (in A406) :

```sh
cd build/Release; make install DESTDIR=$HOME/path/to/dir
export PATH=$PATH:$HOME/path/to/dir/usr/local/bin/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/path/to/dir/usr/local/lib/
```
Don't forget to add those exports to your .zshrc to make it permanent. udev is already configured.

- Remove stlink sources
```sh
cd ../../..
rm -rf stlink
```

## Load and Start

- start gdb server in another terminal :
```sh
make startgdbserver
```

- run program on gdb :
```sh
make debug
flash
continue
```
You may need to allow .gdbinit file.
