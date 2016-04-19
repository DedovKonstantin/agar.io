#include "Game.hpp"

int main()
{
	try
	{
		Game game(Vector2<size_t>(1000, 500));
		game.Run();
	}
	catch (const RunTimeException& e)
	{
		cout << e << endl;
	}

	return 0;
}
