#include <cs50.h>
#include <stdio.h>

int main(void)
{
  /* initialize pyramid height variable */
  int height = 0;
  /* prompt for input and validate data */
  do
  {
    printf("Height: ");
    height = get_int();
    if (height == 0)
    {
      return 0;
    }
  }
  while (height < 0 || height > 23);
  
  /* loop for number of iterations equal to height of pyramid */
  for (int i = 0; i < height; i++) 
  {
    /* row = i. Loop through number of spaces in row */
    for (int j = 0; j < height - i - 1; j++) 
    {
      printf("%s", " ");
    }
    /* loop through number of hashtags in row starting at row + 1 */
    for (int k = 0; k < i + 1; k++)
    {
      printf("#");
    }
    /* print two spaces between each row */
    printf("%s", "  ");
    /* print hashtags of right pyramid equal to number of hashtags on left side */
    for (int l = 0; l < i + 1; l++)
    {
      printf("#");
    }
    /* print newline to begin new row */
    printf("\n");
  }
}