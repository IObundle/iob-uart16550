# SPDX-FileCopyrightText: 2025 IObundle
#
# SPDX-License-Identifier: MIT


def setup(py_params_dict):
    CSR_IF = py_params_dict["csr_if"] if "csr_if" in py_params_dict else "iob"
    NAME = py_params_dict["name"] if "name" in py_params_dict else "iob_uart16550"

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
                "name": "iob_csrs_cbus_s",
                "descr": "Control and Status Registers interface",
                "signals": {
                    "type": CSR_IF,
                    "ADDR_W": "ADDR_W",
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
        #
        # Wires
        #
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
        ],
    }
    #
    # Blocks
    #
    attributes_dict |= {
        "subblocks": [
            {
                "core_name": "iob_universal_converter",
                "instance_name": "iob_universal_converter",
                "instance_description": "Convert CSRs interface into internal IOb port",
                "subordinate_if": CSR_IF,
                "manager_if": "wb",
                "parameters": {
                    "ADDR_W": "ADDR_W",
                    "DATA_W": "DATA_W",
                    "READ_BYTES": 1,
                },
                "connect": {
                    "clk_en_rst_s": "clk_en_rst_s",
                    "s_s": "iob_csrs_cbus_s",
                    "m_m": "internal_uart_cbus",
                },
            },
            {  # Priority encoder for uart_top.v
                "core_name": "iob_prio_enc",
                "instantiate": False,
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
   uart_top uart16550 (
      .wb_clk_i (clk_i),
      // WISHBONE interface
      .wb_rst_i (arst_i),

      .wb_dat_o (internal_wb_dat),
      .wb_dat_i (internal_wb_datout),
      .wb_ack_o (internal_wb_ack),
      .wb_adr_i (internal_wb_adr),
      .wb_cyc_i (internal_wb_cyc),
      .wb_sel_i (internal_wb_sel),
      .wb_stb_i (internal_wb_stb),
      .wb_we_i  (internal_wb_we),
      .int_o    (interrupt_o),
`ifdef UART_HAS_BAUDRATE_OUTPUT
      .baud1_o  (),
`endif
      // UART signals
      .srx_pad_i(rs232_rxd_i),
      .stx_pad_o(rs232_txd_o),
      .rts_pad_o(rs232_rts_o),
      .cts_pad_i(rs232_cts_i),
      .dtr_pad_o(),
      .dsr_pad_i(1'b1),
      .ri_pad_i (1'b0),
      .dcd_pad_i(1'b0)
   );
"""
            }
        ],
    }

    return attributes_dict
