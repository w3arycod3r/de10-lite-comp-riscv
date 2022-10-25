// (C) 2001-2022 Intel Corporation. All rights reserved.
// Your use of Intel Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files from any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Intel Program License Subscription 
// Agreement, Intel FPGA IP License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Intel and sold by 
// Intel or its authorized distributors.  Please refer to the applicable 
// agreement for further details.



// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// $Id: //acds/rel/21.1std/ip/merlin/altera_merlin_router/altera_merlin_router.sv.terp#1 $
// $Revision: #1 $
// $Date: 2020/06/22 $
// $Author: psgswbuild $

// -------------------------------------------------------
// Merlin Router
//
// Asserts the appropriate one-hot encoded channel based on 
// either (a) the address or (b) the dest id. The DECODER_TYPE
// parameter controls this behaviour. 0 means address decoder,
// 1 means dest id decoder.
//
// In the case of (a), it also sets the destination id.
// -------------------------------------------------------

`timescale 1 ns / 1 ns

module Computer_System_mm_interconnect_0_router_default_decode
  #(
     parameter DEFAULT_CHANNEL = 19,
               DEFAULT_WR_CHANNEL = -1,
               DEFAULT_RD_CHANNEL = -1,
               DEFAULT_DESTID = 15 
   )
  (output [99 - 95 : 0] default_destination_id,
   output [23-1 : 0] default_wr_channel,
   output [23-1 : 0] default_rd_channel,
   output [23-1 : 0] default_src_channel
  );

  assign default_destination_id = 
    DEFAULT_DESTID[99 - 95 : 0];

  generate
    if (DEFAULT_CHANNEL == -1) begin : no_default_channel_assignment
      assign default_src_channel = '0;
    end
    else begin : default_channel_assignment
      assign default_src_channel = 23'b1 << DEFAULT_CHANNEL;
    end
  endgenerate

  generate
    if (DEFAULT_RD_CHANNEL == -1) begin : no_default_rw_channel_assignment
      assign default_wr_channel = '0;
      assign default_rd_channel = '0;
    end
    else begin : default_rw_channel_assignment
      assign default_wr_channel = 23'b1 << DEFAULT_WR_CHANNEL;
      assign default_rd_channel = 23'b1 << DEFAULT_RD_CHANNEL;
    end
  endgenerate

endmodule


module Computer_System_mm_interconnect_0_router
(
    // -------------------
    // Clock & Reset
    // -------------------
    input clk,
    input reset,

    // -------------------
    // Command Sink (Input)
    // -------------------
    input                       sink_valid,
    input  [113-1 : 0]    sink_data,
    input                       sink_startofpacket,
    input                       sink_endofpacket,
    output                      sink_ready,

    // -------------------
    // Command Source (Output)
    // -------------------
    output                          src_valid,
    output reg [113-1    : 0] src_data,
    output reg [23-1 : 0] src_channel,
    output                          src_startofpacket,
    output                          src_endofpacket,
    input                           src_ready
);

    // -------------------------------------------------------
    // Local parameters and variables
    // -------------------------------------------------------
    localparam PKT_ADDR_H = 67;
    localparam PKT_ADDR_L = 36;
    localparam PKT_DEST_ID_H = 99;
    localparam PKT_DEST_ID_L = 95;
    localparam PKT_PROTECTION_H = 103;
    localparam PKT_PROTECTION_L = 101;
    localparam ST_DATA_W = 113;
    localparam ST_CHANNEL_W = 23;
    localparam DECODER_TYPE = 0;

    localparam PKT_TRANS_WRITE = 70;
    localparam PKT_TRANS_READ  = 71;

    localparam PKT_ADDR_W = PKT_ADDR_H-PKT_ADDR_L + 1;
    localparam PKT_DEST_ID_W = PKT_DEST_ID_H-PKT_DEST_ID_L + 1;



    // -------------------------------------------------------
    // Figure out the number of bits to mask off for each slave span
    // during address decoding
    // -------------------------------------------------------
    localparam PAD0 = log2ceil(64'h4000000 - 64'h0); 
    localparam PAD1 = log2ceil(64'h8010000 - 64'h8000000); 
    localparam PAD2 = log2ceil(64'h9002000 - 64'h9000000); 
    localparam PAD3 = log2ceil(64'hff200010 - 64'hff200000); 
    localparam PAD4 = log2ceil(64'hff200030 - 64'hff200020); 
    localparam PAD5 = log2ceil(64'hff200040 - 64'hff200030); 
    localparam PAD6 = log2ceil(64'hff200050 - 64'hff200040); 
    localparam PAD7 = log2ceil(64'hff200060 - 64'hff200050); 
    localparam PAD8 = log2ceil(64'hff200070 - 64'hff200060); 
    localparam PAD9 = log2ceil(64'hff200080 - 64'hff200070); 
    localparam PAD10 = log2ceil(64'hff200110 - 64'hff200100); 
    localparam PAD11 = log2ceil(64'hff200120 - 64'hff200110); 
    localparam PAD12 = log2ceil(64'hff201008 - 64'hff201000); 
    localparam PAD13 = log2ceil(64'hff202020 - 64'hff202000); 
    localparam PAD14 = log2ceil(64'hff202040 - 64'hff202020); 
    localparam PAD15 = log2ceil(64'hff202048 - 64'hff202040); 
    localparam PAD16 = log2ceil(64'hff203014 - 64'hff203010); 
    localparam PAD17 = log2ceil(64'hff203030 - 64'hff203020); 
    localparam PAD18 = log2ceil(64'hff203040 - 64'hff203030); 
    localparam PAD19 = log2ceil(64'hff204020 - 64'hff204000); 
    localparam PAD20 = log2ceil(64'hff204022 - 64'hff204020); 
    localparam PAD21 = log2ceil(64'hff204080 - 64'hff204040); 
    // -------------------------------------------------------
    // Work out which address bits are significant based on the
    // address range of the slaves. If the required width is too
    // large or too small, we use the address field width instead.
    // -------------------------------------------------------
    localparam ADDR_RANGE = 64'hff204080;
    localparam RANGE_ADDR_WIDTH = log2ceil(ADDR_RANGE);
    localparam OPTIMIZED_ADDR_H = (RANGE_ADDR_WIDTH > PKT_ADDR_W) ||
                                  (RANGE_ADDR_WIDTH == 0) ?
                                        PKT_ADDR_H :
                                        PKT_ADDR_L + RANGE_ADDR_WIDTH - 1;

    localparam RG = RANGE_ADDR_WIDTH-1;
    localparam REAL_ADDRESS_RANGE = OPTIMIZED_ADDR_H - PKT_ADDR_L;

      reg [PKT_ADDR_W-1 : 0] address;
      always @* begin
        address = {PKT_ADDR_W{1'b0}};
        address [REAL_ADDRESS_RANGE:0] = sink_data[OPTIMIZED_ADDR_H : PKT_ADDR_L];
      end   

    // -------------------------------------------------------
    // Pass almost everything through, untouched
    // -------------------------------------------------------
    assign sink_ready        = src_ready;
    assign src_valid         = sink_valid;
    assign src_startofpacket = sink_startofpacket;
    assign src_endofpacket   = sink_endofpacket;
    wire [PKT_DEST_ID_W-1:0] default_destid;
    wire [23-1 : 0] default_src_channel;




    // -------------------------------------------------------
    // Write and read transaction signals
    // -------------------------------------------------------
    wire read_transaction;
    assign read_transaction  = sink_data[PKT_TRANS_READ];


    Computer_System_mm_interconnect_0_router_default_decode the_default_decode(
      .default_destination_id (default_destid),
      .default_wr_channel   (),
      .default_rd_channel   (),
      .default_src_channel  (default_src_channel)
    );

    always @* begin
        src_data    = sink_data;
        src_channel = default_src_channel;
        src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = default_destid;

        // --------------------------------------------------
        // Address Decoder
        // Sets the channel and destination ID based on the address
        // --------------------------------------------------

    // ( 0x0 .. 0x4000000 )
    if ( {address[RG:PAD0],{PAD0{1'b0}}} == 32'h0   ) begin
            src_channel = 23'b0010000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 15;
    end

    // ( 0x8000000 .. 0x8010000 )
    if ( {address[RG:PAD1],{PAD1{1'b0}}} == 32'h8000000   ) begin
            src_channel = 23'b0000100000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 12;
    end

    // ( 0x9000000 .. 0x9002000 )
    if ( {address[RG:PAD2],{PAD2{1'b0}}} == 32'h9000000   ) begin
            src_channel = 23'b0000000000000000010000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 19;
    end

    // ( 0xff200000 .. 0xff200010 )
    if ( {address[RG:PAD3],{PAD3{1'b0}}} == 32'hff200000   ) begin
            src_channel = 23'b0000010000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 11;
    end

    // ( 0xff200020 .. 0xff200030 )
    if ( {address[RG:PAD4],{PAD4{1'b0}}} == 32'hff200020   ) begin
            src_channel = 23'b0000000001000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 6;
    end

    // ( 0xff200030 .. 0xff200040 )
    if ( {address[RG:PAD5],{PAD5{1'b0}}} == 32'hff200030   ) begin
            src_channel = 23'b0000000010000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 7;
    end

    // ( 0xff200040 .. 0xff200050 )
    if ( {address[RG:PAD6],{PAD6{1'b0}}} == 32'hff200040  && read_transaction  ) begin
            src_channel = 23'b0100000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 16;
    end

    // ( 0xff200050 .. 0xff200060 )
    if ( {address[RG:PAD7],{PAD7{1'b0}}} == 32'hff200050   ) begin
            src_channel = 23'b0001000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 14;
    end

    // ( 0xff200060 .. 0xff200070 )
    if ( {address[RG:PAD8],{PAD8{1'b0}}} == 32'hff200060   ) begin
            src_channel = 23'b0000000000100000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 4;
    end

    // ( 0xff200070 .. 0xff200080 )
    if ( {address[RG:PAD9],{PAD9{1'b0}}} == 32'hff200070   ) begin
            src_channel = 23'b1000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 5;
    end

    // ( 0xff200100 .. 0xff200110 )
    if ( {address[RG:PAD10],{PAD10{1'b0}}} == 32'hff200100   ) begin
            src_channel = 23'b0000000000001000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 2;
    end

    // ( 0xff200110 .. 0xff200120 )
    if ( {address[RG:PAD11],{PAD11{1'b0}}} == 32'hff200110   ) begin
            src_channel = 23'b0000000000010000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 3;
    end

    // ( 0xff201000 .. 0xff201008 )
    if ( {address[RG:PAD12],{PAD12{1'b0}}} == 32'hff201000   ) begin
            src_channel = 23'b0000000000000000000100;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 10;
    end

    // ( 0xff202000 .. 0xff202020 )
    if ( {address[RG:PAD13],{PAD13{1'b0}}} == 32'hff202000   ) begin
            src_channel = 23'b0000000100000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 9;
    end

    // ( 0xff202020 .. 0xff202040 )
    if ( {address[RG:PAD14],{PAD14{1'b0}}} == 32'hff202020   ) begin
            src_channel = 23'b0000001000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 8;
    end

    // ( 0xff202040 .. 0xff202048 )
    if ( {address[RG:PAD15],{PAD15{1'b0}}} == 32'hff202040  && read_transaction  ) begin
            src_channel = 23'b0000000000000000100000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 17;
    end

    // ( 0xff203010 .. 0xff203014 )
    if ( {address[RG:PAD16],{PAD16{1'b0}}} == 32'hff203010  && read_transaction  ) begin
            src_channel = 23'b0000000000000100000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 21;
    end

    // ( 0xff203020 .. 0xff203030 )
    if ( {address[RG:PAD17],{PAD17{1'b0}}} == 32'hff203020   ) begin
            src_channel = 23'b0000000000000010000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 20;
    end

    // ( 0xff203030 .. 0xff203040 )
    if ( {address[RG:PAD18],{PAD18{1'b0}}} == 32'hff203030   ) begin
            src_channel = 23'b0000000000000000001000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 18;
    end

    // ( 0xff204000 .. 0xff204020 )
    if ( {address[RG:PAD19],{PAD19{1'b0}}} == 32'hff204000   ) begin
            src_channel = 23'b0000000000000000000001;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 0;
    end

    // ( 0xff204020 .. 0xff204022 )
    if ( {address[RG:PAD20],{PAD20{1'b0}}} == 32'hff204020   ) begin
            src_channel = 23'b0000000000000000000010;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 1;
    end

    // ( 0xff204040 .. 0xff204080 )
    if ( {address[RG:PAD21],{PAD21{1'b0}}} == 32'hff204040   ) begin
            src_channel = 23'b0000000000000001000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 22;
    end

end


    // --------------------------------------------------
    // Ceil(log2()) function
    // --------------------------------------------------
    function integer log2ceil;
        input reg[65:0] val;
        reg [65:0] i;

        begin
            i = 1;
            log2ceil = 0;

            while (i < val) begin
                log2ceil = log2ceil + 1;
                i = i << 1;
            end
        end
    endfunction

endmodule


