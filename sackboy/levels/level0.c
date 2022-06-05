#include "level0.h"

NODE_DECLARATION
CVECTOR level0_BGc = { 111, 111, 111, 0 };

VECTOR level0_BKc = { 63, 63, 63, 0 };

MESH_ANIMS_TRACKS level0_modelPlane_anims = {
	1,
	{
		&level0_modelPlane_anim_Kite_base|Kite_base|PlaneAction|Kite_base|PlaneAction
	}
};

VANIM  level0_modelPlane_anim_Kite_base|Kite_base|PlaneAction|Kite_base|PlaneAction = {
	1, // number of frames e.g   20
	6, // number of vertices e.g 21
	-1, // anim cursor : -1 means not playing back
	0,  // lerp cursor
	0,  // loop : if -1 , infinite loop, if n > 0, loop n times
	1,  // playback direction (1 or -1)
	0,  // ping pong animation (A>B>A)
	0, // use lerp to interpolate keyframes
	{   // vertex pos as BVECTORs e.g 20 * 21 BVECTORS
		//Frame 0
		{ -68,6,-65 },
		{ -42,-13,-60 },
		{ 0,1,-65 },
		{ 68,6,-65 },
		{ 42,-13,-60 },
		{ 0,5,65 }
	}
};

CAMPATH level0_camPath = {
	0,
	0,
	0,
	{0}
};

MATRIX level0_lgtmat = {
	0, 0, 0,
	0, 0, 0,
	0, 0, 0
	};

MATRIX level0_cmat = {
//   L1   L2   L3
	0, 0, 0, // R
	0, 0, 0, // G
	0, 0, 0  // B
	};

SVECTOR level0_modelPlane_mesh[] = {
	{ -68,7,-64,0 },
	{ -41,-13,-59,0 },
	{ 0,2,-65,0 },
	{ 69,7,-64,0 },
	{ 42,-13,-59,0 },
	{ 0,6,65,0 }
};

SVECTOR level0_modelPlane_normal[] = {
	2457,3022,-1266, 0,
	1013,3864,-904, 0,
	0,4094,-118, 0,
	-2457,3022,-1266, 0,
	-1013,3864,-904, 0,
	0,4046,-636, 0
};

CVECTOR level0_modelPlane_color[] = {
	80, 80, 80, 0,
	128, 128, 128, 0,
	128, 128, 128, 0,
	80, 80, 80, 0,
	128, 128, 128, 0,
	128, 128, 128, 0,
	80, 80, 80, 0,
	128, 128, 128, 0,
	128, 128, 128, 0,
	80, 80, 80, 0,
	128, 128, 128, 0,
	128, 128, 128, 0
};

PRIM level0_modelPlane_index[] = {
	0,1,5,0,4,
	1,2,5,0,4,
	3,5,4,0,4,
	4,5,2,0,4
};

BODY level0_modelPlane_body = {
	{0, 0, 0, 0},
	0,0,0, 0,
	0,0,0, 0,
	10,
	ONE/10,
	-68,-13,-65, 0,
	68,6,65, 0,
	0,
	};

TMESH level0_modelPlane = {
	level0_modelPlane_mesh,
	level0_modelPlane_normal,
	0,
	level0_modelPlane_color, 
	4
};

MESH level0_meshPlane = {
	12,
	&level0_modelPlane,
	level0_modelPlane_index,
	0,
	0,
	{0}, // Matrix
	{0,0,0, 0}, // position
	{0,0,0, 0}, // rotation
	0, // isProp
	0, // isRigidBody
	0, // isStaticBody
	0, // isRound 
	0, // isPrism
	0, // isAnim
	0, // isActor
	0, // isLevel
	0, // isWall
	0, // isBG
	0, // isSprite
	0, // p
	0, // otz
	&level0_modelPlane_body,
	&level0_modelPlane_anims, // Mesh anim tracks
	0, // Current VANIM
	subs_Plane,
	0 // Screen space coordinates
};

MESH * level0_meshes[1] = {
	&level0_meshPlane
}; 

int level0_meshes_length = 1;

CAMANGLE level0_camAngle_NULL = {
	&level0_camPos_NULL,
	0,
	 0,
	 { 0 },
	 { 0 },
	 0,
	 0
};

CAMANGLE * level0_camAngles[0] = {
};

MESH * level0_actorPtr = &level0_meshCube;
