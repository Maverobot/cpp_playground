#include "box2dsim.h"

#include <iostream>

int main(int, char**) {
  b2World world(b2Vec2(0.0f, -10.0f));

  // Ground body
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(0.0f, -10.0f);
  b2Body* groundBody = world.CreateBody(&groundBodyDef);

  // Ground fixture
  b2PolygonShape groundBox;
  groundBox.SetAsBox(50.0f, 10.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);

  // Dynamic body
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 40.0f);
  b2Body* body = world.CreateBody(&bodyDef);

  // Dynamic body fixture
  b2CircleShape dynamicCircle;
  dynamicCircle.m_radius = 1.0f;
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;

  body->CreateFixture(&fixtureDef);

  auto print_state = [&body] {
    body->ApplyForceToCenter(b2Vec2(1.0f, 0), true);
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
  };

  Box2DSim sim;
  sim.run(world, print_state);
}
