/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:28:39 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/17 10:53:47 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

int	check_dir(char *str, char **error_msg, int i)
{
	DIR	*dir;

	if (!str)
		return (0);
	if (i || str[0] == '/' || !ft_strncmp(str, "./", 2))
	{
		dir = opendir(str);
		if (dir)
		{
			closedir(dir);
			if (error_msg)
				*error_msg = ft_strjoin(str, ": Is a directory\n");
			return (0);
		}
	}
	return (1);
}

int	is_buildin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	return (0);
}

static int	copy_from_next(t_input **cpy, t_input **next)
{
	ft_lstcopy(*cpy, (*cpy)->next);
	if (!(*cpy)->txt)
		return (0);
	*next = *cpy;
	ft_lstdelone((*cpy)->next);
	return (1);
}

t_input	**check_empty_txt(t_input **input)
{
	t_input	*cpy;
	t_input	*next;

	cpy = *input;
	while (cpy)
	{
		next = cpy->next;
		if (!cpy->txt)
		{
			if (cpy == *input && next)
			{
				if (!copy_from_next(&cpy, &next))
					return (malloc_error_free(shank_input(input)));
			}
			else if (cpy == *input && !next)
				return (NULL);
			else
				ft_lstdelone(cpy);
		}
		cpy = next;
	}
	if (!input || !(*input))
		return (NULL);
	return (input);
}

int	check_access(char *path, char **error_msg)
{
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) != 0 && !ft_strncmp(path, "./", 2))
		{
			if (*error_msg)
			{
				free(error_msg);
				error_msg = NULL;
			}
			*error_msg = ft_strjoin(path, " Permission denied\n");
			return (0);
		}
		else
			return (1);
	}
	return (1);
}
