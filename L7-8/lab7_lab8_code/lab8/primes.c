#include "kernel/types.h"
#include "user/user.h"

// prints the primes from 2 to 35 with a chain of processes
void sieve(int left) __attribute__((noreturn));

void
sieve(int left)           // left: read end of the pipe from the previous stage
{
  int p, n;
  if(read(left, &p, sizeof(p)) != sizeof(p)){
    close(left);
    exit(0);
  }
  printf("prime %d\n", p);

  int right[2];
  pipe(right);
  if(fork() == 0){
    close(right[1]);
    close(left);
    sieve(right[0]);
  }
  close(right[0]);
  while(read(left, &n, sizeof(n)) == sizeof(n)){
    if(n % p != 0)
      write(right[1], &n, sizeof(n));
  }
  close(left);
  close(right[1]);
  wait(0);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int first[2];
  pipe(first);
  if(fork() == 0){
    close(first[1]);
    sieve(first[0]);
  }
  close(first[0]);
  for(int i = 2; i <= 35; i++)
    write(first[1], &i, sizeof(i));
  close(first[1]);
  wait(0);
  exit(0);
}
