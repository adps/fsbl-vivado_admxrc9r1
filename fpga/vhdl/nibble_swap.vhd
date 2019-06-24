----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12.06.2018 16:24:23
-- Design Name: 
-- Module Name: nibble_swap - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: Swaps the nibbles of the data.
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity nibble_swap is
    Port ( data_in : in STD_LOGIC_VECTOR (31 downto 0);
           data_out : out STD_LOGIC_VECTOR (31 downto 0));
end nibble_swap;

architecture Behavioral of nibble_swap is

begin

  data_out(3 downto 0)   <= data_in(31 downto 28);
  data_out(7 downto 4)   <= data_in(27 downto 24);
  data_out(11 downto 8)  <= data_in(23 downto 20);
  data_out(15 downto 12) <= data_in(19 downto 16);
  data_out(19 downto 16) <= data_in(15 downto 12);
  data_out(23 downto 20) <= data_in(11 downto 8);
  data_out(27 downto 24) <= data_in(7 downto 4);
  data_out(31 downto 28) <= data_in(3 downto 0);


end Behavioral;
