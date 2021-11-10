/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  ROUTE Plugin
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

#include "calculatorgui_impl.h"
#include "wx/math.h"



bool UTF82WC(const std::string  source, std::wstring & outstr)
{
	wxMBConvUTF8 conv;

	size_t nRealSize;
	wxWCharBuffer theBuffer = conv.cMB2WC(source.c_str(), source.size(), &nRealSize);
	if (nRealSize) {
		outstr.assign(theBuffer.data(), nRealSize);
		return true;
	}
	else {
		return false;
	}
}


CfgDlg::CfgDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : CfgDlgDef( parent, id, title, pos, size, style )
{
}

HlpDlg::HlpDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : HlpDlgDef( parent, id, title, pos, size, style )
{
}


FunDlg::FunDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : FunDlgDef( parent, id, title, pos, size, style )
{
    this->LoadCategories();
    this->LoadFunctions(wxT("All"),wxT("All"));
}

DegreeDlg::DegreeDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : MyDialog5(parent, id, title, pos, size, style)
{
}

void FunDlg::LoadFunctions(wxString Category, wxString Unit)
{
    
	//Clear Pulldown
    this->m_Function_Dropdown->Clear();
    //Load functions into pulldown
    for ( unsigned int count = 0; count < testf.m_Formula.GetCount() ; count++)
	{
        if (Category.IsSameAs(testf.m_Category[count], false) || Category.IsSameAs(wxT("all"), false)   ) this->m_Function_Dropdown->Append(testf.m_ShortDesc[count]);
	}
    this->m_Function_Dropdown->SetSelection(0);
    //Load default function in the menu
    this->OnItemSelect();
}

void FunDlg::LoadCategories(void)
{
    wxArrayString Categories;
    //Clear Pulldown
    this->m_Function_Categories->Clear();
    this->m_Function_Categories->Append(_("All"));
    //Cycle through functions
    for ( unsigned int count = 0; count < testf.m_Formula.GetCount() ; count++)
	{
        if (Categories.Index(testf.m_Category[count], false, false)==wxNOT_FOUND)
        {
            //Add Category to Wxarraystring
            Categories.Add(testf.m_Category[count]);
            //Add category to pulldown
            this->m_Function_Categories->Append(testf.m_Category[count]);
            //printf("Just added: %s\n",(const char*) testf.m_Category[count].mb_str() );
        }
	}
	this->m_Function_Categories->SetSelection(0);
}

void FunDlg::OnCategorySelect( wxCommandEvent& event )
{
    this->LoadFunctions(event.GetString(),wxT("All"));
	this->Fit();
	this->Layout();
}

void FunDlg::OnClose( wxCommandEvent& event )
{
	this->Close();
}

void FunDlg::OnItemSelect( wxCommandEvent& event )
{
    this->OnItemSelect();
	this->Fit();
	this->Layout();
}

void FunDlg::OnItemSelect(void)
    {
	m_button7->Show();
    wxString Selected_Result=	this->m_Function_Dropdown->GetString( this->m_Function_Dropdown->GetCurrentSelection()); // Return selected formula
    unsigned int n;
    for ( n = 0; n < testf.m_Formula.GetCount() ; n++)
    {
        if(Selected_Result==testf.m_ShortDesc[n]) break;//find out number of formula
    }
    this->testf.Selected_Formula=n;
    this->m_Function->SetLabel(testf.m_Formula[testf.Selected_Formula]);

	wxString resultFormula = m_Function->GetLabelText();
	if (resultFormula == wxString(_T("vhull=1.34*sqrt(LWL)"))) {
		m_textExtraDescription->SetValue(_("LWL = Length of water line\nVhull = Hull speed"));
		m_textExtraDescription->Show();
	}
	else if (resultFormula == wxString(_T("Maximum_Wavelength = 1.5 * sqrt(fetch)"))) {
		m_textExtraDescription->SetValue(_("fetch = distance over water that the wind blows"));
		m_textExtraDescription->Show();
	}
	else if (resultFormula == wxString(_T("Waveheight=0 ,CDU10=(0.879+0.075*U10)*10^-3,U10 < 8 ? alpha = 1 : alpha = 0.6 + 0.05 * U10 ,Cd=CDU10/alpha ,F_hat=(Fetch*9.80665)/U10^2 ,F_star=F_hat/Cd,F_tilde=CDU10*F_star ,H_tilde=0.3*alpha*(1-(1+0.004*sqrt(F_tilde/alpha))^-2) ,H_star=H_tilde/CDU10 ,H_hat=H_star*Cd ,Waveheight=H_hat*(U10^2)/9.80665"))) {
		m_textExtraDescription->SetValue(_("fetch = distance over water that the wind blows\nU10 - windspeed (10m) must be entered as m/s"));
		m_textExtraDescription->Show();
	}
	else if (resultFormula == wxString(_T("distance_horizon=sqrt(height* (height + 12756.32))"))) {
		m_textExtraDescription->SetValue(_("Height_observer is the height of the observer\nHeight_object is the height of the object"));
		m_textExtraDescription->Show();
	
	}else if (resultFormula == wxString(_T("DegreesIn=DegreesOut"))) {	
		m_button7->Hide();
		Plugin_Dialog->OnCalculateDegrees();
	}
	else {
		m_textExtraDescription->SetValue(_T(""));
		m_textExtraDescription->Hide();
	}	    

    this->m_Description->SetLabel(testf.m_LongDesc[testf.Selected_Formula]);
    this->m_Output_Parameter->SetLabel(testf.m_Formula[testf.Selected_Formula].BeforeFirst('='));
    PopulatePuldown(testf.m_Result_Unit[testf.Selected_Formula],this->m_Output_Parameter_UnitC, m_panel9);//Populate output units. Needs to be expanded for multiple. note m_panel 9 needs to be updated.
//Multiple return values update

    this->Parameter->SetLabel(testf.m_Input_parameter[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit[testf.Selected_Formula],this->Units, m_panel9);

    this->Parameter1->SetLabel(testf.m_Input_parameter1[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit1[testf.Selected_Formula],this->Units1, m_panel1);

    this->Parameter2->SetLabel(testf.m_Input_parameter2[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit2[testf.Selected_Formula],this->Units2, m_panel2);

    this->Parameter3->SetLabel(testf.m_Input_parameter3[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit3[testf.Selected_Formula],this->Units3, m_panel3);

    this->Parameter4->SetLabel(testf.m_Input_parameter4[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit4[testf.Selected_Formula],this->Units4, m_panel4);

    this->Parameter5->SetLabel(testf.m_Input_parameter5[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit5[testf.Selected_Formula],this->Units5, m_panel5);

    this->Parameter6->SetLabel(testf.m_Input_parameter6[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit6[testf.Selected_Formula],this->Units6, m_panel6);

    this->Parameter7->SetLabel(testf.m_Input_parameter7[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit7[testf.Selected_Formula],this->Units7, m_panel7);

    this->Parameter8->SetLabel(testf.m_Input_parameter8[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit8[testf.Selected_Formula],this->Units8, m_panel8);

    this->Parameter9->SetLabel(testf.m_Input_parameter9[testf.Selected_Formula]);
    PopulatePuldown(testf.m_Input_unit9[testf.Selected_Formula],this->Units9, m_panel9);

    this->m_Function->Wrap(400); ///Width of description can be put in settings
    this->m_Description->Wrap(400); ///Width of description can be put in settings
    this->Fit();
    this->m_Function_Result->SetValue(_T(""));
    }

    void FunDlg::PopulatePuldown(wxString& Input_Units, wxChoice* Pulldown, wxPanel *Panel)
    {
        if (Input_Units.IsEmpty())
            {
            Panel->Show(false);
            }
        else
            {
            //Clear Pulldown
            Pulldown->Clear();

            //Show panel
            Panel->Show(true);
            //Determine output units
            int unit_index=this->Units_conv.m_Unit.Index(Input_Units, false, false);
            if (unit_index==wxNOT_FOUND)
                {
                //Unit not found
              //  printf("Cannot find unit: %s\n",(const char*) Input_Units.mb_str() );
                Pulldown->Append(Input_Units);
                }
            else
                {
                //unit found
               // printf("Found unit: %s",(const char*) Input_Units.mb_str() );
               // printf(" of category: %s\n",(const char*) this->Units_conv.m_Unit_category[unit_index].mb_str() );

                for ( unsigned int count = 0; count < this->Units_conv.m_Unit.GetCount() ; count++)
                    {
                    if (Units_conv.m_Unit_category[unit_index].IsSameAs(this->Units_conv.m_Unit_category[count], false) )
                        {
                           // printf("Other units found in category: %s\n",(const char*) Units_conv.m_Unit[count].mb_str() );
                            if (Units_conv.m_Display[count].IsSameAs(wxT("TRUE"), false) ) Pulldown->Append(Units_conv.m_Unit[count]);
                        }
                    }
                }
            Pulldown->SetSelection(0);
            }
    }

wxString FunDlg::Unit_Conversion( wxString Output_Unit,wxString Input_Unit, wxString Var)
{
//This function returns the factor to convert from input to output unit
#ifdef DEBUG
printf("\n\n------------------------------------------------------\n");
printf("Input_Unit: %s\n",(const char*) Input_Unit.mb_str() );
printf("Output_Unit: %s\n",(const char*) Output_Unit.mb_str() );
printf("Var: %s\n",(const char*) Var.mb_str() );
#endif // DEBUG

//If Var is empty, return 0, nothing else to do
if (Var.IsEmpty()) return wxT("0");

if (Input_Unit.CmpNoCase(Output_Unit)!=0) //Convert if input unit is not output unit
    {
     //find input unit factor
     int to_base = this->Units_conv.m_Unit.Index(Input_Unit, false);
     int to_output = this->Units_conv.m_Unit.Index(Output_Unit, false);
     if (to_base==wxNOT_FOUND || to_output==wxNOT_FOUND)
        {
         #ifdef DEBUG
         printf("not found unit, end conversion\n");
         #endif // DEBUG
         return( Var );
        }
    else
        {
        #ifdef DEBUG
        printf("Conversion to base: %s\n",(const char*) this->Units_conv.m_Conversion[to_base].mb_str() );
        printf("Conversion to output: 1/%s\n",(const char*) this->Units_conv.m_Conversion[to_output].mb_str() );
        #endif // DEBUG
            //Check if to_base contains VAR, if yes to_base= replace VAR with Var else to_base=Var*to_base
            if (Units_conv.m_Conversion[to_base].Contains(wxT("VAR")))
                {
                Input_Unit = wxT("(")+Units_conv.m_Conversion[to_base].BeforeFirst('=')+wxT(")");
                Input_Unit.Replace(wxT("VAR"), Var);
                #ifdef DEBUG
                printf("Replace mode.... Input_Unit: -->%s<--\n",(const char*) Input_Unit.mb_str() );
                #endif // DEBUG
                }
            else
            {
                Input_Unit =wxT("(")+Var+wxT("*")+Units_conv.m_Conversion[to_base]+wxT(")");
                #ifdef DEBUG
                printf("Normal Mode: Input_Unit: -->%s<--\n",(const char*) Input_Unit.mb_str() );
                #endif // DEBUG
            }


            //Check if to_output contains VAR, if yes, replace VAR with to_base string, if not devide by to_output
            if (Units_conv.m_Conversion[to_output].Contains(wxT("VAR")))
                {
                Output_Unit = Units_conv.m_Conversion[to_output].AfterLast('=');
                Output_Unit.Replace(wxT("VAR"), Input_Unit);
                #ifdef DEBUG
                printf("Replace mode.... Output_Unit: -->%s<--\n",(const char*) Input_Unit.mb_str() );
                {wxString returnf=wxT("(")+Output_Unit+wxT(")");
                printf("Returning: -->%s<--\n",(const char*) returnf.mb_str() );}
                #endif
                return (wxT("(")+Output_Unit+wxT(")"));
                }
            else
            {
                Output_Unit =Input_Unit+wxT("/")+Units_conv.m_Conversion[to_output];
                #ifdef DEBUG
                printf("Normal Mode: Input_Unit: -->%s<--\n",(const char*) Output_Unit.mb_str() );
                {wxString returnf=wxT("(")+Output_Unit+wxT(")");
                printf("Returning: -->%s<--\n",(const char*) returnf.mb_str() );}
                #endif
                return (wxT("(")+Output_Unit+wxT(")"));
            }
        }
    }
else
    {
    #ifdef DEBUG
    printf("Nothing to do, return input variable\n");
    #endif
    return (Var);
    }
}

void FunDlg::OnOutputParameterChange(wxCommandEvent& event) {

	m_Function_Result->Clear();

}

void FunDlg::OnExtraCalculate( wxCommandEvent& event )
{
    //Calculate the result of the function
    /*
    To implement for multiple return values
    1)find out if there are multiple return values, and if so, how many (e.g. via number of output units)
    2)extract substring for each function
    3)loop over routine below, with exeption, that result has to end up in the right result box


    */
	wxString ResultDegrees = testf.m_Formula[testf.Selected_Formula];
	
    wxString Result=testf.m_Formula[testf.Selected_Formula].BeforeFirst('=');
    wxString Formula=testf.m_Formula[testf.Selected_Formula].AfterFirst('=');
    /*
    if (!testf.m_Input_parameter[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter[testf.Selected_Formula],this->NotEmpty(this->Value->GetValue()),true);
    if (!testf.m_Input_parameter1[testf.Selected_Formula].IsEmpty()) Formula.Replace(testf.m_Input_parameter1[testf.Selected_Formula],this->NotEmpty(this->Value1->GetValue()),true);
    if (!testf.m_Input_parameter2[testf.Selected_Formula].IsEmpty()) Formula.Replace(testf.m_Input_parameter2[testf.Selected_Formula],this->NotEmpty(this->Value2->GetValue()),true);
    if (!testf.m_Input_parameter3[testf.Selected_Formula].IsEmpty()) Formula.Replace(testf.m_Input_parameter3[testf.Selected_Formula],this->NotEmpty(this->Value3->GetValue()),true);
    if (!testf.m_Input_parameter4[testf.Selected_Formula].IsEmpty()) Formula.Replace(testf.m_Input_parameter4[testf.Selected_Formula],this->NotEmpty(this->Value4->GetValue()),true);
    if (!testf.m_Input_parameter5[testf.Selected_Formula].IsEmpty()) Formula.Replace(testf.m_Input_parameter5[testf.Selected_Formula],this->NotEmpty(this->Value5->GetValue()),true);
    if (!testf.m_Input_parameter6[testf.Selected_Formula].IsEmpty()) Formula.Replace(testf.m_Input_parameter6[testf.Selected_Formula],this->NotEmpty(this->Value6->GetValue()),true);
    if (!testf.m_Input_parameter7[testf.Selected_Formula].IsEmpty()) Formula.Replace(testf.m_Input_parameter7[testf.Selected_Formula],this->NotEmpty(this->Value7->GetValue()),true);
    if (!testf.m_Input_parameter8[testf.Selected_Formula].IsEmpty()) Formula.Replace(testf.m_Input_parameter8[testf.Selected_Formula],this->NotEmpty(this->Value8->GetValue()),true);
    if (!testf.m_Input_parameter9[testf.Selected_Formula].IsEmpty()) Formula.Replace(testf.m_Input_parameter9[testf.Selected_Formula],this->NotEmpty(this->Value9->GetValue()),true);
    */
    //Add input parameters converted for unit
    if (!testf.m_Input_parameter [testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit[testf.Selected_Formula],this->Units->GetString( this->Units->GetCurrentSelection()),this->Value->GetValue()),true);
    if (!testf.m_Input_parameter1[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter1[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit1[testf.Selected_Formula],this->Units1->GetString( this->Units1->GetCurrentSelection()),this->Value1->GetValue()),true);
    if (!testf.m_Input_parameter2[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter2[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit2[testf.Selected_Formula],this->Units2->GetString( this->Units2->GetCurrentSelection()),this->Value2->GetValue()),true);
    if (!testf.m_Input_parameter3[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter3[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit3[testf.Selected_Formula],this->Units3->GetString( this->Units3->GetCurrentSelection()),this->Value3->GetValue()),true);
    if (!testf.m_Input_parameter4[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter4[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit4[testf.Selected_Formula],this->Units4->GetString( this->Units4->GetCurrentSelection()),this->Value4->GetValue()),true);
    if (!testf.m_Input_parameter5[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter5[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit5[testf.Selected_Formula],this->Units5->GetString( this->Units5->GetCurrentSelection()),this->Value5->GetValue()),true);
    if (!testf.m_Input_parameter6[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter6[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit6[testf.Selected_Formula],this->Units6->GetString( this->Units6->GetCurrentSelection()),this->Value6->GetValue()),true);
    if (!testf.m_Input_parameter7[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter7[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit7[testf.Selected_Formula],this->Units7->GetString( this->Units7->GetCurrentSelection()),this->Value7->GetValue()),true);
    if (!testf.m_Input_parameter8[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter8[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit8[testf.Selected_Formula],this->Units8->GetString( this->Units8->GetCurrentSelection()),this->Value8->GetValue()),true);
    if (!testf.m_Input_parameter9[testf.Selected_Formula].IsEmpty())  Formula.Replace(testf.m_Input_parameter9[testf.Selected_Formula],Unit_Conversion(testf.m_Input_unit9[testf.Selected_Formula],this->Units9->GetString( this->Units9->GetCurrentSelection()),this->Value9->GetValue()),true);

    //Convert formula to desired output unit
    Formula=Unit_Conversion(this->m_Output_Parameter_UnitC->GetString( this->m_Output_Parameter_UnitC->GetCurrentSelection()),testf.m_Result_Unit[testf.Selected_Formula],Formula);

    Plugin_Dialog->m_result->SetValue(Result.Append('=').Append(Formula));
    //wxMessageBox(Formula);
    //Plugin_Dialog->OnCalculate();
    this->m_Function_Result->SetValue(Plugin_Dialog->OnCalculate());
}

void FunDlg::OnToggle( wxCommandEvent& event ){
    //wxMessageBox(_("toggle"));
    this->m_Description->Show(this->m_checkBox8->GetValue());
    this->Fit();
}

Dlg::Dlg(wxWindow *parent, calculator_pi *ppi)
	: DlgDef(parent)
{
	pPlugIn = ppi;
	wxString pi = "pi";
	wxString e = "e";
	wxString dtr = "dtr";
	MuParser.ClearConst();
	MuParser.DefineConst(WxString2StdString(pi), 3.141592653589793238462643);
	MuParser.DefineConst(WxString2StdString(e), 2.718281828459045235360287);
	MuParser.DefineConst(WxString2StdString(dtr), 0.0174532925199433);
    MuParser.SetVarFactory(facfun_type(AddVariable),&MuParser);

    this->m_listCtrl->Show(false);

    this->m_Overview->Layout();
    this->Layout();
    this->Fit();
    this->m_Overview->Fit();
    this->m_Overview->Layout();
    wh = this->GetSize();
	xy = this->GetPosition();
    i_counter=0;
    item_counter=0;
    MemoryFull=false;
    m_pHelpdialog=NULL;
    m_pFunctiondialog=NULL; //So we can check that the Function Function dialog has been opened.
	m_pDegreeDialog = NULL;
}
Dlg::~Dlg()
{
	
}

void Dlg::OnCalculateDegrees(void)
{
	if (NULL == m_pDegreeDialog)
	{
		m_pDegreeDialog = new DegreeDlg(this);	
		m_pDegreeDialog->SetSize(400, 500);
		m_pDegreeDialog->Plugin_Dialog = this;
		m_pDegreeDialog->Fit();
		m_pDegreeDialog->Layout();
	}
	m_pDegreeDialog->Show(!m_pDegreeDialog->IsShown());
}


void Dlg::OnClose(wxCloseEvent& event)
{
	pPlugIn->OnCalculatorDialogClose();
}

void Dlg::OnFunctionCalc( void )
{

        // Plugin_Dialog->OnFunctionCalc() ;
}

void Dlg::OnHelp( wxCommandEvent& event ){
    OnHelp();}

void Dlg::OnHelp( void )
{
    if (NULL==m_pHelpdialog)
    {
        //HlpDlg *m_pHelpdialog = new HlpDlg(this);m_pHelpdialog->Show(true);
        m_pHelpdialog = new HlpDlg(this);
		m_pHelpdialog->SetSize(650, 500);
		m_pHelpdialog->m_scrolledWindow2->Fit();
        m_pHelpdialog->m_textCtrl3->Fit();
        m_pHelpdialog->m_textCtrl3->Layout();
    }
    m_pHelpdialog->Show(!m_pHelpdialog->IsShown());
}

void Dlg::OnFunction( wxCommandEvent& event )
{
    OnFunction();
}

void Dlg::OnFunction( void )
{
        if(NULL == m_pFunctiondialog){
            //FunDlg *m_pFunctiondialog = new FunDlg(this);
			m_pFunctiondialog = new FunDlg(this); 
			m_pFunctiondialog->Plugin_Dialog = this;
			m_pFunctiondialog->SetSize(600, 700);
			m_pFunctiondialog->m_scrolledWindow1->Fit();
			m_pFunctiondialog->Layout();
        }
        m_pFunctiondialog->Show(!m_pFunctiondialog->IsShown());
}

void Dlg::set_History(void)
{
    if(this->m_Help->GetValue())
    {
		m_bshowhistory = true;
		m_bshowhistoryP = true;
		//Capture dialog position
        xy=this->GetPosition();
        //Capture dialog size
        wh=this->GetSize();
        wh.y=-1; //Set default dialog height
		this->m_HistoryPulldown->Show(true);
    	this->m_listCtrl->Show(true);
    	this->m_listCtrl->Show(true);
    	this->m_listCtrl->Fit();
        this->m_Overview->Layout();
        this->Fit();  

		pPlugIn->SaveConfig();
    }
    else
    {
		m_bshowhistory = false;
		m_bshowhistoryP = false;
		this->m_HistoryPulldown->Show(false);
		this->m_listCtrl->Show(false);
    	this->m_listCtrl->Show(false);
    	this->m_listCtrl->Fit();
        this->m_Overview->Layout();
        this->Fit();
        this->Layout();
        this->SetPosition(xy);
        //Capture dialog size
        this->SetSize(wh);

		pPlugIn->SaveConfig();
    }
    wxMilliSleep(50);
}

#ifdef DEBUG
void Dlg::OnTest(wxCommandEvent& event){
   wxMessageBox(_("Test"));
}

void Dlg::OnTest(wxMouseEvent& event){
   wxMessageBox(_("Mouse"));
}

void Dlg::OnTest(wxListEvent& event){
   wxMessageBox(_("List"));
}
#endif // DEBUG
void Dlg::OnItem(wxListEvent& event){
    long item = -1;
    wxString ItemText;	
    for ( ;; )
    {
        item = this->m_listCtrl->GetNextItem(item,
                                     wxLIST_NEXT_ALL,
                                     wxLIST_STATE_SELECTED);
		if (item == -1) break;
		
        ItemText=this->m_listCtrl->GetItemText(item);		
        ItemText=ItemText.BeforeFirst('=');
        //ItemText=ItemText.BeforeFirst(' ');
        m_result->AppendText(ItemText);
    }
	
}

void Dlg::OnToggle( wxCommandEvent& event )
{
        this->set_History();
		
}

void Dlg::set_Buttons(void)
{
    this->m_HelpButton->Show(m_bshowhelpB);
    this->m_Help->Show(m_bshowhistoryB);
    this->Calculate->Show(m_bCalculateB);
    this->m_Function->Show(m_bshowFunction);
	
    this->m_Overview->Fit();
    this->m_Overview->Layout();
    this->m_Help->SetValue(m_bshowhistory);

    if (m_bshowhistory||m_bshowhistoryP)this->set_History();
}

void Dlg::OnCalculate( wxCommandEvent& event )
{
    OnCalculate();
}

void Dlg::OnClear(wxCommandEvent& event)
{
	m_result->SetValue(_T(""));
}


wxString Dlg::OnCalculate( void )
{
    //char* test;
    wxString Text = m_result->GetValue();

    bool error_check=false;
    if ((Text.StartsWith(_("Error"))) ){
        error_check=true;
    }

	if (Text.StartsWith(_T("min")) || Text.StartsWith(_T("max")) || Text.StartsWith(_T("sum")) || Text.StartsWith(_T("avg"))) {
		if (!Text.Contains(_T("(")) && !Text.Contains(_T(")"))) {
			error_check = true;
			m_result->SetValue(_("Error:Missing parenthesis"));
			return wxT("");
		}
	}

    if ((Text.StartsWith(_T("clear"))) || (Text.StartsWith(_T("Clear")))|| (Text.StartsWith(_T("CLEAR")))){ //clear old results
        m_listCtrl->ClearAll();
        error_check=true;
    }
    if ((Text.StartsWith(_T("HideHelp"))) || (Text.StartsWith(_T("hidehelp")))|| (Text.StartsWith(_T("HIDEHELP"))) ||(Text.StartsWith(_T("ShowHelp"))) || (Text.StartsWith(_T("showhelp")))|| (Text.StartsWith(_T("SHOWHELP"))) ){
        m_bshowhelpB=(!m_bshowhelpB);
        this->set_Buttons();
        error_check=true;
    }

    if ((Text.StartsWith(_T("HideHistory"))) || (Text.StartsWith(_T("hidehistory")))|| (Text.StartsWith(_T("HIDEHISTORY")))||(Text.StartsWith(_T("ShowHistory"))) || (Text.StartsWith(_T("showhistory")))|| (Text.StartsWith(_T("SHOWHISTORY")))){
        m_bshowhistoryB=(!m_bshowhistoryB);
        this->set_Buttons();
        error_check=true;
    }

    if ((Text.StartsWith(_T("HideCalculate"))) || (Text.StartsWith(_T("hidecalculate")))|| (Text.StartsWith(_T("HIDECALCULATE"))) || (Text.StartsWith(_T("ShowCalculate"))) || (Text.StartsWith(_T("showcalculate")))|| (Text.StartsWith(_T("SHOWCALCULATE")))){
        m_bCalculateB=(!m_bCalculateB);
        this->set_Buttons();
        error_check=true;
    }

    if ((Text.StartsWith(_T("HideFunction"))) || (Text.StartsWith(_T("hidefunction")))|| (Text.StartsWith(_T("HIDEFUNCTION"))) || (Text.StartsWith(_T("showfunction")))|| (Text.StartsWith(_T("ShowFunction")))|| (Text.StartsWith(_T("SHOWFUNCTION")))){
        m_bshowFunction=(!m_bshowFunction);
        this->set_Buttons();
        error_check=true;
    }

    if ((Text.StartsWith(_T("Help"))) || (Text.StartsWith(_T("HELP")))|| (Text.StartsWith(_T("help")))){
        this->OnHelp ();
        error_check=true;
    }

    if ((Text.StartsWith(_T("Function"))) || (Text.StartsWith(_T("function")))|| (Text.StartsWith(_T("FUNCTION")))){
        this->OnFunction ();
        error_check=true;
    }

    if ((Text.StartsWith(_T("history"))) || (Text.StartsWith(_T("History")))|| (Text.StartsWith(_T("HISTORY")))){
        m_bshowhistory=(!m_bshowhistory);
        this->m_Help->SetValue(m_bshowhistory);
        this->set_History();
        error_check=true;
    }

    if (error_check)
        {
        m_result->SetValue(_T(""));
        return wxT("");
        }
    else
        {
        wxString mystring;
        #ifdef DEBUG
        printf("Input: %s\n",(const char*) Text.mb_str() );
        #endif // DEBUG

        MuParser.SetExpr(WxString2StdString(Text)); //Typecast to mu::stringtype
        double Muparser_result=0;
        try
        {
            Muparser_result = MuParser.Eval();//Get the result
            mystring=wxT("ans=")+double2wxT(Muparser_result);//set ans string (borrow the mystring);
           // mystring.Replace(wxT(","),wxT("."),TRUE);//dont think this is required when not using locale --->test
           // MuParser.SetExpr((mu::string_type) mystring.mb_str()); //This works in linux, but causes compiler error in windows
             MuParser.SetExpr(WxString2StdString(mystring));//Store the answer in ans
			 mystring = Report_Value(Muparser_result, m_iCalc_Reporting); // m_iCalc_Reporting);//Format result as per setting.
            Muparser_result = MuParser.Eval();//Evaluate for ans

        }
        //catch(...)
        catch(mu::Parser::exception_type &e)
        {
            {
            mu::string_type sLine;
            sLine=e.GetMsg();
            wxString tempstring(sLine.c_str(), wxConvUTF8);
            mystring=wxT("Error: ")+tempstring;
            }
            error_check=true;
            //printf("Message: %s\n",(const char*) mystring.mb_str() );
        }
        if (m_blogresults) wxLogMessage(_("Calculator INPUT:") + Text + _(" Calculator output:") + mystring); //log into OpenCPN

        if((!this->m_Help->GetValue()) || (error_check)) //print result in messagebox if not history box or error
            m_result->SetValue(mystring.c_str());
        else
            m_result->SetValue(_T(""));

        Text.Right(Text.Length()-3);

        if (!error_check )
            {
            if ((this->m_Help->GetValue())) //if ((this->m_Help->GetValue()) || (m_bcapturehidden))
                {
                if ((int)(this->m_HistoryPulldown->GetCount())<(int)Max_Results){//Items in pulldown memory less than X
                    HistoryPulldownitemIndex=m_HistoryPulldown->Append(Text + wxT(" = ") + mystring); //Append
                }
                else
                {
                    HistoryPulldownitemIndex++;
                    if (HistoryPulldownitemIndex>=Max_Results) HistoryPulldownitemIndex=0;
                    this->m_HistoryPulldown->SetString(HistoryPulldownitemIndex,Text + wxT(" = ") + mystring);//Overwrite previous value
                }

                m_HistoryPulldown->SetSelection(HistoryPulldownitemIndex); //Activate lastest result in pulldown

                item_counter++;
                if (this->m_listCtrl->GetItemCount()<Max_Results*5){//Items in pulldown memory less than 5*max results
                //do nothing
                }
                else{
                    if (item_counter>=Max_Results*5) item_counter=0;
                    m_listCtrl->DeleteItem(item_counter);
                }
                itemIndex = m_listCtrl->InsertItem(item_counter, Text + wxT(" = ") + mystring); //Here input+result are stored in the memory box

                //printf("Total items in m_listctr %ld\n",m_listCtrl->GetItemCount());
                m_listCtrl->EnsureVisible(itemIndex); //make sure latest result is visible in history box
                //printf("Item counter:%d, Max results: %d, ItemIndex: %ld\n",HistoryPulldownitemIndex,Max_Results,itemIndex);
                }
            //event.Skip();
            }
            return mystring;
        }
        return wxT("");//Just in case, should not be required
}

void Dlg::OnKey (wxKeyEvent& event)
{
    wxChar uc = event.GetUnicodeKey();
    // It's a "normal" character. Notice that this includes
    // control characters in 1..31 range, e.g. WXK_RETURN or
    // WXK_BACK, so check for them explicitly.
    if ( ((uc >= 32) && (uc<=127))  )
        m_result->AppendText(uc);
    else if ( uc == 13 )
        OnCalculate();
    else if ( uc == 8 ){
        wxString Text = m_result->GetValue();
        m_result->SetValue(Text.Left(Text.Len()-1 ));
    }
}

void Dlg::key_shortcut(wxKeyEvent& event) {
    // of course, it doesn't have to be the control key. You can use others:
    // http://docs.wxwidgets.org/stable/wx_wxkeyevent.html
   // if(event.GetModifiers() == wxMOD_CONTROL) {
        switch(event.GetKeyCode()) {
            case WXK_DOWN: // can return the upper ASCII value of a key
                // do whatever you like for a Ctrl+G event here!
                //wxMessageBox(_("down") );
                down();
                //OnCursor();
                break;
            case WXK_UP: // we also have special keycodes for non-ascii values.
                // get a full list of special keycodes here:
                // http://docs.wxwidgets.org/stable/wx_keycodes.html
                //wxMessageBox(_("up") );
                up();

                break;
            default: // do nothing

                break;
      //  }
   }
   event.Skip();
}

void Dlg::up()
{
    i_counter--;
    if (i_counter<0) i_counter=this->HistoryPulldownitemIndex;
    m_result->SetValue(this->m_HistoryPulldown->GetString(i_counter ));


}

void Dlg::down()
{
    i_counter++;
    if (i_counter>this->HistoryPulldownitemIndex) i_counter=0;
    m_result->SetValue(this->m_HistoryPulldown->GetString(i_counter ));




}

wxString Dlg::Report_Value(double in_Value, int in_mode){
    wxString Temp_String=wxT("");
    int human_magnitude=0;
    double result=0;
    switch(in_mode) {
		case 0:
			//printf("Three decimal places (Default)\n");
			return wxString::Format(wxT("%15.3f"), in_Value);
			break;	
		case 1:
            //printf("Precise\n");
            return wxString::Format(wxT("%15.15g"), in_Value);
            break;
        case 2:
            //printf("Precise, thousands separator\n");
            //setlocale(LC_ALL,""); //Causes Serious errors in OPENCPN, rounding all tracks waypoints and incoming data.
            return ThousandSeparator(wxString::Format(wxT("%15.15g"), in_Value));
            //return Temp_String;
            break;
        case 3:
            //printf("Succinct\n");
            return wxString::Format(wxT("%15.7g"), in_Value);
            break;
        case 4:
            //printf("Succinct, thousands separator\n");
            return ThousandSeparator(wxString::Format(wxT("%15.7g"), in_Value));
            break;
        case 5:
            //printf("Scientific\n");
            return wxString::Format(wxT("%.15e"), in_Value);
            break;
        case 6:
            //printf("Humanise\n");
            try{
                Temp_String=wxT("log10(abs(")+double2wxT(in_Value)+wxT("))/3");

                 MuParser.SetExpr(WxString2StdString(Temp_String));
                human_magnitude=(int) MuParser.Eval();
                if (in_Value<1) {human_magnitude--;}
                Temp_String=double2wxT(in_Value)+wxT("*10^(-3*")+double2wxT((double)human_magnitude)+wxT(")");

             //   MuParser.SetExpr(static_cast<const char*>(Temp_String.mb_str()));
             MuParser.SetExpr(WxString2StdString(Temp_String));
                result=MuParser.Eval();
                if (in_Value==0) {human_magnitude=0;}
                switch(human_magnitude){
                    case 8:Temp_String=wxT("yotta");break;
                    case 7:Temp_String=wxT("zetta");break;
                    case 6:Temp_String=wxT("exa");break;
                    case 5:Temp_String=wxT("peta");break;
                    case 4:Temp_String=wxT("tera");break;
                    case 3:Temp_String=wxT("giga");break;
                    case 2:Temp_String=wxT("mega");break;
                    case 1:Temp_String=wxT("kilo");break;
                    case 0:Temp_String=wxT("");break;
                    case -1:Temp_String=wxT("milli");break;
                    case -2:Temp_String=wxT("micro");break;
                    case -3:Temp_String=wxT("nano");break;
                    case -4:Temp_String=wxT("pico");break;
                    case -5:Temp_String=wxT("femto");break;
                    case -6:Temp_String=wxT("atto");break;
                    case -7:Temp_String=wxT("zepto");break;
                    case -8:Temp_String=wxT("yocto");break;

                    default: // do nothing
                        Temp_String=wxT("result out of SI prefixes range");
                        result=in_Value;
                    break;
                }
            }
            catch(mu::Parser::exception_type &e) {
                #ifdef DEBUG
                printf("Error Humanising number!!\n");
                #endif // DEBUG
                }
            return double2wxT(result) + wxT(" ") + Temp_String;
            break;
        default: // do nothing
            return wxT("Mode not Implemented");
            break;
        }
}

wxString Dlg::double2wxT(double in_Value){
    return wxString::Format(wxT("%15.15g"), in_Value);
}

wxString Dlg::ThousandSeparator(wxString Number_in){
    wxString Decimal_part=wxT("");
    //int i = Number_in.find(wxT("."));
    if((int)Number_in.find(wxT("."))<0) {Decimal_part=wxT("");} else {Decimal_part=wxT(".")+Number_in.AfterFirst('.');}
    Number_in=Number_in+wxT("."); //Add decimal point at the end
    wxString Temp_String=Number_in.BeforeFirst('.'); //Extract part before decimal point
    wxString Temp_String2=wxT("");

    while (Temp_String.length()>3) {
        Temp_String2=wxT(" ")+Temp_String.Right(3)+Temp_String2;
        printf("Temp String2:%s\n",(const char*)Temp_String2.mb_str());
        Temp_String=Temp_String.Left(Temp_String.length()-3);
        printf("Temp String:%s\n",(const char*)Temp_String.mb_str());
        }
    return Temp_String+Temp_String2+Decimal_part;
}


void Dlg::OnHistoryPulldown ( wxCommandEvent& event ){
    wxString Selected_Result=	this->m_HistoryPulldown->GetString( this->m_HistoryPulldown->GetCurrentSelection());
	if (Selected_Result == wxEmptyString) {
		this->m_result->SetFocus();
	}
    Selected_Result=Selected_Result.BeforeFirst('=');
    //ItemText=ItemText.BeforeFirst(' ');
    this->m_result->AppendText(Selected_Result);
}
/*
std::string Dlg::WxString2StdString(wxString wxString_in){
    return std::string(wxString_in.mb_str());
}


mu::string_type Dlg::WxString2StdString(wxString wxString_in){
	

	std::string s = std::string(wxString_in).c_str();
	std::wstring ws;
	UTF82WC(s, ws);
	mu::string_type mws = ws;

	
	

	return ws;
}*/

mu::string_type Dlg::WxString2StdString(wxString wxString_in){
    return wxString_in.wchar_str();
}

void DegreeDlg::OnConvertToDegree(wxCommandEvent& event)
{
	double DDLat;
	double DDLon;

	double DDLat1;
	double DDLon1;

	double MMLat;
	double MMLon;

	double SSLat;
	double SSLon;

	int DDlat1;
	int DDlon1;

	int MMlat1;
	int MMlon1;

	int NS;
	int EW;

	double MMlat0;
	double MMlon0;

	double MMlat2;
	double MMlon2;

	double SSlat1;
	double SSlon1;

	double value;

	wxString s;
	wxString s1;
	wxString m1;
	wxString d1;

	switch (m_wxNotebook234->GetSelection()) {
		case 0:
			s = m_Lat1->GetValue();
			s.ToDouble(&value);
			DDLat = value;
			s = m_Lon1->GetValue();
			s.ToDouble(&value);
			DDLon = value;

			DDlat1 = abs(int(DDLat));
			DDlon1 = abs(int(DDLon));

			// set the ddmm page

			m_Lat1_d1->SetValue(wxString::Format(_T("%i"), (int)DDlat1));
			m_Lon1_d1->SetValue(wxString::Format(_T("%i"), (int)DDlon1));

			MMlat0 = (fabs(DDLat) - double(DDlat1)) * 60;
			MMlon0 = (fabs(DDLon) - double(DDlon1)) * 60;

			m_Lat1_m1->SetValue(wxString::Format(_T("%8.6f"), MMlat0));
			m_Lon1_m1->SetValue(wxString::Format(_T("%8.6f"), MMlon0));
			
			if (DDLat > 0) {
				m_Lat1_NS1->SetSelection(0);
				m_Lat1_NS->SetSelection(0);
			}
			else {
				m_Lat1_NS1->SetSelection(1);
				m_Lat1_NS->SetSelection(1);
			}

			if (DDLon > 0) {
				m_Lon1_EW1->SetSelection(0);
				m_Lon1_EW->SetSelection(0);
			}
			else {
				m_Lon1_EW1->SetSelection(1);
				m_Lon1_EW->SetSelection(1);
			}
			
			// set the ddmmss page			

			m_Lat1_d->SetValue(wxString::Format(_T("%i"), abs((int)DDlat1)));
			m_Lon1_d->SetValue(wxString::Format(_T("%i"), abs((int)DDlon1)));	

			m_Lat1_m->SetValue(wxString::Format(_T("%i"), abs((int)MMlat0)));
			m_Lon1_m->SetValue(wxString::Format(_T("%i"), abs((int)MMlon0)));

			MMlat2 = int(MMlat0);
			MMlon2 = int(MMlon0);

			SSlat1 = (MMlat0 - MMlat2) * 60;
			SSlon1 = (MMlon0 - MMlon2) * 60;

			m_Lat1_s->SetValue(wxString::Format(_T("%8.6f"), SSlat1));
			m_Lon1_s->SetValue(wxString::Format(_T("%8.6f"), SSlon1));



			break;

		case 1:

			d1 = m_Lat1_d1->GetValue();
			d1.ToDouble(&value);
			DDLat = value;
			d1 = m_Lon1_d1->GetValue();
			d1.ToDouble(&value);
			DDLon = value;

			m1 = m_Lat1_m1->GetValue();
			m1.ToDouble(&value);
			MMLat = value;
			m1 = m_Lon1_m1->GetValue();
			m1.ToDouble(&value);
			MMLon = value;

			DDLat1 = DDLat + (MMLat / 60);
			DDLon1 = DDLon + (MMLon / 60);

			//wxMessageBox(wxString::Format(_T("%f"), DDLat1));
			//wxMessageBox(wxString::Format(_T("%f"), DDLon1));

			NS = m_Lat1_NS1->GetSelection();
			if (NS == 1) {
				DDLat1 *= -1;
			}
			EW = m_Lon1_EW1->GetSelection();
			if (EW == 1) {
				DDLon1 *= -1;
			}

			// set the dd.dddd page

			m_Lat1->SetValue(wxString::Format(_T("%8.6f"), DDLat1));
			m_Lon1->SetValue(wxString::Format(_T("%8.6f"), DDLon1));	

			MMlat1 = int(MMLat);
			MMlon1 = int(MMLon);

			// set the ddmmss page
			if (m_Lat1_NS1->GetSelection() == 0) {
				m_Lat1_NS->SetSelection(0);
			}
			else {
				m_Lat1_NS->SetSelection(1);
			}
			if (m_Lon1_EW1->GetSelection() == 0) {
				m_Lon1_EW->SetSelection(0);
			}
			else {
				m_Lon1_EW->SetSelection(1);
			}


			m_Lat1_d->SetValue(wxString::Format(_T("%i"), abs((int)DDLat1)));
			m_Lon1_d->SetValue(wxString::Format(_T("%i"), abs((int)DDLon1)));


			m_Lat1_m->SetValue(wxString::Format(_T("%i"), MMlat1));
			m_Lon1_m->SetValue(wxString::Format(_T("%i"), MMlon1));

			MMlat2 = (MMLat - double(MMlat1))*60;
			MMlon2 = (MMLon - double(MMlon1))*60;

			m_Lat1_s->SetValue(wxString::Format(_T("%8.6f"), MMlat2));
			m_Lon1_s->SetValue(wxString::Format(_T("%8.6f"), MMlon2));

			
			break;
		case 2:
			d1 = m_Lat1_d->GetValue();
			d1.ToDouble(&value);
			DDLat = value;
			d1 = m_Lon1_d->GetValue();
			d1.ToDouble(&value);
			DDLon = value;

			m1 = m_Lat1_m->GetValue();
			m1.ToDouble(&value);
			MMLat = value;
			m1 = m_Lon1_m->GetValue();
			m1.ToDouble(&value);
			MMLon = value;

			s1 = m_Lat1_s->GetValue();
			s1.ToDouble(&value);
			SSLat = value;
			s1 = m_Lon1_s->GetValue();
			s1.ToDouble(&value);
			SSLon = value;

			DDLat1 = DDLat + MMLat / 60 + SSLat / 3600;
			DDLon1 = DDLon + MMLon / 60 + SSLon / 3600;

			NS = m_Lat1_NS->GetSelection();
			if (NS == 1) {
				DDLat1 *= -1;
				m_Lat1_NS1->SetSelection(1);
				m_Lat1_NS->SetSelection(1);
			}
			else {
				m_Lat1_NS1->SetSelection(0);
				m_Lat1_NS->SetSelection(0);
			}

			EW = m_Lon1_EW->GetSelection();
			if (EW == 1) {
				DDLon1 *= -1;
				m_Lon1_EW1->SetSelection(1);
				m_Lon1_EW->SetSelection(1);
			}
			else {
				m_Lon1_EW1->SetSelection(0);
				m_Lon1_EW->SetSelection(0);
			}

			// set dd.ddd page

			m_Lat1->SetValue(wxString::Format(_T("%8.6f"), DDLat1));
			m_Lon1->SetValue(wxString::Format(_T("%8.6f"), DDLon1));

			// set ddmm page

			m_Lat1_d1->SetValue(wxString::Format(_T("%i"), abs((int)DDLat1)));
			m_Lon1_d1->SetValue(wxString::Format(_T("%i"), abs((int)DDLon1)));


			DDLat1 = MMLat + SSLat / 60;
			DDLon1 = MMLon + SSLon / 60;

			m_Lat1_m1->SetValue(wxString::Format(_T("%8.6f"), DDLat1));
			m_Lon1_m1->SetValue(wxString::Format(_T("%8.6f"), DDLon1));


			break;
	}
	


}

void DegreeDlg::OnNoteBookFit(wxBookCtrlEvent& event) {
	
}

void DegreeDlg::OnCloseDegreeDlg(wxCloseEvent& event) {
	Plugin_Dialog->m_pDegreeDialog->Hide();
	Plugin_Dialog->m_pFunctiondialog->m_button7->Show();
	Plugin_Dialog->m_pFunctiondialog->LoadFunctions(wxT("All"), wxT("All"));
	Plugin_Dialog->m_pFunctiondialog->m_scrolledWindow1->Fit();
	Plugin_Dialog->m_pFunctiondialog->Layout();
	//Plugin_Dialog->m_pFunctiondialog->Refresh();
	Refresh(m_parent);
}

void DegreeDlg::OnClose(wxCommandEvent& event) {
	Plugin_Dialog->m_pDegreeDialog->Hide();
	Plugin_Dialog->m_pFunctiondialog->m_button7->Show();
	Plugin_Dialog->m_pFunctiondialog->LoadFunctions(wxT("All"), wxT("All"));
	Plugin_Dialog->m_pFunctiondialog->m_scrolledWindow1->Fit();
	Plugin_Dialog->m_pFunctiondialog->Layout();
	//Plugin_Dialog->m_pFunctiondialog->Refresh();
	Refresh(m_parent);
}


