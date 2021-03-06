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
	//exit command
	if (strcmp(line, "exit") == 0){
		exit(0);
	}

	short status;
	int j = 0;

	while(j < line[j]){
		j++;
	}

	char **array = (char **)malloc(sizeof(char *) * (j+1));
	//char **array2 = (char **)malloc(sizeof(char *) * (j+1));
	char *tokens = strtok(line, " ");
	//char *tokens2
	int i = 0, p = 0;
	int count = 0;
	while (tokens != NULL){
		array[i] = tokens;
		printf("Command: %s\n", tokens);
		tokens = strtok(NULL, " ");
		i++;
		//counts how many commands and files are inputted
		count++;
	}
	array[i] = NULL;
	int u = 0;
	for (u; u < 5; u++){
		printf("array:  %s", &array);
	}
	//("array: %s\n   %s\n", *array, array[1]);

	//pipe
	int fd[2];
	pid_t pid;

	//create pipe
	if (pipe(fd) < 0){
		perror("pipe error");
	}

	int size = 0;
	int sizeArray = count;
	int v = 0;

	while (size < sizeArray){
		
		//create a fork
		pid = fork();

		if (pid < 0){
			perror("problem forking");
		}
		else if (pid > 0){
			//parent process
			wait(NULL);
		}
		else{
			//child process
			//command without arguments
			if (execlp(array[v], array[v], NULL) < 0){
				printf("command error");
			}
			//command with arguments
			else{
				if (execlp(array[v], array[v], array[v+1], NULL) < 0){
					printf("command error 2");
				}
			}
			perror("exec problem");
		}
		size++;

		//with pipe
	}


}
