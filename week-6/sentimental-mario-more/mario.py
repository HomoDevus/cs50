from cs50 import get_int

while True:
    height = get_int("Type height of the piramid: ")
    if 0 < height <= 8:
        break
    print("Height should be more than 0 and less than 9")

for i in range(1, height + 1):
    spaces = height - i
    print(" " * spaces, end="")
    print("#" * i, end="  ")
    print("#" * i)