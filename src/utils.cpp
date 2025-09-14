/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:48:35 by emgul            #+#    #+#              */
/*   Updated: 2025/09/14 12:04:10 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void *ft_memset(void *str, int c, size_t n)
{
    unsigned char *buffer;

    buffer = (unsigned char *)str;
    while (n--)
        *buffer++ = (unsigned char)c;
    return (str);
}

size_t ft_strlen(const char *str)
{
    size_t len;

    if (!str)
        return (0);
    len = 0;
    while (str[len])
        len++;
    return (len);
}

size_t ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
    size_t len;
    size_t i;

    if (!dst || !src)
        return (0);
    len = ft_strlen(src);
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
