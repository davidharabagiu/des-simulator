#include "simulatorframe.h"

namespace
{
const char* kFrameTitle{"DES Simulator"};
const char* kMenuTitle{"&Menu"};
const char* kAboutText{
    "DES Simulator is a visual simulator for the DES (Data Encryption Standard) algorithm written in C++14. "
    "It allows for visualising the steps of the algorithm in real time. It was built for learning purposes by "
    "David Bazatu as an assignment for an Applied Cryptography class. No licence is associated with this program, "
    "it belongs to the public domain."};
const char* kAboutTitle{"About"};
const wxSize kFrameSize{640, 480};
const long kFrameStyle{wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN};
} //namespace

SimulatorFrame::SimulatorFrame()
    : wxFrame{nullptr, wxID_ANY, kFrameTitle, wxDefaultPosition, kFrameSize, kFrameStyle}
{
}

void SimulatorFrame::Initialize()
{
    auto* menu = new wxMenu;
    menu->Append(wxID_ABOUT);
    menu->Append(wxID_EXIT);

    auto* menu_bar = new wxMenuBar;
    menu_bar->Append(menu, kMenuTitle);
    SetMenuBar(menu_bar);

    Bind(wxEVT_MENU, &SimulatorFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &SimulatorFrame::OnExit, this, wxID_EXIT);
}

void SimulatorFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(kAboutText, kAboutTitle, wxOK | wxICON_INFORMATION);
}

void SimulatorFrame::OnExit(wxCommandEvent &event)
{
    Close(false);
}
