from player.inverted_index import InvertedIndex
from player.inverted_intersect import InvertedIntersect

from game.wordle import Wordle, Letter

import time

class BenchmarkInvIndex:
    game = None
    wordle_words = None
    inv_index = None
    inv_intersect = None
    opener = None
    num_tries = None

    def __init__(self, opener):
        self.game = Wordle()
        self.wordle_words = self.game.get_wordle_words()
        self.inv_index = InvertedIndex()
        self.inv_intersect = InvertedIntersect()
        self.opener = opener

    def set_opener(self, opener):
        '''
        Setter for opening word

        @Param opener: word to be set as opener
        @Return: None
        '''
        self.opener = opener

    def benchmark_all(self):
        '''
        Tests all wordle words

        @Return: None (output in terminal)
        '''
        self.reset_num_tries()
        ctr = 0
        for word in self.wordle_words:
            self.game.set_word_of_the_day(word)
            self.play(self.opener, self.wordle_words)
            print('Solved \'' + word + '\' in ' + str(self.num_tries) + ' tries.')
            ctr += self.num_tries
            self.reset_num_tries()
        avg_tries = ctr / len(self.wordle_words)
        print("Average number of tries per word: " + str(avg_tries))

    def benchmark_words_starting_with(self, letter):
        '''
        Tests all wordle words starting with letter

        @Param letter: the starting letter of the words you want to test
        @Return: None (output in terminal)
        '''
        self.reset_num_tries()
        ctr = 0
        w_s_w = self.inv_index.index(self.wordle_words)[letter + '0'][1:]
        print('Words starting with \'' + letter + '\'')
        start = time.time()
        for word in w_s_w:
            self.game.set_word_of_the_day(word)
            self.play(self.opener, self.wordle_words)
            # print('Solved \'' + word + '\' in ' + str(self.num_tries) + ' tries.')
            ctr += self.num_tries
            self.reset_num_tries()
        duration = round(time.time() - start, 4)
        num_words = len(w_s_w)
        avg_tries = round(ctr / num_words, 4)
        avg_time = round(duration / num_words, 4)
        print("Number of words:\t\t" + str(num_words))
        print("Total time taken:\t\t" + str(duration) + 's')
        print("Average tries per word:\t\t" + str(avg_tries))
        print("Average time per word:\t\t" + str(avg_time) + 's')
        print()

    def benchmark(self, word):
        '''
        Tests a single word

        @Param word: word to test
        @Return: None (output in terminal)
        '''
        self.reset_num_tries()
        self.game.set_word_of_the_day(word)
        self.play(self.opener, self.wordle_words)
        print('Number of tries to win: ' + str(self.num_tries))

    def incr_num_tries(self):
        self.num_tries += 1

    def reset_num_tries(self):
        self.num_tries = 0

    def win(self, res):
        for r in res:
            if r != Letter.GREEN:
                return False
        return True
        
    def play(self, guess, valid_guesses):
        res = self.game.guess(guess)
        self.incr_num_tries()
        if self.win(res):

            return
        index = self.inv_index.index(valid_guesses)
        intersection = self.inv_intersect.get_intersection(guess,
                                                           res,
                                                           index,
                                                           valid_guesses)
        print(intersection)
        self.play(intersection[0], intersection)

    def benchmark_alphabet(self):
        '''
        Tests all wordle words alphabetically

        @Return: None (output in terminal)
        '''
        alphabet = [chr(i) for i in range(97, 97+26)]
        print('Playing with opener \'' + self.opener + '\'\n')
        start = time.time()
        for letter in alphabet:
            self.benchmark_words_starting_with(letter)
        duration = round((time.time() - start) / 60, 4)
        print('Total time to solve '
              + str(len(self.wordle_words))
              + ' words was '
              + str(duration)
              + ' min.')

def main():
    b = BenchmarkInvIndex('pares')
    # b.benchmark_alphabet()
    b.benchmark('speak')

if __name__ == '__main__':
    main()