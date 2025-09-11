#include <stdio.h>
#include <stdbool.h>


bool is_hex(const char* str);
void hex_to_ip_string(unsigned int hex_ip, char* output);

bool validate(const char* ip);
char find_class(const char* ip);
void ip_to_hex(const char* ip);
int is_valid_ipv6_group(const char *group);
int is_valid_ipv6(const char *ip);