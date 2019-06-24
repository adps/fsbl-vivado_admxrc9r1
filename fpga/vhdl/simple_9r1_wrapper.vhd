--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
--Date        : Thu Jun 13 15:43:55 2019
--Host        : chris-pc running 64-bit major release  (build 9200)
--Command     : generate_target simple_9r1_wrapper.bd
--Design      : simple_9r1_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library UNISIM;
use UNISIM.VCOMPONENTS.all;
entity simple_9r1_wrapper is
  port (
    fabric_clk_n : in std_logic;
    fabric_clk_p : in std_logic
    );
end simple_9r1_wrapper;

architecture STRUCTURE of simple_9r1_wrapper is
  component simple_9r1 is
    port (
      fabric_clk_clk_p : in std_logic_vector (0 to 0);
      fabric_clk_clk_n : in std_logic_vector (0 to 0)
      );
  end component simple_9r1;
begin
  simple_9r1_i : component simple_9r1
    port map (
      fabric_clk_clk_n(0) => fabric_clk_n,
      fabric_clk_clk_p(0) => fabric_clk_p
      );
end STRUCTURE;
