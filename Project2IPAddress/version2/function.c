#include <stdio.h>
#include <stdbool.h>

// Validate an IPv4 address
bool validate(const char* ip)
{
    int a, b, c, d;
    char dummy;

    if(sscanf(ip, "%d.%d.%d.%d%c", &a, &b, &c, &d, &dummy) != 4)
        return false;

    if(a < 0 || a > 255)
        return false;
    if(b < 0 || b > 255)
        return false;
    if(c < 0 || c > 255)
        return false;
    if(d < 0 || d > 255)
        return false;

    return true;
}

// Find the IP class based on first octet
char find_class(const char* ip)
{
    int a, b, c, d;
    char dummy;

    if(sscanf(ip, "%d.%d.%d.%d%c", &a, &b, &c, &d, &dummy) != 4)
        return 'X'; // Invalid

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

    return 'X'; // Out of valid IP range
}