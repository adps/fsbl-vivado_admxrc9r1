/* Writes data to the 9z2 FPGA, and reads that data back.
 * The FPGA implements a nibble-swap to check that the data
 * is definitely being transferred between the processor
 * and the FPGA
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <termios.h>
#include <string.h>

#define FPGA_ADDR 0x80000000

int main(void)
{
  int i=0;
  int reg_data = 0x0;

  char cUserInput[100];
  
  char *pFPGA;
  int fd;
  fd = open ("/dev/mem", O_RDWR);
  assert(fd >= 0);

  //Map the FPGA space.
  volatile int *pFPGAreg;
	pFPGA = mmap(NULL, 0x100, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t) (FPGA_ADDR));
	pFPGAreg = (int *) pFPGA;
  
  printf("Type q to quit or press ctrl-c\n");
  while(1)
  {
    
    printf("Enter an 8 digit hex number:\n");
    scanf("%s", &cUserInput);   

    if(cUserInput[0] == 'q')
      return 0;
    
    if( cUserInput[strspn(cUserInput, "0123456789abcdefABCDEF")] )
    {
      printf("Not a hex string. Enter a valid hex string or q to quit.\n");
      continue;
    }
    //Write data to the FPGA, then read it back.
    //If the FPGA is working, it will swap the nibbles.
    pFPGAreg[0] = strtol(cUserInput, NULL, 16);
  
    volatile int nReadData = pFPGAreg[0];
  
    printf("0x%08x\n\n", nReadData);
  
  }

  return 0;
}