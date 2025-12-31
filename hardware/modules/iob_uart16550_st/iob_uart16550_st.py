# SPDX-FileCopyrightText: 2025 IObundle
#
# SPDX-License-Identifier: MIT


def setup(py_params_dict):
    attributes_dict = {
        "generate_hw": True,
        "confs": [],
        "ports": [
            {
                "name": "clk_rst_s",
                "descr": "Clock and reset",
                "signals": {
                    "type": "iob_clk",
                    "params": "a",
                },
            },
            {
                "name": "wb_s",
                "descr": "CSRs wisbhone subordinate interface",
                "signals": {
                    "type": "wb",
                    "ADDR_W": "`UART_ADDR_WIDTH",
                    "DATA_W": "`UART_DATA_WIDTH",
                },
            },
            {
                "name": "rs232_m",
                "descr": "RS232 interface",
                "signals": {
                    "type": "rs232",
                    "N_PINS": 8,
                },
            },
            {
                "name": "interrupt_o",
                "descr": "UART16550 interrupt related signals",
                "signals": [
                    {
                        "name": "int_o",
                        "width": "1",
                        "descr": "UART interrupt source",
                    },
                ],
            },
            # { # ifdef UART_HAS_BAUDRATE_OUTPUT
            #     "name": "baud_o",
            #     "descr": "UART16550 baud output",
            #     "signals": [
            #         {
            #             "name": "baud_o",
            #             "descr": "UART16550 baud output",
            #             "width": "1",
            #         },
            #     ],
            # },
        ],
        "subblocks": [
            {
                "core_name": "iob_prio_enc",
                "instantiate": False,
            },
        ],
    }

    return attributes_dict
