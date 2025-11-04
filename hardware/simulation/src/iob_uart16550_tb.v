// SPDX-FileCopyrightText: 2025 IObundle
//
// SPDX-License-Identifier: MIT

`timescale 1ns / 1ps

`include "uart_defines.vh"

module iob_uart16550_tb;

   reg                            clkr;
   reg                            cker;
   reg                            wb_rst_ir;
   wire    [`UART_ADDR_WIDTH-1:0] wb_adr_i;
   wire    [                31:0] wb_dat_i;
   wire    [                31:0] wb_dat_o;
   wire                           wb_we_i;
   wire                           wb_stb_i;
   wire                           wb_cyc_i;
   wire                           wb_ack_o;
   wire    [                 3:0] wb_sel_i;
   wire                           int_o;
   wire                           pad_stx_o;
   wire                           rts_o;
   reg                            pad_srx_ir;

   // All the signals and regs named with a 1 are receiver fifo signals
   wire    [`UART_ADDR_WIDTH-1:0] wb1_adr_i;
   wire    [                31:0] wb1_dat_i;
   wire    [                31:0] wb1_dat_o;
   wire                           wb1_we_i;
   wire                           wb1_stb_i;
   wire                           wb1_cyc_i;
   wire                           wb1_ack_o;
   wire    [                 3:0] wb1_sel_i;
   wire                           int1_o;
   wire                           stx1_o;
   wire                           rts1_o;
   reg                            srx1_ir;

   wire                           clk = clkr;
   wire                           wb_rst_i = wb_rst_ir;
   wire                           pad_srx_i = pad_srx_ir;
   wire                           cts_i = 1;  //cts_ir;

   wire                           srx1_i = srx1_ir;
   wire                           cts1_i = 1;  //cts1_ir;

   reg     [                31:0] dat_o;

   integer                        e;
   integer                        fd;
   integer                        failed = 0;

   localparam BYTE_1 = 8'b10000001;
   localparam BYTE_2 = 8'b01000010;

   iob_uut uart_snd (
      .clk_i (clk),
      .cke_i (cker),
      .arst_i(wb_rst_i),

      // Wishbone signals
      .wb_dat_o   (wb_dat_o),
      .wb_datout_i(wb_dat_i),
      .wb_ack_o   (wb_ack_o),
      .wb_adr_i   (wb_adr_i),
      .wb_cyc_i   (wb_cyc_i),
      .wb_sel_i   (wb_sel_i),
      .wb_stb_i   (wb_stb_i),
      .wb_we_i    (wb_we_i),

      // interrupt request
      .int_o(int_o),

      // UART signals
      // serial input/output
      .rs232_txd_o(pad_stx_o),
      .rs232_rxd_i(pad_srx_i),

      // modem signals
      .rs232_rts_o(rts_o),
      .rs232_cts_i(cts_i)
   );

   iob_uut uart_rcv (
      .clk_i (clk),
      .cke_i (cker),
      .arst_i(wb_rst_i),

      // Wishbone signals
      .wb_dat_o   (wb1_dat_o),
      .wb_datout_i(wb1_dat_i),
      .wb_ack_o   (wb1_ack_o),
      .wb_adr_i   (wb1_adr_i),
      .wb_cyc_i   (wb1_cyc_i),
      .wb_sel_i   (wb1_sel_i),
      .wb_stb_i   (wb1_stb_i),
      .wb_we_i    (wb1_we_i),
      // interrupt request
      .int_o      (int1_o),

      // UART signals
      // serial input/output
      .rs232_txd_o(stx1_o),
      .rs232_rxd_i(srx1_i),

      // modem signals
      .rs232_rts_o(rts1_o),
      .rs232_cts_i(cts1_i)
   );

   /////////// CONNECT THE UARTS
   always @(pad_stx_o) begin
      srx1_ir = pad_stx_o;
   end

   always @(stx1_o) begin
      pad_srx_ir = stx1_o;
   end

`ifdef VCD
   initial begin
      $dumpfile("uut.vcd");
      $dumpvars();
   end
`endif

   initial begin
      clkr = 0;
      // #50000 $display("Teste failed due to timeout!");
      // $finish();
   end

   initial begin
      $display("Data bus is %0d-bit. UART uses %0d-bit addr.", `UART_DATA_WIDTH, `UART_ADDR_WIDTH);
   end

   always begin
      #5 clkr = ~clk;
   end

   wire [31:0] aux_wb_adr_i;
   assign wb_adr_i = aux_wb_adr_i[4:0];
   wb_mast wbm (  // Outputs
      .adr (aux_wb_adr_i),
      .dout(wb_dat_i),
      .cyc (wb_cyc_i),
      .stb (wb_stb_i),
      .sel (wb_sel_i),
      .we  (wb_we_i),
      // Inputs
      .clk (clk),
      .rst (wb_rst_i),
      .din (wb_dat_o),
      .ack (wb_ack_o),
      .err (1'b0),
      .rty (1'b0)
   );

   wire [31:0] aux_wb1_adr_i;
   assign wb1_adr_i = aux_wb1_adr_i[4:0];
   wb_mast wbm1 (  // Outputs
      .adr (aux_wb1_adr_i),
      .dout(wb1_dat_i),
      .cyc (wb1_cyc_i),
      .stb (wb1_stb_i),
      .sel (wb1_sel_i),
      .we  (wb1_we_i),
      // Inputs
      .clk (clk),
      .rst (wb_rst_i),
      .din (wb1_dat_o),
      .ack (wb1_ack_o),
      .err (1'b0),
      .rty (1'b0)
   );

   // The test sequence
   initial begin
      cker = 1'b1;
      #1 wb_rst_ir = 1;
      cker = 1'b0;
      #10 wb_rst_ir = 0;
      #10 cker = 1'b1;

      //write to lcr. set bit 7
      //wb_cyc_ir = 1;
      wbm.wb_wr1(`UART_REG_LC, 4'b1000, {8'b10011011, 24'b0});
      // set dl to divide by 3
      wbm.wb_wr1(`UART_REG_DL1, 4'b0001, 32'd2);
      @(posedge clk);
      @(posedge clk);
      // restore normal registers
      wbm.wb_wr1(`UART_REG_LC, 4'b1000, {8'b00011011, 24'b0});  //00011011 

      fork
         begin
            $display("%m : %t : sending : %h", $time(), BYTE_1);
            wbm.wb_wr1(0, 4'h1, BYTE_1);
            @(posedge clk);
            @(posedge clk);
            $display("%m : %t : sending : %h", $time(), BYTE_2);
            wbm.wb_wr1(0, 4'h1, BYTE_2);
            wait (uart_snd.uart16550_inst.uart16550.regs.tstate == 0 && uart_snd.uart16550_inst.uart16550.regs.transmitter.tf_count == 0);
         end
      join

      // Exercise all Write registers
      // Transmitter Holding Register
      wbm.wb_wr1(`UART_REG_TR, 4'b0001, 32'hFF);
      wbm.wb_wr1(`UART_REG_TR, 4'b0001, 32'h0);
      // Interrupt Enable
      wbm.wb_wr1(`UART_REG_IE, 4'b0010, 32'hFF00);
      wbm.wb_wr1(`UART_REG_IE, 4'b0010, 32'h0);
      // FIFO Control
      wbm.wb_wr1(`UART_REG_FC, 4'b0100, 32'hFF0000);
      wbm.wb_wr1(`UART_REG_FC, 4'b0100, 32'h0);
      wbm.wb_wr1(`UART_REG_FC, 4'b0100, {8'b0, 8'b11000000, 16'b00}); // default value
      // Line Control
      wbm.wb_wr1(`UART_REG_LC, 4'b1000, 32'hFF000000);
      wbm.wb_wr1(`UART_REG_LC, 4'b1000, {8'b11, 24'b00}); // default value
      // Modem Control
      wbm.wb_wr1(`UART_REG_MC, 4'b0001, 32'hFF);
      wbm.wb_wr1(`UART_REG_MC, 4'b0001, 32'b11000000); // default value

      // reset core
      #1 wb_rst_ir = 1;
      #10 wb_rst_ir = 0;

   end

   // receiver side
   initial begin
      #11;
      //write to lcr. set bit 7
      //wb_cyc_ir = 1;
      wbm1.wb_wr1(`UART_REG_LC, 4'b1000, {8'b10011011, 24'b0});
      // set dl to divide by 3
      wbm1.wb_wr1(`UART_REG_DL1, 4'h1, 32'd2);
      @(posedge clk);
      @(posedge clk);
      // restore normal registers
      wbm1.wb_wr1(`UART_REG_LC, 4'b1000, {8'b00011011, 24'b0});
      wbm1.wb_wr1(`UART_REG_IE, 4'b0010, {16'b0, 8'b00001111, 8'b0});
      wait (uart_rcv.uart16550_inst.uart16550.regs.receiver.rf_count == 2);
      wbm1.wb_rd1(0, 4'h1, dat_o);
      $display("%m : %t : Data out: %h", $time(), dat_o[7:0]);
      if (dat_o != BYTE_1) failed = failed + 1;
      @(posedge clk);
      wbm1.wb_rd1(0, 4'h1, dat_o);
      $display("%m : %t : Data out: %h", $time(), dat_o[7:0]);
      if (dat_o != BYTE_2) failed = failed + 1;

      // Exercise Write registers
      // Transmitter Holding Register
      wbm.wb_wr1(`UART_REG_TR, 4'b0001, 32'hFF);
      wbm.wb_wr1(`UART_REG_TR, 4'b0001, 32'h0);
      // Interrupt Enable
      wbm.wb_wr1(`UART_REG_IE, 4'b0010, 32'hFF00);
      wbm.wb_wr1(`UART_REG_IE, 4'b0010, 32'h0);
      // FIFO Control
      wbm.wb_wr1(`UART_REG_FC, 4'b0100, 32'hFF0000);
      wbm.wb_wr1(`UART_REG_FC, 4'b0100, 32'h0);
      wbm.wb_wr1(`UART_REG_FC, 4'b0100, {8'b0, 8'b11000000, 16'b00}); // default value
      // Line Control
      wbm.wb_wr1(`UART_REG_LC, 4'b1000, 32'hFF000000);
      wbm.wb_wr1(`UART_REG_LC, 4'b1000, {8'b11, 24'b00}); // default value
      // Modem Control
      wbm.wb_wr1(`UART_REG_MC, 4'b0001, 32'hFF);
      wbm.wb_wr1(`UART_REG_MC, 4'b0001, 32'b11000000); // default value

      $display("%m : Finish");
      fd = $fopen("test.log", "w");
      if (!failed) begin
         $display("All tests passed!");
         $fdisplay(fd, "Test passed!");
      end else begin
         $display("Failed tests: %d", failed);
         $fdisplay(fd, "Test failed!");
      end
      $fclose(fd);
      $finish();
   end

endmodule
