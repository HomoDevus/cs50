import csv
import sys


def main():
    dnas = []
    dna_to_search_seq = {}

    # TODO: Check for command-line usage
    if (len(sys.argv) != 3):
        print("Usage: dna.py DNA_SAMPLES DNA_TO_SEARCH")
        sys.exit(1)

    # TODO: Read database file into a variable
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for person in reader:
            dnas.append(person)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file:
        dna_to_search = file.readline().rstrip()

    # TODO: Find longest match of each STR in DNA sequence
    for dna_str in dnas[0]:
        if dna_str == 'name':
            continue
        dna_to_search_seq[dna_str] = longest_match(dna_to_search, dna_str)

    # TODO: Check database for matching profiles
    for dna in dnas:
        is_all_mached = True
        for dna_str, repetitions in dna.items():
            if dna_str == 'name':
                continue
            if dna_to_search_seq[dna_str] != int(repetitions):
                is_all_mached = False
                break
        if is_all_mached:
            print(dna['name'])
            return

    print("No match")
    print(dna_to_search_seq)
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
