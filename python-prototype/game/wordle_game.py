from game.wordle import *

class WordleGame:
    game = None
    player = None
    tries = None
    response = None
    guess = None
    
    def __init__(self):
        self.game = Wordle()

    def get_game(self):
        '''
        Getter for Wordle object

        @return: Wordle object
        '''
        return self.game

    def play(self):
        '''
        Emulates a game of Wordle, i.e. guess word of the day

        @return: None
        '''
        self.tries = 6

        print('-' * 70)
        print('Wordle: Guess the Word of the Day!')
        print('-' * 70)
        inp = input('Manually set word of the day (y/n)? ')
        if inp == 'y':
            word = None
            while not self.game.is_valid_word(word):
                word = input('Enter a word of the day (invalid words are ignored): ')
            self.game.set_word_of_the_day(word)
        else:
            self.game.random_word_of_the_day()
        print('-' * 70)
        
        while self.tries != 0:
            print('Number of tries remaining: ' + str(self.tries))
            print('Word of the day (for debugging): ' +
                  self.game.get_word_of_the_day())
            self.guess = None
            while not self.game.is_valid_word(self.guess):
                self.guess = input("Guess (invalid words are ignored): ")
            self.response = self.game.guess(self.guess)
            print(str([res.name for res in self.response]) + '\n')
            if self.player_win():
                print('Correct!')
                return
            self.tries -= 1

        print('You have 0 tries remaining.')
        print('The word of the day was \"' +
              self.game.get_word_of_the_day() + '\".')

    # def get_guess(self):
    #     '''
    #     Getter for the guessed word

    #     @Return: guessed word
    #     '''
    #     return self.guess

    # def get_response(self):
    #     '''
    #     Getter for the color of each letter of the guessed word (wordle response)

    #     @Return: list of colors of each letter of the guessed word
    #     '''
    #     return self.response

    def player_win(self):
        '''
        Checks if player guessed the word

        @return: True if player guessed the word, False otherwise
        '''
        for color in self.response:
            if color != Letter.GREEN:
                return False
        return True