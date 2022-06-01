#include "priority_queue.hh"
#include "plansza.hh"
#include <limits>

int sprawdz_przekatna_lp(int i0, int j0, int dlugosc, int k, int gracz, int &najdluzszy, Plansza &plansza){
  int dobre, puste, punkty=0;
  for(int i=i0, j=j0; dlugosc; --dlugosc, ++i, ++j){
    dobre = puste = 0;
    for(int l=0; l<k && i+l<i0+dlugosc && j+l<j0+dlugosc; ++l){
      switch(gracz*plansza(i+l,j+l)){
      case 0: ++puste; break;
      case 1: ++dobre; break;
      case -1: ++j; l=dobre=0; break;
      }
    }
    if(dobre+puste < k) dobre = 0;
    if(dobre > najdluzszy) najdluzszy = dobre;
    punkty += dobre;
  }
  return punkty;
}

int sprawdz_przekatna_pl(int i0, int j0, int dlugosc, int k, int gracz, int &najdluzszy, Plansza &plansza){
  int dobre, puste, punkty=0;
  for(int i=i0, j=j0; dlugosc; --dlugosc, ++i, --j){
    dobre = puste = 0;
    for(int l=0; l<k && i+l<i0+dlugosc && j-l>=0; ++l){
      switch(gracz*plansza(i+l,j-l)){
      case 0: ++puste; break;
      case 1: ++dobre; break;
      case -1: --j; l=dobre=0; break;
      }
    }
    if(dobre+puste < k) dobre = 0;
    if(dobre > najdluzszy) najdluzszy = dobre;
    punkty += dobre;
  }
  return punkty;
}
int punkty_heurystyczne(Plansza &plansza, int gracz){
  int punkty=0, puste=0, dobre=0, najdluzszy=0;
  int n = plansza.rozmiar(), k = plansza.ile_w_rzedzie();

  for(int i=0; i<n; ++i)
    for(int j=0; j<n-k+1; ++j){
      dobre = puste = 0;
      for(int l=0; l<k && j+l<n; ++l){
	switch(gracz*plansza(i,j+l)){
	case 0: ++puste; break;
	case 1: ++dobre; break;
	case -1: ++j; l=dobre=0; break;
	}
      }
      if(dobre+puste < k) dobre = 0;
      if(dobre > najdluzszy) najdluzszy = dobre;
      punkty += dobre;
    }
  dobre=puste=0;
  for(int j=0; j<n; ++j)
    for(int i=0; i<n-k+1; ++i){
      dobre = puste = 0;
      for(int l=0; l<k && i+l<n && j<n; ++l){
	
	switch(gracz*plansza(i+l, j)){
	case 0: ++puste; break;
	case 1: ++dobre; break;
	case -1: ++j; l=dobre=0; break;
	}
      }
      if(dobre+puste < k) dobre = 0;
      if(dobre > najdluzszy) najdluzszy = dobre;
      punkty += dobre;
    }
  
  for(int i=0; i<n-k+1; ++i)
    punkty += sprawdz_przekatna_lp(i, 0, n-i, k, gracz, najdluzszy, plansza);
  for(int j=1; j<n-k+1; ++j)
    punkty += sprawdz_przekatna_lp(0, j, n-j, k, gracz, najdluzszy, plansza);
  for(int i=0; i<n-k+1; ++i)
    punkty += sprawdz_przekatna_pl(i, n-1, n-i, k, gracz, najdluzszy, plansza);
  for(int j=n-2; j>=k-1; --j)
    punkty += sprawdz_przekatna_pl(0, j, j+1, k, gracz, najdluzszy, plansza);
  punkty += 5*najdluzszy;

  return punkty;
}

int funkcja_heurystyczna(Plansza &plansza, int glebokosc, int gracz){
  if(plansza.sprawdz_zwyciestwo(1))
    return (500 + glebokosc);
  if(plansza.sprawdz_zwyciestwo(-1))
    return -(500 + glebokosc);

  int punkty = punkty_heurystyczne(plansza, 1);
  int negpunkty = punkty_heurystyczne(plansza, -1);

  return (punkty-negpunkty+gracz*glebokosc);
}

PriorityQueue <Plansza> generuj_ruchy(Plansza &plansza, int gracz){
  PriorityQueue<Plansza> lista_ruchow;
  int n = plansza.rozmiar(), k = plansza.ile_w_rzedzie();
  
  for(int i=0; i<n; ++i)
    for(int j=0; j<n; ++j)
      if(plansza(i,j) == 0){
	plansza(i,j) = gracz;	
	lista_ruchow.insert(plansza, -gracz*funkcja_heurystyczna(plansza, 0, gracz));
	plansza(i,j) = 0;
      }
  return lista_ruchow;
}

int max(int a, int b) { return a>b ? a : b;}

int minimax(Plansza &plansza, int glebokosc, int gracz, bool czy_rekurencyjne_wywolanie=false){
  if(glebokosc == 0 || plansza.sprawdz_koniec()){
    return gracz*funkcja_heurystyczna(plansza, glebokosc, gracz);
     }

  PriorityQueue<Plansza> mozliwe_ruchy = generuj_ruchy(plansza, gracz);
  int wartosc = std::numeric_limits<int>::lowest(), max_wartosc = wartosc;
  Plansza wybrany_ruch;

  while(!mozliwe_ruchy.empty()){
    Plansza mozliwa_sytuacja = mozliwe_ruchy.min(); mozliwe_ruchy.removeMin();

    wartosc = max(wartosc, -minimax(mozliwa_sytuacja, glebokosc-1, -gracz, true));
    
    if(wartosc > max_wartosc){
      max_wartosc = wartosc;
      wybrany_ruch = mozliwa_sytuacja;
    }
  }

  if(!czy_rekurencyjne_wywolanie){
    plansza = wybrany_ruch;
  }

  return wartosc;
}

/*int main(){
  int n=5, k=5;
  Plansza plansza(n, k);
  int zadane[5][5] = {{-1,  0,  0,  0, -1},
		      { 0,  0,  0,  0,  0},
		      {-1,  0, -1,  1,  0},
		      { 1,  0,  0,  1,  1},
		      {-1,  0,  0,  0,  0}};
  
  

  //for(int i=0; i<n; ++i){
  //for(int j=0; j<n; ++j)
  //  plansza(i,j) = zadane[i][j];
  //  }

std::cout << funkcja_heurystyczna(plansza, 0) << "\n";
//minimax(plansza, 4, std::numeric_limits<int>::max(), std::numeric_limits<int>::lowest(), -1);
while(1 && !plansza.sprawdz_koniec()){
    minimax(plansza, 5, std::numeric_limits<int>::max(), std::numeric_limits<int>::lowest(), 1);
    std::cout << "\n";
    for(int i=0; i<n; ++i){
      for(int j=0; j<n; ++j){
	switch(plansza(i,j)){
	case 0:
	  std::cout << "-" << " "; break;
	case 1:
	  std::cout << "X" << " "; break;
	case -1:
	  std::cout << "O" << " "; break;
	}
      }
      std::cout << "\n";
    }
    //std::cout << funkcja_heurystyczna(plansza, 0) << "\n";
    int i, j;
    std::cout << "Wykonaj ruch: ";
    std::cin >> i >> j;
    plansza(i,j) = -1;
    //minimax(plansza, 4, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), 1);
    
    std::cout << "____________________________________________________\n***************************************************\n";
  }
  for(int i=0; i<n; ++i){
    for(int j=0; j<n; ++j){
      switch(plansza(i,j)){
      case 0:
	std::cout << "-" << " "; break;
      case 1:
	std::cout << "X" << " "; break;
      case -1:
	std::cout << "O" << " "; break;
      }
    }
    std::cout << "\n";
    }
    return 0;
}

*/
