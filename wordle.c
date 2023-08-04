#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define WHITE "\033[1;37m"
#define GREY "\033[1;30m"
#define RED	"\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define DEF "\033[0m"
#define UNL "\u001b[4m"

typedef struct t_line s_line;
void	draw(s_line *head);

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

typedef struct t_line
{
	char	*letters;
	char	*colors;
	s_line	*next;
}				s_line;

void	free_stuff(s_line *head)
{
	s_line *tmp = head;
	while (tmp != NULL)
	{
		free(tmp->letters);
		free(tmp->colors);
		s_line *next = tmp->next;
		free(tmp);
		tmp = next;
	}
}

char	*toupper_string(char *str)
{
	int i = 0;
	while (str[i])
	{
		str[i] = toupper(str[i]);
		i++;
	}
	return (str);
}

char	*tolower_string(char *str)
{
	int i = 0;
	while (str[i])
	{
		str[i] = tolower(str[i]);
		i++;
	}
	return (str);
}

int	get_max_word_count(FILE *word_list_file)
{
	char	c;
	int		i;

	i = 0;
	while ((c = fgetc(word_list_file)) != EOF)
	{
		if (c == '\n')
		{
			i++;
		}
	}
	return (i);
}

void	set_stuff(char *clue, const char *answer, const char *guess, s_line *head)
{
	s_line	*tmp;
	int		i;
	static int line_counter = 0;

	tmp = head;
	i = 0;
	while (line_counter > i)
	{
		tmp = tmp->next;
		i++;
	}
	i = 0;
	while (guess[i])
	{
		tmp->letters[i * 2 + 1] = toupper(guess[i]);
		tmp->colors[i] = '0';
		if (clue[i] == '1')
			tmp->colors[i] = '1';
		else if (clue[i] == '2')
			tmp->colors[i] = '2';
		i++;
	}
	line_counter++;
}

bool	eval(const char *answer, const char *guess, s_line *head)
{
	int		i;
	int		j;

	char	clue[6] = {'#', '#', '#', '#', '#', '\0'};
	bool	answer_flags[5] = {false, false, false, false, false};
	i = 0;
	// look for greens
	while (i < 5)
	{
		if (guess[i] == answer[i])
		{
			clue[i] = '1';
			answer_flags[i] = true;
		}
		i++;
	}
	i = 0;
	// look for yellows
	while (i < 5)
	{
		if (clue[i] == '#')
		{
			j = 0;
			while (j < 5)
			{
				if (guess[i] == answer[j] && !answer_flags[j]) // so if the letter was found elsewhere ->
				{
					clue[i] = '2';
					answer_flags[j] = true; // reserve
					break ;
				}
				j++;
			}
		}
		i++;
	}
	set_stuff(clue, answer, guess, head);
	return (strcmp(guess, answer) == 0);
}

void	constructor(s_line *head)
{
	s_line *tmp;

	tmp = head;
	for (int i = 0; i < 6; i++)
	{
		tmp->letters = malloc(11);
		for (int j = 0; j < 10; j++)
		{
			if (j % 2)
				tmp->letters[j] = '_';
			else
				tmp->letters[j] = ' ';
		}
		tmp->letters[10] = '\0';
		tmp->next = malloc(sizeof(s_line));
		tmp = tmp->next;
	}
	tmp->next = NULL;
	tmp = head;
	for (int i = 0; i < 6; i++)
	{
		tmp->colors = malloc(6);
		bzero(tmp->colors, 6);
		tmp = tmp->next;
	}
}

void	draw(s_line *head)
{
	s_line *tmp;
	int		c_counter;

	tmp = head;
	c_counter = 0;
	clear_screen();
	printf("\033[4m" WHITE "W O R D L E\n\n" DEF);
	for(int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (!(j % 2))
			{
				printf("%c", tmp->letters[j]);
				continue ;
			}
			if (tmp->colors[c_counter] == '0')
			{
				printf(GREY"%c"DEF, tmp->letters[j]);
				c_counter++;
			}
			else if (tmp->colors[c_counter] == '1')
			{
				printf(GREEN"%c"DEF, tmp->letters[j]);
				c_counter++;
			}
			else if (tmp->colors[c_counter] == '2')
			{
				printf(YELLOW"%c"DEF, tmp->letters[j]);
				c_counter++;
			}
			else
				printf("%c", tmp->letters[j]);
		}
		printf("\n");
		tmp = tmp->next;
		c_counter = 0;
	}
	printf("\n");
}


int	handle_exceptions(char	*guess, FILE *word_list_file)
{
	int found = 0;
	char	*word = malloc(6);
	rewind(word_list_file);
	while (fgets(word, sizeof(word), word_list_file))
	{
		word[5] = '\0';
		if (!strcmp(word, guess))
			found = 1;
	}
	if (!strcmp(guess, "exit") || !strcmp(guess, "quit"))
	{
		printf(YELLOW"GOODBYE!\n\n"DEF);
		exit(0);
	}
	else if (strlen(guess) != 5 || !found)
	{
		printf(RED"INVALID WORD\n\n"DEF);
		return (1);
	}
	return (0);
}

void	print_blocks(s_line *head)
{
	s_line *tmp;
	int		c_counter;

	c_counter = 0;
	tmp = head;
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			if (tmp->letters[1] == '_')
				return ;
			if (!(j % 2))
			{
				printf("%c", tmp->letters[j]);
				continue ;
			}
			if (tmp->colors[c_counter] == '0')
			{
				printf(GREY"■"DEF);
				c_counter++;
			}
			else if (tmp->colors[c_counter] == '1')
			{
				printf(GREEN"■"DEF);
				c_counter++;
			}
			else if (tmp->colors[c_counter] == '2')
			{
				printf(YELLOW"■"DEF);
				c_counter++;
			}
			else
				printf("%c", tmp->letters[j]);
		}
		printf("\n");
		tmp = tmp->next;
		c_counter = 0;
	}
	printf("\n");
}

char	*game(char *answer, FILE *word_list_file)
{
	int		num_of_guesses;
	bool	final_guess;
	char	*guess;
	s_line	*head;

	num_of_guesses = 0;
	final_guess = false;
	guess = malloc(sizeof(char) * 6);
	head = malloc(sizeof(s_line));

	constructor(head);
	draw(head);

	while (num_of_guesses < 6) // main loop of 6
	{
		printf("> ");
		if (fgets(guess, sizeof(guess), stdin) == NULL)
		{
			printf(YELLOW"GOODBYE!\n\n"DEF);
			exit(0);
		}
		char *p = strchr(guess, '\n');
		if (p)
			*p = 0;
		else
		{
			scanf("%*[^\n]");scanf("%*c"); // stupid way of clearing the buffer if nl
		}
		guess[5] = '\0';
		if (handle_exceptions(guess, word_list_file))
			continue;
		num_of_guesses++;
		final_guess = eval(answer, guess, head); // evaluation of a guess
		draw(head);
		if (final_guess)
		{
			printf(GREEN" YOU  WIN!\n"DEF);
			print_blocks(head);
			free_stuff(head);
			return (guess);
		}
	}
	printf(RED" YOU LOSE!\n"DEF);
	printf(YELLOW"   %s\n"DEF, toupper_string(answer));
	free_stuff(head);
	return (guess);
}

int	main(void)
{
	char	**word_list;
	char	*word;
	int		i;
	int		word_count;
	int		max_words_count;
	FILE	*word_list_file;
	FILE	*word_list_file_ans;
	char	*answer;

	word_list_file = fopen("words.txt", "r");
	word_list_file_ans = fopen("words_ans.txt", "r");
	max_words_count = get_max_word_count(word_list_file);
	word_list = calloc(max_words_count, sizeof(char *));
	word = malloc(sizeof(char) * 6);
	word_count = 0;
	rewind(word_list_file); // set the file pointer back to beginning
	while (fscanf(word_list_file, "%s", word) != EOF
		&& word_count < max_words_count)
	{
		word_list[word_count] = word;
		word_count++;
		word = malloc(sizeof(char) * 6);
	}
	int word_count_ans = get_max_word_count(word_list_file_ans);
	rewind(word_list_file_ans);
	srand(time(NULL));
	int seed = (rand() % word_count_ans);

	answer = malloc(6);
	while (seed--)
		fgets(answer, sizeof(answer), word_list_file_ans);
	answer[5] = '\0';

	// play the game loop
	free(game(answer, word_list_file));
	fclose(word_list_file);
	fclose(word_list_file_ans);
	// free shit
	i = 0;
	while (i < word_count)
		free(word_list[i++]); 
	free(word_list);
	free(word);
	free(answer);
	exit(0);
	return (0);
}