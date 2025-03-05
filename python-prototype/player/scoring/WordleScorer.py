from collections import Counter
from typing import Iterator, Iterable, List, Tuple, Text, Dict, Union

NESTED_DICT = Dict[str, Dict[str, Union[str, float]]]
WORD_TUPLE = Iterable[Tuple[Text, List[str]]]

class WordleScorer:
	def __init__(self, word_of_the_day=None, wordle_words_path=None):
		"""
		WordlerScorer class for returning multiple statistics on 
		the valid-wordle-words file and for evaluating each guess.

		Note: wotd is needed to instantiate the class! ;) 

		Example: 
		>>> scorer = WordleScorer("decay")
		>>> scorer.word_scores() # returns best starting words
		"""
		self.word_of_the_day = word_of_the_day
		if self.word_of_the_day is None:
			raise ValueError("Word of the Day is not provided.")
		
		self.wordle_words_path = wordle_words_path
		if self.wordle_words_path is None:
			raise ValueError("Please include your path.")

		self.valid_wordle_words_path = wordle_words_path
		self.words_tuple = self.read_file(self.valid_wordle_words_path)

		self.overall_freq = self.get_overall_freq()
		self.pos1_freq = self.get_first_freq()
		self.pos2_freq = self.get_second_freq()
		self.pos3_freq = self.get_third_freq()
		self.pos4_freq = self.get_fourth_freq()
		self.pos5_freq = self.get_fifth_freq()

	def read_file(self, wordle_words_path: str) -> WORD_TUPLE:
		"""
		Generates (word, character_list) tuple from the lines in the text file.
		Example: ('aahed', ['a', 'a', 'h', 'e', 'd'])

		ValidWordleWords file contains one 5 letter word per line. Each line
		is composed of a valid guess for the game. 
		"""
		infile = wordle_words_path
		with open(infile, 'r') as f:
			words_tup = []
			for line in f:
				word = line.rstrip()
				characters = [*word]
				tup = (word, characters)
				words_tup.append(tup)
			return words_tup

	def count_words(self) -> int:
		"""
		Method that counts the total amount of words in the curated list 
		of 5-letter words. Should be called upon after each guess!
		"""
		count = 0
		for word in self.words_tuple:
			count += 1
		return count

	def levenshtein_distance(self, guess_word: str) -> int:
		"""
		The levenshtein distance is used as an evaluation metric and is 
		calculated after guessing is finished and the wotd is revealed.

		>>> scorer.levenshtein_distance("delay") # prints 1
		"""
		word_of_the_day = self.word_of_the_day
		s1 = len(guess_word)
		s2 = len(word_of_the_day)

		D = [[0 for i in range(s2 +1)] for j in range(s1+1)]

		for i in range(1, s1 + 1):
			D[i][0] = i

		for j in range(1, s2 + 1):
			D[0][j] = s2

		for i in range(1, s1+1):
			for j in range(1, s2+1):
				if guess_word[i-1] == word_of_the_day[j-1]:
					D[i][j] = D[i-1][j-1]
				else:
					D[i][j] = min(D[i-1][j], D[i][j-1], D[i-1][j-1]) + 1
		return D[i][j]

	def get_overall_freq(self) -> Dict:
		"""
		Method for calculating the overall letter distribution and returns
		a dictionary in descending order. 

		All letter frequency methods populate the attribute sort_freq and
		call the letter_scores() method to generate a score. 

		Note: dict must be properly sorted to calculate the scoring! This 
		means using Counter() exclusively will generate an incorrect scoring.
		"""
		letter_counts = Counter()
		for tup in self.words_tuple:
			letter_counts.update(tup[0])
		sort_freq = dict(sorted(letter_counts.items(), key=lambda x: x[1], reverse=True))
		self.sort_freq = sort_freq
		self.overall_scores = self.letter_scores()
		return sort_freq

	def get_first_freq(self) -> Dict:
		"""
		Method for calculating the starting letter frequency and returns
		a dictionary in descending order.
		"""
		letter_counts = Counter()
		for tup in self.words_tuple:
			letter_counts.update(tup[0][0])
		sort_freq = dict(sorted(letter_counts.items(), key=lambda x: x[1], reverse=True))
		self.sort_freq = sort_freq
		self.pos1_scores= self.letter_scores()
		return sort_freq

	def get_second_freq(self) -> Dict:
		letter_counts = Counter()
		for tup in self.words_tuple:
			letter_counts.update(tup[0][1])
		sort_freq = dict(sorted(letter_counts.items(), key=lambda x: x[1], reverse=True))
		self.sort_freq = sort_freq
		self.pos2_scores = self.letter_scores()
		return sort_freq

	def get_third_freq(self) -> Dict:
		letter_counts = Counter()
		for tup in self.words_tuple:
			letter_counts.update(tup[0][2])
		sort_freq = dict(sorted(letter_counts.items(), key=lambda x: x[1], reverse=True))
		self.sort_freq = sort_freq
		self.pos3_scores = self.letter_scores()
		return sort_freq

	def get_fourth_freq(self) -> Dict:
		letter_counts = Counter()
		for tup in self.words_tuple:
			letter_counts.update(tup[0][3])
		sort_freq = dict(sorted(letter_counts.items(), key=lambda x: x[1], reverse=True))
		self.sort_freq = sort_freq
		self.pos4_scores = self.letter_scores()
		return sort_freq

	def get_fifth_freq(self) -> Dict:
		letter_counts = Counter()
		for tup in self.words_tuple:
			letter_counts.update(tup[0][4])
		sort_freq = dict(sorted(letter_counts.items(), key=lambda x: x[1], reverse=True))
		self.sort_freq = sort_freq
		self.pos5_scores = self.letter_scores()
		return sort_freq

	def letter_scores(self) -> NESTED_DICT:
		"""
		Generates a score for each letter distribution from the sort_freq attribute in 
		each get_frequency method by using the relative frequencies to scale each score.

		A low score is desirable and indicates the penalty for choosing the letter. The 
		score is incremented with each for loop by adding the previous zipf score.

		Adjusted Zipf's law: freq ∝ 1 / (rank + β) where β is letter frequency/total frequency

		Ex: {'e': {'relFrq': 0.10037, 'zipf': 0.90879, 'score': 0.90879}, 
			...
			 'q': {'relFrq': 0.00195, 'zipf': 0.03846, 'score': 3.71671}}
		"""
		total = sum(self.sort_freq.values())
		scores = {}
		rank = 1
		accumulator = 0
		for key, val in self.sort_freq.items():
			scores[key] = {}
			scores[key]['relFrq'] = round(val/total, 5)
			scores[key]['zipf'] = round(1 / (rank + scores[key]['relFrq']), 5)
			rank += 1
			scores[key]['score'] = round(accumulator + scores[key]['zipf'], 5)
			accumulator = scores[key]['score']
		return scores

	def word_scores(self, penalty=3.0) -> Dict:
		self.penalty = penalty
		scores_tup = []
		for word in self.words_tuple:
			score = 0
			if len(set(word[0])) < 5:
				score += self.penalty
			for idx, char in enumerate(word[0]):
				score += self.overall_scores[char]['score']
				if idx == 0:
					score += self.pos1_scores[char]['score']
				elif idx == 1:
					score += self.pos2_scores[char]['score']
				elif idx == 2:
					score += self.pos3_scores[char]['score']
				elif idx == 3:
					score += self.pos4_scores[char]['score']
				elif idx == 4:
					score += self.pos5_scores[char]['score']
				tup = (word[0], round(score, 5))
			scores_tup.append(tup)
		scores = dict(scores_tup)
		scores_asc = dict(sorted(scores.items(), key=lambda x: x[1], reverse=False))
		return scores_asc

	def recompute_scoring(self, reduced_words_tuple, penalty) -> None:
		self.penalty = penalty
		self.words_tuple = reduced_words_tuple
		self.overall_freq = self.get_overall_freq()
		self.pos1_freq = self.get_first_freq()
		self.pos2_freq = self.get_second_freq()
		self.pos3_freq = self.get_third_freq()
		self.pos4_freq = self.get_fourth_freq()
		self.pos5_freq = self.get_fifth_freq()

		return self.word_scores(self.penalty)
