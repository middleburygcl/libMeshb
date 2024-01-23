#include <stdio.h>
#include <stdlib.h>
#include <libmeshb7.h>

int main(int argc, const char* argv[])
{
   int i, NmbVer, NmbPtp, ver, dim, *RefTab, (*PtpTab)[6];
   long long InpMsh, OutMsh;
   double (*VerTab)[4];

   if (argc < 3) {
      printf("test_pentatopes [input] [output]\n");
      return(1);
   }

   /*-----------------------------------*/
   /* Read input mesh file              */
   /*-----------------------------------*/
   if(!(InpMsh = GmfOpenMesh(argv[1], GmfRead, &ver, &dim)))
      return(1);

   printf("InpMsh: idx = %lld, version = %d, dimension = %d\n", InpMsh, ver, dim);

   if( (ver != 2) || (dim != 4) )
      exit(1);

   // Read the number of vertices and allocate memory
   NmbVer = (int)GmfStatKwd(InpMsh, GmfVertices);
   printf("InpMsh: nmb vertices = %d\n", NmbVer);
   VerTab = malloc((NmbVer+1) * 4 * sizeof(double));
   RefTab = malloc((NmbVer+1) * sizeof(int));

   // Read the number of pentatopes and allocate memory
   NmbPtp = (int)GmfStatKwd(InpMsh, GmfPentatopes);
   printf("InpMsh: nmb pentatopes = %d\n", NmbPtp);
   PtpTab = malloc((NmbPtp+1) * 5 * sizeof(int));

   // Read the vertices
   GmfGotoKwd(InpMsh, GmfVertices);

   for(i=1;i<=NmbVer;i++)
      GmfGetLin(  InpMsh, GmfVertices, &VerTab[i][0], &VerTab[i][1],
                  &VerTab[i][2], &VerTab[i][3], &RefTab[i] );

   // Read the pentatopes
   GmfGotoKwd(InpMsh, GmfPentatopes);

   for(i=1;i<=NmbPtp;i++)
      GmfGetLin(  InpMsh, GmfPentatopes, &PtpTab[i][0], &PtpTab[i][1],
                  &PtpTab[i][2], &PtpTab[i][3], &PtpTab[i][4], &PtpTab[i][5] );

   // Close the input mesh
   GmfCloseMesh(InpMsh);


   /*-----------------------------------*/
   /* Write the output mesh file        */
   /*-----------------------------------*/

   if(!(OutMsh = GmfOpenMesh(argv[2], GmfWrite, ver, dim)))
      return(1);

   GmfSetKwd(OutMsh, GmfVertices, NmbVer);
   for(i=1;i<=NmbVer;i++)
      GmfSetLin(  InpMsh, GmfVertices, VerTab[i][0], VerTab[i][1],
                  VerTab[i][2], VerTab[i][3], RefTab[i] );

   GmfSetKwd(OutMsh, GmfPentatopes, NmbPtp);
   for(i=1;i<=NmbPtp;i++)
   {
      GmfSetLin(  OutMsh, GmfPentatopes, PtpTab[i][0],
                  PtpTab[i][1], PtpTab[i][2], PtpTab[i][3], PtpTab[i][4], PtpTab[i][5] );
   }

   // Do not forget to close the mesh file
   GmfCloseMesh(OutMsh);

   free(PtpTab);
   free(RefTab);
   free(VerTab);

   return(0);
}
