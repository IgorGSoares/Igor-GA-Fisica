#include "test.h"
#include<cmath>
#include "imgui/imgui.h"
#include<stdio.h>
#include <stdlib.h>
#include<iostream>

using namespace std;

b2Vec2 gravity;
float gs[6] = {0, -1.67, -3.721, -9.81, -11.15, -24.79};
bool moon = false, earth = true, zero = false, mars = false, netune = false, jupiter = false/*, sun = false*/;
//bool change = false;


enum tiposDeObjetos { BOTAO, PLAYER, RESTO };

struct userData {
	int ID;
};


class GA : public Test //você cria a sua classe derivada da classe base Test
{
public:

	b2Body* b1;

	b2Vec2 force;
	//b2Vec2 ptG;

	int count = 0;

	b2Body* magnet1;
	b2Body* magnet2;
	float magnetRadius = 5;
	float magnetForce1 = 2000; //- para atrair, + para afastar
	float magnetForce2 = -500;
	b2Vec2 magnetPos1/* = magnet.getWorldCenter()*/;
	b2Vec2 magnetPos2;
	b2Vec2 playerPos/* = player.getWorldCenter()*/;
	bool collision = false;

	b2Body* bt1;
	b2Body* bt2;

	//b2Fixture* m_sensor;

	bool podeRand = true;
	int r = 3;


	GA() {
		// Aqui no construtor você inicializa a cena
		userData* ud1 = new userData;
		ud1->ID = RESTO;
		uintptr_t resto = (uintptr_t)ud1;

		userData* ud2 = new userData;
		ud2->ID = PLAYER;
		uintptr_t player = (uintptr_t)ud2;

		userData* ud3 = new userData;
		ud3->ID = BOTAO;
		uintptr_t botao = (uintptr_t)ud3;

		//b1 = CreateBox(0.0f, 2.0f, 1.0f, 2.0f, 0.5f, 1.0f, 0.5f);
		CreateFloor(-30, 20, 2, 20);
		CreateFloor(30, 20, 2, 20);
		CreateFloor(0, 0, 30, 2);
		CreateFloor(0, 40, 30, 2);

		magnet1 = CreateCircle(-20, 15, 2);
		magnet2 = CreateCircle(20, 10, 2);

		b1 = CreateBox(7.0f, 1.0f, 1.0f, 1.0f, 0.9f, 1.0f, 0.5f, player);


		CreateFloor(10, 20, 5, 0.5);
		CreateFloor(-20, 30, 5, 0.5);

		//bt1 = CreateBox(10, 25, 0.5, 0.5, 0.9f, 1.0f, 0.5f, botao);
		//bt2 = CreateBox(-20, 35, 0.5, 0.5, 0.9f, 1.0f, 0.5f, botao);

		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.SetTwoSided(b2Vec2(-40.0f, -2.0f), b2Vec2(40.0f, -2.0f));
			ground->CreateFixture(&shape, 0.0f);
		}
		b2Body* body;
		body = CreateBox(-5.0f, 10.0f, 4, 0.5, 1.0f, 0.3f, 0.0f, resto);
		b2WheelJointDef jd;
		// Horizontal
		{
			jd.Initialize(ground, body, body->GetPosition(), b2Vec2(1.0f, 0.0f));
			jd.motorSpeed = 20.0f;
			jd.maxMotorTorque = 10000.0f;
			jd.enableMotor = true;
			jd.lowerTranslation = -3.0f;
			jd.upperTranslation = 3.0f;
			jd.enableLimit = true;
		}
		float hertz = 1.0f;
		float dampingRatio = 0.7f;
		b2LinearStiffness(jd.stiffness, jd.damping, hertz, dampingRatio, ground, body);
		b2WheelJoint* m_joint;
		m_joint = (b2WheelJoint*)m_world->CreateJoint(&jd);


		b2Body* body2;
		body2 = CreateBox(22.0f, 32.0f, 3, 0.5, 1.0f, 0.3f, 0.0f, resto);
		b2WheelJointDef jd2;
		// Horizontal
		{
			jd2.Initialize(ground, body2, body2->GetPosition(), b2Vec2(1.0f, 0.0f));
			jd2.motorSpeed = -20.0f;
			jd2.maxMotorTorque = 10000.0f;
			jd2.enableMotor = true;
			jd2.lowerTranslation = -3.0f;
			jd2.upperTranslation = 3.0f;
			jd2.enableLimit = true;
		}
		b2LinearStiffness(jd2.stiffness, jd2.damping, hertz, dampingRatio, ground, body2);
		b2WheelJoint* m_joint2;
		m_joint2 = (b2WheelJoint*)m_world->CreateJoint(&jd2);


		CreateSensor(10, 25, botao);
		CreateSensor(-20, 35, botao);
	}


	void UpdateUI() override
	{
		ImGui::SetNextWindowPos(ImVec2(10.0f, 100.0f));
		ImGui::SetNextWindowSize(ImVec2(200.0f, 200.0f));
		ImGui::Begin("Custom Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		if (ImGui::RadioButton("Zero", zero == true))
		{
			
			
			//sun = false;
		}

		if (ImGui::RadioButton("Moon", moon == true))
		{
			
			//sun = false;
		}

		if (ImGui::RadioButton("Mars", mars == true))
		{
			
			//sun = false;
		}

		if (ImGui::RadioButton("Earth", earth == true))
		{
			
			//sun = false;
		}

		if (ImGui::RadioButton("Netune", netune == true))
		{
			
			//sun = false;
		}

		if (ImGui::RadioButton("Jupiter", jupiter == true))
		{
			
			//sun = false;
		}

		/*if (ImGui::RadioButton("Sun", sun == true))
		{
			gravity = b2Vec2(0.0f, -274.0f);
			earth = false;
			moon = false;
			zero = false;
			mars = false;
			netune = false;
			jupiter = false;
			sun = true;
		}*/

		ImGui::End();
	}

	void Step(Settings& settings) override
	{

		magnetPos1 = magnet1->GetWorldCenter();
		playerPos = b1->GetWorldCenter();

		b2Vec2 magnetDist1 = b2Vec2((playerPos.x - magnetPos1.x), (playerPos.y - magnetPos1.y));
		float distance1 = (float)sqrt(pow(magnetDist1.x, 2) + pow(magnetDist1.y, 2));



		magnetPos2 = magnet2->GetWorldCenter();
		playerPos = b1->GetWorldCenter();
		b2Vec2 magnetDist2 = b2Vec2((playerPos.x - magnetPos2.x), (playerPos.y - magnetPos2.y));
		float distance2 = (float)sqrt(pow(magnetDist2.x, 2) + pow(magnetDist2.y, 2));


		if (distance1 <= magnetRadius) {
			float vecSum = abs(magnetDist1.x) + abs(magnetDist1.y);
			b2Vec2 force = b2Vec2((magnetForce1 * magnetDist1.x * ((1 / vecSum) * magnetRadius / distance1)),
				(magnetForce1 * magnetDist1.y * ((1 / vecSum) * magnetRadius / distance1)));
			//b2Vec2 force = magnetForce * magnetDist;
			b1->ApplyForceToCenter(force, true);
			//collision = true;
		}
		/*else {
			collision = false;
		}*/

		if (distance2 <= magnetRadius) {
			float vecSum = abs(magnetDist2.x) + abs(magnetDist2.y);
			b2Vec2 force = b2Vec2((magnetForce2 * magnetDist2.x * ((1 / vecSum) * magnetRadius / distance2)),
				(magnetForce2 * magnetDist2.y * ((1 / vecSum) * magnetRadius / distance2)));
			//b2Vec2 force = magnetForce * magnetDist;
			b1->ApplyForceToCenter(force, true);
			collision = true;
		}
		else {
			collision = false;
		}

		gravity = b2Vec2(0, gs[r]);
		m_world->SetGravity(gravity);

		switch (r)
		{
		case 0:
			earth = false;
			moon = false;
			zero = true;
			mars = false;
			netune = false;
			jupiter = false;
			break;
		case 1:
			earth = false;
			moon = true;
			zero = false;
			mars = false;
			netune = false;
			jupiter = false;
			break;
		case 2:
			earth = false;
			moon = false;
			zero = false;
			mars = true;
			netune = false;
			jupiter = false;
			break;
		case 3:
			earth = true;
			moon = false;
			zero = false;
			mars = false;
			netune = false;
			jupiter = false;
			break;
		case 4:
			earth = false;
			moon = false;
			zero = false;
			mars = false;
			netune = true;
			jupiter = false;
			break;
		case 5:
			earth = false;
			moon = false;
			zero = false;
			mars = false;
			netune = false;
			jupiter = true;
			break;
		}



		
		Test::Step(settings);



		//show some text in the main screen
		//g_debugDraw.DrawString(5, m_textLine, "Este e' um template para os exercicios!! :)");
		//m_textLine += 15;
	}

	static Test* Create()  //a classe Test que instancia um objeto da sua nova classe
						   //o autor da Box2D usa um padrão de projeto chamado Factory
						   //para sua arquitetura de classes
	{
		return new GA;
	}

	b2Body* CreateBox(float x, float y, float width, float height,
		float density, float friction, float restitution, uintptr_t ptrData)
	{
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		bodyDef.userData.pointer = ptrData;
		b2Body* body = m_world->CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(width, height);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = density;

		// Override the default friction.
		fixtureDef.friction = friction;

		//bounciness
		fixtureDef.restitution = restitution;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);

		return body;
	}

	b2Body* CreateFloor(float x, float y, float width, float height)
	{
		// Define the ground body.
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(x, y);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

		// Define the ground box shape.
		b2PolygonShape groundBox;

		// The extents are the half-widths of the box.
		groundBox.SetAsBox(width, height);

		// Add the ground fixture to the ground body.
		groundBody->CreateFixture(&groundBox, 0.0f);

		return groundBody;
	}

	void Keyboard(int key) override
	{
		switch (key)
		{
		case GLFW_KEY_W:
			if (count < 10)//count<2
			{
				//force = decomposeVector(90.0f, 3000.0f);
				if (collision == false)
				{
					force = decomposeVector(90.0f, 50.0f + count);
					count += 5;//count++
				}
				else
				{
					force = decomposeVector(90.0f, 125.0f);
					count = 10;
				}
				b1->ApplyForceToCenter(force, true);
				b1->ApplyLinearImpulse(force, b1->GetWorldCenter(), true);

			}
			else
			{
				count = 0;
			}
			break;

		case GLFW_KEY_A:
			//force = b2Vec2(-2000.0, 0.0);
			force = b2Vec2(-30.0, 0.0);
			/*ptG = b1->GetWorldPoint(b2Vec2(-0.5f, 2.0f));
			b1->ApplyForce(force, ptG, true);*/
			b1->ApplyForceToCenter(force, true);
			b1->ApplyLinearImpulse(force, b1->GetWorldCenter(), true);
			break;

		case GLFW_KEY_D:
			//force = b2Vec2(2000.0, 0.0);
			force = b2Vec2(30.0, 0.0);
			/*ptG = b1->GetWorldPoint(b2Vec2(-0.5f, 2.0f));
			b1->ApplyForce(force, ptG, true);*/
			b1->ApplyForceToCenter(force, true);
			b1->ApplyLinearImpulse(force, b1->GetWorldCenter(), true);
			break;

		}
	}

	b2Vec2 decomposeVector(float a, float m)
	{
		b2Vec2 dec;
		dec.x = m * cos(degToRadian(a));
		dec.y = m * sin(degToRadian(a));
		return dec;
	}

	float degToRadian(float a)
	{
		return a * b2_pi / 180.0f;
	}


	b2Body* CreateCircle(float x, float y, float radius) //static
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(x, y);
		b2Body* body = m_world->CreateBody(&bodyDef);

		b2CircleShape circle;
		circle.m_radius = radius;
		//circle.m_p.Set(0.0f, 2.0f); ???

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circle;

		body->CreateFixture(&fixtureDef);

		return body;
	}

	void BeginContact(b2Contact* contact)
	{
		srand(time(NULL));

		//Só pra debug
		b2Fixture* fa, * fb;
		fa = contact->GetFixtureA();
		fb = contact->GetFixtureB();

		b2Body* bodyA = NULL, * bodyB = NULL;
		bodyA = fa->GetBody();
		bodyB = contact->GetFixtureB()->GetBody();

		if (bodyA && bodyB)
		{
			//Checa se os objetos possuem userData
			uintptr_t bodyUserDataA = bodyA->GetUserData().pointer;
			uintptr_t bodyUserDataB = bodyB->GetUserData().pointer;

			if (bodyUserDataA && bodyUserDataB)
			{
				userData* udA = reinterpret_cast<userData*>(bodyUserDataA);
				userData* udB = reinterpret_cast<userData*>(bodyUserDataB);

				//Provoca dano no porco
				if (udA->ID == PLAYER && udB->ID == BOTAO || udA->ID == BOTAO && udB->ID == PLAYER)
				{
					if (podeRand == true)
					{
						podeRand = false;
						r = rand() % 6;
					}
				}
			}
		}
	}

	void EndContact(b2Contact* contact)
	{
		//Só pra debug
		b2Fixture* fa, * fb;
		fa = contact->GetFixtureA();
		fb = contact->GetFixtureB();

		b2Body* bodyA = NULL, * bodyB = NULL;
		bodyA = fa->GetBody();
		bodyB = contact->GetFixtureB()->GetBody();

		if (bodyA && bodyB)
		{
			//Checa se os objetos possuem userData
			uintptr_t bodyUserDataA = bodyA->GetUserData().pointer;
			uintptr_t bodyUserDataB = bodyB->GetUserData().pointer;

			if (bodyUserDataA && bodyUserDataB)
			{
				userData* udA = reinterpret_cast<userData*>(bodyUserDataA);
				userData* udB = reinterpret_cast<userData*>(bodyUserDataB);

				//Provoca dano no porco
				if (udA->ID == PLAYER && udB->ID == BOTAO || udA->ID == BOTAO && udB->ID == PLAYER)
				{
					podeRand = true;
				}
			}
		}
	}

	b2Body* CreateSensor(float x, float y, uintptr_t ptrData)
	{

		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(x, y);
		bodyDef.userData.pointer = ptrData;
		b2Body* body = m_world->CreateBody(&bodyDef);

		b2CircleShape shape;
		shape.m_radius = 1.0f;

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.isSensor = true;

		body->CreateFixture(&fd);

		return body;
		//m_sensor = ground->CreateFixture(&fd);
	}
};

//Aqui fazemos o registro do novo teste 
static int testIndex = RegisterTest("Examples", "GA", GA::Create);