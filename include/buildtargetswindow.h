#ifndef BUILDTARGETSWINDOW_H
#define BUILDTARGETSWINDOW_H

#include <wx/panel.h>
#include <wx/treectrl.h>
#include <sdk.h>
#include <cbproject.h>


class BuildTargetsWindow : public wxPanel
{
    public:
        BuildTargetsWindow(wxWindow* parent);
        virtual ~BuildTargetsWindow();

        void ActivateProject(cbProject* project);
        void ClearView();

    protected:

    private:

        wxTreeCtrl* m_treeCtrl;

        cbProject* m_cur_project;

        void OnItemActivated(wxTreeEvent& event);
        void OnItemContext(wxTreeEvent& event);
        void OnItemContextMenu(wxCommandEvent& event);

        static const long ID_TREE_CTRL;
        static const long ID_MENU_REBUILD;
        static const long ID_MENU_CLEAN;
        static const long ID_MENU_BUILD;
        static const long ID_MENU_RUN;

        DECLARE_EVENT_TABLE();
};

#endif // BUILDTARGETSWINDOW_H
