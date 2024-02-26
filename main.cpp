#include <SDL.h>
#include <SDL_ttf.h>


SDL_Event evento;
SDL_Surface *tela = NULL;
bool executando = true;

const int screen_width = 480;
const int screen_height = 640;
const int screen_bpp = 32;

// para o framerate
Uint32 start = 0;
const int fps = 30;
const int framerate =  1000/fps;

// use essa função pra carregar uma imagem.bmp e deixa o fundo transparente
SDL_Surface *fundo_transparente(const char *filename, Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_Surface *load = NULL;
    SDL_Surface *otimizado = NULL;

    load = SDL_LoadBMP(filename);

    if(load != NULL)
    {
        otimizado = SDL_DisplayFormat(load);
        SDL_FreeSurface(load);

        if(otimizado != NULL)
        {
            SDL_SetColorKey(otimizado, SDL_SRCCOLORKEY, SDL_MapRGB(otimizado->format, red, green, blue));
        }
    }

    return otimizado;
}

// use essa função pra desenhar uma imagem na tela
void DrawImage(int x, int y, SDL_Surface *image)
{
    SDL_Rect mover;
    mover.x = x;
    mover.y = y;

    SDL_BlitSurface(image, NULL, tela, &mover);
}

// use essa função pra desenhar uma imagem cortada na tela
// nota: os valores de corte você pode pegar no gimp
void DrawCutImage(int x, int y, int cx, int cy, int cw, int ch, SDL_Surface *image)
{
    SDL_Rect mover;
    mover.x = x;
    mover.y = y;

    SDL_Rect corte;
    corte.x = cx;
    corte.y = cy;
    corte.w = cw;
    corte.h = ch;

    SDL_BlitSurface(image, &corte, tela, &mover);
}

// use essa função pra desenhar texto na tela usando fonte ttf
void DrawText(int x, int y, char *text, Uint8 red, Uint8 green, Uint8 blue, TTF_Font *font)
{
    SDL_Surface *buffer = NULL;
    SDL_Color color = {red, green, blue};

    buffer = TTF_RenderText_Solid(font, text, color);

    SDL_Rect mover;
    mover.x = x;
    mover.y = y;

    SDL_BlitSurface(buffer, NULL, tela, &mover);
    SDL_FreeSurface(buffer);
}

// use essa função pra desenhar um texto no centro da tela usando fonte ttf
void DrawTextCenter(int x, int y, char *text, Uint8 red, Uint8 green, Uint8 blue, TTF_Font *font)
{
    SDL_Surface *buffer = NULL;
    SDL_Color color = {red, green, blue};

    buffer = TTF_RenderText_Solid(font, text, color);

    SDL_Rect mover;
    mover.x = x+(screen_width - buffer->w)/2;
    mover.y = y+(screen_height - buffer->h)/2;

    SDL_BlitSurface(buffer, NULL, tela, &mover);
    SDL_FreeSurface(buffer);
}



SDL_Surface *vLine = NULL;
SDL_Surface *hLine = NULL;
SDL_Surface *xImage = NULL;
SDL_Surface *oImage = NULL;
SDL_Surface *ricosImage = NULL;

TTF_Font *ttfFile = NULL;

// use essa função pra carregar arquivos
// nota: essa função só deve ser chamada no começo do programa
void LoadFiles()
{
    vLine = SDL_LoadBMP("gfx/linhav.bmp");
    hLine = SDL_LoadBMP("gfx/linhah.bmp");
    xImage = fundo_transparente("gfx/x.bmp", 0,255,255);
    oImage = fundo_transparente("gfx/o.bmp", 0,255,255);

    ricosImage = fundo_transparente("gfx/ricos.bmp",0,255,255);

    ttfFile = TTF_OpenFont("fontes/times.ttf", 23);
}


// use essa função pra fechar arquivos
// nota: essa função só deve ser chamada no final do programa
void CloseFiles()
{
    SDL_FreeSurface(vLine);
    SDL_FreeSurface(hLine);
    SDL_FreeSurface(xImage);
    SDL_FreeSurface(oImage);
    TTF_CloseFont(ttfFile);
    SDL_FreeSurface(ricosImage);
}

// os estados de tela do jogo
enum state
{
	TURN_X,
	TURN_O,
	X_WIN,
	O_WIN,
	TIE,
};

int estado = TURN_X; // a variável estado muda a tela do jogo
int round = 0; // essa variável serve pra lógica de empate

char* status;

// esse array reprenta o tabuleiro
char board[9];

// use essa função pra colocar um 'X' ou 'O' no tabuleiro
// id posição do array Type = 'X' ou 'O'
void setCell(int id, char Type)
{
    if(id < 0 || id >=9) return;
    board[id] = Type;
    round++;
}

// essa função pega a posição x e y e usa na função setCell
void ButtonDown(int mx, int my)
{
    int id = mx/160;
    id = id + ((my/160)*3);

    // não deixa clicar fora da área do jogo
    if(mx > 480 || my > 480)
    {
        return;
    }

    if(board[id] != 0)
    {
        return;
    }

    if(estado == TURN_X)
    {
        setCell(id, 'X');
        estado = TURN_O;
    }

    else
    {
        setCell(id, 'O');
        estado = TURN_X;
    }
}


// programação do mouse
void mouse()
{
    int x,y;
    if(evento.type == SDL_MOUSEBUTTONDOWN)
    {
        // se apertou o botão esquerdo do mouse
        if(evento.button.button == SDL_BUTTON_LEFT)
        {
            x = evento.button.x;
            y = evento.button.y;

            ButtonDown(x,y);
        }
    }
}

// essa função verifica se o 'X' ganhou
void checkXWin()
{
	if(board[0] == 'X' && board[1] == 'X' && board[2] == 'X')
	{
		status = "Parabéns X ganhou na primeira linha horizontal!";
		estado = X_WIN;
		DrawCutImage(0,0,0,0,480,160,ricosImage);
	}

	if(board[3] == 'X' && board[4] == 'X' && board[5] == 'X')
	{
		status = "Parabéns X ganhou na segunda linha horizontal!";
		estado = X_WIN;
		DrawCutImage(0,160,0,0,480,160,ricosImage);
	}


	if(board[6] == 'X' && board[7] == 'X' && board[8] == 'X')
	{
		status = "Parabéns X ganhou na terceira linha horizontal!";
		estado = X_WIN;
		DrawCutImage(0,320,0,0,480,160,ricosImage);
	}

	if(board[0] == 'X' && board[3] == 'X' && board[6] == 'X')
	{
		status = "Parabéns X ganhou na primeira linha vertical!";
		estado = X_WIN;
		DrawCutImage(0,0,0,480,160,480,ricosImage);
	}


	if(board[1] == 'X' && board[4] == 'X' && board[7] == 'X')
	{
		status = "Parabéns X ganhou na segunda linha vertical!";
		estado = X_WIN;
		DrawCutImage(160,0,0,480,160,480,ricosImage);
	}


	if(board[2] == 'X' && board[5] == 'X' && board[8] == 'X')
	{
		status = "Parabéns X ganhou na terceira linha vertical!";
		estado = X_WIN;
		DrawCutImage(320,0,0,480,160,480,ricosImage);
	}

	// diagonais
	if(board[0] == 'X' && board[4] == 'X' && board[8] == 'X')
	{
		status = "Parabéns X ganhou na primeira diagonal!";
		estado = X_WIN;
		DrawCutImage(0,0,0,960,480,480,ricosImage);
	}

	if(board[2] == 'X' && board[4] == 'X' && board[6] == 'X')
	{
		status = "Parabéns X ganhou na segunda diagonal!";
		estado = X_WIN;
		DrawCutImage(0,0,0,1440,480,480,ricosImage);
	}
}


// essa função verifica se o 'O' ganhou
void checkOWin()
{
	if(board[0] == 'O' && board[1] == 'O' && board[2] == 'O')
	{
		status = "Parabéns O ganhou na primeira linha horizontal!";
		estado = O_WIN;
		DrawCutImage(0,0,0,0,480,160,ricosImage);
	}

	if(board[3] == 'O' && board[4] == 'O' && board[5] == 'O')
	{
		status = "Parabéns O ganhou na segunda linha horizontal!";
		estado = O_WIN;
		DrawCutImage(0,160,0,0,480,160,ricosImage);
	}

	if(board[6] == 'O' && board[7] == 'O' && board[8] == 'O')
	{
		status = "Parabéns O ganhou na terceira linha horizontal!";
		estado = O_WIN;
		DrawCutImage(0,0,0,480,160,480,ricosImage);
	}

	if(board[0] == 'O' && board[3] == 'O' && board[6] == 'O')
	{
		status = "Parabéns O ganhou na primeira linha vertical!";
		estado = O_WIN;
		DrawCutImage(160,0,0,480,160,480,ricosImage);
	}

	if(board[1] == 'O' && board[4] == 'O' && board[7] == 'O')
	{
		status = "Parabéns O ganhou na segunda linha vertical!";
		estado = O_WIN;
		DrawCutImage(320,0,0,480,160,480,ricosImage);
	}


	if(board[2] == 'O' && board[5] == 'O' && board[8] == 'O')
	{
		status = "Parabéns O ganhou na terceira linha vertical!";
		estado = O_WIN;
		DrawCutImage(0,0,0,960,480,480,ricosImage);
	}

	// diagonais
	if(board[0] == 'O' && board[4] == 'O' && board[8] == 'O')
	{
		status = "Parabéns O ganhou na primeira diagonal!";
		estado = O_WIN;
		DrawCutImage(0,0,0,960,480,480,ricosImage);
	}

	if(board[2] == 'O' && board[4] == 'O' && board[6] == 'O')
	{
		status = "Parabéns O ganhou na segunda diagonal!";
		estado = O_WIN;
		DrawCutImage(0,0,0,1440,480,480,ricosImage);
	}
}


// essa função verifica se teve empate
void checkTie()
{
	if(round >= 9 && estado != X_WIN && estado != O_WIN)
	{
		estado = TIE;
	}
}

// essa função reseta o jogo
void ResetGame()
{
	for(int i = 0; i < 9; i++)
	{
		board[i] = 0;
	}

	round = 0;
	estado = rand() % 2;
}

// use essa função pra desenhar 'X' ou o 'O'
void DrawX_and_O()
{
    // esse código desenha o 'X' ou o 'O' se tiver o valor deles no array board
    for(int i = 0; i < 9; i++)
    {
        if(board[i] == 'X')
        {
            DrawImage((i%3)*160, (i/3)*160, xImage);
        }

        if(board[i] == 'O')
        {
            DrawImage((i%3)*160, (i/3)*160, oImage);
        }
    }
}

// essa função desenha o tabuleiro
void DrawBoard()
{
    // desenha o tabuleiro
    DrawImage(160,0,vLine);
    DrawImage(320,0,vLine);
    DrawImage(0,160,hLine);
    DrawImage(0,320,hLine);
}

void DrawWin()
{
    if(estado == X_WIN)
    {
        DrawTextCenter(0,180,status,0,0,0,ttfFile);
    }

    else if(estado == O_WIN)
    {
        DrawTextCenter(0,180,status,0,162,232,ttfFile);
    }


    DrawTextCenter(0,200,"Aperte f pra resetar o jogo", 0,0,0,ttfFile);
    DrawTextCenter(0,220,"Aperte escape pra sair do jogo", 0,0,0,ttfFile);


    Uint8 * tecla = SDL_GetKeyState(NULL);
    if(tecla[SDLK_f])
    {
        ResetGame();
    }

    if(tecla[SDLK_ESCAPE])
    {
        executando = false;
    }
}

// essa função desenha o empate
void DrawTie()
{

	DrawTextCenter(0,180,"Oops parece que houve um empate",255,0,0, ttfFile);
    DrawTextCenter(0,200,"Aperte f pra resetar o jogo", 0,0,0,ttfFile);
    DrawTextCenter(0,220,"Aperte escape pra sair do jogo", 0,0,0,ttfFile);


    Uint8 * tecla = SDL_GetKeyState(NULL);
    if(tecla[SDLK_f])
    {
        ResetGame();
    }

    if(tecla[SDLK_ESCAPE])
    {
        executando = false;
    }
}

int main(int argc, char*args[])
{
SDL_Init(SDL_INIT_EVERYTHING);
tela = SDL_SetVideoMode(screen_width,screen_height,screen_bpp,SDL_SWSURFACE);

SDL_WM_SetCaption("Jogo da velha", NULL);


TTF_Init();
LoadFiles();

ResetGame();


// game loop
while(executando)
{
    start = SDL_GetTicks();
    while(SDL_PollEvent(&evento))
    {

        if(estado == TURN_X)
        {
            mouse();
        }

        else if(estado == TURN_O)
        {
            mouse();
        }

        // clicou no x da janela
        if(evento.type == SDL_QUIT)
        {
            executando = false; // fecha o programa
        }
    }

    SDL_FillRect(tela, 0, 0xffffff);

    // os estados do jogo
    switch(estado)
    {
        case TURN_X: DrawTextCenter(0,180,"Vez do X", 0,0,0,ttfFile); break;
        case TURN_O: DrawTextCenter(0,180,"Vez do O", 0,0,0,ttfFile); break;
        case X_WIN: DrawWin(); break;
        case O_WIN: DrawWin(); break;
        case TIE: DrawTie(); break;
    }

    DrawX_and_O();
    DrawBoard();
    checkXWin();
    checkOWin();
    checkTie();


    SDL_Flip(tela);
    if(framerate > (SDL_GetTicks()-start))
    {
        SDL_Delay(framerate - (SDL_GetTicks()-start));
    }
}


TTF_Quit();
CloseFiles();
SDL_Quit();
return 0;
}
