#include <stdio.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <stdint.h>
#include <errno.h>

int main()
{
  int fd = bpf_obj_get("/sys/fs/bpf/xdp/globals/allowed_ports");
  if (fd < 0)
  {
    printf("Could not open map");
    return -1;
  }


  uint16_t key;
  uint16_t next_key;
  uint8_t value;
  
  printf("--- Allowed Ports ---\n");
  if (bpf_map_get_next_key(fd, NULL, &key) != 0)
  {
    return 0;
  }
  
  do
  {
    if (bpf_map_lookup_elem(fd, &key, &value) == 0)
    {
      printf("Port: %hu (Val: %d)\n", key, value);
    }
  } while (bpf_map_get_next_key(fd, &key, &key) == 0);   
  return 0;
}
  
