#include "GameObjects.h"

#include <Windows.h>
#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <cmath>
#define pi 3.14159265f

float *getvertexpositions(float x_Cord, float y_Cord, float x_Span, float y_Span)
{

	float *ptr = new float[16];

	//0
	ptr[0] = -x_Span / 2 + x_Cord;
	ptr[1] = -y_Span / 2 + y_Cord;
	ptr[2] =  0.0f;
	ptr[3] =  0.0f;
	//1
	ptr[4] =  x_Span / 2 + x_Cord;
	ptr[5] = -y_Span / 2 + y_Cord;
	ptr[6] =  1.0f;
	ptr[7] =  0.0f;
	//2
	ptr[8] =  x_Span / 2 + x_Cord;
	ptr[9] =  y_Span / 2 + y_Cord;
	ptr[10] = 1.0f;
	ptr[11] = 1.0f;
	//3
	ptr[12] = -x_Span / 2 + x_Cord;
	ptr[13] =  y_Span / 2 + y_Cord;
	ptr[14] =  0.0f;
	ptr[15] =  1.0f;
	
	return ptr;
}

void draw(const std::string & path, float *vertices)
{
	unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
	};

	VertexArray va;
	VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	glm::mat4 proj = glm::ortho(-960.0f, 960.0f, -540.0f, 540.0f, -1.0f, 1.0f);

	Shader shader("res/shaders/basic01.shader");
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", proj);

	Texture texture(path);
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;
	renderer.Draw(va, ib, shader);
}

vector::vector(float angle)
{
	m_angle = angle;
}

void vector::setangle(float angle)
{
	m_angle = angle;
}

float vector::getsin()
{
	return sin(m_angle * pi / 180);
}

float vector::getcos()
{
	return cos(m_angle * pi / 180);
}

elements::elements(float xCord, float yCord, float xSpan, float ySpan, float speed)
{
	m_xCord = xCord;
	m_yCord = yCord;
	m_speed = speed;
	m_xSpan = xSpan;
	m_ySpan = ySpan;
	m_vertexpositions = getvertexpositions(m_xCord, m_yCord, m_xSpan, m_ySpan);
}

redfighter::redfighter() : elements(0.0f, -390.0f, 200.0f, 200.0f, 10.0f)
{
	m_state = redfighterstate(4);
}

void redfighter::move(direction dir_arg)
{
	switch (dir_arg)
	{
	case RIGHT:
		if(m_xCord + m_xSpan / 2 + m_speed < 960.0f)
			m_xCord += m_speed;
		break;
	case LEFT:
		if (m_xCord - m_xSpan / 2 - m_speed > -960.0f)
			m_xCord -= m_speed;
		break;
	case UP:
		if (m_yCord + m_ySpan / 2 + m_speed < 540.0f)
			m_yCord += m_speed;
		break;
	case DOWN:
		if (m_yCord - m_ySpan / 2 - m_speed > -540.0f)
			m_yCord -= m_speed;
		break;
	default:
		break;
	}

	delete[] m_vertexpositions;
	m_vertexpositions = getvertexpositions(m_xCord, m_yCord, m_xSpan, m_ySpan);
}

void redfighter::changestate(bool right)
{
	if ((right) && (int(m_state) < 8))
	{
		m_state = redfighterstate(int(m_state) + 1);
	}
	else if ((!right) && (int(m_state) > 0))
	{
		m_state = redfighterstate(int(m_state) - 1);
	}
}

redfighterstate redfighter::getstate()
{
	return (m_state);
}

void redfighter::render()
{
	redfighterstate currentstate = getstate();
	switch (currentstate)
	{
	case left01:
		m_path = "res/textures/redfighter/redfighter0001.png";
		break;
	case left02:
		m_path = "res/textures/redfighter/redfighter0002.png";
		break;
	case left03:
		m_path = "res/textures/redfighter/redfighter0003.png";
		break;
	case left04:
		m_path = "res/textures/redfighter/redfighter0004.png";
		break;
	case center:
		m_path = "res/textures/redfighter/redfighter0005.png";
		break;
	case right04:
		m_path = "res/textures/redfighter/redfighter0006.png";
		break;
	case right03:
		m_path = "res/textures/redfighter/redfighter0007.png";
		break;
	case right02:
		m_path = "res/textures/redfighter/redfighter0008.png";
		break;
	case right01:
		m_path = "res/textures/redfighter/redfighter0009.png";
		break;
	default:
		m_path = "";
		break;
	}
	draw(m_path, m_vertexpositions);
}

spaceship::spaceship(ship type, float x_Cord, float y_Cord, float speed) : elements(x_Cord, y_Cord, 200.0f, 200.0f, speed)
{
	switch (type)
	{
	case cargoship:
		m_path = "res/textures/enemies/cargoship.png";
		break;
	case carrier:
		m_path = "res/textures/enemies/carrier.png";
		break;
	case cruiser:
		m_path = "res/textures/enemies/cruiser.png";
		break;
	case destroyer:
		m_path = "res/textures/enemies/destroyer.png";
		break;
	case shuttle:
		m_path = "res/textures/enemies/shuttle.png";
		break;
	case tribase0001:
		m_path = "res/textures/enemies/tribase0001.png";
		break;
	case tribase0002:
		m_path = "res/textures/enemies/tribase0002.png";
		break;
	case tribase0003:
		m_path = "res/textures/enemies/tribase0003.png";
		break;
	default:
		m_path = "";
		break;
	}

	m_vertexpositions = getvertexpositions(m_xCord, m_yCord, m_xSpan, m_ySpan);
}

void spaceship::move()
{
	m_xCord += m_speed * m_dir.getsin();
	m_yCord -= m_speed * m_dir.getcos();
	delete[] m_vertexpositions;
	m_vertexpositions = getvertexpositions(m_xCord, m_yCord, m_xSpan, m_ySpan);
}

void spaceship::render()
{
	draw(m_path, m_vertexpositions);
}

background::background() : elements(0.0f, 0.0f, 1920.0f, 1920.0f, 0.0f)
{
	m_vertexpositions = getvertexpositions(m_xCord, m_yCord, m_xSpan, m_ySpan);
	m_path = "res/textures/background/space.png";
}

void background::render()
{
	draw(m_path, m_vertexpositions);
}

bullet::bullet(bullettype type, bulletcolor color, float x_Cord, float y_Cord, float speed) :elements(x_Cord, y_Cord, 100.0f, 100.0f, speed)
{
	switch (color)
	{
	case blue:
		switch (type)
		{
		case type01:
			m_path = "res/textures/bullets/bullet0101.png";
			break;
		case type02:
			m_path = "res/textures/bullets/bullet0102.png";
			break;
		case type03:
			m_path = "res/textures/bullets/bullet0103.png";
			break;
		case type04:
			m_path = "res/textures/bullets/bullet0104.png";
			break;
		case type05:
			m_path = "res/textures/bullets/bullet0105.png";
			break;
		case type06:
			m_path = "res/textures/bullets/bullet0106.png";
			break;
		case type07:
			m_path = "res/textures/bullets/bullet0107.png";
			break;
		case type08:
			m_path = "res/textures/bullets/bullet0108.png";
			break;
		default:
			break;
		}
		break;
	case red:
		switch (type)
		{
		case type01:
			m_path = "res/textures/bullets/bullet0201.png";
			break;
		case type02:
			m_path = "res/textures/bullets/bullet0202.png";
			break;
		case type03:
			m_path = "res/textures/bullets/bullet0203.png";
			break;
		case type04:
			m_path = "res/textures/bullets/bullet0204.png";
			break;
		case type05:
			m_path = "res/textures/bullets/bullet0205.png";
			break;
		case type06:
			m_path = "res/textures/bullets/bullet0206.png";
			break;
		case type07:
			m_path = "res/textures/bullets/bullet0207.png";
			break;
		case type08:
			m_path = "res/textures/bullets/bullet0208.png";
			break;
		default:
			break;
		}
		break;
	case orange:
		switch (type)
		{
		case type01:
			m_path = "res/textures/bullets/bullet0301.png";
			break;
		case type02:
			m_path = "res/textures/bullets/bullet0302.png";
			break;
		case type03:
			m_path = "res/textures/bullets/bullet0303.png";
			break;
		case type04:
			m_path = "res/textures/bullets/bullet0304.png";
			break;
		case type05:
			m_path = "res/textures/bullets/bullet0305.png";
			break;
		case type06:
			m_path = "res/textures/bullets/bullet0306.png";
			break;
		case type07:
			m_path = "res/textures/bullets/bullet0307.png";
			break;
		case type08:
			m_path = "res/textures/bullets/bullet0308.png";
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	m_dir.setangle(180.0f);
	m_vertexpositions = getvertexpositions(m_xCord, m_yCord, m_xSpan, m_ySpan);
}

void bullet::move()
{
	m_xCord += m_speed * m_dir.getsin();
	m_yCord -= m_speed * m_dir.getcos();
	delete[] m_vertexpositions;
	m_vertexpositions = getvertexpositions(m_xCord, m_yCord, m_xSpan, m_ySpan);
}

void bullet::render()
{
	draw(m_path, m_vertexpositions);
}

gameinstance::gameinstance()
{
	for (int i = 0; i < 3; i++)
	{
		spaceship *newenemy = new spaceship(ship(i % 8), i * 100.0f, i * -100.0f, 2.0f);
		m_enemies.push_back(*newenemy);
	}
	for (int i = 0; i < 30; i++)
	{
		bullet *newbullet = new bullet(bullettype(i % 8), bulletcolor(i % 3), i * -50.0f, i * 0.0f, 3.0f);
		m_bullets.push_back(*newbullet);
	}

}

void gameinstance::advance()
{
	for (std::list<spaceship>::iterator itr = m_enemies.begin(); itr != m_enemies.end(); itr++)
	{
		(*itr).move();
	}
	for (std::list<bullet>::iterator itr = m_bullets.begin(); itr != m_bullets.end(); itr++)
	{
		(*itr).move();
	}
	if (GetAsyncKeyState(VK_LEFT) && 0x8000)
	{
		m_player.move(direction::LEFT);
		if (GetAsyncKeyState(VK_LSHIFT) && 0x8000)
			m_player.move(direction::LEFT);
		m_player.changestate(false);
	}
	else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
	{
		m_player.move(direction::RIGHT);
		if (GetAsyncKeyState(VK_LSHIFT) && 0x8000)
			m_player.move(direction::RIGHT);
		m_player.changestate(true);
	}
	else if (int(m_player.getstate()) < 4)
	{
		m_player.changestate(true);
	}
	else if (int(m_player.getstate()) > 4)
	{
		m_player.changestate(false);
	}

	if (GetAsyncKeyState(VK_UP) && 0x8000)
	{
		m_player.move(direction::UP);
		if (GetAsyncKeyState(VK_LSHIFT) && 0x8000)
			m_player.move(direction::UP);
	}
	else if (GetAsyncKeyState(VK_DOWN) && 0x8000)
	{
		m_player.move(direction::DOWN);
		if (GetAsyncKeyState(VK_LSHIFT) && 0x8000)
			m_player.move(direction::DOWN);
	}
}

void gameinstance::render()
{
	m_newbackground.render();
	m_player.render();
	for (std::list<spaceship>::iterator itr = m_enemies.begin(); itr != m_enemies.end(); itr++)
	{
		(*itr).render();
	}
	for (std::list<bullet>::iterator itr = m_bullets.begin(); itr != m_bullets.end(); itr++)
	{
		(*itr).render();
	}
}
