#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h> //for the ipv4 header struct
#include <linux/tcp.h> //for the tcp header struct
#include <bpf/bpf_helpers.h>

//bpf map where keys are allowed ports
struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __uint(max_entries, 1024);
  __type(key, __u16);
  __type(value, __u8);
  __uint(pinning, 1);
} allowed_ports SEC(".maps");

SEC("xdp")
int xdp_filter(struct xdp_md *ctx)
{
  void *pkt = (void*)(long)ctx->data;
  void *pkt_end = (void*)(long)ctx->data_end;
  //check for bad packets / non ipv4
  struct ethhdr *eth = pkt;
  if (((void*)(eth+1) > pkt_end) || (eth->h_proto != __constant_htons(ETH_P_IP)))
  {
    return XDP_PASS;
  }
  struct iphdr *ipv4_header = pkt + sizeof(*eth);
  //check for bad packets / non tcp
  //6 is the value of PACKETPP_IPPROTO_TCP
  if (((void*)(ipv4_header+1) > pkt_end) || (ipv4_header->protocol != 6))
  {
    return XDP_PASS;
  }
  struct tcphdr *tcp = (void*)ipv4_header + ipv4_header->ihl*4;
  if ((void*)(tcp+1) > pkt_end)
  {
    return XDP_PASS;
  }
  //main fw logic
  __u16 dport = __constant_ntohs(tcp->dest);
  __u8 *whitelisted = bpf_map_lookup_elem(&allowed_ports, &dport);
  if (tcp->ack || !tcp->syn)
  {
    return XDP_PASS;
  }
  if(!bpf_map_lookup_elem(&allowed_ports, &dport))
  {
    return XDP_DROP;
  }
    return XDP_PASS;
}

char LICENSE[] SEC("license") = "GPL";
