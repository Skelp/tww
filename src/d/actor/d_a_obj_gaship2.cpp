/**
 * d_a_obj_gaship2.cpp
 * Object - Ganon Room (destroyed) (Forsaken Fortress 3)
 */

#include "d/actor/d_a_obj_gaship2.h"
#include "d/res/res_yakerom.h"
#include "SSystem/SComponent/c_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "m_Do/m_Do_mtx.h"

const char daObjGaship2::Act_c::M_arcname[] = "YakeRom";

/* 00000078-0000009C       .text solidHeapCB__Q212daObjGaship25Act_cFP10fopAc_ac_c */
BOOL daObjGaship2::Act_c::solidHeapCB(fopAc_ac_c *i_this) {
    return ((Act_c *) i_this)->create_heap();
}

/* 0000009C-00000220       .text create_heap__Q212daObjGaship25Act_cFv */
bool daObjGaship2::Act_c::create_heap() {
    J3DModelData *mdl_data;
    cBgD_t *bgw_data;

    mdl_data = (J3DModelData *) (dComIfG_getObjectRes(M_arcname, YAKEROM_BDL_YAKEROM));
    JUT_ASSERT(0x5A, mdl_data != NULL);

    mpModel = mDoExt_J3DModel__create(mdl_data, 0, 0x11000002);
    set_mtx();
    bgw_data = (cBgD_t *) (dComIfG_getObjectRes(M_arcname, YAKEROM_DZB_YAKEROM));
    JUT_ASSERT(0x67, bgw_data != NULL);
    if (bgw_data != NULL) {
        mpBgW = new dBgW();
        if (mpBgW != NULL && (mpBgW->Set(bgw_data, cBgW::MOVE_BG_e, &mMtx) == true)) {
            return false;
        }
    }

    return mdl_data != NULL && mpModel != NULL && bgw_data != NULL && mpBgW != NULL;
}

/* 00000220-000002F8       .text _create__Q212daObjGaship25Act_cFv */
cPhs_State daObjGaship2::Act_c::_create() {
    fopAcM_SetupActor(this, Act_c);
    cPhs_State phase_state = dComIfG_resLoad(&mphs, M_arcname);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0x0)) {
            fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
            dComIfG_Bgsp()->Regist(mpBgW, this);
            mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
        } else {
            phase_state = cPhs_ERROR_e;
        }
    }
    return phase_state;
}

/* 000002F8-00000384       .text _delete__Q212daObjGaship25Act_cFv */
bool daObjGaship2::Act_c::_delete() {
    if (heap != NULL && mpBgW != NULL) {
        if (mpBgW->ChkUsed()) {
            dComIfG_Bgsp()->Release(mpBgW);
        }
    }

    dComIfG_resDelete(&mphs, M_arcname);
    return true;
}

/* 00000384-00000430       .text set_mtx__Q212daObjGaship25Act_cFv */
void daObjGaship2::Act_c::set_mtx() {
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mMtx);
    mpModel->calc();
}

/* 00000430-00000468       .text _execute__Q212daObjGaship25Act_cFv */
bool daObjGaship2::Act_c::_execute() {
    set_mtx();
    mpBgW->Move();
    return TRUE;
}

/* 00000468-00000508       .text _draw__Q212daObjGaship25Act_cFv */
bool daObjGaship2::Act_c::_draw() {
    dKy_getEnvlight().settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    dComIfGd_setListBG();
    dKy_getEnvlight().setLightTevColorType(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return TRUE;
};

namespace daObjGaship2 {
    namespace {
        cPhs_State Mthd_Create(void *i_this) {
            return ((daObjGaship2::Act_c *) i_this)->_create();
        }

        BOOL Mthd_Delete(void *i_this) {
            return ((daObjGaship2::Act_c *) i_this)->_delete();
        }

        BOOL Mthd_Execute(void *i_this) {
            return ((daObjGaship2::Act_c *) i_this)->_execute();
        }

        BOOL Mthd_Draw(void *i_this) {
            return ((daObjGaship2::Act_c *) i_this)->_draw();
        }

        BOOL Mthd_IsDelete(void *i_this) {
            return TRUE;
        }

        static actor_method_class Mthd_Table = {
            (process_method_func) Mthd_Create,
            (process_method_func) Mthd_Delete,
            (process_method_func) Mthd_Execute,
            (process_method_func) Mthd_IsDelete,
            (process_method_func) Mthd_Draw,
        };
    }
}

actor_process_profile_definition g_profile_Obj_Gaship2 = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Gaship2,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjGaship2::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Gaship2,
    /* Actor SubMtd */ &daObjGaship2::Mthd_Table,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
