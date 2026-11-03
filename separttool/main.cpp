//
//  main.cpp
//  separttool
//
//  Created by erd on 03.07.25.
//  Copyright © 2025 tihmstar. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <sys/stat.h>
#include <fcntl.h>


#include <libgeneral/macros.h>
#include <libgeneral/Mem.hpp>
#include <libgeneral/Utils.hpp>
#include "../include/img4tool/img4tool.hpp"
#include "../include/img4tool/separttool.hpp"

#define MIN(a,b) ((a) < (b) ? (a) : (b))

using namespace tihmstar;

static struct option longopts[] = {
    { "help",           no_argument,        NULL, 'h' },
    { NULL, 0, NULL, 0 }
};

void cmd_help(){
    printf(
           "Usage: separttool [OPTIONS] FILE\n"
           "Parses binary sep art\n\n"
           "  -h, --help\t\t\tprints usage information\n"
           "\n"
           );
}

MAINFUNCTION
int main_r(int argc, const char * argv[]) {
    printf("%s\n",tihmstar::img4tool::version());
    
    const char *lastArg = NULL;
    int optindex = 0;
    int opt = 0;

    tihmstar::Mem workingBuf;

    while ((opt = getopt_long(argc, (char* const *)argv, "h", longopts, &optindex)) >= 0) {
        switch (opt) {
            case 0: //long opts
            {
                std::string curopt = longopts[optindex].name;
                
//                if (curopt == "compression") {
//                    compressionType = optarg;
//                }else
                {
                    error("Unknown curopt '%s'",curopt.c_str());
                }
                break;
            }
                
            case 'h':
                cmd_help();
                return 0;
            default:
                cmd_help();
                return -1;
        }
    }
    
    if (argc-optind == 1) {
        argc -= optind;
        argv += optind;
        lastArg = argv[0];
    }else{
        cmd_help();
        return -2;
    }
    
    workingBuf = tihmstar::readFile(lastArg);
    
    //printing only
    img4tool::ASN1DERElement separt(workingBuf.data(),workingBuf.size(), true); workingBuf.disown();
    printf("SEP ART:\n");
    printf("\tFormatversion: %10d\n",tihmstar::separttool::getARTFormatversion(separt));
    printf("\tHardware cntr: %10llu\n",tihmstar::separttool::getSEPHardwareCounter(separt));
    {
        auto hash = tihmstar::separttool::getAPTicketHash(separt);
    printf("\tAPTicket Hash: ");
        for (int i=0; i<hash.size(); i++) {
            printf("%02x",hash.data()[i]);
        }
        printf("\n");
    }
    {
        uint32_t cntrsCnt = tihmstar::separttool::getStoredCountersCount(separt);
    printf("\tStoredCounters [%d]:\n",cntrsCnt);
        for (int i=0; i<cntrsCnt; i++) {
            auto cntr = tihmstar::separttool::getStoredCounterMemByIndex(separt, i);
            int64_t v = tihmstar::separttool::getStoredCounterByID(separt, cntr.first);
            printf("\t\t[%2d] : %15lld | ",cntr.first,v);
            for (int z=0; z<cntr.second.size(); z++) {
                printf("%02x",cntr.second.data()[z]);
            }
            printf("\n");
        }
    }
    {
        auto sig = tihmstar::separttool::getSEPSignature(separt);
    printf("\tSignature: ");
        for (int i=0; i<sig.size(); i++) {
            printf("%02x",sig.data()[i]);
        }
        printf("\n");
    }

    return 0;
}
