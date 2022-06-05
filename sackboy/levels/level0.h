#pragma once
#include "../custom_types.h"
#include "../include/defines.h"

extern LEVEL level0;
extern CVECTOR level0_BGc;
extern VECTOR level0_BKc;
extern CAMPATH level0_camPath;
extern MATRIX level0_lgtmat;
extern MATRIX level0_cmat;
extern SVECTOR modelPlane_mesh[];
extern SVECTOR level0_modelPlane_normal[];
extern CVECTOR level0_modelPlane_color[];
extern PRIM level0_modelPlane_index[];
extern BODY  level0_modelPlane_body;
extern TMESH level0_modelPlane;
extern MESH level0_meshPlane;
extern MESH * level0_meshes[1];
extern int level0_meshes_length;
extern CAMANGLE level0_camAngle_NULL;
extern CAMANGLE * level0_camAngles[0];
extern MESH * level0_actorPtr;
extern MESH * level0_levelPtr;
extern MESH * level0_propPtr;
extern CAMANGLE * level0_camPtr;
extern NODE * level0_curNode;
