/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:10:02 by pleander          #+#    #+#             */
/*   Updated: 2024/05/03 13:05:04 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

char	*grow_buffer(char *buf, size_t buf_size, size_t n_bytes);
char	*copy_buf_to_nl(char *next_line, char *read_buf, size_t n_bytes);
char	*read_until_nl(char *next_line, char *read_buf, int fd);
void	*reset_buf(char *buf);
char	*get_next_line(int fd);

/* Grows the size of the passed buffer by n_bytes.
 *
 * First the function creates a new buffer which is n_bytes larger than the
 * buf_size. Then the old buffer is copied to the start of the new buffer.
 * Finally the old buffer is freed and the new buffer is returned. Returns NULL
 * in case of error.
 */
char	*grow_buffer(char *buf, size_t buf_size, size_t n_bytes)
{
	size_t	len;
	char	*new_buf;

	len = buf_size + n_bytes;
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

/* Copies the buffer to the next_line buffer and shifts read_buf
 *
 * If the next_line buffer has not yet been initialized it is first
 * initialized. Then the next_line buffer is grown to fit the new data.
 * Finally, the data from read_buf is concatenated to the next_line and any
 * possibly remaining characters in the read_buf are moved to the beginning
 * of read_buf.
 */
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
	new_nl = grow_buffer(next_line, ft_strlen(next_line) + 1, n_bytes);
	if (!new_nl)
		return (NULL);
	ft_strncat(new_nl, read_buf, n_bytes);
	ft_memcpy(read_buf, read_buf + n_bytes, BUFFER_SIZE - n_bytes);
	read_buf[BUFFER_SIZE - n_bytes] = 0;
	return (new_nl);
}

/* Reads the fd until a newline '\n' character is encountered
 *
 * The function keeps appending characters to the next_line buffer until
 * a newline character is encoutered. At that point the characters up to and
 * including the newline character are copied to the next_line buffer.
 * Returns the next_line buffer or NULL if reading fails.
 */
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

/* Resets the passed buffer and returs NULL 
 */
void	*reset_buf(char *buf)
{
	*buf = 0;
	return (NULL);
}

/* Gets the next line from an fd
 *
 * Reads the next line from a file descriptor. Reads from the buffer until a
 * newline is returned and returns all characters up to that point. Returns
 * NULL if in error is encoutered.
 */
char	*get_next_line(int fd)
{
	static char	read_buf[BUFFER_SIZE + 1];
	char		*next_line;
	char		*newline_pos;

	if (fd < 0 || BUFFER_SIZE < 1 || read(fd, NULL, 0) < 0)
		return (reset_buf(read_buf));
	next_line = NULL;
	newline_pos = ft_strchr(read_buf, '\n');
	if (!newline_pos)
	{
		next_line = read_until_nl(next_line, read_buf, fd);
		if (!next_line)
			return (NULL);
		newline_pos = ft_strchr(read_buf, '\n');
	}
	if (!newline_pos)
		newline_pos = read_buf + ft_strlen(read_buf) - 1;
	next_line = copy_buf_to_nl(next_line, read_buf, newline_pos - read_buf + 1);
	if (next_line != NULL && ft_strlen(next_line) == 0)
	{
		free(next_line);
		next_line = NULL;
	}
	return (next_line);
}
