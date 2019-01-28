
#ifndef included_globals
#define included_globals

/* globals */

#define menu_layer while(1)
#define menu_close break

#define COLOR_BLACK 0x0000
#define COLOR_BLUE 0x187F
#define COLOR_LIGHT_BLUE 0x6FF
#define COLOR_RED  0xF000
#define COLOR_PINK 0x9990
#define COLOR_GREEN  0x0F00
#define COLOR_LIGHT_GREEN 0x0FF0
#define COLOR_YELLOW  0xFE00
#define COLOR_DARK_WHITE 0xA615
#define COLOR_WHITE  0xFFFF
#define BLACK_16 0x0000
#define GHOSTWHITE_16 0xFFDF
#define MAROON_16 0x0010
#define GREEN_16 0x0400
#define LIGHTGREEN_16 0x9752
#define LIGHTYELLOW_16 0xE7DF
#define LIGHTGRAY_16 0xD69A
#define DARKORANGE_16 0x045F
#define DARKRED_16 0x0011
#define GRAY_16 0x8410
#define LIGHTPINK_16 0xC59F
#define DARKGRAY_16 0xAD55
#define BURLYWOOD_16 0x85DB
#define BLANCHEDALMOND_16 0xCF5F

#define UPPER_LEFT_CONER 0,0
#define LOWER_LEFT_CONER 0,59
#define UPPER_RIGHT_CONER 79,0
#define LOWER_RIGHT_CONER 79,59

#define MENU_MAX 4
#define KEY_UP 'W'
#define KEY_DOWN 'S'
#define KEY_RIGHT 'D'
#define KEY_LEFT  'A'
#define KEY_ENTER '\0'

enum
{
	NONE = -1,
	SAVE_MAX = 10,
	LU = 0,
	RL = 1,
	BOX_CELL_UNIT = 8,
	BOX_START_X = 16,
	BOX_START_Y = 16,
	BOX_END_X = 112,
	BOX_END_Y = 208,
	BOX_LOWER_LEFT_X = BOX_START_X,
	BOX_LOWER_LEFT_Y = BOX_END_Y,
	BOX_LOWER_RIGHT_X = BOX_END_X,
	BOX_LOWER_RIGHT_Y = BOX_END_Y,
	BOX_UPPER_LEFT_X = BOX_START_X,
	BOX_UPPER_LEFT_Y = BOX_START_Y,
	BOX_UPPER_RIGHT_X = BOX_END_X,
	BOX_UPPER_RIGHT_Y = BOX_START_Y,


	// make cell square
	SQ_SIZE = 2,
	SQ_START_LU_X = (BOX_START_X / 4 + BOX_CELL_UNIT / 4),
	SQ_START_LU_Y = (BOX_START_Y / 4 + BOX_CELL_UNIT / 4),
	SQ_START_RL_X = (SQ_START_LU_X + SQ_SIZE),
	SQ_START_RL_Y = (SQ_START_LU_Y + SQ_SIZE)
};

void delay(int count)
{
	usleep(1000 * count);
}

typedef enum BLOCK_TYPE
{
	BACKGROUND	= 0,
	SQUARE		= 1,
	I_BLOCK		= 2,
}BLOCK_TYPE;

typedef struct menu
{
	char *str[MENU_MAX];
	unsigned int ch;
	unsigned int sel;
	char *name;
}menu;

typedef struct cell
{
	int cx;
	int cy;
}cell;

typedef struct point
{
	int x;
	int y;
}point;

typedef struct points {
	point p1;
	point p2;
}points;

int isEqualPoints(points ps1, points ps2)
{
	if (ps1.p1.x == ps2.p1.x &&
		ps1.p1.y == ps2.p1.y &&
		ps1.p2.x == ps2.p2.x &&
		ps1.p2.y == ps2.p2.y) {
		return 1;
	}
	return 0;
}

point init_point(int x, int y)
{
	point p;
	p.x = x;
	p.y = y;
	return p;
}

points init_points(point p1, point p2)
{
	points ps;
	ps.p1 = p1;
	ps.p2 = p2;
	return ps;
}

point cell_to_point(cell c)
{
	#define CELL_START_BIAS_X 16
	#define CELL_START_BIAS_Y 16
	point p;
	p.x = (c.cx * 8 + CELL_START_BIAS_X)/4;
	p.y = (c.cy * 8 + CELL_START_BIAS_Y)/4;
	return p;
}

point point_LU_to_RL(point lu)
{
	#define CELL_SQ_SIZE 2
	point rl;
	rl.x = lu.x + CELL_SQ_SIZE;
	rl.y = lu.y + CELL_SQ_SIZE;
	return rl;
}

void copy_points(points *ps, points ref_ps)
{
	ps->p1.x = ref_ps.p1.x;
	ps->p1.y = ref_ps.p1.y;
	ps->p2.x = ref_ps.p2.x;
	ps->p2.y = ref_ps.p2.y;

}

void move_rect_points(points *ps, int x, int y)
{
	ps->p1.x += x;
	ps->p1.y += y;
	ps->p2.x += x;
	ps->p2.y += y;
}

void shift_rect_points(points *ps, int ps_list_len)
{
	int i;
	points new_ps[ps_list_len];
	for (i = 0; i < ps_list_len-1; i++)
	{
		// shift
		ps[i] = ps[i + 1];
		ps[i].p1.x = ps[i + 1].p1.x - 2;
		//ps[i]->p1.y = ps[i + 1]->p1.y;
		ps[i].p2.x = ps[i + 1].p2.x - 2;
		//ps[i]->p2.y = ps[i + 1]->p2.y;
	}
}

cell init_cell(int x, int y)
{
	cell c = { x,y };
	return c;
}

cell point_to_cell(point p)
{
	cell c;
	c.cx = ((p.x*4) - CELL_START_BIAS_X)/8;
	c.cy = ((p.y*4) - CELL_START_BIAS_Y)/8;
	return c;
}



typedef struct cell_square
{
	point lu;	// left upper point
	point rl;	// right upper point
}cell_square;

cell_square init_csq(point lu, point rl)
{
	cell_square s;
	s.lu = init_point(lu.x, lu.y);
	s.rl = init_point(rl.x, rl.y);
	return s;
}

void move_csq(cell_square *cs, char cmd)
{
	#define SQ_STEP 2
	if (cmd == KEY_DOWN)
	{
		cs->lu.y += SQ_STEP;
		cs->rl.y += SQ_STEP;
	}
	else if (cmd == KEY_LEFT)
	{
		cs->lu.x -= SQ_STEP;
		cs->rl.x -= SQ_STEP;
	}
	else if (cmd == KEY_RIGHT)
	{
		cs->lu.x += SQ_STEP;
		cs->rl.x += SQ_STEP;
	}
	else if (cmd == KEY_UP)
	{
		cs->lu.y -= SQ_STEP;
		cs->rl.y -= SQ_STEP;
	}
}



typedef struct square
{
#define SQUARE_CELL_SIZE 4
	cell_square csq[SQUARE_CELL_SIZE];
	int square_state[SQUARE_CELL_SIZE];
}square;

square init_sq()
{
	square s;
	cell c[SQUARE_CELL_SIZE] =
	{
		{ 5,0 },
	{ 6,0 },
	{ 5,1 },
	{ 6,1 }
	};
	int i;
	for (i = 0; i < SQUARE_CELL_SIZE; i++)
	{
		point cell_p_lu = cell_to_point(c[i]);
		point cell_p_rl = point_LU_to_RL(cell_p_lu);
		s.csq[i] = init_csq(cell_p_lu, cell_p_rl);
		s.square_state[i] = 0;
	}
	return s;
}

void draw_sq(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev, square s)
{
	int i;
	for (i = 0; i < SQUARE_CELL_SIZE; i++)
	{
		draw_box(pixel_buffer_dev,
			s.csq[i].lu,
			s.csq[i].rl,
			COLOR_YELLOW
		);
	}
}

void move_sq(square *s, char cmd)
{
	int i;
	for (i = 0; i < SQUARE_CELL_SIZE; i++)
	{
		move_csq(&s->csq[i], cmd);
	}
}



/* board */
typedef struct board
{
	#define NONE  0
	#define BLOCK 1
	#define BOARD_WIDTH_MAX 16
	#define BOARD_HEIGHT_MAX 16

	int map[BOARD_WIDTH_MAX][BOARD_HEIGHT_MAX];
	int block_type[BOARD_WIDTH_MAX][BOARD_HEIGHT_MAX];
}board;

board init_board()
{
	board b;
	int i, j;
	for (i = 0; i < BOARD_WIDTH_MAX; i++)
	{
		for (j = 0; j < BOARD_HEIGHT_MAX; j++)
		{
			b.map[i][j] = 0;
			b.block_type[i][j] = 0;
		}
	}
	return b;
}

/*
unsigned int get_block_color(int block_type)
{
	switch (block_type)
	{
	case SQUARE:
		return COLOR_YELLOW;
		break;
	default:
		return COLOR_DARK_WHITE;
		break;
	}
}

*/


unsigned int get_block_color(int block_type)
{
	switch (block_type)
	{
	case 0:
		return BLACK_16;
		break;
	case 1:
		return GHOSTWHITE_16;
		break;
	case 2:
		return MAROON_16;
		break;
	case 3:
		return GREEN_16;
		break;
	case 4:
		return LIGHTGREEN_16;
		break;
	case 5:
		return LIGHTYELLOW_16;
		break ;
	case 6:
		return LIGHTGRAY_16;
		break;
	case 7:
		return DARKORANGE_16;
		break;
	case 8:
		return DARKRED_16;
		break;

	case 9:
		return GRAY_16;
		break;
	case 10:
		return LIGHTPINK_16;
		break;
	case 11:
		return BLANCHEDALMOND_16;
		break;
	case 12:
		return DARKGRAY_16;
		break;
	case 13:
		return BURLYWOOD_16;
		break;

	default:
		return COLOR_DARK_WHITE;
		break;
	}
}

int isblock(board b, cell c)
{
	if (b.map[c.cx][c.cy] == BLOCK)
		return 1;
	return 0;
}


// bug
void draw_board(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev, board b)
{
	#define BOARD_DRAW_BEGIN_X 10
	#define BOARD_DRAW_BEGIN_Y 5
	int i, j;
	for (i = 0; i < BOARD_WIDTH_MAX; i++)
	{
		for (j = 0; j < BOARD_HEIGHT_MAX; j++)
		{
			// i, j -> cell [i,j]
			cell cell_lu = init_cell(i+10, j+5);
			point point_lu = cell_to_point(cell_lu);
			point point_rl = point_LU_to_RL(point_lu);
			unsigned int color = get_block_color(b.map[j][i]);
			draw_box(pixel_buffer_dev,
				point_lu,
				point_rl,
				color
			);
		}
	}
}

void set_board_pixel(board *b, int pix[BOARD_WIDTH_MAX][BOARD_HEIGHT_MAX])
{
	int x, y;
	for (x = 0; x < BOARD_WIDTH_MAX; x++)
	{
		for (y = 0; y < BOARD_HEIGHT_MAX; y++)
		{
			if (pix[x][y] != 0)
			{
				b->map[x][y] = pix[x][y];
				b->block_type[x][y] = BLOCK;

			}
		}
	}
}

/*
void set_board_pixel(board *b, int pix[BOARD_WIDTH_MAX][BOARD_HEIGHT_MAX])
{
	int x, y;
	for (x = 0; x < BOARD_WIDTH_MAX; x++)
	{
		for (y = 0; y < BOARD_HEIGHT_MAX; y++)
		{
			if (pix[x][y] != 0)
			{
				b->map[x][y] = SQUARE;
				b->block_type[x][y] = BLOCK;

			}
		}
	}
}
*/

void save_board_square_data(board *b,square s)
{
	int i;
	for (i = 0; i < SQUARE_CELL_SIZE; i++)
	{
		cell c = point_to_cell(s.csq[i].lu);
		b->map[c.cx][c.cy] = BLOCK;
		b->block_type[c.cx][c.cy] = SQUARE;
	}
}

typedef struct param
{
	char name[8];
	int life, attack, defense;
}param;

void act(alt_up_parallel_port_dev *char_buf_dev,
	struct param* actor, struct param* target, int command)
{
	int point;
	char str[128];
	char str2[128];
	int total = 0;
	char point_str[8];

	switch (command)
	{
	case 0:
		point = actor->attack - target->defense;
		if (actor->attack <= target->defense) {
			point = 10;
		}
		target->life -= point;

		alt_up_char_buffer_string(char_buf_dev, "attack of ", 19, 48);
		total += strlen("attack of ");
		alt_up_char_buffer_string(char_buf_dev, actor->name,
			19 + total, 48);

		total = 0;
		alt_up_char_buffer_string(char_buf_dev, target->name,
			19 + total, 48+1);
		total += strlen(target->name);
		alt_up_char_buffer_string(char_buf_dev,"suffered ",
			19 + total, 48 + 1);
		total += strlen("suffered ");

		
		sprintf(str, "%d", point);
		alt_up_char_buffer_string(char_buf_dev, point_str,
			19 + total, 48 + 1);
		total += strlen(point_str);
		alt_up_char_buffer_string(char_buf_dev, " damage!",
			19 + total, 48 + 1);

		// sprintf(str, "attack of %s! %s suffered %d damage",actor->name, target->name, point);
		//alt_up_char_buffer_string(char_buf_dev, str, 19, 48);
		break;
	case 1:
		point = 20;
		actor->attack += point;


		int total = 0;
		alt_up_char_buffer_string(char_buf_dev, actor->name,
			19 + total, 48);
		total += strlen(actor->name);
		alt_up_char_buffer_string(char_buf_dev, ": The attack power has increased.",
			19 + total, 48);

		total = 0;
		alt_up_char_buffer_string(char_buf_dev, actor->name,
			19 + total, 48+1);

		sprintf(point_str, "%d", point);
		alt_up_char_buffer_string(char_buf_dev, point_str,
			19 + total, 48+1);
		total += strlen(point_str);
		alt_up_char_buffer_string(char_buf_dev, "points up!",
			19 + total, 48+1);

		//sprintf(str, "%s: The attack power has increased.",actor->name);
		//sprintf(str2, "%s: %d points up", actor->name, point);
		//alt_up_char_buffer_string(char_buf_dev, str, 19, 48);
		//alt_up_char_buffer_string(char_buf_dev, str2, 19, 51);
		break;
	case 2:
		point = 50;
		actor->life += point;
		sprintf(str, "Recovery of %s",actor->name);
		sprintf(str2, "%s, %d points recovery!", actor->name, point);
		alt_up_char_buffer_string(char_buf_dev, str, 19, 48);
		alt_up_char_buffer_string(char_buf_dev, str2, 19, 51);
		break;
	case 3:
		point = 20;
		actor->defense += point;
		sprintf(str, "%s: Rising Defense Point",actor->name);
		sprintf(str2, "%s:%d points up!", actor->name, point);
		alt_up_char_buffer_string(char_buf_dev, str, 19, 48);
		alt_up_char_buffer_string(char_buf_dev, str2, 19, 51);
		break;
	case 4:
		point = 100;
		target->attack -= point;
		sprintf(str, "%s: Attack power decrease",actor->name);
		sprintf(str2, " %s:%d points down!", target->name, point);
		alt_up_char_buffer_string(char_buf_dev, str, 19, 48);
		alt_up_char_buffer_string(char_buf_dev, str2, 19, 51);
		break;
	}
	alt_up_char_buffer_string(char_buf_dev, str, 19, 48);
}

#endif