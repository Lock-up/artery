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

    // Open CAM logfile
    const std::string filename = std::to_string(mIdentity->application).append("_cams.log");
    mCamLogfile.open(filename.c_str(), std::ios::app);
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

        // vanetza::facilities::print_indented(EV_INFO, *cam, " ");
        // vanetza::facilities::print_indented(mCamLogfile, *cam, " ");
        mCamLogfile << vanetza::facilities::format_json(*cam) << "\n";
    }
}

} // namespace artery
