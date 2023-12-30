#include "lib/images.h"

void main()
{
  Image image = random_image(3, 3);

  for (int i = 0; i < 3; i++)
  {
    print2DArray(image.image[i], 3, 3);
    printf("\n");
  }

  Image gray = RGB2Gray_scale(&image);
  print2DArray(gray.gray_scale, 3, 3);

  freeImage(&image);
}