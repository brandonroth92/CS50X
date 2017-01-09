#include <cs50.h>
#include <stdio.h>

int main(void)
{
  /* prompt input of CC number and store in long long */
  printf("Number: ");
  long long ccNumber = get_long_long();
  
  /* validate  that number is greater than 0 or prompt for retry */
  while (ccNumber <= 0) {
    printf("Retry: ");
    ccNumber = get_long_long();
  }
  
  int count = 0;
  int firstInt = 0;
  
  /* initialize temp number equal to ccNumber to get number length */
  long long testDigits = ccNumber;
  while (testDigits > 0) {
    testDigits /= 10;
    count++;
  }
  
  /* place each digit into array */
  int numberArray[count];
  for (int i = 0; i < count; i++) {
    numberArray[i] = ccNumber % 10;
    ccNumber /= 10;
  }
  
  /* store first integer of number for use in validating visa vs. mastercard numbers */
  firstInt = numberArray[count-1];
  
  /* multiply every other number starting with 2nd to last number */
  for (int i = 1; i < count; i+=2) {
    numberArray[i] *= 2;
  }
    
  int sum = 0;
  /* add individual digits of products */  
  for (int i = 1; i < count; i+=2) {
    if (numberArray[i] > 9) {
      sum += 1 + (numberArray[i] - 10);
    } else {
      sum += numberArray[i];
    }
  }
  
  /* add other digits to sum */
  for (int i = 0; i < count; i+=2) {
    sum += numberArray[i];
  }
  
  /* validate number and determine company */
  if (!(sum % 10) == 0) {
    printf("INVALID\n");
  } else if (count == 15) {
    printf("AMEX\n");
  } else if (count == 16 && firstInt == 4) {
    printf("VISA\n");
  } else if (count == 13 || count == 16) {
    printf("MASTERCARD\n");
  }
}