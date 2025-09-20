// quadrato disegnato programma
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
RenderWindow window(sf::VideoMode(400,400),"Tetris");
RectangleShape quadrato(Vector2f(20,20));

Text textScrittaTetris,textScrittaTime,textVariabileTime,textScrittaLevel,textVariabileLevel,textScrittaScore,textVariabileScore,textScrittaGameover;
Font font;

float tempo_secondi;
float tempo_caduta_tetramino=1000.0;
float tempo_scatto_livello=10.0;
int score=0,livello=0;

int Vettore_CoordinateX[4], Vettore_CoordinateY[4],Vettore_CoordX_Preced[4],Vettore_CoordY_Preced[4];
int i, numero_colore;
int Matrice_tetramini_collocati[20][10]={0};
bool gameover=0;

int Matrice_Coordinate_Iniziali_Tetramini[7][4][2]=
{
	20,20,//S
	0,40,
	20,40,
	0,60,
	
	20,20,//T
	0,40,
	20,40,
	20,60,
	
	20,0,//I
	20,20,
	20,40,
	20,60,
	
	20,20,//J
	20,40,
	0,60,
	20,60,
	
	0,20,//L
	20,20,
	20,40,
	20,60,
	
	0,20,//Z
	0,40,
	20,40,
	20,60,
	
	0,20,//O
	20,20,
	0,40,
	20,40,
};

//Funzione nuovo tetremino
void nuovo_tetramino()
{
	i=rand()%7;
	numero_colore=i+1;
	
	for(int n=0;n<4;n++)
	{
		Vettore_CoordinateX[n]=Matrice_Coordinate_Iniziali_Tetramini[i][n][0]+80;
		
		Vettore_CoordinateY[n]=Matrice_Coordinate_Iniziali_Tetramini[i][n][1];
	}
}

//Funzione collisione
bool collisione(void)
{
	for(int n=0;n<4;n++)
	
	if (Vettore_CoordinateX[n]<0||Vettore_CoordinateX[n]>=200||Vettore_CoordinateY[n]>=400) return 1;
	//collisione con un altro tetramino già collocato
	else if (Matrice_tetramini_collocati[Vettore_CoordinateY[n]/20][Vettore_CoordinateX[n]/20]&&Vettore_CoordinateY[n]/20>=0) return 1;
	return 0;
}

//Funzione(sottoprogramma) per la gestione del movimento orizontale
void Movimento_Orizzontale(int DELTAX)
{
	for(int n=0;n<4;n++)
	{
		//prima di spostare salviamo le coordinate x e y
		Vettore_CoordX_Preced[n]=Vettore_CoordinateX[n];
		Vettore_CoordY_Preced[n]=Vettore_CoordinateY[n];
		//spostiamo a destra o sinistra
		Vettore_CoordinateX[n]=Vettore_CoordinateX[n]+=DELTAX;
	}
	//se il blocco è usciot a destra o sinistra rimettilo dov'era prima
	if(collisione()) 
		for(int n=0;n<4;n++)
		{
		Vettore_CoordinateX[n]=Vettore_CoordX_Preced[n];
		Vettore_CoordinateY[n]=Vettore_CoordY_Preced [n];	
		}
}

//Funzione controlla se riga da cancellare
void controlla_riga()
{
    int n=19; 
	//scansiona Matrice_tetramini_collocati riga per riga partendo dal basso
    for(int i=19;i>=0;i--)
    {
        //INIZIO FOR ESTERNO
        bool vuoto=0; 
		//scansiona la riga
        for(int j=0;j<10;j++)
        {
            //INIZIO FOR INTERNO
            if(!Matrice_tetramini_collocati[i][j]) vuoto=1; //se c'è un elemento vuoto poni flag vuoto=1
            Matrice_tetramini_collocati[n][j]=Matrice_tetramini_collocati[i][j];
        }
        //FINE FOR INTERNO
        if(vuoto) 
			n--;
		else score=score+100;
        
    }
    //FINE FOR ESTERNO
}

//Funzione(sottoprogramma) per la gestione del movimento verticale
void Movimento_Verticale(void)
{
	for(int n=0;n<4;n++)
	{
		//prima di spostare salviamo le coordinate x e y
		Vettore_CoordX_Preced[n]=Vettore_CoordinateX[n];
		Vettore_CoordY_Preced[n]=Vettore_CoordinateY[n];
		//sposta in basso di 20 pixel
		Vettore_CoordinateY[n]=Vettore_CoordinateY[n]+=20;
	}
	if (collisione())//se tocca il fonto genera un nuovo tetramino o tocca un altro tetramino già collocato
	{
		for(int n=0;n<4;n++)
		{
			Matrice_tetramini_collocati[Vettore_CoordY_Preced[n]/20][Vettore_CoordX_Preced[n]/20]=numero_colore;
			//se dopo il movimento verticale c'è una collisione e se una delle coordinate Y dei quadrati del tetramino è negativa vuol dire ho superato il limite superiore quindi game over
			if(Vettore_CoordY_Preced[n]<0)
			gameover=1;
		}

		controlla_riga();
		nuovo_tetramino();
	}
}

//Funzione(sottoprogramma) rotazione
void Rotazione(void)
{
    for(int n=0;n<4;n++)
    {
        // prima di ruotare salviamo le coordinate X e Y
        Vettore_CoordX_Preced[n]=Vettore_CoordinateX[n];
        Vettore_CoordY_Preced[n]=Vettore_CoordinateY[n];
    }

    int centro_rot_x=Vettore_CoordinateX[2]; 
    int centro_rot_y=Vettore_CoordinateY[2];

    for(int n=0;n<4;n++)
    {
        int a=Vettore_CoordinateX[2];
        int b=Vettore_CoordinateY[2];

        int coord_x_prec=Vettore_CoordinateX[n];
        int coord_y_prec=Vettore_CoordinateY[n];

        Vettore_CoordinateX[n]=coord_y_prec-b+a;
        Vettore_CoordinateY[n]=-coord_x_prec+a+b;
    }

    // se il blocco ruotando è uscito a destra o sinistra rimettilo dov'era prima
    if(collisione())
    {
        for(int n=0;n<4;n++)
        {
            Vettore_CoordinateX[n]=Vettore_CoordX_Preced[n];
            Vettore_CoordinateY[n]=Vettore_CoordY_Preced[n];
        }
    }
}

//Funzione(sottoprogramma) disegna tetramino in movimento
void disegna_tetramino_in_movimento()
{
	for(int n=0;n<4;n++)
	{
		switch(numero_colore)
		{
			case 1:quadrato.setFillColor(Color::Blue);
			break;
			case 2:quadrato.setFillColor(Color::Red);
			break;
			case 3:quadrato.setFillColor(Color::Green);
			break;
			case 4:quadrato.setFillColor(Color::Yellow);
			break;
			case 5:quadrato.setFillColor(Color::Magenta);
			break;
			case 6:quadrato.setFillColor(Color::Cyan);
			break;
			case 7:quadrato.setFillColor(Color::White);
			break;
		}
		
		quadrato.setOutlineColor(Color::Black);
		quadrato.setOutlineThickness(1.f);
		
		quadrato.setPosition(Vector2f(Vettore_CoordinateX[n],Vettore_CoordinateY[n]));
		window.draw(quadrato);
	}
}

//Funzione(sottoprogramma) disegna tetramini collocati
void disegna_tetramini_collocati()
{
	for(int i=0;i<20;i++)
		for(int j=0;j<10;j++)
		{
			if (Matrice_tetramini_collocati[i][j]==0) continue;
			switch(Matrice_tetramini_collocati[i][j])
			{
				case 1:quadrato.setFillColor(Color::Blue);
				break;
				case 2:quadrato.setFillColor(Color::Red);
				break;
				case 3:quadrato.setFillColor(Color::Green);
				break;
				case 4:quadrato.setFillColor(Color::Yellow);
				break;
				case 5:quadrato.setFillColor(Color::Magenta);
				break;
				case 6:quadrato.setFillColor(Color::Cyan);
				break;
				case 7:quadrato.setFillColor(Color::White);
				break;
			}
		
			quadrato.setOutlineColor(Color::Black);
			quadrato.setOutlineThickness(1.f);
		
			quadrato.setPosition(Vector2f(j*20,i*20));
			window.draw(quadrato);
		}
}

//Funzione scritte
void scritte_statiche()
{
	font.loadFromFile("arial1.ttf");
	//scritta statica Tetris
	textScrittaTetris.setFont(font);
	textScrittaTetris.setCharacterSize(60);
	textScrittaTetris.setFillColor(Color::Red);
	textScrittaTetris.setOutlineColor(Color::Yellow);
	textScrittaTetris.setOutlineThickness(5);
	textScrittaTetris.setString("Tetris");
	textScrittaTetris.setPosition(220,0);
	
	//scritta statica Time
	textScrittaTime.setFont(font);
	textScrittaTime.setCharacterSize(30);
	textScrittaTime.setStyle(Text::Regular);
	textScrittaTime.setFillColor(Color::Green);
	textScrittaTime.setString("Time");
	textScrittaTime.setPosition(250,100);
	
	//settaggi per Variabile Time
	textVariabileTime.setFont(font);
	textVariabileTime.setCharacterSize(25);
	textVariabileTime.setStyle(Text::Regular);
	textVariabileTime.setFillColor(Color::Green);
	textVariabileTime.setPosition(300,130);
	
	//scritta statica Level
	textScrittaLevel.setFont(font);
	textScrittaLevel.setCharacterSize(30);
	textScrittaLevel.setStyle(Text::Regular);
	textScrittaLevel.setFillColor(Color::Blue);
	textScrittaLevel.setString("Level");
	textScrittaLevel.setPosition(250,200);
	
	//settaggi per Variabile Level
	textVariabileLevel.setFont(font);
	textVariabileLevel.setCharacterSize(25);
	textVariabileLevel.setStyle(Text::Regular);
	textVariabileLevel.setFillColor(Color::Blue);
	textVariabileLevel.setPosition(300,230);
	
	//scritta statica Score
	textScrittaScore.setFont(font);
	textScrittaScore.setCharacterSize(30);
	textScrittaScore.setStyle(Text::Regular);
	textScrittaScore.setFillColor(Color::Magenta);
	textScrittaScore.setString("Score");
	textScrittaScore.setPosition(250,290);
	
	//settaggi per Variabile Score
	textVariabileScore.setFont(font);
	textVariabileScore.setCharacterSize(25);
	textVariabileScore.setStyle(Text::Regular);
	textVariabileScore.setFillColor(Color::Magenta);
	textVariabileScore.setPosition(300,320);
	
	//scritta statica Gameover
	textScrittaGameover.setFont(font);
	textScrittaGameover.setCharacterSize(80);
	textScrittaGameover.setFillColor(Color::Yellow);
	textScrittaGameover.setOutlineColor(Color::Red);
	textScrittaGameover.setOutlineThickness(5);
	textScrittaGameover.setString("Gameover");
	textScrittaGameover.setPosition(13,150);
}

void scritte_variabili()
{
	char Variabileinformatostringa[10];
	//scritta Variabile Time
	itoa(tempo_secondi,Variabileinformatostringa,10);
	textVariabileTime.setString(Variabileinformatostringa);
	//scritta Variabile level
	itoa(livello,Variabileinformatostringa,10);
	textVariabileLevel.setString(Variabileinformatostringa);
	//scritta Variabile score
	itoa(score,Variabileinformatostringa,10);
	textVariabileScore.setString(Variabileinformatostringa);
}

//Programma princpale main
int main()
{
	Music music;
	music.openFromFile("musica.wav");
	music.play();
	music.setLoop(true);
	
	//per importare lo sfondo
	Texture texture1;
	texture1.loadFromFile("sfondo.png");
	Sprite sfondo(texture1);
	scritte_statiche();
	nuovo_tetramino();	
	Clock clocktetris;
	float tempo_precedente=clocktetris.getElapsedTime().asMilliseconds();
	float tempo_precedente_liv=clocktetris.getElapsedTime().asSeconds();
	
	while(window.isOpen())
	{
		if(tempo_caduta_tetramino<100.0)
		{
			tempo_caduta_tetramino=100.0;
			livello=10;
		}
		if((clocktetris.getElapsedTime().asMilliseconds()-tempo_precedente)>=tempo_caduta_tetramino)
		{
			tempo_precedente=clocktetris.getElapsedTime().asMilliseconds();
			Movimento_Verticale();
		}
		tempo_secondi=clocktetris.getElapsedTime().asSeconds();
		if (clocktetris.getElapsedTime().asSeconds()-tempo_precedente_liv>tempo_scatto_livello)
		{
			tempo_caduta_tetramino-=100;
			livello++;
			tempo_precedente_liv=clocktetris.getElapsedTime().asSeconds();
		}
		
		Event event;
		while(window.pollEvent(event))
		{
			if(event.type==Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
			if (event.key.code==Keyboard::Left) Movimento_Orizzontale(-20);
			else if (event.key.code==Keyboard::Right)Movimento_Orizzontale(20);
			else if (event.key.code==Keyboard::Up)Rotazione();
			else if (event.key.code==Keyboard::Down)Movimento_Verticale();
		}
		
	scritte_variabili();
	if(!gameover)
	{
		window.clear(Color::Black);
		window.draw(sfondo);
		window.draw(textScrittaTetris);
		window.draw(textScrittaTime);
		window.draw(textVariabileTime);
		window.draw(textScrittaLevel);
		window.draw(textVariabileLevel);
		window.draw(textScrittaScore);
		window.draw(textVariabileScore);
		disegna_tetramini_collocati();
		disegna_tetramino_in_movimento();
	}
	else window.draw(textScrittaGameover);
	
	window.display();
	}
}
