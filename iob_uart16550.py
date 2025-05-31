def setup(py_params_dict):
    attributes_dict = {
        "generate_hw": False,
        "version": "0.1",
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
                "descr": "CPU native interface",
                "signals": {
                    "type": "iob",
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
        "subblocks": [
            {
                "core_name": "iob_iob2wishbone",
            },
            {
                "core_name": "iob_prio_enc",
            },
        ],
        "superblocks": [
            # Simulation wrapper
            {
                "core_name": "iob_uart16550_sim",
                "dest_dir": "hardware/simulation/src",
            },
        ],
    }

    return attributes_dict
