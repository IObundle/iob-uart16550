# SPDX-FileCopyrightText: 2025 IObundle
#
# SPDX-License-Identifier: MIT


def setup(py_params_dict):
    CSR_IF = py_params_dict["csr_if"] if "csr_if" in py_params_dict else "iob"
    NAME = py_params_dict["name"] if "name" in py_params_dict else "iob_uart16550"

    IF_DISPLAY_NAME = {
        "iob": "IOb",
        "axil": "AXI-Lite",
        "wb": "Wishbone",
    }

    attributes_dict = {
        "name": NAME,
        "generate_hw": True,
        "description": "IObundle's adaptation of the UART16550 from https://opencores.org/projects/uart16550.",
        "version": "0.1",
        "board_list": ["iob_aes_ku040_db_g"],
        #
        # Confs
        #
        "confs": [
            {
                "name": "ADDR_W",
                "descr": "Address bus width",
                "type": "P",
                "val": 5,
            },
            {
                "name": "DATA_W",
                "descr": "Data bus width",
                "type": "P",
                "val": 32,
            },
        ],
        #
        # Ports
        #
        "ports": [
            {
                "name": "clk_en_rst_s",
                "descr": "Clock, clock enable and reset",
                "signals": {
                    "type": "iob_clk",
                },
            },
            {
                "name": "csrs_cbus_s",
                "descr": f"Control and status interface, when selecting the {IF_DISPLAY_NAME[CSR_IF]} CSR interface.",
                "signals": {
                    "type": CSR_IF,
                    "ADDR_W": 5,
                },
            },
            {
                "name": "rs232_m",
                "descr": "RS232 interface",
                "signals": {
                    "type": "rs232",
                },
            },
            {
                "name": "interrupt_o",
                "descr": "UART16550 interrupt related signals",
                "signals": [
                    {
                        "name": "interrupt_o",
                        "width": "1",
                        "descr": "UART interrupt source",
                    },
                ],
            },
        ],
    }
    # Document all supported CSR interfaces
    for supported_if in ["iob", "axil", "wb"]:
        # CSR_IF has already been documented previously. Only document other supported interfaces.
        if CSR_IF != supported_if:
            attributes_dict["ports"].insert(
                2,
                {
                    "name": f"csrs_cbus_{supported_if}_s",
                    "doc_only": True,
                    "descr": f"Control and status interface, when selecting the {IF_DISPLAY_NAME[supported_if]} CSR interface.",
                    "signals": {
                        "type": supported_if,
                        "ADDR_W": 5,
                    },
                },
            )

    #
    # Wires
    #
    attributes_dict |= {
        "wires": [
            {
                "name": "internal_uart_cbus",
                "descr": "UART CSRs bus",
                "signals": {
                    "type": "wb",
                    "prefix": "internal_",
                    "ADDR_W": "ADDR_W",
                },
            },
            {
                "name": "rs232_internal",
                "descr": "RS232 interface",
                "signals": {
                    "type": "rs232",
                    "prefix": "internal_",
                    "N_PINS": 8,
                },
            },
        ],
    }
    #
    # Blocks
    #
    attributes_dict |= {
        "subblocks": [
            {
                "core_name": "iob_universal_converter",
                "instance_name": "universal_converter",
                "instance_description": "Convert CSRs interface into internal wishbone bus",
                "subordinate_if": CSR_IF,
                "manager_if": "wb",
                "parameters": {
                    "ADDR_W": "ADDR_W",
                    "DATA_W": "DATA_W",
                    "READ_BYTES": 1,
                },
                "connect": {
                    "clk_en_rst_s": "clk_en_rst_s",
                    "s_s": "csrs_cbus_s",
                    "m_m": "internal_uart_cbus",
                },
            },
            {
                "core_name": "iob_uart16550_st",
                "instance_name": "serial_transceiver",
                "instance_description": "UART16550 Serial Transceiver",
                "connect": {
                    # "clk_rst_s": "clk_en_rst_s", # Connected automatically?
                    "wb_s": "internal_uart_cbus",
                    "rs232_m": "rs232_internal",
                    "interrupt_o": "interrupt_o",
                },
            },
            {  # Currently used for docs only
                "core_name": "iob_csrs",
                "instantiate": False,
                "instance_name": "csrs",
                "instance_description": "Control/Status Registers",
                "autoaddr": False,
                "csrs": [
                    {
                        "name": "rbr_thr_dll",
                        "addr": 0,
                        "mode": "RW",
                        "n_bits": 8,
                        "rst_val": 0,
                        "log2n_items": 0,
                        "descr": "RBR (Receiver Buffer Register) when read, THR (Transmitter Holding Register) when written. When LCR.DLAB bit is set, this address accesses the Divisor Latch LSB (DLL).",
                    },
                    {
                        "name": "ier_dlm",
                        "addr": 1,
                        "mode": "RW",
                        "n_bits": 8,
                        "rst_val": 0,
                        "log2n_items": 0,
                        "descr": "Interrupt Enable Register. When LCR.DLAB bit is set, this address accesses the Divisor Latch MSB (DLM).",
                    },
                    {
                        "name": "iir_fcr",
                        "addr": 2,
                        "mode": "RW",
                        "n_bits": 8,
                        "rst_val": 0xC1,
                        "log2n_items": 0,
                        "descr": "IIR (Interrupt Identification Register) when read, FCR (FIFO Control Register) when written.",
                    },
                    {
                        "name": "lcr",
                        "addr": 3,
                        "mode": "RW",
                        "n_bits": 8,
                        "rst_val": 3,
                        "log2n_items": 0,
                        "descr": "Line Control Register. The DLAB bit (MSB) controls access to the Divisor Latch registers.",
                    },
                    {
                        "name": "mcr",
                        "addr": 4,
                        "mode": "W",
                        "n_bits": 8,
                        "rst_val": 0,
                        "log2n_items": 0,
                        "descr": "Modem Control Register.",
                    },
                    {
                        "name": "lsr",
                        "addr": 5,
                        "mode": "R",
                        "n_bits": 8,
                        "rst_val": 0x60,
                        "log2n_items": 0,
                        "descr": "Line Status Register.",
                    },
                    {
                        "name": "msr",
                        "addr": 6,
                        "mode": "R",
                        "n_bits": 8,
                        "rst_val": 0,
                        "log2n_items": 0,
                        "descr": "Modem Status Register.",
                    },
                    # {  # Scratch Register not available in OpenCore's uart16550
                    #     "name": "scr",
                    #     "addr": 7,
                    #     "mode": "RW",
                    #     "n_bits": 8,
                    #     "rst_val": 0,
                    #     "log2n_items": 0,
                    #     "descr": "Scratch Register.",
                    # },
                ],
                "csr_if": CSR_IF,
            },
        ],
        "superblocks": [
            # Simulation wrapper
            {
                "core_name": "iob_uart16550_sim",
                "dest_dir": "hardware/simulation/src",
                "csr_if": CSR_IF,
            },
        ],
        "sw_modules": [
            {
                "core_name": "iob_coverage_analyze",
                "instance_name": "iob_coverage_analyze_inst",
            },
        ],
        #
        # Snippets
        #
        "snippets": [
            {
                "verilog_code": """
      // Assign RS232 interface signals
      assign internal_rs232_rxd = rs232_rxd_i;
      assign rs232_txd_o = internal_rs232_txd;
      assign rs232_rts_o = internal_rs232_rts;
      assign internal_rs232_cts = rs232_cts_i;
      // internal_rs232_dtr floating
      assign internal_rs232_dsr = 1'b1;
      assign internal_rs232_ri = 1'b0;
      assign internal_rs232_dcd = 1'b0;
"""
            }
        ],
    }

    return attributes_dict
