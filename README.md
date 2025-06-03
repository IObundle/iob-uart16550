<!--
SPDX-FileCopyrightText: 2025 IObundle

SPDX-License-Identifier: MIT
-->

## IOb-UART16550 ##

This is an adaptation of the UART16550 at https://opencores.org/projects/uart16550 to the IOb-SoC.

Note: This version has a FIFO with [256 bytes](https://github.com/IObundle/iob-uart16550/blob/master/hardware/src/uart_defines.vh#L231).

Note: This version was modified to use a dedicated hardware based control of RTS/CTS signals (software control of these signals is ignored). Therefore, the software should have hardware flow control disabled to avoid issues.

## How to build the core with Py2HWSW ##
The [Py2HWSW](https://nlnet.nl/project/Py2HWSW/) workflow allows to automatically generate verilog components used by the projects core Verilog. It allows to create bus interfaces with ease and use existing Verilog modules.
To use Py2HWSW the project should have a *core_name*.py file in the root directory.
The main commands to use the Py2HWSW workflow are:
- `make setup`: creates a build directory in the projects parent directory.
- `make clean`: removes the build directory.

An example of cleaning a previous build, creating a new build and simulating the project is:
- `make clean && make setup && make -C ../iob_uart16550_V0.10 sim-run`

## Funding

This core is used as part of the [SoCLinux](https://nlnet.nl/project/SoCLinux) project. This project is funded through [NGI Zero Core](https://nlnet.nl/core), a fund established by [NLnet](https://nlnet.nl) with financial support from the European Commission's [Next Generation Internet](https://ngi.eu) program. Learn more at the [NLnet project page](https://nlnet.nl/project/SoCLinux).

[<img src="https://nlnet.nl/logo/banner.png" alt="NLnet foundation logo" width="20%" />](https://nlnet.nl)
[<img src="https://nlnet.nl/image/logos/NGI0_tag.svg" alt="NGI Zero Logo" width="20%" />](https://nlnet.nl/core)

