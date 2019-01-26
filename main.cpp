#include <SFML/Graphics.hpp>
#include "declarations.h"
#include "Entity.h"
#include <iostream>
#include "FileLoader.h"
#include "drawMap.h"
#include <Windows.h>
#include <ctime>
#include <cstdlib>

unsigned char map[MAPLENGTH][MAPHEIGHT];
extern sf::Sprite mapSprite;


int main()
{	
	srand(time(0));


	sf::RenderWindow window(sf::VideoMode(200, 200), "Stressed Out!", sf::Style::Fullscreen);
	//window.setFramerateLimit(50);
	window.setVerticalSyncEnabled(1);
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	sf::Texture mosTexture;
	sf::Texture textures;
	sf::Texture fantomaTexture;

	sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
	

	mosTexture.loadFromFile("mos.png");
	textures.loadFromFile("textures.png");
	fantomaTexture.loadFromFile("fantoma soacra.png");

	mapSprite.setTexture(textures);
	


	loadMap("map.txt");
	
	
	Entity fantoma(&fantomaTexture);
	fantoma.speed = 0.20f;
	fantoma.setPosition({ 20 * 80, 80 });
	float fantomaTime = (rand() %35000) + 5000 + GetTickCount();
	bool fantomaAwake = 1;
	
	
	//fantoma



	Entity mos(&mosTexture);
	mos.position = { 100,100 };
	mos.speed = 0.45f;

	float time = GetTickCount();
	float deltaTime = 0;

	int screenWith = window.getSize().x;
	int screenHeight = window.getSize().y;

	while (window.isOpen())
	{


		deltaTime = GetTickCount() - time;
		time = GetTickCount();
		//deltaTime = 0.01;

		handleEvents(window);
		
		//handle key

		sf::Vector2i deplasation = { 0,0 };
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			deplasation.x += 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			deplasation.x -= 1;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			deplasation.y -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			deplasation.y += 1;
		}
			
		if(deplasation.x != 0 && deplasation.y !=0)
		{
			mos.setPosition({ mos.getPosition().x + (int)(deplasation.x * deltaTime * mos.speed / (float)sqrt(2)), mos.getPosition().y + (int)(deplasation.y * deltaTime * mos.speed / (float)sqrt(2)) });
		}else
		{
			mos.setPosition({ mos.getPosition().x + (int)(deplasation.x * deltaTime * mos.speed), mos.getPosition().y + (int)(deplasation.y * deltaTime * mos.speed)});
		}

		//view.setViewport(sf::FloatRect( mos.sprite.getPosition().x - (window.getSize().x /2.f), mos.sprite.getPosition().y - (window.getSize().y / 2.f), window.getSize().x, window.getSize().y ));
	
		//fantoma
		if(GetTickCount() > fantomaTime)
		{
			fantomaTime = (rand() % 25000) + 5000 + GetTickCount();
			if(fantomaAwake)
			{
				fantomaAwake = 0;
			}else
			{
				fantomaAwake = 1;
			}

			fantoma.setPosition({ 20 * 80, 80 });
		}




		//fantoma

		fixCollisionWall(mos);
		fixCollision(mos);
		updateMovement(mos);


		view.setCenter((int)mos.getPosition().x, (int)mos.getPosition().y);

		if (view.getCenter().x < screenWith / 2.f) { view.setCenter((int)(screenWith / 2.f), (int)view.getCenter().y); }
		if (view.getCenter().x > -(screenWith / 2.f) + (80 * MAPLENGTH)) { view.setCenter((int)(-(screenWith / 2.f) + (80 * MAPLENGTH)), (int)view.getCenter().y); }
		if (view.getCenter().y < screenHeight / 2.f) { view.setCenter((int)(view.getCenter().x), (int)(screenHeight / 2.f)); }
		if (view.getCenter().y > -(screenHeight / 2.f) + (80 * MAPHEIGHT)) { view.setCenter((int)(view.getCenter().x), (int)(-(screenHeight / 2.f) + (80 * MAPHEIGHT))); }
		///fix screen position ^


		drawMap(&window);

		if(fantomaAwake)
		{
			sf::Vector2i deplasation = { 0,0 };
			if (abs(mos.getcenterx() - fantoma.getcenterx()) < 30)
			{
				deplasation.x = 0;
			}
			else if(mos.getPosition().x > fantoma.getPosition().x)
			{
				deplasation.x = 1;
			}else
			{
				deplasation.x = -1;
			}
			if (abs(mos.getcentery() - fantoma.getcentery()) < 30)
			{
				deplasation.y = 0;
			}
			else if (mos.getPosition().y > fantoma.getPosition().y)
			{
				deplasation.y = 1;
			}
			else
			{
				deplasation.y = -1;
			}

			if (deplasation.x != 0 && deplasation.y != 0)
			{
				fantoma.setPosition({ fantoma.getPosition().x + (int)(deplasation.x * deltaTime * fantoma.speed / (float)sqrt(2)), fantoma.getPosition().y + (int)(deplasation.y * deltaTime * fantoma.speed / (float)sqrt(2)) });
			}
			else
			{
				fantoma.setPosition({ fantoma.getPosition().x + (int)(deplasation.x * deltaTime * fantoma.speed), fantoma.getPosition().y + (int)(deplasation.y * deltaTime * fantoma.speed) });
			}

			fixCollisionWall(fantoma);
			updateMovement(fantoma);
			fantoma.draw(&window);
		}

		

		window.setView(view);
		mos.draw(&window);
		
		window.display();

		window.clear();
	}

	return 0;
}