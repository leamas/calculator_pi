/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  CALCULATOR Plugin
 * Author:   SaltyPaws
 *
 ***************************************************************************
 *   Copyright (C) 2012 by Brazil BrokeTail                                *
 *   $EMAIL$                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#ifndef _CALCULATOR_PI_H_
#define _CALCULATOR_PI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/fileconf.h>

#include "ocpn_plugin.h"
#include "calculatorgui_impl.h"
#include "config.h"

//#define DEBUG  //No debugging text in production plugin!

class Dlg;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define CALCULATOR_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class calculator_pi : public opencpn_plugin_116
{
public:
      calculator_pi(void *ppimgr);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

//    The required override PlugIn Methods
      int GetToolbarToolCount(void);
      void ShowPreferencesDialog( wxWindow* parent );
      void OnToolbarToolCallback(int id);

//    Optional plugin overrides
      void SetColorScheme(PI_ColorScheme cs);

//    The override PlugIn Methods

//    Other public methods
      void SetCalculatorDialogX         (int x){ m_calculator_dialog_x = x;};
      void SetCalculatorDialogY         (int x){ m_calculator_dialog_y = x;};
      void SetCalculatorDialogWidth     (int x){ m_calculator_dialog_width = x;};
      void SetCalculatorDialogHeight    (int x){ m_calculator_dialog_height = x;};
      //void GetMaxResults                (int x){ m_iMaxResults = x;}
	  wxBitmap m_panelBitmap;
	  void				OnCalculatorDialogClose();
	  bool              SaveConfig(void);
	  
	  bool              m_bshowhistory;
	  bool              m_bshowhistoryB;
	  bool              m_bshowhistoryP;

private:
      Dlg               *m_pDialog;
      void              SettingsPropagate(void);  
      wxFileConfig      *m_pconfig;
      wxWindow          *m_parent_window;
      bool              LoadConfig(void);
      

      int               m_calculator_dialog_x, m_calculator_dialog_y,m_calculator_dialog_width,m_calculator_dialog_height;
      int               m_display_width, m_display_height;
      int               m_iOpacity;
      int               m_iMaxResults;
      int               m_Calculator_tool_id;
      int               m_CalculatorFX_tool_id;
      bool              m_bshowhelpB;
      
      bool              m_bCalculateB;
      bool              m_bshowFunction;
     
      //bool              m_bcapturehidden;
      bool              m_blogresults;

      int               m_iCalc_Reporting;
      
      bool              m_bshowfunction_Open_CPN_BAR;
	  bool			    m_bShowCalculator;
	  bool			    m_bShowCalculatorFX;
};

#endif
