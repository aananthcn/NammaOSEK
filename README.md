NammaOSEK (a.k.a FreeOSEK. Underconstruction 🛠🔧⛏)
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

What is NammaOSEK?
------------------

NammaOSEK (aka FreeOSEK) is a intended to provide a free to use with no liability licensed (MIT) software, writen against OSEK specification version 2.3.3. This is created to learn OSEK standard practically. And therefore setting a path towards learning as well as teaching AUTOSAR core skills.

What is Namma? Namma means our in Tamil and Kannada. We used to say "Avan namma friendu" in Tamil. Which means "he is our friend". So, this is (y)our OSEK, free to use with no liability. 

**Start date**: 27 June 2021, Sunday<br>
Motivation to create an OS came from **Ashokkumar Narayanan** (one of my best buddies in Visteon Chennai).

### Supported Boards
 * qemu-versatilepb
 * stm32f407vet6
 * rp2040 (Raspberry Pi Pico)

### Python based GUI
![OSEK Builder](https://github.com/aananthcn/NammaOSEK/blob/main/doc/gui-taskview.png)

<br>

NammaOSEK High Level Architecture
---
The NammaOSEK project uses a python (tkinter) based GUI as system configuration UI which can help developers to edit and save OS configs to OIL file. Then it can invoke Python scripts as code generators to convert the OIL file to C source file. The workflow supported by NammaOSEK is shown in picture below.
![High Level Architecture and Workflow](https://github.com/aananthcn/NammaOSEK/blob/main/doc/workflow.png)

OSEK Features & Status
---
S.No | Feature | Status | Notes |
---  | ---     | ---    | ---   |
1 | Basic and Extended tasks | SUPPORTED | |
2 | Multiple request of task activation | SUPPORTED | |
3 | Number of tasks | More than 16 | *ECC2, limited by CPU power and RAM* |
4 | More than 1 task per priority | SUPPORTED | |
5 | Number of priorities | more than 16 | *ECC2* |
6 | Number of events per task | 64 | *Bit mask for unsigned64_t* |
7 | Separate stack for every task | SUPPORTED | |
8 | Premption of tasks | NOT SUPPORTED | *Will be supported soon* |
9 | TerminateTask, ChainTask, Schedule | SUPPORTED | |
10 | Application Modes | SUPPORTED | *Not yet tested* |
11 | ISR | SUPPORTED | *Board specific code calls ISR configured (handler with IRQn) in ISR tab of OSEK-Builder.xlsx. Only timer ISR works for now!* |
12 | RESOURCES | SUPPORTED | *Supports Priority Ceiling Protocol, but higher priority task cannot get a resource if busy!* |

<br>

Performance Metrics
---
**Board**: qemu-versatilepb
 * **ROM Usage**: 32016 bytes
 * **RAM Usage**: 4104 bytes (*454 bytes {kernel RAM usage} + 3650 bytes {stack + context save areas; this can be reduced based on application.}*)
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
   * `apt install gcc-arm-none-eabi qemu-system-arm gdb-multiarch python3`
<br>

Cloning NammaOSEK
---
* `git clone git@github.com:aananthcn/NammaOSEK.git`
<br>

Board selection
---
* `cd NammaOSEK` (Move to the cloned location)
* Run `./configure` to know the list of boards supported
* `./configure qemu-versatilepb` to select QEMU board
* Always do a `make clean` after selecting the board.
<br>



Building the NammaOSEK image
---
* `cd NammaOSEK` (Move to the cloned location)
* `make` 
* After a successfuld build, you should see following output files created in the current directory
  * osek-os.bin
  * osek-os.elf
  * osek-os.map
<br>


Configuring NammaOSEK & (Re-)Generating OIL files and Source files
---
* Open Windows command prompt and change directory to NammaOSEK (the cloned directory)
* Run `python tools\osek-builder-gui.py` in Windows command prompt.
  * You should see a GUI (tkinter based) opening. Do the following:
    * File -> "Open OIL File" -> select "osek_os-cortex-m4.oil"
    * Naviage through different tabs, make edits
    * File -> "Save As", and save it with same or different file name.
    * Generate -> "Generate Source"
* After the last step above, you should see new source files generated in path ./tools/src
* Now, go to section "**Building the NammaOSEK image**" (above) to rebuild NammaOSEK.
<br>


Running NammaOSEK on QEMU (ARM926EJ-S)
----
To run the image on QEMU, use the following command
* `QEMU_AUDIO_DRV=none qemu-system-arm -M versatilepb -m 128M -nographic -kernel osek-os.bin`

To stop QEMU running in -nographic mode, type the following
* `CTRL+a x`
<br>

Debugging NammaOSEK on QEMU (ARM926EJ-S)
----
To debug using gdb, use the following command in 
* `QEMU_AUDIO_DRV=none qemu-system-arm -M versatilepb -m 128M -nographic `**`-s -S`**` -kernel osek-os.bin`
* Open another terminal and type the following commands
  * `gdb-multiarch`
  * Then inside the gdb console type the following:
    * `target remote localhost:1234`
    * `file osek-os.elf`
    * `b main`
    * `c`
    * *...other commands*

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

