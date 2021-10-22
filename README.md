FreeOSEK (Underconstruction 🛠🔧⛏)
========

This is created to learn OSEK standard practically. And therefore setting a path towards learning AUTOSAR.

Some of this work is derived from Zephyr OS, hence most of the files here would have the same content and license information (Apache 2.0)

Start date: 27 June 2021, Sunday
Motivation to create an OS came from Ashokkumar Narayanan.

References
==========
1. https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/


<br>

Running FreeOSEK on QEMU (ARM926EJ-S)
====

ARM926EJ-S User Guide:
* https://developer.arm.com/documentation/dui0224/i/
* https://developer.arm.com/documentation/ddi0287/b/

<br>

**Follow the steps below to create "osek-os.bin" image**:
* cd to root directory
* ./configure qemu-versatilepb
* source <path-to-arm-toolchain-path.sh>
* Manually set the path `LIB_GCC_A_PATH` correctly (i.e, the path to libgcc.a file) in file "qemu-versatilepb.mk"
* make

To run the image on QEMU, use the following command
* `QEMU_AUDIO_DRV=none qemu-system-arm -M versatilepb -m 128M -nographic -kernel osek-os.bin`

To debug using gdb, use the following command
* `QEMU_AUDIO_DRV=none qemu-system-arm -M versatilepb -m 128M -nographic -s -S -kernel osek-os.bin`
