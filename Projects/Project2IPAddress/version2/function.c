#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Check if input is hex format
bool is_hex(const char* str)
{
    if(strncmp(str, "0x", 2) == 0 || strncmp(str, "0X", 2) == 0)
        str += 2;

    int len = strlen(str);
    if(len != 8)
        return false;

    for(int i = 0; i < len; i++) {
        char ch = str[i];
        if(!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))) {
            return false;
        }
    }
    return true;
}

// Convert hex to IPv4 doteed format string C0A80001 to 192.168.0.1
void hex_to_ip_string(unsigned int hex_ip, char* output)
{
    unsigned int a = (hex_ip >> 24) & 0xFF;
    unsigned int b = (hex_ip >> 16) & 0xFF;
    unsigned int c = (hex_ip >> 8) & 0xFF;
    unsigned int d = hex_ip & 0xFF;
    sprintf(output, "%u.%u.%u.%u", a, b, c, d);
}

// Validate dotted-decimal IP with no leading zeros
bool validate(const char* ip)
{
    int a, b, c, d;
    char dummy;

    // Check for leading zeros manually
    int i = 0;
    while(ip[i] != '\0') {
        if(ip[i] == ' ')
            return false;
        if(ip[i] == '.') {
            if(ip[i + 1] == '0' && ip[i + 2] >= '0' && ip[i + 2] <= '9') {
                return false; // leading zero detected
            }
        }
        i++;
    }

    if(sscanf(ip, "%d.%d.%d.%d%c", &a, &b, &c, &d, &dummy) != 4)
        return false;

    if(a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255)
        return false;

    return true;
}

// Determine class function
char find_class(const char* ip)
{
    int a, b, c, d;
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    if(a >= 0 && a <= 127)
        return 'A';
    else if(a >= 128 && a <= 191)
        return 'B';
    else if(a >= 192 && a <= 223)
        return 'C';
    else if(a >= 224 && a <= 239)
        return 'D';
    else if(a >= 240 && a <= 255)
        return 'E';
    else
        return 'X';
}

// conversion function of ip to hexadecimal no
void ip_to_hex(const char* ip)
{
    int a, b, c, d;
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    unsigned int ip_hex = (a << 24) | (b << 16) | (c << 8) | d;
    printf("Hexadecimal Fromat for given Ip address %s is 0x%08X\n", ip, ip_hex);
}

//-----------------------------------------------------------------
// Function for 1pv6

int is_valid_ipv6_group(const char* group)
{
    int len = strlen(group);
    if(len == 0 || len > 4)
        return 0;
    for(int i = 0; i < len; ++i) {
        if(!isxdigit(group[i]))
            return 0;
    }
    return 1;
}

int is_valid_ipv6(const char* ip)
{
    if(!ip || strlen(ip) == 0)
        return 0;

    int double_colon = 0;
    char ip_copy[100];
    strncpy(ip_copy, ip, 99);
    ip_copy[99] = '\0';

    char* tokens[100];
    int token_count = 0;

    // Tokenize with ':'
    char* token = strtok(ip_copy, ":");
    while(token != NULL) {
        tokens[token_count++] = token;
        token = strtok(NULL, ":");
    }

    // Count number of colons
    int colon_count = 0;
    for(int i = 0; ip[i]; ++i) {
        if(ip[i] == ':')
            colon_count++;
    }

    // Detect "::"
    for(int i = 0; ip[i]; ++i) {
        if(ip[i] == ':' && ip[i + 1] == ':') {
            if(double_colon)
                return 0; // more than one "::"
            double_colon = 1;
        }
    }

    if(double_colon) {
        // "::" is used, allow less than 8 groups
        if(token_count > 8)
            return 0;
    } else {
        // No "::" — must have exactly 8 groups
        if(token_count != 8)
            return 0;
    }

    for(int i = 0; i < token_count; ++i) {
        if(!is_valid_ipv6_group(tokens[i]))
            return 0;
    }

    return 1;
}