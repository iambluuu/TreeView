#include "Node.h"

bool GraphNode::isHover(sf::Vector2f mousePos) {
	sf::FloatRect hitbox;

	hitbox.left = pos.x - 23;
	hitbox.top = pos.y - 23;
	hitbox.width = 46;
	hitbox.height = 46;

	return hitbox.contains(mousePos);
}

void GraphNode::OnClick() {
	isHeld = 1;
	isFixed = 1;
}

void GraphNode::OnRelease() {
	isHeld = 0;
	isFixed = 0;
}

void GraphNode::HandleEvent(const sf::Event* l_event, sf::RenderWindow* l_target, sf::Vector2f offset) {
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*l_target));

	if (isHover(mousePos + offset)) {
		if (l_event->type == sf::Event::MouseButtonPressed && l_event->key.code == sf::Mouse::Left) {
			OnClick();
		}
		else if (l_event->type == sf::Event::MouseButtonReleased && l_event->key.code == sf::Mouse::Left) {
			OnRelease();
		}
	}

	if (isHeld) {
		pos = mousePos + offset;
	}
}