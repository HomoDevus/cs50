from cs50 import get_string
import re


def get_letters_amount(text):
    text = re.sub(r'[^a-zA-Z]', '', text)
    return len(text)


def get_words_amount(text):
    return len(text.split(" "))


def get_sentences_amount(text):
    return len(re.split(r'\. |\? |! ', text))


text = get_string("Type in text: ")
# Avarage number of letters per 100 words
avr_letters = get_letters_amount(text) / (get_words_amount(text) / 100)
# Avarage number of sentences per 100 words
avr_sentences = get_sentences_amount(text) / (get_words_amount(text) / 100)

# Calculate Coleman-Liau index
colemanIndex = 0.0588 * avr_letters - 0.296 * avr_sentences - 15.8

if (colemanIndex >= 16):
    print("Grade 16+")
elif (colemanIndex < 1):
    print("Before Grade 1")
else:
    print(f"Grade {round(colemanIndex)}")