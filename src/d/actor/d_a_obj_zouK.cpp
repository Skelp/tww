/**
 * d_a_obj_zouK.cpp
 * Object - Hyrule Castle - Knight statues (Master Sword chamber)
 */

#include "d/actor/d_a_obj_zouK.h"
#include "d/res/res_vzouk.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_rain.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/JUtility/JUTAssert.h"

namespace daObjZouk {
    namespace {
        static const Attr_c L_attr = {
            288.0f, 1200.0f, 530.0f,
            418.0f, 0.0f, 500.0f,
            26.0f, 235.0f, 1323.0f,
            0.0f, 1003.0f, 305.0f,
        };

        inline const Attr_c & attr() { return L_attr; }
    }

    static const dCcD_SrcCyl M_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ ~(AT_TYPE_LIGHT | AT_TYPE_UNK400000 | AT_TYPE_WIND | AT_TYPE_UNK20000 | AT_TYPE_WATER),
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_GrpAll_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ 0,
            /* SrcGObjAt Spl     */ 0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ 0,
            /* SrcGObjTg HitMark */ 0,
            /* SrcGObjTg Spl     */ 0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_Shield_e | dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGCylS
        {
            /* Center */ 0.0f, 0.0f, 0.0f,
            /* Radius */ 100.0f,
            /* Height */ 200.0f,
        },
    };
}

const char daObjZouk::Act_c::M_arcname[6] = "VzouK";

/* 00000078-0000009C       .text solidHeapCB__Q29daObjZouk5Act_cFP10fopAc_ac_c */
BOOL daObjZouk::Act_c::solidHeapCB(fopAc_ac_c* i_this) {
    return ((Act_c*)i_this)->create_heap();
}

/* 0000009C-00000468       .text create_heap__Q29daObjZouk5Act_cFv */
bool daObjZouk::Act_c::create_heap() {
    J3DModelData* mdl_data = (J3DModelData*)dComIfG_getObjectRes(M_arcname, VZOUK_INDEX_BDL_VZOUK);
    JUT_ASSERT(0x171, mdl_data != NULL);
    M_bck_data = (J3DAnmTransformKey*)dComIfG_getObjectRes(M_arcname, VZOUK_INDEX_BCK_VZOUK);
    JUT_ASSERT(0x175, M_bck_data != NULL);
    if (mdl_data != NULL && M_bck_data != NULL) {
        M_anm = new mDoExt_McaMorf(
            mdl_data,
            NULL, NULL,
            M_bck_data,
            J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, 1,
            NULL,
            0x00000000,
            0x11020203
        );
    }
    JUT_ASSERT(0x183, M_anm != NULL);
    if (M_anm != NULL) {
        if (dComIfGs_isCollect(0, 1)) {
            M_anm->setPlaySpeed(0.0f);
            M_anm->setFrame(M_bck_data->getFrameMax() - 1.0f);
        } else {
            M_anm->setPlaySpeed(0.0f);
            M_anm->setFrame(0.0f);
        }
    }
    set_mtx();

    cBgD_t* bgw_data_before = (cBgD_t*)dComIfG_getObjectRes(M_arcname, VZOUK_INDEX_DZB_MAEKISI);
    JUT_ASSERT(0x196, bgw_data_before != NULL);
    if (bgw_data_before != NULL) {
        mBgBefore = new dBgW();
        if (mBgBefore != NULL) {
            if (mBgBefore->Set(bgw_data_before, dBgW::MOVE_BG_e, &mBgMtx) == true)
                return false;
        }
    }

    cBgD_t* bgw_data_after = (cBgD_t*)dComIfG_getObjectRes(M_arcname, VZOUK_INDEX_DZB_ATOKISI);
    JUT_ASSERT(0x1a4, bgw_data_after != NULL);
    if (bgw_data_after != NULL) {
        mBgAfter = new dBgW();
        if (mBgAfter != NULL) {
            if (mBgAfter->Set(bgw_data_after, dBgW::MOVE_BG_e, &mBgMtx) == true)
                return false;
        }
    }

    bool ret = false;
    if (mdl_data != NULL && M_bck_data != NULL && M_anm != NULL && mBgBefore != NULL && mBgAfter != NULL)
        ret = true;
    return ret;
}

/* 00000468-00000724       .text _create__Q29daObjZouk5Act_cFv */
cPhs_State daObjZouk::Act_c::_create() {
    fopAcM_SetupActor(this, Act_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, M_arcname);

    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0x0)) {
            fopAcM_SetMtx(this, M_anm->getModel()->getBaseTRMtx());
            fopAcM_setCullSizeBox(this, -1000.0f, -0.0f, -1000.0f, 1000.0f, 2800.0f, 1000.0f);
            cXyz gndPos(current.pos.x, current.pos.y + 100.0f, current.pos.z);
            mGndChk.SetPos(&gndPos);
            mGndChk.SetActorPid(fopAcM_GetID(this));
            mGndY = dComIfG_Bgsp()->GroundCross(&mGndChk);

            mStts0.Init(0xFF, 0xFF, this);
            mCyl0.Set(M_cyl_src);
            mCyl0.SetStts(&mStts0);
            mCyl0.SetTgVec((cXyz&)cXyz::Zero);
            mCyl0.OnTgNoHitMark();

            mStts1.Init(0xFF, 0xFF, this);
            mCyl1.Set(M_cyl_src);
            mCyl1.SetStts(&mStts1);
            mCyl1.SetTgVec((cXyz&)cXyz::Zero);
            mCyl1.OnTgNoHitMark();

            mStts2.Init(0xFF, 0xFF, this);
            mCyl2.Set(M_cyl_src);
            mCyl2.SetStts(&mStts2);
            mCyl2.SetTgVec((cXyz&)cXyz::Zero);
            mCyl2.OnTgNoHitMark();

            field_0x294 = 0;
            field_0x298 = 0;

            s32 arg0 = param_get_arg0();
            if (arg0 == 0) {
                if (dComIfGs_isCollect(0, 1)) {
                    if (mBgAfter != NULL) {
                        dComIfG_Bgsp()->Regist(mBgAfter, this);
                        mBgAfter->SetCrrFunc(NULL);
                        mBgMode = 1;
                    }
                } else {
                    if (mBgBefore != NULL) {
                        dComIfG_Bgsp()->Regist(mBgBefore, this);
                        mBgBefore->SetCrrFunc(NULL);
                        mBgMode = 0;
                    }
                }
            }
        } else {
            ret = cPhs_ERROR_e;
        }
    }

    return ret;
}

/* 00001094-00001164       .text _delete__Q29daObjZouk5Act_cFv */
bool daObjZouk::Act_c::_delete() {
    if (heap != NULL) {
        if (mBgBefore != NULL && mBgBefore->ChkUsed())
            dComIfG_Bgsp()->Release(mBgBefore);
        if (mBgAfter != NULL && mBgAfter->ChkUsed())
            dComIfG_Bgsp()->Release(mBgAfter);
    }
    dComIfG_resDelete(&mPhs, M_arcname);
    return TRUE;
}

/* 00001164-0000123C       .text set_mtx__Q29daObjZouk5Act_cFv */
void daObjZouk::Act_c::set_mtx() {
    cXyz trans(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(trans);
    mDoMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
    M_anm->getModel()->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    M_anm->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), mMtx);
}

/* 0000123C-00001240       .text texture_scroll__Q29daObjZouk5Act_cFv */
void daObjZouk::Act_c::texture_scroll() {
}

/* 00001240-00001270       .text play_stop_joint_anime__Q29daObjZouk5Act_cFv */
void daObjZouk::Act_c::play_stop_joint_anime() {
    M_anm->play(NULL, NULL, NULL);
}

/* 00001270-000012F0       .text jokai_demo__Q29daObjZouk5Act_cFv */
BOOL daObjZouk::Act_c::jokai_demo() {
    if (dComIfGp_demo_getActor(demoActorID) != NULL) {
        dDemo_setDemoData(
            this,
            dDemo_actor_c::ENABLE_TRANS_e | dDemo_actor_c::ENABLE_ROTATE_e | dDemo_actor_c::ENABLE_ANM_e | dDemo_actor_c::ENABLE_ANM_FRAME_e,
            M_anm, "VzouK"
        );
        return true;
    } else {
        return false;
    }
}

/* 000012F0-00001510       .text _execute__Q29daObjZouk5Act_cFv */
bool daObjZouk::Act_c::_execute() {
    set_mtx();
    texture_scroll();
    fopAcM_rollPlayerCrash(this, 288.0f, 0x0D);
    if (!jokai_demo())
        play_stop_joint_anime();
    if (param_get_arg0() == 0) {
        if (dComIfGp_event_runCheck()) {
            if (mBgBefore != NULL && mBgAfter != NULL) {
                if (mBgAfter->ChkUsed())
                    dComIfG_Bgsp()->Release(mBgAfter);
                if (!mBgBefore->ChkUsed()) {
                    dComIfG_Bgsp()->Regist(mBgBefore, this);
                    mBgBefore->SetCrrFunc(NULL);
                    mBgMode = 0;
                }

                if (mBgBefore->ChkUsed())
                    mBgBefore->Move();
            }
        } else {
            if (mBgMode == 0 && dComIfGs_isCollect(0, 1) && mBgBefore != NULL && mBgAfter != NULL) {
                if (mBgBefore->ChkUsed())
                    dComIfG_Bgsp()->Release(mBgBefore);
                dComIfG_Bgsp()->Regist(mBgAfter, this);
                mBgAfter->SetCrrFunc(NULL);
                mBgMode = 1;
            }

            if (mBgMode == 0) {
                if (mBgBefore != NULL)
                    mBgBefore->Move();
            } else {
                if (mBgAfter != NULL)
                    mBgAfter->Move();
            }
        }
    }

    return true;
}

static void dummy() {
    const f32 floats[] = {
        1200.0f, 500.0f, 26.0f,
        530.0f, 418.0f, 1003.0f,
        305.0f, 235.0f, 1323.0f,
    };
}

/* 00001510-000015F0       .text _draw__Q29daObjZouk5Act_cFv */
bool daObjZouk::Act_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    dComIfGd_setListBG();
    g_env_light.setLightTevColorType(M_anm->getModel(), &tevStr);
    setEffectMtx(&eyePos, 0.5f);
    M_anm->updateDL();
    dComIfGd_setList();
    dComIfGd_setSimpleShadow2(&current.pos, mGndY, 388.0f, mGndChk, shape_angle.y, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return true;
}

/* 000015F0-00001804       .text setEffectMtx__Q29daObjZouk5Act_cFPC4cXyzf */
void daObjZouk::Act_c::setEffectMtx(const cXyz* pos, f32 scaleMag) {
    static Mtx mtx_adj = {
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, -0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.0f,
    };

    f32 inv = 1.0f / scaleMag;
    camera_class * camera = dCam_getCamera();
    cXyz lookDir = *pos - camera->mLookat.mEye;
    cXyz lightDir;
    cXyz refl;

    dKyr_get_vectle_calc(&tevStr.mLightPosWorld, (cXyz*)pos, &lightDir);
    C_VECHalfAngle(&lookDir, &lightDir, &refl);
    Mtx lookatMtx;
    C_MTXLookAt(lookatMtx, &cXyz::Zero, &cXyz::BaseY, &refl);
    mDoMtx_stack_c::scaleS(inv, inv, 1.0f);
    mDoMtx_stack_c::concat(mtx_adj);
    mDoMtx_stack_c::concat(lookatMtx);
    mDoMtx_stack_c::get()[0][3] = 0.0f;
    mDoMtx_stack_c::get()[1][3] = 0.0f;
    mDoMtx_stack_c::get()[2][3] = 0.0f;

    J3DModelData* modelData = M_anm->getModel()->getModelData();
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* mat = modelData->getMaterialNodePointer(i);
        for (u32 j = 0; j < 8; j++) {
            J3DTexMtx* texMtx = mat->getTexMtx(j);
            if (texMtx != NULL && texMtx->getTexMtxInfo().mInfo == 11) {
                texMtx->getTexMtxInfo().setEffectMtx(mDoMtx_stack_c::get());
            }
        }
    }
}

namespace daObjZouk {
    namespace {
        cPhs_State Mthd_Create(void* i_this) {
            return ((Act_c*)i_this)->_create();
        }

        BOOL Mthd_Delete(void* i_this) {
            return ((Act_c*)i_this)->_delete();
        }

        BOOL Mthd_Execute(void* i_this) {
            return ((Act_c*)i_this)->_execute();
        }

        BOOL Mthd_Draw(void* i_this) {
            return ((Act_c*)i_this)->_draw();
        }

        BOOL Mthd_IsDelete(void* i_this) {
            return TRUE;
        }

        static actor_method_class Mthd_Table = {
            (process_method_func)daObjZouk::Mthd_Create,
            (process_method_func)daObjZouk::Mthd_Delete,
            (process_method_func)daObjZouk::Mthd_Execute,
            (process_method_func)daObjZouk::Mthd_IsDelete,
            (process_method_func)daObjZouk::Mthd_Draw,
        };
    }
}

actor_process_profile_definition g_profile_Obj_Zouk = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Zouk,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjZouk::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Zouk,
    /* Actor SubMtd */ &daObjZouk::Mthd_Table,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
