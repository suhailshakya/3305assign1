/******************************************************************************
* 
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* shell.c processes the users input, passes it to a tokenizer, and then passes
* the tokens obtained to the process_commands function which will process and
* execute the commands.
*
******************************************************************************/

#include "shell.h"
#define STDIN_FILENO 0
#define STDOUT_FILENO 1

/******************************************************************************
* Processes the input and determine whether it is a user interface operation 
* or a set of commands that will need to be executed.
******************************************************************************/
void shell(char* filename)
{

	/**************************************************************************
	* short			special_char 	determines whether the character to be 
	*								processed is special or not.
	* int 			len 			keeps track of the current line length
	* char 			line 			holds current line
	**************************************************************************/
	short special_char = FALSE;
	int status, len = 0;
	char ch, *line = (char*)malloc(MAX_LEN);
	FILE *fp = NULL;

	if (filename != NULL)
	{
		fp = fopen(filename, READ);

		if (fp == NULL) printf("Unable to open %s\n", filename);

	}
	

	/**************************************************************************
	* Loops until the user exits the program.
	**************************************************************************/
	print_user();
	while(TRUE)
	{

		ch = getch(fp);
			
		if (special_char)
		{
			special_char = FALSE;
			ch = switch_keypad(ch);
		}

		/**********************************************************************
		* switchs arround possible cases depending on the read character
		**********************************************************************/
		switch(ch)
		{
			/******************************************************************
			* handles the ascii translation of arrow characters
			******************************************************************/
			case '\033':
				getch(NULL);
				special_char = TRUE;
				continue;
				break;

			/******************************************************************
			* ignore arrow characters and tab
			******************************************************************/
			case KEYLEFT:
			case KEYRIGHT:
			case KEYUP:
			case KEYDOWN:
			case '\t':
				break;

			/******************************************************************
			* handles backspacing
			******************************************************************/
			case DELETE:
			{
				if (len > 0) 
					delete(--len, line);
				break;
			}

			/******************************************************************
			* if the maximum line length is not exceeded the program will print
			* the character. if the character is not a new line then continue. 
			* Else, terminate line, pass it to the execute_commands methond,
			* allocate a new line, and reset the line length. 
			******************************************************************/
			default:
			{
				if (len < MAX_LEN)
				{

					if (ch != '\n')
					{
						printf("%c", ch);
						line[len++] = ch;
					}

					else if (ch == '\n' && len > 0)
					{
						printf("%c", ch);
						line[len] = '\0';
						status = execute_commands(line);
						
						free(line);
						if (status == TERMINATE) exit(SUCCESS);

						line = (char*)malloc(MAX_LEN);
						len = 0;

						print_user();
					}
				}
				break;
			}
		}
	}

	fclose(fp);
}

/******************************************************************************
* execute_commands will process and execute the commands in the variable line.
******************************************************************************/
short execute_commands(char* line)
{
	//count the size of the array from the textfile
	int j = 0;
	while (line[j]){
		j++;
	}

		short status;
		
		//exit command
		if (strcmp(line, "exit") == 0){
			exit(0);
		}
		else{

			char **array = malloc(sizeof(char* ) * j);
			//char **array2 = malloc(sizeof(char*) * j);

			char *tokens;
			//char *tokens2;
			int i = 0;
			int p = 0;
			tokens = strtok(line, " ");
			//tokens2 = strtok(line, "|><");

			while(tokens != NULL){
				array[i] = tokens;
				printf("%s\n", tokens);
				tokens = strtok(NULL, " ");
				i++;
			}
			array[i] = NULL;
			/**while(tokens2 != NULL){
				array2[p++] = tokens;
				tokens2 = strtok(NULL, " ");
				printf("%s\n", tokens2);
			}**/

			//create pipe
			int fd[2];

			if (pipe(fd) < 0){
				perror("pipe error");
			}
			//successful pipe created
			int size = 0;

			while (size < sizeof(array)/sizeof(array[0])){
				if (strcmp(array[i], "|") == 0){
					pid_t pid = fork();
					
					if(pid < 0){
						perror("Problem forking");
						exit(1);
					}
					else if(pid > 0){
						//parent process
						close(fd[0]);
						
						if(dup2(fd[1], STDOUT_FILENO) < 0){
							perror("Can't dup");
							exit(1);			
						}
						//execvp(array[i], array);
						execlp(array[i], array[i], NULL);
						perror("exec problem");
						exit(1);
					}
					else{
						close(fd[1]);
					
						if(dup2(fd[0], STDIN_FILENO) < 0){
							perror("Can't dup");
							exit(1);
						}
						execvp(array[i], array);
						perror("Exec problem");
						exit(1);
					}
					return 0;
				}
				i++;
			}
		status = 0;	
		return status;
	}	
}
	/*	if ((strcmp(array[i], "<") == 0){
			dup2(file, stdoutFile);//create dup2 to output to file
			close(file); 
		
			//fork
			pid_t pid;
			pid = fork();
			if (pid < 0) {
				perror("Problem forking");
				exit(1);
			} 
			else if (pid > 0) {
				//parent process 
				perror("exec problem");
				close(fd[0]);
				exit(1);
			}
			else {
				//child process
				execvp(array[i], array);
				perror("exec problem");
				close(fd[1]);
				exit(1);
			}

			close(fd);
		}
		if ((strcmp(array[i], ">") == 0){
			
		}
		/*if ((strcmp(array[i], "<") == 0) && (strcmp(array[i], "|") == 0)
		if ((strcmp(array[i], ">") == 0) && (strcmp(array[i], "|") == 0)
		if ((strcmp(array[i], "<") == 0) && (strcmp(array[i], ">") == 0)
		if ((strcmp(array[i], "<") == 0) && (strcmp(array[i], ">") == 0) && (strcmp(array[i], "|") == 0)*/




