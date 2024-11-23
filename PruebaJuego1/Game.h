#pragma once
#include <iostream>		
#include"vector"
#include"ctime"
#include<sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
// Clase contenedora del juego 
class Game
{
private:
	// variables
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;
	
	//funciones privadas
	void initVariables();
	void initWindow();
	void initEnemies();
	void initFonts();
	void initTexts();

	//Posiciones del mouse
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Variables del juego
	int points;
	int health;
	float enemySpawnTimer;
	float maxEnemySpawnTimer;
	int maxEnemies;
	bool mouseHold;
	bool endGame;

	//Recursos
	sf::Font font;
	sf::Text textPoints;

	//Objetos del juego
	sf::RectangleShape enemy;
	std::vector< sf::RectangleShape > enemies;
	

public:
	//constructors / desctructors 
	Game();
	virtual ~Game();
	//getters
	const bool getRunning();
	//Updates
	void pollEvents();
	void update();
	void updateMousPos();
	void updateEnemies();
	void updateHUD();
	//funciones del juego
	void spawnEnemy();
	//renders
	void render();
	void renderEnemies(sf::RenderTarget& target);
	void renderHUD(sf::RenderTarget& target);
};

