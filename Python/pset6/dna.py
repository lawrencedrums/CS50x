# Started 8 June, 2020. Submitted 11 June, 2020.
# Lawrence Wong. CS50x. Pset6 - DNA.

import csv
import sys


def main():
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    text_content = read_text(sys.argv[2])

    with open(sys.argv[1], "r") as f:
        csv_file = csv.reader(f)
        # Store the header of file in a list, also skip to the next line
        header = next(csv_file)
        # Throw in the content of the text file and the header minus the name for comparison
        DNA_sequence = get_sequence(text_content, header[1:])

        for line in csv_file:
            # Print the name of the candidiate if a match is found
            if line[1:] == DNA_sequence:
                print(line[0])
                sys.exit(0)

        else:
            print("No match")
            sys.exit(0)


def read_text(txt):
    # Get contents in sequence file
    with open(txt) as text_file:
        content = text_file.read()
    return content


def get_sequence(target_STR, header_STR):
    # Get the largest number of consecutive STRs in the header of CSV
    scanned_DNA = []

    for i in header_STR:
        max_count = 0
        consec_count = 1
        start_pos = 0

        while True:
            # .find() returns the position of substring if found, return -1 if not
            last_pos = target_STR.find(i, start_pos)
            # Check if the starting positition and result are the same, i.e. consecutive STRs
            if start_pos == last_pos and last_pos != 0:
                consec_count += 1
            # Ensures consective STR starting from the first is only counted once
            elif last_pos == 0:
                consec_count = 1
            # Break the loop if there's no more of that particular STR left in the text file
            elif last_pos == -1:
                if max_count < consec_count:
                    max_count = consec_count
                break
            # Record the highest number of consecutive STRs when the streak breaks
            elif start_pos != last_pos:
                if max_count < consec_count:
                    max_count = consec_count
                consec_count = 1
            # Update the searching positition to be right after the result
            start_pos = last_pos + len(i)

        scanned_DNA.append(str(max_count))

    return scanned_DNA


main()
