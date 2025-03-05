
import matplotlib.pyplot as plt
from collections import Counter

# path to valid-wordle-words.txt
file_path = 'valid-wordle-words.txt'

# read words
def read_words(file_path):
    with open(file_path, 'r') as file:
        return [word.strip().lower() for word in file.readlines()]

# calculate frequency of each letter
def letter_frequency(words):
    letter_counts = Counter(letter for word in words for letter in word)
    return letter_counts

# calculate frequency of each starting letter
def starting_letter_frequency(words):
    starting_letter_counts = Counter(word[0] for word in words)
    return starting_letter_counts

# plot Zipf's law
def plot_zipfs_law(letter_counts, title):

    # sort letters by frequency
    letters, frequencies = zip(*letter_counts.most_common())
    ranks = range(1, len(frequencies) + 1)

    # apply Zipf's law: f = c / r (where f is frequency, r is rank, c is frequency of most common letter)
    constant = frequencies[0]
    expected_frequencies = [constant / r for r in ranks]
    plt.figure(figsize=(10, 5))

    # plotting actual and expected frequencies (log-log plot)
    plt.loglog(ranks, frequencies, marker='o', linestyle='none', label='Actual Frequencies')
    plt.loglog(ranks, expected_frequencies, label='Expected by Zipf\'s Law', linestyle='--')
    plt.xlabel('Rank of letter (most common to least common)')
    plt.ylabel('Frequency of letter')
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.xticks(ranks, letters)
    plt.show()

def main():
    words = read_words(file_path)
    # analyze all letters
    all_letter_counts = letter_frequency(words)
    plot_zipfs_law(all_letter_counts, 'All Letter Frequency Analysis Using Zipf\'s Law')
    # analyze starting letters
    start_letter_counts = starting_letter_frequency(words)
    plot_zipfs_law(start_letter_counts, 'Starting Letter Frequency Analysis Using Zipf\'s Law')

main()
