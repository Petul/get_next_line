/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:13:32 by pleander          #+#    #+#             */
/*   Updated: 2024/05/02 16:13:34 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <limits.h>
#include "get_next_line_bonus.h"

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
	char	*new_nl;

	if (!next_line)
	{
		next_line = malloc(sizeof(char));
		if (!next_line)
			return (NULL);
		next_line[0] = 0;
	}
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
	char	*newline_pos;
	int		bytes_read;

	newline_pos = NULL;
	bytes_read = -1;
	while (!newline_pos && bytes_read != 0)
	{
		next_line = copy_buf_to_nl(next_line, read_buf, BUFFER_SIZE);
		if (!next_line)
			return (NULL);
		bytes_read = read(fd, read_buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(next_line);
			*read_buf = 0;
			return (NULL);
		}
		if ((size_t)bytes_read < BUFFER_SIZE)
			ft_memset(read_buf + (size_t)bytes_read, 0,
				BUFFER_SIZE - (size_t)bytes_read);
		read_buf[bytes_read] = '\0';
		newline_pos = ft_strchr(read_buf, '\n');
	}
	return (next_line);
}

void	*reset_buf(char *buf)
{
	*buf = 0;
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*fd_arr[OPEN_MAX][BUFFER_SIZE + 1];
	char		*next_line;
	char		*newline_pos;

	if (fd < 0 || fd > OPEN_MAX || BUFFER_SIZE < 1)
		return (NULL);
	if (read(fd, NULL, 0) < 0)
		return (reset_buf((char *)fd_arr[fd]));
	next_line = NULL;
	newline_pos = ft_strchr((char *)fd_arr[fd], '\n');
	if (!newline_pos)
	{
		next_line = read_until_nl(next_line, (char *)fd_arr[fd], fd);
		if (!next_line)
			return (NULL);
		newline_pos = ft_strchr((char *)fd_arr[fd], '\n');
	}
	if (!newline_pos)
		newline_pos = (char *)fd_arr[fd] + ft_strlen((char *)fd_arr[fd]) - 1;
	next_line = copy_buf_to_nl(next_line, (char *)fd_arr[fd], newline_pos - (char *)fd_arr[fd] + 1);
	if (next_line != NULL && ft_strlen(next_line) == 0)
	{
		free(next_line);
		next_line = NULL;
	}
	return (next_line);
}
