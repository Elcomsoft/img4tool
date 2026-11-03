//
//  separttool.cpp
//  img4tool
//
//  Created by erd on 03.07.25.
//  Copyright © 2025 tihmstar. All rights reserved.
//

#include "../include/img4tool/separttool.hpp"

#include <libgeneral/macros.h>

using namespace tihmstar;
using namespace tihmstar::img4tool;


int separttool::getARTFormatversion(const ASN1DERElement &art){
    auto formatvers = art[0];
    return (int)formatvers.getIntegerValue();
}

uint64_t separttool::getSEPHardwareCounter(const img4tool::ASN1DERElement &art){
    auto seq1 = art[1];
    auto hwctr = seq1[0];
    return hwctr.getIntegerValue();
}

img4tool::ASN1DERElement separttool::setSEPHardwareCounter(const img4tool::ASN1DERElement &art, uint64_t ctr){
    auto seq1old = art[1];
    ASN1DERElement hwctr = ASN1DERElement::makeASN1Integer(ctr);
    ASN1DERElement newseq1(seq1old.tag(), NULL, 0);
    newseq1 += hwctr;
    {
        int i=-1;
        for (auto e : seq1old) {
            if (++i == 0) continue;
            newseq1 += e;
        }
    }
    ASN1DERElement newart(art.tag(), NULL, 0);
    newart+=art[0];
    newart+=newseq1;
    newart+=art[2];
 
    return newart;
}

tihmstar::Mem separttool::getAPTicketHash(const img4tool::ASN1DERElement &art){
    auto seq1 = art[1];
    auto hash = seq1[1];
    return {hash.payload(),hash.payloadSize()};
}

img4tool::ASN1DERElement separttool::setAPTicketHash(const img4tool::ASN1DERElement &art, const void *buf, size_t bufSize){
    auto seq1old = art[1];
    ASN1DERElement hash{{
        .tagNumber = img4tool::ASN1DERElement::TagOCTET,
        .isConstructed = img4tool::ASN1DERElement::Primitive,
        .tagClass = img4tool::ASN1DERElement::Universal,
    }};
    hash.appendPayload(buf, bufSize);
    ASN1DERElement newseq1(seq1old.tag(), NULL, 0);
    {
        int i=-1;
        for (auto e : seq1old) {
            if (++i == 1){
                newseq1 += hash;
                continue;
            }
            newseq1 += e;
        }
    }
    ASN1DERElement newart(art.tag(), NULL, 0);
    newart+=art[0];
    newart+=newseq1;
    newart+=art[2];
 
    return newart;
}

uint32_t separttool::getStoredCountersCount(const img4tool::ASN1DERElement &art){
    auto seq1 = art[1];
    auto storedctrs = seq1[4];
    return (uint32_t)storedctrs.numElements();
}

std::pair<uint8_t,tihmstar::Mem> separttool::getStoredCounterMemByIndex(const img4tool::ASN1DERElement &art, int cntIdx){
    auto seq1 = art[1];
    auto storedctrs = seq1[4];
    auto tgt = storedctrs[cntIdx];
    
    uint8_t id = tgt.tag().tagNumber;
    tihmstar::Mem val{(const void*)tgt.payload(),tgt.payloadSize()};
    return {id,val};
}

std::pair<uint8_t,int64_t> separttool::getStoredCounterByIndex(const img4tool::ASN1DERElement &art, int cntIdx){
    auto seq1 = art[1];
    auto storedctrs = seq1[4];
    auto tgt = storedctrs[cntIdx];
    
    uint8_t id = tgt.tag().tagNumber;
    tihmstar::Mem val{(const void*)tgt.payload(),tgt.payloadSize()};
    return {id,tgt.getIntegerValue()};
}

tihmstar::Mem separttool::getStoredCounterMemByID(const img4tool::ASN1DERElement &art, uint8_t cntID){
    auto seq1 = art[1];
    auto storedctrs = seq1[4];

    for (auto tgt : storedctrs){
        uint8_t id = tgt.tag().tagNumber;
        if (id == cntID){
            return {(const void*)tgt.payload(),tgt.payloadSize()};
        }
    }
    reterror("Failed to found counter with id %d",cntID);
}

int64_t separttool::getStoredCounterByID(const img4tool::ASN1DERElement &art, uint8_t cntID){
    auto seq1 = art[1];
    auto storedctrs = seq1[4];
    
    for (auto tgt : storedctrs){
        uint8_t id = tgt.tag().tagNumber;
        if (id == cntID){
            return tgt.getIntegerValue();
        }
    }
    reterror("Failed to found counter with id %d",cntID);
}

img4tool::ASN1DERElement separttool::setStoredCounterByID(img4tool::ASN1DERElement &art, uint8_t cntID, int64_t ctr){
    auto vers = art[0];
    auto seq1old = art[1];
    auto sig = art[2];
    
    auto setold = seq1old[4];

    ASN1DERElement swctrs{{
        .tagNumber = img4tool::ASN1DERElement::TagSET,
        .isConstructed = img4tool::ASN1DERElement::Constructed,
        .tagClass = img4tool::ASN1DERElement::Universal,
    }};
    
    ASN1DERElement injectCtr = ASN1DERElement::makeASN1Integer(ctr);
    *((uint8_t*)injectCtr.buf()) = 0xC0 | cntID;
    
    {
        bool didInject = false;
        for (auto octr : setold) {
            assure(octr.tag().tagClass == ASN1DERElement::Private);
            if (octr.tag().tagNumber >= cntID) {
                if (!didInject){
                    swctrs+=injectCtr;
                    didInject = true;
                }
                if (octr.tag().tagNumber == cntID) continue; //skip orig
            }
            swctrs+=octr;
        }
    }
    
    ASN1DERElement newseq1(seq1old.tag(), NULL, 0);
    {
        int i=-1;
        for (auto e : seq1old) {
            if (++i == 4){
                newseq1 += swctrs;
                continue;
            }
            newseq1 += e;
        }
    }
    
    ASN1DERElement newart(art.tag(), NULL, 0);
    newart+=art[0];
    newart+=newseq1;
    newart+=art[2];

    return newart;
}

tihmstar::Mem separttool::getSEPSignature(const img4tool::ASN1DERElement &art){
    auto sig = art[2];
    return {sig.payload(),sig.payloadSize()};
}

img4tool::ASN1DERElement separttool::setSEPSignature(const img4tool::ASN1DERElement &art, const void *buf, size_t bufSize){
    ASN1DERElement sig{{
        .tagNumber = img4tool::ASN1DERElement::TagOCTET,
        .isConstructed = img4tool::ASN1DERElement::Primitive,
        .tagClass = img4tool::ASN1DERElement::Universal,
    }};
    sig.appendPayload(buf, bufSize);
    ASN1DERElement newart(art.tag(), NULL, 0);
    newart+=art[0];
    newart+=art[1];
    newart+=sig;
 
    return newart;
}
