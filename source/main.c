/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:41 by roversch          #+#    #+#             */
/*   Updated: 2025/07/03 17:31:56 by nhendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

int	main(int argc, char **argv, char **envp)
{
	t_history	*hist;
	char		**environment;
	int			retval;

	environment = ft_matdup(envp);
	(void)argc;
	(void)argv;
	retval = 0;
	hist = init_hist();
	init_signals();
	while (1)
	{
		retval = shelly(&environment, retval, hist);
		if (retval == -1)
			break ;
	}
	free_array(environment);
	burn_history(hist);
}
