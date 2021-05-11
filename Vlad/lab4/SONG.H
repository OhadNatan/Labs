

struct sonsquery {
	int next;
};

struct sons {
	int quantity;
	int first;
	int last;
	struct sonsquery soq[NPROC];
};

extern struct sons procson[];
