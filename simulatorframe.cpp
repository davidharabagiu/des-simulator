#include "simulatorframe.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdint>

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
const wxSize kFrameSize{1100, 500};
const long kFrameStyle{wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN};

const wxWindowID kIdKeyInput = ::wxNewId();
const wxWindowID kIdPlainText = ::wxNewId();

std::string ToHexString(const std::vector<uint8_t>& bytes)
{
    std::ostringstream ss;
    for (auto b : bytes)
    {
        ss << std::hex << std::setfill('0') << std::uppercase << std::setw(2) << unsigned{b} << ' ';
    }
    return ss.str();
}

std::vector<bool> ToBinary(const std::vector<uint8_t>& bytes)
{
    std::vector<bool> bits;
    bits.reserve(bytes.size() * 8);
    for (auto b : bytes)
    {
        for (int i = 0; i != 8; ++i)
        {
            bits.push_back(b & 0x80u);
            b <<= 1u;
        }
    }
    return bits;
}

std::vector<uint8_t> FromBinary(const std::vector<bool>& bits)
{
    std::vector<uint8_t> bytes;
    bytes.reserve(bits.size() / 8);
    for (int i = 0; i < bits.size() / 8 * 8; i += 8)
    {
        uint8_t byte = 0;
        uint8_t mask = 0x80u;
        for (int j = 0; j != 8; ++j)
        {
            if (bits[i+j])
            {
                byte |= mask;
            }
            mask >>= 1u;
        }
        bytes.push_back(byte);
    }
    return bytes;
}

} //namespace

BEGIN_EVENT_TABLE(SimulatorFrame, wxFrame)
    EVT_TEXT(kIdKeyInput, SimulatorFrame::OnKeyChanged)
    EVT_TEXT(kIdPlainText, SimulatorFrame::OnPlainTextChanged)
END_EVENT_TABLE()

SimulatorFrame::SimulatorFrame()
    : wxFrame{nullptr, wxID_ANY, kFrameTitle, wxDefaultPosition, kFrameSize}
{
}

void SimulatorFrame::Initialize()
{
    auto menu = new wxMenu;
    menu->Append(wxID_ABOUT);
    menu->Append(wxID_EXIT);

    auto menu_bar = new wxMenuBar;
    menu_bar->Append(menu, kMenuTitle);
    SetMenuBar(menu_bar);

    Bind(wxEVT_MENU, &SimulatorFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &SimulatorFrame::OnExit, this, wxID_EXIT);

    Centre();

    auto sizer_key_input = new wxBoxSizer{wxHORIZONTAL};
    sizer_key_input->Add(new wxStaticText {this, -1, _T("Key: ")}, 0, wxALIGN_CENTER_VERTICAL);
    text_input_key_ = new wxTextCtrl{ this, kIdKeyInput };
    text_input_key_->SetMaxLength(8);
    sizer_key_input->Add(text_input_key_, 1, wxEXPAND);
    text_actual_key_hex_ = new wxTextCtrl{this, -1, wxEmptyString};
    text_actual_key_hex_->SetEditable(false);
    sizer_key_input->Add(text_actual_key_hex_, 2, wxEXPAND | wxLEFT, 10);

    auto sizer_effective_key = new wxBoxSizer{wxHORIZONTAL};
    sizer_effective_key->Add(new wxStaticText{this, -1, _T("Effective key: ")}, 0, wxALIGN_CENTER_VERTICAL);
    text_effective_key_ = new wxTextCtrl{this, -1, wxEmptyString};
    text_effective_key_->SetEditable(false);
    sizer_effective_key->Add(text_effective_key_, 1, wxEXPAND);

    auto sizer_round_keys = new wxBoxSizer{wxVERTICAL};
    sizer_round_keys->Add(new wxStaticText{this, -1, _T("Round keys:")}, 1, wxEXPAND);
    text_round_keys_ = new wxTextCtrl{this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE};
    text_round_keys_->SetEditable(false);
    sizer_round_keys->Add(text_round_keys_, 12, wxEXPAND);

    auto sizer_key_config = new wxBoxSizer{wxVERTICAL};
    sizer_key_config->Add(sizer_key_input, 1, wxEXPAND, 10);
    sizer_key_config->Add(sizer_effective_key, 1, wxEXPAND | wxTOP, 10);
    sizer_key_config->Add(sizer_round_keys, 100, wxEXPAND | wxTOP, 10);

    auto sizer_text_input = new wxBoxSizer{wxHORIZONTAL};
    sizer_text_input->Add(new wxStaticText {this, -1, _T("Plain text: ")}, 0, wxALIGN_CENTER_VERTICAL);
    text_input_plain_ = new wxTextCtrl{ this, kIdPlainText };
    text_input_plain_->SetMaxLength(8);
    sizer_text_input->Add(text_input_plain_, 1, wxEXPAND);

    auto sizer_text_permuted = new wxBoxSizer{wxHORIZONTAL};
    sizer_text_permuted->Add(new wxStaticText {this, -1, _T("Permuted text: ")}, 0, wxALIGN_CENTER_VERTICAL);
    text_permuted_plain_ = new wxTextCtrl{ this, -1 };
    text_permuted_plain_->SetEditable(false);
    sizer_text_permuted->Add(text_permuted_plain_, 1, wxEXPAND);

    auto sizer_encryption_rounds = new wxBoxSizer{wxVERTICAL};
    sizer_encryption_rounds->Add(sizer_text_permuted, 1, wxEXPAND | wxBOTTOM, 10);
    sizer_encryption_rounds->Add(new wxStaticText{this, -1, _T("Cipher text after each encryption round:")}, 1, wxEXPAND);
    text_encryption_rounds_ = new wxTextCtrl{this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE};
    text_encryption_rounds_->SetEditable(false);
    sizer_encryption_rounds->Add(text_encryption_rounds_, 12, wxEXPAND);

    auto sizer_encrypted_text_permuted = new wxBoxSizer{wxHORIZONTAL};
    sizer_encrypted_text_permuted->Add(new wxStaticText {this, -1, _T("Permuted encrypted text: ")}, 0, wxALIGN_CENTER_VERTICAL);
    text_encrypted_permuted_plain_ = new wxTextCtrl{ this, -1 };
    text_encrypted_permuted_plain_->SetEditable(false);
    sizer_encrypted_text_permuted->Add(text_encrypted_permuted_plain_, 1, wxEXPAND);

    auto sizer_decryption_rounds = new wxBoxSizer{wxVERTICAL};
    sizer_decryption_rounds->Add(sizer_encrypted_text_permuted, 1, wxEXPAND | wxBOTTOM, 10);
    sizer_decryption_rounds->Add(new wxStaticText{this, -1, _T("Cipher text after each decryption round:")}, 1, wxEXPAND);
    text_decryption_rounds_ = new wxTextCtrl{this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE};
    text_decryption_rounds_->SetEditable(false);
    sizer_decryption_rounds->Add(text_decryption_rounds_, 12, wxEXPAND);

    auto sizer_algo_steps = new wxBoxSizer{wxHORIZONTAL};
    sizer_algo_steps->Add(sizer_encryption_rounds, 1, wxEXPAND);
    sizer_algo_steps->Add(sizer_decryption_rounds, 1, wxEXPAND | wxLEFT, 10);

    auto sizer_text_config = new wxBoxSizer{wxVERTICAL};
    sizer_text_config->Add(sizer_text_input, 1, wxEXPAND);
    sizer_text_config->Add(sizer_algo_steps, 100, wxEXPAND | wxTOP, 10);

    auto sizer_main = new wxBoxSizer{wxHORIZONTAL};
    sizer_main->Add(sizer_key_config, 1, wxEXPAND | wxALL, 10);
    sizer_main->Add(sizer_text_config, 3, wxEXPAND | wxALL, 10);
    SetSizer(sizer_main);

    UpdateCipher();
}

void SimulatorFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(kAboutText, kAboutTitle, wxOK | wxICON_INFORMATION);
}

void SimulatorFrame::OnExit(wxCommandEvent &event)
{
    Close(false);
}

void SimulatorFrame::OnKeyChanged(wxCommandEvent &event)
{
    UpdateCipher();
}

void SimulatorFrame::UpdateCipher()
{
    std::string key_str{text_input_key_->GetValue().mb_str()};
    std::vector<uint8_t> key_bytes(8, 0);
    std::copy_n(key_str.cbegin(), std::min(8lu, key_str.size()), key_bytes.begin());
    text_actual_key_hex_->SetValue(wxString{ ToHexString(key_bytes)});
    cipher_.set_key(ToBinary(key_bytes));

    std::string text_str{text_input_plain_->GetValue().mb_str()};
    std::vector<uint8_t> text_bytes(8, 0);
    std::copy_n(text_str.cbegin(), std::min(8lu, text_str.size()), text_bytes.begin());
    cipher_.set_text(ToBinary(text_bytes));

    auto effective_key = FromBinary(cipher_.effective_key());
    text_effective_key_->SetValue(wxString{ ToHexString(effective_key)});

    wxString round_keys_text;
    {
        int idx = 1;
        for (const auto& rk : cipher_.round_keys())
        {
            round_keys_text += wxString::Format(wxT("Round %i: "), idx) + wxString{ ToHexString(FromBinary(rk)) } + "\n";
            ++idx;
        }
    }
    text_round_keys_->SetValue(round_keys_text);

    auto permuted_text = FromBinary(cipher_.permuted_text());
    text_permuted_plain_->SetValue(wxString{ ToHexString(permuted_text)});

    wxString encryption_rounds_text;
    {
        int idx = 1;
        for (const auto& rnd : cipher_.encryption_rounds())
        {
            encryption_rounds_text += wxString::Format(wxT("Round %i: "), idx) + wxString{ ToHexString(FromBinary(rnd)) } + "\n";
            ++idx;
        }
        encryption_rounds_text += wxString{"Cipher text: "} + wxString{ToHexString(FromBinary(cipher_.cipher_text()))};
    }
    text_encryption_rounds_->SetValue(encryption_rounds_text);

    auto permuted_cipher_text = FromBinary(cipher_.permuted_cipher_text());
    text_encrypted_permuted_plain_->SetValue(wxString{ ToHexString(permuted_cipher_text)});

    wxString decryption_rounds_text;
    {
        int idx = 1;
        for (const auto& rnd : cipher_.decryption_rounds())
        {
            decryption_rounds_text += wxString::Format(wxT("Round %i: "), idx) + wxString{ ToHexString(FromBinary(rnd)) } + "\n";
            ++idx;
        }
        auto decrypted_bytes = FromBinary(cipher_.decrypted_text());
        std::string decrypted_text{ decrypted_bytes.cbegin(), decrypted_bytes.cend() };
        decrypted_text.erase(std::find(decrypted_text.begin(), decrypted_text.end(), '\0'), decrypted_text.end());
        decryption_rounds_text += wxString{"Decrypted text: "} + decrypted_text;
    }
    text_decryption_rounds_->SetValue(decryption_rounds_text);
}

void SimulatorFrame::OnPlainTextChanged(wxCommandEvent &event)
{
    UpdateCipher();
}
