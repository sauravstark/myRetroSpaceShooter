#pragma once
#include <string>
#include <list>

enum direction {RIGHT, LEFT, UP, DOWN};
enum redfighterstate {left01, left02, left03, left04, center, right04, right03, right02, right01};
enum ship {cargoship, carrier, cruiser, destroyer, shuttle, tribase0001, tribase0002, tribase0003};
enum bullettype {type01, type02, type03, type04, type05, type06, type07, type08};
enum bulletcolor {blue, red, orange};

class vector {
private:
	float m_angle;
public:
	vector(float angle = 0.0f);
	void setangle(float angle);
	float getsin();
	float getcos();
};

class elements {
protected:
	float m_xCord, m_yCord;
	float m_xSpan, m_ySpan;
	float m_speed;
	float *m_vertexpositions;
	std::string m_path;
public:
	elements(float x_arg, float y_arg, float x_arg2, float y_arg2, float speed);
	virtual void render() = 0;
};

class redfighter:public elements {
private:
	redfighterstate m_state;
public:
	redfighter();
	void move(direction dir_arg);
	void changestate(bool right);
	redfighterstate getstate();
	void render();
};

class spaceship: public elements {
private:
	vector m_dir;
public:
	spaceship(ship type, float x_Cord, float y_Cord, float speed);
	void move();
	void render();
};

class bullet: public elements {
private:
	vector m_dir;
public:
	bullet(bullettype type, bulletcolor color, float x_Cord, float y_Cord, float speed);
	void move();
	void render();
};

class background: public elements {
public:
	background();
	void render();
};

class gameinstance {
private:
	redfighter m_player;
	std::list<spaceship> m_enemies;
	std::list<bullet> m_bullets;
	background m_newbackground;
public:
	gameinstance();
	void advance();
	void render();
};
