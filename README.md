FreeOSEK (Underconstruction 🛠🔧⛏)
========

This is created to learn OSEK standard practically. And therefore setting a path towards learning AUTOSAR.

Some of this work is derived from Zephyr OS, hence most of the files here would have the same content and license information (Apache 2.0)

Start date: 27 June 2021, Sunday
Motivation to create an OS came from Ashokkumar Narayanan.

<br><br>

About this OS
===

What is OSEK?
-------------
OSEK stands for "Offene Systeme und deren Schnittstellen für die Elektronik in Kraftfahrzeugen", which means "Open Systems and their Interfaces for the Electronics in Motor Vehicles". 

As the name suggests, it an Operating Systems developed for automotive control systems which are driven by stringent real time and tiny embedded electronic control units (a.k.a ECUs). It is primarily a single processor operating systems meant for distributed embedded control units that are interconnected over one or more of the following networks:
 1. CAN
 2. LIN
 3. FlexRay
 4. Ethernet

The OS and its standardised interfaces are described in greater details in OSEK Specifications provided in the 1st link in the Reference section below.

<br>

What is FreeOSEK?
-----------------

FreeOSEK is a intended to provide a free to use with no liability licensed software, writen against OSEK specification version 2.3.3.

<br>

FreeOSEK High Level Architecture
---

<br><br>

Getting Started
===

Prerequisites
----

Building FreeOSEK



Running FreeOSEK on QEMU (ARM926EJ-S)
----

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

<br><br>

References
==========
1. OSEK Specification
   * https://www.irisa.fr/alf/downloads/puaut/TPNXT/images/os223.pdf
2. OSEK Implementation Language (OIL)
   * http://www.irisa.fr/alf/downloads/puaut/TPNXT/images/oil25.pdf
3. QEMU ARM versatilepb Tutorials
   * https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu
   * https://balau82.wordpress.com/2012/04/15/arm926-interrupts-in-qemu/

