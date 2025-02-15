/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:28:53 by miyolchy          #+#    #+#             */
/*   Updated: 2025/02/15 17:39:26 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*set_remainder(char *line)
{
	char	*remainder;
	size_t	i;

	i = 0;
	while (line[i] != '\0' && line[i] != '\n')
		++i;
	if (line[i] == '\0' || line[i + 1] == '\0')
		return (NULL);
	remainder = ft_substr(line, i + 1, ft_strlen(line) - i + 1);
	if (*remainder == '\0')
	{
		free(remainder);
		remainder = NULL;
	}
	line[i + 1] = '\0';
	return (remainder);
}

static char	*fill_line(int fd, char *buffer, char *remainder)
{
	char	*tmp;
	ssize_t	read_bytes;

	if (!remainder)
		remainder = ft_strdup("");
	read_bytes = 1;
	while (read_bytes > 0 && !ft_strchr(remainder, '\n'))
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(remainder);
			return (NULL);
		}
		if (read_bytes == 0)
			break ;
		buffer[read_bytes] = '\0';
		tmp = remainder;
		remainder = ft_strjoin(tmp, buffer);
		free(tmp);
	}
	return (remainder);
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(remainder);
		remainder = NULL;
		return (NULL);
	}
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	line = fill_line(fd, buffer, remainder);
	free(buffer);
	if (!line || *line == '\0')
	{
		free(line);
		return (NULL);
	}
	remainder = set_remainder(line);
	return (line);
}
