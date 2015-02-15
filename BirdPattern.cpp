class BirdPattern : public Pattern
{
private:
	Color pattern[8][12];

public:
	static const int WIDTH = 12;
	static const int HEIGHT = 8;
	BirdPattern()
	{
		int i, j;
		for (i=0; i<HEIGHT; i++)
		{
			for (j=0; j< WIDTH; j++)
			{
				pattern[i][j] = Color(222, 222, 222);
			}
		}
		
		pattern[2][2] = Color(24,7,108);

		pattern[3][2] = Color(24,7,108);
		pattern[3][3] = Color(24,7,108);
		pattern[3][6] = Color(24,7,108);
		pattern[3][7] = Color(24,7,108);

		pattern[4][3] = Color(24,7,108);
		pattern[4][4] = Color(24,7,108);
		pattern[4][5] = Color(24,7,108);
		pattern[4][6] = Color(24,7,108);
		pattern[4][7] = Color(0,0,0);
		pattern[4][8] = Color(24,7,108);

		pattern[5][3] = Color(24,7,108);
		pattern[5][4] = Color(24,7,108);
		pattern[5][5] = Color(24,7,108);
		pattern[5][6] = Color(24,7,108);
		pattern[5][7] = Color(24,7,108);
		pattern[5][8] = Color(24,7,108);
		pattern[5][9] = Color(250,237,0);

		pattern[6][1] = Color(24,7,108);
		pattern[6][2] = Color(24,7,108);
		pattern[6][3] = Color(24,7,108);
		pattern[6][4] = Color(24,7,108);
		pattern[6][5] = Color(24,7,108);

	}
	virtual Color getColor(int x, int y)
	{
		int xPattern = x % WIDTH;
		int yPattern = y % HEIGHT;

		return pattern[yPattern][xPattern];
	}
};