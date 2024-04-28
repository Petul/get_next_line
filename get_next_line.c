/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:10:02 by pleander          #+#    #+#             */
/*   Updated: 2024/04/26 16:22:35 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"
#include <stdio.h>

/* Reads bytes from the specified fd and stores them
* in the buf. Returns amount of bytes read from the fd */
int read_to_buf(int fd, char *buf, size_t n_bytes)
{
	int bytes_read;

	bytes_read = read(fd, buf, n_bytes);
	buf[bytes_read] = '\0';
	return (bytes_read);

}
#include <stdio.h>
char	*grow_buffer(char *buf, size_t n_bytes)
{
	int		len;
	char	*new_buf;

	len = ft_strlen(buf) + 1 + n_bytes;
	new_buf = malloc(sizeof(char) * len);
	if (!new_buf)
	{
		free(buf);
		return (NULL);
	}
	ft_memcpy(new_buf, buf, ft_strlen(buf) + 1);
	free(buf);
	return (new_buf);
}


char	*get_next_line(int fd)
{
	static char read_buf[BUFFER_SIZE + 1];
	char		*next_line;
	char		*newline;
	int			bytes_read;
	int			bytes_in_buffer;
	int			next_line_len;

	bytes_in_buffer = ft_strlen(read_buf);
	next_line = malloc(sizeof(char));
	next_line[0] = '\0'; 
	next_line = grow_buffer(next_line, bytes_in_buffer);
	ft_memcpy(next_line, read_buf, BUFFER_SIZE);
	next_line[bytes_in_buffer] = 0;
	bytes_read = read_to_buf(fd, read_buf, BUFFER_SIZE - bytes_in_buffer);
	if (bytes_read <= 0)
		return (NULL);
	newline = ft_strchr(read_buf, '\n');
	while (!newline)
	{
		next_line = grow_buffer(next_line, bytes_read);
		if (!next_line)
		{
			return(NULL);
			//free allocated stuff
		}
		ft_strlcat(next_line, read_buf, ft_strlen(next_line) + bytes_read + 1);
		bytes_read = read_to_buf(fd, read_buf, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			return (NULL);
			//free allocated stuff
		}
		newline = ft_strchr(read_buf, '\n');
	}
	bytes_in_buffer = newline - read_buf + 1;
	next_line = grow_buffer(next_line, bytes_in_buffer);
	next_line_len = ft_strlen(next_line);
	ft_memcpy(next_line + ft_strlen(next_line), read_buf, bytes_in_buffer);
	ft_memset(next_line + next_line_len + bytes_in_buffer, 0, 1);
	ft_memmove(read_buf, newline + 1, BUFFER_SIZE - bytes_in_buffer + 1);
	ft_memset(read_buf + (BUFFER_SIZE - bytes_in_buffer), 0, bytes_in_buffer);
	return (next_line);
}
