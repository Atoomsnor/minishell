/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator_of_ark.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:48:53 by nhendrik          #+#    #+#             */
/*   Updated: 2025/05/28 14:35:44 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h>


// char *find_var_in_env(char *var_name, char **env)
// {
// 	int i;
// 	int	j;

// 	i = 0;
// 	while (env[i])
// 	{
// 		if (ft_strncmp(var_name, env[i], ft_strlen(var_name)) == 0)
// 			break ; 
// 		i++;
// 	}
// 	if (!env[i])
// 		return (NULL);
// 	j = 0;
// 	while(env[i][j])
// 	{
// 		if (env[i][j] == '=')
// 			return (ft_substr(env[i], j + 1, ft_strlen(env[i]) - j + 1));
// 		j++;
// 	}
// 	return (NULL);
// }

char *find_var_in_env(char *var_name, char **env)
{
	int i;

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

char *trim_var_name(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (NULL);
	while(str[i])
	{
		if (str[i] == '=')
			return (ft_substr(str, i + 1, ft_strlen(str) - i + 1));
		i++;
	}
	return (NULL);
}

char *remove_wildcard(char *str, char *var, int start_pos, int end_pos)
{
	char *out;

	out = ft_strjoin(ft_substr(str, 0, start_pos), var);
	out = ft_strjoin(out, ft_substr(str, end_pos, ft_strlen(str) - end_pos));
	return (out);
}

char *handle_wildcard(char *str, char **env, int retval)
{
	int i;
	int wc;
	char *var_name;

	i = 0;
	if (!str)
		return (NULL);
	while(str[i] && str[i] != '$')
		i++;
	if (!str[i])
		return (NULL);
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
	printf("var_name %s\n", var_name);
	str = remove_wildcard(str, var_name, wc, i);
	return (str);
}