/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:46:10 by pleander          #+#    #+#             */
/*   Updated: 2024/04/26 16:17:11 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (i <= (int)ft_strlen(s))
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	return (0);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const unsigned char	*s;	
	unsigned char		*d;
	size_t				i;

	d = dst;
	s = src;
	i = 0;
	if (!dst && !src)
		return (dst);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	n;

	src_len = ft_strlen(src);
	dst_len = ft_strlen(dst);
	n = 1;
	while (src[n - 1] && (dst_len + n) < dstsize)
	{
		dst[dst_len + n - 1] = src[n - 1];
		n++;
	}
	dst[dst_len + n - 1] = '\0';
	return (dst_len + src_len);
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*p;	
	size_t			i;

	p = b;
	i = 0;
	while (i < len)
	{
		p[i] = c;
		i++;
	}
	return (b);
}
