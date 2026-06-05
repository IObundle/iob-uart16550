<!--
SPDX-FileCopyrightText: 2025 IObundle

SPDX-License-Identifier: MIT
-->

## IOb-UART16550 ##

This is an adaptation of the UART16550 at https://opencores.org/projects/uart16550 to the IOb-SoC.

Note: This version has a FIFO with [256 bytes](https://github.com/IObundle/iob-uart16550/blob/master/hardware/src/uart_defines.vh#L231).

## How to build the core with Py2HWSW ##
The [Py2HWSW](https://nlnet.nl/project/Py2HWSW/) workflow allows to automatically generate verilog components used by the projects core Verilog. It allows to create bus interfaces with ease and use existing Verilog modules.
To use Py2HWSW the project should have a *core_name*.py file in the root directory.
The main commands to use the Py2HWSW workflow are:
- `make setup`: creates a build directory in the projects parent directory.
- `make clean`: removes the build directory.

An example of cleaning a previous build, creating a new build and simulating the project is:
- `make clean && make setup && make -C ../iob_uart16550_V0.10 sim-run`

## FuseSoC

A [FuseSoC](https://github.com/olofk/fusesoc)-compatible pre-built version of IOb-UART16550 is available in the official [FuseSoC Package Directory](https://cores.fusesoc.net/cores/?search=iob_uart16550).

There is also a [FuseSoC](https://github.com/olofk/fusesoc)-compatible pre-built version of IOb-uart16550 available in the [repository's release page](https://github.com/IObundle/iob-uart16550/releases) and in the [iob-uart16550-fs](https://github.com/IObundle/iob-uart16550-fs) repository.
The core's Verilog sources are available in the `iob_uart16550/hardware/src/` directory of the compressed tar.gz file in the release page.

To use this pre-built core in FuseSoC, extract the compressed tar.gz file to a [FuseSoC library directory](https://fusesoc.readthedocs.io/en/stable/user/overview.html#discover-cores-the-package-manager).

### Signature verification

IObundle core's published for FuseSoC are cryptographically signed by IObundle.

The core's signature can be verified by adding the contents of [IObundle's trustfile](https://github.com/IObundle/py2hwsw/blob/main/py2hwsw/fusesoc_trustfile.pub) to your local fusesoc installation's trustfile.
More information about FuseSoC core signing and verification can be found in the [FuseSoC documentation](https://fusesoc.readthedocs.io/en/stable/user/build_system/core_files.html#signed-core-files).

## Funding

This core is used as part of the [SoCLinux](https://nlnet.nl/project/SoCLinux) project. This project is funded through [NGI Zero Core](https://nlnet.nl/core), a fund established by [NLnet](https://nlnet.nl) with financial support from the European Commission's [Next Generation Internet](https://ngi.eu) program. Learn more at the [NLnet project page](https://nlnet.nl/project/SoCLinux).

[<img src="https://nlnet.nl/logo/banner.png" alt="NLnet foundation logo" width="20%" />](https://nlnet.nl)
[<img src="https://nlnet.nl/image/logos/NGI0_tag.svg" alt="NGI Zero Logo" width="20%" />](https://nlnet.nl/core)

