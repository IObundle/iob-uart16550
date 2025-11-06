# SPDX-FileCopyrightText: 2025 IObundle
#
# SPDX-License-Identifier: MIT


def setup(py_params_dict):
    params = {
        # Type of interface for CSR bus
        "csr_if": "iob",
    }

    # Update params with values from py_params_dict
    for param in py_params_dict:
        if param in params:
            params[param] = py_params_dict[param]

    attributes_dict = {
        "name": "iob_uut",
        "generate_hw": True,
    }
    #
    # Confs
    #
    attributes_dict["confs"] = [
        {
            "name": "ADDR_W",
            "descr": "Data bus width",
            "type": "P",
            "val": 6,  # 2x UART16550 registers
        },
        {
            "name": "DATA_W",
            "descr": "Data bus width",
            "type": "P",
            "val": 32,
        },
    ]
    #
    # Ports
    #
    attributes_dict["ports"] = [
        {
            "name": "clk_en_rst_s",
            "descr": "Clock, clock enable and reset",
            "signals": {
                "type": "iob_clk",
            },
        },
        {
            "name": "pbus_s",
            "descr": "Testbench UART16550 sim wrapper csrs interface",
            "signals": {
                "type": "iob",
                "ADDR_W": 6,
            },
        },
    ]
    #
    # Wires
    #
    attributes_dict["wires"] = [
        {
            "name": "split_reset",
            "descr": "Reset signal for iob_split components",
            "signals": [
                {"name": "arst_i"},
            ],
        },
        # UART16550 #0
        {
            "name": "uart0_csrs",
            "descr": "uart16550 #0 CSRs interface",
            "signals": {
                "type": "iob",
                "prefix": "uart16550_0_csrs_",
                "ADDR_W": 5,
            },
        },
        {
            "name": "uart0_interrupt",
            "descr": "UART16550 #0 Interrupt signal",
            "signals": [
                {
                    "name": "uart16550_0_interrupt",
                    "width": "1",
                },
            ],
        },
        {
            "name": "uart0_rs232",
            "descr": "uart16550 #0 RS232 interface",
            "signals": {
                "type": "rs232",
                "prefix": "uart0_",
            },
        },
        # TODO: interrupt, rs232
        # TODO: UART16550 #1
        {
            "name": "uart1_csrs",
            "descr": "uart16550 #1 CSRs interface",
            "signals": {
                "type": "iob",
                "prefix": "uart16550_1_csrs_",
                "ADDR_W": 5,
            },
        },
        {
            "name": "uart1_interrupt",
            "descr": "UART16550 #1 Interrupt signal",
            "signals": [
                {
                    "name": "uart16550_1_interrupt",
                    "width": "1",
                },
            ],
        },
        {
            "name": "uart1_rs232",
            "descr": "uart16550 #1 RS232 interface",
            "signals": {
                "type": "rs232",
                "prefix": "uart1_",
            },
        },
    ]
    #
    # Blocks
    #
    attributes_dict["subblocks"] = [
        {
            "core_name": "iob_uart16550",
            "instance_name": "uart16550_inst0",
            "instance_description": "Unit Under Test (UUT) UART16550 instance 0.",
            "connect": {
                "clk_en_rst_s": "clk_en_rst_s",
                "iob_csrs_cbus_s": "uart0_csrs",
                "rs232_m": "uart0_rs232",
                "interrupt_o": "uart0_interrupt",
            },
        },
        {
            "core_name": "iob_uart16550",
            "instance_name": "uart16550_inst1",
            "instance_description": "Unit Under Test (UUT) UART16550 instance 1.",
            "connect": {
                "clk_en_rst_s": "clk_en_rst_s",
                "iob_csrs_cbus_s": "uart1_csrs",
                "rs232_m": "uart1_rs232",
                "interrupt_o": "uart1_interrupt",
            },
        },
        {
            "core_name": "iob_split",
            "name": "tb_pbus_split",
            "instance_name": "iob_pbus_split",
            "instance_description": "Split between testbench peripherals",
            "connect": {
                "clk_en_rst_s": "clk_en_rst_s",
                "reset_i": "split_reset",
                "input_s": "pbus_s",
                "output_0_m": "uart0_csrs",
                "output_1_m": "uart1_csrs",
            },
            "num_outputs": 2,
            "addr_w": 6,
        },
    ]
    #
    # Snippets
    #
    attributes_dict["snippets"] = [
        {
            "verilog_code": """
        assign uart1_rs232_rxd = uart0_rs232_txd;
        assign uart0_rs232_rxd = uart1_rs232_txd;
        assign uart0_rs232_cts = 1'b1;
        assign uart1_rs232_cts = 1'b1;
            """,
        },
    ]

    return attributes_dict
