#include "libbb.h"
#include "peekpoke.h"


//usage:#define mvtime_trivial_usage
//usage:       "mvtime returns time since boot"
//usage:
//usage:#define mvtime_full_usage "\n\n"
//usage:       "mvtime returns time since boot\n"

//usage:
//usage:#define mvtime_example_usage
//usage:       "\n"

int mvtime_main(int argc __attribute__ ((unused)), char **argv __attribute__ ((unused)));
int mvtime_main(int argc __attribute__ ((unused)), char **argv __attribute__ ((unused))) {
  unsigned int t = 0, bid;
  int fd;
  unsigned char *start;

  fd = open("/dev/mem", O_RDWR|O_SYNC);
  if (fd == -1) {
    perror("open(/dev/mem):");
    return 0;
  }

  start = mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0xe8000000);
  if (start == MAP_FAILED) {
    perror("mmap:");
    return 0;
  }

  bid = PEEK32((unsigned int)start);
  if ((bid&0xfff000) == 0xb48000)
    t = PEEK32((unsigned int)(start+0x40));

  close(fd);

  fprintf(stdout, "%d.%02d\n",t/1000000,(t%1000000)/10000);
  fflush(stdout);

  return EXIT_SUCCESS;
}

