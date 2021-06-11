#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
    POINT current_mouse_pos = {};
    ::GetCursorPos(&current_mouse_pos);

    if (first_time)
    {
        old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
        first_time = false;
    }

    if (current_mouse_pos.x != old_mouse_pos.x || current_mouse_pos.y != old_mouse_pos.y)
    {
        std::map<InputListener*, InputListener*>::iterator it = listeners.begin();

        while (it != listeners.end())
        {
            it->second->onMouseMove(Point(current_mouse_pos.x - old_mouse_pos.x, current_mouse_pos.y - old_mouse_pos.y));
            ++it;
        }
    }
    old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);

    if (::GetKeyboardState(keys_state))
    {
        for (unsigned int i = 0; i < 256; i++)
        {
            if(keys_state[i] & 0x80) //KEY IS DOWN
            {
                std::map<InputListener*, InputListener*>::iterator it = listeners.begin();

                while (it != listeners.end())
                {
                    if (i == VK_LBUTTON)
                    {
                        if (old_keys_state[i] != keys_state[i]) it->second->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
                    }
                    else if (i == VK_RBUTTON)
                    {
                        if (old_keys_state[i] != keys_state[i]) it->second->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
                    }
                    else it->second->onKeyDown(i);
                    ++it;
                }
            }
            else //KEY IS UP
            { 
                if (old_keys_state[i] != keys_state[i])
                {
                    std::map<InputListener*, InputListener*>::iterator it = listeners.begin();

                    while (it != listeners.end())
                    {
                        if (i == VK_LBUTTON) it->second->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
                        else if (i == VK_RBUTTON) it->second->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
                        else it->second->onKeyUp(i);
                        ++it;
                    }
                }
            }
        }
        ::memcpy(old_keys_state, keys_state, 256*sizeof(unsigned char));
    }
}

void InputSystem::addListener(InputListener* listener)
{
    listeners.insert(std::make_pair<InputListener*, InputListener*>(std::forward< InputListener*>(listener), std::forward< InputListener*>(listener)));
}

void InputSystem::removeListener(InputListener* listener)
{
    std::map<InputListener*, InputListener*>::iterator it = listeners.find(listener);

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

InputSystem* InputSystem::get()
{
    static InputSystem ioSystem;
    return &ioSystem;
}
