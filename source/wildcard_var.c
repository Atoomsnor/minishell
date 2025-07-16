/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:59:31 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/14 12:17:01 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

static char	*find_and_trim_var(char *var_name, char **env, int gate)
{
	int	i;

	i = -1;
	if (!var_name)
		return (NULL);
	if (gate == 69)
	{
		while (env[++i])
			if (ft_strncmp(var_name, env[i], ft_strlen(var_name)) == 0
				&& env[i][ft_strlen(var_name)] == '=')
				return (free_and_null(var_name), env[i]);
		if (var_name[0] == '"')
			return (var_name);
		free_and_null(var_name);
	}
	else if (gate == 420)
	{
		while (var_name[++i])
			if (var_name[i] == '=')
				return (ft_substr(var_name, i + 1,
						ft_strlen(var_name) - i + 1));
	}
	return (NULL);
}

static char	*get_var(char *var_name, int retval, char **env)
{
	if (!ft_strncmp(var_name, "?", 1))
	{
		if (var_name[1] != '\0')
			var_name = ft_strjoin_free(ft_itoa(retval), ft_substr_free(var_name,
						1, ft_strlen(var_name) - 1), 3);
		else
		{
			free_and_null(var_name);
			var_name = ft_itoa(retval);
		}
		if (!var_name)
			return (NULL);
	}
	else
	{
		var_name = find_and_trim_var(var_name, env, 69);
		if (!var_name)
			return ("");
		if (var_name[0] == '"')
			return (var_name);
		var_name = find_and_trim_var(var_name, env, 420);
	}
	return (var_name);
}

char	*set_var_name(char *str, char **env, int retval, int *i)
{
	char	*var_name;
	int		wc;

	wc = *i;
	while (str[*i] && !ft_iswhitespace(str[*i])
		&& str[*i] != '\'' && str[*i] != '"' && (str[*i] != '$' || *i == wc))
		(*i)++;
	if (ft_iswhitespace(str[*i]) || str[*i] == '\''
		|| str[*i] == '"' || str[*i] == '$')
		(*i)--;
	var_name = ft_substr(str, wc + 1, (*i) - wc);
	if (!var_name)
		return (NULL);
	if (var_name[0] == '\0')
		return (free_and_null(var_name), NULL);
	var_name = get_var(var_name, retval, env);
	if (!var_name)
		return (NULL);
	(*i)++;
	return (var_name);
}
