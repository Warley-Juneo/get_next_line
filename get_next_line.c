/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjuneo-f <wjuneo-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 12:31:24 by wjuneo-f          #+#    #+#             */
/*   Updated: 2021/08/20 16:31:44 by wjuneo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void	free_ptr(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static char	*correct_line(char **buffer_backup)
{
	char	*line_correct;
	char	*free_temp;
	size_t	c;

	c = 0;
	while ((*buffer_backup)[c] != '\0' && (*buffer_backup)[c] != '\n')
		c++;
	free_temp = *buffer_backup;
	line_correct = ft_substr(free_temp, 0, c + 1);
	*buffer_backup = ft_strdup(&(*buffer_backup)[c + 1]);
	free_ptr(&free_temp);
	return (line_correct);
}

static char	*treat_line(int fd, char **buffer, char **backup_buffer)
{
	char	*free_temp;
	int		joao;

	joao = fd;
	while(joao)
	{
		joao = read(fd, *buffer, BUFFER_SIZE);
		*buffer[joao] = '\0';
		free_temp = *backup_buffer;
		*backup_buffer = ft_strjoin(*backup_buffer, *buffer);
		if (ft_strchr(*backup_buffer, '\n'))
		{
			free_ptr(&free_temp);
			return (correct_line(backup_buffer));
		}
		free_ptr(&free_temp);
	}
	return (NULL);
}

static char	*extratc_line(int fd, char **buffer, char **backup_buffer)
{
	char	*free_temp;

	free_temp = *backup_buffer;
	*backup_buffer = ft_strjoin(*backup_buffer, *buffer);
	free_ptr(&free_temp);
	if (ft_strchr(*backup_buffer, '\n'))
	{
		return (correct_line(backup_buffer));
	}
	else if (!ft_strchr(*backup_buffer, '\n'))
	{
		return (treat_line(fd, &(*buffer), &(*backup_buffer)));
	}
	return (NULL);
}

static char	*add_line(int fd, char **buffer, char **backup_buffer)
{
	int		result_of_reading;

	if (ft_strchr(*backup_buffer, '\n'))
		return (correct_line(backup_buffer));
	result_of_reading = read(fd, *buffer, BUFFER_SIZE);
//	*buffer[result_of_reading] = '\0';
	if (result_of_reading <= 0)
		return (NULL);
	if (result_of_reading)
		return (extratc_line(fd, &(*buffer), &(*backup_buffer)));

	return (NULL);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*buffer_backup;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!buffer_backup)
		buffer_backup = ft_strdup("");
	line = add_line(fd, &buffer, &buffer_backup);
	free_ptr(&buffer);
	if (!line && !*buffer_backup)
		free_ptr(&buffer_backup);
	return (line);
}

int main(int argc, char **argv)
{
	char	*str;
	int		fd;
	argc++;

	fd = open(argv[1], O_RDONLY);
	str = get_next_line(fd);
	while (str)
	{
		printf("%s", str);
		free(str);
		str = get_next_line(fd);
	}
	return (0);
}
