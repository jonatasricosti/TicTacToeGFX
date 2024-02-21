#include <SDL.h>


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

// use essa fun��o pra carregar uma imagem.bmp e deixa o fundo transparente
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

// use essa fun��o pra desenhar uma imagem na tela
void DrawImage(int x, int y, SDL_Surface *image)
{
    SDL_Rect mover;
    mover.x = x;
    mover.y = y;

    SDL_BlitSurface(image, NULL, tela, &mover);
}


SDL_Surface *vLine = NULL;
SDL_Surface *hLine = NULL;
SDL_Surface *xImage = NULL;
SDL_Surface *oImage = NULL;

// use essa fun��o pra carregar arquivos
// nota: essa fun��o s� deve ser chamada no come�o do programa
void LoadFiles()
{
    vLine = SDL_LoadBMP("gfx/linhav.bmp");
    hLine = SDL_LoadBMP("gfx/linhah.bmp");
    xImage = fundo_transparente("gfx/x.bmp", 0,255,255);
    oImage = fundo_transparente("gfx/o.bmp", 0,255,255);
}


// use essa fun��o pra fechar arquivos
// nota: essa fun��o s� deve ser chamada no final do programa
void CloseFiles()
{
    SDL_FreeSurface(vLine);
    SDL_FreeSurface(hLine);
    SDL_FreeSurface(xImage);
    SDL_FreeSurface(oImage);
}

// esse array reprenta o tabuleiro
char board[9] = {'X','X','O','O','X','X','X','O','O'};;

// essa fun��o reseta o jogo
void ResetGame()
{
	for(int i = 0; i < 9; i++)
	{
		board[i] = 0;
	}
}

// use essa fun��o pra desenhar 'X' ou o 'O'
void DrawX_and_O()
{
    // esse c�digo desenha o 'X' ou o 'O' se tiver o valor deles no array board
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

// essa fun��o desenha o tabuleiro
void DrawBoard()
{
    // desenha o tabuleiro
    DrawImage(vLine->h/3,0,vLine);
    DrawImage(2*vLine->h/3,0,vLine);
    DrawImage(0,hLine->w/3,hLine);
    DrawImage(0,2*hLine->w/3,hLine);

}

int main(int argc, char*args[])
{
SDL_Init(SDL_INIT_EVERYTHING);
tela = SDL_SetVideoMode(screen_width,screen_height,screen_bpp,SDL_SWSURFACE);
LoadFiles();

//ResetGame();

// game loop
while(executando)
{
    start = SDL_GetTicks();
    while(SDL_PollEvent(&evento))
    {
        // clicou no x da janela
        if(evento.type == SDL_QUIT)
        {
            executando = false; // fecha o programa
        }
    }

    SDL_FillRect(tela, 0, 0xffffff);

    DrawX_and_O();
    DrawBoard();

    SDL_Flip(tela);
    if(framerate > (SDL_GetTicks()-start))
    {
        SDL_Delay(framerate - (SDL_GetTicks()-start));
    }
}


CloseFiles();
SDL_Quit();
return 0;
}
