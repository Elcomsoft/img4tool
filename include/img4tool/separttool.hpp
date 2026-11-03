//
//  separttool.hpp
//  img4tool
//
//  Created by erd on 03.07.25.
//  Copyright © 2025 tihmstar. All rights reserved.
//

#ifndef separttool_hpp
#define separttool_hpp

#include <img4tool/ASN1DERElement.hpp>

#include <libgeneral/Mem.hpp>

#include <stdint.h>

namespace tihmstar {
namespace separttool {

int getARTFormatversion(const img4tool::ASN1DERElement &art);
uint64_t getSEPHardwareCounter(const img4tool::ASN1DERElement &art);
img4tool::ASN1DERElement setSEPHardwareCounter(const img4tool::ASN1DERElement &art, uint64_t ctr);

tihmstar::Mem getAPTicketHash(const img4tool::ASN1DERElement &art);
img4tool::ASN1DERElement setAPTicketHash(const img4tool::ASN1DERElement &art, const void *buf, size_t bufSize);

uint32_t getStoredCountersCount(const img4tool::ASN1DERElement &art);
std::pair<uint8_t,tihmstar::Mem> getStoredCounterMemByIndex(const img4tool::ASN1DERElement &art, int cntIdx);
std::pair<uint8_t,int64_t> getStoredCounterByIndex(const img4tool::ASN1DERElement &art, int cntIdx);
tihmstar::Mem getStoredCounterMemByID(const img4tool::ASN1DERElement &art, uint8_t cntID);
int64_t getStoredCounterByID(const img4tool::ASN1DERElement &art, uint8_t cntID);
img4tool::ASN1DERElement setStoredCounterByID(img4tool::ASN1DERElement &art, uint8_t cntID, int64_t ctr);

tihmstar::Mem getSEPSignature(const img4tool::ASN1DERElement &art);
img4tool::ASN1DERElement setSEPSignature(const img4tool::ASN1DERElement &art, const void *buf, size_t bufSize);

}
}
#endif /* separttool_hpp */
