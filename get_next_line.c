/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wjuneo-f <wjuneo-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 10:15:21 by wjuneo-f          #+#    #+#             */
/*   Updated: 2021/08/23 13:46:07 by wjuneo-f         ###   ########.fr       */
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

static char	*correct_line(char **backup_buffer)
{
	char	*free_temp;
	char	*line;
	size_t	c;

	c = 0;
	while ((*backup_buffer)[c] != '\0' && (*backup_buffer)[c] != '\n')
		c++;
	if ((*backup_buffer)[c] == '\n')
		c++;
	free_temp = *backup_buffer;
	line = ft_substr(free_temp, 0, c);
	*backup_buffer = ft_strdup(&(*backup_buffer)[c]);
	free_ptr(&free_temp);
	return (line);
}

static char	*add_line(int fd, char **buffer, char **backup_buffer)
{
	char	*temp_free;
	int		result_of_reading;

	if (ft_strchr(*backup_buffer, '\n'))
		return (correct_line(backup_buffer));
	result_of_reading = 1;
	while (!ft_strchr(*backup_buffer, '\n') && result_of_reading)
	{
		result_of_reading = read(fd, *buffer, BUFFER_SIZE);
		if (result_of_reading == -1)
		{
			free_ptr(backup_buffer);
			return (NULL);
		}
		(*buffer)[result_of_reading] = '\0';
		temp_free = *backup_buffer;
		*backup_buffer = ft_strjoin(temp_free, *buffer);
		free_ptr(&temp_free);
	}
	if (result_of_reading == 0 && !**backup_buffer)
	{
		free_ptr(backup_buffer);
		return (NULL);
	}
	if (ft_strchr(*backup_buffer, '\n'))
		return (correct_line(backup_buffer));

	if (!ft_strchr(*backup_buffer, '\n') && **backup_buffer)
		return (correct_line(backup_buffer));

	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*backup_buffer;
	char		*line;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!backup_buffer)
		backup_buffer = ft_strdup("");
	line = add_line(fd, &buffer, &backup_buffer);
	free_ptr(&buffer);
	return (line);
}

// int main(void)
// {
// 	char	*str;
// 	int		fd;

// 	fd = open("arquivo.txt", O_RDONLY);
// 	while(1)
//     {
//         str = get_next_line(fd);
//         if (!str)
//         	break ;
//         printf("%s", str);
//         free(str);
//     }
// 	return (0);
// }
