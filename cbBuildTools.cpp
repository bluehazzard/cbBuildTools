#include <sdk.h> // Code::Blocks SDK
#include <configurationpanel.h>
#include "cbBuildTools.h"

// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<cbBuildTools> reg(_T("cbBuildTools"));
}

const long cbBuildTools::ID_MENU_BUILD_TOOLS_VIEW               = wxNewId();
const long cbBuildTools::ID_MENU_SHOW_BUILD_TARGETS_PANEL       = wxNewId();



// events handling
BEGIN_EVENT_TABLE(cbBuildTools, cbPlugin)
    // add any events you want to handle here
    EVT_MENU(ID_MENU_SHOW_BUILD_TARGETS_PANEL, cbBuildTools::OnWindowMenu)
END_EVENT_TABLE()

// constructor
cbBuildTools::cbBuildTools()
{
    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(_T("cbBuildTools.zip")))
    {
        NotifyMissingFile(_T("cbBuildTools.zip"));
    }
}

// destructor
cbBuildTools::~cbBuildTools()
{
}

void cbBuildTools::OnAttach()
{
    // do whatever initialization you need for your plugin
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be TRUE...
    // You should check for it in other functions, because if it
    // is FALSE, it means that the application did *not* "load"
    // (see: does not need) this plugin...
    m_buildTargetsPanel = new BuildTargetsWindow(Manager::Get()->GetAppWindow());
    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
    evt.name = _T("Build Tools: Target Window");
    evt.title = _("Build Tools: Target window");
    evt.pWindow = m_buildTargetsPanel;
    evt.dockSide = CodeBlocksDockEvent::dsFloating;
    evt.desiredSize.Set(400, 300);
    evt.floatingSize.Set(400, 300);
    evt.minimumSize.Set(200, 150);
    Manager::Get()->ProcessEvent(evt);

        // EVT_PROJECT_ACTIVATE
    Manager::Get()->RegisterEventSink(cbEVT_PROJECT_ACTIVATE, new cbEventFunctor<cbBuildTools, CodeBlocksEvent>(this, &cbBuildTools::OnProjectActivatedEvent));

}

void cbBuildTools::OnRelease(bool appShutDown)
{
    // do de-initialization for your plugin
    // if appShutDown is true, the plugin is unloaded because Code::Blocks is being shut down,
    // which means you must not use any of the SDK Managers
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be FALSE...
}

void cbBuildTools::BuildMenu(wxMenuBar* menuBar)
{
    //The application is offering its menubar for your plugin,
    //to add any menu items you want...
    //Append any items you need in the menu...
    //NOTE: Be careful in here... The application's menubar is at your disposal.
    size_t i;
    int idx = menuBar->FindMenu(_("&View"));
    if (idx != wxNOT_FOUND)
    {
        wxMenu* menu = menuBar->GetMenu(idx);
        wxMenuItemList& items = menu->GetMenuItems();

        wxMenu* subMenu = new wxMenu(wxT(""));
        subMenu->AppendCheckItem(ID_MENU_SHOW_BUILD_TARGETS_PANEL, _("Build targets"), _("Toggle displaying the 'Build targets' panel"));

        // find the first separator and insert before it
        menu->AppendSubMenu(subMenu, _("Build tools"), _("Build tools views"));
    }
}

void cbBuildTools::BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    //Some library module is ready to display a pop-up menu.
    //Check the parameter \"type\" and see which module it is
    //and append any items you need in the menu...
    //TIP: for consistency, add a separator as the first item...
    NotImplemented(_T("cbBuildTools::BuildModuleMenu()"));
}

bool cbBuildTools::BuildToolBar(wxToolBar* toolBar)
{
    //The application is offering its toolbar for your plugin,
    //to add any toolbar items you want...
    //Append any items you need on the toolbar...
    NotImplemented(_T("cbBuildTools::BuildToolBar()"));

    // return true if you add toolbar items
    return false;
}

void cbBuildTools::OnWindowMenu(wxCommandEvent& event)
{
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = m_buildTargetsPanel;
    Manager::Get()->ProcessEvent(evt);
}

void cbBuildTools::OnProjectActivatedEvent(CodeBlocksEvent& event)
{
    cbProject* pCBProject = event.GetProject();
    m_buildTargetsPanel->ActivateProject(pCBProject);
}
