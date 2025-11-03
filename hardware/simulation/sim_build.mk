# SPDX-FileCopyrightText: 2025 IObundle
#
# SPDX-License-Identifier: MIT

# SOURCES
VTOP:=iob_uart16550_tb

# annotations path
CUSTOM_COVERAGE_FLAGS=cov_annotated
CUSTOM_COVERAGE_FLAGS+=-E iob_uart16550_tb.v
CUSTOM_COVERAGE_FLAGS+=-E iob_uut.v
CUSTOM_COVERAGE_FLAGS+=-E wb_mast.v
# CUSTOM_COVERAGE_FLAGS+=--waive uart_coverage.waiver
# CUSTOM_COVERAGE_FLAGS+=--waived-tag
CUSTOM_COVERAGE_FLAGS+=-o uart16550_coverage.rpt

#tests
TEST_LIST+=test1
test1:
	make run SIMULATOR=$(SIMULATOR)
