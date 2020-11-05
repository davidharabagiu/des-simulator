#ifndef DES_SIMULATOR_SIMULATORFRAME_H
#define DES_SIMULATOR_SIMULATORFRAME_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

class SimulatorFrame : public wxFrame
{
public:
    SimulatorFrame();
    void Initialize();

private:
    void OnAbout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
};

#endif //DES_SIMULATOR_SIMULATORFRAME_H
