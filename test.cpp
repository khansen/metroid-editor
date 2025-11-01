void DrawNameTable(unsigned char *nametable[30][32], unsigned int *bitmap);
void DrawNameTable(unsigned char *nametable[30][32], unsigned int *bitmap)
{
unsigned int bmpos = 0;
for (int y=0; y<30; y++)
    {
    for (int x=0; x<32; x++)
        DrawTile(nametable[y][x], 0, 0, bmpos+x+x);
    bmpos += 512;
    }   // End Y loop
};

//---------------------------------------------------------------------------

// Draw Structure -     draws a structure on the nametable according to
//                      position and palette bits

void DrawStruct(unsigned char *objptr, unsigned char *nametable[30][32], unsigned int areanum)
{
unsigned int posx;
unsigned int posy;
unsigned char *structdata;
unsigned int structpos = 0;
unsigned int xlength;
unsigned char *macroptr;
unsigned int macroofs;
unsigned int macronum;

posx = (objptr[0] & 0x0F) << 1;
posy = (objptr[0] & 0xF0) >> 3;
structdata = GetStructPointer(objptr[1], areanum);  // Get pointer to structure data

while (structdata[structpos] != 0xFF)   // Start drawing macros
    {
    if (posy >= 30) break;
    xlength = structdata[structpos++] & 0x0F;  // Number of macros to draw horizontally
    for (unsigned int i=0; i<xlength; i++)  // Do'em all
        {
        if (posx >= 32) break;

// Draw one macro

        macronum = structdata[structpos++];
        macroofs = Areas[areanum].macroofs - 0x08000 + Areas[areanum].ROMofs;
        macroptr = &fileptr[macroofs + (macronum << 2)]; // Set up pointer to macro data
        nametable[posy][posx] = macroptr[0];
        nametable[posy][posx+1] = macroptr[1];
        nametable[posy+1][posx] = macroptr[2];
        nametable[posy+1][posx+1] = macroptr[3];

        posx += 2;
        }
    posy += 2;
    }

};

