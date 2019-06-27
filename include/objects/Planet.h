//
// Created by jprolejko on 26.02.19.
//

#ifndef GRAVITY_OBJECTS_PLANET_H
#define GRAVITY_OBJECTS_PLANET_H

#include <objects/AbstractCelestialBody.h>

#include <ResourceManager.h>


class Planet : public AbstractCelestialBody {

public:
    Planet() = default;

    Planet(const sf::Vector2f &position, const sf::Vector2f &velocity, float radius);

    void update(float time_elapsed);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};


#endif // GRAVITY_OBJECTS_PLANET_H
