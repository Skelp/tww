/**
 * d_a_obj_ojtree.cpp
 * Object - Tall thin poles leading to the Deku Leaf (Forest Haven interior)
 */

#include "d/res/res_ojtree.h"
#include "d/actor/d_a_obj_ojtree.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_mtx.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

Mtx daObjOjtree::Act_c::M_tmp_mtx;
const char daObjOjtree::Act_c::M_arcname[] = "Ojtree";

/* 00000078-0000012C       .text CreateHeap__Q211daObjOjtree5Act_cFv */
BOOL daObjOjtree::Act_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(M_arcname, OJTREE_BDL_OJTREE);
    JUT_ASSERT(67, model_data != NULL);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000022);
    return !!mpModel;
}

/* 0000012C-000001EC       .text Create__Q211daObjOjtree5Act_cFv */
BOOL daObjOjtree::Act_c::Create() {
    cXyz pos;

    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    init_mtx();
    fopAcM_setCullSizeBox(this, -500.0f, -1.0f, -300.0f, 251.0f, 5001.0f, 251.0f);
    pos.set(current.pos);
    pos.y += 5000.0f;
    fopAcM_create(PROC_JBO, 0, &pos, home.roomNo, &shape_angle, NULL, 0xff, NULL);
    mLockTimer = 2;
    return TRUE;
}

/* 000001EC-000002E4       .text Mthd_Create__Q211daObjOjtree5Act_cFv */
cPhs_State daObjOjtree::Act_c::Mthd_Create() {
    fopAcM_SetupActor(this, daObjOjtree::Act_c);
       
    cPhs_State phase_state = dComIfG_resLoad(&mPhs, M_arcname);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(M_arcname, OJTREE_DZB_OJTREE, NULL, 0x26a0);
        JUT_ASSERT(123, (phase_state == cPhs_COMPLEATE_e) || (phase_state == cPhs_ERROR_e));
    }
    
    return phase_state;
}

/* 000002E4-000002EC       .text Delete__Q211daObjOjtree5Act_cFv */
BOOL daObjOjtree::Act_c::Delete() {
    return TRUE;
}

/* 000002EC-00000338       .text Mthd_Delete__Q211daObjOjtree5Act_cFv */
BOOL daObjOjtree::Act_c::Mthd_Delete() {
    s32 result = MoveBGDelete();
    dComIfG_resDelete(&mPhs, M_arcname);
    return result;
}

/* 00000338-000003B8       .text set_mtx__Q211daObjOjtree5Act_cFv */
void daObjOjtree::Act_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), M_tmp_mtx);
}

/* 000003B8-000003F4       .text init_mtx__Q211daObjOjtree5Act_cFv */
void daObjOjtree::Act_c::init_mtx() {
    mpModel->setBaseScale(scale);
    set_mtx();
}

/* 000003F4-0000045C       .text Execute__Q211daObjOjtree5Act_cFPPA3_A4_f */
BOOL daObjOjtree::Act_c::Execute(Mtx** pMtx) {
    if(mLockTimer != 0)
    {
        if (--mLockTimer == 0) 
        {
           mpBgW->SetLock(); 
        }
    }
    set_mtx();
    *pMtx = &M_tmp_mtx;
    return TRUE;
}

/* 0000045C-000004FC       .text Draw__Q211daObjOjtree5Act_cFv */
BOOL daObjOjtree::Act_c::Draw() {
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return TRUE;
}

namespace daObjOjtree {
    namespace {
        cPhs_State Mthd_Create(void* i_this) {
            return static_cast<Act_c*>(i_this)->Mthd_Create();
        }
        
        BOOL Mthd_Delete(void* i_this) {
            return static_cast<Act_c*>(i_this)->Mthd_Delete();
        }
        
        BOOL Mthd_Execute(void* i_this) {
            return static_cast<Act_c*>(i_this)->MoveBGExecute();
        }
        
        BOOL Mthd_Draw(void* i_this) {
            return static_cast<Act_c*>(i_this)->MoveBGDraw();
        }
        
        BOOL Mthd_IsDelete(void* i_this) {
            return static_cast<Act_c*>(i_this)->MoveBGIsDelete();
        }

        static actor_method_class Mthd_Table = {
            (process_method_func)Mthd_Create,
            (process_method_func)Mthd_Delete,
            (process_method_func)Mthd_Execute,
            (process_method_func)Mthd_IsDelete,
            (process_method_func)Mthd_Draw,
        };
    }
}

actor_process_profile_definition g_profile_Obj_Ojtree = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Ojtree,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjOjtree::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Ojtree,
    /* Actor SubMtd */ &daObjOjtree::Mthd_Table,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
