#include "Menu.h"

void Menu::Activate() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	renderer->Reset(0);

	codeWindow->Clear();
	renderer->OnSkipForward();
}