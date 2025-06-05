/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator_of_ark.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:48:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/05 17:52:25 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h>

char	*find_var_in_env(char *var_name, char **env)
{
	int	i;

	i = 0;
	if (!var_name)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(var_name, env[i], ft_strlen(var_name)) == 0)
			return (env[i]);
		i++;
	}
	return (NULL);
}

char	*trim_var_name(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '=')
			return (ft_substr(str, i + 1, ft_strlen(str) - i + 1));
		i++;
	}
	return (NULL);
}

char	*set_var_name(char *str, char **env, int retval, int i)
{
	char	*var_name;
	int		wc;

	wc = i;
	while (str[i] && str[i] != ' ')
		i++;
	if (str[i] == ' ')
		i--;
	var_name = ft_substr(str, wc + 1, i - wc);
	if (!var_name)
		return (NULL);
	if (var_name[0] == '\0')
		return (str);
	if (!ft_strncmp(var_name, "?", 2))
		var_name = ft_itoa(retval);
	else
	{
		var_name = find_var_in_env(var_name, env);
		if (!var_name)
			return (NULL);
		var_name = trim_var_name(var_name);
	}
	return (var_name);
}

char	*remove_wildcard(char *str, char *var, int start_pos, int end_pos)
{
	char	*out;

	out = ft_strjoin(ft_substr(str, 0, start_pos), var);
	out = ft_strjoin(out, ft_substr(str, end_pos, ft_strlen(str) - end_pos));
	return (out);
}

char	*handle_wildcard(char *str, char **env, int retval)
{
	char	*var_name;
	int		i;
	int		wc;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != '$')
		i++;
	if (!str[i])
		return (NULL);
	wc = i;
	var_name = set_var_name(str, env, retval, i);
	printf("var_name %s\n", var_name);
	str = remove_wildcard(str, var_name, wc, i);
	return (str);
}
