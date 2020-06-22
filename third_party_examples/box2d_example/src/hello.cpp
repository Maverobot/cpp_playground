#include "box2dsim.h"

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

  Box2DSim sim;
  sim.run(world);

  /**
   * TODO:
   * 1. Expose step callback function to apply forces on dynamics bodys
   */
}
