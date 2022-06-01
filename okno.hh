#pragma once
#include <wx/wx.h>
#include <wx/display.h>
#include <wx/popupwin.h>
#include <wx/numdlg.h>
#include <wx/spinctrl.h>
#include "plansza.hh"

class App : public wxApp
{
public:
  virtual bool OnInit();
};

class Okno : public wxFrame
{
public:
  Okno();
  ~Okno();
  void PrzyWcisnieciu(wxCommandEvent &zdarzenie);
  void ustawienia(wxCommandEvent& zdarzenie);
  void aktualizuj_plansze();
  void wyczysc_plansze(wxCommandEvent& zdarzenie);
  wxButton** przyciski = nullptr;
  Plansza *plansza = new Plansza(4,4);
  int kto_zaczyna = 1;

  wxDECLARE_EVENT_TABLE();
};

