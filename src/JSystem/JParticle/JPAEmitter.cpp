//
// Generated by dtk
// Translation Unit: JPAEmitter.cpp
//

#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPADynamicsBlock.h"
#include "JSystem/JParticle/JPAExtraShape.h"
#include "JSystem/JParticle/JPASweepShape.h"
#include "JSystem/JParticle/JPAKeyBlock.h"
#include "JSystem/JMath/JMATrigonometric.h"
#include "dolphin/mtx/mtx.h"
#include "dolphin/mtx/mtxvec.h"

JPAEmitterInfo JPABaseEmitter::emtrInfo;

/* 8025C128-8025C254       .text calcVolumePoint__14JPABaseEmitterFv */
void JPABaseEmitter::calcVolumePoint() {
    emtrInfo.mVolumePos.zero();
    emtrInfo.mVelOmni.set(getRandomSF(), getRandomSF(), getRandomSF());
    emtrInfo.mVelAxis.set(getRandomSF(), 0.0f, getRandomSF());
}

/* 8025C254-8025C394       .text calcVolumeLine__14JPABaseEmitterFv */
void JPABaseEmitter::calcVolumeLine() {
    if (checkEmDataFlag(0x02)) {
        emtrInfo.mVolumePos.set(0.0f, 0.0f, emtrInfo.mVolumeSize * ((f32)emtrInfo.mVolumeEmitIdx / ((f32)emtrInfo.mVolumeEmitCount - 1.0f) - 0.5f));
        emtrInfo.mVolumeEmitIdx++;
    } else {
        emtrInfo.mVolumePos.set(0.0f, 0.0f, emtrInfo.mVolumeSize * getRandomSF());
    }

    emtrInfo.mVelOmni.mul(emtrInfo.mVolumePos, emtrInfo.mEmitterGlobalScale);
    emtrInfo.mVelAxis.set(emtrInfo.mVolumePos.x, 0.0f, emtrInfo.mVolumePos.z);
}

/* 8025C394-8025C538       .text calcVolumeCircle__14JPABaseEmitterFv */
void JPABaseEmitter::calcVolumeCircle() {
    s16 angle;
    if (checkEmDataFlag(0x02)) {
        s16 idx = (0x10000 * emtrInfo.mVolumeEmitIdx / emtrInfo.mVolumeEmitCount);
        angle = idx * mVolumeSweep;
        emtrInfo.mVolumeEmitIdx++;
    } else {
        angle = mVolumeSweep * getRandomSS();
    }

    f32 rad = getRandomF();
    if (checkEmDataFlag(0x01))
        rad = 1.0f - rad * rad;
    rad = emtrInfo.mVolumeSize * (mVolumeMinRad + rad * (1.0f - mVolumeMinRad));

    emtrInfo.mVolumePos.set(rad * JMASSin(angle), 0.0f, rad * JMASCos(angle));
    emtrInfo.mVelOmni.mul(emtrInfo.mVolumePos, emtrInfo.mEmitterGlobalScale);
    emtrInfo.mVelAxis.set(emtrInfo.mVolumePos.x, 0.0f, emtrInfo.mVolumePos.z);
}

/* 8025C538-8025C63C       .text calcVolumeCube__14JPABaseEmitterFv */
void JPABaseEmitter::calcVolumeCube() {
    emtrInfo.mVolumePos.set(emtrInfo.mVolumeSize * getRandomSF(), emtrInfo.mVolumeSize * getRandomSF(), emtrInfo.mVolumeSize * getRandomSF());
    emtrInfo.mVelOmni.mul(emtrInfo.mVolumePos, emtrInfo.mEmitterGlobalScale);
    emtrInfo.mVelAxis.set(emtrInfo.mVolumePos.x, 0.0f, emtrInfo.mVolumePos.z);
}

/* 8025C63C-8025C8A4       .text calcVolumeSphere__14JPABaseEmitterFv */
void JPABaseEmitter::calcVolumeSphere() {
    s16 x;
    s16 angle;
    if (checkEmDataFlag(0x02)) {
        u16 angleNo = (emtrInfo.mVolumeEmitAngleCount * 0x10000) / emtrInfo.mVolumeEmitAngleMax;
        x = (u16)((emtrInfo.mVolumeEmitXCount * 0x8000) / (emtrInfo.mDivNumber - 1) + 0x4000);
        angle = (f32)angleNo * mVolumeSweep + 32768.0f;
        emtrInfo.mVolumeEmitAngleCount++;
        if (emtrInfo.mVolumeEmitAngleCount == emtrInfo.mVolumeEmitAngleMax) {
            emtrInfo.mVolumeEmitAngleCount = 0;
            ++emtrInfo.mVolumeEmitXCount;
            if (emtrInfo.mVolumeEmitXCount * 2 < emtrInfo.mDivNumber) {
                emtrInfo.mVolumeEmitAngleMax = (emtrInfo.mVolumeEmitAngleMax != 1) ? emtrInfo.mVolumeEmitAngleMax + 4 : emtrInfo.mVolumeEmitAngleMax + 3;
            } else {
                emtrInfo.mVolumeEmitAngleMax = (emtrInfo.mVolumeEmitAngleMax != 4) ? emtrInfo.mVolumeEmitAngleMax - 4 : 1;
            }
        }
    } else {
        x = getRandomSS() >> 1;
        angle = mVolumeSweep * getRandomSS();
    }

    f32 rad = getRandomF();
    if (checkEmDataFlag(0x01)) {
        rad = 1.0f - rad * rad * rad;
    }
    rad = emtrInfo.mVolumeSize * (mVolumeMinRad + rad * (1.0f - mVolumeMinRad));

    emtrInfo.mVolumePos.set(
        rad * JMASCos(x) * JMASSin(angle),
        -rad * JMASSin(x),
        rad * JMASCos(x) * JMASCos(angle)
    );
    emtrInfo.mVelOmni.mul(emtrInfo.mVolumePos, emtrInfo.mEmitterGlobalScale);
    emtrInfo.mVelAxis.set(emtrInfo.mVolumePos.x, 0.0f, emtrInfo.mVolumePos.z);
}

/* 8025C8A4-8025CA28       .text calcVolumeCylinder__14JPABaseEmitterFv */
void JPABaseEmitter::calcVolumeCylinder() {
    s16 angle = mVolumeSweep * getRandomSS();
    f32 rad = getRandomF();
    if (checkEmDataFlag(0x01))
        rad = 1.0f - rad * rad;
    rad = emtrInfo.mVolumeSize * (mVolumeMinRad + rad * (1.0f - mVolumeMinRad));

    // Fakematch, needed to force mRandomSeed.value to be reloaded before the third random call
    *(f32*)NULL = *(f32*)NULL;

    emtrInfo.mVolumePos.set(rad * JMASSin(angle), emtrInfo.mVolumeSize * getRandomRF(), rad * JMASCos(angle));
    emtrInfo.mVelOmni.mul(emtrInfo.mVolumePos, emtrInfo.mEmitterGlobalScale);
    emtrInfo.mVelAxis.set(emtrInfo.mVolumePos.x, 0.0f, emtrInfo.mVolumePos.z);
}

/* 8025CA28-8025CB54       .text calcVolumeTorus__14JPABaseEmitterFv */
void JPABaseEmitter::calcVolumeTorus() {
    s16 angle1 = mVolumeSweep * getRandomSS();
    s16 angle2 = getRandomSS();
    f32 rad = mVolumeMinRad * emtrInfo.mVolumeSize;

    emtrInfo.mVelAxis.set(rad * JMASSin(angle1) * JMASCos(angle2), rad * JMASSin(angle2), rad * JMASCos(angle1) * JMASCos(angle2));
    emtrInfo.mVolumePos.set(emtrInfo.mVelAxis.x + emtrInfo.mVolumeSize * JMASSin(angle1), emtrInfo.mVelAxis.y, emtrInfo.mVelAxis.z + emtrInfo.mVolumeSize * JMASCos(angle1));
    emtrInfo.mVelOmni.mul(emtrInfo.mVolumePos, emtrInfo.mEmitterGlobalScale);
}

/* 8025CB54-8025D0B0       .text create__14JPABaseEmitterFP20JPADataBlockLinkInfo */
void JPABaseEmitter::create(JPADataBlockLinkInfo* info) {
    mpDataLinkInfo = info;

    JPADynamicsBlock * dyn = getEmitterDataBlockInfoPtr()->getDynamics();
    dyn->getEmitterScl(mEmitterScale);
    dyn->getEmitterTrs(mEmitterTranslation);
    dyn->getEmitterRot(mEmitterRot);
    mVolumeType = dyn->getVolumeType();
    mRateStep = dyn->getRateStep();
    mDivNumber = dyn->getDivNumber();
    mRate = dyn->getRate();
    mRateRndm = dyn->getRateRndm();
    mMaxFrame = dyn->getMaxFrame();
    mStartFrame = dyn->getStartFrame();
    mVolumeSize = dyn->getVolumeSize();
    mVolumeSweep = dyn->getVolumeSweep();
    mVolumeMinRad = dyn->getVolumeMinRad();
    mLifeTime = dyn->getLifeTime();
    mLifeTimeRndm = dyn->getLifeTimeRndm();
    mMoment = dyn->getMoment();
    mMomentRndm = dyn->getMomentRndm();
    mInitialVelRatio = dyn->getInitVelRatio();
    mAccelRndm = dyn->getAccelRndm();
    mAirResist = dyn->getAirResist();
    mAirResistRndm = dyn->getAirResistRndm();
    mInitialVelOmni = dyn->getInitVelOmni();
    mInitialVelAxis = dyn->getInitVelAxis();
    mInitialVelRndm = dyn->getInitVelRndm();
    mInitialVelDir = dyn->getInitVelDir();
    mAccel = dyn->getAccel();
    dyn->getEmitterDir(mEmitterDir);
    mEmitterDir.normalize();
    mSpread = dyn->getSpread();
    mDataFlag = dyn->getDataFlag();
    mUseKeyFlag = dyn->getUseKeyFlag();
    initStatus(JPAEmtrStts_FirstEmit | JPAEmtrStts_RateStepEmit);
    MTXIdentity(mGlobalRotation);
    mGlobalDynamicsScale.x = 1.0f;
    mGlobalDynamicsScale.y = 1.0f;
    mGlobalDynamicsScale.z = 1.0f;
    mGlobalTranslation.zero();
    mGlobalParticleScale.x = 1.0f;
    mGlobalParticleScale.y = 1.0f;
    mGlobalParticleScale.z = 1.0f;
    mGlobalEnvColor.r = mGlobalEnvColor.g = mGlobalEnvColor.b = mGlobalEnvColor.a = 0xFF;
    mGlobalPrmColor.r = mGlobalPrmColor.g = mGlobalPrmColor.b = mGlobalPrmColor.a = 0xFF;
    mEmitCount = 0.0f;
    mRateStepTimer = 0.0f;
    mTick.setFrame(0.0f);
    mTime.setFrame(0.0f);
    mUserData = 0;
    mRandomSeed.setSeed(emtrInfo.mRandom.get());
    mFieldManager.initField(info, &emtrInfo);

    switch (mVolumeType) {
    case 4: mVolumeFunc = &JPABaseEmitter::calcVolumePoint; break;
    case 6: mVolumeFunc = &JPABaseEmitter::calcVolumeLine; break;
    case 5: mVolumeFunc = &JPABaseEmitter::calcVolumeCircle; break;
    case 0: mVolumeFunc = &JPABaseEmitter::calcVolumeCube; break;
    case 1: mVolumeFunc = &JPABaseEmitter::calcVolumeSphere; break;
    case 2: mVolumeFunc = &JPABaseEmitter::calcVolumeCylinder; break;
    case 3: mVolumeFunc = &JPABaseEmitter::calcVolumeTorus; break;
    default: mVolumeFunc = NULL; break;
    }
}

/* 8025D0B0-8025D294       .text calcEmitterInfo__14JPABaseEmitterFv */
void JPABaseEmitter::calcEmitterInfo() {
    emtrInfo.mpCurEmitter = this;
    emtrInfo.mVolumeEmitXCount = 0;
    emtrInfo.mVolumeEmitAngleCount = 0;
    emtrInfo.mVolumeEmitAngleMax = 1;
    emtrInfo.mDivNumber = mDivNumber * 2 + 1;
    emtrInfo.mVolumeSize = mVolumeSize;
    
    Mtx mtxScale, mtxRot, mtx;
    MTXScale(mtxScale, mEmitterScale.x, mEmitterScale.y, mEmitterScale.z);
    JPAGetXYZRotateMtx((mEmitterRot.x << 14) / 0x5a, (mEmitterRot.y << 14) / 0x5a, (mEmitterRot.z << 14) / 0x5a, mtxRot);
    
    MTXScale(mtx, mGlobalDynamicsScale.x, mGlobalDynamicsScale.y, mGlobalDynamicsScale.z);
    MTXConcat(mGlobalRotation, mtx, mtx);
    mtx[0][3] = mGlobalTranslation.x;
    mtx[1][3] = mGlobalTranslation.y;
    mtx[2][3] = mGlobalTranslation.z;
    MTXCopy(mGlobalRotation, emtrInfo.mGlobalRot);
    MTXConcat(mGlobalRotation, mtxRot, emtrInfo.mEmitterGlobalRot);
    MTXConcat(emtrInfo.mEmitterGlobalRot, mtxScale, emtrInfo.mEmitterGlobalSR);
    JPAGetDirMtx(mEmitterDir, emtrInfo.mEmitterDirMtx);
    emtrInfo.mEmitterGlobalScale.mul(mEmitterScale, mGlobalDynamicsScale);
    emtrInfo.mEmitterTranslation.x = mEmitterTranslation.x;
    emtrInfo.mEmitterTranslation.y = mEmitterTranslation.y;
    emtrInfo.mEmitterTranslation.z = mEmitterTranslation.z;
    emtrInfo.mPublicScale.mul(mGlobalDynamicsScale, JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
    MTXMultVec(mtx, mEmitterTranslation, emtrInfo.mEmitterGlobalCenter);
}

/* 8025D294-8025D3C0       .text calc__14JPABaseEmitterFv */
void JPABaseEmitter::calc() {
    emtrInfo.mVolumeEmitCount = 0;
    if (!checkStatus(JPAEmtrStts_StopCalc)) {
        calcKey();
        calcBeforeCB();
        calcEmitterInfo();
        mDraw.calc();
        mFieldManager.preCalc();
        if (!checkStatus(JPAEmtrStts_EnableDeleteEmitter))
            calcCreatePtcls();
        calcAfterCB();
        calcParticle();
        calcChild();
        mTick.incFrame();
    } else {
        calcBeforeCB();
        calcAfterCB();
    }
}

/* 8025D3C0-8025D5D4       .text calcCreatePtcls__14JPABaseEmitterFv */
void JPABaseEmitter::calcCreatePtcls() {
    if (checkStatus(JPAEmtrStts_RateStepEmit)) {
        s32 emitCount = 0;
        if (checkEmDataFlag(0x02)) { // Fixed interval
            emitCount = (mVolumeType == 1) ?
                (mDivNumber + 1) * (mDivNumber - 1) * 4 + 6 : // Sphere
                mDivNumber;
            emtrInfo.mVolumeEmitIdx = 0;
        } else {
            f32 incr = mRate * (1.0f + mRateRndm * getRandomRF());
            f32 newCounter = mEmitCount + incr;
            mEmitCount = newCounter;

            if (newCounter >= 1) {
                emitCount = mEmitCount;
                mEmitCount -= emitCount;
            } else if (incr > 0 && checkStatus(JPAEmtrStts_FirstEmit)) {
                emitCount = 1;
            }
        }

        emtrInfo.mVolumeEmitCount = emitCount;

        if (checkStatus(JPAEmtrStts_StopEmit))
            emitCount = 0;

        for (s32 i = 0; i < emitCount; i++) {
            JPABaseParticle* ptcl = createParticle();
            if (ptcl == NULL)
                break;
        }
    }

    if (++mRateStepTimer >= mRateStep + 1) {
        mRateStepTimer -= mRateStep + 1;
        setStatus(JPAEmtrStts_RateStepEmit);
    } else {
        clearStatus(JPAEmtrStts_RateStepEmit);
    }
    clearStatus(JPAEmtrStts_FirstEmit);
}

/* 8025D5D4-8025D670       .text createChildren__14JPABaseEmitterFP15JPABaseParticle */
void JPABaseEmitter::createChildren(JPABaseParticle* ptcl) {
    s32 num = getEmitterDataBlockInfoPtr()->getSweepShape()->getRate();
    for (s32 i = 0; i < num; i++) {
        JPABaseParticle * chld = getPtclFromVacList();
        if (chld == NULL)
            break;
        mChildParticles.prepend(&chld->mLink);
        chld->initChild(ptcl);
        mDraw.initChild(ptcl, chld);
    }
}

/* 8025D670-8025D6E0       .text createParticle__14JPABaseEmitterFv */
JPABaseParticle * JPABaseEmitter::createParticle() {
    JPABaseParticle * ptcl = getPtclFromVacList();
    if (ptcl != NULL) {
        mActiveParticles.prepend(&ptcl->mLink);
        (this->*mVolumeFunc)();
        ptcl->initParticle();
        mDraw.initParticle(ptcl);
    }
    return ptcl;
}

/* 8025D6E0-8025D7E0       .text calcParticle__14JPABaseEmitterFv */
void JPABaseEmitter::calcParticle() {
    for (JSULink<JPABaseParticle> * link = mActiveParticles.getFirst(); link != NULL;) {
        JSULink<JPABaseParticle> * next = link->getNext();
        JPABaseParticle * ptcl = (JPABaseParticle *) link->getObjectPtr();
        ptcl->incFrame();
        if (!ptcl->checkStatus(0x80)) {
            ptcl->calcVelocity();
            ptcl->calcCB(this);
            if (!ptcl->checkStatus(0x02)) {
                mDraw.calcParticle(ptcl);
                if (getEmitterDataBlockInfoPtr()->getSweepShape() != NULL && ptcl->checkCreateChild())
                    createChildren(ptcl);
                ptcl->calcPosition();
            }
        } else {
            ptcl->setStatus(0x02);
        }

        if (ptcl->checkStatus(0x02))
            deleteParticle(ptcl, &mActiveParticles);

        link = next;
    }
}

/* 8025D7E0-8025D8CC       .text calcChild__14JPABaseEmitterFv */
void JPABaseEmitter::calcChild() {
    for (JSULink<JPABaseParticle> * link = mChildParticles.getFirst(); link != NULL;) {
        JSULink<JPABaseParticle> * next = link->getNext();
        JPABaseParticle * ptcl = (JPABaseParticle *) link->getObjectPtr();
        ptcl->incFrame();
        if (!ptcl->checkStatus(0x80)) {
            if (ptcl->getAge() != 0)
                ptcl->calcVelocity();
            ptcl->calcCB(this);
            if (!ptcl->checkStatus(0x02)) {
                mDraw.calcChild(ptcl);
                ptcl->calcPosition();
            }
        } else {
            ptcl->setStatus(0x02);
        }

        if (ptcl->checkStatus(0x02))
            deleteParticle(ptcl, &mChildParticles);

        link = next;
    }
}

/* 8025D8CC-8025DA90       .text calcKey__14JPABaseEmitterFv */
void JPABaseEmitter::calcKey() {
    for (s32 i = 0; i < getEmitterDataBlockInfoPtr()->getKeyNum(); i++) {
        JPAKeyBlock* key = getEmitterDataBlockInfoPtr()->getKey()[i];
        f32 tick = mTick.getFrame();
        const f32* dataPtr = key->getKeyDataPtr();
        u32 dataNum = key->getNumber();
        if (key->isLoopEnable()) {
            s32 tickMax = (s32)(dataPtr[(dataNum - 1) * 4]) + 1;
            s32 numLoops = (s32)tick / tickMax;
            tick -= numLoops * tickMax;
        }
        f32 value = JPAGetKeyFrameValue(tick, dataNum, dataPtr);

        switch (key->getID()) {
        case 0: mRate = value; break;
        case 1: mVolumeSize = value; break;
        case 2: mVolumeSweep = value; break;
        case 3: mVolumeMinRad = value; break;
        case 4: mLifeTime = value; break;
        case 5: mMoment = value; break;
        case 6: mInitialVelOmni = value; break;
        case 7: mInitialVelAxis = value; break;
        case 8: mInitialVelDir = value; break;
        case 9: mSpread = value; break;
        case 10: mDraw.setKeyScl(value); break;
        }
    }
}

/* 8025DA90-8025DAD8       .text deleteParticle__14JPABaseEmitterFP15JPABaseParticleP26JSUList<15JPABaseParticle> */
void JPABaseEmitter::deleteParticle(JPABaseParticle* ptcl, JSUList<JPABaseParticle>* list) {
    JSUPtrList * ptrlist = list;
    ptrlist->remove(&ptcl->mLink);
    mpPtclVacList->prepend(&ptcl->mLink);
}

/* 8025DAD8-8025DB68       .text deleteAllParticle__14JPABaseEmitterFv */
void JPABaseEmitter::deleteAllParticle() {
    for (JSULink<JPABaseParticle>* link = mActiveParticles.getFirst(); link != NULL;) {
        JSULink<JPABaseParticle>* next = link->getNext();
        mActiveParticles.remove(link);
        mpPtclVacList->prepend(link);
        link = next;
    }

    for (JSULink<JPABaseParticle>* link = mChildParticles.getFirst(); link != NULL;) {
        JSULink<JPABaseParticle>* next = link->getNext();
        mChildParticles.remove(link);
        mpPtclVacList->prepend(link);
        link = next;
    }
}

/* 8025DB68-8025DBB4       .text getPtclFromVacList__14JPABaseEmitterFv */
JPABaseParticle * JPABaseEmitter::getPtclFromVacList() {
    JPABaseParticle * ptcl = NULL;
    if (mpPtclVacList->getNumLinks() != 0) {
        ptcl = (JPABaseParticle*)mpPtclVacList->getFirstLink()->getObjectPtr();
        mpPtclVacList->remove(&ptcl->mLink);
    }
    return ptcl;
}

/* 8025DBB4-8025DC2C       .text doStartFrameProcess__14JPABaseEmitterFv */
bool JPABaseEmitter::doStartFrameProcess() {
    if (mTime.getFrame() >= mStartFrame)
        return true;

    if (!checkStatus(JPAEmtrStts_StopCalc)) {
        mTime.incFrame();
    }

    return false;
}

/* 8025DC2C-8025DCDC       .text doTerminationProcess__14JPABaseEmitterFv */
bool JPABaseEmitter::doTerminationProcess() {
    if (mMaxFrame == 0) {
        return false;
    } else if (mMaxFrame < 0) {
        mFlags |= JPAEmtrStts_EnableDeleteEmitter;
        return getParticleNumber() == 0;
    } else if (mTick.getFrame() >= mMaxFrame) {
        mFlags |= JPAEmtrStts_EnableDeleteEmitter;
        if (mFlags & JPAEmtrStts_Immortal)
            return false;
        return getParticleNumber() == 0;
    } else {
        return false;
    }
}

/* 8025DCDC-8025DD5C       .text calcEmitterGlobalPosition__14JPABaseEmitterFRQ29JGeometry8TVec3<f> */
void JPABaseEmitter::calcEmitterGlobalPosition(JGeometry::TVec3<float>& dst) {
    Mtx mtx;
    MTXScale(mtx, mGlobalDynamicsScale.x, mGlobalDynamicsScale.y, mGlobalDynamicsScale.z);
    MTXConcat(mGlobalRotation, mtx, mtx);
    mtx[0][3] = mGlobalTranslation.x;
    mtx[1][3] = mGlobalTranslation.y;
    mtx[2][3] = mGlobalTranslation.z;
    MTXMultVec(mtx, mEmitterTranslation, dst);
}

/* 8025DD5C-8025DDE8       .text calcgReRDirection__14JPABaseEmitterFv */
void JPABaseEmitter::calcgReRDirection() {
    // gReR = globalRotation emitterRotation
    Mtx mtx;
    JPAGetXYZRotateMtx((mEmitterRot.x << 14) / 0x5a, (mEmitterRot.y << 14) / 0x5a, (mEmitterRot.z << 14) / 0x5a, mtx);
    MTXConcat(mGlobalRotation, mtx, mtx);
    MTXMultVec(mtx, mEmitterDir, emtrInfo.mgReRDir);
}

/* 8025DDE8-8025DE2C       .text getPivotX__14JPABaseEmitterFv */
u8 JPABaseEmitter::getPivotX() {
    if (getEmitterDataBlockInfoPtr()->getExtraShape() != NULL)
        return getEmitterDataBlockInfoPtr()->getExtraShape()->getPivotX();
    else
        return 1;
}

/* 8025DE2C-8025DE70       .text getPivotY__14JPABaseEmitterFv */
u8 JPABaseEmitter::getPivotY() {
    if (getEmitterDataBlockInfoPtr()->getExtraShape() != NULL)
        return getEmitterDataBlockInfoPtr()->getExtraShape()->getPivotY();
    else
        return 1;
}
