#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
 
#define LEN 512
 
typedef unsigned short u16;
typedef unsigned long u32;
 
//2nd
unsigned short csum (unsigned short *buf, int nwords);
uint16_t udp_checksum(const struct iphdr  *ip,
        const struct udphdr *udp,
        const uint16_t *buf);
 
int enviopack(char *a, int b, int c, char *d, int querylen)
 {
//  int d_port = 4321;  
   // char a[] = "192.168.1.1";
    char ip_d[20] = "0.0.0.0";
    int s_port = b;
    int PSOURCE = s_port;
    int PDEST = c;
    char * SOURCE = ip_d;
    char * DEST = a;
    int s;
    struct sockaddr_in daddr, saddr, answerip;
    char packet[LEN];
    /* point the iphdr to the beginning of the packet */
    struct iphdr *ip = (struct iphdr *)packet;  
    struct udphdr *udp = (struct udphdr *)((void *) ip + sizeof(struct iphdr));
    struct dnshdr *dns = (struct dnshdr *)((void *) udp + sizeof(struct udphdr));
 
    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }
    daddr.sin_family = AF_INET;
    saddr.sin_family = AF_INET;
    daddr.sin_port = htons(PDEST);
    saddr.sin_port = htons(PSOURCE);
    inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
    inet_pton(AF_INET, SOURCE, (struct in_addr *)&saddr.sin_addr.s_addr);
    memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
    memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));
    memset(udp, 0, sizeof(struct udphdr));
    memset(ip, 0, sizeof(struct iphdr));
    ip->ihl = 5; //header length
    ip->version = 4;
    ip->tos = 0x0;
    ip->id = 0;
    ip->frag_off = 0;  
    ip->ttl = 110;          /* default value */
    ip->protocol = 17;  //IPPROTO_RAW;  /* protocol at L4 */
    ip->check = 0;          /* not needed in iphdr */
    ip->saddr = saddr.sin_addr.s_addr;
    ip->daddr = daddr.sin_addr.s_addr;
    char * packet_pos  = ((void *) udp) + sizeof(struct udphdr);
    udp->source = htons(PSOURCE);
    udp->dest = htons (PDEST);
    int sizedata = querylen;  
   // memset(((void *) udp) + sizeof(struct udphdr), 'A', sizedata);  
    int sizeudpdata = sizeof(struct udphdr) + querylen;
    ip->tot_len = htons(sizeudpdata + sizeof(struct iphdr));    /* 16 byte value */
    udp->len = htons(sizeudpdata);
     udp->check = 0;
    int optval = 1;
    if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int))
            < 0)
        perror ("IP HDRINCL");
        int sizepacket = sizeof(struct iphdr) + sizeudpdata;
               memcpy(((void *) udp) + sizeof(struct udphdr), (char*)d, querylen);
sendto(s, (char *)packet, sizepacket, 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) ;
        //    close(s);
 
}
 
//http://www.linuxquestions.org/questions/linux-networking-3/udp-checksum-algorithm-845618/
//modified by Gabriel Serme
struct pseudo_hdr {
    u_int32_t source;
    u_int32_t dest;
    u_int8_t zero; //reserved, check http://www.rhyshaden.com/udp.htm
    u_int8_t protocol;
    u_int16_t udp_length;
};
 
uint16_t udp_checksum(const struct iphdr  *ip,
        const struct udphdr *udp,
        const uint16_t *buf)
{
    //take in account padding if necessary
    int calculated_length = ntohs(udp->len)%2 == 0 ? ntohs(udp->len) : ntohs(udp->len) + 1;
 
    struct pseudo_hdr ps_hdr = {0};
    bzero (&ps_hdr, sizeof(struct pseudo_hdr));
    uint8_t data[sizeof(struct pseudo_hdr) + calculated_length];
    bzero (data, sizeof(struct pseudo_hdr) + calculated_length );
 
    ps_hdr.source = ip->saddr;
    ps_hdr.dest = ip->daddr;
    ps_hdr.protocol = IPPROTO_UDP; //17
    ps_hdr.udp_length = udp->len;
 
    memcpy(data, &ps_hdr, sizeof(struct pseudo_hdr));
    memcpy(data + sizeof(struct pseudo_hdr), buf, ntohs(udp->len) ); //the remaining bytes are set to 0
 
    return csum((uint16_t *)data, sizeof(data)/2);
}
 
/* Not my code */
unsigned short csum (unsigned short *buf, int nwords)
{
    unsigned long sum;
 
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
 
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}
