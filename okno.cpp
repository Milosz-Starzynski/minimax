#include "okno.hh"
#include "minimax.cpp"
#include <wx/toolbar.h>

wxBEGIN_EVENT_TABLE(Okno, wxFrame)
wxEND_EVENT_TABLE()

bool App::OnInit()
{
    Okno *okno = new Okno();
    okno->Show(true);
        
    return true;
    }

Okno::Okno()
  : wxFrame(NULL, wxID_ANY,  L"K\u00f3\u0142ko i krzy\u017cyk")
{
  wxDisplay ekran(wxDisplay::GetFromWindow(this));
  wxRect rozmiar = ekran.GetGeometry();
  int wymiar = 0.8*std::min(rozmiar.GetHeight(), rozmiar.GetWidth());
  this->SetSize(wxSize(wymiar,wymiar));

  wxToolBar *toolbar = CreateToolBar();
  wxBitmap ikonka(wxT("ikonka.png"), wxBITMAP_TYPE_PNG);
  wxBitmap powtorz(wxT("powtorz.png"), wxBITMAP_TYPE_PNG);
  toolbar->AddTool(123, wxT("Ustawienia"), ikonka);
  toolbar->Realize();
  toolbar->AddTool(124, wxT("Wyczyść planszę"), powtorz);
  toolbar->Realize();
  Connect(123, wxEVT_COMMAND_TOOL_CLICKED, 
	  wxCommandEventHandler(Okno::ustawienia));
  Connect(124, wxEVT_COMMAND_TOOL_CLICKED, 
	  wxCommandEventHandler(Okno::wyczysc_plansze));
    
  int n = plansza->rozmiar();

  przyciski = new wxButton*[n*n];
  
  wxGridSizer *siatka = new wxGridSizer(n, n, 0, 0);
  wxFont czcionka(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
  
  for(int i=0; i<n*n; ++i){
    przyciski[i] = new wxButton(this, 10000+i);
    siatka->Add(przyciski[i], 1, wxEXPAND | wxALL);

    przyciski[i]->SetFont(czcionka);
    przyciski[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Okno::PrzyWcisnieciu, this);
  }

  this->SetSizer(siatka);
  siatka->Layout();
  
}

Okno::~Okno(){
  delete[] przyciski;
}

void Okno::PrzyWcisnieciu(wxCommandEvent &zdarzenie){
  int i = zdarzenie.GetId() - 10000;

  przyciski[i]->SetLabel("O");
  (*plansza)(i/plansza->rozmiar(), i%plansza->rozmiar()) = -1;

  przyciski[i]->Enable(false);

  int g = 15/plansza->rozmiar()<2 ? 2 : 15/plansza->rozmiar();

  minimax(*plansza, g, 1);
  aktualizuj_plansze();
  if(plansza->sprawdz_zwyciestwo(1)){
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("X wygrało"), wxT("Koniec gry!"), wxOK);
    dial->ShowModal();
  }
  else if(plansza->sprawdz_zwyciestwo(-1)){
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("O wygrało"), wxT("Koniec gry!"), wxOK);
    dial->ShowModal();
  }
  else if(plansza->sprawdz_koniec()){
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Remis"), wxT("Koniec gry!"), wxOK);
    dial->ShowModal();
  }
  zdarzenie.Skip();
}

void Okno::ustawienia(wxCommandEvent& zdarzenie){
  //gra();
  wxNumberEntryDialog wybor_n =  wxNumberEntryDialog(NULL, wxT("Wybierz rozmiar planszy:"), wxT(""), wxT("Ustawienia"), 3, 3, 50);
  wybor_n.ShowModal();
  int n = wybor_n.GetValue();
  wxNumberEntryDialog wybor_k =  wxNumberEntryDialog(NULL, wxT("Określ, ile znaków w linii jest potrzebnych do wygranej:"), wxT(""), wxT("Ustawienia"),n,3,n);
  wybor_k.ShowModal();
  int k = wybor_k.GetValue();
  wxArrayString array;
  array.Add("komputer"); array.Add("gracz");
  kto_zaczyna = wxGetSingleChoiceIndex(wxT("Kto ma zaczynać grę?"), wxT(""), array);
  delete plansza;
  plansza = new Plansza(n, k);

  delete [] przyciski;
  przyciski = new wxButton*[n*n];
  wxCommandEvent zda;
  wyczysc_plansze(zda);
  
}

void Okno::aktualizuj_plansze(){
  for(int i=0; i<plansza->rozmiar(); ++i)
    for(int j=0; j<plansza->rozmiar(); ++j)
      switch((*plansza)(i,j)){
      case 1:
	przyciski[i*plansza->rozmiar()+j]->SetLabel("X");
	przyciski[i*plansza->rozmiar()+j]->Enable(false);
	break;
      case -1:
	przyciski[i*plansza->rozmiar()+j]->SetLabel("O");
	przyciski[i*plansza->rozmiar()+j]->Enable(false);
	break;
      default: break;
      }
}

void Okno::wyczysc_plansze(wxCommandEvent& zdarzenie){

  for(int i=0; i<plansza->rozmiar(); ++i)
    for(int j=0; j<plansza->rozmiar(); ++j)
      (*plansza)(i,j) = 0;

  wxGridSizer *siatka = new wxGridSizer(plansza->rozmiar(), plansza->rozmiar(), 0, 0);
  wxFont czcionka(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
  for(int i=0; i<plansza->rozmiar()*plansza->rozmiar(); ++i){
    przyciski[i] = new wxButton(this, 10000+i);
    siatka->Add(przyciski[i], 1, wxEXPAND | wxALL);

    przyciski[i]->SetFont(czcionka);
    przyciski[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Okno::PrzyWcisnieciu, this);
  }
  this->SetSizer(siatka);
  siatka->Layout();
  wxDisplay ekran(wxDisplay::GetFromWindow(this));
  wxRect rozmiar = ekran.GetGeometry();
  int wymiar = 0.8*std::min(rozmiar.GetHeight(), rozmiar.GetWidth());
  this->SetSize(wxSize(0,0));
  this->SetSize(wxSize(wymiar,wymiar));

  if(kto_zaczyna == 0){
    minimax(*plansza, 15/plansza->rozmiar()<2 ? 2 : 15/plansza->rozmiar(), 1);
    aktualizuj_plansze();
  }
}
