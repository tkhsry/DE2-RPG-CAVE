

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// include sys
#include "altera_up_avalon_parallel_port.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
// #include "altera_up_avalon_character_lcd.h"
#include "sys/alt_stdio.h"
#include "sys/alt_timestamp.h"
#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_keyboard.h"
//#include "alt_types.h"
//#include "system.h"

// include my liblary
#include "de2sys/globals.h"
#include "de2sys/menu.h"


void init_check(alt_up_parallel_port_dev*, char*);
void init_screen(alt_up_pixel_buffer_dma_dev*, alt_up_parallel_port_dev*);

unsigned char ix = 0;
char kbd_input(alt_up_ps2_dev*);

int main(void)
{
	/* declare variables to point to devices that are opened */
	alt_up_parallel_port_dev *char_buf_dev;
	alt_up_ps2_dev *ps2;
	alt_up_pixel_buffer_dma_dev *pixel_buffer_dev;
	alt_up_pixel_buffer_dma_dev *pixel_bg_dev;


	pixel_buffer_dev = alt_up_pixel_buffer_dma_open_dev("/dev/VGA_Pixel_Buffer");
	pixel_bg_dev = alt_up_pixel_buffer_dma_open_dev("/dev/VGA_Pixel_Buffer");
	char_buf_dev = alt_up_char_buffer_open_dev("/dev/VGA_Char_Buffer");
	ps2 = alt_up_ps2_open_dev("/dev/PS2_Port");

	init_check(pixel_buffer_dev, "VGA pixel buffer");
	init_check(char_buf_dev, "char_buf_dev");
	init_check(ps2, "keyboard");

	alt_up_ps2_init(ps2);
	alt_up_ps2_clear_fifo(ps2);
	

	// MAIN LOCAL VAL
	///////////////////////////////////////////////////////////////////////
	char KBD_VAL;
	int RETURN_MENU_FLAG = 0;
	int MENU_SELECT = -1;

#ifndef MASTER
	char *menu_list[] = { "RPG","CAVE","NONE","END" };
#else
	char *menu_list[] = { "START","","","" };
#endif
	menu main_menu = init_menu(menu_list, "GAME MENU");

MAIN_MENU:
	if (RETURN_MENU_FLAG == 1)
	{
		MENU_SELECT = -1;
		RETURN_MENU_FLAG = 0;
	}
	/* menu draw  */
	clear_screen(pixel_buffer_dev, char_buf_dev);
	draw_main_menu_frame(pixel_buffer_dev);							// draw main menu
	print_menu_list(pixel_buffer_dev, char_buf_dev, &main_menu);	// draw main menu str
#ifdef MASTER
	alt_up_char_buffer_string(char_buf_dev, "Please Push Enter", TEXT_WIDTH, TOP_HEIGHT);
#endif

	/* menu loop */
	while (MENU_SELECT == -1)
	{
		KBD_VAL = kbd_input(ps2);
		if (KBD_VAL == KEY_DOWN)
		{
			inc_ch(&main_menu);
			alt_up_char_buffer_clear(char_buf_dev);
			print_menu_list(pixel_buffer_dev, char_buf_dev, &main_menu);
		}
		else if (KBD_VAL == KEY_UP)
		{
			dec_ch(&main_menu);
			alt_up_char_buffer_clear(char_buf_dev);
			print_menu_list(pixel_buffer_dev, char_buf_dev, &main_menu);
		}
		else if (KBD_VAL == KEY_ENTER)
		{
			MENU_SELECT = get_ch(main_menu);
			clear_screen(pixel_buffer_dev, char_buf_dev);
		}
	}

	switch (MENU_SELECT)
	{
	case 0:
#ifdef MASTER
		goto CAVE;
#else 
		goto RPG;
#endif
	case 1:
#ifdef MASTER
#else
		goto CAVE;
#endif
	case 3:
		goto END;
	default:
		goto END;
	}


	/* goto program */
RPG:
	{
		struct param you = { "Yusya", 100, 100, 100 };
		struct param *enemy;
		struct param enemy1 = { "Demon", 100, 100, 50 };
		struct param enemy2 = { "Dragon", 150, 150, 100 };
		struct param enemy3 = { "KIMURA", 2000, 180, 200 };
		enum {
			ATTACK = 0,
			DEFENCE = 1,
			RECOVERY = 2,
			PBOOST = 3,
		};
		char *yusya_menu[] = { "ATTACK", "DEFENCE", "RECOVERY", "POW BOOST" };
		menu yusya = init_menu(yusya_menu, "Yusya");
		int start_flag = 1;
		char KBD_VAL_;

		draw_main_menu_frame(pixel_buffer_dev);
		print_menu_list(pixel_buffer_dev, char_buf_dev, &yusya);
		draw_status_menu_frame(pixel_buffer_dev);
		print_status_list(pixel_buffer_dev, char_buf_dev, you);

		while (1)
		{
			if (start_flag)
			{
				enemy = &enemy1;
				//printf("\n%s ‚ªŒ»‚ê‚½!\n\n", enemy.name);
				//alt_up_char_buffer_string(char_buf_dev, str, TEXT_WIDTH, TOP_HEIGHT);
				char opening_text[32] = "A";
				//sprintf(opening_text, "A %s draws near!",enemy->name);
				alt_up_char_buffer_string(char_buf_dev, "A", TEXT_WIDTH, TOP_HEIGHT);
				alt_up_char_buffer_string(char_buf_dev, enemy->name, TEXT_WIDTH + 3, TOP_HEIGHT);
				alt_up_char_buffer_string(char_buf_dev, " draws near!",
					TEXT_WIDTH + 3 + strlen(enemy->name), TOP_HEIGHT);
				start_flag = 0;
			}

			KBD_VAL_ = kbd_input(ps2);
			if (KBD_VAL_ == KEY_DOWN)
			{
				inc_ch(&yusya);
				alt_up_char_buffer_clear(char_buf_dev);
				print_menu_list(pixel_buffer_dev, char_buf_dev, &yusya);
				print_status_list(pixel_buffer_dev, char_buf_dev, you);
			}
			else if (KBD_VAL_ == KEY_UP)
			{
				dec_ch(&yusya);
				alt_up_char_buffer_clear(char_buf_dev);
				print_menu_list(pixel_buffer_dev, char_buf_dev, &yusya);
				print_status_list(pixel_buffer_dev, char_buf_dev, you);
			}
			else if (KBD_VAL_ == 'Q')
			{
				RETURN_MENU_FLAG = 1;
				goto MAIN_MENU;

			}
			else if (KBD_VAL_ == KEY_ENTER)
			{
				// act(char_buf_dev, &yusya, enemy,get_ch(yusya));
				int sel_ch = get_ch(yusya);
				switch (sel_ch)
				{
				case ATTACK:
				{
					board char_pixel;
					/*
					int pix[BOARD_WIDTH_MAX][BOARD_HEIGHT_MAX] = {
						{ 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
						{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
						{ 0, 1, 0, 0, 0, 11, 11, 11, 11, 11, 0, 1, 0, 0, 0, 0 },
						{ 0, 1, 0, 0, 0, 0, 0, 11, 0, 0, 11, 1, 0, 0, 0, 0 },
						{ 0, 1, 0, 0, 11, 11, 11, 11, 11, 11, 11, 1, 0, 0, 0, 0 },
						{ 0, 1, 11, 0, 11, 0, 1, 11, 0, 1, 11, 1, 0, 0, 0, 0 },
						{ 0, 1, 11, 11, 11, 11, 11, 11, 11, 11, 11, 1, 0, 0, 0, 0 },
						{ 0, 1, 11, 11, 11, 11, 11, 11, 11, 11, 11, 1, 0, 0, 0, 0 },
						{ 0, 0, 1, 11, 11, 11, 10, 10, 10, 11, 11, 1, 0, 0, 0, 0 },
						{ 0, 0, 1, 13, 11, 11, 11, 11, 11, 11, 1, 0, 0, 0, 0, 0 },
						{ 0, 0, 1, 13, 13, 13, 11, 11, 11, 13, 1, 0, 0, 0, 0, 0 },
						{ 0, 1, 1, 13, 13, 13, 13, 13, 13, 11, 1, 1, 1, 0, 0, 0 },
						{ 12, 12, 12, 13, 11, 11, 11, 11, 11, 11, 12, 12, 12, 1, 1, 0 },
						{ 12, 12, 12, 12, 12, 12, 11, 11, 11, 12, 12, 12, 12, 12, 12, 0 },
						{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 },
						{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 }
					};
					*/

					// DRAGON
					int pix[BOARD_WIDTH_MAX][BOARD_HEIGHT_MAX] = {
					{ 0, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
					{ 0, 0, 0, 0, 5, 5, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0 },
					{ 0, 0, 0, 0, 3, 3, 2, 3, 3, 3, 0, 0, 0, 0, 0, 0 },
					{ 0, 0, 0, 0, 3, 3, 2, 2, 3, 3, 9, 0, 0, 0, 0, 0 },
					{ 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0 },
					{ 4, 0, 0, 3, 3, 3, 3, 3, 3, 0, 4, 4, 0, 0, 0, 0 },
					{ 4, 4, 0, 3, 3, 3, 3, 0, 0, 4, 4, 4, 4, 0, 0, 0 },
					{ 4, 4, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 0, 0 },
					{ 4, 4, 3, 3, 1, 1, 1, 3, 3, 4, 4, 4, 4, 4, 0, 0 },
					{ 4, 0, 3, 7, 7, 7, 7, 7, 3, 3, 3, 4, 0, 4, 4, 0 },
					{ 0, 0, 3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 0, 0, 4, 0 },
					{ 0, 0, 3, 7, 7, 7, 7, 7, 3, 3, 3, 3, 0, 0, 0, 0 },
					{ 0, 0, 3, 3, 1, 1, 1, 3, 3, 3, 3, 3, 3, 0, 0, 3 },
					{ 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
					{ 0, 0, 3, 3, 3, 0, 3, 3, 3, 0, 3, 3, 0, 0, 0, 0 },
					{ 0, 0, 9, 9, 3, 0, 9, 9, 3, 0, 9, 3, 0, 0, 0, 0 }
					};

					set_board_pixel(&char_pixel, pix);
					draw_board(pixel_buffer_dev, char_pixel);
					break;
				}
				case DEFENCE:
				{
					board char_pixel;
					int pix[BOARD_WIDTH_MAX][BOARD_HEIGHT_MAX] = {
					{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
					{ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
					{ 6, 0, 6, 0, 6, 0, 1, 1, 2, 2, 2, 1, 1, 0, 0, 0 },
					{ 6, 0, 6, 0, 6, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },
					{ 6, 0, 6, 0, 6, 2, 2, 0, 2, 2, 2, 0, 2, 2, 0, 0 },
					{ 0, 6, 6, 6, 0, 2, 2, 0, 2, 2, 2, 0, 2, 2, 0, 0 },
					{ 0, 0, 6, 0, 0, 0, 2, 2, 2, 0, 2, 2, 2, 0, 0, 0 },
					{ 0, 0, 6, 0, 0, 8, 0, 2, 2, 2, 2, 2, 0, 8, 0, 0 },
					{ 0, 0, 6, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
					{ 0, 0, 6, 0, 8, 8, 8, 2, 2, 2, 2, 2, 3, 3, 3, 3 },
					{ 0, 0, 6, 8, 0, 8, 0, 2, 2, 2, 2, 2, 0, 8, 0, 8 },
					{ 0, 0, 6, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0 },
					{ 0, 0, 6, 0, 0, 0, 0, 9, 9, 9, 9, 9, 0, 0, 0, 0 },
					{ 0, 0, 6, 0, 0, 0, 0, 9, 9, 9, 9, 9, 0, 0, 0, 0 },
					{ 0, 0, 6, 0, 0, 0, 0, 9, 9, 0, 9, 9, 0, 0, 0, 0 },
					{ 0, 0, 6, 0, 0, 0, 0, 9, 9, 0, 9, 9, 0, 0, 0, 0 }
					};
					set_board_pixel(&char_pixel, pix);
					draw_board(pixel_buffer_dev, char_pixel);
					act(char_buf_dev, &yusya, enemy, sel_ch);
					break;

				}
				case RECOVERY:
				{
					act(char_buf_dev, &yusya, enemy, sel_ch);
					break;
				}
				case PBOOST:
				{
					act(char_buf_dev, &yusya, enemy, sel_ch);
					break;
				}

				} // switch end
			}
		} // main loop end
		goto END;
	}

CAVE:
	{
		/*
			game monitor : 80x60
			rect : 2x3
		*/

		// make timer
		double  pst_t = 0;			// past time
		double cur_t;				// current time	
		time_t t_s = time(NULL);	// get start time stamp

		// game variables
		enum { GAMEOVER = 0, NOT_GAMEOVER = 1 };
		char KBD_VAL_;
		int START_FLAG_ = 1;
		unsigned int score = 0;
		unsigned int walls = 40;
		unsigned int ship_x = 8;
		unsigned int ship_y = 30;
		int ship_y_velocity = 0;
		int gameStatus = NOT_GAMEOVER;
		
		srand((unsigned)time(NULL));
		int slope = rand() % 6 + 1;
		
		point LEFT_CONER = init_point(0, 0);
		point RIGHT_CONER = init_point(80, 60);
		points rect_points[walls];

		int i;
		for (i = 0; i < walls; i++)
		{
			point left = init_point(i * 2, 14);
			point right = init_point((i * 2) + 2 , 14 + 25);
			rect_points[i] = init_points(left, right);
		}

		draw_box(pixel_bg_dev, LEFT_CONER, RIGHT_CONER, COLOR_GREEN);	// draw back ground

		while (gameStatus == NOT_GAMEOVER)
		{
			alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 1);
			draw_box(pixel_bg_dev, LEFT_CONER, RIGHT_CONER, COLOR_GREEN);	// draw back ground

			if (rect_points[ship_x/4+1].p1.y > ship_y - 1 ||
				rect_points[ship_x/4+1].p2.y < ship_y + 1)
			{
				gameStatus = GAMEOVER;
			}


			// draw holes
			int r;
			for (r = 0; r < walls; r++)
			{
				draw_box(pixel_buffer_dev, rect_points[r].p1, rect_points[r].p2, COLOR_BLACK);
			}

			// update rect_points
			points rect_test = rect_points[walls - 1];
			move_rect_points(&rect_test, 0, slope);
			if (rect_test.p1.y <= 1 || rect_test.p2.y >= 60)
			{
				int sig = (slope > 0) ? -1 : 1;
				slope = (rand() % 6 + 1) * sig;
				move_rect_points(&rect_test, 0, slope);
			}
			shift_rect_points(&rect_points, walls);
			rect_points[walls - 1] = rect_test;

			// move
			score += 1;
			KBD_VAL_ = kbd_input(ps2);
			if (KBD_VAL_ == KEY_UP)
			{
				ship_y -= 2;
			}
			else if (KBD_VAL_ == KEY_DOWN)
			{
				ship_y += 2;
			}


			// print str
			char score_str[10];
			sprintf(score_str, "%d", score);
			alt_up_char_buffer_clear(char_buf_dev);
			alt_up_char_buffer_string(char_buf_dev, "Score is ", 60, 2);
			alt_up_char_buffer_string(char_buf_dev, ">", ship_x, ship_y);
			alt_up_char_buffer_string(char_buf_dev, score_str, 69, 2);

			if (gameStatus == GAMEOVER)
			{
				break;
			}

			delay(200);
		} // loop end


		// continue menu
		const unsigned int MENU_SIZE = 13;
		point menu_left_corner = init_point(40 - (MENU_SIZE / 2), 30 - (MENU_SIZE / 2));
		point menu_right_corner = init_point(40 + (MENU_SIZE / 2), 30 + (MENU_SIZE / 2));
		draw_box(pixel_buffer_dev, menu_left_corner, menu_right_corner, DARKGRAY_16);
		int cursor = 0;
		while (1)
		{
			KBD_VAL_ = kbd_input(ps2);
			alt_up_char_buffer_clear(char_buf_dev);
			alt_up_char_buffer_string(char_buf_dev, "Continue ?", 36, 24);
			alt_up_char_buffer_string(char_buf_dev, "Yes", 38, 26);
			alt_up_char_buffer_string(char_buf_dev, "No", 38, 28);
			alt_up_char_buffer_string(char_buf_dev, ">", 36, 26 + cursor);
			if (KBD_VAL_ == KEY_DOWN)
			{
				cursor = (cursor == 0) ? 2 : 0;
				
			}
			else if (KBD_VAL_ == KEY_UP)
			{
				cursor = (cursor == 0) ? 2 : 0;
			}
			else if (KBD_VAL_ == KEY_ENTER)
			{
				switch (cursor)
				{
				case 0:
					goto CAVE;
				case 1:
					RETURN_MENU_FLAG = 1;
					goto MAIN_MENU;
				default :
					RETURN_MENU_FLAG = 1;
					goto MAIN_MENU;
				}
			}
		}
	} // cave end

END:
	return 0;

}// main() end



void init_check(alt_up_parallel_port_dev *dev, char *str)
{
	if (dev == NULL)
	{
		alt_printf("Error: could not open %s device\n", str);
	}
	else
	{
		alt_printf("Opened %s device \n", str);
	}
}

void clear_screen(alt_up_pixel_buffer_dma_dev *pixel_dev, alt_up_parallel_port_dev *char_buf_dev)
{
	alt_up_char_buffer_clear(char_buf_dev);
	alt_up_pixel_buffer_dma_clear_screen(pixel_dev, 0);
}

char kbd_input(alt_up_ps2_dev *ps2)
{
	KB_CODE_TYPE decode_mode;
	alt_u8 buffer;
	char ascii;
	if ((decode_scancode(ps2, &decode_mode, &buffer, &ascii) == 0))
	{
		ix = ix + 1;
		if ((ix % 2) == 0)
		{
			ix = 0;
			return ascii;
		}
	}
	return -1;
}