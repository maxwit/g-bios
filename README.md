<h1>A light-weight bootloader for IoT beginers</h1>

# Build Steps

```shell
make BOARD_defconfig
make
```
please replace the BOARD_defconfig with a real defconfig name, for example:
```shell
make s3c6410_defconfig
make
```
check build/configs for all supported defconfig files.