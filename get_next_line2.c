/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjuneo-f <wjuneo-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 08:35:34 by wjuneo-f          #+#    #+#             */
/*   Updated: 2021/08/19 23:04:57 by wjuneo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 <files>.c

static void	free_ptr(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static char	*correct_line(char **backup_buffer)
{
	char	*line_correct;
	char	*free_temp;
	size_t	c;

	c = 0;
	while ((*backup_buffer)[c] != '\0' && (*backup_buffer)[c] != '\n')
		c++;
	free_temp = *backup_buffer;
	line_correct = ft_substr((char *)&(*backup_buffer), 0, c + 1);
	*backup_buffer = ft_strdup(&(*backup_buffer)[c + 1]);
	free_ptr(&free_temp);
	return (line_correct);
}

static char	*add_line(int fd, char **buffer, char **backup_buffer)
{
	char	*line;
	int		result_of_reading;
	size_t	correct;

	if (ft_strchr(*backup_buffer, '\n'))
			return (correct_line(backup_buffer));
	line = ft_strdup("");
-
	result_of_reading = 1;
	while (result_of_reading = read(fd, ))
	if (result_of_reading == 0 || result_of_reading < 0)
	{
		free_ptr(backup_buffer);
		return (NULL);
	}
	if (!ft_strchr(*buffer, '\n'))
	{
		temp = line;
		line = ft_strjoin(temp, *buffer);
		free(temp);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char *backup_buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!backup_buffer)
		backup_buffer = ft_strdup("");
	line = add_line(fd, &buffer, &backup_buffer);
	free_ptr(&buffer);
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
