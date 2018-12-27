#include "draw.h"

int main() {

	struct Console *c;
	struct Item *m;
	struct Item *ctrl;
	struct Controls st;
	BOOL isRun = TRUE;
	console_init(&c);
	printf("X= %d, Y= %d\n", c->width, c->height);
	menu_page(&m, c);
	make_event_menu(c, m, &ctrl,&st);
	return 0;
}				