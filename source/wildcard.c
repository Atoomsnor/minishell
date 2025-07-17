/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:48:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/07/17 12:14:19 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

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
	if (var && (var[0] != '\0' || !recur))
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
	free_and_null(substr);
	free_and_null(out);
	return (ret);
}

static int	get_positions(int pos[3], char *str, int recur)
{
	pos[0] = 0;
	while (str && str[pos[0]] && str[pos[0]] != '$')
		pos[0]++;
	if (!str || !str[pos[0]])
		return (0);
	pos[1] = pos[0];
	pos[2] = recur;
	return (1);
}

static char	*recursive_wildcard(char *str, int pos[3], char **env, int retval)
{
	char	*ret;

	ret = NULL;
	if (!str[pos[1] + 1] || str[pos[1] + 1] == '"'
		|| str[pos[1] + 1] == '\'' || ft_iswhitespace(str[pos[1] + 1]))
		ret = ft_strjoin_free(ft_substr(str, 0, pos[0]),
				ft_substr(str, pos[1], ft_strlen(str) - pos[1]), 3);
	else
		ret = ft_strjoin_free(ft_substr(str, 0, &str[pos[0] + 1] - str - 1),
				handle_wildcard(&str[pos[0 + 1]], env, retval,
					pos[2] + 1), 3);
	if (str && !pos[2])
		free_and_null(str);
	return (ret);
}

static char	*varcheck_wildcard(char *str, char **env, int retval, int pos[3])
{
	if (has_char(&str[pos[0] + 1], '$') >= 0)
		return (recursive_wildcard(str, pos, env, retval));
	if (!pos[2] && pos[0])
		return (ft_strjoin_free(ft_substr(str, 0, pos[0]),
				ft_substr_free(str, pos[1], ft_strlen(str) - pos[0] - 1), 3));
	if (!pos[2])
		return (free_and_null(str));
	return (str);
}

char	*handle_wildcard(char *str, char **env, int retval, int recur)
{
	char	*var_name;
	char	*ret;
	int		pos[3];

	if (!str || !get_positions(pos, str, recur))
		return (NULL);
	ret = NULL;
	var_name = set_var_name(str, env, retval, &pos[1]);
	if (!var_name)
		return (str);
	if (var_name[0] == '\0')
		return (varcheck_wildcard(str, env, retval, pos));
	str = remove_wildcard(str, var_name, pos, recur);
	if (!str)
		return (NULL);
	if (has_char(&str[pos[0]], '$') >= 0)
		ret = handle_wildcard(str, env, retval, recur + 1);
	if (ret && ret != str)
		return (free_and_null(str), ret);
	return (str);
}
