// XA track playback example
// base on `psyq/addons/scee/CD/XAPLAYER`
// Refs : http://psx.arthus.net/code/XA/XATUT.pdf
//        http://psx.arthus.net/code/XA/xatut.zip
//        http://psx.arthus.net/code/XA/XA%20ADPCM%20documentation.txt
// Schnappy 2021
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
// CD library
//#include <libcd.h>
// SPU library
#include <libspu.h>

//#include "modeldrawer.c"

#include "kite_base.c"
//#include "sackboy.c"
//#include "kite_outline.c"

#define VMODE 0                 // Video Mode : 0 : NTSC, 1: PAL
#define SCREENXRES 320          // Screen width
#define SCREENYRES 240 + (VMODE << 4)          // Screen height : If VMODE is 0 = 240, if VMODE is 1 = 256 
#define CENTERX SCREENXRES/2    // Center of screen on x 
#define CENTERY SCREENYRES/2    // Center of screen on y
#define OTLEN       2048        // Maximum number of OT entries
#define PRIMBUFFLEN 32768       // Maximum number of POLY_GT3 primitives
#define MARGINX 0                // margins for text display
#define MARGINY 32
#define FONTSIZE 8 * 7           // Text Field Height
DISPENV disp[2];                 // Double buffered DISPENV and DRAWENV
DRAWENV draw[2];
u_long      ot[2][OTLEN];                   // Ordering table (contains addresses to primitives)
char    primbuff[2][PRIMBUFFLEN] = {0}; // Primitive list // That's our prim buffer
char * nextpri = primbuff[0];                       // Primitive counter
short           db  = 0;                        // Current buffer counter
// Prototypes
void init(void);
void display(void);
void LoadTexture(u_long * tim, TIM_IMAGE * tparam);

// SPU attributes
//SpuCommonAttr spuSettings;
//#define CD_SECTOR_SIZE 2048
// XA
// Sector offset for XA data 4: simple speed, 8: double speed
//#define XA_SECTOR_OFFSET 4
// Number of XA files
//#define XA_TRACKS 1
//typedef struct {
//    int start;
//    int end;
//} XA_TRACK;
// Declare an array of XA_TRACK
////XA_TRACK XATrack[XA_TRACKS];
// Name of file to load
//static char * loadXA = "\\MUSIC.XA;1";
//CdlFILE XAPos = {0};
// Start and end position of XA data, in sectors
//static int      StartPos, EndPos;
// Current pos in file
//static int      CurPos = -1;
// Playback status : 0 not playing, 1 playing
//static int gPlaying = 0;
// Current XA channel 
//static char channel = 0;

bool DebugCam=true;

extern unsigned long _binary_TIM_vignette_tim_start[];
extern unsigned long _binary_TIM_vignette_tim_end[];
extern unsigned long _binary_TIM_vignette_tim_length;
extern unsigned long _binary_TIM_stroke_tim_start[];
extern unsigned long _binary_TIM_stroke_tim_end[];
extern unsigned long _binary_TIM_stroke_tim_length;

TIM_IMAGE vig;
TIM_IMAGE stroke;

int     i;
int countyThing=0;
    int     PadStatus;
    int     TPressed=0;
    int     AutoRotate=0;
    long    t, p, OTz, Flag;                // t == vertex count, p == depth cueing interpolation value, OTz ==  value to create Z-ordered OT, Flag == see LibOver47.pdf, p.143
    POLY_GT3 *poly = {0};                           // pointer to a POLY_G4 
	POLY_G3 *npoly = {0};                           // pointer to a POLY_G4 
    SVECTOR Rotate={ 0 };                   // Rotation coordinates
    VECTOR  Trans={ 0, 0, CENTERX * ONE * 3, 0 };     // Translation coordinates
                                            // Scaling coordinates
    VECTOR  Scale={ 1,1,1, 0 };     // ONE == 4096
    MATRIX  Matrix={0};    

	DR_MODE * dr_mode;

int time;

SVECTOR Prot={ 0, 0, 0 };                   // Rotation coordinates
VECTOR  Ppos={ 0, 0, 0, 0 };     // Translation coordinates

VECTOR p1;
VECTOR p2;
VECTOR p3;

VECTOR Ptrans={0,0,0};

int year;
int day;
int minute;
int secound;

short envdata[32][32];

VECTOR envmodel[1024];

void draw_sprt(TIM_IMAGE *tim, int x, int y, int xoff, int yoff) {
  SPRT *prim = (SPRT *)nextpri;
  setSprt(prim);
  setRGB0(prim, 0x80, 0x80, 0x80);
  setXY0(prim, x, y);
  setUV0(prim, xoff, yoff);
  setWH(prim, tim->prect->w, tim->prect->h);
  prim->clut = getClut(tim->crect->x, tim->crect->y);
  addPrim(ot[db], prim);
  nextpri += sizeof(SPRT);

  DR_TPAGE *tprim = (DR_TPAGE *)nextpri;
  setDrawTPage(tprim, 0, 1, getTPage(tim->mode, 0, tim->prect->x, tim->prect->y));
  addPrim(ot[db], tprim);
  nextpri += sizeof(DR_TPAGE);
}

void draw_quad(TIM_IMAGE *tim, int x, int y, int w, int h, int w2, int h2, int xoff, int yoff) {
RECT tws = {0, 0, 0, 0};            // Texture window coordinates : x, y, w, h
  POLY_FT4 *prim = (POLY_FT4 *)nextpri;
  setPolyFT4(prim);
  setRGB0(prim, 0x80, 0x80, 0x80);
  //if(flipped){	
	//setXY4(prim, x+w2, y, x, y, x+w2, y + h2, x , y + h2);
  //}else{
	setXY4(prim, x, y, x + w2, y, x, y + h2, x + w2, y + h2);
  //}  
  //if(trans){
	
  	setSemiTrans(prim, 1);
  //}
  setUV4(prim, xoff, yoff, xoff + w, yoff, xoff, yoff + h, xoff + w, yoff + h);
  prim->tpage = getTPage(tim->mode, 2, tim->prect->x, tim->prect->y);
  prim->clut = getClut(tim->crect->x, tim->crect->y);
  addPrim(ot[db], prim);
  nextpri += sizeof(POLY_FT4);
}


void draw_bloom(int yoff, int yoff2) {
  RECT tws = {0, 0, 0, 0};
  POLY_FT4 *prim = (POLY_FT4 *)nextpri;
  setPolyFT4(prim);
  setRGB0(prim, 128,126,112);
  setXY4(prim, 0, yoff2, 256, yoff2, 0, yoff2+240, 256, yoff2+240);
  //setDrawStp(prim, 1);
  setUV4(prim, 0, 0, 256, 0, 0, 240, 256, 240);
  prim->tpage = getTPage(2, 2, 0, yoff);
  addPrim(ot[db], prim);
  nextpri += sizeof(POLY_FT4);
}

void init(void)
{
	for(int j=0;j<32;j++){
		for(int i=0;i<32;i++){
			envdata[i][j]=1;
			if(i>8 && i<16){
				envdata[i][j]=i*j*16;
			}
		}
	}
	// Reset the GPU before doing anything and the controller
    PadInit(0);
    ResetGraph(0);
    // Initialize and setup the GTE
    InitGeom();
    SetGeomOffset(CENTERX, 120);        // x, y offset
    SetGeomScreen(CENTERX);                 // Distance between eye and screen  
        // Set the display and draw environments
    SetDefDispEnv(&disp[0], 0, 0         , SCREENXRES, SCREENYRES);
    SetDefDispEnv(&disp[1], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[0], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[1], 0, 0, SCREENXRES, SCREENYRES);
    if (VMODE)
    {
        SetVideoMode(MODE_PAL);
        disp[0].screen.y += 8;
        disp[1].screen.y += 8;
    }
    SetDispMask(1);                 // Display on screen    
    setRGB0(&draw[0], 224, 254, 255); // set color for first draw area
    setRGB0(&draw[1], 224, 254, 255); // set color for second draw area
    draw[0].isbg = 1;               // set mask for draw areas. 1 means repainting the area with the RGB color each frame 
    draw[1].isbg = 1;
    PutDispEnv(&disp[db]);          // set the disp and draw environnments
    PutDrawEnv(&draw[db]);
    FntLoad(960, 0);                // Load font to vram at 960,0(+128)
    FntOpen(MARGINX, SCREENYRES - MARGINY - FONTSIZE, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 280 ); // FntOpen(x, y, width, height,  black_bg, max. nbr. chars
}

void display(void){
    DrawSync(0);
    VSync(0);
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    DrawOTag(&ot[db][OTLEN - 1]);
    db = !db;
    nextpri = primbuff[db];
}

void LoadTexture(u_long * tim, TIM_IMAGE * tparam){     // This part is from Lameguy64's tutorial series : lameguy64.net/svn/pstutorials/chapter1/3-textures.html login/pw: annoyingmous
        OpenTIM(tim);                                   // Open the tim binary data, feed it the address of the data in memory
        ReadTIM(tparam);                                // This read the header of the TIM data and sets the corresponding members of the TIM_IMAGE structure
        
        LoadImage(tparam->prect, tparam->paddr);        // Transfer the data from memory to VRAM at position prect.x, prect.y
        DrawSync(0);                                    // Wait for the drawing to end
        
        if (tparam->mode & 0x8){ // check 4th bit       // If 4th bit == 1, TIM has a CLUT
            LoadImage(tparam->crect, tparam->caddr);    // Load it to VRAM at position crect.x, crect.y
            DrawSync(0);                                // Wait for drawing to end
    }

}

int main(void)
{
    // Matrix data for the GTE
    // Init display
    init();     

	LoadTexture(_binary_TIM_kitebase_tim_start , &kite_base_tim);
	LoadTexture(_binary_TIM_stroke_tim_start , &stroke);
	LoadTexture(_binary_TIM_vignette_tim_start , &vig);
    // SPU setup
    // Init Spu
    SpuInit();

    while (1)  // infinite loop
    {   
        // Read pad status
        PadStatus = PadRead(0);
        if (DebugCam == true) {
			if (PadStatus & PADselect) {
				Rotate.vx = Rotate.vy = Rotate.vz = 0;
	            Scale.vx = Scale.vy = Scale.vz = ONE/2;
	            Trans.vx = Trans.vy = 0;
	            Trans.vz = CENTERX * 3;
	        	DebugCam=!DebugCam;  
	        }
            if (PadStatus & PADL1) Trans.vz -= 4;
            if (PadStatus & PADR1) Trans.vz += 4;
            if (PadStatus & PADL2) Rotate.vz -= 8;
            if (PadStatus & PADR2) Rotate.vz += 8;
            if (PadStatus & PADLup)     Rotate.vx -= 8;
            if (PadStatus & PADLdown)   Rotate.vx += 8;
            if (PadStatus & PADLleft)   Rotate.vy -= 8;
            if (PadStatus & PADLright)  Rotate.vy += 8;
            if (PadStatus & PADRup)     Trans.vy -= 2;
            if (PadStatus & PADRdown)   Trans.vy += 2;
            if (PadStatus & PADRleft)   Trans.vx -= 2;
            if (PadStatus & PADRright)  Trans.vx += 2;
        }else{
			kite_update();
		}
		
        ClearOTagR(ot[db], OTLEN);

		/*
		if(db==1){
			draw_bloom(0,240);
		}else{ 
			draw_bloom(240,0);
		}
		*/

        //draw_quad(&vig,0,0,160,120,321,241,0,0);
		Ptrans=(VECTOR){Ppos.vx/ONE,Ppos.vy/ONE,Ppos.vz/ONE};
		drawmodel(kite_base, kite_base_mesh, kite_base_index, (VECTOR){0,0,0}, Prot, kite_base_tim);
		//drawmodel(Kite_outline, Kite_outline_mesh, Kite_outline_index,(VECTOR){px,py,pz},(SVECTOR){pxa,pya,pza,0}, Outline);
		drawenv();

        FntPrint("Kite! - debug%d\n", db);  
        FntPrint("hello world");  
        FntFlush(-1);
        display();
    }
    return 0;
}

void kite_update(){
	/*
	if(Prot.vx>0){
		Prot.vx-=4;
	}else if(Prot.vx<0){
		Prot.vx+=4;
	}

	if(Prot.vz>0){
		Prot.vz-=4;
	}else if(Prot.vz<0){
		Prot.vz+=4;
	}
	*/
	VECTOR dir = {csin(Prot.vy)*-1,ccos(Prot.vy)*-1,csin(Prot.vx)};
	Ppos.vx+=dir.vx;
	Ppos.vz+=dir.vy;	
	Ppos.vy+=dir.vz;
    if (PadStatus & PADLup)     Prot.vx -= 8;
    if (PadStatus & PADLdown)   Prot.vx += 8;
    if (PadStatus & PADLleft)   Prot.vy -= 8;
    if (PadStatus & PADLright)  Prot.vy += 8;
    //Trans.vx = Ppos.vx/ONE;
    //Trans.vy = (Ppos.vy+ONE)/ONE;
    //Trans.vz = (Ppos.vz-ONE)/ONE;
}

void drawenv(){
	for(int j=0;j<8;j++){
		for(int i=0;i<8;i++){
			short height1 = envdata[i][j];
			short height2 = envdata[i+1][j];
			short height3 = envdata[i][j+1];
			short height4 = envdata[i+1][j+1];
			SVECTOR p1 = {(i-4)*512,height1,(j-4)*512};
			SVECTOR p2 = {((i-4)+1)*512,height2,(j-4)*512};
			SVECTOR p3 = {(i-4)*512,height3,((j-4)+1)*512};
			SVECTOR p4 = {((i-4)+1)*512,height4,((j-4)+1)*512};
			drawtriangle(p1,p2,p3,(VECTOR){0,0,0},(VECTOR){255,0,0},(VECTOR){0,255,0},Ptrans,(SVECTOR){0,0,0,0},kite_base_tim);
			drawtriangle(p3,p2,p4,(VECTOR){0,255,0},(VECTOR){255,0,0},(VECTOR){255,255,0},Ptrans,(SVECTOR){0,0,0,0},kite_base_tim);
		}
	}
}

void drawtriangle(SVECTOR p1, SVECTOR p2, SVECTOR p3, VECTOR uv1, VECTOR uv2, VECTOR uv3, VECTOR LPos, SVECTOR LRot, TIM_IMAGE texture){
		
		RECT tws = {0, 0, 0, 0};            // Texture window coordinates : x, y, w, h

		SVECTOR temp1={Rotate.vx+LRot.vx,Rotate.vy+LRot.vy,Rotate.vz+LRot.vz,Rotate.pad+LRot.pad};
		VECTOR temp2={Trans.vx+LPos.vx,Trans.vy+LPos.vy,Trans.vz+LPos.vz};

        TransMatrix(&Matrix,&temp2);
		RotMatrix(&temp1, &Matrix);
        ScaleMatrix(&Matrix, &Scale);
         
        SetTransMatrix(&Matrix);
		SetRotMatrix(&Matrix);

 			poly = (POLY_GT3 *)nextpri;

            SetPolyGT3(poly);

			((POLY_GT3 *)poly)->tpage = getTPage(texture.mode&0x3, 0,
                                                 texture.prect->x,
                                                 texture.prect->y
                                                );

            setRGB0(poly, 128,128,128);
            setRGB1(poly, 128,128,128);
            setRGB2(poly, 128,128,128);
            // Rotate, translate, and project the vectors and output the results into a primitive

			// If 8/4bpp, load CLUT to vram
                if ( (texture.mode & 0x3) < 2 ) {
                    setClut( poly,          
                             texture.crect->x,
                             texture.crect->y
                    );
                }
                // Set stpFlag
                SetSemiTrans(poly, 1);
			
            setUV3(poly, uv1.vx, uv1.vy,
                         uv2.vx, uv2.vy,
                         uv3.vx, uv3.vy);

            OTz  = RotTransPers(&p1, (long*)&poly->x0, &p, &Flag);
            OTz += RotTransPers(&p3, (long*)&poly->x1, &p, &Flag);
            OTz += RotTransPers(&p2, (long*)&poly->x2, &p, &Flag);
            
            // Sort the primitive into the OT
            OTz /= 3;
            if ((OTz > 0) && (OTz < OTLEN))
                AddPrim(&ot[db][OTz-2], poly);
            
            nextpri += sizeof(POLY_GT3);
            
            dr_mode = (DR_MODE *)nextpri;
            
            setDrawMode(dr_mode,1,0, getTPage(texture.mode&0x3, 0,
                                              texture.prect->x,
                                              texture.prect->y), &tws);  //set texture window
        
            AddPrim(&ot[db], dr_mode);
            
            nextpri += sizeof(DR_MODE);
}

void drawmodel(TMESH model, SVECTOR model_mesh[], int model_index[], VECTOR LPos, SVECTOR LRot, TIM_IMAGE texture){
	
		RECT tws = {0, 0, 0, 0};            // Texture window coordinates : x, y, w, h
		// Convert and set the matrixes
		SVECTOR temp1={Rotate.vx+LRot.vx,Rotate.vy+LRot.vy,Rotate.vz+LRot.vz,Rotate.pad+LRot.pad};
		VECTOR temp2={Trans.vx+LPos.vx,Trans.vy+LPos.vy,Trans.vz+LPos.vz};

        TransMatrix(&Matrix,&temp2);
		RotMatrix(&temp1, &Matrix);
        ScaleMatrix(&Matrix, &Scale);
         
        SetTransMatrix(&Matrix);
		SetRotMatrix(&Matrix);
        
		//RECT tws = {0, 0, 32, 32};
		
        
        // Render the sample vector model
        t=0;
        
        // modelSackboy is a TMESH, len member == # vertices, but here it's # of triangle... So, for each tri * 3 vertices ...
        for (i = 0; i < (model.len*3); i += 3) {               
            
            poly = (POLY_GT3 *)nextpri;

            
            // Initialize the primitive and set its color values
            
            SetPolyGT3(poly);

            ((POLY_GT3 *)poly)->tpage = getTPage(texture.mode&0x3, 0,
                                                 texture.prect->x,
                                                 texture.prect->y
                                                );

            setRGB0(poly, model.c[i].r/2 , model.c[i].g/2   , model.c[i].b/2);
            setRGB1(poly, model.c[i+2].r/2, model.c[i+2].g/2, model.c[i+2].b/2);
            setRGB2(poly, model.c[i+1].r/2, model.c[i+1].g/2, model.c[i+1].b/2);
			
			// If 8/4bpp, load CLUT to vram
                if ( (texture.mode & 0x3) < 2 ) {
                    setClut( poly,          
                             texture.crect->x,
                             texture.crect->y
                    );
                }
                // Set stpFlag
                SetSemiTrans(poly, 1);
			
            setUV3(poly, model.u[i].vx, model.u[i].vy,
                         model.u[i+2].vx, model.u[i+2].vy,
                         model.u[i+1].vx, model.u[i+1].vy);
                         
            // Rotate, translate, and project the vectors and output the results into a primitive

            OTz  = RotTransPers(&model_mesh[model_index[t+((t/3)*2)]]  , (long*)&poly->x0, &p, &Flag);
            OTz += RotTransPers(&model_mesh[model_index[t+((t/3)*2)+2]], (long*)&poly->x1, &p, &Flag);
            OTz += RotTransPers(&model_mesh[model_index[t+((t/3)*2)+1]], (long*)&poly->x2, &p, &Flag);
            
            // Sort the primitive into the OT
            OTz /= 3;
            if ((OTz > 0) && (OTz < OTLEN))
                AddPrim(&ot[db][OTz-2], poly);
            
            nextpri += sizeof(POLY_GT3);
            
            t+=3;
            
        }
        
            dr_mode = (DR_MODE *)nextpri;
            
            setDrawMode(dr_mode,1,0, getTPage(texture.mode&0x3, 0,
                                              texture.prect->x,
                                              texture.prect->y), &tws);  //set texture window
        
            AddPrim(&ot[db], dr_mode);
            
            nextpri += sizeof(DR_MODE);
        
}
