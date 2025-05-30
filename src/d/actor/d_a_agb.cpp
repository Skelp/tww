/**
 * d_a_agb.cpp
 * Tingle Tuner Cursor
 */

#include "d/actor/d_a_agb.h"
#include "d/res/res_agb.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTGba.h"
#include "d/actor/d_a_player_main.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"
#include "d/d_meter.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_kankyo_wether.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_dvd_thread.h"
#include "m_Do/m_Do_gba_com.h"
#include "m_Do/m_Do_mtx.h"
#include "d/actor/d_a_npc_md.h"
#include "d/actor/d_a_npc_cb1.h"
#include "d/actor/d_a_bomb.h"
#include "stdio.h"

#include "weak_bss_3569.h" // IWYU pragma: keep
#include "weak_data_1811.h" // IWYU pragma: keep

static mDoDvdThd_toMainRam_c* l_gbaCommand;

int daAgb_c::mEffect;
daAgb_c::daAgb_ItemBuy daAgb_c::mItemBuy;
#if VERSION > VERSION_JPN
daAgb_c::daAgb_GbaFlg daAgb_c::mGbaFlg;
daAgb_c::daAgb_Switch daAgb_c::mSwitch;
daAgb_c::daAgb_Item daAgb_c::mItem;
daAgb_c::daAgb_Shop daAgb_c::mShop;
#endif

/* 800CF5EC-800CF6B8       .text __ct__11daAgb_HIO_cFv */
daAgb_HIO_c::daAgb_HIO_c() {
    field_0x04[0].mColor.r = 0;
    field_0x04[0].mColor.g = 255;
    field_0x04[0].mColor.b = 0;
    field_0x04[0].mColor.a = 255;

    field_0x04[1].mColor.r = 0;
    field_0x04[1].mColor.g = 30;
    field_0x04[1].mColor.b = 0;
    field_0x04[1].mColor.a = 0;

    field_0x14 = 25.0f;
    field_0x18 = 50.0f;
    field_0x1c = 781.25f;
    field_0x20 = 170.0f;
    field_0x24 = 0.0f;
    field_0x28 = 5.0f;
    field_0x2c = 10.0f;
    field_0x30 = 15.0f;
}

static daAgb_HIO_c l_HIO;

/* 800CF700-800CF71C       .text init__10dMsgCtrl_cFUs */
int dMsgCtrl_c::init(u16 param_0) {
    field_0x0 = param_0;
    mMsgID = fpcM_ERROR_PROCESS_ID_e;
    mpMsg = NULL;
    return 1;
}

/* 800CF71C-800CF7B4       .text execute__10dMsgCtrl_cFv */
int dMsgCtrl_c::execute() {
    if (mMsgID == fpcM_ERROR_PROCESS_ID_e) {
        mMsgID = fopMsgM_messageSet(field_0x0);
    } else if (mpMsg == NULL) {
        mpMsg = fopMsgM_SearchByID(mMsgID);
    } else {
        if (mpMsg->mStatus == fopMsgStts_BOX_CLOSED_e) {
            mpMsg->mStatus = fopMsgStts_MSG_DESTROYED_e;
            dComIfGp_event_reset();
        }

        return mpMsg->mStatus;
    }

    return -1;
}

static dMsgCtrl_c l_msgCtrl;

#if VERSION <= VERSION_JPN
u8 sjis2chrNo(const char* i_chr) {
    static u8 l_sjis1chrNo[] = {
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2D, 0x20, 0x3F,
        0x21, 0x2A, 0x2C, 0x2D, 0x5B, 0x5D, 0x30, 0x31,
        0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x52, 0x4C, 0x41, 0x42, 0x53, 0x54, 0x2A, 0x45,
        0x43, 0x50, 0x55, 0x48, 0x44, 0x46, 0x47, 0x49,
        0x4A, 0x4B, 0x4D, 0x4E, 0x4F, 0x51, 0x56, 0x57,
        0x58, 0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65,
        0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,
        0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
        0x76, 0x77, 0x78, 0x79, 0x7A, 0x2A, 0x28, 0x29,
        0x2A, 0x2E, 0x27, 0x22, 0x00,
    };

    static u8 l_sjis2chrNo[] = {
        0x82, 0xA0, 0x82, 0xA2, 0x82, 0xA4, 0x82, 0xA6,
        0x82, 0xA8, 0x82, 0xA9, 0x82, 0xAB, 0x82, 0xAD,
        0x82, 0xAF, 0x82, 0xB1, 0x82, 0xB3, 0x82, 0xB5,
        0x82, 0xB7, 0x82, 0xB9, 0x82, 0xBB, 0x82, 0xBD,
        0x82, 0xBF, 0x82, 0xC2, 0x82, 0xC4, 0x82, 0xC6,
        0x82, 0xC8, 0x82, 0xC9, 0x82, 0xCA, 0x82, 0xCB,
        0x82, 0xCC, 0x82, 0xCD, 0x82, 0xD0, 0x82, 0xD3,
        0x82, 0xD6, 0x82, 0xD9, 0x82, 0xDC, 0x82, 0xDD,
        0x82, 0xDE, 0x82, 0xDF, 0x82, 0xE0, 0x82, 0xE2,
        0x82, 0xE4, 0x82, 0xE6, 0x82, 0xE7, 0x82, 0xE8,
        0x82, 0xE9, 0x82, 0xEA, 0x82, 0xEB, 0x82, 0xED,
        0x82, 0xF0, 0x82, 0xF1, 0x82, 0x9F, 0x82, 0xA1,
        0x82, 0xA3, 0x82, 0xA5, 0x82, 0xA7, 0x82, 0xC1,
        0x82, 0xE1, 0x82, 0xE3, 0x82, 0xE5, 0x83, 0x41,
        0x83, 0x43, 0x83, 0x45, 0x83, 0x47, 0x83, 0x49,
        0x83, 0x4A, 0x83, 0x4C, 0x83, 0x4E, 0x83, 0x50,
        0x83, 0x52, 0x83, 0x54, 0x83, 0x56, 0x83, 0x58,
        0x83, 0x5A, 0x83, 0x5C, 0x83, 0x5E, 0x83, 0x60,
        0x83, 0x63, 0x83, 0x65, 0x83, 0x67, 0x83, 0x69,
        0x83, 0x6A, 0x83, 0x6B, 0x83, 0x6C, 0x83, 0x6D,
        0x83, 0x6E, 0x83, 0x71, 0x83, 0x74, 0x83, 0x7A,
        0x83, 0x7D, 0x83, 0x7E, 0x83, 0x80, 0x83, 0x81,
        0x83, 0x82, 0x83, 0x84, 0x83, 0x86, 0x83, 0x88,
        0x83, 0x89, 0x83, 0x8B, 0x83, 0x8C, 0x83, 0x8D,
        0x83, 0x8F, 0x83, 0x92, 0x83, 0x93, 0x83, 0x40,
        0x83, 0x42, 0x83, 0x44, 0x83, 0x46, 0x83, 0x48,
        0x83, 0x62, 0x83, 0x83, 0x83, 0x85, 0x83, 0x87,
        0x81, 0x45, 0x81, 0x5B, 0x81, 0x40, 0x81, 0x48,
        0x81, 0x49, 0x81, 0x42, 0x81, 0x41, 0x81, 0x60,
        0x81, 0x75, 0x81, 0x76, 0x82, 0x4F, 0x82, 0x50,
        0x82, 0x51, 0x82, 0x52, 0x82, 0x53, 0x82, 0x54,
        0x82, 0x55, 0x82, 0x56, 0x82, 0x57, 0x82, 0x58,
        0x82, 0x71, 0x82, 0x6B, 0x82, 0x60, 0x82, 0x61,
        0x82, 0x72, 0x82, 0x73, 0x81, 0x7B, 0x82, 0x64,
        0x82, 0x62, 0x82, 0x6F, 0x82, 0x74, 0x82, 0x67,
        0x82, 0x63, 0x82, 0x65, 0x82, 0x66, 0x82, 0x68,
        0x82, 0x69, 0x82, 0x6A, 0x82, 0x6C, 0x82, 0x6D,
        0x82, 0x6E, 0x82, 0x70, 0x82, 0x75, 0x82, 0x76,
        0x82, 0x77, 0x82, 0x78, 0x82, 0x79, 0x82, 0x81,
        0x82, 0x82, 0x82, 0x83, 0x82, 0x84, 0x82, 0x85,
        0x82, 0x86, 0x82, 0x87, 0x82, 0x88, 0x82, 0x89,
        0x82, 0x8A, 0x82, 0x8B, 0x82, 0x8C, 0x82, 0x8D,
        0x82, 0x8E, 0x82, 0x8F, 0x82, 0x90, 0x82, 0x91,
        0x82, 0x92, 0x82, 0x93, 0x82, 0x94, 0x82, 0x95,
        0x82, 0x96, 0x82, 0x97, 0x82, 0x98, 0x82, 0x99,
        0x82, 0x9A, 0x81, 0x63, 0x81, 0x69, 0x81, 0x6A,
        0x81, 0x7E, 0x81, 0x44, 0x81, 0x66, 0x81, 0x68,
        0x81, 0x9C, 0x82, 0xAA, 0x82, 0xAC, 0x82, 0xAE,
        0x82, 0xB0, 0x82, 0xB2, 0x82, 0xB4, 0x82, 0xB6,
        0x82, 0xB8, 0x82, 0xBA, 0x82, 0xBC, 0x82, 0xBE,
        0x82, 0xC0, 0x82, 0xC3, 0x82, 0xC5, 0x82, 0xC7,
        0x82, 0xCE, 0x82, 0xD1, 0x82, 0xD4, 0x82, 0xD7,
        0x82, 0xDA, 0x83, 0x4B, 0x83, 0x4D, 0x83, 0x4F,
        0x83, 0x51, 0x83, 0x53, 0x83, 0x55, 0x83, 0x57,
        0x83, 0x59, 0x83, 0x5B, 0x83, 0x5D, 0x83, 0x5F,
        0x83, 0x61, 0x83, 0x64, 0x83, 0x66, 0x83, 0x68,
        0x83, 0x6F, 0x83, 0x72, 0x83, 0x75, 0x83, 0x78,
        0x83, 0x7B, 0x83, 0x94, 0x82, 0xCF, 0x82, 0xD2,
        0x82, 0xD5, 0x82, 0xD8, 0x82, 0xDB, 0x83, 0x70,
        0x83, 0x73, 0x83, 0x76, 0x83, 0x79, 0x83, 0x7C,
        0x83, 0x5C, 0x83, 0x77, 0x83, 0x8A, 0x81, 0x7C,
        0x81, 0x45, 0x00,
    };

    char* chrNo;
    if (i_chr[0] & 0x80) {
        chrNo = (char*)l_sjis2chrNo;
        for (u8 i = 0; i < ARRAY_SIZE(l_sjis2chrNo)/2; chrNo += 2, i++) {
            if (i_chr[0] == chrNo[0] && i_chr[1] == chrNo[1] ||
                (i_chr[0] == 0x83 && i_chr[1] == 0x5C && chrNo[0] == 0x91 && chrNo[1] == 0x66) ||
                (i_chr[0] == 0x83 && i_chr[1] == 0x77 && chrNo[0] == 0x82 && chrNo[1] == 0xD6) ||
                (i_chr[0] == 0x83 && i_chr[1] == 0x8A && chrNo[0] == 0x82 && chrNo[1] == 0xE8) ||
                (i_chr[0] == 0x81 && i_chr[1] == 0x7C && chrNo[0] == 0x81 && chrNo[1] == 0x5B) ||
                (i_chr[0] == 0x81 && i_chr[1] == 0x45 && chrNo[0] == 0x81 && chrNo[1] == 0x42)
            ) {
                if (i == 0xF0)
                    return 0x45;
                if (i == 0xF1)
                    return 0x1C;
                if (i == 0xF2)
                    return 0x27;
                if (i == 0xF3)
                    return 0x6D;
                if (i == 0xF4)
                    return 0x6C;
                return i;
            }
        }
    } else {
        chrNo = (char*)l_sjis1chrNo;
        for (u8 i = 0; i < ARRAY_SIZE(l_sjis1chrNo)-1; chrNo++, i++) {
            if (i_chr[0] == chrNo[0]) {
                return i;
            }
        }
    }

    return 0xFF;
}
#else
/* 800CF7B4-800CF7F8       .text sjis2chrNo__FUc */
u8 sjis2chrNo(u8 i_chr) {
    static u8 l_sjis2chrNo[] = {
#if VERSION == VERSION_USA
        ' ',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',
        'O',  'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  'a',  'b',  'c',
        'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r',
        's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '0',  '1',  '2',  '3',  '4',  '5',  '6',
        '7',  '8',  '9',  ' ',  0xf8, 0xd8, 0xfe, 0xde, 0x9c, 0x8c, 0xe6, 0xc6, 0xf0, 0xd0, 0xc7,
        0xe7, 0xdf, ' ',  ' ',  '+',  0xbf, 0xa1, 0xab, 0xbb, 0x84, 0x93, '!',  ' ',  '-',  '?',
        ' ',  ' ',  '(',  ')',  ' ',  '.',  ' ',  0xd7, 0xf7, '\'', '\"', 0x85, '/',  0xc0, 0xc8,
        0xcc, 0xd2, 0xd9, 0xe0, 0xe8, 0xec, 0xf2, 0xf9, 0xc1, 0xc9, 0xcd, 0xd3, 0xda, 0xdd, 0xe1,
        0xe9, 0xed, 0xf3, 0xfa, 0xfd, 0xc2, 0xca, 0xce, 0xd4, 0xdb, 0xe2, 0xea, 0xee, 0xf4, 0xfb,
        0xc3, 0xd1, 0xd5, 0xe3, 0xf1, 0xf5, 0xc4, 0xcb, 0xcf, 0xd6, 0xdc, 0x9f, 0xe4, 0xeb, 0xef,
        0xf6, 0xfc, 0xff, 0xc5, 0xe5, 0x8a, 0x9a, 0xba, 0xaa,
#elif VERSION == VERSION_PAL
        ' ',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',
        'O',  'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  'a',  'b',  'c',
        'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r',
        's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '0',  '1',  '2',  '3',  '4',  '5',  '6',
        '7',  '8',  '9',  ' ',  0xf8, 0xd8, 0xfe, 0xde, 0x9c, 0x8c, 0xe6, 0xc6, 0xf0, 0xd0, 0xc7,
        0xe7, 0xdf, ' ',  ' ',  '+',  0xbf, 0xa1, 0xab, 0xbb, 0x84, 0x93, '!',  '~',  '-',  '?',
        '/',  ' ',  '(',  ')',  ' ',  '.',  ',',  0xd7, 0xf7, '\'', '\"', 0x85, ':',  '&',  0xc0,
        0xc8, 0xcc, 0xd2, 0xd9, 0xe0, 0xe8, 0xec, 0xf2, 0xf9, 0xc1, 0xc9, 0xcd, 0xd3, 0xda, 0xdd,
        0xe1, 0xe9, 0xed, 0xf3, 0xfa, 0xfd, 0xc2, 0xca, 0xce, 0xd4, 0xdb, 0xe2, 0xea, 0xee, 0xf4,
        0xfb, 0xc3, 0xd1, 0xd5, 0xe3, 0xf1, 0xf5, 0xc4, 0xcb, 0xcf, 0xd6, 0xdc, 0x9f, 0xe4, 0xeb,
        0xef, 0xf6, 0xfc, 0xff, 0xc5, 0xe5, 0x8a, 0x9a, 0xba, 0xaa,
#endif
    };

    for (u8 i = 0; i < ARRAY_SIZE(l_sjis2chrNo); i++) {
        if (i_chr == l_sjis2chrNo[i]) {
            return i;
        }
    }

    return 0xFF;
}
#endif

/* 800CF7F8-800CF894       .text NameConv__7daAgb_cFv */
void daAgb_c::NameConv() {
    int i;
    const char* name = dComIfGs_getPlayerName();
    mPlayerName = 0;

    for (i = 0; i < 8; i++) {
#if VERSION <= VERSION_JPN
        u8 chr = sjis2chrNo(name);
        mPlayerName |= chr;
        if (*name & 0x80)
            name += 2;
        else
            name++;
#else
        u8 chr = sjis2chrNo(*name);
        mPlayerName |= chr;
        name++;
#endif

        if (i < 7) {
            mPlayerName <<= 8;
        }
    }
}

/* 800CF894-800CFA30       .text uploadInitCheck__7daAgb_cFv */
int daAgb_c::uploadInitCheck() {
    int roomNo = dComIfGp_roomControl_getStayNo();
    dStage_FileList_dt_c* fili_p =
        dComIfGp_roomControl_getStatusRoomDt(roomNo)->getFileListInfo();

    if ((dStage_stagInfo_GetSTType(dComIfGp_getStageStagInfo()) != dStageType_SEA_e && !dMap_isEnableDispMap()) ||
        (fili_p != NULL && dStage_FileList_dt_CheckAgbCom(fili_p)))
    {
        l_msgCtrl.init(1);
        mUploadAction  = UpAct_UNKA;
    } else if (!mDoGaC_getComEnable()) {
        field_0x66f = false;
        mIsMsgSend = false;
        JUTGba::getManager()->doInitProbe(1, NULL, NULL);
        JUTGba::getManager()->doInitProbe(2, NULL, NULL);
        JUTGba::getManager()->doInitProbe(3, NULL, NULL);
        mUploadAction  = UpAct_UNK1;
    } else if (mDoGaC_GbaLink()) {
        l_msgCtrl.init(2);
        mUploadAction  = UpAct_UNKA;
    } else {
        JUTGba::getManager()->doInitProbe(1, NULL, NULL);
        JUTGba::getManager()->doInitProbe(2, NULL, NULL);
        JUTGba::getManager()->doInitProbe(3, NULL, NULL);
        mUploadAction  = UpAct_UNK1;
        mDoGaC_ConnectSleep();
    }

    return 1;
}

/* 800CFA30-800CFB68       .text uploadPortCheckWait__7daAgb_cFv */
int daAgb_c::uploadPortCheckWait() {
    u32 sp14[3];
    int sp8[3];

    for (int i = 0; i < 3; i++) {
        sp8[i] = JUTGba::getManager()->resultInitProbe(i + 1, &sp14[i]);
        if (sp8[i] == -2) {
            return 1;
        }
    }

    if (mDoGaC_GbaLink()) {
        l_msgCtrl.init(2);
        mUploadAction  = UpAct_UNKA;
    } else {
        for (int i = 0; i < 3; i++) {
            if (sp8[i] == 0 && sp14[i] == 0x40000) {
                mPortNo = i + 1;
                l_msgCtrl.init(5);
                mUploadAction  = UpAct_UNK2;
                return 1;
            }
        }

        l_msgCtrl.init(3);
        mUploadAction  = UpAct_UNKA;
        mDoGaC_ConnectWake();
    }

    return 1;
}

extern JKRHeap* dMsg_getAgbWorkArea();

/* 800CFB68-800CFC94       .text uploadSelect__7daAgb_cFv */
int daAgb_c::uploadSelect() {
    if (l_msgCtrl.execute() == fopMsgStts_MSG_DISPLAYED_e) {
        if (dComIfGp_checkMesgCancelButton() != 0 || l_msgCtrl.getSelectNum() != 0) {
            l_msgCtrl.setMsgStatus(fopMsgStts_MSG_ENDS_e);
            mDoGaC_ConnectWake();
        } else {
            l_msgCtrl.setMsgStatus(fopMsgStts_MSG_CONTINUES_e);
            fopMsgM_messageSet(6);  // "Now calling Tingle..."
            mUploadAction  = UpAct_UNK3;

#if VERSION <= VERSION_JPN
            l_gbaCommand = mDoDvdThd_toMainRam_c::create("/res/Gba/client.bin", 0, dMsg_getAgbWorkArea());
#elif VERSION == VERSION_USA
            l_gbaCommand = mDoDvdThd_toMainRam_c::create("/res/Gba/client_u.bin", 0, dMsg_getAgbWorkArea());
#elif VERSION == VERSION_PAL
            char path[28];
            char pathNum[4];
            strcpy(path, "/res/Gba/client_");
            sprintf(pathNum, "%d", dComIfGs_getPalLanguage());
            strcat(path, pathNum);
            strcat(path, ".bin");
            l_gbaCommand = mDoDvdThd_toMainRam_c::create(path, 0, dMsg_getAgbWorkArea());
#endif
            JUT_ASSERT(VERSION_SELECT(591, 591, 860, 861), l_gbaCommand != NULL);

            mDoGaC_GbaReboot();
            mDoGaC_setPortNo(mPortNo);
            field_0x66f = false;
            mIsMsgSend = false;
        }
    }

    return 1;
}

/* 800CFC94-800CFD60       .text uploadJoyboot1__7daAgb_cFv */
int daAgb_c::uploadJoyboot1() {
    if (l_gbaCommand->sync()) {
        void* programp = l_gbaCommand->getMemAddress();
        JUT_ASSERT(VERSION_SELECT(622, 622, 891, 892), programp != NULL);

        JUTGba::getManager()->doJoyBoot(mDoGaC_getPortNo(), 3, -1, (u8*)programp,
                                        l_gbaCommand->getMemSize() - 4, NULL, NULL);
        mUploadAction  = UpAct_UNK4;
        field_0x664 = 450;
    }

    return 1;
}

/* 800CFD60-800CFEA0       .text uploadJoyboot2__7daAgb_cFv */
int daAgb_c::uploadJoyboot2() {
    u8 sp8[16];

    int result = JUTGba::getManager()->resultJoyBoot(mDoGaC_getPortNo(), sp8);
    if (result != -2) {
        JKRHeap::free(l_gbaCommand->getMemAddress(), NULL);
        delete l_gbaCommand;

        if (result == 0) {
            field_0x664 = 15;
            mUploadAction  = UpAct_UNK6;
        } else {
            l_msgCtrl.setMsgStatus(fopMsgStts_MSG_CONTINUES_e);
            fopMsgM_messageSet(7);  // "An error has occurred."
            fopMsgM_messageSendOn();
            mUploadAction  = UpAct_UNKA;
        }
    } else {
        field_0x664--;
        if (field_0x664 == 0) {
            l_msgCtrl.setMsgStatus(fopMsgStts_MSG_CONTINUES_e);
            fopMsgM_messageSet(7);  // "An error has occurred."
            fopMsgM_messageSendOn();
            mUploadAction  = UpAct_UNKA;

            JKRHeap::free(l_gbaCommand->getMemAddress(), NULL);
            delete l_gbaCommand;
        }
    }

    return 1;
}

/* 800CFEA0-800CFF58       .text uploadMessageLoad__7daAgb_cFv */
int daAgb_c::uploadMessageLoad() {
    field_0x664--;
    if (field_0x664 == 0) {
#if VERSION != VERSION_PAL
        l_gbaCommand = mDoDvdThd_toMainRam_c::create("/res/Gba/msg_LZ.bin", 0, NULL);
#else
        char path[28];
        char pathNum[4];
        strcpy(path, "/res/Gba/msg_LZ");
        sprintf(pathNum, "%d", dComIfGs_getPalLanguage());
        strcat(path, pathNum);
        strcat(path, ".bin");
        l_gbaCommand = mDoDvdThd_toMainRam_c::create(path, 0, NULL);
#endif
        JUT_ASSERT(VERSION_SELECT(715, 715, 1000, 1001), l_gbaCommand != NULL);

        mUploadAction  = UpAct_UNK7;
        mDoGaC_onComEnable();
        mDoGaC_ComStart();
    }

    return 1;
}

/* 800CFF58-800CFF78       .text uploadMessageLoad2__7daAgb_cFv */
int daAgb_c::uploadMessageLoad2() {
    if (l_gbaCommand->sync()) {
        mUploadAction  = UpAct_UNK5;
    }

    return 1;
}

/* 800CFF78-800D00C8       .text uploadConnect__7daAgb_cFv */
int daAgb_c::uploadConnect() {
    if (mDoGaC_getComEnable() && mDoGaC_GbaLink()) {
        void* programp = l_gbaCommand->getMemAddress();
        JUT_ASSERT(VERSION_SELECT(760, 760, 1045, 1046), programp != NULL);
        mDoGac_SendDataSet((u32*)programp, l_gbaCommand->getMemSize(), 0, 0);

        mUploadAction  = UpAct_UNK8;
        NameConv();
        mDoGac_SendDataSet((u32*)&mPlayerName, sizeof(mPlayerName), 10, 0);

        dMap_mapAGBSendIslandData();
    } else {
        mDoGaC_GbaReboot();
        l_msgCtrl.setMsgStatus(fopMsgStts_MSG_CONTINUES_e);
        fopMsgM_messageSet(7);  // "An error has occurred."
        fopMsgM_messageSendOn();
        mUploadAction  = UpAct_UNKA;

        JKRHeap::free(l_gbaCommand->getMemAddress(), NULL);
        delete l_gbaCommand;
    }

    return 1;
}

/* 800D00C8-800D01F4       .text uploadMessageSend__7daAgb_cFv */
int daAgb_c::uploadMessageSend() {
    if (mDoGaC_getDataStatus(0) == 0) {
        mIsMsgSend = true;
        if (field_0x67a) {
            l_msgCtrl.setMsgStatus(fopMsgStts_MSG_CONTINUES_e);
            fopMsgM_messageSet(8);  // "Tingle appeared on your Game Boy Advance!"
            fopMsgM_messageSendOn();
            mUploadAction  = UpAct_UNKB;
            field_0x664 = 60;

            dComIfGs_onEventBit(0x1A20);
            dComIfGp_particle_set(dPa_name::ID_COMMON_02E7, &current.pos, NULL, NULL, 255, &field_0x684);

            JKRHeap::free(l_gbaCommand->getMemAddress(), NULL);
            delete l_gbaCommand;
        }
    } else if (mDoGaC_getDataStatus(0) == 9) {
#if VERSION <= VERSION_JPN
        mUploadAction = UpAct_UNK5;
#else
        field_0x664 = 5;
        mUploadAction  = UpAct_UNK9;
#endif
    }

    return 1;
}

#if VERSION > VERSION_JPN
/* 800D01F4-800D021C       .text uploadRetryWait__7daAgb_cFv */
int daAgb_c::uploadRetryWait() {
    field_0x664--;
    if (field_0x664 == 0) {
        mUploadAction  = UpAct_UNK5;
    }

    return 1;
}
#endif

/* 800D021C-800D0264       .text uploadMsgEndWait__7daAgb_cFv */
int daAgb_c::uploadMsgEndWait() {
    if (l_msgCtrl.execute() == fopMsgStts_MSG_DISPLAYED_e) {
        l_msgCtrl.setMsgStatus(fopMsgStts_MSG_ENDS_e);
    }

    return 1;
}

/* 800D0264-800D02E8       .text uploadMsgEndTimer__7daAgb_cFv */
int daAgb_c::uploadMsgEndTimer() {
    l_msgCtrl.execute();
    if (field_0x664 != 0) {
        field_0x664--;

        if (field_0x664 == 0) {
            l_msgCtrl.setMsgStatus(fopMsgStts_MSG_ENDS_e);
            fopMsgM_messageSendOn();
        } else if (field_0x664 == 30) {
            field_0x66f = true;
        }
    }

    return 1;
}

daAgb_c::uploadFunc daAgb_c::uploadFuncTable[] = {
    &daAgb_c::uploadInitCheck,
    &daAgb_c::uploadPortCheckWait,
    &daAgb_c::uploadSelect,
    &daAgb_c::uploadJoyboot1,
    &daAgb_c::uploadJoyboot2,
    &daAgb_c::uploadConnect,
    &daAgb_c::uploadMessageLoad,
    &daAgb_c::uploadMessageLoad2,
    &daAgb_c::uploadMessageSend,
#if VERSION > VERSION_JPN
    &daAgb_c::uploadRetryWait,
#endif
    &daAgb_c::uploadMsgEndWait,
    &daAgb_c::uploadMsgEndTimer,
};

/* 800D02E8-800D0364       .text modeLoad__7daAgb_cFv */
void daAgb_c::modeLoad() {
    if (eventInfo.checkCommandTalk()) {
        uploadFunc func = uploadFuncTable[mUploadAction];
        (this->*func)();
    } else {
        mUploadAction  = UpAct_UNK0;
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        mMode = MODE_MOVE;
    }
}

/* 800D0364-800D0490       .text modeLookAttention__7daAgb_cFv */
void daAgb_c::modeLookAttention() {
    static bool se_flag = false;

    if (eventInfo.checkCommandDemoAccrpt()) {
        if (!se_flag) {
            fopAcM_seStart(this, JA_SE_CV_CHI_MEGAHORN, 0);
            se_flag = true;
        }

        if (dComIfGp_evmng_endCheck("DEFAULT_AGB_LOOK_ATTENTION")) {
            dComIfGp_event_reset();
            mMode = MODE_MOVE;
            se_flag = false;

            field_0x65c = 1;
            if (!mHold) {
                shape_angle.setall(0);
                field_0x628 = 2.5f;
            }
        }
    } else {
        fopAcM_orderOtherEvent2(this, "DEFAULT_AGB_LOOK_ATTENTION", dEvtFlag_TALK_e);
    }
}

/* 800D0490-800D0580       .text DungeonNoGet__7daAgb_cFv */
u8 daAgb_c::DungeonNoGet() {
    u8 var_r0 = DungeonNoTable[field_0x66c];

    u8 dungeon_no = var_r0;
    if (dungeon_no == 0) {
        if (field_0x67e) {
            switch (field_0x66d) {
            case 1:
                dungeon_no = 10;
                break;
            case 11:
                dungeon_no = 6;
                break;
            case 13:
                dungeon_no = 7;
                break;
            case 41:
                dungeon_no = 8;
                break;
            case 44:
                dungeon_no = 9;
                break;
            case 4:
                dungeon_no = 14;
                break;
            case 20:
                dungeon_no = 15;
                break;
            case 23:
                dungeon_no = 12;
                break;
            case 40:
                dungeon_no = 16;
                break;
            case 45:
                dungeon_no = 13;
                break;
            }
        }

        if (field_0x66d != field_0x66e) {
            resetCursor(true);
        }
    }

    field_0x66e = field_0x66d;
    return dungeon_no;
}

u8 daAgb_c::DungeonNoTable[] = {
    0, 11, 11, 1, 2, 3, 4, 5, 17, 17, 17, 17, 17, 17, 17, 17,
};

/* 800D0580-800D05D4       .text MapNoSet__7daAgb_cFUcUcUc */
void daAgb_c::MapNoSet(u8 param_0, u8 param_1, u8 param_2) {
    field_0x66c = param_0;
    field_0x66d = param_1;
    field_0x67e = param_2;
    field_0x670 = true;

    mFlags.field_0xb_3 = DungeonNoGet();
}

/* 800D05D4-800D05F0       .text onFree__7daAgb_cFv */
void daAgb_c::onFree() {
    mIsFree = true;
    setTargetID(fpcM_ERROR_PROCESS_ID_e);
    setFollowTarget(false);
}

/* 800D05F0-800D0608       .text onHold__7daAgb_cFv */
void daAgb_c::onHold() {
    mHold = true;
    mBrk.setPlaySpeed(6.0f);
}

/* 800D0608-800D0620       .text offHold__7daAgb_cFv */
void daAgb_c::offHold() {
    mHold = false;
    mBrk.setPlaySpeed(1.0f);
}

/* 800D0620-800D070C       .text resetCursor__7daAgb_cFb */
void daAgb_c::resetCursor(bool param_0) {
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    mIsFree = false;
    setFollowTarget(false);
    setTargetID(fpcM_ERROR_PROCESS_ID_e);

    if (fopAcM_GetName(player_p) != PROC_NPC_KAM) {
        current.pos = player_p->current.pos;
        home.pos = player_p->current.pos;
    } else {
        current.pos = daPy_getPlayerLinkActorClass()->current.pos;
        home.pos = daPy_getPlayerLinkActorClass()->current.pos;
    }

    field_0x628 = 2.5f;
    shape_angle.setall(0);

    if (param_0) {
        if (field_0x66b == 14) {
            field_0x65c = 0;
        }

        field_0x675 = false;
        field_0x676 = false;
    }

    mHold = false;
    mBrk.setPlaySpeed(1.0f);
}

/* 800D070C-800D0734       .text FlashCheck__7daAgb_cFv */
bool daAgb_c::FlashCheck() {
    if (mIsFree || mFollowTarget == true) {
        return true;
    }

    return false;
}

/* 800D0734-800D0978       .text FlagsRecv__7daAgb_cFv */
void daAgb_c::FlagsRecv() {
    interface_of_controller_pad* pad_p = &g_mDoCPd_cpadInfo[mDoGaC_getPortNo()];
    u32 temp_r3 = BigLittleChange(mGbaFlg.field_0x0);

    u16 buttons = temp_r3 >> 0x10U;
    pad_p->mButtonHold.up    = buttons & 0x0040;
    pad_p->mButtonHold.down  = buttons & 0x0080;
    pad_p->mButtonHold.left  = buttons & 0x0020;
    pad_p->mButtonHold.right = buttons & 0x0010;
    pad_p->mButtonHold.a     = buttons & 0x0001;
    pad_p->mButtonHold.b     = buttons & 0x0002;
    pad_p->mButtonHold.r     = buttons & 0x0100;
    pad_p->mButtonHold.l     = buttons & 0x0200;
    pad_p->mButtonHold.start = buttons & 0x0008;

    // Buttons just pressed this frame
    u16 triggeredButtons = buttons & (buttons ^ mPrevButtons);
    pad_p->mButtonTrig.up    = triggeredButtons & 0x0040;
    pad_p->mButtonTrig.down  = triggeredButtons & 0x0080;
    pad_p->mButtonTrig.left  = triggeredButtons & 0x0020;
    pad_p->mButtonTrig.right = triggeredButtons & 0x0010;
    pad_p->mButtonTrig.a     = triggeredButtons & 0x0001;
    pad_p->mButtonTrig.b     = triggeredButtons & 0x0002;
    pad_p->mButtonTrig.r     = triggeredButtons & 0x0100;
    pad_p->mButtonTrig.l     = triggeredButtons & 0x0200;
    pad_p->mButtonTrig.start = triggeredButtons & 0x0008;

    g_mDoCPd_cpadInfo[mDoGaC_getPortNo()].mGamepadErrorFlags = 0;

    mPrevButtons = buttons;
    field_0x673 = mGbaFlg.field_0x3;
    field_0x630 = BigLittleChange(mGbaFlg.field_0x4) >> 0x10;
    field_0x632 = BigLittleChange(mGbaFlg.field_0x6) >> 0x10;
    field_0x67a = mGbaFlg.field_0x2.m2;

    if (!dComIfGs_isEventBit(0x1708) && mGbaFlg.field_0x2.m3) {
        dComIfGs_onEventBit(0x1708);
    }

    if (!dComIfGs_isEventBit(0x1A10) && mGbaFlg.field_0x2.m4) {
        dComIfGs_onEventBit(0x1A10);
    }

    if (!dComIfGs_isEventBit(0x1A08) && mGbaFlg.field_0x2.m5) {
        dComIfGs_onEventBit(0x1A08);
    }
}

/* 800D0978-800D0A54       .text SwitchOn__7daAgb_cFv */
void daAgb_c::SwitchOn() {
    if (mSwitch.stageNo == (u8)dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        u8 var_r31 = mSwitch.field_0x3 - 1;

        if (mSwitch.field_0x0 != 0xFF) {
            dComIfGs_revSwitch(mSwitch.field_0x0, mSwitch.roomNo);
        }

        if (var_r31 != 9) {
            u8 sw = mSwitch.field_0x4;
            if (sw != 0xFF) {
                if (var_r31 == 0 || var_r31 == 1 || var_r31 == 8) {
                    dComIfGs_onSwitch(sw, mSwitch.roomNo);
                } else {
                    dComIfGs_revSwitch(sw, mSwitch.roomNo);
                }
            }
        }
    }

    mDoGaC_DataStatusReset(8);
}

/* 800D0A54-800D1188       .text GbaItemUse__7daAgb_cFv */
void daAgb_c::GbaItemUse() {
    u32 temp_r29 = mItem.field_0x0;
    u32 var_r28 = 0;

    u32 temp_r0 = temp_r29 - 3;
    if (temp_r0 == 0 || temp_r0 == 1 || temp_r0 == 2 || temp_r29 == 0x11 || temp_r29 == 0x12) {
        int roomNo = dComIfGp_roomControl_getStayNo();
        dStage_FileList_dt_c* fili_p = dComIfGp_roomControl_getStatusRoomDt(roomNo)->getFileListInfo();

        if (fili_p != NULL && dStage_FileList_dt_CheckAgbHover(fili_p)) {
            if (mBombDeny) {
                mEffect = BigLittleChange((mDenyMessage << 0x10) | 0x300);
                return;
            } else if (temp_r29 == 5) {
                f32 temp_f1 = cM_rndF(100.0f);

                if (temp_f1 < 10.0f) {
                    var_r28 = 0x13;
                } else if (temp_f1 < 40.0) {
                    var_r28 = 0xA;
                } else if (temp_f1 < 70.0) {
                    var_r28 = 0xD;
                } else if (dComIfGs_isEventBit(0x2E08)) {
                    var_r28 = 0xC;
                } else {
                    var_r28 = 0xD;
                }

                if ((var_r28 == 0xC || var_r28 == 0xD) &&
                    (daPy_getPlayerLinkActorClass()->checkEquipDragonShield()))
                {
                    var_r28 = 0xA;
                }
            }
        } else {
            mEffect = 0x20000;
            return;
        }
    }

    if (dComIfGs_getRupee() < mItem.field_0x1) {
        mEffect = 0x10000;
        return;
    }

    switch (temp_r29) {
    case 16:
        if (daPy_getPlayerLinkActorClass()->checkNoControll() ||
            dComIfGp_checkPlayerStatus0(0, daPyStts0_CRAWL_e) ||
            (
                daPy_getPlayerActorClass()->checkPlayerFly() &&
                !dComIfGp_checkPlayerStatus0(0, daPyStts0_SWIM_e) &&
                !dComIfGp_checkPlayerStatus0(0, daPyStts0_SHIP_RIDE_e)
            )
        ) {
            mEffect = BigLittleChange(0x1F0300);
            return;
        } else if (field_0x67d) {
            mEffect = BigLittleChange((field_0x662 << 0x10) | 0x300);
            return;
        }
        field_0x67f = true;
        break;
    case 2:
        if (field_0x67b) {
            mEffect = BigLittleChange((field_0x65e << 0x10) | 0x300);
            return;
        }

        if (daPy_getPlayerLinkActorClass()->checkNoControll() &&
            fopAcM_searchActorDistanceXZ(this, daPy_getPlayerLinkActorClass()) < 232.0f)
        {
            mEffect = 0;
            return;
        }

        if (isFree() || getFollowTarget() == 1) {
            if (!isFree() && dComIfGs_getGbaRupeeCount() < 24 &&
                fopAcM_IsExecuting(getTargetID()))
            {
                fopAc_ac_c* actor_p = fopAcM_SearchByID(getTargetID());

                if (cM_rndF(5.0f) < 4.0) {
                    field_0x640 = dItem_YELLOW_RUPEE_e;
                } else {
                    field_0x640 = dItem_RED_RUPEE_e;
                }

                field_0x634 = actor_p->current.pos;
                dComIfGs_incGbaRupeeCount();
                temp_r29 = 15;
            }

            fopAcM_create(PROC_BOMB, daBomb_c::prm_make(daBomb_c::STATE_8, false, false), &current.pos);
            field_0x65c = 120;
        } else {
            temp_r29 = 0xe;
            onFree();
            field_0x65c = 270;
            mBrk.setPlaySpeed(6.0f);
        }
        break;
    case 0x15:
        resetCursor(false);
        fopAcM_create(PROC_BOMB, daBomb_c::prm_make(daBomb_c::STATE_8, false, false), &current.pos);
        field_0x65c = 120;
        break;
    case 0x11:
        mEffect = BigLittleChange(temp_r29);
        return;
    case 3:
        daPy_getPlayerLinkActorClass()->setHoverBoots(5*30);
        resetCursor(false);
        field_0x65c = 150;
        mBrk.setPlaySpeed(6.0f);
        break;
    case 0x12:
        if (daPy_getPlayerLinkActorClass()->checkEquipDragonShield()) {
            mEffect = 0;
            return;
        } else {
            mEffect = BigLittleChange(temp_r29);
            return;
        }
        break;
    case 4:
        if (daPy_getPlayerLinkActorClass()->checkEquipDragonShield()) {
            mEffect = 0;
            return;
        } else {
            daPy_getPlayerLinkActorClass()->setTinkleShield(300);
            resetCursor(false);
            field_0x65c = 300;
            mBrk.setPlaySpeed(6.0f);
        }
        break;
    case 12:
        if (mBombDeny) {
            mEffect = BigLittleChange((mDenyMessage << 0x10) | 0x300);
            return;
        } else {
            daPy_getPlayerLinkActorClass()->setHoverBoots(10*30);
            if (!daPy_getPlayerLinkActorClass()->checkEquipDragonShield()) {
                daPy_getPlayerLinkActorClass()->setTinkleShield(300);
            }

            resetCursor(false);
            field_0x65c = 300;
            mBrk.setPlaySpeed(6.0f);
        }
        break;
    case 13:
        if (mBombDeny) {
            mEffect = BigLittleChange((mDenyMessage << 0x10) | 0x300);
            return;
        } else {
            if (daPy_getPlayerLinkActorClass()->checkEquipDragonShield()) {
                mEffect = 0;
                return;
            }

            daPy_getPlayerLinkActorClass()->setTinkleShield(600);

            resetCursor(false);
            field_0x65c = 600;
            mBrk.setPlaySpeed(6.0f);
        }
        break;
    case 5:
        temp_r29 |= (var_r28 << 8);
        resetCursor(false);
        break;
    case 0x13:
        resetCursor(false);
        field_0x65c = 30;
        break;
    case 7:
        if (dComIfGs_checkGetItem(dItem_BAIT_BAG_e)) {
            if (dComIfGs_checkBaitItemEmpty()) {
                temp_r29 |= 0x1000000;
            }

            if (dComIfGs_checkGetItem(dItem_BOMB_BAG_e) && dComIfGs_getBombNum() < dComIfGs_getBombMax()) {
                temp_r29 |= 0x10000;
            }

            if (dComIfGs_getItem(dInvSlot_BOW_e) != dItem_NONE_e && dComIfGs_getArrowNum() < dComIfGs_getArrowMax()) {
                temp_r29 |= 0x100;
            }
        }
        break;
    case 0x14:
        resetCursor(false);
        break;
    case 8:
        dComIfGp_setItemLifeCount(12.0f);
        resetCursor(false);
        field_0x65c = 60;
        break;
    case 9:
        dComIfGp_setItemMagicCount(8);
        resetCursor(false);
        field_0x65c = 60;
        break;
    case 10:
        dComIfGp_setItemLifeCount(dComIfGs_getMaxLife());
        dComIfGp_setItemMagicCount(dComIfGs_getMaxMagic());
        resetCursor(false);
        field_0x65c = 60;
        break;
    default:
        mEffect = 0;
        return;
    }

    offActive();
    dComIfGp_setItemRupeeCount(-mItem.field_0x1);
    mEffect = BigLittleChange(temp_r29);
    field_0x66b = temp_r29;
}

/* 800D1188-800D12E4       .text Shopping__7daAgb_cFv */
void daAgb_c::Shopping() {
    daAgb_ItemBuy& itemBuy = mItemBuy;
    mItemBuy.U8.field_0x0 = mShop.field_0x0;
    
    // The usage of single | instead of double || here looks like a bug
    if (!(dComIfGp_event_runCheck() | dMenu_flag())) {
        if (dComIfGs_getRupee() < mShop.field_0x2) {
            itemBuy.U8.field_0x1 = 3;
            return;
        } else if (mShop.field_0x0 == 0) {
            if (dComIfGs_getArrowNum() < dComIfGs_getArrowMax()) {
                dComIfGp_setItemArrowNumCount(mShop.field_0x1);
            } else {
                itemBuy.U8.field_0x1 = 2;
                return;
            }
        } else if (mShop.field_0x0 == 1) {
            if (dComIfGs_getBombNum() < dComIfGs_getBombMax()) {
                dComIfGp_setItemBombNumCount(mShop.field_0x1);
            } else {
                itemBuy.U8.field_0x1 = 2;
                return;
            }
        } else {
            if (dComIfGs_checkBaitItemEmpty()) {
                dComIfGs_setBaitItem(dItem_BIRD_BAIT_5_e);
            } else {
                itemBuy.U8.field_0x1 = 2;
                return;
            }
        }
        dComIfGp_setItemRupeeCount(-mShop.field_0x2);
        itemBuy.U8.field_0x1 = 1;
    } else {
        itemBuy.U8.field_0x1 = 0;
    }
}

/* 800D12E4-800D1A3C       .text FlagsSend__7daAgb_cFUl */
void daAgb_c::FlagsSend(u32 stage_type) {
    if (field_0x670 &&
        (stage_type == dStageType_DUNGEON_e ||
        stage_type == dStageType_FF1_e ||
        stage_type == dStageType_MINIBOSS_e ||
        stage_type == dStageType_SEA_e))
    {
        mFlags.field_0xa_7 = 0;
    } else {
        mFlags.field_0xa_7 = 1;
    }
    mFlags.field_0xa_6 = dMenu_flag();
    mFlags.field_0xa_5 = dComIfGp_event_getMode() != dEvtMode_NONE_e;
    if (dStage_checkRestart() || dComIfGp_checkPlayerStatus0(0, daPyStts0_UNK20000000_e)) {
        mFlags.field_0xa_4 = 1;
    } else {
        mFlags.field_0xa_4 = 0;
    }
    mFlags.field_0x0 = BigLittleChange(dComIfGs_getRupee()) >> 16;
    mFlags.field_0x5_6 = dComIfGs_getWalletSize();
    if (dKyw_gbwind_use_check()) {
        mFlags.field_0xb_6 = 1;
        s16 r3;
        if (dComIfGp_event_runCheck() &&
            (dComIfGp_evmng_startCheck("TACT_WINDOW2_SHIP") || dComIfGp_evmng_startCheck("TACT_WINDOW2"))
        ) {
            r3 = field_0x658;
        } else {
            cXyz* wind_vec = dKyw_get_wind_vec();
            r3 = cM_atan2s(wind_vec->x, wind_vec->z);
        }
        mFlags.field_0x2 = (r3 & 0xFF00) >> 8;
        field_0x658 = r3;
    } else {
        mFlags.field_0xb_6 = 0;
    }
    mFlags.field_0x3_4 = dComIfGs_isStageTbox(dSv_save_c::STAGE_DRC, 0xF) != FALSE;
    mFlags.field_0x3_3 = dComIfGs_isStageTbox(dSv_save_c::STAGE_FW, 0xF) != FALSE;
    mFlags.field_0x3_2 = dComIfGs_isStageTbox(dSv_save_c::STAGE_TOTG, 0xF) != FALSE;
    mFlags.field_0x3_1 = dComIfGs_isStageTbox(dSv_save_c::STAGE_ET, 0xF) != FALSE;
    mFlags.field_0x3_0 = dComIfGs_isStageTbox(dSv_save_c::STAGE_WT, 0xF) != FALSE;
    mFlags.field_0x4 = (field_0x65c + 29) / 30;
    mFlags.field_0x6_0 = dKy_get_dayofweek();
    mFlags.field_0x6_3 = dKy_getdaytime_hour();
    mFlags.field_0x7_2 = dKy_getdaytime_minute();
    mFlags.field_0x8_1 = dComIfGs_getLife();
    mFlags.field_0xa_1 = dComIfGs_isEventBit(0x1708);
    mFlags.field_0x7_1 = dComIfGs_isEventBit(dSv_evtBit_c::UNLOCK_TINGLE_BALLOON_DISCOUNT);
    mFlags.field_0x7_0 = dComIfGs_isEventBit(dSv_evtBit_c::UNLOCK_TING_DISCOUNT);
    
    if (!dComIfGs_isEventBit(dSv_evtBit_c::MET_KORL) || dComIfGs_isEventBit(0x1E80)) {
        mFlags.field_0x9_7 = 0;
        mFlags.field_0x9_6 = 0;
        mFlags.field_0x9_5 = 0;
    } else if (dComIfGs_isSymbol(2)) {
        mFlags.field_0x9_7 = 0;
        mFlags.field_0x9_6 = 0;
        mFlags.field_0x9_5 = 1;
    } else if (dComIfGs_isSymbol(1)) {
        mFlags.field_0x9_7 = 0;
        mFlags.field_0x9_6 = 1;
        mFlags.field_0x9_5 = 0;
    } else {
        mFlags.field_0x9_7 = 1;
        mFlags.field_0x9_6 = 0;
        mFlags.field_0x9_5 = 0;
    }
    
    if (dComIfGs_isEventBit(0x3920)) {
        mFlags.field_0x9_4 = !dComIfGs_isEventBit(dSv_evtBit_c::PLACED_DINS_PEARL);
        mFlags.field_0x9_3 = !dComIfGs_isEventBit(dSv_evtBit_c::PLACED_FARORES_PEARL);
        mFlags.field_0x9_2 = !dComIfGs_isEventBit(dSv_evtBit_c::PLACED_NAYRUS_PEARL);
    } else {
        mFlags.field_0x9_4 = 0;
        mFlags.field_0x9_3 = 0;
        mFlags.field_0x9_2 = 0;
    }
    
    if (dComIfGs_isEventBit(0x1820)) {
        mFlags.field_0x9_1 = !dComIfGs_isStageBossEnemy(dSv_save_c::STAGE_WT);
        mFlags.field_0x9_0 = !dComIfGs_isStageBossEnemy(dSv_save_c::STAGE_ET);
    } else {
        mFlags.field_0x9_1 = 0;
        mFlags.field_0x9_0 = 0;
    }
    mFlags.field_0xa_3 = dComIfGs_isEventBit(0x1E40);
    mFlags.field_0xa_0 = mIsFree;
    mFlags.field_0xb_7 = getFollowTarget();
    if (stage_type == dStageType_MINIBOSS_e) {
        mFlags.field_0x8_0 = 1;
    } else {
        mFlags.field_0x8_0 = 0;
    }
    mFlags.field_0x3_7 = dComIfGs_isTact(0);
    
    if (*(u16*)&g_mDoCPd_cpadInfo[0].mButtonHold || // fakematch? is controller_pad_buttons supposed to be a u16?
        g_mDoCPd_cpadInfo[0].mMainStickValue ||
        g_mDoCPd_cpadInfo[0].mCStickValue)
    {
        mFlags.field_0x3_6 = 1;
    } else {
        mFlags.field_0x3_6 = 0;
    }
    if (dComIfGs_getMaxMagic() != 0) {
        mFlags.field_0xb_0 = 1;
    } else {
        mFlags.field_0xb_0 = 0;
    }
    mFlags.field_0x5_2 = field_0x675;
    mFlags.field_0x5_1 = dComIfGs_checkGetItem(COTTAGE_PAPER) != FALSE;
    mDoGac_SendDataSet((u32*)&mFlags, 0xC, 9, 0);
}

/* 800D1A3C-800D25D8       .text CursorMove__7daAgb_cFP10fopAc_ac_cUl */
void daAgb_c::CursorMove(fopAc_ac_c* actor, u32 stage_type) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    
    f32 f31;
    if (stage_type == dStageType_SEA_e) {
        f31 = field_0x67e ? 50.0f : 781.25f;
    } else {
        f31 = 25.0f;
    }
    
    if (cLib_chaseF(&field_0x628, 2.5f, field_0x62c) &&
        field_0x673 && isActive() && !mHold &&
        !CPad_CHECK_HOLD_L(mDoGaC_getPortNo())
    ) {
        if (CPad_CHECK_HOLD_LEFT(mDoGaC_getPortNo())) {
            actor->home.pos.x -= f31;
        } else if (CPad_CHECK_HOLD_RIGHT(mDoGaC_getPortNo())) {
            actor->home.pos.x += f31;
        }
        if (CPad_CHECK_HOLD_UP(mDoGaC_getPortNo())) {
            actor->home.pos.z -= f31;
        } else if (CPad_CHECK_HOLD_DOWN(mDoGaC_getPortNo())) {
            actor->home.pos.z += f31;
        }
        
        if (stage_type == dStageType_SEA_e && !field_0x67e) {
            if (actor->home.pos.x < -350000.0f) {
                actor->home.pos.x = -350000.0f;
            } else if (actor->home.pos.x > 350000.0f) {
                actor->home.pos.x = 350000.0f;
            }
            if (actor->home.pos.z < -350000.0f) {
                actor->home.pos.z = -350000.0f;
            } else if (actor->home.pos.z > 350000.0f) {
                actor->home.pos.z = 350000.0f;
            }
        } else {
            int roomNo = dComIfGp_roomControl_getStayNo();
            stage_map_info_class* mapInfo = dComIfGp_roomControl_getStatusRoomDt(roomNo)->getMapInfo();
            if (mapInfo) {
                if (actor->home.pos.x < mapInfo->field_0x18) {
                    actor->home.pos.x = mapInfo->field_0x18;
                } else if (actor->home.pos.x > mapInfo->field_0x20) {
                    actor->home.pos.x = mapInfo->field_0x20;
                }
                if (actor->home.pos.z < mapInfo->field_0x1C) {
                    actor->home.pos.z = mapInfo->field_0x1C;
                } else if (actor->home.pos.z > mapInfo->field_0x24) {
                    actor->home.pos.z = mapInfo->field_0x24;
                }
            }
        }
        
        cLib_chaseF(&actor->current.pos.x, actor->home.pos.x, f31);
        cLib_chaseF(&actor->current.pos.z, actor->home.pos.z, f31);
        
        f32 playerDist = fopAcM_searchPlayerDistanceXZ(actor);
        if (playerDist > 212100.0f) {
            if (mIsFree || !mFollowTarget) {
                cXyz r1_50 = player->current.pos - actor->current.pos;
                actor->home.pos.x = player->current.pos.x - r1_50.x * 212100.0f / playerDist;
                actor->home.pos.z = player->current.pos.z - r1_50.z * 212100.0f / playerDist;
            } else {
                mIsFree = false;
                setFollowTarget(false);
                setTargetID(fpcM_ERROR_PROCESS_ID_e);
            }
        }
    }
    
    cXyz r1_44 = actor->old.pos;
    cXyz r1_38 = actor->old.pos;
    r1_38.y += 171.0f;
    dBgS_LinkLinChk r1_11C;
    r1_11C.Set(&r1_44, &r1_38, actor);
    r1_11C.ClrSttsGroundOff();
    r1_11C.ClrSttsWallOff();
    f32 f31_2 = 171.0f;
    if (dComIfG_Bgsp()->LineCross(&r1_11C)) {
        f31_2 = r1_11C.GetCross().y - 55.0f - actor->current.pos.y;
        if (f31_2 < 20.0f) {
            f31_2 = 20.0f;
        }
    }
    mCrrPos.SetWall(f31_2, 50.0f);
    mAcchCir.SetWall(f31_2, 40.0f);
    mCrrPos.CrrPos(*dComIfG_Bgsp());
    mAcch.CrrPos(*dComIfG_Bgsp());
    
    {
        cXyz r1_2C = actor->old.pos;
        r1_2C.y += f31_2;
        cXyz r1_20 = actor->current.pos;
        r1_20.y += f31_2;
        dBgS_LinkLinChk r1_B0;
        r1_B0.Set(&r1_2C, &r1_20, actor);
        if (dComIfG_Bgsp()->LineCross(&r1_B0)) {
            actor->current.pos = actor->old.pos;
        }
    }
    
    int roomNo = dComIfG_Bgsp()->GetRoomId(mCrrPos.mGndChk);
    actor->current.roomNo = actor->tevStr.mRoomNo = roomNo;
    if (roomNo != player->current.roomNo) {
        return;
    }
    
    field_0x679 = true;
    
    if (mCrrPos.ChkXCrr()) {
        actor->home.pos.x = actor->current.pos.x;
    }
    if (mCrrPos.ChkZCrr()) {
        actor->home.pos.z = actor->current.pos.z;
    }
    
    cXyz r1_14;
    dBgS_GndChk r1_5C;
    f32 f30 = -G_CM3D_F_INF;
    r1_5C.OffWall();
    r1_14.y = actor->current.pos.y + 150.0f;
    static cXy l_ckOffset[] = {
        {-49.0f, -49.0f},
        {49.0f, -49.0f},
        {-49.0f, 49.0f},
        {49.0f, 49.0f},
    };
    for (int i = 0; i < 4; i++) {
        r1_14.x = actor->current.pos.x + l_ckOffset[i].x;
        r1_14.z = actor->current.pos.z + l_ckOffset[i].y;
        r1_5C.SetPos(&r1_14);
        f32 groundY = dComIfG_Bgsp()->GroundCross(&r1_5C);
        if (groundY > f30) {
            f30 = groundY;
        }
    }
    
    cLib_chaseF(&actor->current.pos.y, actor->home.pos.y, 25.0f);
    if (f30 > actor->current.pos.y && f30 < actor->current.pos.y + 170.0f + 1.0f) {
        actor->home.pos.y = f30;
    }
}

/* 800D272C-800D303C       .text modeMove__7daAgb_cFv */
void daAgb_c::modeMove() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    
    // single | instead of double ||: bug?
    BOOL r26 = dComIfGp_event_runCheck() | dMenu_flag();
    
    stage_stag_info_class* stag_info = dComIfGp_getStageStagInfo();
    u16 stage_type = dStage_stagInfo_GetSTType(stag_info);
    
    if (eventInfo.checkCommandTalk()) {
        mUploadAction  = UpAct_UNK0;
        mMode = MODE_LOAD;
        return;
    }
    
    if (field_0x65c != 0) {
        if (field_0x66b == 3 || field_0x66b == 12) {
            field_0x65c = daPy_getPlayerLinkActorClass()->checkTinkleHover();
            if (field_0x65c == 0) {
                mBrk.setPlaySpeed(1.0f);
            }
        } else if (field_0x66b == 4 || field_0x66b == 13) {
            field_0x65c = daPy_getPlayerLinkActorClass()->checkTinkleShield();
            if (field_0x65c == 0) {
                mBrk.setPlaySpeed(1.0f);
            }
        } else if (!r26) {
            field_0x65c--;
            
            if (field_0x66b == 0xE) {
                if (field_0x65c == 120) {
                    fopAcM_create(PROC_BOMB, daBomb_c::prm_make(daBomb_c::STATE_8, false, false), &current.pos);
                } else if (field_0x65c == 0) {
                    resetCursor(false);
                }
            }
            
            if (field_0x66b == 15 && field_0x65c == 30) {
                fopAcM_fastCreateItem(&field_0x634, field_0x640, -1, NULL, NULL, 5.0f, 50.0f, -7.0f);
            }
        }
    }
    if (field_0x673 && !r26 && field_0x65c == 0) {
        onActive();
    } else {
        offActive();
    }
    
    if (mDoGaC_GbaLink() && mDoGaC_RecvStatusCheck(6) && mDoGac_SendStatusCheck(7)) {
        if (isActive() || mItem.field_0x0 == 0x15) {
            GbaItemUse();
        } else {
            mEffect = 0;
        }
        
        mDoGaC_DataStatusReset(6);
        mDoGac_SendDataSet((u32*)&mEffect, 4, 7, mEffect);
    }
    
    if (field_0x67f) {
        field_0x67f = false;
        
        if (!field_0x67d &&
            !daPy_getPlayerLinkActorClass()->checkNoControll() &&
            !dComIfGp_checkPlayerStatus0(0, daPyStts0_CRAWL_e) &&
            (
                !daPy_getPlayerActorClass()->checkPlayerFly() ||
                dComIfGp_checkPlayerStatus0(0, daPyStts0_SWIM_e) ||
                dComIfGp_checkPlayerStatus0(0, daPyStts0_SHIP_RIDE_e)
            )
        ) {
            mMode = MODE_LOOK_ATTENTION;
            offActive();
            eyePos = current.pos;
            
            cXyz sp3c = eyePos - player->current.pos;
            if (sp3c.absXZ() < 10.0f) {
                eyePos.x += 10.0f * cM_ssin(player->shape_angle.y);
                eyePos.z += 10.0f * cM_scos(player->shape_angle.y);
            }
            attention_info.position = eyePos;
            
            if (isHold()) {
                s16 angle = fopAcM_searchPlayerAngleY(this);
                if (!field_0x675) {
                    angle = -angle;
                }
                shape_angle.z = angle;
            } else {
                shape_angle.y = fopAcM_searchPlayerAngleY(this);
                shape_angle.x = cM_atan2s((player->eyePos - eyePos).absXZ(), player->eyePos.y - eyePos.y);
                field_0x628 = 50.0f;
            }
            
            field_0x66b = 0x10;
            field_0x65c = 3*30;
        }
    }
    
    if (mDoGaC_GbaLink() && mDoGaC_RecvStatusCheck(0xC) && mDoGac_SendStatusCheck(0xD)) {
        Shopping();
        mDoGaC_DataStatusReset(0xC);
        mDoGac_SendDataSet((u32*)&mItemBuy, 4, 0xD, mItemBuy.U32);
    }
    
    if ((g_mDoCPd_cpadInfo[mDoGaC_getPortNo()].mGamepadErrorFlags == 0 && fopAcM_GetName(player) != PROC_NPC_KAM) &&
        ((isActive() && !field_0x675 && CPad_CHECK_TRIG_R(mDoGaC_getPortNo())) ||
        (mFlags.field_0x3_5 != 0 && (CPad_CHECK_TRIG_R(mDoGaC_getPortNo()) || CPad_CHECK_TRIG_A(mDoGaC_getPortNo())))))
    {
        offHold();
        mIsFree = false;
        if (getFollowTarget() != 0) {
            setTargetID(fpcM_ERROR_PROCESS_ID_e);
            setFollowTarget(false);
        } else if (stage_type != dStageType_MINIBOSS_e) {
            dAttList_c* attList = dComIfGp_getAttention().GetLockonList(0);
            if (attList) {
                fopAc_ac_c* r3 = attList->getActor();
                if (r3) {
                    if (fopAcM_CheckStatusMap(r3, 0) && !fopAcM_CheckStatus(r3, fopAcStts_BOSS_e) && fopAcM_GetName(r3) != PROC_FGANON) {
                        current.pos = r3->current.pos;
                        home.pos = r3->current.pos;
                        setTargetID(attList->getPid());
                        setFollowTarget(true);
                    }
                }
            }
        }
    }
    
    if (!isFree()) {
        if (getFollowTarget() == 1) {
            fopAc_ac_c* r3 = fopAcM_SearchByID(getTargetID());
            if (r3 == NULL || !fopAcM_CheckStatusMap(r3, 0)) {
                setTargetID(fpcM_ERROR_PROCESS_ID_e);
                setFollowTarget(false);
            }
        }
        
        if (getFollowTarget() == 0) {
            if (fopAcM_GetName(player) == PROC_NPC_KAM) {
                onFree();
            } else {
                current.pos = player->current.pos;
                home.pos = player->current.pos;
            }
        } else {
            fopAc_ac_c* r3 = fopAcM_SearchByID(getTargetID());
            current.pos = r3->current.pos;
            home.pos = r3->current.pos;
        }
        
        shape_angle.setall(0);
        field_0x628 = 2.5f;
        
        if (g_mDoCPd_cpadInfo[mDoGaC_getPortNo()].mGamepadErrorFlags == 0 && !CPad_CHECK_HOLD_L(mDoGaC_getPortNo()) && (
            CPad_CHECK_HOLD_LEFT(mDoGaC_getPortNo()) | CPad_CHECK_HOLD_RIGHT(mDoGaC_getPortNo()) |
            CPad_CHECK_HOLD_UP(mDoGaC_getPortNo()) | CPad_CHECK_HOLD_DOWN(mDoGaC_getPortNo())
        ) && isActive() && !mHold)
        {
            onFree();
        }
    } else if (!mHold) {
        CursorMove(this, stage_type);
    }
    
    if (mMode == MODE_MOVE) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
    }
}

/* 800D303C-800D30D4       .text modeDelete__7daAgb_cFv */
void daAgb_c::modeDelete() {
    if (field_0x664 == 0 && mDoGaC_GbaLink() && mDoGac_SendStatusCheck(1))
    {
        mPlayerName = 0x1000000;
        mDoGac_SendDataSet((u32*)&mPlayerName, 4, 1, 0);
        field_0x664++;
    }
}

void daAgb_c::modeProcCall() {
    typedef void (daAgb_c::*modeProc)();
    static modeProc mode_proc[] = {
        &daAgb_c::modeMove,
        &daAgb_c::modeLookAttention,
        &daAgb_c::modeLoad,
        &daAgb_c::modeDelete,
    };

    (this->*mode_proc[mMode])();
}

/* 800D30D4-800D36F4       .text daAgb_Execute__FP7daAgb_c */
static BOOL daAgb_Execute(daAgb_c* i_this) {
    fopAc_ac_c* i_actor = i_this;
    daPy_py_c* player = daPy_getPlayerActorClass();
    i_this->field_0x679 = false;

    if (mDoGaC_GbaLink() && mDoGaC_RecvStatusCheck(4)) {
        i_this->FlagsRecv();
        mDoGaC_DataStatusReset(4);
    } else {
        g_mDoCPd_cpadInfo[mDoGaC_getPortNo()].mGamepadErrorFlags = 1;
    }

    // single | instead of double ||: bug?
    BOOL var_r27 = dComIfGp_event_runCheck() | dMenu_flag();

    u32 stage_type = dStage_stagInfo_GetSTType(dComIfGp_getStageStagInfo());

    if (mDoGaC_GbaLink()) {
        if (mDoGaC_RecvStatusCheck(8)) {
            i_this->SwitchOn();
        }

        if (mDoGac_SendStatusCheck(9)) {
            i_this->FlagsSend(stage_type);
        }

        i_this->field_0x680 = true;
    } else if (i_this->field_0x680) {
        i_this->field_0x680 = false;
        i_this->resetCursor(true);
        i_this->offActive();

        if (i_this->field_0x65c != 0) {
            if (i_this->field_0x66b == 3 || i_this->field_0x66b == 12) {
                daPy_getPlayerLinkActorClass()->setHoverBoots(10);
            } else if (i_this->field_0x66b == 4 || i_this->field_0x66b == 13 || i_this->field_0x66b == 12) {
                daPy_getPlayerLinkActorClass()->setTinkleShield(10);
            }

            i_this->field_0x65c = 0;
        }

        i_this->field_0x66b = 0;
        i_this->field_0x630 = 0;
        i_this->field_0x632 = 0;
        i_this->field_0x684.remove();
    }

    i_this->modeProcCall();

    if (mDoGaC_GbaLink()) {
        if (i_this->isFree()) {
            if (i_this->current.pos.x != i_this->old.pos.x ||
                i_this->current.pos.z != i_this->old.pos.z)
            {
                daAgb_c::mFlags.field_0x5_3 = 1;
            } else {
                daAgb_c::mFlags.field_0x5_3 = 0;
            }
        } else {
            daPy_lk_c* player_p2 = daPy_getPlayerLinkActorClass();
            if ((dComIfGp_getPlayer(0) == player_p2 && !player->checkPlayerFly()) ||
                ((fopAcM_GetName(player) == PROC_NPC_MD && !daNpc_Md_c::isFlying()) ||
                 (fopAcM_GetName(player) == PROC_NPC_CB1 && !daNpc_Cb1_c::isFlying()) ||
                 fopAcM_GetName(player) == PROC_NPC_OS))
            {
                f32 speedF = fabs(player->speedF);

                if (speedF <= 0.0f) {
                    daAgb_c::mFlags.field_0x5_3 = 0;
                } else if (speedF < 5.0f) {
                    daAgb_c::mFlags.field_0x5_3 = 1;
                } else if (speedF < 10.0f) {
                    daAgb_c::mFlags.field_0x5_3 = 2;
                } else if (speedF < 15.0f) {
                    daAgb_c::mFlags.field_0x5_3 = 3;
                } else {
                    daAgb_c::mFlags.field_0x5_3 = 4;
                }
            } else {
                daAgb_c::mFlags.field_0x5_3 = 0;
            }
        }

        if (i_this->isHold() && !var_r27 && i_this->mMode != daAgb_c::MODE_LOOK_ATTENTION) {
            if (i_this->field_0x675) {
                if (i_this->field_0x676) {
                    i_this->shape_angle.z += 0x1000;
                } else {
                    i_this->shape_angle.z = fopAcM_searchPlayerAngleY(i_actor);
                }
            } else {
                i_this->shape_angle.z += 0x200;
            }
        }

        mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y + i_this->field_0x628,
                               i_this->current.pos.z);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
        mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
        mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
        i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

        if (i_this->mMode == daAgb_c::MODE_LOOK_ATTENTION) {
            i_this->mBrk.setFrame(19.0f);
        } else if (i_this->field_0x65c != 0 &&
                   (i_this->field_0x66b == 4 || i_this->field_0x66b == 13 ||
                    i_this->field_0x66b == 12 || i_this->field_0x66b == 3))
        {
            if (i_this->field_0x65c > 60) {
                i_this->mBrk.setFrame(19.0f);
            } else {
                i_this->mBrk.play();
            }
        } else if (((i_this->isActive() || i_this->field_0x676) &&
                    (i_this->isFree() || i_this->getFollowTarget() == 1)) ||
                   (i_this->field_0x66b == 14 && i_this->field_0x65c > 120))
        {
            i_this->mBrk.play();
        } else {
            i_this->mBrk.setFrame(0.0f);
        }
    }

    return TRUE;
}

/* 800D36F4-800D38EC       .text daAgb_Draw__FP7daAgb_c */
static BOOL daAgb_Draw(daAgb_c* i_this) {
    u8 var_r6 = 1;

    if (mDoGaC_GbaLink()) {
        if (i_this->field_0x66f && !daAgb_c::mFlags.field_0xa_7 &&
            (!dComIfGp_event_runCheck() ||
             dComIfGp_evmng_startCheck("DEFAULT_AGB_LOOK_ATTENTION") ||
             i_this->mMode == daAgb_c::MODE_LOAD) &&
            (i_this->field_0x65c == 0 || i_this->mMode == daAgb_c::MODE_LOOK_ATTENTION || i_this->field_0x66b == 0x10 ||
             i_this->field_0x66b == 3 || i_this->field_0x66b == 12 || i_this->field_0x66b == 4 ||
             i_this->field_0x66b == 13 || (i_this->field_0x66b == 14 && i_this->field_0x65c > 120)))
        {
            i_this->mBrk.getBrkAnm()->setFrame(i_this->mBrk.getFrame());

            J3DModelData* modelData = i_this->mpModel->getModelData();
            for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
                J3DMaterial* mat_p = modelData->getMaterialNodePointer(i);
                mat_p->setTevColor(1, &l_HIO.field_0x04[0]);
                mat_p->setTevColor(2, &l_HIO.field_0x04[1]);
            }

            mDoExt_modelUpdateDL(i_this->mpModel);

            if (i_this->field_0x679 && (i_this->current.pos.y - i_this->mCrrPos.GetGroundH()) > 2.5f)
            {
                dComIfGd_setSimpleShadow2(&i_this->current.pos, i_this->mCrrPos.GetGroundH(), 50.0f,
                                          i_this->mCrrPos.mGndChk, 0, 1.0f, &i_this->mTexObj);
            }

            var_r6 = 0;
        }
    }

    dMap_drawPoint(3, i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z,
                   i_this->current.roomNo, -0x8000, var_r6, 0, 0);
    return TRUE;
}

/* 800D38F0-800D38F8       .text daAgb_IsDelete__FP7daAgb_c */
static BOOL daAgb_IsDelete(daAgb_c* i_this) {
    return TRUE;
}

/* 800D38F8-800D394C       .text daAgb_Delete__FP7daAgb_c */
static BOOL daAgb_Delete(daAgb_c* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Agb");
    i_this->field_0x684.remove();
    return TRUE;
}

/* 800D394C-800D396C       .text createHeap_CB__FP10fopAc_ac_c */
static BOOL createHeap_CB(fopAc_ac_c* i_this) {
    return static_cast<daAgb_c*>(i_this)->createHeap();
}

/* 800D396C-800D3B58       .text createHeap__7daAgb_cFv */
BOOL daAgb_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Agb", AGB_BDL_AGBCURSOR);
    JUT_ASSERT(VERSION_SELECT(2960, 2960, 3277, 3286), modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000002);
    if (mpModel == NULL) {
        return FALSE;
    }

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Agb", AGB_BRK_AGBCURSOR);
    if (!mBrk.init(modelData, pbrk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 1)) {
        return FALSE;
    }

    ResTIMG* img_p = modelData->getTexture()->getResTIMG(1);
    GXInitTexObj(&mTexObj, (u8*)img_p + img_p->imageOffset, img_p->width, img_p->height,
                 (GXTexFmt)img_p->format, (GXTexWrapMode)img_p->wrapS, (GXTexWrapMode)img_p->wrapT,
                 (GXBool)(img_p->mipmapCount > 1));
    GXInitTexObjLOD(&mTexObj, (GXTexFilter)img_p->minFilter, (GXTexFilter)img_p->magFilter,
                    img_p->minLOD * 0.125f, img_p->maxLOD * 0.125f, img_p->LODBias * 0.01f,
                    img_p->biasClamp, img_p->doEdgeLOD, (GXAnisotropy)img_p->maxAnisotropy);

    return TRUE;
}

/* 800D3B58-800D3D2C       .text daAgb_Create__FP10fopAc_ac_c */
static cPhs_State daAgb_Create(fopAc_ac_c* i_this) {
    daAgb_c* a_this = (daAgb_c*)i_this;
    fopAcM_SetupActor(i_this, daAgb_c);

    cPhs_State phase = dComIfG_resLoad(&a_this->mPhase, "Agb");
    if (phase == cPhs_COMPLEATE_e) {
        dComIfGp_setAgb(a_this);
        if (!fopAcM_entrySolidHeap(i_this, createHeap_CB, 0x500)) {
            return cPhs_ERROR_e;
        }

        a_this->mCrrPos.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), (void*)NULL, NULL);
        a_this->mCrrPos.SetWall(171.0f, 50.0f);
        a_this->mCrrPos.SetGndUpY(170.0f);
        a_this->mCrrPos.ClrNoRoof();
        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->mAcchCir);
        a_this->mAcch.OnLineCheck();
        a_this->mAcch.SetGrndNone();
        a_this->mAcchCir.SetWall(171.0f, 40.0f);

        TestDataManager[4].field_0x0 = (u32)&a_this->mGbaFlg;
        TestDataManager[8].field_0x0 = (u32)&a_this->mSwitch;
        TestDataManager[6].field_0x0 = (u32)&a_this->mItem;
        TestDataManager[12].field_0x0 = (u32)&a_this->mShop;

        a_this->field_0x670 = false;
        a_this->field_0x65c = 0;
        a_this->field_0x66b = 0;
        a_this->field_0x66f = mDoGaC_getComEnable();
        a_this->mIsMsgSend = mDoGaC_getComEnable() > 0;
        a_this->field_0x654 = 0;
        a_this->setTargetID(fpcM_ERROR_PROCESS_ID_e);
        a_this->setFollowTarget(false);
        a_this->field_0x67b = false;
        a_this->field_0x680 = true;

        fopAcM_setStageLayer(a_this);
        a_this->eventInfo.setEventName("DEFAULT_AGB_USE");
    }

    return phase;
}

static actor_method_class l_daAgb_Method = {
    (process_method_func)daAgb_Create,
    (process_method_func)daAgb_Delete,
    (process_method_func)daAgb_Execute,
    (process_method_func)daAgb_IsDelete,
    (process_method_func)daAgb_Draw,
};

actor_process_profile_definition g_profile_AGB = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_AGB,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daAgb_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_AGB,
    /* Actor SubMtd */ &l_daAgb_Method,
    /* Status       */ fopAcStts_UNK4000_e | fopAcStts_NOPAUSE_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
