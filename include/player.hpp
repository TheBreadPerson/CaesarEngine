#pragma once
#include <transform.hpp>
#include <entity.hpp>
#include <physics.hpp>

class Player
{
public:
	Entity entity;

	Player() : entity() {};

	void init();
	void move();
};