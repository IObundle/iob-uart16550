# SPDX-FileCopyrightText: 2025 IObundle
#
# SPDX-License-Identifier: MIT

# SOURCES
VTOP:=iob_uart16550_tb

#tests
TEST_LIST+=test1
test1:
	make run SIMULATOR=$(SIMULATOR)
