#include "plansza.hh"
#include <iostream>
bool Plansza::sprawdz_zwyciestwo(int gracz){
  int ile_w_rzedzie = 0, n = rozmiar_planszy, k = w_rzedzie;
  for(int i=0; i<n; ++i)
    for(int j=0; j<n; ++j){
      while(_plansza[i][j] == gracz){
	++ile_w_rzedzie; ++j;
	if(j>=n) break;
      }
      if(ile_w_rzedzie >= k) return true;
      else ile_w_rzedzie = 0;
    }

  for(int j=0; j<n; ++j)
    for(int i=0; i<n; ++i){
      while(_plansza[i][j] == gracz){
	++ile_w_rzedzie; ++i;
	if(i>=n) break;
      }
      if(ile_w_rzedzie >= k) return true;
      else ile_w_rzedzie = 0;
    }

  int i, j;
  for(int i0=0; i0<n-k+1;++i0){
    i = i0;
    for(j=0; i<n; ++i, ++j){
      while(_plansza[i][j] == gracz){
	++ile_w_rzedzie; ++j; ++i;
	if(i>=n) break;
      }
      if(ile_w_rzedzie >= k) return true;
      else ile_w_rzedzie = 0;
    }
  }
  for(int j0=1; j0<n-k+1;++j0){
    j=j0;
    for(i=0; j<n; ++i, ++j){
      while(_plansza[i][j] == gracz){
	++ile_w_rzedzie; ++j; ++i;
	if(j>=n) break;
      }
      if(ile_w_rzedzie >= k) return true;
      else ile_w_rzedzie = 0;
    }
  }

  for(int i0=n-1; i0>n-k+1;--i0){
    i=i0;
    for(j=0; i>=0 && j<n; --i, ++j){
      while(_plansza[i][j] == gracz){
	++ile_w_rzedzie; ++j; --i;
	if(i<0) break;
      }
      if(ile_w_rzedzie >= k) return true;
      else ile_w_rzedzie = 0;
    }
  }
  for(int j0=n-1; j0>n-k+1;--j0){
    j=j0;
    for(i=0; j>=0 && i<n; --j, ++i){
      while(_plansza[i][j] == gracz){
	++ile_w_rzedzie; ++i; --j;
	if(j<0) break;
      }
      if(ile_w_rzedzie >= k) return true;
      else ile_w_rzedzie = 0;
    }
  }
  return false;
}

bool Plansza::sprawdz_koniec(){
  if(sprawdz_zwyciestwo(1) || sprawdz_zwyciestwo(-1))
    return true;
  for(int i=0; i<rozmiar_planszy; ++i)
    for(int j=0; j<rozmiar_planszy; ++j)
      if(_plansza[i][j] == 0) return false;
  return true;
}

/*int main(){
  Plansza plansza(5,3);
  int zadana[5][5] = {{0, 0, 0, 0, 0},
		      {0, 0, 0, 0, 0},
		      {0, 0, 1, 0, 0},
		      {0, 1, 0, 0, 0},
		      {1, 0, 0, 0, 0}};
  for(int i=0; i<5; ++i)
    for(int j=0; j<5; ++j)
      plansza(i,j) = zadana[i][j];

  std::cout << plansza.sprawdz_zwyciestwo(1) << "\n";
  return 0;
}
*/
