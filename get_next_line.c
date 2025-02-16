/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyolchy <miyolchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:28:53 by miyolchy          #+#    #+#             */
/*   Updated: 2025/02/16 18:48:24 by miyolchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*set_remainder(char **line)
{
	char	*remainder;
	char	*new_line;
	size_t	i;

	if (!*line)
		return (NULL);
	i = 0;
	while ((*line)[i] && (*line)[i] != '\n')
		++i;
	if ((*line)[i] == '\0' || (*line)[i + 1] == '\0')
		return (NULL);
	remainder = ft_substr(*line, i + 1, ft_strlen(*line) - i - 1);
	new_line = ft_substr(*line, 0, i + 1);
	free(*line);
	*line = new_line;
	if (!*remainder || *remainder == '\0')
	{
		free(remainder);
		remainder = NULL;
	}
	return (remainder);
}

static char	*fill_line(int fd, char *buffer, char *remainder)
{
	char	*tmp;
	ssize_t	read_bytes;

	read_bytes = 1;
	while (read_bytes > 0)
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
		if (!remainder)
			remainder = ft_strdup("");
		tmp = remainder;
		remainder = ft_strjoin(tmp, buffer);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (remainder);
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;
	char		buffer[BUFFER_SIZE + 1];

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(remainder);
		remainder = NULL;
		return (NULL);
	}
	line = fill_line(fd, buffer, remainder);
	if (!line || *line == '\0')
		return (NULL);
	remainder = set_remainder(&line);
	return (line);
}

//# include <fcntl.h>
//
// int main(void)
// {
// 	int fd = open("read_error.txt", O_RDONLY);

// 	if (fd == -1)
// 	{
// 		printf("Error\n");
// 		return (1);
// 	}
// 	char *str;
// 	while ((str = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s", str);
// 		free(str);
// 	}
// 	close (fd);
// 	return (0);
// }