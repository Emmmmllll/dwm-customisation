namespace config{
	
	enum { ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
       ClkClientWin, ClkRootWin, ClkLast }; /* clicks */
	enum {Grabbed = 1, Repeat = 1<<1, Release = 1<<2};

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

#ifdef _GLIBCXX_VECTOR
	extern std::vector<Key> keys;
	extern std::vector<Button> buttons;
	// Layout * layouts;

#endif // _GLIBCXX_VECTOR

	// Rule * rules;
	// char ** tags;

	// bool showbar;
	// bool topbar = true;

	
	void parse_keybinds();
	void cleanup();
	void register_actionfunc(void (*func)(Arg *), const char * name, const unsigned char argtype);
}