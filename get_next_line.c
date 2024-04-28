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

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 8
#endif

/* Reads bytes from the specified fd and stores them
* in the buf. Returns amount of bytes read from the fd */
int read_to_buf(int fd, char *buf, size_t n_bytes)
{
	int bytes_read;

	bytes_read = read(fd, buf, n_bytes);
	buf[bytes_read] = '\0';
	return (bytes_read);

}
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
char	*move_remaining_to_nl(char *next_line, char *read_buf, char *newline)
{
	size_t	bytes_to_linebreak;
	size_t	next_line_len;

	bytes_to_linebreak = newline - read_buf + 1;
	next_line = grow_buffer(next_line, bytes_to_linebreak);
	if (!next_line)
		return (NULL);
	next_line_len = ft_strlen(next_line);
	ft_memcpy(next_line + ft_strlen(next_line), read_buf, bytes_to_linebreak);
	ft_memset(next_line + next_line_len + bytes_to_linebreak, 0, 1);
	ft_memcpy(read_buf, newline + 1, BUFFER_SIZE - bytes_to_linebreak + 1);
	ft_memset(read_buf + (BUFFER_SIZE - bytes_to_linebreak), 0, bytes_to_linebreak);
	return (next_line);
}

char	*process_read_buffer(char *read_buf, char *next_line, int bytes_read, int fd)
{
		char	*newline;

		newline = ft_strchr(read_buf, '\n'); //Check for newline in buffer
		if (newline)
			return (move_remaining_to_nl(next_line, read_buf, newline));
		next_line = grow_buffer(next_line, bytes_read);
		if (!next_line)
			return(NULL);
		ft_strlcat(next_line, read_buf, ft_strlen(next_line) + bytes_read + 1);
		ft_memset(read_buf, 0, BUFFER_SIZE + 1); //Reset buffer after full copy
		bytes_read = read_to_buf(fd, read_buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(next_line);
			return (NULL);
		}
		return (next_line);
}
char	*get_next_line(int fd)
{
	static char read_buf[BUFFER_SIZE + 1];
	char		*next_line;
	int			bytes_read;
	next_line = malloc(sizeof(char));
	next_line[0] = 0;
	if (ft_strlen(read_buf) > 0)
		bytes_read = ft_strlen(read_buf);
	else
		bytes_read = read_to_buf(fd, read_buf, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		next_line = process_read_buffer(read_buf, next_line, bytes_read, fd);
		if (!next_line)
			return (NULL);
		if (ft_strchr(next_line, '\n'))
			return (next_line);
		bytes_read = ft_strlen(read_buf);
	}
	if (next_line[0] == 0)
	{
		free(next_line);
		return (NULL);
	}
	return (next_line);
}
