#include "pch.h"
#include "Model.h"
#include "ConsoleRendering.h"

// ¡Ü
//¦£¦¢¦¤
//¦£ ¦¤



void Model::Render(const Vector2& _position)
{
	ConsoleRendering::Draw(static_cast<int>(_position.X), static_cast<int>(_position.Y) - 2,		_T('¡Ü'));
	ConsoleRendering::Draw(static_cast<int>(_position.X), static_cast<int>(_position.Y) - 1,		_T('¦¢'));
	ConsoleRendering::Draw(static_cast<int>(_position.X) + 2, static_cast<int>(_position.Y) - 1,	_T('¡¬'));
	ConsoleRendering::Draw(static_cast<int>(_position.X) - 3, static_cast<int>(_position.Y) - 1,	_T('£¯'));
	ConsoleRendering::Draw(static_cast<int>(_position.X) + 2, static_cast<int>(_position.Y),		_T('¡¬'));
	ConsoleRendering::Draw(static_cast<int>(_position.X) - 3, static_cast<int>(_position.Y),		_T('£¯'));

	// Test
	//ConsoleRendering::Draw(static_cast<int>(_position.X), static_cast<int>(_position.Y), "O");
}
