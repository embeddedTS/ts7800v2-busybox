#include "libbb.h"
#include "peekpoke.h"
         

unsigned int eptime(void);

//usage:#define eptime_trivial_usage
//usage:       "Queries the onboard timer to see how much time has elapsed"
//usage:       "since boot"
//usage:       "\n"
//usage:#define eptime_full_usage "\n\n"
//usage:       "eptime returns time since boot\n"
//usage:       "Queries the onboard timer to see how much time has elapsed"
//usage:       "since boot"
//usage:       "\n"
//usage:#define eptime_example_usage
//usage:       "\n"

unsigned int eptime(void) 
{
  unsigned int t, i;
  int devmem;
  unsigned int eptimers_page; //modelreg, pldrevreg, ts7300reg;
  
  devmem = open("/dev/mem", O_RDWR|O_SYNC);
  if (devmem == -1) {
    perror("open(/dev/mem):");
    return 0;
  }

  eptimers_page = (unsigned int)mmap(0, getpagesize(), 
    PROT_READ|PROT_WRITE, MAP_SHARED, devmem, 0x80810000);
  
  /*
  pldrevreg = (unsigned int)mmap(0, getpagesize(), 
    PROT_READ|PROT_WRITE, MAP_SHARED, devmem, 0x23400000);
  
  modelreg = (unsigned int)mmap(0, getpagesize(), 
    PROT_READ|PROT_WRITE, MAP_SHARED, devmem, 0x22000000);
  
  ts7300reg = (unsigned int)mmap(0, getpagesize(), 
    PROT_READ|PROT_WRITE, MAP_SHARED, devmem, 0x12000000);
  
  if (((PEEK16(modelreg) & 0x7) == 0x3) && ((PEEK16(pldrevreg) & 0x7) >= 0x3)) {
    t = PEEK32(ts7300reg + 0x4);
    i = t / 14745600 * 100;

    i += (t % 14745600) / 147456;
  } else {*/
    t = PEEK32(eptimers_page + 0x0060);
    i = t / 983040 * 100;
    i += (t % 983040) / 9830;
  //}

  munmap((void*)eptimers_page, getpagesize());
  //munmap(pldrevreg, getpagesize());
  //munmap(modelreg, getpagesize());
  //munmap(ts7300reg, getpagesize());
  close(devmem);
  return i;
}


int eptime_main(int argc __attribute__ ((unused)), char **argv  __attribute__ ((unused)));
int eptime_main(int argc  __attribute__ ((unused)), char **argv  __attribute__ ((unused))) {
  unsigned int t = eptime();

  fprintf(stdout, "%d.%02d\n", t / 100, t % 100);
  fflush(stdout);

  return 0;
}

