#include "TileMap.h"
#include <XEngine.h> // <> for external includes, "" for internal includes
#include "GridBasedGraph.h"
TileMap tileMap;
X::TextureId textureId;
X::Math::Vector2 position;
X::Math::Vector2 pathPosition;
X::Math::Vector2 tilePosition;

//--------------------------------------------------

void GameInit()
{
	tileMap.LoadTiles("tiles.txt");
	tileMap.LoadMap("map.txt");

	textureId = X::LoadTexture("bird1.png");
	position = { 100.0f, 100.0f };
	pathPosition = {29,19};
}

bool GameLoop(float deltaTime)
{
	//use the arrow keys to move and WASD to move the 
	tilePosition = { (position.x / 32.0f) + 1.5f, (position.y / 32.0f) + 1.5f };

	std::vector<X::Math::Vector2> path = tileMap.FindPathBFS(tilePosition.x, tilePosition.y, pathPosition.x, pathPosition.y);
	std::vector<X::Math::Vector2> path2 = tileMap.FindPathDFS(tilePosition.x, tilePosition.y, pathPosition.x, pathPosition.y);
	std::vector<X::Math::Vector2> path3 = tileMap.FindPathDijkstra(tilePosition.x, tilePosition.y, pathPosition.x, pathPosition.y);
	std::vector<X::Math::Vector2> path4 = tileMap.FindPathAStar(tilePosition.x, tilePosition.y, pathPosition.x, pathPosition.y);
	const float moveSpeed = 200.0f; // pixel per second
	if (X::IsKeyDown(X::Keys::RIGHT))
		position.x += moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::LEFT))
		position.x -= moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::DOWN))
		position.y += moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::UP))
		position.y -= moveSpeed * deltaTime;

	if (X::IsKeyDown(X::Keys::D) && pathPosition.x < 29)
		pathPosition.x+=.1f;
	if (X::IsKeyDown(X::Keys::A) && pathPosition.x > 0)
		pathPosition.x-=.1f;
	if (X::IsKeyDown(X::Keys::S) && pathPosition.y < 19)
		pathPosition.y+=.1f;
	if (X::IsKeyDown(X::Keys::W) && pathPosition.y > 0)
		pathPosition.y-=.1f;
	tileMap.Render();
	for (auto space : path) X::DrawScreenCircle(space.x, space.y, 10, X::Colors::Aqua);

	for (auto space : path2) X::DrawScreenCircle(space.x, space.y, 11, X::Colors::Red);

	for (auto space : path3) X::DrawScreenCircle(space.x, space.y, 12, X::Colors::MediumPurple);

	for (auto space : path4) X::DrawScreenCircle(space.x, space.y, 13, X::Colors::HotPink);
	X::DrawSprite(textureId, position, X::Pivot::TopLeft, X::Flip::Horizontal);
	X::DrawScreenCircle(tileMap.GetPixelPosition(pathPosition.x, pathPosition.y), 20.0f, X::Colors::DarkBlue);
	X::DrawScreenDiamond(tileMap.GetPixelPosition(tilePosition.x, tilePosition.y), 5.0f, X::Colors::Cyan);

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{

}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Pathfinding");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}