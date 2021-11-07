FreeOSEK (Underconstruction 🛠🔧⛏)
========
What is OSEK?
-------------
OSEK stands for "Offene Systeme und deren Schnittstellen für die Elektronik in Kraftfahrzeugen", which means "**Open Systems and their Interfaces for the Electronics in Motor Vehicles**". 

As the name suggests, it an Operating Systems developed for automotive control systems which are driven by stringent real time and tiny embedded electronic control units (a.k.a ECUs). It is primarily a single processor operating systems meant for distributed embedded control units that are interconnected over one or more of the following networks:
 1. CAN
 2. LIN
 3. FlexRay
 4. Ethernet

The OS and its standardised interfaces are described in greater details in OSEK Specifications provided in the 1st link in the Reference section below.

<br>

What is FreeOSEK?
-----------------

FreeOSEK is a intended to provide a free to use with no liability licensed (Apache 2.0/MIT) software, writen against OSEK specification version 2.3.3. This is created to learn OSEK standard practically. And therefore setting a path towards learning as well as teaching AUTOSAR core skills.

Some small percentage of this work is derived from Zephyr OS, mpaland/printf etc., hence most of the files here would have the same content and license information (Apache 2.0)

**Start date**: 27 June 2021, Sunday<br>
Motivation to create an OS came from **Ashokkumar Narayanan** (one of my best buddies in Visteon Chennai).

<br>

FreeOSEK High Level Architecture
---
The FreeOSEK project uses Excel document as system configuration UI tool and Python scripts as code generators. The workflow supported by FreeOSEK is shown in picture below.
![High Level Architecture and Workflow](https://github.com/aananthcn/FreeOSEK/blob/main/doc/workflow.png)

OSEK Features & Status
---
 * **Basic and Extended tasks** - SUPPORTED
 * **Multiple request of task activation** - SUPPORTED
 * **Number of tasks** - more than 16 (ECC2, limited by CPU power and RAM)
 * **More than 1 task per priority** - SUPPORTED
 * **Number of priorities** - more than 16
 * **Number of events per task** - 64
 * **Separate stack for every task** - SUPPORTED
 * **Premption of tasks** - `NOT SUPPORTED` (will be supported soon)
 * **TerminateTask, ChainTask, Schedule** - SUPPORTED
 * **Application Modes** - SUPPORTED (not yet tested)

<br>

Performance Metrics
---
**Board**: qemu-versatilepb
 * **ROM Usage**: 30396 bytes (29.7kB)
 * **RAM Usage**: 4904 bytes (*353 bytes (OS variables) + 4551 bytes (stack + context save areas; this can be reduced based on application.*)
 * **OS Initialization time**: 1.64 ms
 * **Inter-task switching Latency**: 144 µs (*measurment scenario: Task A --> B; last-line of Task-A to first-line of Task-B*)

<br><br>

Getting Started
===

Prerequisites
----
 * Please use either one of the following as your development system
   * Ubuntu 20.04 OS
   * Windows 10 + WSL2 + Ubuntu 20.04 inside WLS2
 * Install following packages inside Ubuntu
   * `apt install gcc-arm-none-eabi qemu-system-arm gdb-multiarch`

<br>

Cloning & Building FreeOSEK
---
* `git clone git@github.com:aananthcn/FreeOSEK.git`
* `cd FreeOSEK`
* `./configure qemu-versatilepb`
* `make`
* Here you should see following output files created in the current directory
  * osek-os.bin
  * osek-os.elf
  * osek-os.map

<br>

Running FreeOSEK on QEMU (ARM926EJ-S)
----
To run the image on QEMU, use the following command
* `QEMU_AUDIO_DRV=none qemu-system-arm -M versatilepb -m 128M -nographic -kernel osek-os.bin`

<br>

Debugging FreeOSEK on QEMU (ARM926EJ-S)
----
To debug using gdb, use the following command in 
* `QEMU_AUDIO_DRV=none qemu-system-arm -M versatilepb -m 128M -nographic `**`-s -S`**` -kernel osek-os.bin`
* Open another terminal and type the following commands
  * `gdb-multiarch`
  * Then inside the gdb console type the following:
    * target remote localhost:1234
    * file osek-os.elf
    * b main
    * c
    * <other commands>

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
4. ARM926EJ-S User Guide(s)
   * https://developer.arm.com/documentation/dui0224/i/
   * https://developer.arm.com/documentation/ddi0287/b/

