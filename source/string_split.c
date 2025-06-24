/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:49:11 by nhendrik          #+#    #+#             */
/*   Updated: 2025/06/24 18:32:27 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h>

static int	ft_count(char const *s, char c)
{
	int	nb;
	int	i;
	int quote;

	nb = 0;
	i = 0;
	quote = 0;
	while (s[i])
	{
		if (!quote && s[i] == '\'')
			quote = '\'';
		else if (!quote && s[i] == '"')
			quote = '"';
		else if (s[i] == quote)
		{
			nb++;
			quote = 0;
		}
		else if (s[i] != c && (i == 0 || s[i - 1] == c) && !quote)
			nb++;
		i++;
	}
	return (nb);
}

static int	ft_len(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
	{
		if (s[i] == '\'' && c != '"' && s[i] != c)
			i += ft_len(&s[i + 1], '\'') + 2;
		else if (s[i] == '\"' && c != '\'' && s[i] != c)
			i += ft_len(&s[i + 1], '"') + 2;
		else
			i++;
	}
	return (i);
}

// static char	*another_one(char const *s, char c, int *i)
// {
// 	char	*ret;
// 	int		w_len;

// 	ret = NULL;
// 	w_len = *i;
// 	if ((has_char((char *)&s[*i], '\'') >= 0 && s[*i] != '\'') || (s[*i] != '"' && has_char((char *)&s[*i], '"') >= 0))
// 	{
// 		while (s[w_len] && s[w_len] != '\'' && s[w_len] != '"' && s[w_len] != c)
// 			w_len++;
// 		ret = ft_substr(s, *i, w_len);
// 		if (!ret)
// 			return (free(ret), NULL);
// 		*i += w_len;
// 	}
// 	else
// 	{
// 		w_len = ft_len(&s[*i], c);
// 		ret = ft_substr(s, *i, w_len);
// 		if (!ret)
// 			return (free(ret), NULL);
// 		*i += w_len;
// 	}
// 	return (ret);
// }

static char	*another_one(char const *s, char c, int *i)
{
	char	*ret;
	int		w_len;

	ret = NULL;
	w_len = ft_len(&s[*i], c);
	ret = ft_substr(s, *i, w_len);
	if (!ret)
		return (free(ret), NULL);
	*i += w_len;
	return (ret);
}

// static int	ft_len(char const *s, char c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i] && s[i] != c)
// 		i++;
// 	return (i);
// }

// static char	*another_one(char const *s, char c, int *i)
// {
// 	char	*ret;
// 	int		w_len;

// 	ret = NULL;
// 	w_len = 0;
// 	if (s[*i] == '"')
// 	{
// 		w_len = ft_len(&s[*i + 1], '"');
// 		ret = ft_substr(s, *i, w_len + 2);
// 		if (!ret)
// 			return (free(ret), NULL);
// 		*i += w_len + 2;
// 		if (s[*i] != c)
// 			ret = ft_strjoin(ret, another_one(s, c, i));
// 	}
// 	else if (s[*i] == '\'')
// 	{
// 		w_len = ft_len(&s[*i + 1], '\'');
// 		ret = ft_substr(s, *i, w_len + 2);
// 		if (!ret)
// 			return (free(ret), NULL);
// 		*i += w_len + 2;
// 		if (s[*i] != c)
// 			ret = ft_strjoin(ret, another_one(s, c, i));
// 	}
// 	else
// 	{
// 		w_len = ft_len(&s[*i], c);
// 		ret = ft_substr(s, *i, w_len);
// 		if (!ret)
// 			return (free(ret), NULL);
// 		*i += w_len;
// 	}
// 	return (ret);
// }

static char	**ft_bigsplit(char const *s, char c, char **n_str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!n_str)
		return (NULL);
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			n_str[j] = another_one(s, c, &i);
			if (!n_str[j])
				return (free_array(n_str), NULL);
			j++;
		}
	}
	n_str[j] = NULL;
	return (n_str);
}

// version of ft_split that preserves anything in quotes
char	**ft_string_split(char const *s, char c)
{
	char	**n_str;

	if (!s)
		return (NULL);
	n_str = ft_calloc(sizeof(char *), ft_count(s, c) + 1);
	if (!n_str)
		return (NULL);
	return (ft_bigsplit(s, c, n_str));
}
