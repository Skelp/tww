//
// Generated by dtk
// Translation Unit: d_a_magma.cpp
//

#include "d/actor/d_a_magma.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_com_inf_game.h"
#include "d/d_magma.h"

daMagma_c::~daMagma_c() {
    dComIfG_resDelete(&mPhs, "Magma");
}

cPhs_State daMagma_c::create() {
    fopAcM_SetupActor(this, daMagma_c);

    cPhs_State result = dComIfG_resLoad(&mPhs, "Magma");
    if (result != cPhs_COMPLEATE_e) {
        return result;
    }

    if (dComIfGp_createMagma()) {
        dComIfGp_getMagma()->newFloor(
            current.pos,
            scale,
            current.roomNo,
            getPathNo()
        );
    }

    return cPhs_ERROR_e;
}

/* 00000078-00000080       .text daMagma_IsDelete__FP9daMagma_c */
static BOOL daMagma_IsDelete(daMagma_c* i_this) {
    return TRUE;
}

/* 00000080-000000CC       .text daMagma_Delete__FP9daMagma_c */
static BOOL daMagma_Delete(daMagma_c* i_this) {
    i_this->~daMagma_c();
    return TRUE;
}

/* 000000CC-00000178       .text daMagma_Create__FP10fopAc_ac_c */
static cPhs_State daMagma_Create(fopAc_ac_c* i_this) {
    return ((daMagma_c*)i_this)->create();
}

static actor_method_class l_daMagma_Method = {
    (process_method_func)daMagma_Create,
    (process_method_func)daMagma_Delete,
    (process_method_func)NULL,
    (process_method_func)daMagma_IsDelete,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_MAGMA = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_MAGMA,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daMagma_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_MAGMA,
    /* Actor SubMtd */ &l_daMagma_Method,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
