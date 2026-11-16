#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[32];

char *readfile(char *path, int mode)
{
  int fd = open(path, mode);

  memset(buf, 0, sizeof(buf));
  read(fd, buf, sizeof(buf) - 1);
  close(fd);
  return buf;
}

int main(void)
{
  int fd;

  fd = open("/file", O_CREATE | O_RDWR);
  write(fd, "hello", 5);
  close(fd);
  symlink("/file", "/link");
  fd = open("/link", O_WRONLY);
  write(fd, "HELLO", 5);
  close(fd);
  printf(1, "write via link, read via file: %s\n", readfile("/file", O_RDONLY));

  symlink("/file", "/c");
  symlink("/c", "/b");
  symlink("/b", "/a");
  printf(1, "chain a -> b -> c: %s\n", readfile("/a", O_RDONLY));

  symlink("/nothing", "/dangling");
  printf(1, "dangling link open: %d\n", open("/dangling", O_RDONLY));

  symlink("/loop2", "/loop1");
  symlink("/loop1", "/loop2");
  printf(1, "loop open: %d\n", open("/loop1", O_RDONLY));

  printf(1, "O_NOFOLLOW read target: %s\n", readfile("/link", O_NOFOLLOW));

  unlink("/link");
  printf(1, "after unlink, target: %s\n", readfile("/file", O_RDONLY));

  printf(1, "O_NOFOLLOW | O_CREATE: %d\n", open("/new", O_NOFOLLOW | O_CREATE));
  printf(1, "O_CREATE on link: %d\n", open("/a", O_CREATE | O_RDWR));
  printf(1, "O_CREATE on dangling link: %d\n", open("/dangling", O_CREATE | O_RDWR));
  exit();
}
