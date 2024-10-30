#include "GSource.h"
#include <GAnimation.h>
GSource &getSource() { return GSource::getSource(); }

sf::Sprite &getSprite(int id) { return getSource().getSprite(id); }
