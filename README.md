<h1>g-bios: A light-weight bootloader</h1>

# Features
1. Support serial load

# Build

```shell
make <BOARD>_defconfig
make
```

please replace the <BOARD>_defconfig with a real defconfig name, for example:
```shell
make armvirt_defconfig
make
```

run `make help` for all supported defconfig files.
