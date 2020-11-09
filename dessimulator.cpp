#include "dessimulator.h"
#include "simulatorframe.h"

bool DesSimulator::OnInit()
{
    auto frame = new SimulatorFrame;
    SetTopWindow(frame);
    frame->Initialize();
    frame->Show(true);
    return true;
}
