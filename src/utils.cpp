/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:48:35 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 12:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int isspace(int c)
{
    if (c == ' ' || (c >= 9 && c <= 13))
        return (1);
    return (0);
}

long strtol(const char *ptr, char **endptr)
{
    const char *str = ptr;
    int sign = 0;
    long result = 0;

    if (!str)
    {
        if (endptr)
            *endptr = NULL;
        return (0);
    }
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    if (*str == '+' || *str == '-')
    {
        if (*str == '-')
            sign = 1;
        str++;
    }
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    if (endptr)
        *endptr = const_cast<char *>(str);
    return (sign ? -result : result);
}

void *memset(void *str, int c, size_t n)
{
    unsigned char *buffer;

    buffer = (unsigned char *)str;
    while (n--)
        *buffer++ = (unsigned char)c;
    return (str);
}

size_t strlen(const char *str)
{
    size_t len;

    len = 0;
    while (str[len])
        len++;
    return (len);
}

size_t strlcpy(char *dst, const char *src, size_t dstsize)
{
    size_t len;
    size_t i;

    len = strlen(src);
    i = 0;
    if (dstsize)
    {
        while (i < (dstsize - 1) && src[i])
        {
            dst[i] = src[i];
            i++;
        }
        dst[i] = '\0';
    }
    return (len);
}

std::string strtrim(const std::string &str)
{
    size_t first;
    size_t last;

    first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return ("");
    last = str.find_last_not_of(" \t\n\r");
    return (str.substr(first, (last - first + 1)));
}
