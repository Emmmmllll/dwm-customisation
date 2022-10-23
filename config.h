
enum { ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
       ClkClientWin, ClkRootWin, ClkLast }; /* clicks */

typedef struct {
	unsigned int click;
	unsigned int mask;
	unsigned int button;
	void (*func)(const Arg *arg);
	const Arg arg;
} Button;

typedef struct {
	unsigned int mod;
	unsigned char grabRepRelMask;
	KeySym keysym;
	void (*func)(const Arg *);
	const Arg arg;
} Key;

void parse_keyBinds(Key * keys, Button * buttons);