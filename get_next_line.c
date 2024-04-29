/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:10:02 by pleander          #+#    #+#             */
/*   Updated: 2024/04/29 09:42:42 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

/* Reads bytes from the specified fd and stores them
* in the buf. Returns amount of bytes read from the fd */
int	read_to_buf(int fd, char *buf, size_t n_bytes)
{
	int	bytes_read;

	bytes_read = read(fd, buf, n_bytes);
	buf[bytes_read] = '\0';
	return (bytes_read);
}

/* Grows the buffer size by n_bytes, initialized the new memory as 0
 * and returns the increased buffer */
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

/* copies remaining characters in read_buf to next_line and moves the 
* unused characters to the start of read_buf */
char	*copy_buf_to_nl(char *next_line, char *read_buf, size_t n_bytes)
{
	char *new_nl;

	new_nl = grow_buffer(next_line, n_bytes);
	if (!new_nl)
		return (NULL);
	ft_strncat(new_nl, read_buf, n_bytes);
	ft_memcpy(read_buf, read_buf + n_bytes, BUFFER_SIZE - n_bytes);
	read_buf[BUFFER_SIZE - n_bytes] = 0;
	return (new_nl);
	
}
char	*read_until_nl(char *next_line, char *read_buf, int fd)
{
	char *newline_pos;
	int	bytes_read;

	newline_pos = NULL;
	bytes_read = -1;
	while (!newline_pos && bytes_read != 0)
	{
		next_line = copy_buf_to_nl(next_line, read_buf, BUFFER_SIZE);
		if (!next_line)
			return (NULL);
		bytes_read = read_to_buf(fd, read_buf, BUFFER_SIZE);
		newline_pos = ft_strchr(read_buf, '\n');
	}
	return (next_line);
}

char	*get_next_line(int fd)
{
	static char	read_buf[BUFFER_SIZE + 1]; // Is it allowed to have BUFFER_SIZE  + 1?
	char		*next_line;
	char		*newline_pos;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	next_line = malloc(sizeof(char));
	next_line[0] = 0;
	newline_pos = ft_strchr(read_buf, '\n');
	if (!newline_pos)
	{
		next_line = read_until_nl(next_line, read_buf, fd);
		newline_pos = ft_strchr(read_buf, '\n');
		if (!newline_pos)
			return (NULL);
	}
	next_line = copy_buf_to_nl(next_line, read_buf, newline_pos - read_buf + 1);
	if (ft_strlen(next_line) == 0)
		return (NULL);
	return (next_line);
}
