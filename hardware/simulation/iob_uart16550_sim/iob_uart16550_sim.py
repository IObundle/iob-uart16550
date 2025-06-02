# SPDX-FileCopyrightText: 2025 IObundle
#
# SPDX-License-Identifier: MIT


def setup(py_params_dict):
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
            "val": 5,
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
            "name": "wb_s",
            "descr": "Testbench uart csrs interface",
            "signals": {
                "type": "wb",
                "ADDR_W": "ADDR_W",
            },
        },
        {
            "name": "rs232_m",
            "descr": "UART rs232 signals",
            "signals": {
                "type": "rs232",
            },
        },
        {
            "name": "interrupt_o",
            "descr": "UART16550 interrupt signal",
            "signals": [
                {
                    "name": "int_o",
                    "width": "1",
                    "descr": "UART interrupt source",
                },
            ],
        },
    ]
    #
    # Wires
    #
    attributes_dict["wires"] = [
        {
            "name": "uart_cbus",
            "descr": "UART CSRs bus",
            "signals": {
                "type": "iob",
                "prefix": "internal_",
                "ADDR_W": "ADDR_W",
            },
        },
    ]
    #
    # Blocks
    #
    attributes_dict["subblocks"] = [
        {
            "core_name": "iob_wishbone2iob",
            "instance_name": "wishbone2iob",
            "instance_description": "Wishbone to IOb converter.",
            "parameters": {
                "ADDR_W": "ADDR_W",
                "DATA_W": "DATA_W",
            },
            "connect": {
                "clk_en_rst_s": "clk_en_rst_s",
                "wb_s": "wb_s",
                "iob_m": "uart_cbus",
            },
        },
        {
            "core_name": "iob_uart16550",
            "instance_name": "uart16550_inst",
            "instance_description": "Unit Under Test (UUT) UART16550 instance.",
            "connect": {
                "clk_en_rst_s": "clk_en_rst_s",
                "iob_csrs_cbus_s": "uart_cbus",
                "rs232_m": "rs232_m",
                "interrupt_o": "interrupt_o",
            },
        },
    ]

    return attributes_dict
