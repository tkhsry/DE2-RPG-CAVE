
#ifndef included_menu
#define included_menu

/* menu method */
menu init_menu(char *s[],char *n )
{
	int i;
	menu m;
	for (i = 0; i < MENU_MAX; i++)
		m.str[i] = s[i];
	m.name = n;
	m.ch = 0;
	m.sel = 0;
	return m;
}

void inc_ch(menu *m)
{
	m->ch++;
}

void dec_ch(menu *m)
{
	m->ch--;
}

int get_ch(menu m)
{
	return m.ch;
}


/* menu draw method*/
enum {
	TOP_WIDTH = 7,		// top x
	TOP_HEIGHT = 48,	// top y
	D = 3,				// height interval
	TEXT_WIDTH = 19,	// main menu text start -> (7,19)
	TEXT_WIDTH_MAX = 75,
	NAME_WIDTH = 5,
	NAME_HEIGHT = 45
};

void draw_box(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev,
	point p1, point p2, unsigned color)
{
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, p1.x * 4, p1.y * 4, p2.x * 4, p2.y * 4, color, 0);
}

void draw_line(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev, point p1, point p2)
{
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, p1.x, p1.y, p2.x, p2.y, 0xffff, 0);
}

void draw_menu(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev, point p1, point p2)
{
	// menu ã˜g
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, p1.x, p1.y, p2.x, p1.y, 0xffff, 0);
	// menu ¶˜g
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, p1.x, p1.y, p1.x, p2.y, 0xffff, 0);
	// menu ‰E˜g
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, p2.x, p1.y, p2.x, p2.y, 0xffff, 0);
	// menu ‰º˜g
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, p1.x, p2.y, p2.x, p2.y, 0xffff, 0);
}

void draw_main_menu_frame(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev)
{
	enum {
		C = 70	// ’†‰›ü‚ÌˆÊ’u
	};
	point p1 = init_point(10, 185);
	point p2 = init_point(310, 235);
	draw_menu(pixel_buffer_dev, p1, p2);
	// ’†‰›˜g
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, C, p1.y, C, p2.y, 0xffff, 0);

	point np1 = init_point(10, 175);	// name point (upper left  coner)
	point np2 = init_point(70, 185);	// name point (lower right coner)
	draw_menu(pixel_buffer_dev, np1, np2);
}

void draw_status_menu_frame(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev)
{
	point p1 = init_point(10, 20);
	point p2 = init_point(70, 100);
	draw_menu(pixel_buffer_dev, p1, p2);
}

void print_status_list(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev,
	alt_up_parallel_port_dev *char_buf_dev, param st)
{
	enum {
		TOP_STATUS_HEIGHT = 9
	};
	char stbuf[10];
	sprintf(stbuf, "HP %d", st.life);
	alt_up_char_buffer_string(char_buf_dev, st.name, TOP_WIDTH - 2, TOP_STATUS_HEIGHT -1);
	alt_up_char_buffer_string(char_buf_dev, stbuf, TOP_WIDTH - 2, TOP_STATUS_HEIGHT + 1);
	sprintf(stbuf, "ATK %d", st.attack);
	alt_up_char_buffer_string(char_buf_dev, stbuf, TOP_WIDTH - 2, TOP_STATUS_HEIGHT + 3);
	sprintf(stbuf, "DEF %d", st.defense);
	alt_up_char_buffer_string(char_buf_dev, stbuf, TOP_WIDTH - 2, TOP_STATUS_HEIGHT + 5);
}

int print_menu_list(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev,
					alt_up_parallel_port_dev *char_buf_dev, menu *m)
{
	// print name
	alt_up_char_buffer_string(char_buf_dev, m->name,NAME_WIDTH, NAME_HEIGHT);

	// print menu
	int i;
	for (i = 0; i < MENU_MAX; i++) {
		alt_up_char_buffer_string(char_buf_dev, m->str[i], TOP_WIDTH, TOP_HEIGHT + D * i);
	}

	// print select cursor
	if (m->ch >= MENU_MAX)
		m->ch = m->ch % MENU_MAX;
	alt_up_char_buffer_string(char_buf_dev, ">", TOP_WIDTH - 2, TOP_HEIGHT + D * m->ch);
	
	return NONE;
}

#endif