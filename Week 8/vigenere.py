import cs50
import sys

def main():
    
    # Ensure proper usage
    if len(sys.argv) != 2:
        print("Error: Command-line arguments must equal two")
        exit(1)
    
    # Verify key contains only alphabetical characters per the vigenere algorithm specification   
    if sys.argv[1].isalpha() != True:
        print("Error: Keyphrase must include alphabetical characters only")
        exit(2)
        
    # Continue requesting plain text input from user until entered
    while True:
        print("Plaintext: ", end="")
        plainText = cs50.get_string()
        if len(plainText) > 0:
            break
    
    # Initialize variables for algorithm
    key = sys.argv[1]    
    plainTextLength = len(plainText)
    keyLength = len(key)
    
    # Initialize cipherText variable to hold ciphered text globally so that it can be referenced within if statements inside for loop
    cipherText = ""
    
    # j variable is used to wrap around key if plain text length is greater than key length
    j = 0
    
    # Loop through each letter in plain text variable
    # Determine if character is uppercase or lowercase as specification requires maintaining case
    # Shift each character according to algorithm and add to cipher text
    # If plainText[i] was an alphabetical character, increment j to move forward 1 character in keyphrase
    # If plainText[i] was not an alphabetical character, output it unshifted and do not move key position (j) forward
    
    for i in range(plainTextLength):
        if plainText[i].isupper():
            if key[j % keyLength].isupper():
                keyReference = 65
            else:
                keyReference = 97
            cipherText += chr(((ord(plainText[i]) - 65 + (ord(key[j % keyLength])) - keyReference) % 26) + 65)
            j += 1
        elif plainText[i].islower():
            if key[j % keyLength].isupper():
                keyReference = 65
            else:
                keyReference = 97
            cipherText += chr(((ord(plainText[i]) - 97 + (ord(key[j % keyLength])) - keyReference) % 26) + 97)
            j += 1
        else:
            cipherText += plainText[i]
        
    print("ciphertext: {}".format(cipherText))
        

if __name__ == "__main__":
    main()