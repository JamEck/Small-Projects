#include <stdio.h>
#include <stdbool.h>

###### input is pbm images (portable bit map)
###### compile using gcc
###### COMMAND LINE SYNTAX: ./<Executable_Name> <PBM Image File> <Name of Generated Text File>

int map (double, double);
char match (int);
int ab (int);

int main(int argc, char* argv[])
{
  FILE *in  = fopen(argv[1], "r");
  FILE *out = fopen(argv[2], "w");

  unsigned char trash = 0;

  bool color = false;
  int e = 0;          //element index of the input file
  int curr;           //element value of the input file (ascii value)
  int w = 0, h = 0;   //width and height of image
  int A0 = 0;         //How many newlines have we encountered
  int dimLen;         //number of digits in w and h as decimals
  int dataInd = 0;    //index where image data starts

  double R = 0.2943, G = 0.5624, B = 0.1433;
  double pixVal;
  double contrast;

  printf("Contrast? (0: none --> 1: most) ");
  scanf("%lf", &contrast);

  while (fscanf(in, "%c", &trash) == 1)
    {
      curr = (int)trash;

      if (e == 1 && trash == '6')
	color = true;
        
      if (trash == '\n')
        {
	  A0++;
	  if (A0 < 3)
            {
	      dimLen = 0;
	      while (fscanf(in, "%c", &trash) == 1)
                {
		  if (trash == '\n')
		    break;
		  dimLen++;
                }                                           //found dimLen
                
	      rewind(in);
	      for (int i = 0; i <= e; i++)                //go back to beginning of file
		fscanf(in, "%c", &trash);               //scan back to prev newline
                
	      int multiplier = 1;
	      for (int i = 1; i < dimLen; i++)
		multiplier *= 10;
                
	      for (int i = 0; i < dimLen; i++)
                {
		  fscanf(in, "%c", &trash);
		  curr = (int)trash;
                    
		  if (A0 == 1)
		    w += (curr-48)*multiplier;
		  if (A0 == 2)
		    h += (curr-48)*multiplier;
                    
		  multiplier /= 10;
		  e++;
                }
            }
	  if (A0 == 4)
	    {
	      e++;
	      dataInd = e;
	      break;
	    }
	}
        e++;
      }

  if (color)
    {
      fprintf(out, "    ");
      for (e = 0; e < w*h; e++)
	{
	  pixVal = 0;
	  fscanf(in, "%c", &trash);
	  pixVal += R*(int)trash;
	  fscanf(in, "%c", &trash);
	  pixVal += G*(int)trash;
	  fscanf(in, "%c", &trash);
	  pixVal += B*(int)trash;
            
	  //printf("pixval: %.3f\n",pixVal);
	  //printf ("mapped pixval: %d\n", map(pixVal));
	  fprintf(out, "%c ", match(map(pixVal, contrast)));
	  if (e%w == w-1)
	    fprintf(out, "\n    ");
	}
    }
    
  //printf("\n%d %d %d\n", w, h, dataInd);        
 }

int map (double val, double contrast) // map from 0 -> 255 to 185 -> 255
{
  double  diff;// = 127 - val;
  //if (diff < 0)
    //  diff *= -1;
  //diff /= 128;
  
  if (val > 127)
    {
      diff = (255 - val)*contrast;
      val+=diff;
    }
  else
    {
      diff = val*contrast;
      val-=diff;
    }

  //val*=diff;

  val = val*0.275 + 185;
  return (int)val;
}

char match (int val)
{
  int asciival[95] = {
    255, 235, 232, 197, 204, 205, 199,
    243, 227, 227, 219, 227, 240, 239,
    244, 228, 193, 217, 208, 208, 210,
    205, 199, 219, 194, 200, 238, 232,
    225, 226, 226, 224, 189, 203, 195,
    214, 216, 200, 215, 203, 202, 214,
    216, 203, 223, 189, 185, 201, 204,
    196, 198, 204, 218, 206, 210, 194,
    205, 218, 204, 217, 228, 217, 231,
    236, 247, 209, 204, 222, 204, 211,
    220, 199, 211, 223, 221, 210, 225,
    210, 218, 213, 204, 204, 231, 219,
    222, 217, 222, 212, 219, 214, 219,
    218, 227, 218, 233
  };

  int smallest = 255;
  int index = 1;

  for (int i = 0; i < 95; i++)
    {
      //printf("\n %d - %d = %d\n", val, asciival[i], ab(val - asciival[i]));
       if (ab(val-asciival[i]) < smallest)
       { 
	 smallest = ab(val-asciival[i]);
	 index =i;
       } 
    }
  //  printf ("%d", index);  
  return index + 32;
}

int ab (int num)
{
  if (num < 0)
    return -1*num;
  return num;
}
