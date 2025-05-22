// #include "../include/minishell.h"
// #include <fcntl.h>
// #include <sys/wait.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <stdio.h>

// /* Initialize a new command structure */
// t_cmd *init_cmd(void)
// {
// 	t_cmd *cmd = malloc(sizeof(t_cmd));
// 	if (!cmd)
// 		return NULL;
	
// 	cmd->args = NULL;
// 	cmd->in_fd = STDIN_FILENO;
// 	cmd->out_fd = STDOUT_FILENO;
// 	cmd->pipe[0] = -1;
// 	cmd->pipe[1] = -1;
// 	cmd->next = NULL;
	
// 	return cmd;
// }

// /* Add an argument to a command */
// void add_argument(t_cmd *cmd, char *arg)
// {
// 	int count = 0;
// 	if (cmd->args) {
// 		while (cmd->args[count]) 
// 			count++;
// 	}
	
// 	char **new_args = realloc(cmd->args, sizeof(char *) * (count + 2));
// 	if (!new_args)
// 		handle_error("Memory allocation failed");
	
// 	new_args[count] = ft_strdup(arg);
// 	new_args[count + 1] = NULL;
// 	cmd->args = new_args;
// }

// /* Parse input tokens into commands */
// t_cmd *parse_cmds(t_input *input)
// {
// 	t_cmd *head = NULL;
// 	t_cmd *current = NULL;
// 	t_input *tmp = input;
	
// 	while (tmp) {
// 		if (!head) {
// 			head = init_cmd();
// 			current = head;
// 		}
// 		else if (tmp->type == t_pipe) {
// 			current->next = init_cmd();
// 			current = current->next;
// 			tmp = tmp->next;
// 			continue;
// 		}
		
// 		/* Handle redirections */
// 		if (tmp->type == t_left) {
// 			current->in_fd = open(tmp->next->txt, O_RDONLY);
// 			if (current->in_fd < 0)
// 				handle_error("Open input failed");
// 			tmp = tmp->next->next;
// 		}
// 		else if (tmp->type == t_right) {
// 			current->out_fd = open(tmp->next->txt, O_WRONLY|O_CREAT|O_TRUNC, 0644);
// 			if (current->out_fd < 0)
// 				handle_error("Open output failed");
// 			tmp = tmp->next->next;
// 		}
// 		else if (tmp->type == t_append) {
// 			current->out_fd = open(tmp->next->txt, O_WRONLY|O_CREAT|O_APPEND, 0644);
// 			if (current->out_fd < 0)
// 				handle_error("Open append failed");
// 			tmp = tmp->next->next;
// 		}
// 		/* Add regular arguments */
// 		else if (tmp->type == t_txt || tmp->type == t_flag) {
// 			add_argument(current, tmp->txt);
// 			tmp = tmp->next;
// 		}
// 		else {
// 			tmp = tmp->next;
// 		}
// 	}
// 	return head;
// }

// static void setup_redirections(t_cmd *cmd, int prev_pipe_in)
// {
// 	/* Handle input from previous pipe */
// 	if (prev_pipe_in != -1) {
// 		dup2(prev_pipe_in, STDIN_FILENO);
// 		close(prev_pipe_in);
// 	}
	
// 	/* Handle file redirections */
// 	if (cmd->in_fd != STDIN_FILENO) {
// 		dup2(cmd->in_fd, STDIN_FILENO);
// 		close(cmd->in_fd);
// 	}
// 	if (cmd->out_fd != STDOUT_FILENO) {
// 		dup2(cmd->out_fd, STDOUT_FILENO);
// 		close(cmd->out_fd);
// 	}
	
// 	/* Handle output to next pipe */
// 	if (cmd->next) {
// 		dup2(cmd->pipe[1], STDOUT_FILENO);
// 	}
	
// 	/* Close all pipe ends */
// 	close(cmd->pipe[0]);
// 	close(cmd->pipe[1]);
// }

// /* Set up file descriptors for command execution */
// static void execute_pipeline(t_cmd *cmd)
// {
// 	int prev_pipe_in = -1;
// 	pid_t pid;
	
// 	while (cmd) {
// 		if (cmd->next && pipe(cmd->pipe) == -1)
// 			handle_error("pipe failed");
		
// 		pid = fork();
// 		if (pid == -1)
// 			handle_error("fork failed");
		
// 		if (pid == 0) { /* Child process */
// 			setup_redirections(cmd, prev_pipe_in);
// 			execvp(cmd->args[0], cmd->args);
// 			handle_error("exec failed");
// 		}
// 		else { /* Parent process */
// 			/* Close previous pipe input if it exists */
// 			if (prev_pipe_in != -1)
// 				close(prev_pipe_in);
				
// 			/* Save the read end for next command */
// 			if (cmd->next) {
// 				close(cmd->pipe[1]); /* Close write end in parent */
// 				prev_pipe_in = cmd->pipe[0]; /* Save read end for next */
// 			}
			
// 			/* Close file descriptors */
// 			if (cmd->in_fd != STDIN_FILENO)
// 				close(cmd->in_fd);
// 			if (cmd->out_fd != STDOUT_FILENO)
// 				close(cmd->out_fd);
			
// 			cmd = cmd->next;
// 		}
// 	}
	
// 	/* Wait for all children */
// 	while (waitpid(-1, NULL, 0) > 0);
// }

// /* Execute external commands */
// void execute_cmds(t_shell *shell)
// {
// 	t_cmd *cmd_list = parse_cmds(*shell->curr_input);
// 	if (!cmd_list || !cmd_list->args)
// 		return;
	
// 	execute_pipeline(cmd_list);
	
// 	/* TODO: Free cmd_list when implemented */
// }

// /* Error handling */
// void handle_error(const char *msg)
// {
// 	perror(msg);
// 	exit(0);
// }