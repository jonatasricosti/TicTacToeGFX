#include <SDL.h>


SDL_Event evento;
SDL_Surface *tela = NULL;
bool executando = true;

const int screen_width = 640;
const int screen_height = 480;
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

// use essa fun��o pra carregar arquivos
// nota: essa fun��o s� deve ser chamada no come�o do programa
void LoadFiles()
{
    vLine = SDL_LoadBMP("gfx/linhav.bmp");
    hLine = SDL_LoadBMP("gfx/linhah.bmp");
}


// use essa fun��o pra fechar arquivos
// nota: essa fun��o s� deve ser chamada no final do programa
void CloseFiles()
{
    SDL_FreeSurface(vLine);
    SDL_FreeSurface(hLine);
}

// essa fun��o desenha a o tabuleiro e tira uma foto da tela
void DrawBoard()
{
    int margem = 120;

    DrawImage(100+margem,0,vLine);
    DrawImage(screen_width-100-margem,0,vLine);

    DrawImage(0,10+margem,hLine);
    DrawImage(0,400-10-margem,hLine);

    SDL_SaveBMP(tela, "gfx/board.bmp");
}

int main(int argc, char*args[])
{
SDL_Init(SDL_INIT_EVERYTHING);
tela = SDL_SetVideoMode(screen_width,screen_height,screen_bpp,SDL_SWSURFACE);
LoadFiles();

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
