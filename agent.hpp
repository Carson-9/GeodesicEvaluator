#pragma once

enum Genome_enum {
	lifespan,
	speed,
	resourceStock,
	founderChance,

};

class Agent {
public:
	Agent();
	~Agent();




private:
	float genome[];
};