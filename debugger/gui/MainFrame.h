#ifndef RED_O_LATOR_MAINFRAME_H
#define RED_O_LATOR_MAINFRAME_H

#include <wx/aui/aui.h>
#include <string>
#include <vector>
#include "../data/InstructionView.h"
#include "../data/Parameter.h"
#include "../data/Preferences.h"
#include "CodeViewer.h"
#include "KernelPanel.h"
#include "MenuBar.h"
#include "ParametersTable.h"
#include "RegistersTable.h"
#include "Toolbar.h"
#include "debugger/exec/ExecutionContext.h"
#include "hex-view/MemoryView.h"
#include "model/reg/RegData.h"
#include "wx.h"

class EmulatorApp;

class MainFrame : public wxFrame {
   private:
    wxAuiManager auiManager;
    Preferences& preferences;

    MenuBar* menuBar;
    Toolbar* toolbar;
    CodeViewer* textEditor;
    KernelPanel* kernelPanel;
    ParametersTable* paramsPanel;
    MemoryView* memoryPanel;
    RegistersTable* registersPanel;

    wxString fileName;

    void onOpen(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onToggleAddressMargin(wxCommandEvent& event);
    void onRequestMemory(wxCommandEvent& event);

    void bindEvents();

    const EmulatorApp* app;

   public:
    MainFrame(const EmulatorApp* parent,
              const wxString& title,
              const wxSize& size,
              Preferences& preferences);

    void enableTool(int toolId, bool state);

    void selectKernel(int kernelIdx);
    void updateKernelList(const std::vector<std::string>& kernels);
    void enableKernelList(bool state);
    void enableWorkGroupList(bool state);
    void enableMemoryPanel(bool state);

    void setParameters(const ParametersCategory& category,
                       const std::vector<Parameter>& parameters);

    void setInstructions(const std::vector<InstructionView>& instructions);
    void setExecutionMarker(uint64_t address);
    void removeExecutionMarker();

    void onSetBreakpoint(uint64_t address) const;
    void onRemoveBreakpoint(uint64_t address) const;

    void setMemoryView(const std::vector<uint8_t>& memory, uint64_t address);

    void setExecutionContext(const ExecutionContext& context);

    void setRegisters(const RegData& data);
};

#endif  // RED_O_LATOR_MAINFRAME_H
