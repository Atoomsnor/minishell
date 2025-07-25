/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roversch <roversch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/07/17 12:23:00 by roversch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_history	*hist;
	char		**environment;
	static int	retval = 0;
	int			ret;

	(void)argc;
	(void)argv;
	environment = ft_matdup(envp);
	if (!environment)
		return (malloc_error_free(NULL), 0);
	hist = init_hist();
	if (!hist)
		return (malloc_error_free(free_array(environment)), 0);
	init_signals();
	while (1)
	{
		ret = shelly(&environment, retval, hist);
		if (ret == -1)
			break ;
		retval = ret;
	}
	if (environment)
		free_array(environment);
	burn_history(hist);
	return (retval);
}
