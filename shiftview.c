/** Function to shift the current view to the left/right
 *
 * @param: "arg->i" stores the number of tags to shift right (positive value)
 *          or left (negative value)
 */
void
shiftview(const Arg *arg)
{
	Arg shifted;
	Client *c;
	unsigned int tagmask = 0;

	for (c = selmon->clients; c; c = c->next)
		if (!(c->tags & SPTAGMASK))
			tagmask = tagmask | c->tags;

	shifted.ui = selmon->tagset[selmon->seltags] & ~SPTAGMASK;
	if (arg->i > 0) /* left circular shift */
		do {
			shifted.ui = (shifted.ui << arg->i)
			   | (shifted.ui >> (LENGTH(tags) - arg->i));
			shifted.ui &= ~SPTAGMASK;
		} while (tagmask && !(shifted.ui & tagmask));
	else /* right circular shift */
		do {
			shifted.ui = (shifted.ui >> (- arg->i)
			   | shifted.ui << (LENGTH(tags) + arg->i));
			shifted.ui &= ~SPTAGMASK;
		} while (tagmask && !(shifted.ui & tagmask));

	view(&shifted);
}


int alevel = 0xfff;

void shiftchangevolup(const Arg *args) {
	unsigned int tagmask = 0;
	const char precmd[] = "pacmd set-sink-volume";
	size_t size = (sizeof(precmd)/sizeof(char)) + (sizeof(alsadev)/sizeof(char)) + 10;
	char buf[size];
	sprintf(buf,"pacmd set-sink-volume %s 0x%d", alsadev, alevel);
	system(buf);
	alevel+=args->i;
}
void shiftchangevoldown(Arg *args) {
	unsigned int tagmask = 0;
	const char precmd[] = "pacmd set-sink-volume";
	size_t size = (sizeof(precmd)/sizeof(char)) + (sizeof(alsadev)/sizeof(char)) + 10;
	char buf[size];
	sprintf(buf,"pacmd set-sink-volume %s 0x%d", alsadev, args->i);
	system(buf);
	args->i-=0xfff;
}
void shiftchangevolmute(const Arg *args) {
	unsigned int tagmask = 0;
	const char precmd[] = "pacmd set-sink-volume";
	size_t size = (sizeof(precmd)/sizeof(char)) + (sizeof(alsadev)/sizeof(char)) + 10;
	char buf[size];
	sprintf(buf,"pacmd set-sink-volume %s 0x%d", alsadev, args->i);
	system(buf);
}
void
shifttag(const Arg *arg)
{
	Arg a;
	Client *c;
	unsigned visible = 0;
	int i = arg->i;
	int count = 0;
	int nextseltags, curseltags = selmon->tagset[selmon->seltags];

	do {
		if(i > 0) // left circular shift
			nextseltags = (curseltags << i) | (curseltags >> (LENGTH(tags) - i));

		else // right circular shift
			nextseltags = curseltags >> (- i) | (curseltags << (LENGTH(tags) + i));

                // Check if tag is visible
		for (c = selmon->clients; c && !visible; c = c->next)
			if (nextseltags & c->tags) {
				visible = 1;
				break;
			}
		i += arg->i;
	} while (!visible && ++count < 10);

	if (count < 10) {
		a.i = nextseltags;
		tag(&a);
	}
}
