#include "buildtargetswindow.h"

#include <sdk.h> // Code::Blocks SDK
#include <wx/sizer.h>
#include <wx/menu.h>

const long BuildTargetsWindow::ID_TREE_CTRL               = wxNewId();

const long BuildTargetsWindow::ID_MENU_REBUILD            = wxNewId();
const long BuildTargetsWindow::ID_MENU_CLEAN              = wxNewId();
const long BuildTargetsWindow::ID_MENU_BUILD              = wxNewId();
const long BuildTargetsWindow::ID_MENU_RUN                = wxNewId();


BEGIN_EVENT_TABLE(BuildTargetsWindow,wxPanel)
    EVT_TREE_ITEM_ACTIVATED(ID_TREE_CTRL, BuildTargetsWindow::OnItemActivated )
    EVT_TREE_ITEM_MENU(ID_TREE_CTRL, BuildTargetsWindow::OnItemContext )
    EVT_MENU(ID_MENU_REBUILD, BuildTargetsWindow::OnItemContextMenu )
    EVT_MENU(ID_MENU_CLEAN,   BuildTargetsWindow::OnItemContextMenu )
    EVT_MENU(ID_MENU_BUILD,   BuildTargetsWindow::OnItemContextMenu )
    EVT_MENU(ID_MENU_RUN,     BuildTargetsWindow::OnItemContextMenu )
END_EVENT_TABLE()



BuildTargetsWindow::BuildTargetsWindow(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL/* | wxCLIP_CHILDREN*/, wxT("buildTargets"))
{
    wxBoxSizer* bs = new wxBoxSizer(wxVERTICAL);
    m_treeCtrl = new wxTreeCtrl(this, ID_TREE_CTRL);
    bs->Add(m_treeCtrl, 1, wxEXPAND | wxALL);
    SetAutoLayout(true);
    SetSizer(bs);
}

BuildTargetsWindow::~BuildTargetsWindow()
{
    //dtor
    delete m_treeCtrl;
}

void BuildTargetsWindow::ClearView()
{
     m_treeCtrl->DeleteAllItems();
}


void BuildTargetsWindow::ActivateProject(cbProject* project)
{
    m_treeCtrl->DeleteAllItems();

    m_cur_project = project;

    wxString prj_name = project->GetTitle();
    wxTreeItemId root = m_treeCtrl->AddRoot(prj_name);

    unsigned int target_count = project->GetBuildTargetsCount();
    for(unsigned int i = 0; i < target_count; i++)
    {
        ProjectBuildTarget* target = project->GetBuildTarget(i);
        m_treeCtrl->AppendItem(root, target->GetTitle() );
    }

    m_treeCtrl->ExpandAll();
}

void BuildTargetsWindow::OnItemActivated(wxTreeEvent& event)
{
    wxString target_name = m_treeCtrl->GetItemText(event.GetItem());
    ProjectBuildTarget* target = m_cur_project->GetBuildTarget(target_name);
    if(target == nullptr)
    {
        cbMessageBox(_("could not find Target: ") + target_name, _("Error"));
        return;
    }

    // What if more then one compiler plugin are installed?
    cbCompilerPlugin* compiler = nullptr;
    const std::vector<cbCompilerPlugin*> &compilers = Manager::Get()->GetPluginManager()->GetCompilerPlugins();
    if (!compilers.empty())
    {
        compiler = compilers.front();
        compiler->Build(target);
    }
    else
    {
        cbMessageBox(_("could not find compiler"), _("Error"));
        return;
    }
}

void BuildTargetsWindow::OnItemContext(wxTreeEvent& event)
{
    wxMenu menu;

    wxString target_name = m_treeCtrl->GetItemText(event.GetItem());
    if(target_name == m_cur_project->GetTitle())
    {
        // We clicked on the project
        void *data = reinterpret_cast<void *>(m_cur_project);
        menu.SetClientData(data);

    }
    else
    {
        ProjectBuildTarget* target = m_cur_project->GetBuildTarget(target_name);
        if(target == nullptr)
        {
            return;
        }
        void *data = reinterpret_cast<void *>(target);
        menu.SetClientData(data);
        menu.Append(ID_MENU_BUILD, _("Build"));
        menu.Append(ID_MENU_CLEAN, _("Clean"));
        menu.Append(ID_MENU_REBUILD, _("Rebuild"));
        //menu.Append(ID_MENU_RUN, _("Run"));
        PopupMenu(&menu);
    }
}

void BuildTargetsWindow::OnItemContextMenu(wxCommandEvent& event)
{
    cbCompilerPlugin* compiler = nullptr;
    const std::vector<cbCompilerPlugin*> &compilers = Manager::Get()->GetPluginManager()->GetCompilerPlugins();
    if (compilers.empty())
    {
        cbMessageBox(_("could not find compiler"), _("Error"));
        return;
    }

    compiler = compilers.front();

    // The context menu is hit on a target
    ProjectBuildTarget* target = reinterpret_cast<ProjectBuildTarget*>(static_cast<wxMenu *>(event.GetEventObject())->GetClientData());
    if(target == nullptr)
    {
        cbMessageBox(_("could not find Target in context menu: ") , _("Error: context menu"));
        return;
    }

    if(event.GetId() == ID_MENU_REBUILD)
    {
        compiler->Rebuild(target);
    }
    else if(event.GetId() == ID_MENU_CLEAN)
    {
        compiler->Clean(target);
    }
    else if(event.GetId() == ID_MENU_BUILD)
    {
        compiler->Build(target);
    }
    else if(event.GetId() == ID_MENU_RUN)
    {
        compiler->Run(target);
    }

}
