#ifndef DES_SIMULATOR_DESSIMULATOR_H
#define DES_SIMULATOR_DESSIMULATOR_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include <memory>

class DesSimulator : public wxApp
{
public:
    bool OnInit() override;
};

#endif //DES_SIMULATOR_DESSIMULATOR_H
