----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 14.06.2018 09:47:26
-- Design Name: 
-- Module Name: zynq_axi_loop - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity zynq_axi_loop is
  port (
    S_AXI_HP0_FPD_0_araddr  : in  std_logic_vector (39 downto 0);
    S_AXI_HP0_FPD_0_arburst : in  std_logic_vector (1 downto 0);
    S_AXI_HP0_FPD_0_arcache : in  std_logic_vector (3 downto 0);
    S_AXI_HP0_FPD_0_arid    : in  std_logic_vector (5 downto 0);
    S_AXI_HP0_FPD_0_arlen   : in  std_logic_vector (7 downto 0);
    S_AXI_HP0_FPD_0_arlock  : in  std_logic;
    S_AXI_HP0_FPD_0_arprot  : in  std_logic_vector (2 downto 0);
    S_AXI_HP0_FPD_0_arqos   : in  std_logic_vector (3 downto 0);
    S_AXI_HP0_FPD_0_arready : out std_logic;
    S_AXI_HP0_FPD_0_arsize  : in  std_logic_vector (2 downto 0);
    S_AXI_HP0_FPD_0_aruser  : in  std_logic;
    S_AXI_HP0_FPD_0_arvalid : in  std_logic;
    S_AXI_HP0_FPD_0_awaddr  : in  std_logic_vector (39 downto 0);
    S_AXI_HP0_FPD_0_awburst : in  std_logic_vector (1 downto 0);
    S_AXI_HP0_FPD_0_awcache : in  std_logic_vector (3 downto 0);
    S_AXI_HP0_FPD_0_awid    : in  std_logic_vector (5 downto 0);
    S_AXI_HP0_FPD_0_awlen   : in  std_logic_vector (7 downto 0);
    S_AXI_HP0_FPD_0_awlock  : in  std_logic;
    S_AXI_HP0_FPD_0_awprot  : in  std_logic_vector (2 downto 0);
    S_AXI_HP0_FPD_0_awqos   : in  std_logic_vector (3 downto 0);
    S_AXI_HP0_FPD_0_awready : out std_logic;
    S_AXI_HP0_FPD_0_awsize  : in  std_logic_vector (2 downto 0);
    S_AXI_HP0_FPD_0_awuser  : in  std_logic;
    S_AXI_HP0_FPD_0_awvalid : in  std_logic;
    S_AXI_HP0_FPD_0_bid     : out std_logic_vector (5 downto 0);
    S_AXI_HP0_FPD_0_bready  : in  std_logic;
    S_AXI_HP0_FPD_0_bresp   : out std_logic_vector (1 downto 0);
    S_AXI_HP0_FPD_0_bvalid  : out std_logic;
    S_AXI_HP0_FPD_0_rdata   : out std_logic_vector (127 downto 0);
    S_AXI_HP0_FPD_0_rid     : out std_logic_vector (5 downto 0);
    S_AXI_HP0_FPD_0_rlast   : out std_logic;
    S_AXI_HP0_FPD_0_rready  : in  std_logic;
    S_AXI_HP0_FPD_0_rresp   : out std_logic_vector (1 downto 0);
    S_AXI_HP0_FPD_0_rvalid  : out std_logic;
    S_AXI_HP0_FPD_0_wdata   : in  std_logic_vector (127 downto 0);
    S_AXI_HP0_FPD_0_wlast   : in  std_logic;
    S_AXI_HP0_FPD_0_wready  : out std_logic;
    S_AXI_HP0_FPD_0_wstrb   : in  std_logic_vector (15 downto 0);
    S_AXI_HP0_FPD_0_wvalid  : in  std_logic;

    M_AXI_HP0_FPD_0_araddr  : out std_logic_vector (39 downto 0);
    M_AXI_HP0_FPD_0_arburst : out std_logic_vector (1 downto 0);
    M_AXI_HP0_FPD_0_arcache : out std_logic_vector (3 downto 0);
    M_AXI_HP0_FPD_0_arid    : out std_logic_vector (5 downto 0);
    M_AXI_HP0_FPD_0_arlen   : out std_logic_vector (7 downto 0);
    M_AXI_HP0_FPD_0_arlock  : out std_logic;
    M_AXI_HP0_FPD_0_arprot  : out std_logic_vector (2 downto 0);
    M_AXI_HP0_FPD_0_arqos   : out std_logic_vector (3 downto 0);
    M_AXI_HP0_FPD_0_arready : in  std_logic;
    M_AXI_HP0_FPD_0_arsize  : out std_logic_vector (2 downto 0);
    M_AXI_HP0_FPD_0_aruser  : out std_logic;
    M_AXI_HP0_FPD_0_arvalid : out std_logic;
    M_AXI_HP0_FPD_0_awaddr  : out std_logic_vector (39 downto 0);
    M_AXI_HP0_FPD_0_awburst : out std_logic_vector (1 downto 0);
    M_AXI_HP0_FPD_0_awcache : out std_logic_vector (3 downto 0);
    M_AXI_HP0_FPD_0_awid    : out std_logic_vector (5 downto 0);
    M_AXI_HP0_FPD_0_awlen   : out std_logic_vector (7 downto 0);
    M_AXI_HP0_FPD_0_awlock  : out std_logic;
    M_AXI_HP0_FPD_0_awprot  : out std_logic_vector (2 downto 0);
    M_AXI_HP0_FPD_0_awqos   : out std_logic_vector (3 downto 0);
    M_AXI_HP0_FPD_0_awready : in  std_logic;
    M_AXI_HP0_FPD_0_awsize  : out std_logic_vector (2 downto 0);
    M_AXI_HP0_FPD_0_awuser  : out std_logic;
    M_AXI_HP0_FPD_0_awvalid : out std_logic;
    M_AXI_HP0_FPD_0_bid     : in  std_logic_vector (5 downto 0);
    M_AXI_HP0_FPD_0_bready  : out std_logic;
    M_AXI_HP0_FPD_0_bresp   : in  std_logic_vector (1 downto 0);
    M_AXI_HP0_FPD_0_bvalid  : in  std_logic;
    M_AXI_HP0_FPD_0_rdata   : in  std_logic_vector (127 downto 0);
    M_AXI_HP0_FPD_0_rid     : in  std_logic_vector (5 downto 0);
    M_AXI_HP0_FPD_0_rlast   : in  std_logic;
    M_AXI_HP0_FPD_0_rready  : out std_logic;
    M_AXI_HP0_FPD_0_rresp   : in  std_logic_vector (1 downto 0);
    M_AXI_HP0_FPD_0_rvalid  : in  std_logic;
    M_AXI_HP0_FPD_0_wdata   : out std_logic_vector (127 downto 0);
    M_AXI_HP0_FPD_0_wlast   : out std_logic;
    M_AXI_HP0_FPD_0_wready  : in  std_logic;
    M_AXI_HP0_FPD_0_wstrb   : out std_logic_vector (15 downto 0);
    M_AXI_HP0_FPD_0_wvalid  : out std_logic
    );
end zynq_axi_loop;

architecture Behavioral of zynq_axi_loop is

  constant offset : unsigned(M_AXI_HP0_FPD_0_araddr'range) := resize(X"400000000",M_AXI_HP0_FPD_0_araddr'length);

begin

  --Remap the AXI transactions to the range of the DDR_HIGH memory.

  M_AXI_HP0_FPD_0_araddr  <= std_logic_vector(unsigned(S_AXI_HP0_FPD_0_araddr) + offset);
  M_AXI_HP0_FPD_0_arburst <= S_AXI_HP0_FPD_0_arburst;
  M_AXI_HP0_FPD_0_arcache <= S_AXI_HP0_FPD_0_arcache;
  M_AXI_HP0_FPD_0_arid    <= S_AXI_HP0_FPD_0_arid;
  M_AXI_HP0_FPD_0_arlen   <= S_AXI_HP0_FPD_0_arlen;
  M_AXI_HP0_FPD_0_arlock  <= S_AXI_HP0_FPD_0_arlock;
  M_AXI_HP0_FPD_0_arprot  <= S_AXI_HP0_FPD_0_arprot;
  M_AXI_HP0_FPD_0_arqos   <= S_AXI_HP0_FPD_0_arqos;
  M_AXI_HP0_FPD_0_arsize  <= S_AXI_HP0_FPD_0_arsize;
  M_AXI_HP0_FPD_0_aruser  <= S_AXI_HP0_FPD_0_aruser;
  M_AXI_HP0_FPD_0_arvalid <= S_AXI_HP0_FPD_0_arvalid;
  M_AXI_HP0_FPD_0_awaddr  <= std_logic_vector(unsigned(S_AXI_HP0_FPD_0_awaddr) + offset);
  M_AXI_HP0_FPD_0_awburst <= S_AXI_HP0_FPD_0_awburst;
  M_AXI_HP0_FPD_0_awcache <= S_AXI_HP0_FPD_0_awcache;
  M_AXI_HP0_FPD_0_awid    <= S_AXI_HP0_FPD_0_awid;
  M_AXI_HP0_FPD_0_awlen   <= S_AXI_HP0_FPD_0_awlen;
  M_AXI_HP0_FPD_0_awlock  <= S_AXI_HP0_FPD_0_awlock;
  M_AXI_HP0_FPD_0_awprot  <= S_AXI_HP0_FPD_0_awprot;
  M_AXI_HP0_FPD_0_awqos   <= S_AXI_HP0_FPD_0_awqos;
  M_AXI_HP0_FPD_0_awsize  <= S_AXI_HP0_FPD_0_awsize;
  M_AXI_HP0_FPD_0_awuser  <= S_AXI_HP0_FPD_0_awuser;
  M_AXI_HP0_FPD_0_awvalid <= S_AXI_HP0_FPD_0_awvalid;
  M_AXI_HP0_FPD_0_bready  <= S_AXI_HP0_FPD_0_bready;
  M_AXI_HP0_FPD_0_rready  <= S_AXI_HP0_FPD_0_rready;
  M_AXI_HP0_FPD_0_wdata   <= S_AXI_HP0_FPD_0_wdata;
  M_AXI_HP0_FPD_0_wlast   <= S_AXI_HP0_FPD_0_wlast;
  M_AXI_HP0_FPD_0_wstrb   <= S_AXI_HP0_FPD_0_wstrb;
  M_AXI_HP0_FPD_0_wvalid  <= S_AXI_HP0_FPD_0_wvalid;

  S_AXI_HP0_FPD_0_arready <= M_AXI_HP0_FPD_0_arready;
  S_AXI_HP0_FPD_0_awready <= M_AXI_HP0_FPD_0_awready;
  S_AXI_HP0_FPD_0_bid     <= M_AXI_HP0_FPD_0_bid;
  S_AXI_HP0_FPD_0_bresp   <= M_AXI_HP0_FPD_0_bresp;
  S_AXI_HP0_FPD_0_bvalid  <= M_AXI_HP0_FPD_0_bvalid;
  S_AXI_HP0_FPD_0_rdata   <= M_AXI_HP0_FPD_0_rdata;
  S_AXI_HP0_FPD_0_rid     <= M_AXI_HP0_FPD_0_rid;
  S_AXI_HP0_FPD_0_rlast   <= M_AXI_HP0_FPD_0_rlast;
  S_AXI_HP0_FPD_0_rresp   <= M_AXI_HP0_FPD_0_rresp;
  S_AXI_HP0_FPD_0_rvalid  <= M_AXI_HP0_FPD_0_rvalid;
  S_AXI_HP0_FPD_0_wready  <= M_AXI_HP0_FPD_0_wready;

end Behavioral;
