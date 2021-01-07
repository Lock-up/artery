//
// Artery V2X Simulation Framework
// Licensed under GPLv2, see COPYING file for detailed license and warranty terms.
//

package artery.application;

simple RsuCaLogService like ItsG5Service
{
    parameters:
        @signal[CamReceived](type=CaObject);
        @signal[CamLogged](type=CaObject);

        @statistic[reception](source=CamLogged;record=vector(camStationId)?,vector(camGenerationDeltaTime)?);
        @statistic[reception](source=CamReceived;record=vector(camStationId)?,vector(camGenerationDeltaTime)?);
}
