/**
 * d_a_bo.cpp
 * Enemy - Boko Baba
 */

#include "d/actor/d_a_bo.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

/* 000000EC-000001E8       .text smoke_set__FP8bo_class */
void smoke_set(bo_class*) {
    /* Nonmatching */
}

/* 000001E8-00000638       .text nodeCallBack_UP__FP7J3DNodei */
static BOOL nodeCallBack_UP(J3DNode*, int) {
    /* Nonmatching */
}

/* 00000638-000006C8       .text nodeCallBack_DW__FP7J3DNodei */
static BOOL nodeCallBack_DW(J3DNode*, int) {
    /* Nonmatching */
}

/* 000006C8-0000079C       .text execute__22yodare_ato_PcallBack_cFP14JPABaseEmitterP15JPABaseParticle */
void yodare_ato_PcallBack_c::execute(JPABaseEmitter*, JPABaseParticle*) {
    /* Nonmatching */
}

/* 0000079C-00000930       .text draw_SUB__FP8bo_class */
void draw_SUB(bo_class*) {
    /* Nonmatching */
}

/* 00000930-00000AD4       .text daBO_Draw__FP8bo_class */
static BOOL daBO_Draw(bo_class*) {
    /* Nonmatching */
}

/* 00000AD4-00000CD4       .text anm_init__FP8bo_classifUcfii */
void anm_init(bo_class*, int, float, unsigned char, float, int, int) {
    /* Nonmatching */
}

/* 00000CD4-00000E24       .text shock_damage_check__FP8bo_class */
void shock_damage_check(bo_class*) {
    /* Nonmatching */
}

/* 00000E24-000013A4       .text head_atari_check__FP8bo_class */
void head_atari_check(bo_class*) {
    /* Nonmatching */
}

/* 000013A4-0000170C       .text nokezori_damage_rtn__FP8bo_class */
void nokezori_damage_rtn(bo_class*) {
    /* Nonmatching */
}

/* 0000170C-00001BB8       .text body_atari_check__FP8bo_class */
void body_atari_check(bo_class*) {
    /* Nonmatching */
}

/* 00001BB8-00002048       .text damage_check__FP8bo_class */
void damage_check(bo_class*) {
    /* Nonmatching */
}

/* 00002048-00002080       .text angle_initial__FP8bo_class */
void angle_initial(bo_class*) {
    /* Nonmatching */
}

/* 00002080-00002170       .text wait_initial__FP8bo_class */
void wait_initial(bo_class*) {
    /* Nonmatching */
}

/* 00002170-000022F8       .text start_bakutsuki_event_camera__FP10fopAc_ac_c */
void start_bakutsuki_event_camera(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 000022F8-0000233C       .text end_event_camera__FP10fopAc_ac_c */
void end_event_camera(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 0000233C-00002FC4       .text bo_move__FP8bo_class */
void bo_move(bo_class*) {
    /* Nonmatching */
}

/* 00002FC4-0000380C       .text bo2_move__FP8bo_class */
void bo2_move(bo_class*) {
    /* Nonmatching */
}

/* 0000380C-00003AD4       .text bo3_move__FP8bo_class */
void bo3_move(bo_class*) {
    /* Nonmatching */
}

/* 00003AD4-00003E8C       .text bo4_move__FP8bo_class */
void bo4_move(bo_class*) {
    /* Nonmatching */
}

/* 00003E8C-000042B8       .text bo5_move__FP8bo_class */
void bo5_move(bo_class*) {
    /* Nonmatching */
}

/* 000042B8-000048B0       .text daBO_Execute__FP8bo_class */
static BOOL daBO_Execute(bo_class*) {
    /* Nonmatching */
}

/* 000048B0-000048B8       .text daBO_IsDelete__FP8bo_class */
static BOOL daBO_IsDelete(bo_class*) {
    /* Nonmatching */
}

/* 000048B8-00004998       .text daBO_Delete__FP8bo_class */
static BOOL daBO_Delete(bo_class*) {
    /* Nonmatching */
}

/* 00004998-00004D08       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 00004D50-000050DC       .text daBO_Create__FP10fopAc_ac_c */
static cPhs_State daBO_Create(fopAc_ac_c*) {
    /* Nonmatching */
}

static actor_method_class l_daBO_Method = {
    (process_method_func)daBO_Create,
    (process_method_func)daBO_Delete,
    (process_method_func)daBO_Execute,
    (process_method_func)daBO_IsDelete,
    (process_method_func)daBO_Draw,
};

actor_process_profile_definition g_profile_BO = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_BO,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(bo_class),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_BO,
    /* Actor SubMtd */ &l_daBO_Method,
    /* Status       */ fopAcStts_SHOWMAP_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
