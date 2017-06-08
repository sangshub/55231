#pragma once

const int MIN_STR = 64;
const int MID_STR = 128;
const int MAX_STR = 256;

const WORD WINCX = 1024;
const WORD WINCY = 768;

const WORD VTXCNTX = 129;
const WORD VTXCNTZ = 129;
const WORD VTXITV = 1;
const WORD TILESIZE = VTXITV<<1;

const D3DXVECTOR3 g_vLook = D3DXVECTOR3(0.f, 0.f, 1.f);

// Å°°ªÀÔ´Ï´Ù.
const DWORD	KEY_LEFT	= 0x00000001;
const DWORD	KEY_RIGHT	= 0x00000002;
const DWORD	KEY_UP		= 0x00000004;
const DWORD	KEY_DOWN	= 0x00000008;
const DWORD	KEY_SPACE	= 0x00000010;
const DWORD	KEY_LBUTTON	= 0x00000020;
const DWORD	KEY_RBUTTON	= 0x00000040;

// Å¥ºêÅ¸ÀÔ
const DWORD	CUBE_BROK	= 0x00000001; // »ð»Ñ°³Áü
const DWORD	CUBE_BOOM	= 0x00000002; // ÆøÅº»Ñ°³Áü
const DWORD	CUBE_NOTB	= 0x00000004; // ¾È»Ñ°³Áü
const DWORD	CUBE_ITEM	= 0x00000008;
const DWORD	CUBE_THRO	= 0x00000010;
const DWORD	CUBE_EVEN1	= 0x00000020;
const DWORD	CUBE_EVEN2	= 0x00000040;

// ¾ÆÀÌÅÛ Å¸ÀÔ
const DWORD ITEM_ATTACK		= 0x00000001;
const DWORD ITEM_BODY		= 0x00000002;
const DWORD ITEM_BOMB		= 0x00000004;
const DWORD ITEM_FEET		= 0x00000008;
const DWORD ITEM_HEAD		= 0x00000010;
const DWORD ITEM_HOLSTER	= 0x00000020;
const DWORD ITEM_ITEM		= 0x00000040;
const DWORD ITEM_KEY		= 0x00000080;
const DWORD ITEM_MISC		= 0x00000100;
const DWORD ITEM_PACK		= 0x00000200;
const DWORD ITEM_REROAD		= 0x00000400;
const DWORD ITEM_SHOVEL		= 0x00000800;
const DWORD ITEM_SPELL		= 0x00001000;
const DWORD ITEM_TORCH		= 0x00002000;
const DWORD ITEM_GOLD		= 0x00004000;


//Obj Çàµ¿ Å°°ª /////////////////////
const DWORD	STATE_IDLE		= 0x00000001;
const DWORD	STATE_MOVE		= 0x00000002;
const DWORD	STATE_ATT		= 0x00000004;
const DWORD	STATE_HIT		= 0x00000008;
const DWORD	STATE_RROT		= 0x00000010;
const DWORD	STATE_LROT		= 0x00000020;
const DWORD	STATE_SHOVEL	= 0x00000040;
const DWORD	STATE_MOVING	= 0x00000080;
const DWORD	STATE_MONGATT	= 0x00000100;
const DWORD	STATE_JUMP		= 0x00000200;
const DWORD	STATE_MAPJUMP	= 0x00000400;

// º¸½º Çàµ¿
const DWORD BOSS_IDLE = 0x00000001;
const DWORD BOSS_HIDE = 0x00000002;
const DWORD BOSS_APPEAR = 0x00000004;
const DWORD BOSS_DIE = 0x00000008;

// ¸Þ¸ð¸® ÃÖÀûÈ­¸¦ À§ÇÑ Àü¿ª ¼±¾ð
extern const int MIN_STR;
extern const int MID_STR;
extern const int MAX_STR;

extern const WORD WINCX;
extern const WORD WINCY;

extern const WORD VTXCNTX;
extern const WORD VTXCNTZ;
extern const WORD VTXITV;
extern const WORD TILESIZE;

extern const D3DXVECTOR3 g_vLook;

extern const DWORD KEY_LEFT;
extern const DWORD KEY_RIGHT;
extern const DWORD KEY_UP;
extern const DWORD KEY_DOWN;
extern const DWORD KEY_SPACE;
extern const DWORD KEY_LBUTTON;
extern const DWORD KEY_RBUTTON;

extern const DWORD CUBE_BROK;
extern const DWORD CUBE_BOOM;
extern const DWORD CUBE_NOTB;
extern const DWORD CUBE_ITEM;
extern const DWORD CUBE_THRO;
extern const DWORD CUBE_EVEN1;
extern const DWORD CUBE_EVEN2;

extern const DWORD ITEM_ATTACK;
extern const DWORD ITEM_BODY;
extern const DWORD ITEM_BOMB;
extern const DWORD ITEM_FEET;
extern const DWORD ITEM_HEAD;
extern const DWORD ITEM_HOLSTER;
extern const DWORD ITEM_ITEM;
extern const DWORD ITEM_KEY;
extern const DWORD ITEM_MISC;
extern const DWORD ITEM_PACK;
extern const DWORD ITEM_REROAD;
extern const DWORD ITEM_SHOVEL;
extern const DWORD ITEM_SPELL;
extern const DWORD ITEM_TORCH;
extern const DWORD ITEM_GOLD;

extern const DWORD	STATE_IDLE;
extern const DWORD	STATE_MOVE;
extern const DWORD	STATE_ATT;
extern const DWORD	STATE_HIT;
extern const DWORD	STATE_RROT;
extern const DWORD	STATE_LROT;
extern const DWORD	STATE_SHOVEL;
extern const DWORD	STATE_MOVING;
extern const DWORD	STATE_JUMP;
extern const DWORD	STATE_MAPJUMP;

extern const DWORD BOSS_IDLE;
extern const DWORD BOSS_HIDE;
extern const DWORD BOSS_APPEAR;
extern const DWORD BOSS_DIE;