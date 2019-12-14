#include <wx/wx.h>
#include <iostream>
#include <string>
#include <functional>

class frame_t : public wxFrame {
    wxTextCtrl* text_ctrl;
    double current_value;

    public:
    void button_clicked(wxEvent& event) {

    }
    void init_widgets() {
        auto main_sizer = new wxBoxSizer(wxVERTICAL);

        text_ctrl = new wxTextCtrl{this, wxNewId()};
        text_ctrl->SetFont(wxFont{18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL});
        text_ctrl->Disable();
        main_sizer->Add(text_ctrl, wxSizerFlags(0).Expand());

        auto button_sizer = new wxGridSizer(4, {5, 5});
        std::string labels = "789/456*123-0.=+";
        for (const char label : labels) {
            auto button = new wxButton(this, wxNewId(), label);
            button_sizer->Add(button, 1);
            // Bind(wxEVT_BUTTON, this->*button_clicked, button->GetId());
        }
        main_sizer->Add(button_sizer);

        SetSizerAndFit(main_sizer);
    }
    frame_t() {
        current_value = 0;
        Create(nullptr, wxID_ANY, "kalkulator");
        init_widgets();
    }
};

class app_t : public wxApp {
    public:
    virtual bool OnInit() {
        auto frame = new frame_t;
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(app_t);