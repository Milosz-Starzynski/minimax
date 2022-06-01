#pragma once
#include<exception>
class Plansza{
  unsigned rozmiar_planszy, w_rzedzie;
  int **_plansza;

  public:
  int tura=-1, _i,_j;
  Plansza(unsigned n=3, unsigned k=3) : rozmiar_planszy(n), w_rzedzie(k){
    _plansza = new int*[n];
    for(int i = 0; i<n; ++i)
      _plansza[i] = new int[n];
    
    for(int i=0; i<n; ++i)
      for(int j=0; j<n; ++j)
	_plansza[i][j] = 0;
  }

  Plansza(Plansza &plansza) : Plansza(plansza.rozmiar_planszy, plansza.w_rzedzie){
    for(int i=0; i<rozmiar_planszy; ++i)
      for(int j=0; j<rozmiar_planszy; ++j)
	_plansza[i][j] = plansza(i,j);
  }
    
  
  ~Plansza(){
    for(int i = 0; i<rozmiar_planszy; ++i)
      delete [] _plansza[i];
    delete [] _plansza;
  }

  unsigned rozmiar() {return rozmiar_planszy;}
  unsigned ile_w_rzedzie() {return w_rzedzie;}
  unsigned k() {return w_rzedzie;}

  int& operator()(unsigned i, unsigned j) {return _plansza[i][j];}

  Plansza& operator=(Plansza& plansza){
    for(int i = 0; i<rozmiar_planszy; ++i)
      delete [] _plansza[i];
    delete [] _plansza;
    
    _plansza = new int*[plansza.rozmiar_planszy];
    for(int i = 0; i<plansza.rozmiar_planszy; ++i)
      _plansza[i] = new int[plansza.rozmiar_planszy];
    
    rozmiar_planszy = plansza.rozmiar_planszy; w_rzedzie = plansza.w_rzedzie;
    for(int i=0; i<rozmiar_planszy; ++i)
      for(int j=0; j<rozmiar_planszy; ++j){
	if(_plansza[i][j] != plansza(i, j)){
	  _i = i; _j = j;
	}
	_plansza[i][j] = plansza(i,j);
      }
    return *this;
  }

  bool sprawdz_zwyciestwo(int gracz);
  bool sprawdz_koniec();
};
