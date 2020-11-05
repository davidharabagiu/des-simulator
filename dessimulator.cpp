#include "dessimulator.h"
#include "simulatorframe.h"

bool DesSimulator::OnInit()
{
    SetTopWindow(new SimulatorFrame);
    GetTopWindow()->Show(true);
    return true;
}
