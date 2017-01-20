import cs50

def main():
    
    # Prompt for input height and continue prompting while input height is not between 1 and 23 inclusive
    while True:
        print("Height: ", end="")
        height = cs50.get_int()
        if (height > 0 and height < 24):
            break
    
    # Loop through height value
    for i in range(height):
        # Loop for number of spaces in row (spaces = height - 1 - 1) and print
        for j in range(height-i-1):
            print(" ", end="")
        # Loop for number of hashes in row (hashes = row + 1) and print
        for k in range(i + 1):
            print("#", end="")
        # Print two spaces in the middle of each row
        print("  ", end="")
        # Loop again through number of hases in row to print right side
        for l in range(i+1):
            print("#", end="")
        #Print a new line to move to next row
        print()
        
if __name__ == "__main__":
    main()