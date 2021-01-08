/*
* Artery V2X Simulation Framework
* Copyright 2014-2020 Raphael Riebl et al.
* Licensed under GPLv2, see COPYING file for detailed license and warranty terms.
*/

#include "artery/application/CaObject.h"
#include "artery/application/LocalDynamicMap.h"
#include "artery/application/RsuCaLogService.h"
#include "artery/application/Asn1PacketVisitor.h"
#include "artery/application/MultiChannelPolicy.h"
#include "artery/utility/Geometry.h"
#include "artery/utility/Identity.h"
#include <boost/lexical_cast.hpp>
#include <omnetpp/cexception.h>
#include <omnetpp/cxmlelement.h>
#include <vanetza/btp/ports.hpp>
#include <vanetza/facilities/cam_functions.hpp>
#include <cmath>

namespace artery
{

using namespace omnetpp;

static const simsignal_t scSignalCamLogged = cComponent::registerSignal("CamLogged");

Define_Module(RsuCaLogService)

void RsuCaLogService::initialize()
{
    ItsG5BaseService::initialize();
    mIdentity = &getFacilities().get_const<Identity>();
    mTimer = &getFacilities().get_const<Timer>();

    // Open CAM logfile
    char buffer [15];
    time_t tt = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now() );
    struct tm * timeinfo = localtime (&tt);
    strftime (buffer,15,"%y%m%d_%H%M%S_", timeinfo);  

    const std::string filename = buffer + std::to_string(mIdentity->application).append("_cams.json");
    mCamLogfile.open(filename.c_str(), std::ios::trunc);
    mCamLogfile << "[\n]";
    mCamLogfile.flush();
}

void RsuCaLogService::finish()
{
    mCamLogfile.close();
	ItsG5BaseService::finish();
}

void RsuCaLogService::indicate(const vanetza::btp::DataIndication& ind, std::unique_ptr<vanetza::UpPacket> packet)
{
    Enter_Method("indicate");

    Asn1PacketVisitor<vanetza::asn1::Cam> visitor;
    const vanetza::asn1::Cam* cam = boost::apply_visitor(visitor, *packet);
    if (cam && cam->validate()) {
        CaObject obj = visitor.shared_wrapper;
        emit(scSignalCamLogged, &obj);

        const uint64_t etsiBaseTimeStampInMs = boost::posix_time::to_time_t(vanetza::Clock::epoch()) * 1000;
        const uint64_t etsiTimeStampInMs = countTaiMilliseconds(mTimer->getCurrentTime()); 
        const uint64_t simruntimeInMs = omnetpp::simTime().inUnit(SimTimeUnit::SIMTIME_MS);

        long pos = mCamLogfile.tellp();
        if(pos < 4) {
            mCamLogfile.seekp (pos-1);
            mCamLogfile
                << "{\"type\": \"cam\", \"ts_unix\": " 
                << etsiBaseTimeStampInMs + etsiTimeStampInMs
                << ", \"ts_etsi\": "
                << etsiTimeStampInMs
                << ", \"simRuntime\": "
                << simruntimeInMs
                << ", \"data\": "
                << vanetza::facilities::format_json(*cam) << "}\n]";
        }
        else {
            mCamLogfile.seekp (pos-2);
            mCamLogfile
                << ",\n"
                << "{\"type\": \"cam\", \"ts_unix\": " 
                << etsiBaseTimeStampInMs + etsiTimeStampInMs
                << ", \"ts_etsi\": "
                << etsiTimeStampInMs
                << ", \"simRuntime\": "
                << simruntimeInMs
                << ", \"data\": "
                << vanetza::facilities::format_json(*cam) << "}\n]";
        }
        mCamLogfile.flush();
    }
}

} // namespace artery
