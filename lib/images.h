#include <stdio.h>
#include <stdlib.h>
typedef int **Matrice;
#define LAYERS 3
#define PIXEL_GRADES 255
#define R 0
#define G 1
#define B 2

typedef struct
{
  unsigned int resolution;
  unsigned int h;
  unsigned int w;
  int ***image;
  Matrice gray_scale;
} Image;

// Global func
void freeMatrix(Matrice m, int n)
{
  for (int i = 0; i < n; i++)
  {
    free(m[i]);
  }
  free(m);
}

void freeImage(Image *image)
{
  for (int layer = 0; layer < LAYERS; layer++)
  {
    for (int row = 0; row < image->w; row++)
    {
      free(image->image[layer][row]);
    }
    free(image->image[layer]);
  }
}

void print2DArray(Matrice m, int l, int c)
{
  // Find the maximum number of digits in the matrix
  int maxDigits = 0;
  for (int i = 0; i < l; ++i)
  {
    for (int j = 0; j < c; ++j)
    {
      int digits = snprintf(NULL, 0, "%d", m[i][j]);
      if (digits > maxDigits)
        maxDigits = digits;
    }
  }

  // Print the matrix with a fixed width for each element
  for (int i = 0; i < l; ++i)
  {
    for (int j = 0; j < c; ++j)
    {
      printf("%*d\t", maxDigits, m[i][j]);
    }
    printf("\n");
  }
}

void printMatrix(Matrice matrix, int n)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }
}

// Matrix func
Matrice get_random_matrix(int l, int c, int range)
{
  Matrice matrix = (int **)malloc(l * sizeof(int *));
  for (int i = 0; i < l; i++)
  {
    matrix[i] = (int *)malloc(c * sizeof(int)); // Allocate memory for each row
    for (int j = 0; j < c; j++)
    {
      matrix[i][j] = rand() % range;
    }
  }
  return matrix;
}

Matrice get_user_matrix(int l, int c, short unsigned int layer)
{
  int input;

  Matrice matrix = (int **)calloc(l, sizeof(int *));
  printf("Write the value (0 to 255) of ->\n");
  for (int i = 0; i < l; i++)
  {
    matrix[i] = (int *)malloc(c * sizeof(int));
    for (int j = 0; j < c; j++)
    {
      switch (layer)
      {
      case 0:
        printf("Layer R ,pixel ( line:%d , col:%d ) : ", i + 1, j + 1);
        break;
      case 1:
        printf("Layer G ,pixel ( line:%d , col:%d ) : ", i + 1, j + 1);
        break;
      case 2:
        printf("Layer B ,pixel ( line:%d , col:%d ) : ", i + 1, j + 1);
        break;
      }
      scanf("%d", &input);
      matrix[i][j] = input;
    }
  }
  return matrix;
}

void write_matrix2file(Matrice matrix, int rows, int cols, const char *filename)
{
  FILE *file = fopen(filename, "w");

  if (file == NULL)
  {
    printf("Error opening file for writing.\n");
    return;
  }

  fprintf(file, "%d %d\n", rows, cols);

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      fprintf(file, "%d ", matrix[i][j]);
    }
    fprintf(file, "\n");
  }

  fclose(file);
}

void read_matrix_f_file(int ***matrix, int *rows, int *cols, const char *filename)
{
  FILE *file = fopen(filename, "r");

  if (file == NULL)
  {
    printf("Error opening file for reading.\n");
    return;
  }

  fscanf(file, "%d %d", rows, cols);

  *matrix = (int **)malloc(*rows * sizeof(int *));
  for (int i = 0; i < *rows; i++)
  {
    (*matrix)[i] = (int *)malloc(*cols * sizeof(int));
    for (int j = 0; j < *cols; j++)
    {
      fscanf(file, "%d", &(*matrix)[i][j]);
    }
  }

  fclose(file);
}

// Images Func
Image init_image(unsigned int width, unsigned int heigh)
{
  Image m;
  m.h = heigh;
  m.w = width;
  m.resolution = heigh * width;
  m.gray_scale = NULL;

  m.image = calloc(LAYERS, sizeof(int **));
  for (int i = 0; i < LAYERS; i++)
    // {
    //   Matrice layer = calloc(m.w, sizeof(int *));
    //   for (int l = 0; l < m.w; l++)
    //   {
    //     layer[l] = calloc(m.h, sizeof(int));
    //   }
    //   // store each layer in index in the image array
    //   m.image[i] = layer;
    // }

    return m;
}

Image random_image(unsigned int width, unsigned int heigh)
{
  Image m = init_image(width, heigh);

  for (int i = 0; i < LAYERS; i++)
  {
    m.image[i] = get_random_matrix(width, heigh, PIXEL_GRADES);
  }

  return m;
}

Image user_image(unsigned int width, unsigned int heigh)
{
  Image m = init_image(width, heigh);
  for (int i = 0; i < LAYERS; i++)
  {
    m.image[i] = get_user_matrix(width, heigh, i);
    print2DArray(m.image[i], width, heigh);
  }
}

void image2file(Image *image, const char *filename)
{
  FILE *file = fopen(filename, "w");

  if (file == NULL)
  {
    printf("Error opening file for writing.\n");
    return;
  }
  fprintf(file, "%d %d\n", image->w, image->h);

  for (int layer = 0; layer < LAYERS; layer++)
  {
    for (int i = 0; i < image->w; i++)
    {
      for (int j = 0; j < image->h; j++)
      {
        fprintf(file, "%d ", image->image[layer][i][j]);
      }
      fprintf(file, "\n");
    }
    /* code */
  }
}

void image_f_file(Image *image, const char *filename)
{
  FILE *file = fopen(filename, "r");

  if (file == NULL)
  {
    printf("Error opening file for reading.\n");
    return;
  }

  fscanf(file, "%d %d", &image->w, &image->h);
  // printf("test");
  image->resolution = image->h * image->w;

  for (int layer = 0; layer < LAYERS; layer++)
  {
    image->image[layer] = calloc(image->w, sizeof(int));
    for (int i = 0; i < image->w; i++)
    {
      image->image[layer][i] = calloc(image->h, sizeof(int));
      for (int j = 0; j < image->h; j++)
      {
        fscanf(file, "%d", &(image->image[layer])[i][j]);
      }
    }
  }

  fclose(file);
}

Image RGB2Gray_scale(Image *image)
{
  Image gray_scale = init_image(image->w, image->h);
  gray_scale.image = NULL;

  gray_scale.gray_scale = calloc(gray_scale.h, sizeof(int *));
  for (int i = 0; i < gray_scale.h; i++)
  {
    gray_scale.gray_scale[i] = calloc(gray_scale.w, sizeof(int));
  }

  for (int i = 0; i < image->h; i++)
  {
    for (int j = 0; j < image->w; j++)
    {
      gray_scale.gray_scale[i][j] = image->image[R][i][j] * 0.299 + 0.587 * image->image[G][i][j] + 0.114 * image->image[B][i][j];
    }
  }

  return gray_scale;
}