# gearbot
A simple bot using a database and some basic commands to keep track of a discord channel topic.

I've only tested this on Linux and MacOS. It should work on Windows but you're on your own.

## Dependencies
- [DPP Discord Bot Library](https://dpp.dev/)
  - libz
  - OpenSSL (libssl/libcrypto)
- MariaDB Connector/C (libmariadb)
## Building
### Build Environment
- CMake 3.16+
- GCC/clang/MSVC supporting C++17
### Build Steps
- Configure with CMake (e.g. `cmake -S ./ -B build/`).
- run `make` or `ninja` as appropriate in build directory.


## Running
Gearbot requires that a config file called "config.txt" be placed in the same directory as the executable. 
The file works as a series of `key = value` pairs with '#' denoting the line is a comment. At a minimum your config should have the following:
```
AUTH_TOKEN = 0Dg.....
MARIADB_HOSTNAME = 127.0.0.1
MARIADB_USERNAME = dbuser
MARIADB_PASSWORD = dbpassword
RESYNC_COMMANDS = false
```
After that, gearbot can be launched with `./gearbot` or you can install it anywhere you'd like.

## License
The bulk of the code is licensed under the LGPL 2.1, with the exception of fmt_table.hpp which is licensed under the MIT license.
