#include "Game.h"


//constructores y destructores


Game::Game()
{
	this->window = nullptr;
	this->initWindow();
	this->initVariables();
	this->initEnemies();
	try {
		//Manejo de excepciones (no es necesario pero para ir practicando)
		this->initFonts();
		
		this->initTexts();		
	}
	catch (const std::runtime_error e) //Prueba en busca de unn error
	{
		//en caso de encontrarlo exponer por pantalla el mismo
		std::cout << e.what() << std::endl;
	}
}

Game::~Game()
{
	delete this->window;
}


//Window 


const bool Game::getRunning()
{
	return this->window->isOpen();
}

void Game::pollEvents()
{

	while (this->window->pollEvent(this->ev))
	{
		/*Analizar el evento del sistema, si la ventana
		requiere un cierre o si se presiono la tecla esc debe cerrar la misma
		mientras tanto, sigue ejecutando
		*/
		switch (this->ev.type)
		{
		case this->ev.Closed:
			this->window->close();
			break;
		case this->ev.KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape) {
				this->window->close();
			}
			break;
		}
	}
}




//Init


void Game::initVariables()
{
	//Inicializar variables del juego
	this->points = 0;
	this->maxEnemySpawnTimer = 40.f;
	this->enemySpawnTimer = this->maxEnemySpawnTimer;
	this->maxEnemies = 5;
	this->mouseHold = false;
	this->health = 20;
	bool endGame = false;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new sf::RenderWindow(this->videoMode, "Juego 1", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(75);
}

void Game::initEnemies()
{
	this->enemy.setFillColor(sf::Color::Cyan);
	this->enemy.setSize(sf::Vector2f(30.f, 30.f));

}

void Game::initFonts()
{	
	std::string filePath = "Fonts/DS-DIGIB.ttf";

	this->font.loadFromFile(filePath);
	
}

void Game::initTexts()
{
	//Iniciar la interfaz para los puntos
	this->textPoints.setFont(this->font);
	this->textPoints.setFillColor(sf::Color::White);
	this->textPoints.setCharacterSize(30);
	this->textPoints.setString("1/0");
}


//Updates


void Game::update()
{
	this->pollEvents();

	if (!this->endGame)
	{
	this->updateMousPos();

	this->updateEnemies();

	this->updateHUD();
	}
	if (this->health <= 0)  
	{
		endGame = true;
	}
}

void Game::updateMousPos()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
	/*Verificar si transcurrio el tiempo necesario para volver a spawnear un enemigo
	* si todavia no es tiempo, se suma 1 unidad (equivalente a  lo que dure una oscilacion)
	* ademas debe considerar el maximo de enemigos permitidos*/

	bool deleted;

	if (this->enemies.size() < this->maxEnemies) {

		if (this->enemySpawnTimer >= this->maxEnemySpawnTimer) {
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else {
			this->enemySpawnTimer += 1.f;
		}	
	}
	//for para mover todos los objetos del vector una posicion mas abajo
	for (int i =0 ; i < this->enemies.size();i++)
	{
		 deleted = false;

		this->enemies[i].move(0.f, 3.f);
		//check si esta fuera de la ventana de juego

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			deleted = true;
			this->health -= 1;
		}
		
		if (deleted)  this->enemies.erase(this->enemies.begin() + i); 
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		
		if (this->mouseHold == false)
		{
			deleted = false;
			this->mouseHold = true;
			for (size_t i = 0; i < this->enemies.size() && deleted == false ; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					deleted = true;
					this->points += 1;
					
				}
				if (deleted)  this->enemies.erase(this->enemies.begin() + i);
			}
		}
		
	}
	else { 
		this->mouseHold = false; }

}

void Game::updateHUD()
{
	std::stringstream ss;
	//Texto que quiero que se muestre en pantalla (uso sstream para combinar str con int)
	ss << "Points: " << this->points<<"\n"
	<< "Health: "<<this->health<<"\n";
	//castear ss a string y guardarlo en la variable tipo text para que sea dibujable
	this->textPoints.setString(ss.str());
	
}


//renders


void Game::render()
{
	/*
	-Limpiar dibujo anterior
	-Renderizar objetos
	-Dibujar en pantalla
	*/
	this->window->clear();

	//Dibujar objetos

	this->renderEnemies(*this->window);
	this->renderHUD(*this->window);
	//Representarlos en la pantalla
	this->window->display();

}



void Game::renderEnemies(sf::RenderTarget& target)
{
	//renderizar todos los enemigos
	//dibujar todos los objetos del vector
	for (auto& e : enemies)
	{
		target.draw(e);
	}

}

void Game::renderHUD(sf::RenderTarget& target)
{
	target.draw(this->textPoints);
}



//funciones del juego


void Game::spawnEnemy()
{
	/*
		Spawnear un enemigo :
		atribuirle aleatoriamente valores a su:
		- tamanio 
		- color 
		- posicion inicial
		agregarlo a el vector enemies
	*/

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		static_cast<float>(0.f)
	);
	

	int tipe = rand() % 5;

	switch (tipe)
	{
		case 0: 
			this->enemy.setFillColor(sf::Color::Green);
			this->enemy.setSize(sf::Vector2f(20.f, 20.f));
			break;
		case 1:
			this->enemy.setFillColor(sf::Color::Yellow);
			this->enemy.setSize(sf::Vector2f(30.f, 30.f));
			break;
		case 2:
			this->enemy.setFillColor(sf::Color::Red);
			this->enemy.setSize(sf::Vector2f(40.f,40.f));
			break;
		case 3:
			this->enemy.setFillColor(sf::Color::Cyan);
			this->enemy.setSize(sf::Vector2f(80.f, 80.f));
			break;
		case 4:
			this->enemy.setFillColor(sf::Color::Magenta);
			this->enemy.setSize(sf::Vector2f(70.f, 70.f));
			break;
		default:
			this->enemy.setFillColor(sf::Color::Cyan);
			this->enemy.setSize(sf::Vector2f(100.f, 100.f));
			break;
	}
	this->enemies.push_back(enemy);
}
