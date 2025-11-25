#include <stdio.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <stdint.h>

int main()
{
  int fd = bpf_obj_get("/sys/fs/bpf/xdp/globals/allowed_ports");
  if (fd<1)
  {
    printf("err opening allowed_ports map");
    return -1;
  }
  //delete all ports
  uint16_t port;
  uint16_t key;
  uint8_t value = 0;
  while (bpf_map_get_next_key(fd, NULL, &key) == 0)
  {
    bpf_map_delete_elem(fd, &key);
  }
  //add ports from stdin
  while (scanf("%hu", &port) == 1)
  {
    bpf_map_update_elem(fd, &port, &value, BPF_ANY);
  }
  return 0;
}
