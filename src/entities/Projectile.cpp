#include "Projectile.h"

Projectile::Projectile(){
}

Projectile::Projectile(const ProjArgs& projectile, const CommonArgs& common)
	: projectile(projectile), common(common) {
}

Projectile::~Projectile() {
}