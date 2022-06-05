
SVECTOR kite_base_mesh[] = {
	{ -68,7,-64,0 },
	{ -41,-13,-59,0 },
	{ 0,2,-65,0 },
	{ 69,7,-64,0 },
	{ 42,-13,-59,0 },
	{ 0,6,65,0 }
};

SVECTOR kite_base_normal[] = {
	2457,3022,-1266, 0,
	1013,3864,-904, 0,
	0,4094,-118, 0,
	-2457,3022,-1266, 0,
	-1013,3864,-904, 0,
	0,4046,-636, 0
};

SVECTOR kite_base_uv[] = {
	1,60, 0, 0,
	60,60, 0, 0,
	57,3, 0, 0,
	3,126, 0, 0,
	61,126, 0, 0,
	61,65, 0, 0,
	126,59, 0, 0,
	72,57, 0, 0,
	69,4, 0, 0,
	124,124, 0, 0,
	69,124, 0, 0,
	72,64, 0, 0
};

CVECTOR kite_base_color[] = {
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

int kite_base_index[] = {
	0,1,5,0,4,
	1,2,5,0,4,
	3,5,4,0,4,
	4,5,2,0,4
};

TMESH kite_base = {
	kite_base_mesh,
    kite_base_normal,
    kite_base_uv,
    kite_base_color,
    4
};

extern unsigned long _binary_TIM_kitebase_tim_start[];
extern unsigned long _binary_TIM_kitebase_tim_end[];
extern unsigned long _binary_TIM_kitebase_tim_length;

TIM_IMAGE kite_base_tim;
