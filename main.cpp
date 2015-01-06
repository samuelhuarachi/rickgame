#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include "computer.h"
//#include "teclado.h"

int teclas_anteriores[KEY_MAX];

void keyboard_input()
{
   int i;

   for(i = 0; i < KEY_MAX; i++)
     teclas_anteriores[i] = key[i];

   poll_keyboard();
}

int apertou(int TECLA)
{
    return(teclas_anteriores[TECLA] == FALSE && key[TECLA] == TRUE);
}

int segurou(int TECLA)
{
    return(teclas_anteriores[TECLA] == TRUE && key[TECLA] == TRUE);
}

int soltou(int TECLA)
{
    return(teclas_anteriores[TECLA] == TRUE && key[TECLA] == FALSE);
}


volatile int exit_program;
void fecha_programa() { exit_program = TRUE; }
END_OF_FUNCTION(fecha_programa)

volatile int ticks;
void tick_counter() { ticks++; }
END_OF_FUNCTION(tick_counter)

volatile int milisegundos;
void msec_counter() { milisegundos++; }
END_OF_FUNCTION(msec_counter)


enum {SPLASHSCREEN, TITLESCREEN, MAINMENU, GAMESCREEN};
int screen_state;


const int TILESIZE = 32;
enum
{
  NADA = 0,
  GRAMA = 1
};


int** Carregar_Mapa(const char* nome_arquivo, int* linhas, int* colunas)
{
   FILE* f = fopen(nome_arquivo, "r");
   int** matriz;

   if(f != NULL)
   {
     int i, j;

     fscanf(f, "%d %d", linhas, colunas);

     //ALOCA O MAPA
     matriz = (int**) malloc ( (*linhas) * sizeof(int*));
     for(i = 0; i < *linhas; i++)
      matriz[i] = (int*) malloc( (*colunas) * sizeof(int));

     //CARREGA TILES
     for(i = 0; i < *linhas; i++)
     {
        for(j = 0; j < *colunas; j++)
        {
          fscanf(f, "%d", &matriz[i][j]);
        }
     }

     fclose(f);
   }

   return matriz;
}

void Desenhar_Mapa(BITMAP* buffer, int** mapa, int linhas, int colunas, BITMAP* grama, int posCenario)
{
    int i, j;

    for(i = 0; i < linhas; i++)
     {
        for(j = 0; j < colunas; j++)
        {
           //if(mapa[i][j] == NADA)
           //  rectfill(buffer, j * TILESIZE, i * TILESIZE, (j * TILESIZE) + TILESIZE, (i * TILESIZE) + TILESIZE, makecol(0,0,0));

           if(mapa[i][j] == GRAMA)
             //rectfill(buffer, j * TILESIZE, i * TILESIZE, (j * TILESIZE) + TILESIZE, (i * TILESIZE) + TILESIZE, makecol(255,0,0));
                draw_sprite_ex(buffer, grama, j * TILESIZE + posCenario, i * TILESIZE, DRAW_SPRITE_NORMAL, DRAW_SPRITE_NO_FLIP);

        }
     }
}

void Libera_Mapa(int** mapa, int linhas)
{
   int i;
   for(i = 0; i < linhas; i++)
    free(mapa[i]);

   free(mapa);
}


void init(){

    allegro_init();
    install_timer();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
    set_window_title("Tutorial 11 - Animacoes");

    exit_program = FALSE;
    LOCK_FUNCTION(fecha_programa);
    LOCK_VARIABLE(exit_program);
    set_close_button_callback(fecha_programa);

    ticks = 0;
    LOCK_FUNCTION(tick_counter);
    LOCK_VARIABLE(ticks);
    install_int_ex(tick_counter, BPS_TO_TIMER(60));

    milisegundos = 0;
    LOCK_FUNCTION(msec_counter);
    LOCK_VARIABLE(milisegundos);
    install_int_ex(msec_counter, MSEC_TO_TIMER(1));

    screen_state = SPLASHSCREEN;

}

void mainmenu(){
    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);

    int exit_screen = FALSE;
    int opcao = 1;

    ///GAME LOOP
    while(!exit_program && !exit_screen)
    {
        while(ticks > 0 && !exit_program)
        {
            keyboard_input();
            //INPUT
            if(apertou(KEY_ENTER) )
            {
                //Se voce clicou na opacao START GAME
                if(opcao == 1) {
                     exit_screen = TRUE;
                     screen_state = GAMESCREEN;
                }

                if(opcao == 3) {
                    exit_screen = TRUE;
                    screen_state = SPLASHSCREEN;
                }

                //Se voce clicou na opcao quit(SAIR)
                if(opcao == 4) {
                    fecha_programa();
                }
            }

            if(apertou(KEY_UP) ){
                opcao = opcao - 1;
            }
            if(apertou(KEY_DOWN) ){
                opcao = opcao + 1;
            }

            if(opcao > 4) {
                opcao = 1;
            }
            if(opcao < 1) {
                opcao = 4;
            }

            //INPUT
            if(apertou(KEY_ESC))
                fecha_programa();

            textout_centre_ex(buffer, font, "Main Menu", SCREEN_W/2, SCREEN_H/2 - 160, makecol(255,0,0), -1);

            if(opcao == 1) {
                textout_centre_ex(buffer, font, "Start Game", SCREEN_W/2, SCREEN_H/2 - 130, makecol(0,0,255), -1);
            } else {
                textout_centre_ex(buffer, font, "Start Game", SCREEN_W/2, SCREEN_H/2 - 130, makecol(0,0,0), -1);
            }

            if(opcao == 2) {
                textout_centre_ex(buffer, font, "Options", SCREEN_W/2, SCREEN_H/2 - 110, makecol(0,0,255), -1);
            } else {
                textout_centre_ex(buffer, font, "Options", SCREEN_W/2, SCREEN_H/2 - 110, makecol(0,0,0), -1);
            }

            if(opcao == 3) {
                textout_centre_ex(buffer, font, "Intro", SCREEN_W/2, SCREEN_H/2 - 90, makecol(0,0,255), -1);
            } else {
                textout_centre_ex(buffer, font, "Intro", SCREEN_W/2, SCREEN_H/2 - 90, makecol(0,0,0), -1);
            }

            if(opcao == 4) {
                textout_centre_ex(buffer, font, "Quit", SCREEN_W/2, SCREEN_H/2 - 70, makecol(0,0,255), -1);
            }else {
                textout_centre_ex(buffer, font, "Quit", SCREEN_W/2, SCREEN_H/2 - 70, makecol(0,0,0), -1);
            }



            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));
            ticks--;
        }
    }

    ///FINALIZAÇÂO
    destroy_bitmap(buffer);
}



int bounding_box_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    if( (x1 > x2 + w2) || (y1 > y2 + h2) || (x2 > x1 + w1) || (y2 > y1 + h1) )
     return FALSE;
    else
     return TRUE;



}


//Verifica se existe colisao com o mapa
int Verifica_Colisao_Mapa(BITMAP* buffer, int** mapa, int linhas, int colunas, int x, int y, int posCenario)
{
    int i, j;
    int colisao = FALSE;

    for(i = 0; i < linhas; i++)
     {
        for(j = 0; j < colunas; j++)
        {
            if(mapa[i][j] == GRAMA){
                if(bounding_box_collision(x, y, 30, 78, j * TILESIZE + posCenario, i * TILESIZE, 32 , 32 ) == TRUE){
                    colisao = TRUE;
                }
            }
        }
     }

     return colisao;
}



void gamescreen()
{

    computer compute;
    compute.setspeed ( 100 );


    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* link_dir = load_bitmap("rick2.bmp", NULL);
    BITMAP* grama = load_bitmap("images/grama.bmp", NULL);
    BITMAP* rickParado = load_bitmap("images/rick_parado.bmp", NULL);
    BITMAP* background1 = load_bitmap("images/fase1fundo.bmp", NULL);
    BITMAP* montanha = load_bitmap("images/montanha.bmp", NULL);




    ///Variáveis
    int num_frames = 6;
    int frame_w = link_dir->w / num_frames;
    int frame_atual;
    int tempo_troca = 120;

    int exit_screen = FALSE;

    int linhas, colunas;
    int** mapa = Carregar_Mapa("mapa.txt", &linhas, &colunas);

    int x1 = 30;
    int y1 = 150;
    int modo = DRAW_SPRITE_NORMAL;


    int posicaoInicial = 165;
    int aceleracaoGravidade = 5;
    int velocidadeInicial = 0;
    int tempo;
    int tempo_troca2 = 1000;
    int caindo = 1;
    int posicaoCenario = 0;

    int computerValue;

    ///GAME LOOP
    while(!exit_program && !exit_screen)
    {
        while(ticks > 0 && !exit_program)
        {

            //Background
            draw_sprite_ex(buffer, montanha, 0, 0, modo, DRAW_SPRITE_NO_FLIP);
            draw_sprite_ex(buffer, background1, 400 + posicaoCenario * 0.2, 300 , modo, DRAW_SPRITE_NO_FLIP);


            keyboard_input();

            //INPUT
            if(apertou(KEY_ENTER))
            {
                exit_screen = TRUE;
                screen_state = MAINMENU;
            }
            if(apertou(KEY_ESC))
                fecha_programa();

            //Desenhar o mapa
            Desenhar_Mapa(buffer, mapa, linhas, colunas, grama, posicaoCenario);


            //UPDATE
            frame_atual = (milisegundos / tempo_troca) % num_frames;
            tempo = milisegundos / tempo_troca2;
            //EfeitoGravidade();




            //Gravidade
            if(Verifica_Colisao_Mapa(buffer, mapa, linhas, colunas, x1, y1 + 3, posicaoCenario) == FALSE) {
                y1 += 3;
                caindo = 1;
            } else {
                caindo = 0;
            }


            //Movimentação
            if(segurou(KEY_SPACE) && caindo == 0 ){
                if(Verifica_Colisao_Mapa(buffer, mapa, linhas, colunas, x1, y1 - 70, posicaoCenario) == FALSE) {
                    y1 -= 70;
                }
            }



            if(key[KEY_LEFT]){
                if(Verifica_Colisao_Mapa(buffer, mapa, linhas, colunas, x1 - 3, y1, posicaoCenario) == FALSE) {
                    if (x1 <= 276 && posicaoCenario == 0) {
                        x1 -= 3;
                    } else {
                        posicaoCenario += 3;
                    }
                }
            }
            if(key[KEY_RIGHT]){
                if(Verifica_Colisao_Mapa(buffer, mapa, linhas, colunas, x1 + 3, y1, posicaoCenario) == FALSE) {
                    if (x1 < 276 && posicaoCenario == 0) {
                        x1 += 3;
                    }else {
                        posicaoCenario -= 3;
                    }
                }
            }



            draw_sprite_ex(buffer, rickParado, x1, y1, modo, DRAW_SPRITE_NO_FLIP);
            //Desenha circulo nos pes do personagem
            textprintf_ex(buffer, font, 100, 30, makecol(0,0,255), -1, "X: %d , Y: %d", x1, y1);
            textprintf_ex(buffer, font, 100, 45, makecol(0,0,255), -1, "tempo: %d", tempo);
            textprintf_ex(buffer, font, 100, 60, makecol(0,0,255), -1, "Distancia: %d ", posicaoCenario);

            computerValue = compute.readspeed();

            textprintf_ex(buffer, font, 100, 75, makecol(0,0,255), -1, "Computer: %d", computerValue);


            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));

            ticks--;
        }
    }

    ///FINALIZAÇÂO
    Libera_Mapa(mapa, linhas);
    destroy_bitmap(link_dir);
    destroy_bitmap(buffer);
    destroy_bitmap(grama);
    destroy_bitmap(rickParado);
    destroy_bitmap(background1);
    destroy_bitmap(montanha);


}


void splashscreen()
{

	///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    int exit_screen = FALSE;
    double contador = 0;


    ///GAME LOOP
    while(!exit_program && !exit_screen)
    {
        while(ticks > 0 && !exit_program)
        {
            keyboard_input();
            if(apertou(KEY_ENTER) || apertou(KEY_ESC))
            {
                exit_screen = TRUE;
                screen_state = MAINMENU;
            }

            contador = contador + 0.02;

			textout_centre_ex(buffer, font, "Nossa historia comeca o ano de 1910", SCREEN_W/2, SCREEN_H - (contador * 20), makecol(0,0,0), -1);
            textout_centre_ex(buffer, font, "A Terra esta um caos, e os extraterrestres estao prontos para atacar", SCREEN_W/2, SCREEN_H - (contador * 20) + 15, makecol(0,0,0), -1);
            textout_centre_ex(buffer, font, "Existe um homem que pode nos salvar", SCREEN_W/2, SCREEN_H - (contador * 20) + 30, makecol(0,0,0), -1);
            textout_centre_ex(buffer, font, "E o nome dele eh Chico Penteado", SCREEN_W/2, SCREEN_H - (contador * 20) + 45, makecol(0,0,0), -1);
            textout_centre_ex(buffer, font, "O Salvador da patria", SCREEN_W/2, SCREEN_H - (contador * 20) + 60, makecol(0,0,0), -1);

            draw_sprite(screen, buffer, 0, 0);
            clear_to_color(buffer, makecol(255,255,255));

            ticks--;
        }
    }

    ///FINALIZAÇÂO
    destroy_bitmap(buffer);
}


int main()
{




    init();

    ///GAME LOOP
    while(!exit_program)
    {
        if(screen_state == SPLASHSCREEN)
            splashscreen();
        else if(screen_state == MAINMENU)
            mainmenu();
        else if (screen_state == GAMESCREEN)
            gamescreen();
    }


    return 0;
}
END_OF_MAIN();
