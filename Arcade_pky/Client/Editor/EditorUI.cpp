#include "pch.h"

#include "EditorUI.h"

bool EditorUI::Init(const std::string& name)
{
    _name = name;

    SetActive(true);
    SetEnable(true);

    return true;
}
