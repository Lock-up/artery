/*
 * Artery V2X Simulation Framework
 * Copyright 2014-2020 Raphael Riebl et al.
 * Licensed under GPLv2, see COPYING file for detailed license and warranty terms.
 */

#ifndef ARTERY_RSUCALOGSERVICE_H_
#define ARTERY_RSUCALOGSERVICE_H_

#include "artery/application/ItsG5BaseService.h"
#include "artery/utility/Channel.h"
#include <vanetza/asn1/cam.hpp>
#include <omnetpp/simtime.h>
#include <boost/optional/optional.hpp>
#include <list>

#include <fstream>
#include <iostream>


namespace artery
{

// class GeoPosition;
class Identity;
// class LocalDynamicMap;
// class NetworkInterfaceTable;
class Timer;

class RsuCaLogService : public ItsG5BaseService
{
    public:
        void initialize() override;
        void indicate(const vanetza::btp::DataIndication&, std::unique_ptr<vanetza::UpPacket>) override;
        void finish() override;

    private:
        const Identity* mIdentity = nullptr;
        const Timer* mTimer = nullptr;
        std::ofstream mCamLogfile;
};

} // namespace artery

#endif /* ARTERY_RSUCALOGSERVICE_H_ */
