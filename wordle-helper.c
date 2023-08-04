#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WHITE "\033[1;37m"
#define GREY "\033[1;30m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define DEF "\033[0m"

char	black_letters[21];
char	green_letters[6];
char	yellow_letters[6];

int	is_yellow_present_elsewhere(char *word, int pos)
{
	int i = 0;

	while (word[i] && word[i] != '\n')
	{
		if (i == pos)
		{
			i++;
			continue ;
		}
		if (word[i] == yellow_letters[pos])
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	to_lower_string(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		str[i] = tolower(str[i]);
		i++;
	}
}

void	cut_list_by_green_letters(FILE *cut_wordlist)
{
	char	word[7];
	int		i;
	int		match;

	cut_wordlist = fopen("cut_wordlist.txt", "r");
	FILE *temp_file = fopen("temp_cut_wordlist.txt", "w");
	while (fgets(word, sizeof(word), cut_wordlist) != NULL)
	{
		i = 0;
		match = 1;
		if (word[5] == '\n')
			word[5] = '\0';
		while (i < 5)
		{
			if (green_letters[i] != '@' && green_letters[i] != word[i])
			{
				match = 0;
				break ;
			}
			i++;
		}
		if (match)
		{
			fprintf(temp_file, "%s\n", word);
		}
	}
	fclose(temp_file);
	fclose(cut_wordlist);
	remove("cut_wordlist.txt");
	rename("temp_cut_wordlist.txt", "cut_wordlist.txt");
}

void	cut_list_by_yellow_letters(FILE *cut_wordlist)
{
	char	word[7];
	int		match;
	int		i;

	cut_wordlist = fopen("cut_wordlist.txt", "r");
	FILE *temp_file = fopen("temp_cut_wordlist.txt", "w");
	while (fgets(word, sizeof(word), cut_wordlist) != NULL)
	{
		i = 0;
		match = 1;
		if (word[5] == '\n')
			word[5] = '\0';
		while (i < 5)
		{
			if(yellow_letters[i] != '@' && yellow_letters[i] == word[i])
			{
				match = 0;
				break ;
			}
			
			if (yellow_letters[i] != '@' && !is_yellow_present_elsewhere(word, i))
			{
				match = 0;
				break ;
			}
			
			i++;
		}
		if (match)
		{
			fprintf(temp_file, "%s\n", word);
		}
	}
	fclose(temp_file);
	fclose(cut_wordlist);
	remove("cut_wordlist.txt");
	rename("temp_cut_wordlist.txt", "cut_wordlist.txt");
}
void	cut_list_by_black_letters(FILE *cut_wordlist)
{
	char	word[6];
	int		i;
	cut_wordlist = fopen("cut_wordlist.txt", "r");
	FILE *temp_file = fopen("temp_cut_wordlist.txt", "w");

	while (fgets(word, sizeof(word), cut_wordlist) != NULL)
	{
		i = 0;
		while (i < 5)
		{
			if (!strchr(black_letters, word[i]))
				i++;
			else
				break ;
		}
		if (i == 5)
			fprintf(temp_file, "%s\n", word);
	}
	fclose(temp_file);
	fclose(cut_wordlist);
	remove("cut_wordlist.txt");
	rename("temp_cut_wordlist.txt", "cut_wordlist.txt");
}


void	copy_and_cut_list_by_black_letters_first_time(FILE *og_wordlist, FILE *cut_wordlist)
{
	char	word[6];
	int		i;

	while (fgets(word, sizeof(word), og_wordlist) != NULL)
	{
		i = 0;
		while (i < 5)
		{
			if (!strchr(black_letters, word[i]))
				i++;
			else
				break ;
		}
		if (i == 5)
			fprintf(cut_wordlist, "%s\n", word);
	}
	fclose(cut_wordlist);
	fclose(og_wordlist);
}

int	main(int argc, char **argv)
{
	FILE *og_wordlist = fopen("words_ans.txt", "r");

	printf(GREY "Grey letters: " DEF "letters that have been found not to be in the answer\n");
	printf(GREEN "Green letters: " DEF "letters that are found in the answer & in the right position\n");
	printf(YELLOW "Yellow letterss: " DEF "letters that are found in the answer & are not in the right position\n\n");
	printf("Press "WHITE"Enter"DEF" to continue...\n");
	while (getchar() != '\n'){} // these are just waits for the user
	printf("your first guess should be one of these: '"WHITE"raise"DEF"','"WHITE"roate"DEF"' or '"WHITE"soare"DEF"'\n");
	printf("reasoning: they have been found the most effective to start with\n\n");
	printf("Press "WHITE"Enter"DEF" to continue...\n");
	while (getchar() != '\n'){}
	int a = 6;
	while (a--)
	{
		char buf[6]; // 6 to safe space for null term
		FILE *cut_wordlist;

		printf("This is where you should take a guess\n");
		if (a == 5)
			printf("\nPress "WHITE"Enter"DEF" to continue...\n");
		while (getchar() != '\n'){}
		write(1, ".", 1);
		sleep(1);
		write(1, ".", 1);
		sleep(1);
		write(1, ".\n", 2);
		sleep(1);
		printf("You took a guess\n");
		sleep(1);

		printf("- Enter "GREY"Grey letters"DEF" discovered this turn (if not, press enter)\n");
		printf("> ");
		fgets(buf, sizeof(buf), stdin);
		if (buf[0] == '\n')
			buf[0] = '\0';
		sleep(1);
		to_lower_string(buf);
		strcat(black_letters, buf);
		if (a == 5)
		{
			cut_wordlist = fopen("cut_wordlist.txt", "w");
			copy_and_cut_list_by_black_letters_first_time(og_wordlist, cut_wordlist);
		}
		else
		{
			cut_list_by_black_letters(cut_wordlist);
		}
		printf("\n- Enter "GREEN"Green letter"DEF" positions found so far\n");
		printf("EXAMPLE = word: 'crane', format: ("GREEN"cr"DEF"@@"GREEN"e"DEF"), where '@' is anything but a green letter\n");
		printf("> ");
		//fgets(green_letters, sizeof(green_letters), stdin); // <----- tried fgets, but found that scanf works better (not serious shit left in buf)
		scanf("%5s", green_letters);
		if (green_letters[0] == '\n')
		{
			dprintf(2, "error with input: exiting!\n");
			exit(1);
		}
		sleep(1);
		rewind(cut_wordlist); // might be redundant because we are reopening anyways
		cut_list_by_green_letters(cut_wordlist);
		printf("\n- Enter "YELLOW"Yellow letter"DEF" positions found so far\n");
		printf("EXAMPLE = word: 'crane', format: (@@"YELLOW"ce"DEF"@), where '@' is anything but a yellow letter\n");
		printf("> ");
		//fgets(yellow_letters, sizeof(yellow_letters), stdin);
		scanf("%5s", yellow_letters);
		if (yellow_letters[0] == '\n')
		{
			dprintf(2, "error with input: exiting!\n");
			exit(1);
		}
		sleep(1);
		cut_list_by_yellow_letters(cut_wordlist);
	}
	return (0);
}