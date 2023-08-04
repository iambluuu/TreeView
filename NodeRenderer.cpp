#include "NodeRenderer.h"
#include <algorithm>

const sf::Color Black = sf::Color::Black;
const sf::Color White = sf::Color::White;
const sf::Color Green = sf::Color(103, 137, 131);

const sf::Color DarkBlue = sf::Color(45, 55, 73);
const sf::Color LightBlue = sf::Color(53, 66, 89);
const sf::Color LightBeige = sf::Color(240, 233, 210);

void NodeRenderer::PrepareSprite() {

	sf::Sprite Border(m_texture);
	sf::Sprite Fill(m_texture);

	auto NodeColors0 = &m_nodeColor[0];
	NodeColors0->emplace( NodeState::Default, std::tuple<sf::Color, sf::Color, sf::Color>(DarkBlue, LightBeige, DarkBlue) );
	NodeColors0->emplace( NodeState::Selected, std::tuple<sf::Color, sf::Color, sf::Color>(Green, Green, White) );
	NodeColors0->emplace( NodeState::Visited, std::tuple<sf::Color, sf::Color, sf::Color>(Green, White, Green) );

	int sizeX = 46;
	int sizeY = 46;
	Border.setTextureRect(sf::IntRect(0, 0, sizeX, sizeY));
	Fill.setTextureRect(sf::IntRect(sizeX, 0, sizeX, sizeY));
	Border.setOrigin(Border.getLocalBounds().left + Border.getLocalBounds().width / 2, Border.getLocalBounds().top + Border.getLocalBounds().height / 2);
	Fill.setOrigin(Fill.getLocalBounds().left + Fill.getLocalBounds().width / 2, Fill.getLocalBounds().top + Fill.getLocalBounds().height / 2);

	m_nodeGraphics[0] = std::make_pair(Border, Fill);

	sizeX = 92;
	Border.setTextureRect(sf::IntRect(0, sizeY * 1, sizeX, sizeY));
	Fill.setTextureRect(sf::IntRect(sizeX, sizeY * 1, sizeX, sizeY));
	Border.setOrigin(Border.getLocalBounds().left + Border.getLocalBounds().width / 2, Border.getLocalBounds().top + Border.getLocalBounds().height / 2);
	Fill.setOrigin(Fill.getLocalBounds().left + Fill.getLocalBounds().width / 2, Fill.getLocalBounds().top + Fill.getLocalBounds().height / 2);

	m_nodeGraphics[1] = std::make_pair(Border, Fill);

	sizeX = 120;
	Border.setTextureRect(sf::IntRect(0, sizeY * 2, sizeX, sizeY));
	Fill.setTextureRect(sf::IntRect(sizeX, sizeY * 2, sizeX, sizeY));
	Border.setOrigin(Border.getLocalBounds().left + Border.getLocalBounds().width / 2, Border.getLocalBounds().top + Border.getLocalBounds().height / 2);
	Fill.setOrigin(Fill.getLocalBounds().left + Fill.getLocalBounds().width / 2, Fill.getLocalBounds().top + Fill.getLocalBounds().height / 2);

	m_nodeGraphics[2] = std::make_pair(Border, Fill);

	sizeX = 160;
	Border.setTextureRect(sf::IntRect(0, sizeY * 3, sizeX, sizeY));
	Fill.setTextureRect(sf::IntRect(sizeX, sizeY * 3, sizeX, sizeY));
	Border.setOrigin(Border.getLocalBounds().left + Border.getLocalBounds().width / 2, Border.getLocalBounds().top + Border.getLocalBounds().height / 2);
	Fill.setOrigin(Fill.getLocalBounds().left + Fill.getLocalBounds().width / 2, Fill.getLocalBounds().top + Fill.getLocalBounds().height / 2);

	m_nodeGraphics[3] = std::make_pair(Border, Fill);

	std::cerr << "NodeRenderer: Sprite prepared" << m_nodeGraphics.size() << std::endl;
}

void NodeRenderer::Reset(const int& l_stepNum) {
	
	m_curStep = 0;
	m_animationCurrent = 0;
	m_stepNum = l_stepNum;
	m_limitStep = l_stepNum;
}

void NodeRenderer::OnPlay() {
	is_paused = !is_paused;
	is_reverse = 0;
	step_by_step = 0;

	std::cerr << "Is Paused: " << is_paused << std::endl;
}

void NodeRenderer::OnForward() {
	is_paused = 1;

	if (!step_by_step || is_reverse) {
		m_limitStep = std::min(m_curStep + 1, m_stepNum);

		if (!step_by_step)
			m_animationCurrent = m_limitStep * STEP_DURATION;

		step_by_step = 1;
		is_reverse = 0;
		return;
	}

	if (m_limitStep == m_stepNum) {
		m_animationCurrent = m_stepNum * STEP_DURATION;
		return;
	}


	m_limitStep++;
	m_animationCurrent = (m_limitStep - 1) * STEP_DURATION;
}

void NodeRenderer::OnBackward() {
	is_paused = 1;

	if (!step_by_step || !is_reverse) {
		m_limitStep = m_curStep;

		if (!step_by_step)
			m_animationCurrent = m_limitStep * STEP_DURATION;

		step_by_step = 1;
		is_reverse = 1;
		return;
	}

	if (m_limitStep == 0) {
		m_animationCurrent = 0;
		return;
	}

	m_animationCurrent = m_limitStep * STEP_DURATION;
	m_limitStep--;
}

void NodeRenderer:: HandleEvent(sf::Event* l_event) {

	if (l_event->type == sf::Event::KeyPressed) {
		switch (l_event->key.code) {
		case sf::Keyboard::Space:
			OnPlay();
			break;
		case sf::Keyboard::Right:
			OnForward();
			break;
		case sf::Keyboard::Left:
			OnBackward();
			break;
		}
	}
}

void NodeRenderer::Update(const float& l_fT) {


	if (step_by_step) {
		if (m_animationCurrent > m_limitStep * STEP_DURATION) {
			m_animationCurrent = std::max(m_animationCurrent - l_fT * m_speedupRate, m_limitStep * STEP_DURATION);
		}
		else {
			m_animationCurrent = std::min(m_animationCurrent + l_fT * m_speedupRate, m_limitStep * STEP_DURATION);
		}
	}
	else {
		if (is_paused)
			return;

		if (!is_reverse)
			m_animationCurrent = std::min(m_animationCurrent + l_fT * m_speedupRate, m_stepNum * STEP_DURATION);
		else
			m_animationCurrent = std::max(m_animationCurrent - l_fT * m_speedupRate, 0.f);
	}


	m_curStep = GetStep();
}

int NodeRenderer::GetStep() {

	return std::min(m_stepNum - 1, (int)floor(m_animationCurrent / STEP_DURATION));
}

NodeGraphics* NodeRenderer::GetNodeGraphics(int valueNum) {
	return &m_nodeGraphics[valueNum - 1];
}

void NodeRenderer::DrawNode(Node* Cur) {
	if (!Cur)
		return;

	std::vector<NodeInfo>* info = Cur->getInfo();
	int CurStep = m_curStep;

	NodeInfo CurInfo = info->at(CurStep);
	if (!CurInfo.is_visible)
		return;

	float CurStepElapsed = m_animationCurrent - STEP_DURATION * CurStep;
	float percent = CurStepElapsed / STEP_DURATION;


	SharedContext* context = m_stateManager->GetContext();
	sf::RenderWindow* wind = context->m_wind->GetRenderWindow();

	NodeGraphics* CurSprite = GetNodeGraphics(CurInfo.value_num); //do something here 
	sf::Sprite* BorderSprite = &CurSprite->first;
	sf::Sprite* FillerSprite = &CurSprite->second;

	m_label.setString(std::to_string(CurInfo.m_shownValue[0]));
	m_label.setOrigin(m_label.getLocalBounds().left + m_label.getLocalBounds().width / 2, m_label.getLocalBounds().top + m_label.getLocalBounds().height / 2);

	if (CurInfo.is_splitting) {

		return;
	}

	sf::Vector2f coord = GetPosOnScreen(CurInfo.m_coord.first);
	if (CurInfo.is_moving) {
		sf::Vector2f startCoord = GetPosOnScreen(CurInfo.m_coord.first);
		sf::Vector2f endCoord = GetPosOnScreen(CurInfo.m_coord.second);

		coord = startCoord + (endCoord - startCoord) * percent;
	}

	BorderSprite->setPosition(coord);
	FillerSprite->setPosition(coord);
	m_label.setPosition(coord);


	if (CurInfo.is_appearing) {
		if (CurInfo.is_appearing == 1) {
			m_label.setScale(percent, percent);
			BorderSprite->setScale(percent, percent);
			FillerSprite->setScale(percent, percent);
		}
		else {
			m_label.setScale(1 - percent, 1 - percent);
			BorderSprite->setScale(1 - percent, 1 - percent);
			FillerSprite->setScale(1 - percent, 1 - percent);
		}
		
	}
	else {
		m_label.setScale(1, 1);
		BorderSprite->setScale(1, 1);
		FillerSprite->setScale(1, 1);
	}

	
	if (CurInfo.is_stateChanging) {
		auto startColor = GetNodeColor(0, CurInfo.node_state.first); //subject to change, 0 to theme id
		auto endColor = GetNodeColor(0, CurInfo.node_state.second); //subject to change, 0 to theme id

		BorderSprite->setColor(GetColorTransition(percent, std::get<0>(*startColor), std::get<0>(*endColor)));
		FillerSprite->setColor(GetColorTransition(percent, std::get<1>(*startColor), std::get<1>(*endColor)));
		m_label.setFillColor(GetColorTransition(percent, std::get<2>(*startColor), std::get<2>(*endColor)));
	}
	else {
		auto color = GetNodeColor(0, CurInfo.node_state.first); //subject to change, 0 to theme id

		BorderSprite->setColor(std::get<0>(*color));
		FillerSprite->setColor(std::get<1>(*color));
		m_label.setFillColor(std::get<2>(*color));
	}

	wind->draw(*FillerSprite);
	wind->draw(*BorderSprite);


	float TopPos = coord.y;
	float LeftPos = coord.x - (BorderSprite->getLocalBounds().width / 2);
	float spacing = BorderSprite->getLocalBounds().width / (CurInfo.value_num + 1);

	for (int j = 0; j < CurInfo.m_shownValue.size(); j++) {
		LeftPos += spacing;

		if (CurInfo.is_valueChanging && CurInfo.m_shownValue[j] == CurInfo.m_valueChange.first) {
			auto color = GetNodeColor(0, CurInfo.node_state.first);
			
			sf::Color fading = GetColorTransition(percent, std::get<2>(*color), sf::Color::Transparent);

			m_label.setString(std::to_string(CurInfo.m_valueChange.first));
			m_label.setOrigin(m_label.getLocalBounds().left + m_label.getLocalBounds().width / 2, m_label.getLocalBounds().top + m_label.getLocalBounds().height / 2);
			m_label.setPosition(sf::Vector2f(LeftPos, TopPos));
			m_label.setFillColor(fading);

			wind->draw(m_label);

			sf::Color appearing = GetColorTransition(percent, sf::Color::Transparent, std::get<2>(*color));

			m_label.setString(std::to_string(CurInfo.m_valueChange.second));
			m_label.setOrigin(m_label.getLocalBounds().left + m_label.getLocalBounds().width / 2, m_label.getLocalBounds().top + m_label.getLocalBounds().height / 2);
			m_label.setPosition(sf::Vector2f(LeftPos, TopPos));
			m_label.setFillColor(appearing);

			wind->draw(m_label);

			continue;
		}

		m_label.setString(std::to_string(CurInfo.m_shownValue[j]));
		m_label.setOrigin(m_label.getLocalBounds().left + m_label.getLocalBounds().width / 2, m_label.getLocalBounds().top + m_label.getLocalBounds().height / 2);
		m_label.setPosition(sf::Vector2f(LeftPos, TopPos));

		wind->draw(m_label);
	}

	if (Cur->m_type == 0 && CurInfo.node_state.second == NodeState::Selected) {
		m_sideLabel.setString("bf = " + std::to_string(CurInfo.m_bf));
	
		if (CurInfo.m_bf > 1 || CurInfo.m_bf < -1)
			m_sideLabel.setFillColor(sf::Color::Red);
		else
			m_sideLabel.setFillColor(sf::Color::Black);

		m_sideLabel.setOrigin(m_sideLabel.getLocalBounds().left + m_sideLabel.getLocalBounds().width / 2, m_sideLabel.getLocalBounds().top + m_sideLabel.getLocalBounds().height / 2);
		m_sideLabel.setPosition(coord.x - 35, coord.y - 30);
		wind->draw(m_sideLabel);
	}
}

void NodeRenderer::DrawTree(Node* Root)
{
	if (!Root)
		return;

	DrawNode(Root);
	DrawTree(Root->left);
	DrawTree(Root->right);
}

sf::Vector2f NodeRenderer::GetPosOnScreen(std::pair<int, int> treeCoord) {
	float X = MIDDLE_LINE + (treeCoord.first - 0.5) * HORIZONTAL_SPACING;
	float Y = TOP_LINE + treeCoord.second * VERTICAL_SPACING;

	return sf::Vector2f(X, Y);
}

sf::Color NodeRenderer::GetColorTransition(float percent, const sf::Color& start, const sf::Color& end) {
	sf::Color res = sf::Color(0, 0, 0);
	if (start.r > end.r) {
		res.r = start.r - (start.r - end.r) * percent;
	}
	else {
		res.r = start.r + (end.r - start.r) * percent;
	}

	if (start.g > end.g) {
		res.g = start.g - (start.g - end.g) * percent;
	}
	else {
		res.g = start.g + (end.g - start.g) * percent;
	}

	if (start.b > end.b) {
		res.b = start.b - (start.b - end.b) * percent;
	}
	else {
		res.b = start.b + (end.b - start.b) * percent;
	}

	return res;
}

std::tuple<sf::Color, sf::Color, sf::Color>* NodeRenderer::GetNodeColor(int ThemeID, NodeState l_state) {
	NodeColor* nodeColor = &m_nodeColor;
	return &nodeColor->at(ThemeID)[l_state];
}