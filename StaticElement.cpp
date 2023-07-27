#include "StaticElement.h"

StaticElement::StaticElement(UIManager* l_owner){
	m_owner = l_owner;
	m_themeManager = m_owner->GetThemeManager();
	m_sprite = m_themeManager->GetSprite(m_themeID, ElementName::Background, ElementState::Neutral);
}

void StaticElement::HandleEvent(sf::Event* l_event){}
void StaticElement::OnHover(){}
void StaticElement::OnClick(){}
void StaticElement::OnRelease(){}
void StaticElement::OnLeave(){}

void StaticElement::Update(float l_dT){}

void StaticElement::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	wind->draw(*m_sprite);
}

void StaticElement::SetLayer(int l_layer) {
	m_layer = l_layer;
}