/**
 * d_a_floor.cpp
 * Object - Wind Temple - Breakable floor (Iron Boots, Bombs, etc.)
 */

#include "d/actor/d_a_floor.h"
#include "d/res/res_hhyu1.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "m_Do/m_Do_mtx.h"

namespace daFloor_prm {
    inline u32 getSwitchNo(daFloor_c* ac) { return (fopAcM_GetParam(ac) >> 0) & 0xFF; }
};

const char daFloor_c::m_arcname[6] = "Hhyu1";

/* 00000078-000000DC       .text rideCallBack__FP4dBgWP10fopAc_ac_cP10fopAc_ac_c */
void rideCallBack(dBgW*, fopAc_ac_c* i_this, fopAc_ac_c* i_other) {
    if (fopAcM_GetName(i_other) == PROC_PLAYER && !((daPy_py_c*)i_other)->checkEquipHeavyBoots()) {
        ((daFloor_c*)i_this)->field_0x2d9 = 1;
    } else if (fopAcM_GetName(i_other) == PROC_PLAYER && ((daFloor_c*)i_this)->field_0x2d9 && ((daPy_py_c*)i_other)->checkEquipHeavyBoots() && !((daFloor_c*)i_this)->field_0x2d8) {
        ((daFloor_c*)i_this)->field_0x2d8 = 1;
        ((daFloor_c*)i_this)->field_0x2da = 6;
    }
}

/* 000000DC-0000012C       .text Delete__9daFloor_cFv */
BOOL daFloor_c::Delete() {
    mSmokeCallBack.end();
    dComIfG_resDelete(&mPhs, m_arcname);
    return TRUE;
}

/* 0000012C-000001E8       .text CreateHeap__9daFloor_cFv */
BOOL daFloor_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(m_arcname, HHYU1_BDL_HHYU1);
    JUT_ASSERT(0xc1, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000022);
    if (mpModel == NULL)
        return FALSE;
    return TRUE;
}

/* 000001E8-0000025C       .text Create__9daFloor_cFv */
BOOL daFloor_c::Create() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -100.0f, -50.0f, -100.0f, 100.0f, 50.0f, 100.0f);
    set_mtx();
    mpBgW->SetRideCallback(rideCallBack);
    return TRUE;
}

/* 0000025C-00000354       .text _create__9daFloor_cFv */
cPhs_State daFloor_c::_create() {
    fopAcM_SetupActor(this, daFloor_c);

    mSwitchNo = daFloor_prm::getSwitchNo(this);
    if (mSwitchNo != 0xFF && dComIfGs_isSwitch(mSwitchNo, fopAcM_GetHomeRoomNo(this)))
        return cPhs_ERROR_e;

    cPhs_State rt = dComIfG_resLoad(&mPhs, m_arcname);
    if (rt == cPhs_COMPLEATE_e) {
        if (!MoveBGCreate(m_arcname, HHYU1_DZB_HHYU1, NULL, 0x8A0))
            return cPhs_ERROR_e;
    }
    return rt;
}

/* 00000354-000003D4       .text set_mtx__9daFloor_cFv */
void daFloor_c::set_mtx() {
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 000003D4-00000548       .text Execute__9daFloor_cFPPA3_A4_f */
BOOL daFloor_c::Execute(Mtx**) {
    if (field_0x2da != 0) {
        field_0x2da--;
        if (field_0x2da == 0) {
            set_effect();
            fopAcM_seStart(this, JA_SE_OBJ_HEAVY_FLOOR_BRK, 0);
            fopAcM_onSwitch(this, mSwitchNo);
            fopAcM_delete(this);
        }
    }

    if ((dComIfGp_getPlayer(0)->current.pos - dComIfGp_getPlayer(0)->old.pos).absXZ() != 0.0f)
        field_0x2d9 = 0;

    return TRUE;
}

/* 00000548-00000640       .text set_effect__9daFloor_cFv */
void daFloor_c::set_effect() {
    dComIfGp_particle_set(dPa_name::ID_SCENE_81A6, &current.pos, &current.angle);
    JPABaseEmitter* emtr = dComIfGp_particle_set(dPa_name::ID_COMMON_2027, &current.pos, &current.angle, NULL, 255, &mSmokeCallBack, fopAcM_GetRoomNo(this));
    if (emtr != NULL) {
        emtr->setRate(30);
        emtr->setMaxFrame(1);
        emtr->setEmitterScale(JGeometry::TVec3<f32>(1.0f, 0.5f, 1.0f));
        emtr->setGlobalParticleScale(JGeometry::TVec3<f32>(3.0f, 3.0f, 3.0f));
    }
}

/* 00000640-000006E0       .text Draw__9daFloor_cFv */
BOOL daFloor_c::Draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return TRUE;
}

/* 000006E0-00000700       .text daFloor_Create__FPv */
static cPhs_State daFloor_Create(void* i_this) {
    return ((daFloor_c*)i_this)->_create();
}

/* 00000700-00000720       .text daFloor_Delete__FPv */
static BOOL daFloor_Delete(void* i_this) {
    return ((daFloor_c*)i_this)->MoveBGDelete();
}

/* 00000720-0000074C       .text daFloor_Draw__FPv */
static BOOL daFloor_Draw(void* i_this) {
    return ((daFloor_c*)i_this)->MoveBGDraw();
}

/* 0000074C-0000076C       .text daFloor_Execute__FPv */
static BOOL daFloor_Execute(void* i_this) {
    return ((daFloor_c*)i_this)->MoveBGExecute();
}

/* 0000076C-00000774       .text daFloor_IsDelete__FPv */
static BOOL daFloor_IsDelete(void* i_this) {
    return TRUE;
}

static actor_method_class daFloorMethodTable = {
    (process_method_func)daFloor_Create,
    (process_method_func)daFloor_Delete,
    (process_method_func)daFloor_Execute,
    (process_method_func)daFloor_IsDelete,
    (process_method_func)daFloor_Draw,
};

actor_process_profile_definition g_profile_FLOOR = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_FLOOR,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daFloor_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_FLOOR,
    /* Actor SubMtd */ &daFloorMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
