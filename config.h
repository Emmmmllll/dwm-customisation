namespace config{
	
	enum { ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
       ClkClientWin, ClkRootWin, ClkLast }; /* clicks */

	typedef struct {
		unsigned int mod;
		unsigned char grabRepRelMask;
		KeySym keysym;
		void (*func)(Arg *);
		Arg arg;
	} Key;
	typedef struct {
		unsigned int click;
		unsigned int mask;
		unsigned int button;
		void (*func)(Arg *arg);
		Arg arg;
	} Button;

	// Key * keys;
	// Button * buttons;
	// Layout * layouts;
	// Rule * rules;
	// char ** tags;

	// bool showbar;
	// bool topbar = true;

	
	void parse_keybinds();
	void cleanup();
	void register_actionfunc(void (*func)(Arg *), const char * name, const unsigned char argtype);
}