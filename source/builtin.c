/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:15:06 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/20 16:38:48 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	pwd(int fd)
{
	char *cwd;

	printf("pwd%i\n", fd);

	cwd = getcwd(NULL, 0);
	if(!cwd)
		return (0);
	else if (fd < 0)
		return (0);
	else
		ft_putendl_fd(cwd, fd);
	printf("pwd%i\n", fd);
	return (1);
}

int	echo(int fd, char **to_write)
{
	char	*out;
	int 	i;
	int		cmp;

	if (!to_write || !*to_write || fd < 0)
		return (0);
	out = NULL;
	cmp = ft_strncmp(*to_write, "-n", 3);
	i = 0;
	if (!cmp)
		i = 1;
	while (to_write[i])
	{
		out = ft_strjoin(out, to_write[i]);
		out = ft_strjoin(out, " ");
		i++;
	}
	if (cmp)
		out = ft_strjoin(out, "\n");
	ft_putstr_fd(out, fd);
	return (1);
}

int	cd(char *path)
{
	char	*cwd;
	char	*joined_path;

	if (!path)
		return (0);
	cwd = getcwd(NULL, 0);
	if(!cwd)
		return (0);
	if (ft_strncmp(cwd, path, ft_strlen(cwd)) && path[0] != '/')
	{
		joined_path = ft_strjoin(cwd, "/");
		joined_path = ft_strjoin(joined_path, path);
		if (!chdir(joined_path))
			return (0);
	}
	else
	{
		if (!chdir(path))
			return (0);
	}
	return (1);
}

// different type? (struct?)
// takes VAR name & str
// VAR name can be called using a wildcard -> look for array of these vars?
t_var *bi_export(char *var_name, char *str)
{
	t_var *var;

	if (!var_name || !str)
		return (NULL);
	var = malloc(sizeof(struct s_var));
	if (!var)
		return (NULL);
	var->name = var_name;
	var->content = str;
	return (var);
}

// works in sync with export, destroys a saved var
void	unset(char *name)
{
	if (name)
		unlink(name);
}

void bi_exit()
{
	//kill(getpid(), SIGTSTP);
}