#!/usr/bin/env python3

import os

from iob_module import iob_module

# Submodules
from iob_utils import iob_utils
from iob_iob2wishbone import iob_iob2wishbone
from iob_wishbone2iob import iob_wishbone2iob


class iob_uart16550(iob_module):
    name = "iob_uart16550"
    version = "V0.10"
    flows = "sim emb"
    setup_dir = os.path.dirname(__file__)

    @classmethod
    def _create_submodules_list(cls):
        """Create submodules list with dependencies of this module"""
        super()._create_submodules_list(
            [
                {"interface": "iob_s_port"},
                {"interface": "iob_s_portmap"},
                {"interface": "iob_wire"},
                {"interface": "clk_en_rst_s_s_portmap"},
                {"interface": "clk_en_rst_s_port"},
                iob_utils,
                iob_iob2wishbone,
                iob_wishbone2iob,
            ]
        )

    @classmethod
    def _setup_confs(cls):
        super()._setup_confs(
            [
                # Macros
                # Parameters
                {
                    "name": "DATA_W",
                    "type": "P",
                    "val": "32",
                    "min": "NA",
                    "max": "NA",
                    "descr": "Data bus width",
                },
                {
                    "name": "ADDR_W",
                    "type": "P",
                    "val": "16",
                    "min": "NA",
                    "max": "NA",
                    "descr": "Address bus width",
                },
                # Used for regs below
                {
                    "name": "UART_DATA_W",
                    "type": "P",
                    "val": "8",
                    "min": "NA",
                    "max": "8",
                    "descr": "",
                },
            ]
        )

    @classmethod
    def _setup_ios(cls):
        cls.ios += [
            {
                "name": "iob_s_port",
                "descr": "CPU native interface",
                "ports": [],
            },  # TODO: Remove 2 lsbs from addr width.
            {
                "name": "general",
                "descr": "GENERAL INTERFACE SIGNALS",
                "ports": [
                    {
                        "name": "clk_i",
                        "type": "I",
                        "n_bits": "1",
                        "descr": "System clock input",
                    },
                    {
                        "name": "arst_i",
                        "type": "I",
                        "n_bits": "1",
                        "descr": "System reset, asynchronous and active high",
                    },
                    {
                        "name": "cke_i",
                        "type": "I",
                        "n_bits": "1",
                        "descr": "System reset, asynchronous and active high",
                    },
                ],
            },
            {
                "name": "rs232",
                "descr": "UART16550 rs232 interface signals.",
                "ports": [
                    # {'name':'interrupt', 'type':'O', 'n_bits':'1', 'descr':'be done'},
                    {
                        "name": "txd_o",
                        "type": "O",
                        "n_bits": "1",
                        "descr": "Serial transmit line",
                    },
                    {
                        "name": "rxd_i",
                        "type": "I",
                        "n_bits": "1",
                        "descr": "Serial receive line",
                    },
                    {
                        "name": "cts_i",
                        "type": "I",
                        "n_bits": "1",
                        "descr": "Clear to send; the destination is ready to receive a transmission sent by the UART",
                    },
                    {
                        "name": "rts_o",
                        "type": "O",
                        "n_bits": "1",
                        "descr": "Ready to send; the UART is ready to receive a transmission from the sender",
                    },
                ],
            },
            {
                "name": "interrupt",
                "descr": "UART16550 interrupt related signals",
                "ports": [
                    {
                        "name": "interrupt_o",
                        "type": "O",
                        "n_bits": "1",
                        "descr": "UART interrupt source",
                    },
                ],
            },
        ]

    @classmethod
    def _setup_regs(cls):
        cls.autoaddr = False
        cls.regs += [
            {
                "name": "uart16550",
                "descr": "UART16550 registers address width.",
                "regs": [
                    {
                        "name": "DUMMY",
                        "type": "W",
                        "n_bits": 8,
                        "rst_val": 0,
                        "addr": 0x8000,
                        "log2n_items": 0,
                        "autoreg": True,
                        "descr": "Dummy register.",
                    },
                ],
            }
        ]

    @classmethod
    def _setup_block_groups(cls):
        cls.block_groups += []
