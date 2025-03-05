from player.inverted_index import InvertedIndex
from player.inverted_intersect import InvertedIntersect

from game.wordle import Wordle

def main():
    g = Wordle()
    inv_index = InvertedIndex()
    inv_intersect = InvertedIntersect()
    
    g.set_word_of_the_day('drank')
    res = g.guess('prank')
    inv_index.index(g.get_wordle_words())
    intersected =inv_intersect.get_intersection('prank',
                                                res,
                                                inv_index.get_index(),
                                                g.get_wordle_words())
    

if __name__ == '__main__':
    main()