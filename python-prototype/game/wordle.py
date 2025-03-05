from enum import Enum
import random

class Wordle:
    wordle_words = None
    word_of_the_day = None
    __file_name = 'game/valid-wordle-words.txt'

    def __init__(self):
        self.set_wordle_words()

    def random_word_of_the_day(self):
        '''
        Assigns a random wordle word as word of the day

        @return: None
        '''
        self.word_of_the_day = self.wordle_words[
            random.randrange(len(self.wordle_words))]

    def set_word_of_the_day(self, word):
        '''
        Setter for word of the day (for testing purposes)

        @return: None
        '''
        self.word_of_the_day = word

    def get_word_of_the_day(self):
        '''
        Getter for word of the day

        @return: word of the day
        '''
        return self.word_of_the_day

    def set_wordle_words(self):
        '''
        Setter for wordle words

        @return: None
        '''
        self.wordle_words = self.read_file()

    def read_file(self):
        '''
        Reads file of wordle words into list

        @param file_name: name of file to read
        @return: list of words in file
        '''
        w = []
        f = open(self.__file_name, 'r')
        for word in f:
            w.append(word.strip())
        f.close()
        return w

    def get_wordle_words(self):
        '''
        Getter for list of wordle words

        @return: list of wordle words
        '''
        return self.wordle_words

    def is_valid_word(self, guess):
        '''
        Checks whether word that player guessed is valid

        @param guess: guessed word (str)
        @return: True if guess is valid, False otherwise
        '''
        if guess:
            if len(guess) == 5:
                if guess in self.wordle_words:
                    return True
        return False

    def guess(self, guess):
        '''
        Gives response to the guessed word like in Wordle

        @param guess: guess (str), assumes self.is_valid checked for guess first
        @return: an array denoting correctness of each letter in guess
        '''
        word = [letter for letter in self.word_of_the_day]
        guess = [letter for letter in guess.lower()]
        response = []
        for i in range(5):
            response.append(Letter.GRAY)
            if word[i] == guess[i]:
                word[i] = '_'
                response[-1] = Letter.GREEN
        for i in range(5):
            if response[i] == Letter.GREEN:
                continue
            for j in range(5):
                if guess[i] == word[j]:
                    response[i] = Letter.YELLOW
                    word[j] = '_'
                    break
        return response

class Letter(Enum):
    GRAY = -1
    YELLOW = 0
    GREEN = 1