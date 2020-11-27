#include"includes/Input.h"
using namespace DirectX;

void Input::SetMouseMode(Mouse::Mode mode) {
	_pMouse->SetMode(mode);
}

Mouse::State Input::GetMouseState()const { return _pMouse->GetState(); }
Mouse::State Input::GetMouseLastState()const { return _MouseTracker.GetLastState(); }
Keyboard::State Input::GetKeyboardState()const { return _pKeyboard->GetState(); }
Keyboard::State Input::GetKeyboardLastState()const { return _KeyboardTracker.GetLastState(); }

bool Input::IsButtonUp(MouseButton button) const {
	MouseTrackerState state = GetButtonTackerState(button);
	return state == MouseTrackerState::UP;
}
bool Input::IsButtonDown(MouseButton button)const {
	MouseTrackerState state = GetButtonTackerState(button);
	return state == MouseTrackerState::HELD;
}
bool Input::IsButtonReleased(MouseButton button)const {
	MouseTrackerState state = GetButtonTackerState(button);
	return state == MouseTrackerState::RELEASED;
}
bool Input::IsButtonPressed(MouseButton button)const {
	MouseTrackerState state = GetButtonTackerState(button);
	return state == MouseTrackerState::PRESSED;
}

XMFLOAT2 Input::GetMousePosition() const
{
	Mouse::State currentState = GetMouseState();
	return XMFLOAT2(currentState.x, currentState.y);
}

XMFLOAT2 Input::GetMousePositionOffset() const
{
	Mouse::State lastState = GetMouseLastState();
	Mouse::State currentState = GetMouseState();
	return XMFLOAT2(currentState.x - lastState.x, currentState.y - lastState.y);
}

bool Input::IsKeyPressed(KeyCode key)const { return _KeyboardTracker.IsKeyPressed(static_cast<Keyboard::Keys>(key)); }
bool Input::IsKeyReleased(KeyCode key)const { return _KeyboardTracker.IsKeyReleased(static_cast<Keyboard::Keys>(key)); }
bool Input::IsKeyDown(KeyCode key)const { return this->GetKeyboardState().IsKeyDown(static_cast<Keyboard::Keys>(key)); }
bool Input::IsKeyUp(KeyCode key)const { return this->GetKeyboardState().IsKeyUp(static_cast<Keyboard::Keys>(key)); }

void Input::UpdateInput() {
	Mouse::State currentMouseState = this->GetMouseState();
	Keyboard::State currentKeyboardState = this->GetKeyboardState();

	_MouseTracker.Update(currentMouseState);
	_KeyboardTracker.Update(currentKeyboardState);
}

void Input::ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	_pMouse->ProcessMessage(msg, wParam, lParam);
	_pKeyboard->ProcessMessage(msg, wParam, lParam);
}

bool Input::Init(HWND hWnd) {
	_pMouse = std::make_unique<DirectX::Mouse>();
	_pMouse->SetWindow(hWnd);

	_pKeyboard = std::make_unique<DirectX::Keyboard>();
	return TRUE;
}

MouseTrackerState Input::GetButtonTackerState(MouseButton button)const {
	switch (button)
	{
	case Input::MouseButton::LeftButton:
		return _MouseTracker.leftButton;
	case Input::MouseButton::MiddleButton:
		return _MouseTracker.middleButton;
	case Input::MouseButton::RightButton:
		return _MouseTracker.rightButton;
	case Input::MouseButton::XButton1:
		return _MouseTracker.xButton1;
	case Input::MouseButton::XButton2:
		return _MouseTracker.xButton2;
	}
	return MouseTrackerState::UP;
}
