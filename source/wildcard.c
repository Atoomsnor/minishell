/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:48:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/10 15:40:14 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

char	*find_and_trim_var(char *var_name, char **env, int gate)
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

static char	*set_var_name(char *str, char **env, int retval, int *i)
{
	char	*var_name;
	int		wc;

	wc = *i;
	while (str[*i] && !ft_iswhitespace(str[*i])
		&& str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	if (ft_iswhitespace(str[*i]) || str[*i] == '\'' || str[*i] == '"')
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

static char	*remove_wildcard(char *str, char *var, int pos[2], int recur)
{
	char	*substr;
	char	*out;
	char	*ret;

	substr = ft_substr(str, 0, pos[0]);
	if (!substr)
		return (NULL);
	out = ft_strjoin(substr, var);
	free_and_null(substr);
	free_and_null(var);
	if (!out)
		return (NULL);
	substr = ft_substr(str, pos[1], ft_strlen(str) - pos[1]);
	if (!substr)
		return (NULL);
	if (str && !recur)
		free_and_null(str);
	ret = ft_strjoin(out, substr);
	if (!ret)
		return (NULL);
	if (substr)
		free_and_null(substr);
	if (out)
		free_and_null(out);
	return (ret);
}

char	*handle_wildcard(char *str, char **env, int retval, int recur)
{
	char	*var_name;
	char	*ret;
	int		pos[2];

	pos[0] = 0;
	ret = NULL;
	while (str && str[pos[0]] && str[pos[0]] != '$')
		pos[0]++;
	if (!str && !str[pos[0]])
		return (NULL);
	pos[1] = pos[0];
	var_name = set_var_name(str, env, retval, &pos[1]);
	if (!var_name)
		return (str);
	if (var_name[0] == '\0')
		return (free_and_null(str), NULL);
	str = remove_wildcard(str, var_name, pos, recur);
	if (!str)
		return (NULL);
	if (ft_strlen(str) > pos[0] + 1 && has_char(&str[pos[0] + 1], '$') >= 0)
		ret = ft_strjoin_free(ft_substr(str, 0, &str[pos[0] + 1] - str),
				handle_wildcard(&str[pos[0] + 1], env, retval, recur + 1), 3);
	if (ret)
		return (free_and_null(str), ret);
	return (str);
}
