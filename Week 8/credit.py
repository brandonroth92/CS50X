import cs50

def main():
    
    # Prompt for credit card number input using cs50's get_int function to guarantee input is type int
    print("Number: ", end="")
    ccNumber = cs50.get_int()
    
    # Prompt for "Retry: " while input number is less than or equal to 0
    while ccNumber <= 0:
        print("Retry: ", end="")
        ccNumber = cs50.get_int()
    
    # Convert ccNumber to str to use len method to get number of integers    
    count = len(str(ccNumber))
    
    # Create empty list and add individual digits to list for arimethic manipulation as prescribed by algorithm    
    numberArray = []
    for i in range(count):
        numberArray.append(ccNumber % 10)
        ccNumber //= 10
    
    # Remember first integer for use in determining type of card between Visa and MasterCard which can both have 16 digits
    firstInt = numberArray[count-1]
    
    # Multiply numbers in odd indices by two, starting with 2nd to last number (numbers were added to array in reverse order using append)
    for i in range(count):
        if i % 2 != 0:
            numberArray[i] *= 2

    # Initialize variable sum for global use and add values in odd indices, making sure to subtract 10 if num is greater than 10
    sum = 0
    for i in range(count):
        if i % 2 != 0:
            if numberArray[i] > 9:
                sum += 1 + (numberArray[i] - 10)
            else:
                sum += numberArray[i]
    # Add values in even indices to sum
    for i in range(count):
        if i % 2 == 0:
            sum += numberArray[i]
    
    # Test number for invalidity. If valid, use if/else statements to check for type of card and print type
    if sum % 10 != 0:
        print("INVALID")
    elif count == 15:
        print("AMEX")
    elif count == 16 and firstInt == 4:
        print("VISA")
    elif count == 13 or count == 16:
        print("MASTERCARD")
        
        
if __name__ == "__main__":
    main()
    
    
    