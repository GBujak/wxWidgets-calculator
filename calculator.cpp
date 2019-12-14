#include <wx/wx.h>
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <unordered_map>

#define operators {'+', '-', '/', '*'}

class frame_t : public wxFrame {
    wxTextCtrl* text_ctrl;
    std::unordered_map<char, wxButton*> buttons;
    bool operation_selected;
    bool delete_on_next;

    public:

    void do_operation() {
        delete_on_next = true;
        std::string operation = text_ctrl->GetValue().ToStdString();
        text_ctrl->Clear();

        char operation_operator = 0;
        std::stringstream str_left, str_right;
        double left = 0.0;
        double right = 0.0;

        std::stringstream* source = &str_left;
        for (char c : operation) {
            bool insert = true;
            for (char op : operators) if (c == op) {
                operation_operator = c;
                source = &str_right;
                insert = false;
            }
            if (insert) source->put(c);
        }
        left = std::stod(str_left.str());
        right = std::stod(str_right.str());

        double result = 0.0;
        switch (operation_operator) {
            case '+':
                result = left + right;
                break;
            case '-':
                result = left - right;
                break;
            case '*':
                result = left * right;
                break;
            case '/':
                result = left / right;
                break;
        }

        std::ostream{text_ctrl} << result;
    }

    std::function<void(wxEvent&)>
    button_clicked = [this] (wxEvent& event) mutable {
        if (delete_on_next) {
            text_ctrl->Clear();
            delete_on_next = false;
        }

        std::ostream text_stream{this->text_ctrl};
        char label = static_cast<wxButton*>
            (event.GetEventObject())->GetLabel()[0];
    
        if (label == '.') buttons['.']->Disable();
        if (label <= '9' && label >= '0') {
            if (operation_selected) {
                buttons['=']->Enable();
            } else for (char op : operators)
                buttons[op]->Enable();
        }   
        for (char op : operators) if (label == op) {
            buttons['.']->Enable();
            operation_selected = true;
            for (char op : operators) 
                buttons[op]->Disable();
        }
        if (label == '=') {
            operation_selected = false;
            buttons['=']->Disable();
            buttons['.']->Enable();
            do_operation();
            return;
        }
        text_stream << label;
        event.Skip();
    };

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
            buttons[label] = button;
            button_sizer->Add(button, 1);
            Bind(wxEVT_BUTTON, this->button_clicked, button->GetId());
        }
        main_sizer->Add(button_sizer);

        SetSizerAndFit(main_sizer);
    }

    frame_t() {
        Create(nullptr, wxID_ANY, "kalkulator");
        init_widgets();
        operation_selected = false;
        buttons['=']->Disable();
        for (char op : operators)
            buttons[op]->Disable();
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