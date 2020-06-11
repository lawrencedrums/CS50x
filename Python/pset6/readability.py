# Started and submitted 8 June, 2020.
# Lawrence Wong. CS50x. Python - Readability.

def main():
    text = input("Text: ")

    letters, words, sentences = get_numbers(text)

    index = grade(letters, words, sentences)
    
    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def get_numbers(text):
    letters = 0
    words = 0
    sentences = 0

    # Iterate through each characters in text
    for character in text:
        if character.isalpha() == True:
            letters += 1
        elif character.isspace() == True:
            words += 1
        elif character == "." or character == "!" or character == "?":
            sentences += 1
        else:
            continue
    
    # Add the word at the very end
    words += 1

    return letters, words, sentences


def grade(letters, words, sentences):
    # Get letters and sentences average
    L = letters * (100 / words)
    S = sentences * (100 / words)

    # Apply the formula
    index = round((0.0588 * L) - (0.296 * S) - 15.8)

    return index
    

main()
