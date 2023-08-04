import os
import random

color_default = "\033[0m"
color_white = "\033[1;37m"
color_red = "\033[1;31m"
color_grey = "\033[1;30m"
color_green = "\033[1;32m"
color_yellow = "\033[1;33m"


def draw(board):
	os.system('clear')
	print('\u001b[4m' + color_white + 'W O R D L E\n' + color_default)
	for line in board:
		print(line)
	print()


def get_player_input(words):
	guess = None
	while not guess:
		try:
			guess = input('> ').lower()
			if guess == 'exit' or guess == 'quit':
				raise EOFError
			if len(guess) != 5 or guess not in words:
				raise Exception
		except EOFError:
			print(color_yellow + 'GOODBYE!\n' + color_default)
			exit()
		except Exception:
			print(color_red + 'INVALID WORD\n' + color_default)
			guess = None
	return guess


def compare_guess(guess, word, state):
	for i in range(len(state)):
		if guess[i] == word[i]:
			state[i] = 2
		elif guess[i] in word:
			state[i] = 1


def create_guess_string(guess, state):
	guess_string = ""
	for i in range(len(state)):
		color = color_grey
		if state[i] == 1:
			color = color_yellow
		elif state[i] == 2:
			color = color_green
		guess_string += ' ' + color + guess[i].upper()
	guess_string += color_default
	return guess_string


def print_blocks(board):
	for line in board:
		for c in line:
			if c.isupper():
				c = '■'
			elif c == '_':
				return
			print(c, end='')
		print()


def main():
	with open('words.txt') as dict_file:
		words = dict_file.read().split('\n')
	word = random.choice(words)
	board = [' _ _ _ _ _'] * 6
	guess_count = 0

	draw(board)
	for i in range(6):
		state = [0] * 5
		guess = get_player_input(words)
		compare_guess(guess, word, state)
		board[guess_count] = create_guess_string(guess, state)
		guess_count += 1
		draw(board)
		if state.count(2) == len(state):
			print(color_green + ' YOU  WIN!' + color_default)
			print_blocks(board)
			return
	print(color_red + ' YOU LOSE!' + color_default)
	print(color_yellow + '   ' + word.upper() + color_default)


if __name__ == '__main__':
	main()
