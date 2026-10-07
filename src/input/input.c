#include "input/input_system.h"
#include "core/engine.h"
#include <stdlib.h>

void Update_Input(Input_System *input, EngineClass *engine)
{
    // KEYBOARD PROCESSING commands and inputs
    for (int i = 0; i < 256; i++)
    {
        input->newKeys[i] = GetAsyncKeyState(i);

        // Resolve status changes between current state and previous tick frame
        input->keys[i].bPressed = 0;
        input->keys[i].bReleased = 0;

        if (input->newKeys[i] != input->oldKeys[i])
        {
            if (input->newKeys[i] & 0x8000)
            {
                input->keys[i].bPressed = !input->keys[i].bHeld;
                input->keys[i].bHeld = 1;
            }
            else
            {
                input->keys[i].bReleased = 1;
                input->keys[i].bHeld = 0;
            }
        }

        input->oldKeys[i] = input->newKeys[i];
    }

    // MOUSE EVENT QUEUE PROCESSING
    DWORD numEvents = 0;
    GetNumberOfConsoleInputEvents(engine->hwnd, &numEvents);

    if (numEvents > 0)
    {
        INPUT_RECORD *eventBuffer = (INPUT_RECORD *)malloc(sizeof(INPUT_RECORD) * numEvents);
        if (eventBuffer != NULL)
        {
            DWORD eventsRead = 0;
            ReadConsoleInputW(engine->hwnd, eventBuffer, numEvents, &eventsRead);

            for (DWORD i = 0; i < eventsRead; i++)
            {
                switch (eventBuffer[i].EventType)
                {
                case MOUSE_EVENT:
                {
                    MOUSE_EVENT_RECORD mer = eventBuffer[i].Event.MouseEvent;

                    // Capture tracking boundaries cleanly inside engine space
                    if (mer.dwEventFlags == MOUSE_MOVED)
                    {
                        input->mouseX = mer.dwMousePosition.X;
                        input->mouseY = mer.dwMousePosition.Y;
                    }
                    else if (mer.dwEventFlags == 0)
                    {
                        input->mouseX = mer.dwMousePosition.X;
                        input->mouseY = mer.dwMousePosition.Y;
                        for (int b = 0; b < 5; b++)
                            input->newMouseButtons[b] = (mer.dwButtonState & (1 << b)) ? 1 : 0;
                    }

                    break;
                }
                case FOCUS_EVENT:
                    engine->input.inFocus = eventBuffer[i].Event.FocusEvent.bSetFocus;
                    break;
                default:
                    break;
                }
            }
            free(eventBuffer);
        }
    }

    // MOUSE STATE UPDATE
    for (int b = 0; b < 5; b++)
    {
        input->mouse[b].bPressed = 0;
        input->mouse[b].bReleased = 0;

        if (input->newMouseButtons[b] != input->oldMouseButtons[b])
        {
            if (input->newMouseButtons[b])
            {
                input->mouse[b].bPressed = !input->mouse[b].bHeld;
                input->mouse[b].bHeld = 1;
            }
            else
            {
                input->mouse[b].bReleased = 1;
                input->mouse[b].bHeld = 0;
            }
        }
        input->oldMouseButtons[b] = input->newMouseButtons[b];
    }
}