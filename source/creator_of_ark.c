/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator_of_ark.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:48:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/03 16:43:31 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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
	if (var_name[0] == '"')
		return (var_name);
	free_and_null(var_name);
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

char	*set_var_name(char *str, char **env, int retval, int *i)
{
	char	*var_name;
	int		wc;

	wc = *i;
	while (str[*i] && str[*i] != ' ')
		(*i)++;
	if (str[*i] == ' ')
		(*i)--;
	var_name = ft_substr(str, wc + 1, (*i) - wc);
	if (!var_name)
		return (NULL);
	if (var_name[0] == '\0')
		return (free_and_null(var_name), NULL);
	if (!ft_strncmp(var_name, "?", 1))
	{
		if (var_name[1] != '\0')
			var_name = ft_strjoin_free(ft_itoa(retval), ft_substr_free(var_name, 1, ft_strlen(var_name) - 1));
		else
			var_name = ft_itoa(retval);
	}
	else
	{
		var_name = find_var_in_env(var_name, env);
		if (!var_name)
			return ("");
		if (var_name[0] == '"')
			return (var_name);
		var_name = trim_var_name(var_name);
	}
	return (var_name);
}

char	*remove_wildcard(char *str, char *var, int start_pos, int end_pos)
{
	char	*out;
	char	*substr;
	char	*ret;

	substr = ft_substr(str, 0, start_pos);
	if (!substr)
		return (NULL);
	out = ft_strjoin(substr, var);
	free_and_null(substr);
	free_and_null(var);
	if (!out)
		return (NULL);
	substr = ft_substr(str, end_pos, ft_strlen(str) - end_pos);
	if (!substr)
		return (NULL);
	ret = ft_strjoin(out, substr);
	if (!ret)
		return (NULL);
	if (substr)
		free_and_null(substr);
	if (out)
		free_and_null(out);
	return (ret);
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
	var_name = set_var_name(str, env, retval, &i);
	if (!var_name)
		return (str);
	if (var_name[0] == '\0')
		return (free_and_null(str), NULL);
	str = remove_wildcard(str, var_name, wc, i + 1);
	if (!str)
		return (NULL);
	if (ft_strlen(str) > i + 1 && has_char(&str[i + 1], '$') >= 0)
		str = ft_strjoin(ft_substr(str, 0, &str[i + 1] - str), handle_wildcard(&str[i + 1], env, retval));
	return (str);
}
