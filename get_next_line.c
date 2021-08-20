/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjuneo-f <wjuneo-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 01:31:23 by wjuneo-f          #+#    #+#             */
/*   Updated: 2021/08/19 12:05:32 by wjuneo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 <files>.c.

static int	verify_line(char *buffer)
{
	while (*buffer)
	{
		if (*buffer == '\n')
			return (1);
		buffer++;
	}
	return (0);
}

static size_t	termine_line(char *buffer)
{
	size_t	corretor;

	corretor = 0;
	while(buffer[corretor] != '\n')
	{
		corretor++;
	}
	return (corretor + 1);
}

static char *ft_resto(char *buffer)
{
	char	*resto;
	size_t	i;

	i = 0;
	while(*buffer != '\n')
	{
		buffer++;
		i++;
	}
	buffer++;
	resto = ft_strdup(buffer);
	return (resto);
}

char	*get_next_line(int fd)
{
	int BUFFER_SIZE = 15;

	char 		*line;
	static char *resto;
	char		*buffer;
	size_t		size;
	size_t		corretor;
	int			reads;

	reads = 1;
	line = ft_strdup("");
	if (!resto)
		resto = ft_strdup("");
	line = ft_strjoin(line, resto);
	size = BUFFER_SIZE;
	buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);

	if (verify_line(line))
	{
		corretor = termine_line(line);
		resto = ft_resto(line);
		line = ft_substr(line, 0, corretor);
		free(resto);
		return (line);
	}
	while (reads)
	{
		reads = read(fd, buffer, size);
		if (reads != 0)
		{
			buffer[reads] = '\0';
			if (!verify_line(buffer))
			{
				line = ft_strjoin(line, buffer);
			}
			else if(verify_line(buffer))
			{
				corretor = termine_line(buffer);
				resto = ft_resto(buffer);
				buffer = ft_substr(buffer, 0, corretor);
				line = ft_strjoin(line, buffer);
				break;
			}
		}
	}
	free(buffer);
	if (!*line)
	{
		if (resto)
			free(resto);
		free(line);
		return (NULL);
	}
	return (line);
}


int main(int argc, char **argv)
{
	char *str;
	int fd;
	argc++;

	fd = open (argv[1], O_RDONLY);
	str = get_next_line(fd);
	while (str)
	{
		printf("%s", str);
		free(str);
		str = get_next_line(fd);
	}
	return (0);
}
