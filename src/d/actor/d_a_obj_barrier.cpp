/**
 * d_a_obj_barrier.cpp
 * Object - Hyrule barrier
 */

#include "d/actor/d_a_obj_barrier.h"
#include "d/res/res_ycage.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_mtx.h"

namespace {
static const char l_arcname[] = "Ycage";

static const dCcD_SrcCyl l_cyl_at_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ AT_TYPE_UNK800,
        /* SrcObjAt  Atp     */ 1,
        /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
        /* SrcObjTg  Type    */ 0,
        /* SrcObjTg  SPrm    */ 0,
        /* SrcObjCo  SPrm    */ 0,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ 0,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK9,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ dCcG_AtSPrm_NoConHit_e | dCcG_AtSPrm_NoHitMark_e,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ 0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ 0,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCylS
    {
        /* Center */ 0.0f, 0.0f, 0.0f,
        /* Radius */ 1000.0f,
        /* Height */ 10000.0f,
    },
};

static const dCcD_SrcCyl l_cyl_tg_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ ~(AT_TYPE_WATER | AT_TYPE_UNK20000 | AT_TYPE_UNK400000 | AT_TYPE_LIGHT),
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ 0,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ 0,
        /* SrcGObjAt Spl     */ 0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ 0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e | dCcG_TgSPrm_NoHitMark_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCylS
    {
        /* Center */ 0.0f, 0.0f, 0.0f,
        /* Radius */ 1000.0f,
        /* Height */ 10000.0f,
    },
};

static cXyz l_ef_scale(1.0f, 1.0f, 1.0f);
}  // namespace

/* 000000EC-0000018C       .text init_mtx__14daObjBarrier_cFv */
void daObjBarrier_c::init_mtx() {
    mAnm.getMdlP()->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mAnm.getMdlP()->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::scaleM(scale);
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

/* 0000018C-000001B0       .text solidHeapCB__14daObjBarrier_cFP10fopAc_ac_c */
int daObjBarrier_c::solidHeapCB(fopAc_ac_c* i_this) {
    return static_cast<daObjBarrier_c*>(i_this)->create_heap();
}

/* 000001B0-00000340       .text init__18daObjBarrier_anm_cFv */
bool daObjBarrier_anm_c::init() {
    bool rt = true;
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcname, YCAGE_BDL_YCAGE00));
    J3DAnmTextureSRTKey* pbtk = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(l_arcname, YCAGE_BTK_YCAGE00));
    J3DAnmTevRegKey* pbrk = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(l_arcname, YCAGE_BRK_YCAGE00));

    if (modelData == NULL || pbtk == NULL || pbrk == NULL) {
        JUT_ASSERT(407, FALSE);
        rt = false;
    } else {
        mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x1000200);
        BOOL btk_init =
            mBtk.init(modelData, pbtk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0);
        BOOL brk_init =
            mBrk.init(modelData, pbrk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0);

        if (mpModel == NULL || !btk_init || !brk_init) {
            rt = false;
        }
    }

    return rt;
}

/* 00000340-000003E8       .text create_heap__14daObjBarrier_cFv */
bool daObjBarrier_c::create_heap() {
    bool rt = true;
    bool anm_init = mAnm.init();
    bool eff_init = mEffect.init();

    if (!anm_init || !eff_init) {
        rt = false;
    } else {
        cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes(l_arcname, YCAGE_DZB_KEKKAI);
        mpBgW = dBgW_NewSet(dzb, cBgW::MOVE_BG_e, &mBgMtx);
        if (mpBgW == NULL) {
            rt = false;
        }
    }

    return rt;
}

/* 000003E8-00000508       .text checkCollision_Tg__14daObjBarrier_cFv */
bool daObjBarrier_c::checkCollision_Tg() {
    if (mTgCyl.ChkTgHit()) {
        fopAc_ac_c* hit_actor = mTgCyl.GetTgHitAc();
        if (hit_actor != NULL) {
            cXyz hit_pos = hit_actor->current.pos;
            int no_set_effect = true;

            if (hit_actor == dComIfGp_getPlayer(0)) {
                no_set_effect = false;
                if (mTgCyl.GetTgHitObj() != NULL) {
                    hit_pos = *mTgCyl.GetTgHitPosP();
                }
            }

            mEffect.birth(hit_actor, scale.x * 1000.0f, mTgCyl.GetC(), hit_pos, no_set_effect);
        }

        mTgCyl.ClrTgHit();
    }

    return false;
}

/* 00000544-00000608       .text checkCollision_At__14daObjBarrier_cFv */
void daObjBarrier_c::checkCollision_At() {
    if (mAtCyl.ChkAtHit()) {
        fopAc_ac_c* hit_actor = mAtCyl.GetAtHitAc();
        fopAc_ac_c* player_p = dComIfGp_getPlayer(0);

        if (hit_actor != NULL && hit_actor == player_p) {
            mEffect.birth(hit_actor, scale.x * 1000.0f, mAtCyl.GetC(), hit_actor->current.pos, 1);
        }

        mAtCyl.ClrAtHit();
    }
}

/* 00000608-00000754       .text registCollisionTable__14daObjBarrier_cFv */
void daObjBarrier_c::registCollisionTable() {
    cXyz pos = current.pos;
    pos.y -= 300.0f;

    f32 base_radius = scale.x * 1000.0f;
    f32 height = scale.y * 10000.0f + 300.0f;

    mAtCyl.SetC(pos);
    mAtCyl.SetR(base_radius - 60.0f);
    mAtCyl.SetH(height);

    mTgCyl.SetC(pos);
    mTgCyl.SetR(base_radius - 20.0f);
    mTgCyl.SetH(height);

    cXyz at_vec = current.pos - dComIfGp_getPlayer(0)->current.pos;
    mAtCyl.SetAtVec(at_vec);

    dComIfG_Ccsp()->Set(&mAtCyl);
    dComIfG_Ccsp()->Set(&mTgCyl);
}

/* 00000754-00000884       .text brkAnmPlay__14daObjBarrier_cFv */
void daObjBarrier_c::brkAnmPlay() {
    f32 dist_to_playerXZ = (dComIfGp_getPlayer(0)->current.pos - current.pos).absXZ();
    f32 radius = scale.x * 1000.0f - 150.0f;

    f32 var_r3;
    if (dist_to_playerXZ > radius) {
        var_r3 = 0.0f;
    } else {
        var_r3 = radius - dist_to_playerXZ;
    }

    f32 brk_frame;
    if (var_r3 > 700.0f) {
        brk_frame = 1.0f;
    } else {
        brk_frame = (1.0f - var_r3 / 700.0f) * 59.0f + 1.0f;
    }

    mAnm.setBrkFrame(brk_frame);
}

/* 00000884-000009F0       .text break_start_wait_proc__14daObjBarrier_cFv */
void daObjBarrier_c::break_start_wait_proc() {
    // 0x3980: Saw Hyrule 3 Electric Barrier Demo
    if (dComIfGs_isEventBit(0x3980) == true) {
        daPy_py_c* player_p = (daPy_py_c*)daPy_getPlayerActorClass();

        if ((player_p->current.pos - current.pos).absXZ() >= 8800.0f &&
            dComIfGs_getSelectEquip(0) == dItem_MASTER_SWORD_3_e)
        {
            switch (player_p->getCutType()) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                dComIfGs_onEventBit(dSv_evtBit_c::BARRIER_BREAK);
                mEventID = dComIfGp_evmng_getEventIdx("seal");
                mBarrierProc = PROC_BREAK_ORDER;
                break;
            }
        }
    }
}

/* 000009F0-00000A58       .text break_order_proc__14daObjBarrier_cFv */
void daObjBarrier_c::break_order_proc() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        mBarrierProc = PROC_BREAK_END_WAIT;
    } else {
        fopAcM_orderOtherEventId(this, mEventID);
        eventInfo.onCondition(dEvtCnd_UNK2_e);
    }
}

/* 00000A58-00000AB8       .text break_end_wait_proc__14daObjBarrier_cFv */
void daObjBarrier_c::break_end_wait_proc() {
    if (dComIfGp_evmng_endCheck(mEventID)) {
        dComIfGp_event_reset();
        fopAcM_delete(this);
    }
}

/* 00000AB8-00000B34       .text break_check__14daObjBarrier_cFv */
bool daObjBarrier_c::break_check() {
    bool chk = false;

    if (mMoya == 0) {
        switch (mBarrierProc) {
        case PROC_BREAK_START_WAIT:
            break_start_wait_proc();
            break;
        case PROC_BREAK_ORDER:
            break_order_proc();
            chk = true;
            break;
        case PROC_BREAK_END_WAIT:
            break_end_wait_proc();
            chk = true;
            break;
        }
    }

    return chk;
}

/* 00000B34-00000D24       .text setDummyTexture__17daObjBarrier_ef_cFi */
void daObjBarrier_ef_c::setDummyTexture(int i_idx) {
    J3DModelData* modelData = mpModel[i_idx]->getModelData();
    J3DTexture* texture = modelData->getTexture();
    JUTNameTab* textureName = modelData->getTextureName();

    JUT_ASSERT(808, texture != NULL);
    JUT_ASSERT(809, textureName != NULL);

    for (u16 i = 0; i < texture->getNum(); i++) {
        if (strcmp(textureName->getName(i), "__dummy") == 0) {
            texture->setResTIMG(i, *mDoGph_gInf_c::getFrameBufferTimg());
        }
    }

    mDoExt_modelTexturePatch(modelData);
}

/* 00000D24-00000D5C       .text checkHitActor__17daObjBarrier_ef_cFP10fopAc_ac_c */
bool daObjBarrier_ef_c::checkHitActor(fopAc_ac_c* i_checkActor) {
    bool chk = false;

    for (int i = 0; i < 4; i++) {
        if (mHitActor[i] == i_checkActor) {
            chk = true;
            break;
        }
    }

    return chk;
}

/* 00000D5C-000011B8       .text birth__17daObjBarrier_ef_cFP10fopAc_ac_cf4cXyz4cXyzi */
void daObjBarrier_ef_c::birth(fopAc_ac_c* i_hitActor, f32 i_radius, cXyz i_center, cXyz i_hitPos,
                              int i_isNoEff) {
    if (!i_isNoEff || !checkHitActor(i_hitActor)) {
        int effect_idx = -1;
        csXyz angle(0, 0, 0);
        angle.y = cM_atan2s(i_center.x - i_hitPos.x, i_center.z - i_hitPos.z);

        cXyz pos;
        pos.x = i_center.x + i_radius * -cM_ssin(angle.y);
        pos.z = i_center.z + i_radius * -cM_scos(angle.y);
        pos.y = i_hitPos.y;

        int i;
        int active_flags = mActiveEffFlags;
        for (i = 0; i < 4; i++) {
            if (!((active_flags >> i) & 1)) {
                effect_idx = i;
                break;
            }
        }

        if (effect_idx == -1) {
            f32 var_f1 = -1.0f;
            for (i = 0; i < 4; i++) {
                if (mBtk[i].getFrame() > var_f1) {
                    var_f1 = mBtk[i].getFrame();
                    effect_idx = i;
                }
            }
        }

        active_flags = mActiveEffFlags;
        for (i = 0; i < 4; i++) {
            if ((int)((active_flags >> i) & 1) == true) {
                if (i != effect_idx) {
                    mBtk[i].setPlaySpeed((mBtk[i].getEndFrame() - mBtk[i].getFrame()) / 3.0f);
                }
                break;
            }
        }

        mActiveEffFlags |= 1 << effect_idx;
        mPos[effect_idx] = pos;
        mAngle[effect_idx] = angle.y;
        mHitActor[effect_idx] = i_hitActor;

        J3DModelData* modelData = mpModel[effect_idx]->getModelData();

        J3DAnmTextureSRTKey* btk_anm_p = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(l_arcname, YCAGE_BTK_YHRBR00));
        JUT_ASSERT(937, btk_anm_p != NULL);

        J3DAnmTransform* bck_anm_p = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(l_arcname, YCAGE_BCK_YHRBR00));
        JUT_ASSERT(942, bck_anm_p != NULL);

        J3DAnmTevRegKey* brk_anm_p = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(l_arcname, YCAGE_BRK_YHRBR00));
        JUT_ASSERT(947, brk_anm_p != NULL);

        mBtk[effect_idx].init(modelData, btk_anm_p, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1,
                              true, 0);
        mBck[effect_idx].init(modelData, bck_anm_p, TRUE, J3DFrameCtrl::EMode_NONE, 0.0f, 0, -1,
                              true);
        mBrk[effect_idx].init(modelData, brk_anm_p, TRUE, J3DFrameCtrl::EMode_NONE, 0.0f, 0, -1,
                              true, 0);

        mDoMtx_stack_c::transS(pos.x, pos.y, pos.z);
        mDoMtx_stack_c::ZXYrotM(0, angle.y, 0);
        mpModel[effect_idx]->setBaseTRMtx(mDoMtx_stack_c::get());

        dComIfGp_particle_set(dPa_name::ID_SCENE_81A9, &pos, &angle);
    }
}

/* 000011B8-000013E0       .text init__17daObjBarrier_ef_cFv */
bool daObjBarrier_ef_c::init() {
    bool rt = true;
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcname, YCAGE_BDL_YHRBR00));
    J3DAnmTextureSRTKey* pbtk = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(l_arcname, YCAGE_BTK_YHRBR00));
    J3DAnmTransform* pbck = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(l_arcname, YCAGE_BCK_YHRBR00));
    J3DAnmTevRegKey* pbrk = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(l_arcname, YCAGE_BRK_YHRBR00));

    if (modelData == NULL || pbtk == NULL || pbck == NULL || pbrk == NULL) {
        JUT_ASSERT(1016, FALSE);
        rt = false;
    } else {
        for (int i = 0; i < 4; i++) {
            mpModel[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x5020200);
            setDummyTexture(i);

            BOOL btk_init = mBtk[i].init(modelData, pbtk, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0,
                                         -1, false, 0);
            BOOL bck_init =
                mBck[i].init(modelData, pbck, TRUE, J3DFrameCtrl::EMode_NONE, 0.0f, 0, -1, false);
            BOOL brk_init = mBrk[i].init(modelData, pbrk, TRUE, J3DFrameCtrl::EMode_NONE, 0.0f, 0,
                                         -1, false, 0);

            if (mpModel[i] == NULL || !btk_init || !bck_init || !brk_init) {
                rt = false;
                break;
            }
        }
    }

    return rt;
}

/* 000013E0-00001420       .text create__17daObjBarrier_ef_cFv */
void daObjBarrier_ef_c::create() {
    for (int i = 0; i < 4; i++) {
        mpModel[i]->setBaseScale(l_ef_scale);
    }
}

/* 00001420-000014E0       .text execute__17daObjBarrier_ef_cFv */
void daObjBarrier_ef_c::execute() {
    int active_flags = mActiveEffFlags;
    for (int i = 0; i < 4; i++) {
        if (((active_flags >> i) & 1)) {
            mBtk[i].play();

            if (mBtk[i].isStop() == true) {
                active_flags ^= 1 << i;
                mHitActor[i] = NULL;
            }
        }
    }

    mActiveEffFlags = active_flags;
}

/* 000014E0-00001638       .text draw__17daObjBarrier_ef_cFv */
void daObjBarrier_ef_c::draw() {
    J3DModel* model_p;

    int active_flags = mActiveEffFlags;
    for (int i = 0; i < 4; i++) {
        if (((active_flags >> i) & 1)) {
            model_p = mpModel[i];

            J3DModelData* modelData = model_p->getModelData();
            mBtk[i].entry(modelData, getBtkFrame(i));
            mBck[i].entry(model_p->getModelData(), (s16)getBtkFrame(i));
            mBrk[i].entry(model_p->getModelData(), (s16)getBtkFrame(i));

            dComIfGd_setListInvisisble();
            mDoExt_modelUpdateDL(model_p);
            dComIfGd_setList();
        }
    }
}

/* 00001638-0000182C       .text _create__14daObjBarrier_cFv */
cPhs_State daObjBarrier_c::_create() {
    cPhs_State phase = cPhs_ERROR_e;
    fopAcM_SetupActor(this, daObjBarrier_c);

    if (fopAcM_IsFirstCreating(this)) {
        mMoya = param_get_moya();
        if (mMoya != 0) {
            mBarrierActive = true;
        } else if (dComIfGs_isEventBit(dSv_evtBit_c::BARRIER_BREAK) == true) {
            mBarrierActive = false;
        } else {
            mBarrierActive = true;
        }
    }

    if (mBarrierActive == true) {
        phase = dComIfG_resLoad(&mPhase, l_arcname);
    }

    if (phase == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0x30E0)) {
            if (dComIfG_Bgsp()->Regist(mpBgW, this)) {
                phase = cPhs_ERROR_e;
            } else {
                mpBgW->SetCrrFunc(NULL);
                scale.z = scale.x;
                fopAcM_SetMtx(this, mAnm.getMdlP()->getBaseTRMtx());
                init_mtx();

                mAtStts.Init(0xFF, 0xFF, this);
                mAtCyl.Set(l_cyl_at_src);
                mAtCyl.SetStts(&mAtStts);
                mAtCyl.OnBsRevHit();

                mTgStts.Init(0xFF, 0xFF, this);
                mTgCyl.Set(l_cyl_tg_src);
                mTgCyl.SetStts(&mTgStts);
                mTgCyl.OnBsRevHit();

                if (param_get_damage()) {
                    mAtCyl.SetAtAtp(0);
                }

                mEffect.create();
            }
        } else {
            phase = cPhs_ERROR_e;
        }
    }

    return phase;
}

/* 00001A38-00001AD8       .text _delete__14daObjBarrier_cFv */
bool daObjBarrier_c::_delete() {
    if (mBarrierActive == true) {
        dComIfG_resDelete(&mPhase, l_arcname);

        if (heap != NULL && mpBgW != NULL) {
            if (mpBgW->ChkUsed()) {
                dComIfG_Bgsp()->Release(mpBgW);
            }

            mpBgW = NULL;
        }
    }

    return true;
}

/* 00001AD8-00001B64       .text _execute__14daObjBarrier_cFv */
bool daObjBarrier_c::_execute() {
    mAnm.mBtk.play();
    brkAnmPlay();

    mpBgW->Move();
    mAtStts.Move();
    mTgStts.Move();

    checkCollision_At();
    if (!checkCollision_Tg()) {
        registCollisionTable();
    }

    if (!break_check()) {
        mEffect.execute();
    }

    return true;
}

/* 00001B64-00001C1C       .text _draw__14daObjBarrier_cFv */
bool daObjBarrier_c::_draw() {
    if (mBarrierProc) {
        return true;
    }

    if (mMoya == 0) {
        mAnm.getBtkAnmP()->entry(mAnm.getMdlP()->getModelData());
        mAnm.getBrkAnmP()->entry(mAnm.getMdlP()->getModelData(), (s16)mAnm.getBrkFrame());
        mDoExt_modelUpdateDL(mAnm.getMdlP());
    }

    mEffect.draw();
    return true;
}

/* 00001C1C-00001C3C       .text daObjBarrier_Create__FP10fopAc_ac_c */
static cPhs_State daObjBarrier_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjBarrier_c*>(i_this)->_create();
}

/* 00001C3C-00001C60       .text daObjBarrier_Delete__FP14daObjBarrier_c */
static BOOL daObjBarrier_Delete(daObjBarrier_c* i_this) {
    return static_cast<daObjBarrier_c*>(i_this)->_delete();
}

/* 00001C60-00001C84       .text daObjBarrier_Execute__FP14daObjBarrier_c */
static BOOL daObjBarrier_Execute(daObjBarrier_c* i_this) {
    return static_cast<daObjBarrier_c*>(i_this)->_execute();
}

/* 00001C84-00001CA8       .text daObjBarrier_Draw__FP14daObjBarrier_c */
static BOOL daObjBarrier_Draw(daObjBarrier_c* i_this) {
    return static_cast<daObjBarrier_c*>(i_this)->_draw();
}

/* 00001CA8-00001CB0       .text daObjBarrier_IsDelete__FP14daObjBarrier_c */
static BOOL daObjBarrier_IsDelete(daObjBarrier_c* i_this) {
    return TRUE;
}

static actor_method_class l_daObjBarrier_Method = {
    (process_method_func)daObjBarrier_Create,
    (process_method_func)daObjBarrier_Delete,
    (process_method_func)daObjBarrier_Execute,
    (process_method_func)daObjBarrier_IsDelete,
    (process_method_func)daObjBarrier_Draw,
};

actor_process_profile_definition g_profile_Obj_Barrier = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Barrier,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjBarrier_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Barrier,
    /* Actor SubMtd */ &l_daObjBarrier_Method,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
