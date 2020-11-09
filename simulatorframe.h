#ifndef DES_SIMULATOR_SIMULATORFRAME_H
#define DES_SIMULATOR_SIMULATORFRAME_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "interactivedescipher.h"

class SimulatorFrame : public wxFrame
{
public:
    SimulatorFrame();
    void Initialize();

private:
    void OnAbout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnKeyChanged(wxCommandEvent& event);
    void OnPlainTextChanged(wxCommandEvent& event);

    void UpdateCipher();

private:
    wxTextCtrl* text_actual_key_hex_;
    wxTextCtrl* text_input_key_;
    wxTextCtrl* text_effective_key_;
    wxTextCtrl* text_round_keys_;
    wxTextCtrl* text_input_plain_;
    wxTextCtrl* text_permuted_plain_;
    wxTextCtrl* text_encryption_rounds_;
    wxTextCtrl* text_encrypted_permuted_plain_;
    wxTextCtrl* text_decryption_rounds_;

    InteractiveDESCipher cipher_;

    DECLARE_EVENT_TABLE()
};

#endif //DES_SIMULATOR_SIMULATORFRAME_H
