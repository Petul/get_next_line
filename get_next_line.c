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

/* Reads bytes from the specified fd and stores them
* in the buf. Returns amount of bytes read from the fd */
int read_to_buf(int fd, char *buf)
{
	int bytes_read;

	bytes_read = read(fd, buf, BUFFER_SIZE);
	return (bytes_read);

}

char	*increase_buffer(char *buf, size_t n_bytes)
{
	int		len;
	char	*new_buf;

	len = ft_strlen(buf);
	new_buf = malloc(sizeof(char) * (len + n_bytes));
	ft_memcpy(new_buf, buf, len-1);
	free(buf);
	return (new_buf);
}

char	*get_next_line(int fd)
{
	static char read_buf[BUFFER_SIZE];
	char		*next_line;
	char		*newline;
	
	if (read_to_buf(fd, read_buf) <= 0)
		return (NULL);
	newline = ft_strrchr(read_buf, '\n');
			
	return (NULL);
}
