#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
readchar(int fd, char *c)
{
  return read(fd, c, 1) == 1;
}

int
createfile(char *name)
{
  unlink(name);
  return open(name, O_CREATE | O_WRONLY);
}

void
encode(int in, int out)
{
  char c, prev;
  int count;

  if(!readchar(in, &prev))
    return;
  count = 1;
  while(readchar(in, &c)){
    if(c == prev){
      count++;
    } else {
      printf(out, "%c,%d;", prev, count);
      prev = c;
      count = 1;
    }
  }
  printf(out, "%c,%d;", prev, count);
}

int
decode(int in, int out)
{
  char c, next;
  int count, i;

  while(readchar(in, &c)){
    if(!readchar(in, &next) || next != ',')
      return -1;
    count = 0;
    while(readchar(in, &next) && next >= '0' && next <= '9')
      count = count * 10 + (next - '0');
    if(next != ';')
      return -1;
    for(i = 0; i < count; i++)
      write(out, &c, 1);
  }
  return 0;
}

int
main(int argc, char *argv[])
{
  int in, out;

  if(argc != 3 || (strcmp(argv[1], "encode") != 0 && strcmp(argv[1], "decode") != 0)){
    printf(2, "usage: rle encode|decode file\n");
    exit();
  }

  in = open(argv[2], O_RDONLY);
  if(in < 0){
    printf(2, "rle: cannot open %s\n", argv[2]);
    exit();
  }

  if(strcmp(argv[1], "encode") == 0){
    out = createfile("encoded.txt");
    encode(in, out);
  } else {
    out = createfile("decoded.txt");
    if(decode(in, out) < 0)
      printf(2, "rle: bad input\n");
  }

  close(in);
  close(out);
  exit();
}
